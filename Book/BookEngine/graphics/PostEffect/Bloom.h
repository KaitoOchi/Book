#pragma once

namespace nsBookEngine {

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
		/// 初期化処理。
		/// </summary>
		void Init(RenderTarget& mainRenderTarget);

	private:
		RenderTarget m_luminanceRenderTarget;
		Sprite m_luminanceSprite;
		GaussianBlur m_gaussianBlur[4];
		Sprite m_finalSprite;

		LuminanceCB m_luminanceCB;
	};
}