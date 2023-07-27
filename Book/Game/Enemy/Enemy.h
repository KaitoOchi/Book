#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class Gage;
class Game;
class Treasure;
class Enemy :public IGameObject
{
public:
	/// <summary>
	/// �G�l�~�[�̎�ށB
	/// </summary>
	enum EnemyType
	{
		TYPE_NORMAL,	// �ʏ�B
		TYPE_CHARGE,	// �ːi�B
		TYPE_SEARCH,	// ���G�B
		TYPE_CLEAR		// �����B
	};

	/// <summary>
	/// �G�l�~�[�̍s���X�e�[�g�B
	/// </summary>
	enum EnEnemyActionState
	{
		m_ActionState_Craw,					// ����
		m_ActionState_Tracking,				// �ǐ�
		m_ActionState_Search,				// ���G
		m_ActionState_Move_MissingPositon,	// �����������W�܂ňړ�����
		m_ActionState_Search_MissingPlayer,	// ���������v���C���[��T��
		m_ActionState_Call_AroundEnemy,		// ����̓G���Ă�
		m_ActionState_Called,				// CALL����Search�ȊO�����s
		m_ActionState_Charge,				// �ːi
		m_ActionState_ChargeEnd,			// �ːi�I��
		m_ActionState_BackBasedOn,			// �����Ԃɖ߂�
		m_ActionState_Dizzy,				// �M���e�ɂ��������Ƃ�
		m_ActionState_Listen,				// �����e���g�p�����Ƃ�
		m_ActionState_CatchPlayer,			// �ߊl����
	};

protected:
	/// <summary>
	/// �A�j���[�V�����Đ��p�X�e�[�g�B
	/// </summary>
	enum EnAnimationState
	{
		m_enAnimationState_Idle,			// �ҋ@
		m_enAnimationState_Walk,			// ����
		m_enAnimationState_Run,				// ����
		m_enAnimationState_Attack,			// �U��
		m_enAnimationState_Damage,			// ��e
		m_enAnimationState_Dizzy,			// �M���e���󂯂��Ƃ�
		m_enAnimationState_Loss,			// �v���C���[������������
		m_enAnimationState_Call				// ���̃G�l�~�[���Ă�
	};

	/// <summary>
	/// �G�l�~�[�̃A�j���[�V�����B
	/// </summary>
	/// �p�������h���N���X�ŃA�j���[�V������ǂݍ��݁A�֐����ĂԂƍĐ�����܂��B
	enum EnAnimationClip
	{
		m_enAnimation_Idle,					// �ҋ@
		m_enAnimation_Walk,					// ����
		m_enAnimation_Run,					// ����
		m_enAnimation_Attack,				// �U��
		m_enAnimation_Damage,				// ��e
		m_enAnimation_Dizzy,				// �M���e���󂯂��Ƃ�
		m_enAnimation_Loss,					// �v���C���[������������
		m_enAnimation_Call,					// ���̃G�l�~�[���Ă�
		m_enAnimation_Num
	};

	/// <summary>
	/// �^�C�}�[�X�e�[�g�B
	/// </summary>
	enum EnTimerState
	{
		m_TimerState_HitByaFlashbang,		// �M���e���󂯂��Ƃ��̍čs������
		m_TimerState_StayOnThePath,			// ���񎞂̃p�X�ɗ��܂鎞��
		m_TimerState_UntilTheCharge,		// �ːi���s���܂ł̑ҋ@����
		m_TimerState_MissingPlayer,			// �v���C���[�������������̑ҋ@����
		m_TimerState_HitByaSoundbang,		// �����e���g�p���ꂽ���̓��B����߂�܂ł̎���
		m_TimerState_Num
	};

	/// <summary>
	/// �G�t�F�N�g�X�e�[�g�B
	/// </summary>
	enum EnEffectState
	{
		m_EffectState_Star,					// ��
		m_EffectState_ExclamationPoint,		// �I(���Q��)
		m_EffectState_QuestionMark,			// �H(�^�╄)
		m_EffectState_Num
	};

private:
	/// <summary>
	/// �p�X�ړ��p�̍\���́B
	/// </summary>
	struct Point
	{
		Vector3		s_position;				// �|�C���g�̍��W
		int			s_number;				// �|�C���g�̔ԍ�
	};

	/// <summary>
	/// �p�X�X�e�[�g�B
	/// </summary>
	enum EnPathState
	{
		m_PathState_VerticalMovement,		// �c�ړ�
		m_PathState_MoveSideways,			// ���ړ�
		m_PathState_ClockwiseRotation,		// �E���
	};

public:
	Enemy();
	virtual ~Enemy() = 0;
	bool Start();

public:
	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// ���W���擾����
	/// </summary>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// ��]���擾����
	/// </summary>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/// <summary>
	/// �X�P�[����ݒ肷��
	/// </summary>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// �X�P�[�����擾����
	/// </summary>
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/// <summary>
	/// �v���C���[���m�ۂ������ǂ����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetChachPlayerFlag() const
	{
		return m_isChachPlayer;
	}

	/// <summary>
	/// �v���C���[��ǐՂ��Ă��邩�ǂ����Ԃ�
	/// </summary>
	/// <returns>true�Ȃ�ǐՂ��Ă���</returns>
	const bool GetTrackingPlayerFlag() const
	{
		return m_isTrackingPlayer;
	}

	/// <summary>
	/// �M���e�̔�e�t���O��ݒ�
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHitFlashBulletFlag(const bool flag)
	{
		m_hitFlashBullet = flag;
	};

	/// <summary>
	/// �M���e�ɓ��������ǂ����̃t���O���擾����
	/// </summary>
	/// <returns>true�Ȃ瓖������</returns>
	const bool GetHitFlashBulletFlag() const
	{
		return m_hitFlashBullet;
	}

	/// <summary>
	/// �����e�̔�e�t���O��ݒ�
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHearedSoundBulletFlag(const bool flag)
	{
		m_hearedSoundBullet = flag;
	};

	/// <summary>
	/// ���𕷂������ǂ����̃t���O���擾����
	/// </summary>
	/// <returns>true�Ȃ畷����</returns>
	const bool GetHearedSoundBulletFlag() const
	{
		return m_hearedSoundBullet;
	}

	/// <summary>
	/// �A�C�e���̍��W��n��
	/// </summary>
	/// <returns></returns>
	void SetItemPos(const Vector3& position)
	{
		m_itemPosition = position;
	}

	/// <summary>
	/// �`�悷�邩�ǂ������肷��
	/// </summary>
	/// <param name="flag">true�̂Ƃ��`�悵�Ȃ�</param>
	void SetNotDrawFlag(const bool flag)
	{
		m_canDraw = flag;

		if (flag == false) {
			return;
		}

		// �t���O��false�ɂ���
		m_hitFlashBullet = false;
		m_hearedSoundBullet = false;
		m_isTrackingPlayer = false;
		m_isChachPlayer = false;

		// �X�e�[�g�������Ԃɖ߂�
		m_enActionState = m_ActionState_Craw;

		m_naviTimer = 0.0f;
		m_addTimer[m_TimerState_StayOnThePath] = 0.0f;
		m_addTimer[m_TimerState_MissingPlayer] = 0.0f;

		for (int i = 0; i < m_EffectState_Num; i++) {
			m_existsEfectDraw[i] = false;
		}
	}

	/// <summary>
	/// �`�悷�邩�ǂ����̃t���O���擾����
	/// </summary>
	/// <returns>true�Ȃ�`�悵�Ȃ�</returns>
	const bool GetNotDrawFlag() const
	{
		return m_canDraw;
	}

	/// <summary>
	/// �G�l�~�[�̑O���������߂�B
	/// </summary>
	/// <param name="state"></param>
	const Vector3 GetFoward() const
	{
		return m_forward;
	}

	/// <summary>
	/// ���������ǂ������߂�
	/// </summary>
	/// <param name="active">true�Ȃ瓮���Ȃ�</param>
	void SetActiveFlag(const bool active)
	{
		m_canActive = active;
	}

	/// <summary>
	///  ���������ǂ����̎擾
	/// </summary>
	/// <param name="m_activeFlag">true�Ȃ瓮���Ȃ�</param>	
	const bool GetActiveFlag() const
	{
		return m_canActive;
	}

	/// <summary>
	/// �X�|�b�g���C�g���擾�B
	/// </summary>
	SpotLight& GetSpotLight()
	{
		return m_spotLight;
	}

	/// <summary>
	/// �X�|�b�g���C�g�̔ԍ���������
	/// </summary>
	/// <returns></returns>
	void SetSpotLigNum(int number)
	{
		m_spotNum = number;
	}

	/// <summary>
	/// �L�����N�^�[�R���g���[���[���擾
	/// </summary>
	CharacterController& GetCharCon()
	{
		return m_characterController;
	}

	/// <summary>
	/// ���f�������_�[���擾����
	/// </summary>
	ModelRender& GetModelRender()
	{
		return m_enemyModelRender;
	}

	/// <summary>
	/// �G�t�F�N�g���擾�B
	/// </summary>
	/// <returns></returns>
	EffectEmitter* GetEffect()
	{
		if (m_Effect != nullptr) {
			return m_Effect;
		}
		return nullptr;
	}

	/// <summary>
	/// ���݂̍s���p�^�[�����擾����
	/// </summary>
	/// <returns></returns>
	const EnEnemyActionState GetEnemyActionState() const
	{
		return m_enActionState;
	}

	/// <summary>
	/// ��ނ�ݒ肷��
	/// </summary>
	void SetEnemyType(const EnemyType type)
	{
		m_enemyType = type;
	}

	/// <summary>
	/// �G�l�~�[�̏���p�^�[�����w��
	/// </summary>
	/// <param name="0">	�F�c				</param>
	/// <param name="1">	�F��				</param>
	/// <param name="2">	�F�E���(�����`)	</param>
	/// <param name="3">	�F�����(�����`)	</param>
	void SpecifyPath(int pathNumber);

	/// <summary>
	/// call���s���̏W��������W��n��
	/// </summary>
	void SetGatherPosition(const Vector3& position)
	{
		m_gatherPosition = position;
	}

	/// <summary>
	/// ���݂̍s���p�^�[����ݒ肷��
	/// </summary>
	/// <param name="state"></param>
	void SetEnemyActionState(const EnEnemyActionState state)
	{
		m_enActionState = state;
	}

protected:
	/// <summary>
	/// �G�t�F�N�g�̕`������邩�ǂ����̃t���O��ݒ肷��
	/// </summary>
	/// <param name="flag">true�Ȃ�`�悵��</param>
	/// <param name="number">�z��ԍ�</param>
	void SetEffectDrawFlag(const bool flag, const int number)
	{
		m_existsEfectDraw[number] = flag;
	}

	/// <summary>
	/// �^�C�}�[�����Z�b�g����
	/// </summary>
	/// <param name="number">���Z�b�g����^�C�}�[</param>
	void ReSetAddTimer(const int number)
	{
		m_addTimer[number] = 0.0f;
	}

	/// <summary>
	/// �G�t�F�N�g��`�悵�����ǂ����̃t���O��ݒ肷��
	/// </summary>
	/// <param name="number">�ݒ肷��t���O</param>
	/// <param name="flag">true�Ȃ�`�悵��</param>
	void SetEffectDrawFlag(const int number, const bool flag)
	{
		m_existsEfectDraw[number] = flag;
	}

	/// <summary>
	/// �Đ�����A�j���[�V������ݒ肷��
	/// </summary>
	void SetEnemyAnimationState(const EnAnimationState state)
	{
		m_enAnimationState = state;
	}

protected:
	/// <summary>
	/// �A�j���[�V����
	/// </summary>
	void LoadAnimation();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �ǂƏՓ˂������ǂ����̏���
	/// </summary>
	/// <param name="pos">���W</param>
	/// <returns></returns>
	bool WallAndHit(const Vector3& pos);
	/// <summary>
	/// ��]����
	/// </summary>
	/// <param name="rot">���g���������x�N�g��</param>
	void Rotation(const Vector3& rot);
	/// <summary>
	/// �i�r���b�V�����쐬���鏈��
	/// </summary>
	void CreateNavimesh(const Vector3& pos);
	/// <summary>
	/// ����s��
	/// </summary>
	void Action_CrawPath();
	/// <summary>
	/// �ǐՍs��
	/// </summary>
	void Action_TrackingPlayer();
	/// <summary>
	/// ���������Ƃ��̏���
	/// </summary>
	void Action_MissingPlayer();
	/// <summary>
	/// �v���C���[������������̏���
	/// </summary>
	void Action_MoveMissingPosition();
	/// <summary>
	/// �v���C���[������������̏���
	/// </summary>
	void Action_SearchMissingPlayer();
	/// <summary>
	/// �M���e�����������Ƃ��̏���
	/// </summary>
	void Action_HitFlashBullet();
	/// <summary>
	/// �������������ꏊ�ɍs������
	/// </summary>
	/// <param name="pos">�ڕW�n�_</param>
	void Action_GoLocationListenSound(const Vector3& tergetPosition);
	/// <summary>
	/// �s����~
	/// </summary>
	/// <param name="time">��~���鎞��</param>
	/// <param name="timerNumber">�g�p����^�C�}�[���w��</param>
	/// <returns></returns>
	bool Action_StopMove(const float time, const int timerNumber);
	/// <summary>
	/// �v���C���[�𔭌����鏈��
	/// </summary>
	/// <returns></returns>
	void Action_SeachPlayer();
	/// <summary>
	/// �v���C���[���m�ۂ��鏈��
	/// </summary>
	/// <returns></returns>
	bool Action_CatchPlayer();
	/// <summary>
	/// �X�|�b�g���C�g�𐶐�
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="num">�X�|�b�g���C�g�̔ԍ�</param>
	void SpotLight_New(Vector3 position,int num);
	/// <summary>
	/// ���G����
	/// </summary>
	/// <param name="lightrotaition">��]</param>
	/// <param name="lightpos">���W</param>
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	/// <summary>
	/// �x���x�̑�������
	/// </summary>
	void VigilanceCount();
	/// <summary>
	/// ���̃G�t�F�N�g�𐶐�
	/// </summary>
	void Efect_Dizzy();
	/// <summary>
	/// �I�̃G�t�F�N�g�𐶐�
	/// </summary>
	void Efect_FindPlayer();
	/// <summary>
	/// �H�̃G�t�F�N�g�𐶐�
	/// </summary>
	void Efect_MissingPlayer();

protected:
	ModelRender							m_enemyModelRender;							// �G�l�~�[���f��
	SpotLight							m_spotLight;								// �X�|�b�g���C�g
	CharacterController					m_characterController;						// �L�����N�^�[�R���g���[���[
	Game*								m_game = nullptr;							// �Q�[��
	PlayerManagement*					m_playerManagement = nullptr;				// �v���C���[�}�l�W�����g
	EffectEmitter*						m_Effect = nullptr;							// �G�t�F�N�g
	std::vector<Point>					m_pointList;								// �|�C���g�\���̂̔z��
	Point*								m_point = nullptr;							// �|�C���g�\���̂̃|�C���^�A���݂̖ړI�n�ɂȂ�
	Vector3								m_position = Vector3::Zero;					// �G�l�~�[�̍��W
	Vector3								m_forward = Vector3::AxisZ;					// �G�l�~�[�̑O����
	Vector3								m_scale = Vector3::One;						// �G�l�~�[�̃X�P�[��
	Vector3								m_chargeTergetPosition = Vector3::Zero;		// �ːi�p�B�v���C���[�̍��W
	Vector3								m_playerMissiongPosition = Vector3::Zero;	// �����������p�B�v���C���[�̍��W
	Vector3								m_sumPosition = Vector3::Zero;				// ���ړ�����
	Vector3								m_gatherPosition = Vector3::Zero;			// �W��������W
	Vector3								m_itemPosition = Vector3::Zero;				// �A�C�e���̍��W
	Quaternion							m_rotation = Quaternion::Identity;			// �G�l�~�[�̉�]
	AnimationClip						m_enAnimationClips[m_enAnimation_Num];		// �A�j���[�V�����X�e�[�g

private:
	SphereCollider						m_sphereCollider;							// �X�t�B�A�R���C�_�[
	TknFile								m_tknFile;									// tkn�t�@�C��
	PhysicsStaticObject					m_bgObject;									// �ÓI�����I�u�W�F�N�g
	nsAI::NaviMesh						m_nvmMesh;									// �i�r���b�V��
	nsAI::Path							m_path;										// �p�X
	nsAI::PathFinding					m_pathFiding;								// �p�X��T��
	Gage*								m_gage = nullptr;							// �x���x�Q�[�W
	EnAnimationState					m_enAnimationState;							// �Đ�����A�j���[�V�������w�肷��X�e�[�g
	EnEnemyActionState					m_enActionState;							// �G�l�~�[�̍s���p�^�[���X�e�[�g�B
	EnemyType							m_enemyType;								// �G�l�~�[�̎�ށB
	std::array<bool, m_EffectState_Num>	m_existsEfectDraw;							// �G�t�F�N�g��`�悵�����ǂ����̃t���O�Btrue�̂Ƃ��`�悵��
	std::array<float, m_TimerState_Num>	m_addTimer;									// �^�C�}�[�B
	bool								m_hitFlashBullet = false;					// �M���e�������������ǂ���
	bool								m_hearedSoundBullet = false;				// �����e�������������ǂ���
	bool								m_shouldCount = false;						// �����񐔂��J�E���g���邩�ǂ���
	bool								m_isTrackingPlayer = false;					// �v���C���[��ǐՂ��Ă��邩�ǂ���
	bool								m_isChachPlayer = false;					// �v���C���[���m�ۂ������ǂ���
	bool								m_canDraw = false;							// �`�悷�邩�ǂ���
	bool								m_canActive = false;						// �ŏ����瓮���邩������
	int									m_spotNum = 0;								// �X�|�b�g���C�g�̌�
	float								m_naviTimer = 0.0f;							// �i�r���b�V���p�̃^�C�}�[
	float								m_addVicount = 0.0f;						// �x���x�����񐔑��₷
};