#include "BookEnginePreCompile.h"
#include "SpotLight.h"

namespace nsBookEngine{

	SpotLight::SpotLight()
	{

	}

	SpotLight::~SpotLight()
	{

	}

	void SpotLight::Update()
	{
		RenderingEngine::GetInstance()->SetSpotLight(m_spotLig);
	}
}