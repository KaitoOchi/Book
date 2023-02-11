#include "PreCompile.h"
#include "RenderingEngine.h"


namespace nsBookEngine {

	RenderingEngine::RenderingEngine()
	{

	}

	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Init(bool isSofShadow)
	{
		InitMainRenderTarget();
		InitGBuffer();
		InitMainRTSnapshotRenderTarget();
		InitCopyMainRenderTargetToFrameBufferSprite();
		Init2DRenderTarget();
	}

	void RenderingEngine::InitMainRTSnapshotRenderTarget()
	{
		for (auto& snapshotRt : m_mainRTSnapshots) {
			snapshotRt.Create(
				g_graphicsEngine->GetFrameBufferWidth(),
				g_graphicsEngine->GetFrameBufferHeight(),
				1,
				1,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_FORMAT_UNKNOWN
			);
		}
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			g_mainRenderTargetFormat.colorBufferFormat,
			g_mainRenderTargetFormat.depthBufferFormat
		);
	}

	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		// アルベドカラーを出力用のレンダリングターゲットを初期化する
		float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_gBuffer[enGBufferAlbedoDepth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		// 法線出力用のレンダリングターゲットを初期化する
		m_gBuffer[enGBufferNormal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_SNORM,
			DXGI_FORMAT_UNKNOWN
		);


		// メタリック、影パラメータ、スムース出力用のレンダリングターゲットを初期化する    
		m_gBuffer[enGBufferMetaricShadowSmooth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		// テクスチャはyBlurRenderTargetのカラーバッファー
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		// レンダリング先がフレームバッファーなので、解像度はフレームバッファーと同じ
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		// ガンマ補正ありの2D描画のシェーダーを指定する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// 初期化オブジェクトを使って、スプライトを初期化する
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);

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

		// 最終合成用のスプライトを初期化する
		SpriteInitData spriteInitData;
		//テクスチャは2Dレンダ―ターゲット。
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// 解像度はmainRenderTargetの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		//上書き。
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット。
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		//テクスチャはメインレンダ―ターゲット。
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		//解像度は2Dレンダ―ターゲットの幅と高さ
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//レンダリングターゲットのフォーマット。
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::CalcViewProjectionMatrixForViewCulling()
	{
		Matrix projMatrix;
		projMatrix.MakeProjectionMatrix(
			g_camera3D->GetViewAngle() * 1.5f,
			g_camera3D->GetAspect(),
			g_camera3D->GetNear(),
			g_camera3D->GetFar()
		);
		m_viewProjMatrixForViewCulling.Multiply(g_camera3D->GetViewMatrix(), projMatrix);
	}

	void RenderingEngine::Update()
	{
		// ビューカリング用のビュープロジェクション行列の計算。
		CalcViewProjectionMatrixForViewCulling();

		// シーンのジオメトリ情報の更新。
		m_sceneGeometryData.Update();
	}

	void RenderingEngine::ComputeAnimatedVertex(RenderContext& rc)
	{
		for (auto& renderObj : m_renderObjects) {
			renderObj->OnComputeVertex(rc);
		}
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//アニメーション済み頂点の計算
		ComputeAnimatedVertex(rc);

		// 不透明オブジェクトの描画が終わった時点でスナップショットを撮影する
		SnapshotMainRenderTarget(rc, EnMainRTSnapshot::enDrawnOpacity);

		// フォワードレンダリング
		ForwardRendering(rc);

		// ポストエフェクトを実行
		//m_postEffect.Render(rc, m_mainRenderTarget);

		// 2D描画
		Render2D(rc);

		//メインレンダリングターゲットの内容をフレームバッファにコピー
		CopyMainRenderTargetToFrameBuffer(rc);

		// 登録されている描画オブジェクトをクリア
		m_renderObjects.clear();
	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");

		//レンダリングターゲットとして利用できるまで待つ。
				//PRESENTからRENDERTARGETへ。
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRender2D(rc);
		}

		//RENDERTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);
		//PRESENTからRENDERTARGETへ。
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		m_2DSprite.Draw(rc);

		//RENDERTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		BeginGPUEvent("ForwardRendering");

		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer[enGBufferAlbedoDepth].GetRTVCpuDescriptorHandle()
		);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnForwardRender(rc);
		}

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		BeginGPUEvent("RenderToGBuffer");
		// レンダリングターゲットをG-Bufferに変更
		RenderTarget* rts[enGBufferNum] = {
			&m_gBuffer[enGBufferAlbedoDepth],         // 0番目のレンダリングターゲット
			&m_gBuffer[enGBufferNormal],              // 1番目のレンダリングターゲット
			&m_gBuffer[enGBufferMetaricShadowSmooth], // 2番目のレンダリングターゲット
		};

		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットを設定
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRenderToGBuffer(rc);
		}
		// レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
		EndGPUEvent();
	}

	void RenderingEngine::SnapshotMainRenderTarget(RenderContext& rc, EnMainRTSnapshot enSnapshot)
	{
		BeginGPUEvent("SnapshotMainRenderTarget");

		// メインレンダリングターゲットの内容をスナップショット
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRTSnapshots[(int)enSnapshot]);
		rc.SetRenderTargetAndViewport(m_mainRTSnapshots[(int)enSnapshot]);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRTSnapshots[(int)enSnapshot]);

		EndGPUEvent();
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		BeginGPUEvent("CopyMainRenderTargetToFrameBuffer");

		// メインレンダリングターゲットの絵をフレームバッファーにコピー
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// ビューポートを指定する
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