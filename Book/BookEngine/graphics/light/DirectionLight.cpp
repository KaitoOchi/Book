#include "BookEnginePreCompile.h"
#include "DirectionLight.h"

namespace nsBookEngine {

	DirectionLight::DirectionLight()
	{
		//���_�̈ʒu��ݒ肷��
		m_directionLig.eyePos = g_camera3D->GetPosition();
	}

	DirectionLight::~DirectionLight()
	{

	}
}