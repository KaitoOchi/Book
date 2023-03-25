#pragma once
class PlayerManagement;
class GameCamera;
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
		m_enPlayer_2DChanging,//2D�ɐؑ֒�
		m_enPlayer_3DChanging,//3D�ɐؑ֒�
		m_enPlayer_Found,//������
		m_enPlayer_Caught,//�߂܂���
		m_enPlayer_Clear,//�Q�[���N���A
		m_enPlayer_GameOver,//�Q�[���I�[�o�[
		m_enPlayer3D_Throw,//������
		m_enPlayer3D_Steal,//����
		m_enPlayer3D_Push,//����

	};
	Player();
	virtual ~Player();
	virtual bool Start();
	//���W�̎擾
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	//���W�̎擾
	void SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}
	// �O������X�V������
	void ModelRenderUpdate() {
		m_modelRender->SetPosition(m_position);
		m_modelRender->Update();
	}

	//�L�����R���̎擾
	CharacterController* GetCharacon()
	{
		return m_characon;
	}

	//�R���W�����̎擾
	CollisionObject* GetCollision()
	{
		return m_collisionObject;
	}

protected:
	virtual void Update();
	void Move();
	void Jump();
	void Rotation();
	virtual void Animation();
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
	/// <summary>
	/// 2D�ɐؑ֒��X�e�[�g�̑J�ڏ���
	/// </summary>
	void Process2DChangingStateTransition();
	/// <summary>
	/// 3D�ɐؑ֒��X�e�[�g�̑J�ڏ���
	/// </summary>
	void Process3DChangingStateTransition();
	/// <summary>
	/// ������X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessThrowStateTransition();
	/// <summary>
	/// ���ރX�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessStealStateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessPushStateTransition();
	/// <summary>
	/// ������X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessFoundStateTransition();
	/// <summary>
	/// �߂܂�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessCaughtStateTransition();
	/// <summary>
	/// �Q�[���N���A�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessClearStateTransition();
	/// <summary>
	/// �Q�[���I�[�o�[�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessGameOverStateTransition();
	
	bool m_characonState = true;								//�L�����R������邩�ǂ���

	Vector3 m_moveSpeed=Vector3::Zero;							//�ړ����x
	Vector3 m_Lstic = Vector3::Zero;							//���X�e�b�N
	Vector3 m_position = Vector3(0.0f, 0.0f, 0.0f);				//�������W
	Vector3 m_forward = Vector3::AxisZ;							//�v���C���[�̐��ʃx�N�g��
	
	float angle=0;												//��]�p�x
	
	ModelRender *m_modelRender=nullptr;							//3D���f��
	nsK2EngineLow::Texture m_player2D[14];						//�e�N�X�`��
	Quaternion m_rotation;										//��]
	CharacterController *m_characon;							//�L�����R��
	EnPlayerState m_playerState = m_enPlayer_Idle;				//�ҋ@���
	CollisionObject* m_collisionObject = nullptr;							//�R���W����

	GameCamera* gamecamera=nullptr;								//�Q�[���J����
	PlayerManagement* m_playerManagement=nullptr;				//�v���C���[�Ǘ�
};