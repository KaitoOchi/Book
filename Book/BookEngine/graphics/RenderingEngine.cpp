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
		//�f�B���N�V�������C�g��ݒ�
		SetDirectionLight(Vector3(-1, -1, 1), Vector3(0.5f, 0.5f, 0.5f));

		//������ݒ�
		SetAmbient(0.1f);

		//�������C�g��ݒ�
		SetHemiSphereLight(
			Vector3(0.7f, 0.5f, 0.3f),
			Vector3(0.15f, 0.7f, 0.85f),
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