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
		SetDirectionLight(Vector3(-1, -1, 1), Vector3(1.0f, 1.0f, 1.0f));
		//������ݒ�
		SetAmbient(0.3f);

		m_lightCB.directionLig = m_directionLig.GetDirectionLig();
	}

	void RenderingEngine::Update()
	{

	}
}