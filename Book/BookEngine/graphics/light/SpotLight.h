#pragma once

namespace nsBookEngine {

	class SpotLight
	{
	public:
		/// <summary>
		/// �X�|�b�g���C�g�p�̍\����
		/// </summary>
		struct spotLight
		{
			Vector3 spPosition;		//���W
			float pad0;				
			Vector3 spColor;		//�J���[
			float spRange;			//�e���͈�
			Vector3 spDirection;	//����
			float spAngle;			//�ˏo�p�x
		};

	public:
		SpotLight();
		~SpotLight();

		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_spotLig.spPosition = pos;
		}

		/// <summary>
		/// ���W���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPositoin()
		{
			return m_spotLig.spPosition;
		}

		/// <summary>
		/// �J���[��ݒ�B
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_spotLig.spColor = color;
		}

		/// <summary>
		/// �J���[���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetColor()
		{
			return m_spotLig.spColor;
		}

		/// <summary>
		/// �e���͈͂�ݒ�B
		/// </summary>
		/// <param name="range"></param>
		void SetRange(const float range)
		{
			m_spotLig.spRange = range;
		}

		/// <summary>
		/// �e���͈͂��擾�B
		/// </summary>
		/// <returns></returns>
		const float GetRange()
		{
			return m_spotLig.spRange;
		}

		/// <summary>
		/// ������ݒ�B
		/// </summary>
		/// <param name="dir"></param>
		void SetDirection(const Vector3& dir)
		{
			m_spotLig.spDirection = dir;
			m_spotLig.spDirection.Normalize();
		}

		/// <summary>
		/// �������擾�B
		/// </summary>
		/// <returns></returns>
		Vector3& GetDirection()
		{
			return m_spotLig.spDirection;
		}

		/// <summary>
		/// �ˏo�p�x��ݒ�B
		/// </summary>
		/// <param name="angle"></param>
		void SetAngle(const float angle)
		{
			m_spotLig.spAngle = Math::DegToRad(angle);
		}

		/// <summary>
		/// �ˏo�p�x���擾�B
		/// </summary>
		/// <returns></returns>
		const float GetAngle()
		{
			return m_spotLig.spAngle;
		}

		/// <summary>
		/// �\���̂��擾�B
		/// </summary>
		/// <returns></returns>
		spotLight& GetSpotLight()
		{
			return m_spotLig;
		}

		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();

	private:
		spotLight m_spotLig;
	};

}
