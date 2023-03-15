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
		SetDirectionLight(Vector3(-1, -1, 1), Vector3(0.5f, 0.5f, 0.5f));

		//������ݒ�
		SetAmbient(0.1f);

		//�������C�g��ݒ�
		SetHemiSphereLight(
			Vector3(1.0f, 0.5f, 0.2f),
			Vector3(0.3f, 0.5f, 0.1f),
			Vector3(0.0f, 1.0f, 0.0f)
		);

		//�\���̂ɕۑ�
		m_lightCB.directionLig = m_directionLig.GetDirectionLig();
		m_lightCB.hemiSphereLig = m_hemiSphereLig.GetHemiSphereLig();

		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		SetBloomThreshold(1.25f);
		m_bloom.Init(m_mainRenderTarget);

		Init2DRenderTarget();
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

		// �ŏI�����p�̃X�v���C�g������������
		SpriteInitData spriteInitData;
		//�e�N�X�`����2D�����_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D�p�̃V�F�[�_�[���g�p����
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

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		// �e�N�X�`����yBlurRenderTarget�̃J���[�o�b�t�@�[
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		// �����_�����O�悪�t���[���o�b�t�@�[�Ȃ̂ŁA�𑜓x�̓t���[���o�b�t�@�[�Ɠ���
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		// �K���}�␳�����2D�`��̃V�F�[�_�[���w�肷��
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// �������I�u�W�F�N�g���g���āA�X�v���C�g������������
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);

	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		ForwardRendering(rc);

		m_bloom.Render(rc, m_mainRenderTarget);

		Render2D(rc);

		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[
		//CopyMainRenderTargetToFrameBuffer(rc);

		m_renderObjects.clear();
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

		//// �����_�����O�^�[�Q�b�g���N���A
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

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		BeginGPUEvent("CopyMainRenderTargetToFrameBuffer");

		// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// �r���[�|�[�g���w�肷��
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyMainRtToFrameBufferSprite.Draw(rc);

		EndGPUEvent();
	}
}