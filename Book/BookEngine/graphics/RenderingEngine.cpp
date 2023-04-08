#include "BookEnginePreCompile.h"
#include "RenderingEngine.h"


namespace nsBookEngine {

	RenderingEngine* RenderingEngine::m_instance = nullptr;

	RenderingEngine::RenderingEngine()
	{
	}

	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Init()
	{

		//�f�B���N�V�������C�g��ݒ�
		SetDirectionLight(Vector3(1, -1, 1), Vector3(0.5f, 0.5f, 0.5f));

		//������ݒ�
		SetAmbient(0.3f);

		//�������C�g��ݒ�
		SetHemiSphereLight(
			Vector3(0.4f, 0.2f, 0.2f),
			Vector3(0.1f, 0.3f, 0.2f),
			Vector3(0.0f, 1.0f, 0.0f)
		);

		//�\���̂ɕۑ�
		m_lightCB.directionLig = m_directionLig.GetDirectionLig();
		m_lightCB.hemiSphereLig = m_hemiSphereLig.GetHemiSphereLig();

		// 影描画用のライトカメラを作成する
		Camera lightCamera;

		// カメラの位置を設定。これはライトの位置
		lightCamera.SetPosition(0, 500, 0);

		// カメラの注視点を設定。これがライトが照らしている場所
		lightCamera.SetTarget(1, 0, 0);

		// 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
		lightCamera.SetUp(0, 1, 0);

		// ライトビュープロジェクション行列を計算している
		lightCamera.Update();
		m_lightCB.mLVP = lightCamera.GetViewProjectionMatrix();
		//m_lightCB.mLVP = g_camera3D->GetViewProjectionMatrix();

		//���C�������_�����O�^�[�Q�b�g�̐ݒ�
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//�u���[����臒l��ݒ�
		SetBloomThreshold(3.5f);
		m_bloom.Init(m_mainRenderTarget);

		Init2DRenderTarget();

		InitShadowMapRenderTarget();
	}

	void RenderingEngine::Init2DRenderTarget()
	{
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

		m_2DRenderTarget.Create(
			UI_SPACE_WIDTH,
			UI_SPACE_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		// �ŏI�����p�̃X�v���C�g�����������
		SpriteInitData spriteInitData;
		//�e�N�X�`����2D�����_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D�p�̃V�F�[�_�[��g�p����
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		//�㏑���B
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		//�e�N�X�`���̓��C�������_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		//�𑜓x��2D�����_�\�^�[�Q�b�g�̕��ƍ���
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitShadowMapRenderTarget()
	{
		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_shadowMapRenderTarget.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//視点の位置を設定する
		m_lightCB.directionLig.eyePos = g_camera3D->GetPosition();

		//m_lightCB.mLVP = g_camera3D->GetViewProjectionMatrix();

		RenderShadowMap(rc);

		ForwardRendering(rc);

		m_bloom.Render(rc, m_mainRenderTarget);

		Render2D(rc);

		m_renderObjects.clear();
	}

	void RenderingEngine::RenderShadowMap(RenderContext& rc)
	{
		//�e�𐶐����������f����V���h�E�}�b�v�ɕ`�悷��
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMapRenderTarget);
		rc.SetRenderTargetAndViewport(m_shadowMapRenderTarget);
		rc.ClearRenderTargetView(m_shadowMapRenderTarget);

		m_lightCB.shadowReceiver = true;

		//�I�u�W�F�N�g�̕`��
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRenderShadowMap(rc);
		}

		m_lightCB.shadowReceiver = false;


		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRenderTarget);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		BeginGPUEvent("ForwardRendering");

		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

		//rc.SetRenderTarget(
		//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		//);

		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		rc.ClearRenderTargetView(m_mainRenderTarget);

		//�I�u�W�F�N�g�̕`��
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnForwardRender(rc);
		}

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");
		////�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		////PRESENT����RENDERTARGET�ցB
		//rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		//// �����_�����O�^�[�Q�b�g��ݒ�
		//rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		//// �����_�����O�^�[�Q�b�g��N���A
		//rc.ClearRenderTargetView(m_2DRenderTarget);

		//m_mainSprite.Draw(rc);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRender2D(rc);
		}

		////RENDERTARGET����PRESENT�ցB
		//rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);
		////PRESENT����RENDERTARGET�ցB
		//rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

		//// �����_�����O�^�[�Q�b�g��ݒ�
		//rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		//m_2DSprite.Draw(rc);

		////RENDERTARGET����PRESENT�ցB
		//rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}
}