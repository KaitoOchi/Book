#include "BookEnginePreCompile.h"
#include "DirectionLight.h"

namespace nsBookEngine {

	DirectionLight::DirectionLight()
	{
		//視点の位置を設定する
		m_directionLig.eyePos = g_camera3D->GetPosition();
	}

	DirectionLight::~DirectionLight()
	{

	}
}