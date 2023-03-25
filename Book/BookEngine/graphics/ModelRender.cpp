#include "BookEnginePreCompile.h"
#include "ModelRender.h"

#include "graphics/light/DirectionLight.h"
//#include "RenderingEngine.h"


namespace nsBookEngine {

	ModelRender::ModelRender()
	{
	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		
		modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		modelInitData.m_vsEntryPointFunc = "VSMain";

		//if (m_animationClips != nullptr) {
		//	 //アニメーションあり。
		//	modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		//}
	}

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReceiver,
		int maxInstance,
		bool isFrontCullingOnDrawShadowMap)
	{
		//スケルトンを初期化。
		InitSkeleton(filePath);

		//アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		// モデルを初期化。
		InitModel(filePath, enModelUpAxis);

		// 各種ワールド行列を更新する。
		UpdateWorldMatrixInModes();
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンデータを読み込む
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;

		if (m_animationClips != nullptr) {
			m_animation.Init(m_skeleton,
				m_animationClips,
				numAnimationClips);
		}
	}

	void ModelRender::InitModel(
		//RenderingEngine& renderingEngine,
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis
	)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		modelInitData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetLightCB();
		modelInitData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetLightCB());
		modelInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

		// 頂点シェーダーのエントリーポイントをセットアップ。
		SetupVertexShaderEntryPointFunc(modelInitData);

		if (m_skeleton.IsInited()) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
		}

		//modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		m_model.Init(modelInitData);


		//シャドウ用のモデルを初期化
		ModelInitData shadowModelInitData;
		shadowModelInitData.m_fxFilePath = "Assets/shader/shadowMap.fx";
		shadowModelInitData.m_tkmFilePath = tkmFilePath;

		// 頂点シェーダーのエントリーポイントをセットアップ。
		SetupVertexShaderEntryPointFunc(shadowModelInitData);

		m_shadowModel.Init(shadowModelInitData);
		m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}


	void ModelRender::UpdateWorldMatrixInModes()
	{
		if (m_model.IsInited()) {
			m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::Update()
	{
		UpdateWorldMatrixInModes();
		
		if (m_skeleton.IsInited()) {

			if (m_model.IsInited()) {
				m_skeleton.Update(m_model.GetWorldMatrix());
			}
		}

		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		RenderingEngine::GetInstance()->AddRenderObject(this);
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc)
	{
		m_shadowModel.Draw(rc, 1);
	}

	void ModelRender::OnForwardRender(RenderContext& rc)
	{
		m_model.Draw(rc, 1);
	}
}