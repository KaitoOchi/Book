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
		1920,
		1080,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	ModelInitData boxModelInitData;
	boxModelInitData.m_tkmFilePath = "Assets/modelData/box.tkm";
	boxModelInitData.m_fxFilePath = "Assets/shader/model.fx";
	m_model .Init(boxModelInitData);
	m_model.UpdateWorldMatrix({ 0.0f, 100.0f, -100.0f }, g_quatIdentity, Vector3(1.0f, 1.0f, 1.0f));

	//箱モデルのテクスチャをオフスクリーンレンダリングされるテクスチャに切り替える
	m_model.ChangeAlbedoMap(
		"",
		offscreenRenderTarget.GetRenderTargetTexture()
	);

	return true;
}

void Mirror::Update()
{
}

void Mirror::Render(RenderContext& rc)
{
	//todo できない

	////レンダリングターゲットをoffscreenRenderTargetに変更する
	//RenderTarget* rtArray[] = { &offscreenRenderTarget };

	//rc.WaitUntilToPossibleSetRenderTargets(1, rtArray);

	//rc.SetRenderTargets(1, rtArray);uuuuuuuuuu

	//rc.ClearRenderTargetViews(1, rtArray);

	//m_debug->Render(rc);

	//rc.WaitUntilFinishDrawingToRenderTargets(1, rtArray);

	////画面に表示されるレンダリングターゲットに戻す
	//rc.SetRenderTarget(
	//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
	//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	//);

	//m_model.Draw(rc);
}