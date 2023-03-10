#pragma once


namespace nsBookEngine {

	class BookEngine {
	private:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		BookEngine(){};

		/// <summary>
		/// デストラクタ。
		/// </summary>
		~BookEngine();

	public:
		/// <summary>
		/// 初期化データ。
		/// </summary>
		struct InitData {
			HWND hwnd;					// ウィンドウハンドル。
			UINT frameBufferWidth;		// フレームバッファの幅。
			UINT frameBufferHeight;		// フレームバッファの高さ。
			bool isSoftShadow;			// ソフトシャドウを行う？
		};
		/// <summary>
		/// インスタンスの作成。
		/// </summary>
		static void CreateInstance(const InitData& initData)
		{
			m_instance = new BookEngine;
			m_instance->Init(initData);
		}
		/// <summary>
		/// インスタンスの破棄。
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
		}
		/// <summary>
		/// インスタンスを取得。
		/// </summary>
		/// <returns></returns>
		static BookEngine* GetInstance()
		{
			return m_instance;
		}
		/// <summary>
		/// BookEngineを初期化
		/// </summary>
		void Init(const InitData& initData);
		/// <summary>
		/// エンジンの処理を実行。
		/// </summary>
		void Execute();

		CollisionObjectManager* GetCollisionObjectManager()
		{
			return m_collisionObjectManager;
		}

		RenderingEngine* GetRenderingEngine()
		{
			return m_renderingEngine;
		}

	private:
		K2EngineLow m_k2EngineLow;
		CollisionObjectManager* m_collisionObjectManager = nullptr;
		RenderingEngine* m_renderingEngine = nullptr;

		static BookEngine* m_instance;
	};

	// グローバルなアクセスポイント。
	extern BookEngine* g_bookEngine;
	//extern RenderingEngine* g_renderingEngine;
	//extern SceneLight* g_sceneLight;
	//extern CollisionObjectManager* g_collisionObjectManager;
}