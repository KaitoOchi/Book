#pragma once


namespace nsBookEngine {

	/// <summary>
	/// レンダラーの基底クラス。
	/// </summary>
	class IRenderer
	{
	public:
		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="ligNo">ライト番号</param>
		/// <param name="lvpMatrix">ライトビュープロジェクション行列</param>
		virtual void OnRenderShadowMap(
			RenderContext& rc,
			Camera& camera
		)
		{

		}

		/// <summary>
		/// ZPrepaass描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		/// <returns></returns>
		virtual void OnRenderToZPrepass(RenderContext& rc)
		{

		}

		/// <summary>
		/// G-Buffer描画パスから呼ばれる処理。
		/// </summary>
		virtual void OnRenderToGBuffer(RenderContext& rc)
		{

		}
		/// <summary>
		/// フォワードレンダーパスから呼ばれる処理。
		/// </summary>
		virtual void OnForwardRender(RenderContext& rc)
		{

		}

		/// <summary>
		/// ワイプ越しのフォワードレンダーパスから呼ばれる処理。
		/// </summary>
		virtual void OnWipeForwardRender(RenderContext& rc, Camera& camera)
		{

		}

		/// <summary>
		/// 2D描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};
}