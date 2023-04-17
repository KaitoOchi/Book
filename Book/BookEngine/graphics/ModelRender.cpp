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

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadow,
		bool isShadowReceiver,
		D3D12_CULL_MODE cullMode,
		int maxInstance)
	{
		//スケルトンを初期化。
		InitSkeleton(filePath);

		//アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		// モデルを初期化。
		InitModel(filePath, enModelUpAxis, isShadow, isShadowReceiver, cullMode);

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
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis,
		const bool isShadow,
		const bool isShadowReceiver,
		D3D12_CULL_MODE cullMode
	)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		modelInitData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetLightCB();
		modelInitData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetLightCB());
		modelInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
		modelInitData.m_cullMode = cullMode;

		if (isShadowReceiver) {
			modelInitData.m_fxFilePath = "Assets/shader/shadowReceiver.fx";
			modelInitData.m_expandShaderResoruceView[0] = &RenderingEngine::GetInstance()->GetShadowBlur().GetBokeTexture();
		}
		else {
			modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		}

		modelInitData.m_fxFilePath = "Assets/shader/shadowReceiver.fx";

		if (m_skeleton.IsInited()) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}

		m_model.Init(modelInitData);


		if (isShadow) {
			ModelInitData shadowModelInitData;

			//シャドウ用のモデルを初期化
			shadowModelInitData.m_fxFilePath = "Assets/shader/shadowMap.fx";
			shadowModelInitData.m_tkmFilePath = tkmFilePath;
			shadowModelInitData.m_modelUpAxis = modelUpAxis;
			shadowModelInitData.m_expandConstantBuffer = (void*)&RenderingEngine::GetInstance()->GetLightPos();
			shadowModelInitData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetLightPos());

			shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;

			if (m_skeleton.IsInited()) {
				//スケルトンを指定する。
				shadowModelInitData.m_skeleton = &m_skeleton;
				shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			}

			m_shadowModel.Init(shadowModelInitData);
		}
	}


	void ModelRender::UpdateWorldMatrixInModes()
	{
		if (m_model.IsInited()) {
			m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_shadowModel.IsInited()) {
			m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::Update()
	{
		UpdateWorldMatrixInModes();
		
		if (m_skeleton.IsInited()) {
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		RenderingEngine::GetInstance()->AddRenderObject(this);
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc, Camera& camera)
	{
		if (m_shadowModel.IsInited()) {
			m_shadowModel.Draw(rc, camera, 1);
		}
	}

	void ModelRender::OnForwardRender(RenderContext& rc)
	{
		if (m_model.IsInited()) {
			m_model.Draw(rc, 1);
		}
	}
}