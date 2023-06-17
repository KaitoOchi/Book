#pragma once
#include "Player.h"
class PlayerManagement;
class FlashBom;
class SoundBom;
class Player3D :public Player
{
public:
	Player3D();
	~Player3D()override;
	bool Start()override;
	void Update()override;
	void Animation()override;
	void Throw()override;
	void PlayerChang();
	void Render(RenderContext& rc)override;
	/// <summary>
	/// �L�����R���̍쐬�̊֐�
	/// </summary>
	void CreatCharcon();

	void SetPushPosition(const Vector3 pushpos)
	{
		m_pushPosition = pushpos;
	}
	const Vector3 GetPushPosition()const
	{
		return m_pushPosition;
	}

	const EnItemState GetItemState()
	{
		return m_enItemState;
	}

private:
	PlayerManagement* m_playerManagement = nullptr;
	FlashBom* m_flashBom = nullptr;
	SoundBom* m_soundBom = nullptr;
	

	
	bool m_isThrowing = false;

	//�������W�ɓ�����
	Vector3 m_pushPosition = Vector3::Zero;
	Vector3 m_pushRotPos = Vector3::Zero;

	float m_pushRot;
	float m_senkeiPos = 0.0f;
	float m_senkeiTime = 1.0f;

	float m_catchTime = 2.0f;

	
private:
	//�A�j���[�V�����C�x���g�p�̊֐�
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// �v���C���[�̉����o��
	/// </summary>
	void PlayerPush();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessIdleStateTransition()override;
	/// <summary>
	/// �ړ��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessWalkStateTransition()override;
	/// <summary>
	/// �_�b�V���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRunStateTransition()override;
	/// <summary>
	/// �W�����v�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessJumpStateTransition()override;
	/// <summary>
	/// �W�����v�I���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessJumpendStateTransition()override;
	/// <summary>
	/// �ύX�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessChangeStateTransition()override;
	/// <summary>
	/// ������X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessThrowStateTransition()override;
	/// <summary>
	/// ���ރX�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessStealStateTransition()override;

	/// <summary>
	/// ������X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessFoundStateTransition()override;

	/// <summary>
	/// �C��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessDownStateTransition()override;
	/// <summary>
	/// �߂܂�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessCaughtStateTransition()override;
	
	/// <summary>
	/// �߂܂�����̏���
	/// </summary>
	void ProcessCatchingStateTransition()override;


	/// <summary>
	/// �Q�[���N���A�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessClearStateTransition()override;
	/// <summary>
	/// �Q�[���I�[�o�[�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessGameOverStateTransition()override;
	
};

