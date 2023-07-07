#pragma once


namespace nsBookEngine {

	/// <summary>
	/// �����_���[�̊��N���X�B
	/// </summary>
	class IRenderer
	{
	public:
		/// <summary>
		/// �V���h�E�}�b�v�ւ̕`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="ligNo">���C�g�ԍ�</param>
		/// <param name="lvpMatrix">���C�g�r���[�v���W�F�N�V�����s��</param>
		virtual void OnRenderShadowMap(
			RenderContext& rc,
			Camera& camera
		)
		{

		}

		/// <summary>
		/// ZPrepaass�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		/// <returns></returns>
		virtual void OnRenderToZPrepass(RenderContext& rc)
		{

		}

		/// <summary>
		/// G-Buffer�`��p�X����Ă΂�鏈���B
		/// </summary>
		virtual void OnRenderToGBuffer(RenderContext& rc)
		{

		}
		/// <summary>
		/// �t�H���[�h�����_�[�p�X����Ă΂�鏈���B
		/// </summary>
		virtual void OnForwardRender(RenderContext& rc)
		{

		}

		/// <summary>
		/// ���C�v�z���̃t�H���[�h�����_�[�p�X����Ă΂�鏈���B
		/// </summary>
		virtual void OnWipeForwardRender(RenderContext& rc, Camera& camera)
		{

		}

		/// <summary>
		/// 2D�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};
}