
#include "CParticleRenderer_Animated.h"

#include "renderer/material/RrMaterial.h"
#include "renderer/camera/CCamera.h"

CParticleRenderer_Animated::CParticleRenderer_Animated( CParticleEmitter* pInEmitter )
	: CParticleRenderer( pInEmitter )
{
	iFrameCount		= -1;
	iHorizontalDivs = 4;
	iVerticalDivs	= 4;

	fFramesPerSecond	= 10.0f;

	bStretchAnimationToLifetime = false;
	bClampToFrameCount			= false;
}

CParticleRenderer_Animated::~CParticleRenderer_Animated ( void )
{
	;
}

void CParticleRenderer_Animated::CreateMesh ( uint32_t& out_vertCount, uint32_t& out_triCount )
{
	if ( ((int)iHorizontalDivs) == 0 )
	{
		out_vertCount = 0;
		out_triCount = 0;
		return;
	}
	else if ( ((int)iVerticalDivs) == 0 )
	{
		out_vertCount = 0;
		out_triCount = 0;
		return;
	}

	// Draw properly aligned quads
	if ( iRenderMethod == P_FLAT_BILLBOARD_PRECISE )
	{
		// Set needed calculation values
		Vector3d temp;
		CTransform& camtrans = CCamera::activeCamera->transform;
		Quaternion cameraRot = camtrans.rotation.getQuaternion();

		Vector3d vRight (0,1,0), vUp(0,0,1);
		Vector3d corners[5];

		Vector3d vrRight, vrUp;
		Vector3d vCamForward;
		vCamForward = camtrans.Forward();

		Matrix4x4 cameraMatxRot;
		cameraMatxRot.setRotation( cameraRot );
		vRight = cameraMatxRot*vRight;	// Get general up and right vector in camera space
		vUp = cameraMatxRot*vUp;

		Quaternion particleRot;
		Matrix4x4 particleRotMatx;

		Vector2d frameOffset;
		Vector2d frameSize ( 1.0f/iHorizontalDivs, 1.0f/iVerticalDivs );
		int maxFrameCount = (int)iFrameCount;
		if ( maxFrameCount <= 0 ) {
			maxFrameCount = (int)(iHorizontalDivs*iVerticalDivs);
		}

		uint32_t i = 0;
		uint32_t t = 0;
		int frame;
		// Loop through all the particles and add to the list
		for ( uint16_t pi = 0; pi < myEmitter->m_max_particle_index; ++pi )
		{
			const CParticle& particle = myEmitter->m_particles[pi];
			if ( !particle.alive ) continue;
		
			// Get corner positions
			particleRot.AxisAngle( vCamForward, particle.fAngle );
			vrRight = (particleRot*vRight) * particle.fSize;
			vrUp = (particleRot*vUp) * particle.fSize;
			corners[3] = (-vrRight+vrUp);
			corners[2] = (-vrRight-vrUp);
			corners[1] = (vrRight-vrUp);
			corners[0] = (vrRight+vrUp);
			corners[4] = -vCamForward;

			// Set the colors and normals
			temp = cameraRot*(-corners[4]);
			for ( int j = 0; j < 4; ++j )
			{
				pVertices[i+j].r = particle.cColor.red;
				pVertices[i+j].g = particle.cColor.green;
				pVertices[i+j].b = particle.cColor.blue;
				pVertices[i+j].a = particle.cColor.alpha;
				pVertices[i+j].nx = temp.x;
				pVertices[i+j].ny = temp.y;
				pVertices[i+j].nz = temp.z;
			}

			// Calculate the frame
			if ( !bStretchAnimationToLifetime )
			{
				frame = (int)((particle.fStartLife-particle.fLife)*fFramesPerSecond);
			}
			else
			{
				frame = (int)(maxFrameCount*(1-(particle.fLife/particle.fStartLife)));
			}
			if ( bClampToFrameCount )
			{
				if ( frame >= maxFrameCount )
				{
					frame = maxFrameCount-1;
				}
			}
			// Calculate the frame offset
			frameOffset.x = (int(frame) % int(iHorizontalDivs)) / (ftype)(iHorizontalDivs);
			frameOffset.y = (int(frame) / int(iVerticalDivs)) / (ftype)(iVerticalDivs);

			// Set the positions and texture coordinates
			temp = particle.vPosition+corners[0];
			pVertices[i+0].x = temp.x;
			pVertices[i+0].y = temp.y;
			pVertices[i+0].z = temp.z;
			pVertices[i+0].u = frameOffset.x;
			pVertices[i+0].v = frameOffset.y;

			temp = particle.vPosition+corners[1];
			pVertices[i+1].x = temp.x;
			pVertices[i+1].y = temp.y;
			pVertices[i+1].z = temp.z;
			pVertices[i+1].u = frameOffset.x;
			pVertices[i+1].v = frameOffset.y+frameSize.y;

			temp = particle.vPosition+corners[2];
			pVertices[i+2].x = temp.x;
			pVertices[i+2].y = temp.y;
			pVertices[i+2].z = temp.z;
			pVertices[i+2].u = frameOffset.x+frameSize.x;
			pVertices[i+2].v = frameOffset.y+frameSize.y;

			temp = particle.vPosition+corners[3];
			pVertices[i+3].x = temp.x;
			pVertices[i+3].y = temp.y;
			pVertices[i+3].z = temp.z;
			pVertices[i+3].u = frameOffset.x+frameSize.x;
			pVertices[i+3].v = frameOffset.y;

			// Set the triangles
			pTriangles[t+0].vert[0] = i+0;
			pTriangles[t+0].vert[1] = i+1;
			pTriangles[t+0].vert[2] = i+2;
			pTriangles[t+1].vert[0] = i+0;
			pTriangles[t+1].vert[1] = i+2;
			pTriangles[t+1].vert[2] = i+3;

			// Increment step
			i += 4;
			t += 2;
		}

		// Set output count
		out_vertCount = i;
		out_triCount = t;
	}
	else if ( iRenderMethod == P_STRETCHED_BILLBOARD )
	{
		// Set needed calculation values
		Vector3d temp;
		CTransform& camtrans = CCamera::activeCamera->transform;
		Quaternion cameraRot = camtrans.rotation.getQuaternion();

		Vector3d vParticlePosition;
		Vector3d vRight (0,1,0), vUp(0,0,1);
		Vector3d corners[5];

		Vector3d vrRight, vrUp;
		Vector3d vCamForward;
		vCamForward = camtrans.Forward();

		Matrix4x4 cameraMatxRot;
		cameraMatxRot.setRotation( cameraRot );

		Quaternion particleRot;
		Matrix4x4 particleRotMatx;

		Vector2d frameOffset;
		Vector2d frameSize ( 1.0f/iHorizontalDivs, 1.0f/iVerticalDivs );
		int maxFrameCount = (int)iFrameCount;
		if ( maxFrameCount <= 0 ) {
			maxFrameCount = (int)(iHorizontalDivs*iVerticalDivs);
		}

		uint32_t i = 0;
		uint32_t t = 0;
		int frame;
		// Loop through all the particles and add to the list
		for ( uint16_t pi = 0; pi < myEmitter->m_max_particle_index; ++pi )
		{
			const CParticle& particle = myEmitter->m_particles[pi];
			if ( !particle.alive ) continue;

			// Get corner positions// Generate true position
			vParticlePosition = (myEmitter->bSimulateInWorldspace) ? particle.vPosition :( particle.vPosition + myEmitter->transform.position );

			vrRight = particle.vVelocity * fR_SpeedScale;
			if ( vrRight.sqrMagnitude() < sqr(particle.fSize*0.5f) )
			{
				vrRight = vrRight.normal() * particle.fSize*0.5f;
			}
			vrUp = vCamForward.cross( vrRight.normal() );
			if ( vrUp.sqrMagnitude() < 0.01f )
			{	// If it's a degenerate normal, then we make up a new angle based on the particle's angle
				particleRot.AxisAngle( vCamForward, particle.fAngle );
				vrUp = (particleRot*vRight);
			}
			vrUp *= particle.fSize;

			corners[3] = (-vrRight+vrUp);
			corners[2] = (-vrRight-vrUp);
			corners[1] = (vrRight-vrUp);
			corners[0] = (vrRight+vrUp);
			corners[4] = -vCamForward;

			// Set the colors and normals
			temp = cameraRot*(-corners[4]);
			for ( int j = 0; j < 4; ++j ) {
				pVertices[i+j].r = particle.cColor.red;
				pVertices[i+j].g = particle.cColor.green;
				pVertices[i+j].b = particle.cColor.blue;
				pVertices[i+j].a = particle.cColor.alpha;
				pVertices[i+j].nx = temp.x;
				pVertices[i+j].ny = temp.y;
				pVertices[i+j].nz = temp.z;
			}

			// Calculate the frame
			if ( !bStretchAnimationToLifetime ) {
				frame = (int)((particle.fStartLife-particle.fLife)*fFramesPerSecond);
			}
			else {
				frame = (int)(maxFrameCount*(1-(particle.fLife/particle.fStartLife)));
			}
			if ( bClampToFrameCount ) {
				if ( frame >= maxFrameCount ) {
					frame = maxFrameCount-1;
				}
			}
			// Calculate the frame offset
			frameOffset.x = (int(frame) % int(iHorizontalDivs)) / (ftype)(iHorizontalDivs);
			frameOffset.y = (int(frame) / int(iVerticalDivs)) / (ftype)(iVerticalDivs);

			// Set the positions and texture coordinates
			temp = vParticlePosition+corners[0];
			pVertices[i+0].x = temp.x;
			pVertices[i+0].y = temp.y;
			pVertices[i+0].z = temp.z;
			pVertices[i+0].u = frameOffset.x;
			pVertices[i+0].v = frameOffset.y;

			temp = vParticlePosition+corners[1];
			pVertices[i+1].x = temp.x;
			pVertices[i+1].y = temp.y;
			pVertices[i+1].z = temp.z;
			pVertices[i+1].u = frameOffset.x;
			pVertices[i+1].v = frameOffset.y + frameSize.y;

			temp = vParticlePosition+corners[2];
			pVertices[i+2].x = temp.x;
			pVertices[i+2].y = temp.y;
			pVertices[i+2].z = temp.z;
			pVertices[i+2].u = frameOffset.x + frameSize.x;
			pVertices[i+2].v = frameOffset.y + frameSize.y;

			temp = vParticlePosition+corners[3];
			pVertices[i+3].x = temp.x;
			pVertices[i+3].y = temp.y;
			pVertices[i+3].z = temp.z;
			pVertices[i+3].u = frameOffset.x + frameSize.x;
			pVertices[i+3].v = frameOffset.y;

			// Set the triangles
			pTriangles[t+0].vert[0] = i+0;
			pTriangles[t+0].vert[1] = i+1;
			pTriangles[t+0].vert[2] = i+2;
			pTriangles[t+1].vert[0] = i+0;
			pTriangles[t+1].vert[1] = i+2;
			pTriangles[t+1].vert[2] = i+3;

			pTriangles[t+2].vert[0] = i+0;
			pTriangles[t+2].vert[1] = i+2;
			pTriangles[t+2].vert[2] = i+1;
			pTriangles[t+3].vert[0] = i+0;
			pTriangles[t+3].vert[1] = i+3;
			pTriangles[t+3].vert[2] = i+2;

			// Increment step
			i += 4;
			t += 4;
		}

		// Set output count
		out_vertCount = i;
		out_triCount = t;
	}
	else {
		out_vertCount = 0;
		out_triCount = 0;
	}
}

#include "core-ext/system/io/serializer/CBaseSerializer.h"
void	CParticleRenderer_Animated::serialize ( Serializer & ser, const uint ver )
{
	CParticleRenderer::serialize( ser, ver );
	ser & iFrameCount;
	ser & iHorizontalDivs;
	ser & iVerticalDivs;
	ser & fFramesPerSecond;
	ser & bStretchAnimationToLifetime;
	ser & bClampToFrameCount;
}
