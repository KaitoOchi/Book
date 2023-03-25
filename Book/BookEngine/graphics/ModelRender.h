#pragma once
//Programmer : KaitoOchi

#include "geometry/AABB.h"
#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"
//#include "geometry/geometryData.h"

namespace nsBookEngine {

	class ModelRender : public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// �����������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="animationClip">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClip">�A�j���[�V�����N���b�v�̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		/// <param name="isShadowReceiver">true�Ȃ�e�����������B</param>
		/// <param name="maxInstance">�C���X�^���X�̍ő吔�B�C���X�^���V���O�`��p�B</param>
		/// <param name="isFrontCullingOnDrawShadowMap">�V���h�E�}�b�v�`�掞�Ƀt�����g�J�����O��s�����ǂ����B</param>
		void Init(
			const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClip = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReceiver = true,
			int maxInstance = 1,
			bool isFrontCullingOnDrawShadowMap = false);

		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();

		/// <summary>
		/// �`�揈���B
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// �A�j���[�V������Đ��B
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">���`��Ԃ̎���</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ������ǂ����B
		/// </summary>
		/// <returns></returns>
		bool IsPlayingAniamtion()
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// ���f����擾�B
		/// </summary>
		/// <returns></returns>
		Model& GetModel()
		{
			return m_model;
		}

		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// ��]��ݒ�B
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// �g�嗦��ݒ�B
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// �V���h�E�L���X�^�[�̃t���O��ݒ肷��B
		/// </summary>
		/// <param name="flag"></param>
		void SetShadowCasterFlag(bool flag)
		{
			m_isShadowCaster = flag;
		}

		/// <summary>
		/// �A�j���[�V�����̑��x��ݒ肷��B
		/// </summary>
		/// <param name="animationSpeed"></param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// �{�[���̖��O����{�[���ԍ�������B
		/// </summary>
		/// <param name="boneName"></param>
		/// <returns></returns>
		int FindBoneID(const wchar_t* boneName)
		{
			return m_skeleton.FindBoneID(boneName);
		}

		/// <summary>
		/// �{�[���ԍ�����{�[����擾�B
		/// </summary>
		/// <param name="boneNo"></param>
		/// <returns></returns>
		Bone* GetBone(int boneNo)
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// �V���h�E�L���X�^�[�̃t���O�����邩�ǂ����B
		/// </summary>
		/// <returns></returns>
		bool IsShadowCaster()
		{
			return m_isShadowCaster;
		}

	private:
		/// <summary>
		/// �X�P���g���̏������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		void InitSkeleton(const char* filePath);


		/// <summary>
		/// �A�j���[�V�����̏������B
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// �e�탂�f���̃��[���h�s���X�V����B
		/// </summary>
		void UpdateWorldMatrixInModes();

		/// <summary>
		/// ���f����������B
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		void InitModel(
			//RenderingEngine& renderingEngine,
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);

		/// <summary>
		/// �e�탂�f���̒��_�V�F�[�_�[�̃G���g���[�|�C���g��ݒ�B
		/// </summary>
		void SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData);

	private:
		/// <summary>
		///�@�V���h�E�}�b�v�����_�[�p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderShadowMap(RenderContext& rc) override;
		/// <summary>
		/// �t�H���[�h�����_�[�p�X����Ă΂�鏈���B
		/// </summary>
		void OnForwardRender(RenderContext& rc) override;

	private:
		AnimationClip*	m_animationClips = nullptr;
		int				m_numAnimationClips = 0;

		Vector3			m_position = Vector3::Zero;
		Quaternion		m_rotation = Quaternion::Identity;
		Vector3			m_scale = Vector3::One;
		EnModelUpAxis	m_enFbxUpAxis = enModelUpAxisZ;
		Animation		m_animation;
		Model			m_model;
		Model			m_shadowModel;
		bool			m_isUpdateAnimation = true;
		Skeleton		m_skeleton;
		bool			m_isShadowCaster = true;
		float			m_animationSpeed = 1.0f;


	};

}
