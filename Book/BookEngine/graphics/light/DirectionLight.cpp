#include "BookEnginePreCompile.h"
#include "DirectionLight.h"

namespace nsBookEngine {

	DirectionLight::DirectionLight()
	{
		//Ž‹“_‚ÌˆÊ’u‚ðÝ’è‚·‚é
		m_directionLig.eyePos = g_camera3D->GetPosition();
	}

	DirectionLight::~DirectionLight()
	{

	}
}