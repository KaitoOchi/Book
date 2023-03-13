#pragma once

namespace nsBookEngine {

	class Bloom
	{
	public:
		struct LuminanceCB {

			float threshold = 1.0f;	//臒l
		};

	public:
		Bloom();
		~Bloom();

		/// <summary>
		/// �u���[������������臒l��ݒ�B
		/// </summary>
		/// <param name="value"></param>
		void SetThreshold(float threshold)
		{
			m_luminanceCB.threshold = threshold;
		}
		/// <summary>
		/// �u���[������������臒l���擾�B
		/// </summary>
		/// <returns></returns>
		float GetThreshold() const
		{
			return m_luminanceCB.threshold;
		}

		/// <summary>
		/// �|�X�g�G�t�F�N�g�����s�������ʂƂȂ�e�N�X�`�����擾�B
		/// </summary>
		/// <returns></returns>
		Texture& GetResultTexture()
		{
			return m_luminanceRenderTarget.GetRenderTargetTexture();
		}

		/// <summary>
		/// �����������B
		/// </summary>
		void Init(RenderTarget& mainRenderTarget);

		/// <summary>
		/// �`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		RenderTarget m_luminanceRenderTarget;
		Sprite m_luminanceSprite;
		GaussianBlur m_gaussianBlur[4];
		Sprite m_finalSprite;

		LuminanceCB m_luminanceCB;

		Sprite m_copyMainRtSprite;	// ���C�������_�����O�^�[�Q�b�g�ɕ`�悷�邽�߂̃X�v���C�g�B
	};
}