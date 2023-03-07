#pragma once

namespace nsBookEngine {

	/// <summary>
	/// �|�C���g���C�g�B
	/// </summary>
	class PointLight
	{
	public:
		/// <summary>
		/// �|�C���g���C�g�p�̍\����
		/// </summary>
		struct pointLight
		{
			Vector3 ptPosition = Vector3::Zero;		//���W
			float pointNum = 0.0f;					//�|�C���g���C�g�̐�
			Vector3 ptColor = Vector3::Zero;		//�J���[
			float ptRange = 0.0f;					//�e���͈�
		};

		PointLight();
		~PointLight();

		/// <summary>
		/// �����������B
		/// </summary>
		void Init();

		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		void SetPosition(const Vector3& pos)
		{
			m_pointLig.ptPosition = pos;
		}

		/// <summary>
		/// �J���[��ݒ�B
		/// </summary>
		void SetColor(const Vector3& color)
		{
			m_pointLig.ptColor = color;
		}

		/// <summary>
		/// �e���͈͂�ݒ�B
		/// </summary>
		void SetRange(float range)
		{
			m_pointLig.ptRange = range;
		}

		/// <summary>
		/// �\���̂��擾
		/// </summary>
		pointLight& GetPointLig()
		{
			return m_pointLig;
		}

		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();

	private:
		pointLight m_pointLig;
	};
}