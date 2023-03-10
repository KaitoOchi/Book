#include "BookEnginePreCompile.h"
#include "BookEngine.h"

#include "../Game/GameManager.h"


namespace nsBookEngine {

	BookEngine* BookEngine::m_instance = nullptr;
	//SceneLight* g_sceneLight = nullptr;
	BookEngine* g_bookEngine = nullptr;

	void BookEngine::Init(const InitData& initData)
	{
		g_bookEngine = this;
		g_engine = &m_k2EngineLow;

		m_k2EngineLow.Init(
			initData.hwnd,
			initData.frameBufferWidth,
			initData.frameBufferHeight
		);
		g_camera3D->SetPosition({ 0.0f, 200.0f, -400.0f });
		g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
		g_camera3D->Update();

		//�����_�����O�G���W�����Ă�
		RenderingEngine::CreateInstance();
		//�R���W�����I�u�W�F�N�g�}�l�[�W���[���Ă�
		CollisionObjectManager::CreateInstance();

		GameManager::CreateInstance();
	}

	BookEngine::~BookEngine()
	{
		g_engine = nullptr;

		GameManager::DeleteInstance();

		//�����_�����O�G���W�����폜
		RenderingEngine::DeleteInstance();
		//�R���W�����I�u�W�F�N�g�}�l�[�W���[���폜
		CollisionObjectManager::DeleteInstance();
	}

	void BookEngine::Execute()
	{
		auto& renderContext = g_graphicsEngine->GetRenderContext();

		g_engine->BeginFrame();

		g_engine->ExecuteUpdate();

		GameManager::GetInstance()->Update();

		// �����_�����O�G���W���̍X�V�B
		//m_renderingEngine.Update();

		g_engine->ExecuteRender();

		//�����_�����O�G���W�������s�B		
		//m_renderingEngine.Execute(renderContext);

		//�����蔻��`��B
		g_engine->DebubDrawWorld();

		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
}