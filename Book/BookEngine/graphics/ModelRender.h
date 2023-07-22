#pragma once
#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"

namespace nsBookEngine {

	/// <summary>
	/// ���f�������_�[�N���X�B
	/// </summary>
	class ModelRender : public IRenderer
	{
	public:
		enum OutlineMode
		{
			enOutlineMode_None,
			enOutlineMode_3DPlayer,
			enOutlineMode_Enemy,
			enOutlineMode_TranslucentEnemy,
			enOutlineMode_2DPlayer,
		};

	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// �����������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="animationClip">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClip">�A�j���[�V�����̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		/// <param name="isShadow">true�Ȃ�e��^����B</param>
		/// <param name="isShadowReceiver">true�Ȃ�e�𗎂Ƃ��B</param>
		/// <param name="outlineMode">1�Ńv���C���[�A2�œG�̗֊s��</param>
		/// <param name="isFrontCullingOnDrawShadowMap">�J�����O���[�h�B</param>
		/// <param name="useWipe">���C�v���f�����g�p����B</param>
		void Init(
			const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClip = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			const bool isShadow = false,
			const bool isShadowReceiver = false,
			const OutlineMode outlineMode = enOutlineMode_None,
			D3D12_CULL_MODE m_cullMode = D3D12_CULL_MODE_BACK,
			const bool useWipe = false
		);

		/// <summary>
		/// ModelInitData���g�p���������������B
		/// </summary>
		void InitModelData(ModelInitData& initData);

		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();

		/// <summary>
		/// �`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// �A�j���[�V�����̎��s�B
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����̔ԍ��B</param>
		/// <param name="interpolateTime">���s���x�B</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// �A�j���[�V�������Đ����Ă��邩�H
		/// </summary>
		/// <returns></returns>
		const bool IsPlayingAniamtion() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// ���f���̎擾�B
		/// </summary>
		/// <returns></returns>
		Model& GetModel()
		{
			return m_model;
		}
		Model& GetShadowModel()
		{
			return m_shadowModel;
		}

		/// <summary>
		/// ���W�̐ݒ�B
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// ��]�̐ݒ�B
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// �g�嗦�̐ݒ�B
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// �A�j���[�V�����C�x���g�̒ǉ��B
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// �A�j���[�V�����̑��x��ݒ�B
		/// </summary>
		/// <param name="animationSpeed"></param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// �{�[��ID�̎擾�B
		/// </summary>
		/// <param name="boneName"></param>
		/// <returns></returns>
		int FindBoneID(const wchar_t* boneName)
		{
			return m_skeleton.FindBoneID(boneName);
		}

		/// <summary>
		/// �{�[���̎擾�B
		/// </summary>
		/// <param name="boneNo"></param>
		/// <returns></returns>
		Bone* GetBone(int boneNo)
		{
			return m_skeleton.GetBone(boneNo);
		}

	private:
		/// <summary>
		/// �X�P���g���̏������B
		/// </summary>
		/// <param name="filePath">X</param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// �A�j���[�V�����̏������B
		/// </summary>
		/// <param name="animationClips"></param>
		/// <param name="numAnimationClips"></param>
		/// <param name="enModelUpAxis"></param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);

		/// <summary>
		/// ���f���̍X�V�����B
		/// </summary>
		void UpdateWorldMatrixInModes();

		/// <summary>
		/// ���f���̏������B
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		void InitModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			const bool isShadow,
			const bool isShadowReceiver,
			const OutlineMode outlineMode,
			D3D12_CULL_MODE cullMode,
			const bool useWipe
		);

		/// <summary>
		/// �V���h�E�p���f���̏������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		/// <param name="outlineMode">1�Ńv���C���[�A2�œG�̗֊s��</param>
		/// <param name="isFrontCullingOnDrawShadowMap">�J�����O���[�h�B</param>
		void InitShadowModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			const int outlineMode,
			D3D12_CULL_MODE cullMode
		);

		/// <summary>
		/// ZPrepass�p���f���̏������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		/// <param name="isFrontCullingOnDrawShadowMap">�J�����O���[�h�B</param>
		void InitZPrepassModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			D3D12_CULL_MODE cullMode
		);

	private:
		/// <summary>
		/// �V���h�E�}�b�v�̕`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderShadowMap(RenderContext& rc, Camera& camera) override;
		/// <summary>
		/// ZPrepass�̕`�揈���B
		/// </summary>
		void OnRenderToZPrepass(RenderContext& rc) override;
		/// <summary>
		/// �t�H���[�h�����_�����O�̕`�揈���B
		/// </summary>
		void OnForwardRender(RenderContext& rc) override;
		/// <summary>
		/// ���C�v�z���̃t�H���[�h�����_�����O�̕`�揈���B
		/// </summary>
		void OnWipeForwardRender(RenderContext& rc, Camera& camera) override;

	private:
		AnimationClip*	m_animationClips = nullptr;			//�A�j���[�V�����N���b�v
		int				m_numAnimationClips = 0;			//�A�j���[�V�����̐�
		EnModelUpAxis	m_enFbxUpAxis = enModelUpAxisZ;		/* ���f���̏���� */
		Animation		m_animation;						//�A�j���[�V����
		Skeleton		m_skeleton;							//�X�P���g��
		Model			m_model;							//�ʏ탂�f��
		Model			m_shadowModel;						//�V���h�E���f��
		Model			m_zprepassModel;					//ZPrepass���f��
		Model			m_wipeModel;						//���C�v�p���f��
		Vector3			m_position;							//���W
		Quaternion		m_rotation;							//��]
		Vector3			m_scale = Vector3::One;				//�g�嗦
		bool			m_isUpdateAnimation = true;			//�A�j���[�V�����̍X�V
		bool			m_isUseWipeCamera = false;			//���C�v�J�����Ɏʂ����ǂ���
		float			m_animationSpeed = 1.0f;			//�A�j���[�V�������x
	};
}
