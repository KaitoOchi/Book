#pragma once


namespace nsBookEngine {

	/// <summary>
	/// レンダラーの基底クラス。
	/// </summary>
	class IRenderer {
	public:
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
		/// 2D描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};
}