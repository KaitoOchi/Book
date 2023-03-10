#include "BookEnginePreCompile.h"
#include "RenderingEngine.h"


namespace nsBookEngine {

	RenderingEngine* RenderingEngine::m_instance = nullptr;

	RenderingEngine::RenderingEngine()
	{
	}

	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Init()
	{
		//ディレクションライトを設定
		SetDirectionLight(Vector3(-1, -1, 1), Vector3(0.5f, 0.5f, 0.5f));

		//環境光を設定
		SetAmbient(0.1f);

		//半球ライトを設定
		SetHemiSphereLight(
			Vector3(1.0f, 0.5f, 0.2f),
			Vector3(0.3f, 0.5f, 0.1f),
			Vector3(0.0f, 1.0f, 0.0f)
		);

		//構造体に保存
		m_lightCB.directionLig = m_directionLig.GetDirectionLig();
		m_lightCB.hemiSphereLig = m_hemiSphereLig.GetHemiSphereLig();
	}

	void RenderingEngine::Update()
	{

	}
}