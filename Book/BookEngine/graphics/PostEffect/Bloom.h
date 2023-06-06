#pragma once

namespace nsBookEngine {

	/// <summary>
	/// ブルームクラス。
	/// </summary>
	class Bloom
	{
	public:
		struct LuminanceCB {

			float threshold = 1.0f;	//閾値
		};

	public:
		Bloom();
		~Bloom();

		/// <summary>
		/// ブルームが発生する閾値を設定。
		/// </summary>
		/// <param name="value"></param>
		void SetThreshold(float threshold)
		{
			m_luminanceCB.threshold = threshold;
		}

		/// <summary>
		/// ブルームが発生する閾値を取得。
		/// </summary>
		/// <returns></returns>
		float GetThreshold() const
		{
			return m_luminanceCB.threshold;
		}

		/// <summary>
		/// ポストエフェクトを実行した結果となるテクスチャを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetResultTexture()
		{
			return m_luminanceRenderTarget.GetRenderTargetTexture();
		}

		/// <summary>
		/// 初期化処理。
		/// </summary>
		void Init(RenderTarget& mainRenderTarget);

		/// <summary>
		/// 描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		RenderTarget	m_luminanceRenderTarget;	//ブルーム用レンダーターゲット
		GaussianBlur	m_gaussianBlur[4];			//ガウシアンブラー
		LuminanceCB		m_luminanceCB;				//ブルーム用定数バッファ
		Sprite			m_luminanceSprite;			//ブルーム用スプライト
		Sprite			m_finalSprite;				//最終合成用のスプライト
		Sprite			m_copyMainRtSprite;			//メインレンダリングターゲットに描画するためのスプライト
	};
}