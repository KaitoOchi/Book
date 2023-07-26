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
	Enemy();
	virtual ~Enemy()=0;

	bool Start();

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
	bool WallAndHit(Vector3 pos);
	/// <summary>
	/// ��]����
	/// </summary>
	/// <param name="rot">���g���������x�N�g��</param>
	void Rotation(Vector3 rot);
	/// <summary>
	/// �G�l�~�[�̏���p�^�[�����w��
	/// </summary>
	/// <param name="0">	�F�c				</param>
	/// <param name="1">	�F��				</param>
	/// <param name="2">	�F�E���(�����`)	</param>
	/// <param name="3">	�F�����(�����`)	</param>
	void SpecifyPath(int pathNumber);
	/// <summary>
	/// �i�r���b�V�����쐬���鏈��
	/// </summary>
	void CreateNavimesh(Vector3 pos);
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
	void Action_GoLocationListenSound(Vector3 tergetPosition);
	/// <summary>
	/// �s����~
	/// </summary>
	/// <param name="time">��~���鎞��</param>
	/// <param name="timerNumber">�g�p����^�C�}�[���w��</param>
	/// <returns></returns>
	bool Action_StopMove(float time,int timerNumber);
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

	// ----------------------------------------------------------

	// �G�l�~�[�̎��
	enum EnemyType
	{
		TYPE_NORMAL,	// �ʏ�
		TYPE_CHARGE,	// �ːi
		TYPE_SEARCH,	// ���G
		TYPE_CLEAR		// ����
	};
	/// <summary>
	/// ��ނ�ݒ肷��
	/// </summary>
	void SetEnemyType(EnemyType type)
	{
		m_enemyType = type;
	}

	// �G�l�~�[�̃A�j���[�V�����X�e�[�g
	// �p�������h���N���X�ŃA�j���[�V������ǂݍ��݁A�֐����ĂԂƍĐ�����܂��B
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
	
	// �A�j���[�V�����Đ��p�X�e�[�g
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
	/// �Đ�����A�j���[�V������ݒ肷��
	/// </summary>
	void SetEnemyAnimationState(const EnAnimationState state)
	{
		m_enAnimationState = state;
	}

	// �G�l�~�[�̍s���p�^�[��
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

	/// <summary>
	/// ���݂̍s���p�^�[����ݒ肷��
	/// </summary>
	/// <param name="state"></param>
	void SetEnemyActionState(const EnEnemyActionState state) {
		m_enActionState = state;
	}

	/// <summary>
	/// ���݂̍s���p�^�[�����擾����
	/// </summary>
	/// <returns></returns>
	const EnEnemyActionState GetEnemyActionState() const {
		return m_enActionState;
	}

	// ----------------------------------------------------------

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	void SetPosition(const Vector3& position) {
		m_position = position;
	}

	/// <summary>
	/// ���W���擾����
	/// </summary>
	const Vector3& GetPosition() const {
		return m_position;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	void SetRotation(const Quaternion& rotation) {
		m_rotation = rotation;
	}

	/// <summary>
	/// ��]���擾����
	/// </summary>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

	/// <summary>
	/// �X�P�[����ݒ肷��
	/// </summary>
	void SetScale(const Vector3& scale) {
		m_scale = scale;
	}

	/// <summary>
	/// �X�P�[�����擾����
	/// </summary>
	const Vector3& GetScale() const {
		return m_scale;
	}

	/// <summary>
	/// �v���C���[���m�ۂ������ǂ����Ԃ�
	/// </summary>
	/// <returns></returns>
	bool GetChachPlayerFlag() {
		return m_isChachPlayer;
	}

	/// <summary>
	/// �v���C���[��ǐՂ��Ă��邩�ǂ����Ԃ�
	/// </summary>
	/// <returns>true�Ȃ�ǐՂ��Ă���</returns>
	const bool GetTrackingPlayerFlag() const {
		return m_isTrackingPlayer;
	}

	/// <summary>
	/// �G�t�F�N�g�̕`������邩�ǂ����̃t���O��ݒ肷��
	/// </summary>
	/// <param name="flag">true�Ȃ�`�悵��</param>
	/// <param name="number">�z��ԍ�</param>
	void SetEffectDrawFlag(const bool flag,const int number) {
		m_existsEfectDraw[number] = flag;
	}

	/// <summary>
	/// �M���e�̔�e�t���O��ݒ�
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHitFlashBulletFlag(bool flag) {
		m_hitFlashBullet = flag;
	};

	/// <summary>
	/// �M���e�ɓ��������ǂ����̃t���O���擾����
	/// </summary>
	/// <returns>true�Ȃ瓖������</returns>
	const bool GetHitFlashBulletFlag() const {
		return m_hitFlashBullet;
	}

	/// <summary>
	/// �����e�̔�e�t���O��ݒ�
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHearedSoundBulletFlag(bool flag) {
		m_hearedSoundBullet = flag;
	};

	/// <summary>
	/// ���𕷂������ǂ����̃t���O���擾����
	/// </summary>
	/// <returns>true�Ȃ畷����</returns>
	const bool GetHearedSoundBulletFlag()const {
		return m_hearedSoundBullet;
	}

	/// <summary>
	/// �A�C�e���̍��W��n��
	/// </summary>
	/// <returns></returns>
	void SetItemPos(const Vector3& position) {
		m_itemPosition = position;
	}

	/// <summary>
	/// �`�悷�邩�ǂ������肷��
	/// </summary>
	/// <param name="flag">true�̂Ƃ��`�悵�Ȃ�</param>
	void SetNotDrawFlag(bool flag) {
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
	const bool GetNotDrawFlag() const {
		return m_canDraw;
	}

	//�G�l�~�[�̑O���������߂�
	const Vector3 GetFoward()const
	{
		return m_forward;
	}

	Vector3 m_foward=Vector3::AxisZ;

	/// <summary>
	/// ���������ǂ������߂�
	/// </summary>
	/// <param name="active">true�Ȃ瓮���Ȃ�</param>
	void SetActiveFlag(bool active)
	{
		m_canActive = active;
	}

	/// <summary>
	///  ���������ǂ����̎擾
	/// </summary>
	/// <param name="m_activeFlag">true�Ȃ瓮���Ȃ�</param>	
	bool GetActiveFlag()
	{
		return m_canActive;
	}

	// �X�|�b�g���C�g��n��
	SpotLight& GetSpotLight() {
		return m_spotLight;
	}

	/// <summary>
	/// �X�|�b�g���C�g�̔ԍ���������
	/// </summary>
	/// <returns></returns>
	void SetSpotLigNum(int number) {
		m_spotNum = number;
	}

	//--------------------------------------------

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


	//--------------------------------------------

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
	/// �^�C�}�[�����Z�b�g����
	/// </summary>
	/// <param name="number">���Z�b�g����^�C�}�[</param>
	void ReSetAddTimer(const int number) {
		m_addTimer[number] = 0.0f;
	}

	/// <summary>
	/// �G�t�F�N�g��`�悵�����ǂ����̃t���O��ݒ肷��
	/// </summary>
	/// <param name="number">�ݒ肷��t���O</param>
	/// <param name="flag">true�Ȃ�`�悵��</param>
	void SetEffectDrawFlag(const int number,const bool flag) {
		m_existsEfectDraw[number] = flag;
	}

	/// <summary>
	/// �W��������W��n��
	/// </summary>
	void SetGatherPosition(const Vector3 position) {
		m_gatherPosition = position;
	}

private:
	SphereCollider				m_sphereCollider;							// �X�t�B�A�R���C�_�[

	// �p�X�ړ��p�̃|�C���g�\����
	struct Point {
		Vector3					s_position;									// �|�C���g�̍��W
		int						s_number;									// �|�C���g�̔ԍ�
	};

	// �i�r���b�V��
	TknFile						m_tknFile;									// tkn�t�@�C��
	PhysicsStaticObject			m_bgObject;									// �ÓI�����I�u�W�F�N�g
	nsAI::NaviMesh				m_nvmMesh;									// �i�r���b�V��
	nsAI::Path					m_path;										// �p�X
	nsAI::PathFinding			m_pathFiding;								// �p�X��T��

	Gage*						m_gage = nullptr;							// �x���x�Q�[�W

protected:
	Game*						m_game = nullptr;							// �Q�[��
	PlayerManagement*			m_playerManagement = nullptr;				// �v���C���[�}�l�W�����g
	EffectEmitter*				m_Effect = nullptr;							// �G�t�F�N�g

	// �p�X
	std::vector<Point>			m_pointList;								// �|�C���g�\���̂̔z��
	Point*						m_point = nullptr;							// �|�C���g�\���̂̃|�C���^�A���݂̖ړI�n�ɂȂ�

	CharacterController			m_characterController;						// �L�����N�^�[�R���g���[���[

	Vector3						m_position = Vector3::Zero;					// �G�l�~�[�̍��W
	Vector3						m_forward = Vector3::AxisZ;					// �G�l�~�[�̑O����
	Vector3						m_scale = Vector3::One;						// �G�l�~�[�̃X�P�[��
	Vector3						m_chargeTergetPosition = Vector3::Zero;		// �ːi�p�B�v���C���[�̍��W
	Vector3						m_playerMissiongPosition = Vector3::Zero;	// �����������p�B�v���C���[�̍��W
	Vector3						m_sumPosition = Vector3::Zero;				// ���ړ�����
	Vector3						m_gatherPosition = Vector3::Zero;			// �W��������W
	Vector3						m_itemPosition = Vector3::Zero;				// �A�C�e���̍��W

	Quaternion					m_rotation = Quaternion::Identity;			// �G�l�~�[�̉�]
	ModelRender					m_enemyModelRender;							// �G�l�~�[���f��
	SpotLight					m_spotLight;								// �X�|�b�g���C�g

	AnimationClip				m_enAnimationClips[m_enAnimation_Num];		// �A�j���[�V�����X�e�[�g

	enum EnTimerState {
		m_TimerState_HitByaFlashbang,				// �M���e���󂯂��Ƃ��̍čs������
		m_TimerState_StayOnThePath,					// ���񎞂̃p�X�ɗ��܂鎞��
		m_TimerState_UntilTheCharge,				// �ːi���s���܂ł̑ҋ@����
		m_TimerState_MissingPlayer,					// �v���C���[�������������̑ҋ@����
		m_TimerState_HitByaSoundbang,				// �����e���g�p���ꂽ���̓��B����߂�܂ł̎���
		m_TimerState_Num
	};

	enum EnEffectState {
		m_EffectState_Star,							// ��
		m_EffectState_ExclamationPoint,				// �I(���Q��)
		m_EffectState_QuestionMark,					// �H(�^�╄)
		m_EffectState_Num
	};

private:
	enum EnPathState {
		m_PathState_VerticalMovement,				// �c�ړ�
		m_PathState_MoveSideways,					// ���ړ�
		m_PathState_ClockwiseRotation,				// �E���
	};

	// �Đ�����A�j���[�V�������w�肷��X�e�[�g
	EnAnimationState			m_enAnimationState;

	/// <summary>
	/// �G�l�~�[�̍s���p�^�[��
	/// </summary>
	/// <param name="CRAW">					�F����							</param>
	/// <param name="TRACKING">				�F�ǐ�							</param>
	/// <param name="SEARCH">				�F�ҋ@							</param>
	/// <param name="MISSING_MOVEPOSITON">	�F�����������W�܂ňړ�����		</param>
	/// <param name="MISSING_SEARCHPLAYER">	�F���������v���C���[��T��		</param>
	/// <param name="CALL">					�F����̓G���Ă�				</param>
	/// <param name="CALLED">				�FCALL����Search�ȊO�����s		</param>
	/// <param name="CHARGE">				�F�ːi							</param>
	/// <param name="BACKBASEDON">			�F�����Ԃɖ߂�				</param>
	/// <param name="CONFUSION">			�F�M���e�ɂ��������Ƃ�			</param>
	/// <param name="LISTEN">				�F�����e���g�p�����Ƃ�			</param>
	/// <param name="CATCH">				�F�ߊl							</param>
	EnEnemyActionState			m_enActionState;

	// �G�l�~�[�̎��
	EnemyType					m_enemyType;

	/// <summary>
	/// �G�t�F�N�g��`�悵�����ǂ����̃t���O�Btrue�̂Ƃ��`�悵��
	/// </summary>
	/// <param name="0">	�F���̃G�t�F�N�g	</param>
	/// <param name="1">	�F!�̃G�t�F�N�g		</param>
	/// <param name="2">	�F?�̃G�t�F�N�g		</param>
	std::array<bool, 3>			m_existsEfectDraw;

	/// <summary>
	/// </summary>
	/// <param name="0">	�F�M���e���󂯂��Ƃ��̍čs������	</param>
	/// <param name="1">	�F���񎞂̃p�X�ɗ��܂鎞��			</param>
	/// <param name="2">	�F�ːi���s���܂ł̑ҋ@����			</param>
	/// <param name="3">	�F�v���C���[�������������̑ҋ@����	</param>
	/// <param name="4">	�F�����e���g�p���ꂽ���̓��B����߂�܂ł̎���	</param>
	std::array<float, 5>		m_addTimer;

	bool						m_hitFlashBullet = false;			// �M���e�������������ǂ���
	bool						m_hearedSoundBullet = false;		// �����e�������������ǂ���
	bool						m_shouldCount = false;				// �����񐔂��J�E���g���邩�ǂ���
	bool						m_isTrackingPlayer = false;			// �v���C���[��ǐՂ��Ă��邩�ǂ���
	bool						m_isChachPlayer = false;			// �v���C���[���m�ۂ������ǂ���
	bool						m_canDraw = false;					// �`�悷�邩�ǂ���
	bool						m_canActive = false;				// �ŏ����瓮���邩������
	float						m_naviTimer = 0.0f;					// �i�r���b�V���p�̃^�C�}�[
	float						m_addVicount = 0.0f;				// �x���x�����񐔑��₷
	int							m_spotNum = 0;						// �X�|�b�g���C�g�̌�
};