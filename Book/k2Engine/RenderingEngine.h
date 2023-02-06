#pragma once


#include "MyRenderer.h"

namespace nsK2Engine {

	class RenderingEngine
	{
	public:
		RenderingEngine();
		~RenderingEngine();

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
		/// ビューカリング用のビュープロジェクション行列を計算。
		/// </summary>
		void CalcViewProjectionMatrixForViewCulling();

	private:
		std::vector<IRenderer* > m_renderObjects;		//描画オブジェクトのリスト
		Matrix m_viewProjMatrixForViewCulling;

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