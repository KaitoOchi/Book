#include "BookEnginePreCompile.h"
#include "ModelRender.h"

#include "graphics/light/DirectionLight.h"

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
		const OutlineMode outlineMode,
		D3D12_CULL_MODE cullMode,
		const bool useWipe)
	{
		//�X�P���g�����������B
		InitSkeleton(filePath);

		//�A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		// ���f�����������B
		InitModel(filePath, enModelUpAxis, isShadow, isShadowReceiver, outlineMode, cullMode, useWipe);

		// �e�탏�[���h�s����X�V����B
		UpdateWorldMatrixInModes();
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//�X�P���g���f�[�^��ǂݍ���
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
		const OutlineMode outlineMode,
		D3D12_CULL_MODE cullMode,
		const bool useWipe
	)
	{
		//�ʏ탂�f����������
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

		//�e��ݒ�
		if (isShadowReceiver) {
			modelInitData.m_psEntryPointFunc = "PSMainShadow";
		}
		else {
			modelInitData.m_psEntryPointFunc = "PSMain";
		}

		//�A�j���[�V�����L�胂�f���Ȃ�
		if (m_skeleton.IsInited()) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		
		//�֊s����ݒ�
		switch (outlineMode)
		{
		case enOutlineMode_None:
			break;

		//�v���C���[���f���Ȃ�
		case enOutlineMode_3DPlayer:
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinPlayer";
			break;

		//�G���f���Ȃ�
		case enOutlineMode_Enemy:
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinEnemy";
			break;

		//�������G���f���Ȃ�
		case enOutlineMode_TranslucentEnemy:
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinEnemyClear";
			break;

		//2D�v���C���[�Ȃ�
		case enOutlineMode_2DPlayer:
			modelInitData.m_psEntryPointFunc = "PSPlayer2D";
			break;
		}

		//�ʏ탂�f���̏������B
		m_model.Init(modelInitData);

		//���C�v���g�p����Ȃ�B
		if (useWipe) {
			//���C�v�p���f���̏�����
			m_wipeModel.Init(modelInitData);
		}

		//�V���h�E���g�p����Ȃ�B
		if (isShadow) {
			//�V���h�E�p���f���̏�����
			InitShadowModel(tkmFilePath, modelUpAxis, outlineMode, cullMode);
		}

		//�A�E�g���C���𗘗p����Ȃ�B
		if (outlineMode != 0) {
			//ZPrepass�p���f���̏�����
			InitZPrepassModel(tkmFilePath, modelUpAxis, cullMode);
		}
	}

	void ModelRender::InitShadowModel(const char* tkmFilePath,
		EnModelUpAxis modelUpAxis,
		const OutlineMode outlineMode,
		D3D12_CULL_MODE cullMode)
	{
		ModelInitData shadowModelInitData;
		shadowModelInitData.m_fxFilePath = "Assets/shader/shadowMap.fx";
		shadowModelInitData.m_tkmFilePath = tkmFilePath;
		shadowModelInitData.m_modelUpAxis = modelUpAxis;
		shadowModelInitData.m_cullMode = cullMode;
		shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
		shadowModelInitData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetShadowCB();
		shadowModelInitData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetShadowCB());

		if (m_skeleton.IsInited()) {
			//�X�P���g�����w�肷��B
			shadowModelInitData.m_skeleton = &m_skeleton;
			shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}

		//2D�v���C���[�Ȃ�
		if (outlineMode == enOutlineMode_2DPlayer) {
			shadowModelInitData.m_psEntryPointFunc = "PSPlayer2D";
		}

		m_shadowModel.Init(shadowModelInitData);
	}

	void ModelRender::InitZPrepassModel(
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis,
		D3D12_CULL_MODE cullMode )
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/zprepass.fx";
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		modelInitData.m_cullMode = cullMode;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;


		if (m_skeleton.IsInited()) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}

		m_zprepassModel.Init(modelInitData);
	}

	void ModelRender::InitModelData(ModelInitData& initData)
	{
		InitSkeleton(initData.m_tkmFilePath);

		m_model.Init(initData);

		UpdateWorldMatrixInModes();
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{
		//���f���̍X�V����
		if (m_model.IsInited()) {
			m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_shadowModel.IsInited()) {
			m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_zprepassModel.IsInited()) {
			m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_wipeModel.IsInited()) {
			m_wipeModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::Update()
	{
		//���f���̃A�b�v�f�[�g
		UpdateWorldMatrixInModes();
		
		//�X�P���g���̃A�b�v�f�[�g
		if (m_skeleton.IsInited()) {
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//�A�j���[�V�����̃A�b�v�f�[�g
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

	void ModelRender::OnWipeForwardRender(RenderContext& rc, Camera& camera)
	{
		if (m_wipeModel.IsInited()) {
			m_wipeModel.Draw(rc, camera, 1);
		}
	}
}