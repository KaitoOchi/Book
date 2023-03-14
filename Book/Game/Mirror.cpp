#include "stdafx.h"
#include "Mirror.h"

#include "Debug.h"


Mirror::Mirror()
{

}

Mirror::~Mirror()
{

}

bool Mirror::Start()
{
	m_debug = FindGO<Debug>("debug");

	offscreenRenderTarget.Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);

	m_modelRender.Init("Assets/modelData/player/player2D.tkm");
	m_modelRender.SetPosition(Vector3(-20.0f, 190.0f, -370.0f));
	m_modelRender.SetScale(Vector3::One);
	m_modelRender.Update();

	m_boxModelRender.Init("Assets/modelData/box.tkm");
	m_boxModelRender.SetPosition(Vector3(0.0f, 100.0f, -250.0f));
	Quaternion rot;
	rot.SetRotationDegX(90.0f);
	m_boxModelRender.SetRotation(rot);
	m_boxModelRender.SetScale(Vector3::One);
	m_boxModelRender.Update();

	//箱モデルのテクスチャをオフスクリーンレンダリングされるテクスチャに切り替える
	m_boxModelRender.GetModel().ChangeAlbedoMap(
		"",
		offscreenRenderTarget.GetRenderTargetTexture()
	);

	return true;
}

void Mirror::Update()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//レンダリングターゲットをoffscreenRenderTargetに変更する
	renderContext.WaitUntilToPossibleSetRenderTarget(offscreenRenderTarget);

	renderContext.SetRenderTarget(offscreenRenderTarget);

	const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	renderContext.ClearRenderTargetView(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), clearColor);

	m_modelRender.Draw(renderContext);
	m_debug->Render(renderContext);

	renderContext.WaitUntilFinishDrawingToRenderTarget(offscreenRenderTarget);

	//画面に表示されるレンダリングターゲットに戻す
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
}

void Mirror::Render(RenderContext& rc)
{
	//todo できない
	m_boxModelRender.Draw(rc);
	//m_modelRender.Draw(rc);
}