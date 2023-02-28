#include "BookEnginePreCompile.h"
#include "ModelRender.h"
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
		//modelInitData.m_vsSkinEntryPointFunc = "VSMainUsePreComputedVertexBuffer";
		//modelInitData.m_vsEntryPointFunc = "VSMainUsePreComputedVertexBuffer";

		if (m_animationClips != nullptr) {
			// �A�j���[�V��������B
			modelInitData.m_vsSkinEntryPointFunc = "VSMainSkinUsePreComputedVertexBuffer";
		}
	}

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReceiver,
		int maxInstance,
		bool isFrontCullingOnDrawShadowMap)
	{
		// ZPrepass�`��p�̃��f�����������B
		InitModelOnZprepass(filePath, enModelUpAxis);

		// �e�탏�[���h�s����X�V����B
		UpdateWorldMatrixInModes();

		//�X�P���g�����������B
		InitSkeleton(filePath);

		//�A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
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

	void ModelRender::InitModelOnZprepass(
		//RenderingEngine& renderingEngine,
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis
	)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		modelInitData.m_modelUpAxis = modelUpAxis;

		// ���_�V�F�[�_�[�̃G���g���[�|�C���g���Z�b�g�A�b�v�B
		SetupVertexShaderEntryPointFunc(modelInitData);
		// ���_�̎��O�v�Z�������g���B
		//modelInitData.m_computedAnimationVertexBuffer = &m_computeAnimationVertexBuffer;

		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
		}

		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		m_zprepassModel.Init(modelInitData);
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{
		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

	void ModelRender::Update()
	{
		UpdateWorldMatrixInModes();
		
		if (m_skeleton.IsInited()) {
			m_skeleton.Update(m_zprepassModel.GetWorldMatrix());
		}

		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_zprepassModel.Draw(rc, 1);
	}
}