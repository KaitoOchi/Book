#include "PreCompile.h"
#include "RenderingEngine.h"


namespace nsK2Engine {

	RenderingEngine::RenderingEngine()
	{

	}

	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Init(bool isSofShadow)
	{

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
	}
}