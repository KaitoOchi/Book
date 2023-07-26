#pragma once


namespace nsBookEngine {

	/// <summary>
	/// �t�H���g�����_�[�N���X�B
	/// </summary>
	class FontRender : public IRenderer
	{
	public:
		static const int MAX_TEXT_SIZE = 256;	//�����̍ő吔

		~FontRender()
		{

		}

		/// <summary>
		/// �\�����镶����ݒ�B
		/// </summary>
		/// <param name="text"></param>
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}

		/// <summary>
		/// �\�����镶�����擾�B
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetText() const
		{
			return m_text;
		}

		/// <summary>
		/// ���W��ݒ�Bz��0.0f�B
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
		/// �傫����ݒ�B
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const float scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// �傫�����擾�B
		/// </summary>
		/// <returns></returns>
		const float GetScale() const
		{
			return m_scale;
		}

		/// <summary>
		/// �F��ݒ�B
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}

		/// <summary>
		/// �F���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector4& GetColor() const
		{
			return m_color;
		}

		/// <summary>
		/// ��]��ݒ�B
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const float rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// ��]���擾�B
		/// </summary>
		/// <returns></returns>
		const float GetRotation() const
		{
			return m_rotation;
		}

		/// <summary>
		/// �s�{�b�g��ݒ�B
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
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
		/// �`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// �e�̃p�����[�^��ݒ�B
		/// </summary>
		/// <param name="isDrawShadow">�e��`�悷�邩�ǂ����B</param>
		/// <param name="shadowOffset">�s�N�Z���̃I�t�Z�b�g�ʁB</param>
		/// <param name="shadowColor">�e�̐F�B</param>
		void SetShadowParam(const bool isDrawShadow, const float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}

	private:
		/// <summary>
		/// 2D�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		void OnRender2D(RenderContext& rc) override
		{
			m_font.Begin(rc);
			m_font.Draw(m_text, Vector2(m_position.x, m_position.y), m_color, m_rotation, m_scale, m_pivot);
			m_font.End(rc);
		}

	private:
		Font		m_font;								//�t�H���g
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;	//�s�{�b�g
		Vector3		m_position = Vector3::Zero;			//���W
		Vector4		m_color = g_vec4White;				//�F
		wchar_t		m_text[MAX_TEXT_SIZE];				//����
		float		m_scale = 1.0f;						//�傫��
		float		m_rotation = 0.0f;					//��]
	};
}

