#pragma once


class Player : public IGameObject
{
public:
	enum EnPlayerState{
		m_enPlayer_Idle,//�ҋ@��
		m_enPlayer_walk,//����
		m_enPlayer_Run,//����
		m_enPlayer_Jump,//�W�����v
		m_enPlayer_Jumpend,//�W�����v�I���
		m_enPlayer_Change,//�ؑ�
	};
	Player();
	~Player();
	bool Start();
protected:
	void Update();
	void Move();
	void Jump();
	void Rotation();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �ړ��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// �_�b�V���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// �W�����v�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessJumpStateTransition();
	/// <summary>
	/// �W�����v�I���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessJumpendStateTransition();
	/// <summary>
	/// �ύX�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessChangeStateTransition();
	
	Vector3 m_moveSpeed=Vector3::Zero;//�ړ����x
	Vector3 m_Lstic = Vector3::Zero;//���X�e�b�N
	Vector3 m_position = Vector3(0.0f, 0.0f, 0.0f);//�������W
	Vector3 m_forward = Vector3::AxisZ;//�v���C���[�̐��ʃx�N�g��
	
	float m_walk = 20.0f;//�������̏�Z��
	float m_run = 40.0f;//���莞�̏�Z��
	float m_jumpvolume = 150.0f;//�W�����v��
	float angle=0;//��]�p�x
	
	Quaternion m_rotation;//��]
	CharacterController m_characon;//�L�����R��
	EnPlayerState m_playerState = m_enPlayer_Idle;//�ҋ@���
};