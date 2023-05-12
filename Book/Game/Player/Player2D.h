#pragma once
#include "Player.h"
class PlayerManagement;
class Player3D;
class Player2D:public Player
{
public:
	Player2D();
	~Player2D()override;
	bool Start()override;
	void Update()override;
	void PlayerChang();
	void Rotation2D();
	void Throw()override;
	/// <summary>
	/// �L�����R���̍쐬�̊֐�
	/// </summary>
	void CreatCharcon();
	void Animation()override;
	void Render(RenderContext& rc)override;
private:
	int i = 0;
	int j = 0;
	Vector3 m_InputRot=Vector3::Zero;
	Quaternion m_rot;
	PlayerManagement* m_playerManagement = nullptr;
	Player3D* m_player3D = nullptr;
private:
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

