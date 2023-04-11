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
		//���C�g�p�萔�o�b�t�@�̍\����
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig;
			SpotLight::spotLight spotLig;
			HemiSphereLight::hemiSphereLight hemiSphereLig;
			Matrix mLVP;
		};

		//�X�v���C�g�p�萔�o�b�t�@�̍\����
		struct SpriteCB
		{
			float clipSize;
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
		/// �C���X�^���X��擾�B
		/// </summary>
		/// <returns></returns>
		static RenderingEngine* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// �`��I�u�W�F�N�g��ǉ��B
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
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
		/// ���C�g�p�̍\���̂�擾�B
		/// </summary>
		LightCB& GetLightCB()
		{
			return m_lightCB;
		}

		/// <summary>
		/// �X�v���C�g�p�̍\���̂�擾�B
		/// </summary>
		/// <returns></returns>
		SpriteCB& GetSpriteCB()
		{
			return m_spriteCB;
		}

		/// <summary>
		/// シャドウマップ用レンダーターゲットを取得。
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetShadowRenderTarget()
		{
			return m_shadowMapRenderTarget;
		}

		/// <summary>
		/// �u���[������������臒l��ݒ�
		/// </summary>
		void SetBloomThreshold(const float threshold)
		{
			m_bloom.SetThreshold(threshold);
		}

		void Init();

		/// <summary>
		/// ���s�����B
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);

	private:
		/// <summary>
		/// 2D�`��p�̃����_�\�^�[�Q�b�g�������
		/// </summary>
		void Init2DRenderTarget();

		/// <summary>
		/// �V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g��������B
		/// </summary>
		void InitShadowMapRenderTarget();

		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓�e��
		/// �t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�����������
		/// </summary>
		void InitGBuffer();

		/// <summary>
		/// �f�B�t�@�[�h���C�e�B���O��������B
		/// </summary>
		void InitDefferedLighting();

		/// <summary>
		/// �V���h�E�}�b�v�̏����B
		/// </summary>
		/// <param name="rc"></param>
		void RenderShadowMap(RenderContext& rc);

		/// <summary>
		/// �t�H���[�h�����_�����O�̏����B
		/// </summary>
		/// <param name="rc"></param>
		void ForwardRendering(RenderContext& rc);

		/// <summary>
		/// GBuffer�̏����B
		/// </summary>
		/// <param name="rc"></param>
		void RenderToGBuffer(RenderContext& rc);

		/// <summary>
		/// �f�B�t�@�[�h���C�e�B���O�̏����B
		/// </summary>
		/// <param name="rc"></param>
		void DeferredLighting(RenderContext& rc);

		/// <summary>
		/// 2D�`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Render2D(RenderContext& rc);

	private:


		static RenderingEngine* m_instance;

		LightCB m_lightCB;
		SpriteCB m_spriteCB;

		DirectionLight m_directionLig;
		HemiSphereLight m_hemiSphereLig;

		Bloom m_bloom;

		RenderTarget m_mainRenderTarget;								//���C�������_�����O�^�[�Q�b�g

		RenderTarget m_2DRenderTarget;                                  //2D�`��p�̃����_�\�^�[�Q�b�g
		Sprite m_2DSprite;                                              //2D�����p�̃X�v���C�g
		Sprite m_mainSprite;
		Sprite m_copyMainRtToFrameBufferSprite;                         // ���C�������_�����O�^�[�Q�b�g��t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		
		RenderTarget m_shadowMapRenderTarget;							//�V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g
		
		std::vector<IRenderer*> m_renderObjects;

		Camera m_lightCamera;
	};
}