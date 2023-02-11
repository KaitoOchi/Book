#include "PreCompile.h"
#include "BookEngine.h"


namespace nsBookEngine {

	BookEngine* BookEngine::m_instance = nullptr;
	RenderingEngine* g_renderingEngine = nullptr;
	//CollisionObjectManager* g_collisionObjectManager = nullptr;
	BookEngine* g_bookEngine = nullptr;

	void BookEngine::Init(const InitData& initData)
	{
		g_bookEngine = this;
		g_engine = &m_k2EngineLow;
		//g_collisionObjectManager = &m_collisionObjectManager;
		g_renderingEngine = &m_renderingEngine;

		m_k2EngineLow.Init(
			initData.hwnd,
			initData.frameBufferWidth,
			initData.frameBufferHeight
		);

		m_renderingEngine.Init(initData.isSoftShadow);
		g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
		g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	}

	BookEngine::~BookEngine()
	{
		g_renderingEngine = nullptr;
		//g_collisionObjectManager = nullptr;
		g_engine = nullptr;
	}

	void BookEngine::Execute()
	{
		auto& renderContext = g_graphicsEngine->GetRenderContext();

		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_engine->BeginFrame();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
		g_engine->ExecuteUpdate();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
		g_engine->ExecuteRender();

		m_renderingEngine.Execute(renderContext);

		// �f�o�b�O�`�揈�������s����B
		g_engine->DebubDrawWorld();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_engine->EndFrame();
	}
}