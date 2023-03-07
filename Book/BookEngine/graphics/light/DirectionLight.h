#pragma once

namespace nsBookEngine {

	class DirectionLight
	{
	public:
		//�f�B���N�V�������C�g�p�̍\����
		struct directionLight
		{
			Vector3 ligDirection = Vector3::Zero;	//���C�g�̕���
			float pad = 0.0f;
			Vector3 ligColor = Vector3::Zero;		//���C�g�̐F
			float pad2 = 0.0f;
			Vector3 eyePos = Vector3::Zero;			//���_�̈ʒu
			float pad3 = 0.0f;
			Vector3 ambient = Vector3::Zero;
		};

		DirectionLight();
		~DirectionLight();

		/// <summary>
		/// ���C�g�̕�����ݒ�
		/// </summary>
		void SetDirection(const Vector3 dir)
		{
			m_directionLig.ligDirection = dir;
			//���C�g�𐳋K��
			m_directionLig.ligDirection.Normalize();
		}

		/// <summary>
		/// ���C�g�̐F��ݒ�
		/// </summary>
		void SetColor(const Vector3 dir)
		{
			m_directionLig.ligColor = dir;
		}

		/// <summary>
		/// ������ݒ�
		/// </summary>
		void SetAmbientLight(const float amb)
		{
			m_directionLig.ambient.x = amb;
			m_directionLig.ambient.y = amb;
			m_directionLig.ambient.z = amb;
		}

		/// <summary>
		/// �\���̂��擾
		/// </summary>
		directionLight& GetDirectionLig()
		{
			return m_directionLig;
		}

	private:
		directionLight m_directionLig;

	};
}