#pragma once

namespace nsBookEngine {

	/// <summary>
	/// �X�J�C�L���[�u�N���X�B
	/// </summary>
	class SkyCube : public IGameObject
	{
	public:
		SkyCube();
		~SkyCube();
		bool Start();
		void Render(RenderContext& rc);

	public:
		/// <summary>
		/// ��ނ�ݒ�B
		/// </summary>
		void SetType(const int type)
		{
			m_number = type;
		}

		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// ���W���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// �g�嗦��ݒ�B
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// �g�嗦���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetScale() const 
		{
			return m_scale;
		}

		/// <summary>
		/// ���邳��ݒ�B
		/// </summary>
		/// <param name="lum"></param>
		void SetLuminance(const float lum)
		{
			m_luminance = lum;
		}

	private:
		Texture			m_texture;								//�e�N�X�`��
		ModelRender		m_modelRender;							//�V�����f��
		Vector3			m_position;								//���W�@
		Vector3			m_scale = Vector3::One  * 1000.0f;		//�g�嗦
		int				m_number = 0;							//���
		float			m_luminance = 0.75f;					//�P�x
	};
}
