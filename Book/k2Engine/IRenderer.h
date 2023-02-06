#pragma once


namespace nsK2Engine {

	/// <summary>
	/// レンダラーの基底クラス。
	/// </summary>
	class IRenderer {
	public:
		/// <summary>
		/// 頂点計算パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnComputeVertex(RenderContext& rc)
		{
		}
		/// <summary>
		/// ZPrepassから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnZPrepass(RenderContext& rc)
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
		/// 半透明オブジェクト描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnTlanslucentRender(RenderContext& rc)
		{

		}
		/// <summary>
		/// 2D描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
		/// <summary>
		/// レンダラーが描画するオブジェクトの最大座標と最小座標を取得。
		/// </summary>
		/// <param name="vMax"></param>
		/// <param name="vMin"></param>
		/// <param name="isGet"></param>
		virtual void GetAABB(Vector3& vMax, Vector3& vMin, bool& isGet)
		{
			vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
			isGet = false;
		}
	};
}