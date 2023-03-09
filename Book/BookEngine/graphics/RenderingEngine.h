#pragma once

#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"
#include "graphics/light/SpotLight.h"
#include "graphics/light/HemiSphereLight.h"

namespace nsBookEngine {

	class RenderingEngine
	{
	public:
		//���C�g�p�萔�o�b�t�@�̍\����
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig;
			SpotLight::spotLight spotLig;
			HemiSphereLight::hemiSphereLight hemiSphereLig;
		};

	private:
		RenderingEngine();
		~RenderingEngine();

	public:
		/// <summary>
		/// �C���X�^���X�̍쐬�B
		/// </summary>
		static void CreateInstance()
		{
			m_instance = new RenderingEngine;
			m_instance->Init();
		}
		/// <summary>
		/// �C���X�^���X�̔j���B
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
		}
		/// <summary>
		/// �C���X�^���X���擾�B
		/// </summary>
		/// <returns></returns>
		static RenderingEngine* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// �f�B���N�V�������C�g��ݒ�B
		/// </summary>
		/// <param name="dir">���C�g�̕���</param>
		/// <param name="color">���C�g�̃J���[</param>
		void SetDirectionLight(const Vector3& dir, const Vector3 color)
		{
			m_directionLig.SetDirection(dir);
			m_directionLig.SetColor(color);
		}

		/// <summary>
		/// ������ݒ�B
		/// </summary>
		/// <param name="amb">����</param>
		void SetAmbient(const float amb)
		{
			m_directionLig.SetAmbientLight(amb);
		}

		/// <summary>
		/// �|�C���g���C�g��ݒ�
		/// </summary>
		/// <param name="ptlig"></param>
		void SetPointLight(PointLight::pointLight& ptlig)
		{
			//GetLightCB().pointLig = ptlig.GetPointLig();
			GetLightCB().pointLig.ptPosition = ptlig.ptPosition;
			GetLightCB().pointLig.ptColor = ptlig.ptColor;
			GetLightCB().pointLig.ptRange = ptlig.ptRange;
		}

		/// <summary>
		/// �X�|�b�g���C�g��ݒ�
		/// </summary>
		/// <param name="spLig"></param>
		void SetSpotLight(SpotLight::spotLight& spLig)
		{
			GetLightCB().spotLig.spPosition = spLig.spPosition;
			GetLightCB().spotLig.spColor = spLig.spColor;
			GetLightCB().spotLig.spRange = spLig.spRange;
			GetLightCB().spotLig.spDirection = spLig.spDirection;
			GetLightCB().spotLig.spAngle = spLig.spAngle;
		}

		/// <summary>
		/// �������C�g��ݒ�B
		/// </summary>
		/// <param name="grdColor">�Ƃ�Ԃ��̃��C�gRGB�B0.0f�`1.0f�̊�</param>
		/// <param name="skyColor">�V�����C�gRGB�B0.0f�`1.0f�̊�</param>
		/// <param name="grdNormal">�n�ʂ̖@��0.0f�`1.0f�̊�</param>
		void SetHemiSphereLight(const Vector3& grdColor, const Vector3& skyColor, const Vector3& grdNormal)
		{
			m_hemiSphereLig.SetGroundColor(grdColor);
			m_hemiSphereLig.SetSkyColor(skyColor);
			m_hemiSphereLig.SetGroundNormal(grdNormal);
		}

		/// <summary>
		/// ���C�g�p�̍\���̂��擾�B
		/// </summary>
		LightCB& GetLightCB()
		{
			return m_lightCB;
		}

		void Init();
		void Update();

	private:
		static RenderingEngine* m_instance;

		LightCB m_lightCB;

		DirectionLight m_directionLig;
		HemiSphereLight m_hemiSphereLig;
	};
}