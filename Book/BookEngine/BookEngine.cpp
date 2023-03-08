#include "BookEnginePreCompile.h"
#include "BookEngine.h"


namespace nsBookEngine {

	BookEngine* BookEngine::m_instance = nullptr;
	//RenderingEngine* g_renderingEngine = nullptr;
	//SceneLight* g_sceneLight = nullptr;
	//CollisionObjectManager* g_collisionObjectManager = nullptr;
	BookEngine* g_bookEngine = nullptr;

	void BookEngine::Init(const InitData& initData)
	{
		g_bookEngine = this;
		g_engine = &m_k2EngineLow;
		//g_collisionObjectManager = &m_collisionObjectManager;
		//g_renderingEngine = &m_renderingEngine;

		m_k2EngineLow.Init(
			initData.hwnd,
			initData.frameBufferWidth,
			initData.frameBufferHeight
		);
		//m_renderingEngine.Init(initData.isSoftShadow);
		g_camera3D->SetPosition({ 0.0f, 200.0f, -400.0f });
		g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

		m_renderingEngine = new RenderingEngine;
		m_renderingEngine->Init();
		m_collisionObjectManager = new CollisionObjectManager;
	}

	BookEngine::~BookEngine()
	{
		//g_renderingEngine = nullptr;
		//g_collisionObjectManager = nullptr;
		delete m_renderingEngine;
		m_renderingEngine = nullptr;
		delete m_collisionObjectManager;
		m_collisionObjectManager = nullptr;
		g_engine = nullptr;
	}

	void BookEngine::Execute()
	{
		auto& renderContext = g_graphicsEngine->GetRenderContext();

		g_engine->BeginFrame();

		g_engine->ExecuteUpdate();

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