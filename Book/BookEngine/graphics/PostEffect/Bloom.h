#pragma once

namespace nsBookEngine {

	/// <summary>
	/// �u���[���N���X�B
	/// </summary>
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
		RenderTarget	m_luminanceRenderTarget;	//�u���[���p�����_�[�^�[�Q�b�g
		GaussianBlur	m_gaussianBlur[4];			//�K�E�V�A���u���[
		LuminanceCB		m_luminanceCB;				//�u���[���p�萔�o�b�t�@
		Sprite			m_luminanceSprite;			//�u���[���p�X�v���C�g
		Sprite			m_finalSprite;				//�ŏI�����p�̃X�v���C�g
		Sprite			m_copyMainRtSprite;			//���C�������_�����O�^�[�Q�b�g�ɕ`�悷�邽�߂̃X�v���C�g
	};
}