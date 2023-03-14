#pragma once


namespace nsBookEngine {

	/// <summary>
	/// �����_���[�̊��N���X�B
	/// </summary>
	class IRenderer {
	public:
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
		/// 2D�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};
}