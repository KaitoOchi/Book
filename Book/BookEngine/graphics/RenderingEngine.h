#pragma once


#include "MyRenderer.h"
#include "../geometry/SceneGeometryData.h"

namespace nsBookEngine {

	class RenderingEngine
	{
	public:
		RenderingEngine();
		~RenderingEngine();

		//メインレンダリングターゲットのスナップショット
		enum class EnMainRTSnapshot
		{
			enDrawnOpacity,     // 不透明オブジェクトの描画完了時点
			enNum,              // スナップショットの数
		};

		// レンダリングパス
		enum class EnRenderingPass
		{
			enComputeAnimatedVertex,    // アニメーション済み頂点計算。
			enRenderToShadowMap,        // シャドウマップへの描画パス
			enZPrepass,                 // ZPrepass
			enRenderToVolumeLightMap,   // ボリュームライトマップへの描画
			enLightCulling,             // ライトカリング。
			enRenderToGBuffer,          // G-Bufferへの描画パス
			enForwardRender,            // フォワードレンダリングの描画パス
			enPostEffect,               // ポストエフェクト
			enRender2D,                 // 2D描画。
			enRender3D,                 // 3Dモデル最前面
		};

		/// <summary>
		/// イベント。
		/// </summary>
		enum EnEvent {
			enEventReInitIBLTexture,    // IBLテクスチャが再初期化された。
		};

		/// <summary>
		/// レンダリングパイプラインを初期化。
		/// </summary>
		/// <param name="isSoftShadow">ソフトシャドウにするかどうか。</param>
		void Init(bool isSoftShadow);

		/// <summary>
		/// レンダリングパイプラインを実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// 描画オブジェクトを追加。
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// ビューカリングのためのビュープロジェクション行列を取得。
		/// </summary>
		/// <returns></returns>
		const Matrix& GetViewProjectionMatrixForViewCulling()
		{
			return m_viewProjMatrixForViewCulling;
		}


		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();

		/// <summary>
		/// イベントリスナーを追加。
		/// </summary>
		/// <param name="pListenerObj"></param>
		/// <param name="listenerFunc"></param>
		void AddEventListener(
			void* pListenerObj,
			std::function<void(EnEvent enEvent)> listenerFunc
		)
		{
			m_eventListeners.push_back({ pListenerObj, listenerFunc });
		}

		void RemoveEventListener(void* pListenerObj)
		{
			auto it = std::find_if(
				m_eventListeners.begin(),
				m_eventListeners.end(),
				[&](const SEventListenerData& listenerData) {return listenerData.pListenerObj == pListenerObj; }
			);
			if (it != m_eventListeners.end()) {
				m_eventListeners.erase(it);
			}
		}

	private:
		/// <summary>
		/// G-Bufferを初期化
		/// </summary>
		void InitGBuffer();
		/// <summary>
		/// アニメーション済み頂点の計算。
		/// </summary>
		/// <remark>
		/// </remark>
		/// <param name="rc">レンダリングコンテキスト</param>
		void ComputeAnimatedVertex(RenderContext& rc);
		/// <summary>
		/// G-Bufferへの描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void RenderToGBuffer(RenderContext& rc);
		/// <summary>
		/// 2D描画。
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);
		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピーする
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		/// <summary>
		/// フォワードレンダリング
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void ForwardRendering(RenderContext& rc);
		/// <summary>
		/// メインレンダリングターゲットを初期化
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// メインレンダリングターゲットののスナップショットを取るための
		/// レンダリングターゲットを初期化
		/// </summary>
		void InitMainRTSnapshotRenderTarget();
		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容を
		/// フレームバッファにコピーするためのスプライトを初期化する
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// メインレンダリングターゲットのスナップショットを撮影。
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="enSnapshot"></param>
		void SnapshotMainRenderTarget(RenderContext& rc, EnMainRTSnapshot enSnapshot);
		/// <summary>
		/// ビューカリング用のビュープロジェクション行列を計算。
		/// </summary>
		void CalcViewProjectionMatrixForViewCulling();
		/// <summary>
		/// 2D描画用の連打林ターゲットを初期化。
		/// </summary>
		void Init2DRenderTarget();

	private:
		// GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedoDepth,           // アルベドと深度値。αに深度値が記憶されています。
			enGBufferNormal,                // 法線
			enGBufferMetaricShadowSmooth,   // メタリック、影パラメータ、スムース。
			// メタリックがr、影パラメータがg、スムースがa。gは未使用。
			enGBufferNum,                   // G-Bufferの数
		};

		RenderTarget m_mainRenderTarget;								//メインレンダリングターゲット
		Sprite m_copyMainRtToFrameBufferSprite;                         // メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		RenderTarget m_mainRTSnapshots[(int)EnMainRTSnapshot::enNum];   // メインレンダリングターゲットのスナップショット
		RenderTarget m_gBuffer[enGBufferNum];                           // G-Buffer
		std::vector<IRenderer*> m_renderObjects;						//描画オブジェクトのリスト
		Matrix m_viewProjMatrixForViewCulling;
		SceneGeometryData m_sceneGeometryData;							//シーンジオメトリ情報
		static RenderingEngine* m_instance;		                        // 唯一のインスタンスのアドレスを記録する変数。
		RenderTarget m_2DRenderTarget;                                  // 2D描画用のレンダ―ターゲット。
		Sprite m_2DSprite;												//2D合成用のスプライト
		Sprite m_mainSprite;

		/// <summary>
		/// イベントリスナーのデータ。
		/// </summary>
		struct SEventListenerData {
			void* pListenerObj;	//リスナーオブジェクト
			std::function<void(EnEvent enEvent)> listnerFunc;
		};

		std::list<SEventListenerData> m_eventListeners;		//イベントリスナー
	};
}