#pragma once

namespace nsBookEngine{

	class HemiSphereLight
	{
	public:
		/// <summary>
		/// �������C�g�̍\����
		/// </summary>
		struct hemiSphereLight {
			Vector3 groundColor = Vector3::Zero;	//�n�ʂ̐F
			float pad0 = 0.0f;
			Vector3 skyColor = Vector3::Zero;		//��̐F
			float pad1 = 0.0f;
			Vector3 groundNormal = Vector3::Zero;	//�n�ʂ̖@��
			float pad2 = 0.0f;
		};

		HemiSphereLight();
		~HemiSphereLight();

		/// <summary>
		/// �n�ʂ̐F��ݒ�
		/// </summary>
		/// <param name="groundColor">RGB�B0.0f�`1.0f�̊�</param>
		void SetGroundColor(const Vector3& groundColor)
		{
			m_hemiSphereLight.groundColor = groundColor;
		}

		/// <summary>
		/// ��̐F��ݒ�
		/// </summary>
		/// <param name="skyColor">RGB�B0.0f�`1.0f�̊�</param>
		void SetSkyColor(const Vector3& skyColor)
		{
			m_hemiSphereLight.skyColor = skyColor;
		}

		/// <summary>
		/// �n�ʂ̖@����ݒ�
		/// </summary>
		/// <param name="groundNormal">0.0f�`1.0f�̊�</param>
		void SetGroundNormal(const Vector3& groundNormal)
		{
			m_hemiSphereLight.groundNormal = groundNormal;
		}

		/// <summary>
		/// �\���̂��擾
		/// </summary>
		/// <returns></returns>
		hemiSphereLight& GetHemiSphereLig()
		{
			return m_hemiSphereLight;
		}

	private:
		hemiSphereLight m_hemiSphereLight;
	};
}