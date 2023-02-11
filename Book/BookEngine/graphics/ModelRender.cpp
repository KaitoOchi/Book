#include "PreCompile.h"
#include "ModelRender.h"
#include "RenderingEngine.h"


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
		bool isShadowReceiver,
		int maxInstance,
		bool isFrontCullingOnDrawShadowMap)
	{
		//�C���X�^���V���O�`��p�̃f�[�^���������B
		InitInstancingDraw(maxInstance);

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
	/*
	void ModelRender::InitInstancingDraw(int maxInstance)
	{
		m_maxInstance = maxInstance;

		if (m_maxInstance > 1) {
			//�C���X�^���X�`��p�̃f�[�^���\�z
			m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);

			//���[���h�s���GPU�ɓ]�����邽�߂̃X�g���N�`���[�h�o�b�t�@���m��
			m_worldMatricArraySB.Init(
				sizeof(Matrix),
				m_maxInstance,
				nullptr;
			);
			m_isEnableInstancingDraw = true;

			// �C���X�^���X�ԍ����烏�[���h�s��̔z��̃C���f�b�N�X�ɕϊ�����e�[�u��������������B
			m_instanceNoToWorldMatrixArrayIndexTable = std::make_unique<int[]>(m_maxInstance);
			for (int instanceNo = 0; instanceNo < m_maxInstance; instanceNo++) {
				m_instanceNoToWorldMatrixArrayIndexTable[instanceNo] = instanceNo;
			}
		}
	}
	*/

	void ModelRender::UpdateInstancingData(int instanceNo, const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(instanceNo < m_maxInstance, "�C���X�^���X�ԍ����s���ł��B");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		Matrix worldMatrix;
		if (m_translucentModel.IsInited()) {
			// ���������f����ZPrepass���f�������������Ă��Ȃ��̂ŁA��������g���B
			worldMatrix = m_translucentModel.CalcWorldMatrix(pos, rot, scale);
		}
		else {
			worldMatrix = m_zprepassModel.CalcWorldMatrix(pos, rot, scale);
		}
		// �C���X�^���X�ԍ�����s��̃C���f�b�N�X���擾����B
		int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];
		// �C���X�^���V���O�`����s���B
		m_worldMatrixArray[matrixArrayIndex] = worldMatrix;
		if (m_numInstance == 0) {
			//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
			// �X�P���g�����X�V�B
			// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
			// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
			// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
			m_skeleton.Update(g_matIdentity);
			//�A�j���[�V������i�߂�B
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{
		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		/*
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_forwardRenderModel.IsInited()) {
			m_forwardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_translucentModel.IsInited()) {
			m_translucentModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		for (auto& models : m_shadowModels) {
			for (auto& model : models) {
				if (model.IsInited()) {
					model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
				}
			}
		}
		*/
	}

	void ModelRender::Update()
	{
		if (m_isEnableInstancingDraw) {
			return;
		}

		UpdateWorldMatrixInModes();

		if (m_skeleton.IsInited()) {
			m_skeleton.Update(m_zprepassModel.GetWorldMatrix());
		}

		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc) {

		if (m_isEnableInstancingDraw) {

		}
		else {
			//�ʏ�`��
			//if (m_geometryDatas[0].IsInViewFrustum()) {
				//�r���[�t���X�^���Ɋ܂܂�Ă���B
			//	g_renderingEngine->AddRenderObject(this);
			//}
		}
	}
}