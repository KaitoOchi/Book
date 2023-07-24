#pragma once
class PlayerManagement;
class GameCamera;
class Game;
class Treasure;  
class GameCamera;
class GameUI;
class Player : public IGameObject
{
public:
	enum EnPlayerState {
		m_enPlayer_Idle,		//�ҋ@��
		m_enPlayer_walk,		//����
		m_enPlayer_Run,			//����
		m_enPlayer_Jump,		//�W�����v
		m_enPlayer_Jumpend,		//�W�����v�I���
		m_enPlayer_Change,		//�ؑ�
		m_enPlayer_2DChanging,	//2D�ɐؑ֒�
		m_enPlayer_3DChanging,	//3D�ɐؑ֒�
		m_enPlayer_Found,		//������
		m_enPlayer_Caught,		//�߂܂���
		m_enPlayer_Catching,	//�߂܂��Ă���
		m_enPlayer_Down,		//�C�⒆
		m_enPlayer_DownEnd,		//�C�����
		m_enPlayer_Clear,		//�Q�[���N���A
		m_enPlayer_GameOver,	//�Q�[���I�[�o�[
		m_enPlayer3D_Throw,		//������
		m_enPlayer3D_Steal,		//����
		m_enPlayer3D_Push,		//����

	};

	EnPlayerState m_playerState = m_enPlayer_Idle;				//�ҋ@���

	//�A�C�e���̐ؑ�
	enum EnItemState
	{
		m_enItem_No,			//���������Ă��Ȃ�
		m_enItem_Flash,			//�M���e			
		m_enItem_SoundBom		//�����e
	};
	EnItemState m_enItemState = m_enItem_Flash;

	bool m_ghostHit = true;										//�ǂɖ��܂����������m����u���b�N�ɓ����������ǂ���

	bool m_Player_Act = true;									//true��������s���\false��������s���ł��Ȃ�

	std::vector<Vector3> m_playerPushPositions;					//�v���C���[�������o�������W
	
public:

	Player();
	virtual ~Player();
	virtual bool Start();
	virtual void Render(RenderContext& rc)=0;
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



	/// <summary>
	///�����|�W�V�����̐ݒ�
	/// </summary>
	/// <returns></returns>
	void SetGhostPosition(const Vector3 ghostpos)
	{
		m_setGhostpos = ghostpos;
	}

	const Vector3 GetGhostPosition()const
	{
		return m_setGhostpos;
	}

	/// <summary>
	/// �����u���b�N�̓����蔻��
	/// </summary>
	void GhostHit();
	
	
	void SetMoveSpeed(const Vector3 speed)
	{
		m_moveSpeed = speed;
	}

	const Vector3 GetMoveSpeed()
	{
		return m_moveSpeed;
	}

	/// <summary>
	/// �v���C���[�X�e�[�g�̕ύX
	/// </summary>
	/// <param name="state">�v���C���[�X�e�[�g</param>
	/// <returns></returns>
	const void SetPlayerState(EnPlayerState state)
	{
		m_playerState = state;
	}
	/// <summary>
	/// �v���C���[�X�e�[�g�̏��
	/// </summary>
	/// <returns></returns>
	const EnPlayerState GetPlayerState()
	{
		return m_playerState;
	}
	/// <summary>
	/// �X�^�~�i�̎擾
	/// </summary>
	/// <returns></returns>
	const float GetStamina()
	{
		return m_stamina;
	}
	/// <summary>
	/// �X�^�~�i�̐ݒ�
	/// </summary>
	/// <param name="stamina"></param>
	/// <returns></returns>
	const void SetStamina(const float stamina)
	{
		m_stamina = stamina;
	}

	/// <summary>
	/// ����邩�ǂ����̔���
	/// </summary>
	/// <param name="run">true�Ȃ瑖���</param>
	/// <returns></returns>
	const void SetRunState(const bool run)
	{
		m_runFlag = run;
	}

	/// <summary>
	/// ����邩�ǂ����̔���̎擾
	/// </summary>
	/// <returns></returns>
	const bool GetRunState()
	{
		return m_runFlag;
	}
	/// <summary>
	/// �v���C���[�̉�]�̐ݒ�
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �v���C���[�̉�]�̎擾
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	ModelRender* GetModel()
	{
		return m_modelRender;
	}

	/// <summary>
	/// ��ꂽ���̃G�t�F�N�g���擾
	/// </summary>
	/// <param name="m_effect"></param>
	/// <returns></returns>
	void SetTireEffect(EffectEmitter* m_effect)
	{
		m_tireEffect = m_effect;
	}
	/// <summary>
	/// ��ꂽ���̃G�t�F�N�g�̏����擾
	/// </summary>
	/// <returns></returns>
	EffectEmitter* GetTireEffect() const
	{
		return m_tireEffect;
	}

	/// <summary>
	/// ��]�p�x��ݒ�
	/// </summary>
	void Angle(const float angle)  
	{
		m_angle = angle;
	}

	/// <summary>
	/// ����邩�ǂ������肷��t���O���擾����
	/// </summary>
	const bool GetRunFlag() const 
	{
		return m_runFlag;
	}

protected:
	virtual void Update();
	void Move();
	void PlayerRun();
	void Jump();
	void Rotation();
	/// <summary>
	/// �߂܂�������
	/// </summary>
	void PlayerCatch();

	virtual void Throw()=0;
	virtual void Animation()=0;
	void Animation3D();
	void Animation2D();
	/// <summary>
	/// �A�C�e���̐ؑ�
	/// </summary>
	void ItemChange();

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
	virtual void ProcessIdleStateTransition()=0;
	
	/// <summary>
	/// �ړ��X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessWalkStateTransition()=0;
	
	/// <summary>
	/// �_�b�V���X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessRunStateTransition()=0;
	
	/// <summary>
	/// �W�����v�X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessJumpStateTransition()=0;
	
	/// <summary>
	/// �W�����v�I���X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessJumpendStateTransition()=0;
	
	/// <summary>
	/// �ύX�X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessChangeStateTransition()=0;

	/// <summary>
	/// ������X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessThrowStateTransition()=0;

	/// <summary>
	/// ���ރX�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessStealStateTransition()=0;

	/// <summary>
	/// ������X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessFoundStateTransition()=0;


	/// <summary>
	/// �C��X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessDownStateTransition()=0;

	/// <summary>
	/// �߂܂�X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessCaughtStateTransition()=0;

	/// <summary>
	/// �߂܂�����̏���
	/// </summary>
	virtual void ProcessCatchingStateTransition() = 0;

	/// <summary>
	/// �Q�[���N���A�X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessClearStateTransition()=0;

	/// <summary>
	/// �Q�[���I�[�o�[�X�e�[�g�̑J�ڏ���
	/// </summary>
	virtual void ProcessGameOverStateTransition()=0;

	/// <summary>
	/// ��ꂽ���̃G�t�F�N�g�̏���
	/// </summary>
	void TireEffect();

protected:
	//�A�j���[�V����
	enum EnAnimationClip {
		m_enAnimationClip_Idle,				//�ҋ@�A�j���[�V����
		m_enAnimationClip_Walk,				//�����A�j���[�V����
		m_enAnimationClip_Run,				//����A�j���[�V����
		m_enAnimationClip_Jump,				//�W�����v�A�j���[�V����
		m_enAnimationClip_Jumpend,			//�W�����v�I���A�j���[�V����
		m_enAnimationClip_Down,				//�_�E���A�j���[�V����
		m_enAnimationClip_DownEnd,			//�_�E���I��
		m_enAnimationClip_Throw,			//������A�j���[�V����
		m_enAnimationClip_CaughtStart,		//�߂܂������̃X�^�[�g�A�j���[�V����
		m_enAnimationClip_Caught,			//�߂܂������̃A�j���[�V����
		m_enAnimationClip_Num,				//�A�j���[�V�����̐�
	};
	AnimationClip		m_animationClips[m_enAnimationClip_Num];

	Vector3				m_moveSpeed=Vector3::Zero;				//�ړ����x
	Vector3				m_Lstic = Vector3::Zero;				//���X�e�b�N
	Vector3				m_position = Vector3::Zero;				//�������W
	Vector3				m_forward = Vector3::AxisZ;				//�v���C���[�̐��ʃx�N�g��
	
	ModelRender *		m_modelRender=nullptr;					//3D���f��
	std::array<nsK2EngineLow::Texture,14> m_player2D;			//�e�N�X�`��
	
	Quaternion			m_rotation;								//��]
	
	CharacterController*m_characon;								//�L�����R��
	CollisionObject*	m_collisionObject = nullptr;			//�R���W����

	GameCamera*			m_gamecamera=nullptr;					//�Q�[���J����
	PlayerManagement*	m_playerManagement=nullptr;				//�v���C���[�Ǘ�
	Treasure*			m_treasure = nullptr;
	Game*				m_game = nullptr;
	GameUI*				m_gameUI = nullptr;

	Vector3				m_ghostPosition = Vector3::Zero;
	Vector3				m_setGhostpos=Vector3::Zero;

private:
	EffectEmitter* m_tireEffect;

	float				m_downTime = 3.0f;						//�C�⎞��
	float				m_angle = 0.0f;							//��]�p�x

	bool				m_playerCaught = false;					//�߂܂������̔���
	bool				m_runFlag = true;						//����邩�̔���

	float				m_stamina = 0.0f;						//�v���C���[�̃X�^�~�i
	float				m_staminaCoolTime = 0.0f;				//�X�^�~�i���񕜂���܂ł̃N�[���^�C��
};	