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
			Vector3 ptPosition;		//���W
			float pad0 = 0.0f;
			Vector3 ptColor;		//�J���[
			float ptRange = 0.0f;	//�e���͈�
		};

		PointLight();
		~PointLight();

		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		void SetPosition(const Vector3& pos)
		{
			m_pointLig.ptPosition = pos;
			//g_camera3D->GetViewMatrix().Apply(m_pointLig.ptPosition);
		}

		/// <sSummary>
		/// ���W���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition()
		{
			return m_pointLig.ptPosition;
		}

		/// <summary>
		/// �J���[��ݒ�B
		/// </summary>
		void SetColor(const Vector3& color)
		{
			m_pointLig.ptColor = color;
		}

		/// <summary>
		/// �J���[���擾�B
		/// </summary>
		/// <param name="range"></param>
		const Vector3& GetColor()
		{
			return m_pointLig.ptColor;
		}

		/// <summary>
		/// �e���͈͂�ݒ�B
		/// </summary>
		void SetRange(float range)
		{
			m_pointLig.ptRange = range;
		}

		/// <summary>
		/// �e���͈͂��擾�B
		/// </summary>
		const float& GetRange()
		{
			return m_pointLig.ptRange;
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