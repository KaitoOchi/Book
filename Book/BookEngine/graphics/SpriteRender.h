#pragma once


namespace nsBookEngine {

	class SpriteRender
	{
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="w">�摜�̉����B</param>
		/// <param name="h">�摜�̏c���B</param>
		/// <param name="alpha">�f�t�H���g�͔����������B</param>
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alpha = AlphaBlendMode_Trans);

		/// <summary>
		/// ���W��ݒ�Bz��0.0f�Œ�B
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
		const Vector3& GetPosition()
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
		const Vector3& GetScale()
		{
			return m_scale;
		}

		/// <summary>
		/// ��]��ݒ�B
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// ��]���擾�B
		/// </summary>
		/// <returns></returns>
		const Quaternion& GetRotation()
		{
			return m_rotation;
		}

		/// <summary>
		/// �s�{�b�g��ݒ�B
		/// </summary>
		/// <param name="pivot"></param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}

		/// <summary>
		/// �s�{�b�g���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector2& GetPivot()
		{
			return m_pivot;
		}

		/// <summary>
		/// ��Z�J���[��ݒ�B
		/// </summary>
		/// <param name="mulColor"></param>
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}

		/// <summary>
		/// ��Z�J���[���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector4& GetMulColor()
		{
			return m_sprite.GetMulColor();
		}

		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update()
		{
			m_sprite.Update(m_position,
				m_rotation,
				m_scale,
				m_pivot);
		}

		/// <summary>
		/// �`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

	private:
		void OnRender2D(RenderContext& rc)
		{
			m_sprite.Draw(rc);
		}

	private:
		Sprite		m_sprite;								//�X�v���C�g
		Vector3		m_position = Vector3::Zero;				//���W
		Quaternion	m_rotation = Quaternion::Identity;		//��]
		Vector3		m_scale = Vector3::One;					//�g�嗦
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;		//�s�{�b�g
	};
}
