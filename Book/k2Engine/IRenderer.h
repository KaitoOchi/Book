#pragma once


namespace nsK2Engine {

	/// <summary>
	/// �����_���[�̊��N���X�B
	/// </summary>
	class IRenderer {
	public:
		/// <summary>
		/// ���_�v�Z�p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnComputeVertex(RenderContext& rc)
		{
		}
		/// <summary>
		/// ZPrepass����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnZPrepass(RenderContext& rc)
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
		/// �������I�u�W�F�N�g�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnTlanslucentRender(RenderContext& rc)
		{

		}
		/// <summary>
		/// 2D�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
		/// <summary>
		/// �����_���[���`�悷��I�u�W�F�N�g�̍ő���W�ƍŏ����W���擾�B
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