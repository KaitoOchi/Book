#pragma once

#include "graphics/IRenderer.h"

#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"
#include "graphics/light/SpotLight.h"
#include "graphics/light/HemiSphereLight.h"
#include "graphics/postEffect/Bloom.h"

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
			int playerAnim2D;									//2D�A�j���[�V�����̔ԍ�
		};

		//���C�g�p�̍\����
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig[4];
			SpotLight::spotLight spotLig[42];
			HemiSphereLight::hemiSphereLight hemiSphereLig;
			ShadowParamCB shadowCB;	
			int ptNum;											//�|�C���g���C�g�̐�
			int spNum;											//�X�|�b�g���C�g�̐�
		};

		//�X�v���C�g�p�̍\����
		struct SpriteCB
		{
			Vector3 clipSize;
			int clipMode = 0;
			int processMode = 0;
			float processRate = 0.0f;
		};

	private:
		RenderingEngine();
		~RenderingEngine();

	public:

		const void Setm_directionLig(Vector3 light,float ambi,Vector3 color)
		{
			m_directionLig.SetDirection(light);
			m_directionLig.SetAmbientLight(ambi);
			m_directionLig.SetColor(color);
		}



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
			GetLightCB().directionLig = m_directionLig.GetDirectionLig();
		}

		/// <summary>
		/// �����̐ݒ�B
		/// </summary>
		/// <param name="amb">�����B</param>
		void SetAmbient(const float amb)
		{
			m_directionLig.SetAmbientLight(amb);
			GetLightCB().directionLig = m_directionLig.GetDirectionLig();
		}

		/// <summary>
		/// �|�C���g���C�g�̐ݒ�B
		/// </summary>
		/// <param name="ptlig"></param>
		void SetPointLight(const int ptNum, PointLight::pointLight& ptlig)
		{
			//GetLightCB().pointLig = ptlig.GetPointLig();
			GetLightCB().pointLig[ptNum].ptPosition = ptlig.ptPosition;
			GetLightCB().pointLig[ptNum].ptColor = ptlig.ptColor;
			GetLightCB().pointLig[ptNum].ptRange = ptlig.ptRange;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̐ݒ�B
		/// </summary>
		/// <param name="spLig"></param>
		void SetSpotLight(const int spNum, SpotLight::spotLight& spLig)
		{
			GetLightCB().spotLig[spNum].spPosition = spLig.spPosition;
			GetLightCB().spotLig[spNum].spColor = spLig.spColor;
			GetLightCB().spotLig[spNum].spRange = spLig.spRange;
			GetLightCB().spotLig[spNum].spDirection = spLig.spDirection;
			GetLightCB().spotLig[spNum].spAngle = spLig.spAngle;
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
		/// �u���[����臒l��ݒ�B
		/// </summary>
		void SetBloomThreshold(const float threshold)
		{
			m_bloom.SetThreshold(threshold);
		}

		/// <summary>
		/// �摜���H�̐ݒ�B
		/// </summary>
		/// <param name="mode">1�ŃZ�s�A��</param>
		void SetScreenProcess(const int mode)
		{
			m_spriteCB.processMode = mode;
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

		/// <summary>
		/// �V���h�E�p�̍\���̂��擾�B
		/// </summary>
		/// <returns></returns>
		ShadowParamCB& GetShadowCB()
		{
			return m_lightCB.shadowCB;
		}

		/// <summary>
		/// ZPrepass�����_�[�^�[�Q�b�g�e�N�X�`�����擾�B
		/// </summary>
		/// <returns></returns>
		Texture& GetZPrepassDepthTexture()
		{
			return m_zprepassRenderTarget.GetRenderTargetTexture();
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
		/// ���C�v�p�̃r���[�|�[�g���擾�B
		/// </summary>
		D3D12_VIEWPORT& GetWipeViewPort()
		{
			return m_viewPorts[1];
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
		/// ZPrepass�p�̃����_�[�^�[�Q�b�g���������B
		/// </summary>
		void InitZPrepassRenderTarget();

		/// <summary>
		/// �r���[�|�[�g�̏������B
		/// </summary>
		void InitViewPort();

		/// <summary>
		/// �V���h�E�}�b�v�̕`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void RenderShadowMap(RenderContext& rc);

		/// <summary>
		/// ZPrepass�̕`�揈���B
		/// </summary>
		void ZPrepass(RenderContext& rc);

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

		LightCB					m_lightCB;							//���C�g�p�̍\����
		SpriteCB				m_spriteCB;							//�X�v���C�g�p�̍\����
		DirectionLight			m_directionLig;						//�f�B���N�V���i�����C�g�̍\����
		HemiSphereLight			m_hemiSphereLig;					//�������C�g�̍\����
		Bloom					m_bloom;							//�u���[��

		RenderTarget			m_mainRenderTarget;					//���C�������_�[�^�[�Q�b�g
		RenderTarget			m_2DRenderTarget;					//2D�����_�[�^�[�Q�b�g
		Sprite					m_2DSprite;                         //2D�X�v���C�g
		Sprite					m_mainSprite;
		Sprite					m_copyMainRtToFrameBufferSprite;    //���C�������_�[�^�[�Q�b�g�̃X�v���C�g
		RenderTarget			m_shadowMapRenderTarget;			//�V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g
		GaussianBlur			m_shadowBlur;						//�V���h�E�p�̃K�E�V�A���u���[
		RenderTarget			m_zprepassRenderTarget;				//ZPrepass�p�̃����_�[�^�[�Q�b�g
		std::vector<IRenderer*> m_renderObjects;					//�����_�����O����I�u�W�F�N�g
		Camera					m_lightCamera;						//���C�g�J����
		Camera					m_wipeCamera;						//���C�v�J�����B

		D3D12_VIEWPORT			m_viewPorts[2];						//�r���[�|�[�g
	};
}