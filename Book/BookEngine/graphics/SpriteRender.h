#pragma once


namespace nsBookEngine {

	/// <summary>
	/// �X�v���C�g�����_�[�N���X�B
	/// </summary>
	class SpriteRender : public IRenderer
	{
	public:
		/// <summary>
		/// �N���b�v���[�h�B
		/// </summary>
		enum SpriteClipMode
		{
			enMode_None,
			enMode_2DGauge,
			enMode_SettingBGM,
			enMode_SettingSFX,
			enMode_UVScroll,
			enMode_CircleGauge,
		};

	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="w">�摜�̉����B</param>
		/// <param name="h">�摜�̏c���B</param>
		/// <param name="alpha">�f�t�H���g�͔����������B</param>
		void Init(const char* filePath, const float w, const float h, const AlphaBlendMode alpha = AlphaBlendMode_Trans, const SpriteClipMode clipMode = enMode_None);

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
		const Quaternion& GetRotation() const
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
		const Vector2& GetPivot() const
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
		const Vector4& GetMulColor() const
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
		/// <summary>
		/// �N���b�v�{�[�h�̐ݒ菈���B
		/// </summary>
		void ClipMode(SpriteInitData& initData, const SpriteClipMode clipMode = enMode_None);

		/// <summary>
		/// 2D�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		void OnRender2D(RenderContext& rc) override
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
