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

		//ディレクショナルライトの設定
		SetDirectionLight(Vector3(1, -1, 1), Vector3(0.5f, 0.5f, 0.5f));

		//環境光の設定
		SetAmbient(0.3f);

		//半球光の設定
		SetHemiSphereLight(
			Vector3(0.4f, 0.2f, 0.2f),
			Vector3(0.1f, 0.3f, 0.2f),
			Vector3(0.0f, 1.0f, 0.0f)
		);

		m_lightCB.directionLig = m_directionLig.GetDirectionLig();
		m_lightCB.hemiSphereLig = m_hemiSphereLig.GetHemiSphereLig();


		// カメラの位置を設定。これはライトの位置
		m_lightCamera.SetPosition(600, 800, 600);

		// カメラの注視点を設定。これがライトが照らしている場所
		m_lightCamera.SetTarget(0, 0, 0);

		// 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
		m_lightCamera.SetUp(1, -1, 1);

		m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		m_lightCamera.Update();

		// ライトビュープロジェクション行列を計算している
		m_lightCB.mLVP = m_lightCamera.GetViewProjectionMatrix();

		//メインレンダーターゲットを設定
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//ブルームを設定
		SetBloomThreshold(20.5f);
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

		//スプライトデータの初期化。
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
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
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//視点の位置を設定する
		m_lightCB.directionLig.eyePos = g_camera3D->GetPosition();

		RenderShadowMap(rc);

		ForwardRendering(rc);

		m_bloom.Render(rc, m_mainRenderTarget);

		Render2D(rc);

		m_renderObjects.clear();
	}

	void RenderingEngine::RenderShadowMap(RenderContext& rc)
	{
		// カメラの位置を設定
		m_lightCamera.SetPosition(Vector3(g_camera3D->GetPosition().x + 400.0f, g_camera3D->GetPosition().y + 200.0f, g_camera3D->GetPosition().z + 400.0f));
		m_lightCamera.SetTarget(g_camera3D->GetTarget());
		m_lightCamera.Update();
		m_lightCB.mLVP = m_lightCamera.GetViewProjectionMatrix();

		//シャドウマップ用のレンダーターゲットの書き込み待ち
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMapRenderTarget);
		rc.SetRenderTargetAndViewport(m_shadowMapRenderTarget);
		rc.ClearRenderTargetView(m_shadowMapRenderTarget);

		//描画処理
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRenderShadowMap(rc, m_lightCamera);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRenderTarget);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		BeginGPUEvent("ForwardRendering");

		//メインレンダーターゲットの書き込み待ち
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget);

		//描画処理
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnForwardRender(rc);
		}

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

		//描画処理
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