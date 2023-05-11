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
		const bool isShadow,
		const bool isShadowReceiver,
		const int outlineMode,
		D3D12_CULL_MODE cullMode,
		int maxInstance)
	{
		//スケルトンを初期化。
		InitSkeleton(filePath);

		//アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		// モデルを初期化。
		InitModel(filePath, enModelUpAxis, isShadow, isShadowReceiver, outlineMode, cullMode);

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
		const int outlineMode,
		D3D12_CULL_MODE cullMode
	)
	{
		//通常モデルを初期化
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		modelInitData.m_cullMode = cullMode;
		modelInitData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetLightCB();
		modelInitData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetLightCB());
		modelInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
		modelInitData.m_expandShaderResoruceView[0] = &RenderingEngine::GetInstance()->GetShadowBlur().GetBokeTexture();
		modelInitData.m_expandShaderResoruceView[1] = &RenderingEngine::GetInstance()->GetZPrepassDepthTexture();

		//影を設定
		if (isShadowReceiver) {
			modelInitData.m_psEntryPointFunc = "PSMainShadow";
		}
		else {
			modelInitData.m_psEntryPointFunc = "PSMain";
		}

		if (m_skeleton.IsInited()) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		
		//輪郭線を設定
		if (outlineMode == 1) {
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinPlayer";
		}
		else if (outlineMode == 2) {
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinEnemy";
		}
		else if (outlineMode == 3) {
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinEnemyClear";
		}

		m_model.Init(modelInitData);


		if (isShadow) {
			//シャドウ用のモデルを初期化
			ModelInitData shadowModelInitData;
			shadowModelInitData.m_fxFilePath = "Assets/shader/shadowMap.fx";
			shadowModelInitData.m_tkmFilePath = tkmFilePath;
			shadowModelInitData.m_modelUpAxis = modelUpAxis;
			shadowModelInitData.m_cullMode = cullMode;
			shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
			shadowModelInitData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetShadowCB();
			shadowModelInitData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetShadowCB());

			if (m_skeleton.IsInited()) {
				//スケルトンを指定する。
				shadowModelInitData.m_skeleton = &m_skeleton;
				shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			}

			m_shadowModel.Init(shadowModelInitData);
		}

		if (outlineMode != 0) {
			//ZPrepassモデルを初期化
			ModelInitData modelInitData;
			modelInitData.m_fxFilePath = "Assets/shader/zprepass.fx";
			modelInitData.m_tkmFilePath = tkmFilePath;
			modelInitData.m_modelUpAxis = modelUpAxis;
			modelInitData.m_cullMode = cullMode;
			modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;


			if (m_skeleton.IsInited()) {
				//スケルトンを指定する。
				modelInitData.m_skeleton = &m_skeleton;
				modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			}

			m_zprepassModel.Init(modelInitData);
		}
	}

	void ModelRender::InitModelData(ModelInitData& initData)
	{
		InitSkeleton(initData.m_tkmFilePath);

		m_model.Init(initData);

		UpdateWorldMatrixInModes();
	}


	void ModelRender::UpdateWorldMatrixInModes()
	{
		if (m_model.IsInited()) {
			m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_shadowModel.IsInited()) {
			m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_zprepassModel.IsInited()) {
			m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
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

	void ModelRender::OnRenderToZPrepass(RenderContext& rc)
	{
		if (m_zprepassModel.IsInited()) {
			m_zprepassModel.Draw(rc);
		}
	}

	void ModelRender::OnForwardRender(RenderContext& rc)
	{
		if (m_model.IsInited()) {
			m_model.Draw(rc, 1);
		}
	}
}