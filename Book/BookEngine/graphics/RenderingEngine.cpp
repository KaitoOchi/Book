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
		SetDirectionLight(Vector3(1, -1, 1), Vector3(0.2f, 0.2f, 0.2f));

		//環境光の設定
		SetAmbient(0.1f);

		//半球光の設定
		SetHemiSphereLight(
			Vector3(0.1f, 0.1f, 0.2f),
			Vector3(0.2f, 0.2f, 0.3f),
			Vector3(0.0f, 1.0f, 0.0f)
		);

		m_lightCB.directionLig = m_directionLig.GetDirectionLig();
		m_lightCB.hemiSphereLig = m_hemiSphereLig.GetHemiSphereLig();



		//メインレンダーターゲットを設定
		float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//ブルームを設定
		SetBloomThreshold(20.0f);
		m_bloom.Init(m_mainRenderTarget);

		Init2DRenderTarget();

		InitShadowMapRenderTarget();

		InitZPrepassRenderTarget();

		InitViewPort();
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
		// カメラの位置を設定。これはライトの位置
		m_lightCamera.SetPosition(Vector3(g_camera3D->GetTarget().x + 50.0f, g_camera3D->GetTarget().y + 600.0f, g_camera3D->GetTarget().z + 50.0f));

		// カメラの注視点を設定。これがライトが照らしている場所
		m_lightCamera.SetTarget(Vector3(g_camera3D->GetTarget().x, g_camera3D->GetTarget().y - 100.0f, g_camera3D->GetTarget().z));

		// 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
		m_lightCamera.SetUp(0, 1, 0);

		//画角を設定
		m_lightCamera.SetViewAngle(g_camera3D->GetViewAngle());

		m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		m_lightCamera.Update();

		// ライトビュープロジェクション行列を計算している
		m_lightCB.shadowCB.mLVP = m_lightCamera.GetViewProjectionMatrix();
		m_lightCB.shadowCB.lightPos = m_lightCamera.GetPosition();

		//シャドウマップ用レンダーターゲットの初期化
		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_shadowMapRenderTarget.Create(
			8184,
			8184,
			1,
			1,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//シャドウマップ用のガウシアンブラーを初期化
		m_shadowBlur.Init(&m_shadowMapRenderTarget.GetRenderTargetTexture());
	}

	void RenderingEngine::InitZPrepassRenderTarget()
	{
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitViewPort()
	{
		//通常画面の描画
		m_viewPorts[0].Width = FRAME_BUFFER_W;   //画面の横サイズ
		m_viewPorts[0].Height = FRAME_BUFFER_H;   //画面の縦サイズ
		m_viewPorts[0].TopLeftX = 0;   //画面左上のx座標
		m_viewPorts[0].TopLeftY = 0;   //画面左上のy座標
		m_viewPorts[0].MinDepth = 0.0f;   //深度値の最小値
		m_viewPorts[0].MaxDepth = 1.0f;   //深度値の最大値
		
		//ワイプ画面の描画
		m_viewPorts[1].Width = 960 / 4 + 3;   //画面の横サイズ
		m_viewPorts[1].Height = 540 / 4 + 3;   //画面の縦サイズ
		m_viewPorts[1].TopLeftX = -290;   //画面左上のx座標
		m_viewPorts[1].TopLeftY = 662;   //画面左上のy座標
		m_viewPorts[1].MinDepth = 0.0f;   //深度値の最小値
		m_viewPorts[1].MaxDepth = 0.5f;   //深度値の最大値

		//ワイプカメラを初期化。
		m_wipeCamera.SetPosition({ 11200.0f, 400.0f, 1300.0f });
		m_wipeCamera.SetTarget({ 11200.0f, 50.0f, 1200.0f });
		m_wipeCamera.SetNear(150.0f);
		m_wipeCamera.SetFar(5000.0f);
		m_wipeCamera.Update();
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//視点の位置を設定する
		m_lightCB.directionLig.eyePos = g_camera3D->GetPosition();
		m_lightCB.directionLig.eyePos.x += 2000.0f;
		m_lightCB.directionLig.eyePos.y += 2000.0f;

		ZPrepass(rc);

		RenderShadowMap(rc);

		ForwardRendering(rc);

		m_bloom.Render(rc, m_mainRenderTarget);

		//ここでエフェクトドロー。
		EffectEngine::GetInstance()->Draw();

		Render2D(rc);

		m_renderObjects.clear();
	}

	void RenderingEngine::RenderShadowMap(RenderContext& rc)
	{
		// カメラの位置を設定
		m_lightCamera.SetPosition(Vector3(g_camera3D->GetTarget().x + 1024.0f, g_camera3D->GetTarget().y + 2192.0f, g_camera3D->GetTarget().z + 1024.0f));
		m_lightCamera.SetTarget(Vector3(g_camera3D->GetTarget().x + 0.0f, g_camera3D->GetTarget().y - 100.0f, g_camera3D->GetTarget().z + 0.0f));
		m_lightCamera.Update();

		m_lightCB.shadowCB.mLVP = m_lightCamera.GetViewProjectionMatrix();
		m_lightCB.shadowCB.lightPos = m_lightCamera.GetPosition();


		//シャドウマップ用のレンダーターゲットの書き込み待ち
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMapRenderTarget);
		rc.SetRenderTargetAndViewport(m_shadowMapRenderTarget);
		rc.ClearRenderTargetView(m_shadowMapRenderTarget);

		//描画処理
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRenderShadowMap(rc, m_lightCamera);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRenderTarget);

		m_shadowBlur.ExecuteOnGPU(rc, 5.0f);

	}

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		// レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRenderToZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		BeginGPUEvent("ForwardRendering");

		//メインレンダーターゲットの書き込み待ち
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.SetRenderTarget(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget);

		//ビューポートを設定
		rc.SetViewportAndScissor(m_viewPorts[0]);
		//通常描画処理
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnForwardRender(rc);
		}

		//ビューポートを設定
		rc.SetViewPort(m_viewPorts[1]);
		//ワイプ描画処理。
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnWipeForwardRender(rc, m_wipeCamera);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRender2D(rc);
		}
	}
}