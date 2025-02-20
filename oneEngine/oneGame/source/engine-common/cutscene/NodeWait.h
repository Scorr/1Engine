#ifndef ENGINE_CUTSCENE_NODE_WAIT_H_
#define ENGINE_CUTSCENE_NODE_WAIT_H_

#include "engine-common/cutscene/Node.h"

namespace common {
namespace cts
{
	//	Node - basis for actions in the cutscene system
	class NodeWait : public Node
	{
	public:
		explicit		NodeWait ( void )
			: Node() {}
		virtual			~NodeWait ( void )
			{}

		ENodeType		GetNodeType ( void ) override
			{ return kNodeTypeWait; }
		int				GetOutputNodeCount ( void ) override
			{ return 1; }
		Node*			GetOutputNode ( const int index ) override
			{ return m_outputNode; }

	public:
		//	IOSetOutputNode( IGNORED, node ) : Sets the output node in the output node list at index.
		void			IOSetOutputNode ( const int index, Node* node ) override
			{ m_outputNode = node; }

	protected:
		Node*		m_outputNode;
	};

}}

#endif//ENGINE_CUTSCENE_NODE_WAIT_H_