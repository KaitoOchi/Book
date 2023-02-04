#include "PreCompile.h"
#include "ModelRender.h"


namespace k2Engine {

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

	void ModelRender::Update()
	{
		if (m_isEnableInstancingDraw) {
			return;
		}
	}
}