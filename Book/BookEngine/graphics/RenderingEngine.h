#pragma once

#include "graphics/IRenderer.h"

#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"
#include "graphics/light/SpotLight.h"
#include "graphics/light/HemiSphereLight.h"
#include "graphics/postEffect/Bloom.h"

#define MAX_POINT_LIGHT 4;
#define MAX_SPOT_LIGHT 4;

namespace nsBookEngine {

	class RenderingEngine
	{
	public:
		//�V���h�E�}�b�v�p�̍\����
		struct ShadowParamCB
		{
			Vector3 lightPos = Vector3::Zero;
			float pad0;
			Matrix mLVP = g_matIdentity;
		};

		//���C�g�p�̍\����
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig;
			SpotLight::spotLight spotLig;
			HemiSphereLight::hemiSphereLight hemiSphereLig;
			ShadowParamCB shadowCB;
		};

		//�X�v���C�g�p�̍\����
		struct SpriteCB
		{
			float clipSize;
		};

	private:
		RenderingEngine();
		~RenderingEngine();

	public:
		/// <summary>
		/// �C���X�^���X�̐����B
		/// </summary>
		static void CreateInstance()
		{
			m_instance = new RenderingEngine;
			m_instance->Init();
		}
		/// <summary>
		/// �C���X�^���X�̍폜�B
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
		}
		/// <summary>
		/// �C���X�^���X�̎擾�B
		/// </summary>
		/// <returns></returns>
		static RenderingEngine* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// �����_�[�I�u�W�F�N�g��ǉ��B
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// �f�B���N�V���i�����C�g�̐ݒ�B
		/// </summary>
		/// <param name="dir">���C�g�̕����B</param>
		/// <param name="color">���C�g�̃J���[�B[</param>
		void SetDirectionLight(const Vector3& dir, const Vector3 color)
		{
			m_directionLig.SetDirection(dir);
			m_directionLig.SetColor(color);
		}

		/// <summary>
		/// �����̐ݒ�B
		/// </summary>
		/// <param name="amb">�����B</param>
		void SetAmbient(const float amb)
		{
			m_directionLig.SetAmbientLight(amb);
		}

		/// <summary>
		/// �|�C���g���C�g�̐ݒ�B
		/// </summary>
		/// <param name="ptlig"></param>
		void SetPointLight(const int ptNum, PointLight::pointLight& ptlig)
		{
			//GetLightCB().pointLig = ptlig.GetPointLig();
			GetLightCB().pointLig.ptPosition = ptlig.ptPosition;
			GetLightCB().pointLig.ptColor = ptlig.ptColor;
			GetLightCB().pointLig.ptRange = ptlig.ptRange;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̐ݒ�B
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

		/// <summary>
		/// �X�v���C�g�p�̍\���̂��擾�B
		/// </summary>
		/// <returns></returns>
		SpriteCB& GetSpriteCB()
		{
			return m_spriteCB;
		}

		Vector3& GetLightPos()
		{
			return m_lightCB.shadowCB.lightPos;
		}

		/// <summary>
		/// �V���h�E�}�b�v�p�����_�[�^�[�Q�b�g���擾�B
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetShadowRenderTarget()
		{
			return m_shadowMapRenderTarget;
		}

		/// <summary>
		/// �V���h�E�}�b�v�p�K�E�V�A���u���[���擾�B
		/// </summary>
		/// <returns></returns>
		GaussianBlur& GetShadowBlur()
		{
			return m_shadowBlur;
		}

		/// <summary>
		/// �u���[����臒l��ݒ�B
		/// </summary>
		void SetBloomThreshold(const float threshold)
		{
			m_bloom.SetThreshold(threshold);
		}

		/// <summary>
		/// �����������B
		/// </summary>
		void Init();

		/// <summary>
		/// ���s�����B
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);

	private:
		/// <summary>
		/// 2D�����_�[�^�[�Q�b�g���������B
		/// </summary>
		void Init2DRenderTarget();

		/// <summary>
		/// �V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g���������B
		/// </summary>
		void InitShadowMapRenderTarget();

		/// <summary>
		/// �V���h�E�}�b�v�̕`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void RenderShadowMap(RenderContext& rc);

		/// <summary>
		/// �t�H���[�h�����_�����O�̕`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void ForwardRendering(RenderContext& rc);

		/// <summary>
		/// 2D�̕`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);

	private:
		static RenderingEngine* m_instance;

		LightCB m_lightCB;
		SpriteCB m_spriteCB;

		DirectionLight m_directionLig;
		HemiSphereLight m_hemiSphereLig;

		Bloom m_bloom;

		RenderTarget m_mainRenderTarget;								//���C�������_�[�^�[�Q�b�g

		RenderTarget m_2DRenderTarget;                                  //2D�����_�[�^�[�Q�b�g
		Sprite m_2DSprite;                                              //2D�X�v���C�g
		Sprite m_mainSprite;
		Sprite m_copyMainRtToFrameBufferSprite;                         //���C�������_�[�^�[�Q�b�g�̃X�v���C�g
		
		RenderTarget m_shadowMapRenderTarget;							//�V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g
		GaussianBlur m_shadowBlur;										//�V���h�E�p�̃K�E�V�A���u���[
		
		std::vector<IRenderer*> m_renderObjects;

		Camera m_lightCamera;
	};
}