#include "BookEnginePreCompile.h"
#include "PointLight.h"


namespace nsBookEngine {

	PointLight::PointLight()
	{

	}

	PointLight::~PointLight()
	{

	}

	void PointLight::Update()
	{
		RenderingEngine::GetInstance()->SetPointLight(m_ptNum, m_pointLig);
	}
}