#pragma once


namespace nsBookEngine {

	class BookEngine
	{
	private:
		BookEngine() {};
		~BookEngine();

	public:
		struct InitData {
			HWND hwnd;					//ウィンドウハンドル。
			UINT frameBufferWidth;		//フレームバッファの幅。
			UINT frameBufferHeight;		//フレームバッファの高さ。
			bool isSoftShadow;			//ソフトシャドウを行うかどうか。
		};

		/// <summary>
		/// インスタンスの作成。
		/// </summary>
		/// <param name="initData"></param>
		static void CreateInstance(const InitData& initData)
		{
			m_instance = new bookEngine;
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
		/// インスタンスの取得。
		/// </summary>
		/// <returns></returns>
		static bookEngine* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// k2Engineを初期化。
		/// </summary>
		void Init(const InitData& initData);

		/// <summary>
		/// エンジンの処理を実行。
		/// </summary>
		void Execute();

	private:
		K2EngineLow m_k2EngineLow;
		CollisionObjectManager m_collisionObjectManager;
		RenderingEngine m_renderingEngine;		//レンダリングエンジン

		static bookEngine* m_instance;			//インスタンス
	};

	//グローバルアクセスポイント
	extern BookEngine* g_bookEngine;
	extern RenderingEngine* g_renderingEngine;
	extern CollisionObjectManager* g_collisionObjectManager;
}

