#include "BookEnginePreCompile.h"
#include "RenderingEngine.h"


namespace nsBookEngine {

	RenderingEngine::RenderingEngine()
	{
	}

	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Init()
	{
		//ディレクションライトを設定
		SetDirectionLight(Vector3(-1, -1, 1), Vector3(1.0f, 1.0f, 1.0f));
		//環境光を設定
		SetAmbient(0.3f);

		m_lightCB.directionLig = m_directionLig.GetDirectionLig();
	}

	void RenderingEngine::Update()
	{

	}
}