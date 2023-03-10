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
		//�f�B���N�V�������C�g��ݒ�
		SetDirectionLight(Vector3(-1, -1, 1), Vector3(0.5f, 0.5f, 0.5f));

		//������ݒ�
		SetAmbient(0.1f);

		//�������C�g��ݒ�
		SetHemiSphereLight(
			Vector3(1.0f, 0.5f, 0.2f),
			Vector3(0.3f, 0.5f, 0.1f),
			Vector3(0.0f, 1.0f, 0.0f)
		);

		//�\���̂ɕۑ�
		m_lightCB.directionLig = m_directionLig.GetDirectionLig();
		m_lightCB.hemiSphereLig = m_hemiSphereLig.GetHemiSphereLig();
	}

	void RenderingEngine::Update()
	{

	}
}