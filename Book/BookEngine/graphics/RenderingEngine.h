#pragma once

#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"

namespace nsBookEngine {

	class RenderingEngine
	{
	public:
		//���C�g�p�萔�o�b�t�@�̍\����
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig;
		};

		RenderingEngine();
		~RenderingEngine();

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
		/// ���C�g�p�̍\���̂��擾�B
		/// </summary>
		LightCB& GetLightCB()
		{
			return m_lightCB;
		}

		void Init();
		void Update();

	private:
		LightCB m_lightCB;

		DirectionLight m_directionLig;
	};
}