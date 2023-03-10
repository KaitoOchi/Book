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

		//レンダリングエンジンを呼ぶ
		RenderingEngine::CreateInstance();
		//コリジョンオブジェクトマネージャーを呼ぶ
		CollisionObjectManager::CreateInstance();

		GameManager::CreateInstance();
	}

	BookEngine::~BookEngine()
	{
		g_engine = nullptr;

		GameManager::DeleteInstance();

		//レンダリングエンジンを削除
		RenderingEngine::DeleteInstance();
		//コリジョンオブジェクトマネージャーを削除
		CollisionObjectManager::DeleteInstance();
	}

	void BookEngine::Execute()
	{
		auto& renderContext = g_graphicsEngine->GetRenderContext();

		g_engine->BeginFrame();

		g_engine->ExecuteUpdate();

		GameManager::GetInstance()->Update();

		// レンダリングエンジンの更新。
		//m_renderingEngine.Update();

		g_engine->ExecuteRender();

		//レンダリングエンジンを実行。		
		//m_renderingEngine.Execute(renderContext);

		//当たり判定描画。
		g_engine->DebubDrawWorld();

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
}