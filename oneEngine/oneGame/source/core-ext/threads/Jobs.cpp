#include "Jobs.h"
#include "core-ext/threads/naming.h"
#include <string>

// Global instance
core::jobs::System* core::jobs::System::Active = NULL;

//	Constructor
// Initializes the job handler thread.
core::jobs::System::System ( const uint8_t threadCount )
	: m_systemEnabled( false ), m_nextJobId( 0 )
{
	unsigned usedThreads = threadCount;
	if ( usedThreads == 0 ) usedThreads = std::thread::hardware_concurrency();
	if ( usedThreads == 0 ) usedThreads = 4;

	// Clear lock states
	m_jobRequestLock.clear();
	m_jobTaskLock.clear();
	// Clear priority flag
	m_highPriorityFlag = 0x00;

	// Enable the system
	if (Active == NULL) Active = this;
	m_systemEnabled = true;

	// But prevent it from going anywhere just yet
	m_systemMainLock.lock();
	// Start the worker thread
	m_managerThread = std::thread( &core::jobs::System::_internal_JobCycle, this );

	// Create the threads
	for ( uint i = 0; i < usedThreads; ++i )
	{
		m_jobStates.push_back( JobState() );
	}
	for ( uint i = 0; i < usedThreads; ++i )
	{
		m_jobThreads.push_back( std::thread( &core::jobs::System::_internal_WorkerCycle, this, &(m_jobStates[i]) ) );
		core::threads::SetThreadName(m_jobThreads.back(), ("JobThread_" + std::to_string(i)).c_str());
	}
	
	// Now allow the system to move forward
	m_systemMainLock.unlock();

}
//	Destructor
// Waits for all jobs to finish and then ends the job handler thread.
core::jobs::System::~System ( void )
{
	if ( this == Active ) Active = NULL;
	// Disable adding new jobs
	m_systemEnabled = false;

	// Clear out all job requests
	while ( m_jobRequestLock.test_and_set() ) {;} // Wait for read lock on the list
	m_jobRequests.clear();
	m_jobRequestLock.clear(); // Clear read lock on the list

	// Notify there's been a job change
	for ( uint i = 0; i < m_jobThreads.size(); ++i ) {
		m_jobsignal.notify();
	}

	// Clear out the tasks
	while ( m_jobTaskLock.test_and_set() ) {;} // Wait for read lock on the list
	m_jobTasks.clear();
	m_jobTaskLock.clear(); // Clear read lock on the list

	// Finish up all jobs
	_internal_WaitForJobs( kJobTypeALL );
	// Join the worker thread to stop any dangling execution
	m_managerThread.join();
	for ( uint i = 0; i < m_jobThreads.size(); ++i ) {
		m_jobStates[i].function = []()->void{;};
		m_jobStates[i].signal.notify();
		m_jobThreads[i].join();
	}
}

//	_internal_AddJob
// Adds a job to the internal list, and may immediately start execution.
core::jobs::JobId core::jobs::System::_internal_AddJob ( const JobRequest& jobToAdd )
{
	JobRequest jobWithId = jobToAdd;
	jobWithId.id.value = ++m_nextJobId;

	if ( m_systemEnabled )
	{
		while ( m_jobRequestLock.test_and_set() ) {;} // Wait for write lock on the list
		m_jobRequests.push_back( jobWithId );
		m_jobRequestLock.clear(); // Clear write lock on the list
		// Notify a job has been added
		m_jobsignal.notify();
	}

	// Ensure the next assigned index will not be invalid.
	if (m_nextJobId == JobId::kInvalidIndex)
	{
		++m_nextJobId;
	}

	return jobWithId.id;
}
//	_internal_WaitForJobs
// Waits for the given job type
void core::jobs::System::_internal_WaitForJobs ( const JobTypeBits jobTypeBits )
{
	// Ignore invalid calls
	if ( ((jobTypeBits & 0xFF00) == 0) && (jobTypeBits & kJobTypeALL) == 0 )
	{
		return;
	}
	// Set priority flag
	m_highPriorityFlag = jobTypeBits;

	bool hasJob, hasTask;
	CHECK_JOBS:
	{
		hasJob = false;
		// Check the job requests
		while ( m_jobRequestLock.test_and_set() ) {;} // Wait for read lock on the list
		for ( auto job = m_jobRequests.begin(); job != m_jobRequests.end(); ++job )
		{
			if ( job->type & jobTypeBits ) { // Check for jobtype match
				hasJob = true;
				break;
			}
		}
		m_jobRequestLock.clear(); // Clear read lock on the list
		if ( hasJob ) {
			// Wait for the jobs to finish
			std::this_thread::yield();
			goto CHECK_JOBS; // Check for jobs again
		}

		hasTask = false;
		// Check the task list
		while ( m_jobTaskLock.test_and_set() ) {;} // Wait for read lock on the list
		for ( auto job = m_jobTasks.begin(); job != m_jobTasks.end(); ++job )
		{
			if ( job->type & jobTypeBits ) { // Check for jobtype match
				hasTask = true;
				break;
			}
		}
		m_jobTaskLock.clear(); // Clear read lock on the list
		if ( hasTask ) {
			// Wait for the jobs to finish
			std::this_thread::yield();
			goto CHECK_JOBS; // Check for jobs again
		}

		// Got to the bottom. Therefore no job left.
	}
	// Reset priority
	m_highPriorityFlag = 0x00;
}

//	_internal_WaitForJob
// Waits for the given job id
void core::jobs::System::_internal_WaitForJob ( const JobId& id )
{
	// Skip invalid job indicies
	if (id.value == JobId::kInvalidIndex)
	{
		return;
	}

	bool hasJob;
	CHECK_JOBS:
	{
		hasJob = false;
		// Find the task request with the matching index:
		while ( m_jobRequestLock.test_and_set() ) {;} // Wait for read lock on the list
		for ( auto job = m_jobRequests.begin(); job != m_jobRequests.end(); ++job )
		{
			if ( job->id.value == id.value ) { // Check for id match
				hasJob = true;
				break;
			}
		}
		m_jobRequestLock.clear(); // Clear read lock on the list

		if ( hasJob ) {
			// Wait for the jobs to finish
			std::this_thread::yield();
			goto CHECK_JOBS; // Check for jobs again
		}
	}

	// Find the job state index with the matching index:
	for ( uint i = 0; i < m_jobStates.size(); ++i )
	{
		if ( m_jobStates[i].id.value == id.value )
		{
			// We wait until perform is false or a new job is slotted into it
			while ( m_jobStates[i].perform
				&& m_jobStates[i].id.value == id.value )
			{
				std::this_thread::yield();
			}
			break;
		}
	}
}

//	_internal_JobCycle
// Job distribution loop.
// Continues while m_systemEnabled is true and the current job list is not empty.
void core::jobs::System::_internal_JobCycle ( void )
{
	// Lock the main system to here
	m_systemMainLock.lock();
	// Spin-lock while system running
	while ( m_systemEnabled )
	{
		// Check through job tasks for a job that has finished
		while ( m_jobTaskLock.test_and_set() ) {;}
		for ( auto task = m_jobTasks.begin(); task != m_jobTasks.end(); )
		{
			if ( m_jobStates[task->worker_index].perform == false )
			{	// Job is done, remove it from the list
				task = m_jobTasks.erase( task );
			}
			else
			{	// Go to next task to check
				++task;
			}
		}
		m_jobTaskLock.clear();

		// Check if there's requests
		while ( m_jobRequestLock.test_and_set() ) {;}
		bool hasRequest = !m_jobRequests.empty();
		m_jobRequestLock.clear();
		// It seems we have a request
		if ( hasRequest )
		{
			// Look for an open thread
			for ( uint i = 0; i < m_jobStates.size(); ++i )
			{
				// Job isn't performing, meaning thread is not busy
				if ( m_jobStates[i].perform == false )
				{
					while ( m_jobRequestLock.test_and_set() ) {;}
					auto requestToUse = m_jobRequests.begin(); // Default to using the first job
					// In high priority mode?
					if ( m_highPriorityFlag != 0x00 )
					{
						// Loop through the requests, and find first bitmask match
						for ( auto request = m_jobRequests.begin(); request != m_jobRequests.end(); ++request )
						{
							if ( request->type & m_highPriorityFlag ) {
								requestToUse = request;
								break;
							}
						}
						// Otherwise, no match was made, and we are using the first job.
					}
					
					// Create the task
					JobTask task;
					task.type = requestToUse->type;
					task.worker_index = i;

					// Copy over job info
					m_jobStates[i].perform = true;
					m_jobStates[i].function = requestToUse->function;
					m_jobStates[i].id = requestToUse->id;

					// 1: Add the task to the tasklist
					while ( m_jobTaskLock.test_and_set() ) {;}
					m_jobTasks.push_back( task );
					m_jobTaskLock.clear();

					// 2: Erase request from the request list
					m_jobRequests.erase( requestToUse );
					bool haveRequests = !m_jobRequests.empty();
					m_jobRequestLock.clear();

					// Notify the thread to start moving
					m_jobStates[i].signal.notify();

					// If we have no more requests, break out of the check loop
					if ( !haveRequests ) {
						break;
					}
				}
			}
		}

		// Give focus to other threads
		std::this_thread::yield();

#	ifdef _USING_JOB_SIGNALING_SYSTEM_
		// Sleep until a job has been requested
		m_jobsignal.wait();
#	endif
	}
	// Main system is done here
	m_systemMainLock.unlock();
}

//	_internal_WorkerCycle
// Job execution loop.
// Is assigned a job by _internal_JobCycle, and then executes.
// When not working, state.perform is set to false.
void core::jobs::System::_internal_WorkerCycle ( core::jobs::JobState* state )
{
	// Reset state to start with
	state->perform = false;

	// Loop the system
	while ( m_systemEnabled )
	{
		// Sleep until the signal is unlocked
		state->signal.wait();
		// Perform the job
		state->function();
		// Clear job id
		state->id.value = JobId::kInvalidIndex;
		// Set job as done
		state->perform = false;
		// Notify that a job has been finished
		m_jobsignal.notify();
		// Give focus to other threads
		std::this_thread::yield();
	}
}