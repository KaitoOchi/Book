#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class Gage;
class Game;
class Enemy :public IGameObject
{
public:
	Enemy();
	virtual ~Enemy()=0;

	bool Start();

	/// <summary>
	/// �A�j���[�V����
	/// </summary>
	void Animation();
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
	/// �i�r���b�V�����쐬����
	/// </summary>
	void Nav(Vector3 pos);
	/// <summary>
	/// ����s��
	/// </summary>
	void Act_Craw();
	/// <summary>
	/// �ǐՍs��
	/// </summary>
	void Act_Tracking();
	/// <summary>
	/// �ڋߍs��
	/// </summary>
	void Act_Access();
	/// <summary>
	/// �ːi�s��
	/// </summary>
	/// <param name="time">�ːi����܂ł̃`���[�W����</param>
	void Act_Charge(float time);
	/// <summary>
	/// �ːi������̍s������
	/// </summary>
	void Act_ChargeEnd();
	/// <summary>
	/// �ǂƂ̏Փ˔���
	/// </summary>
	void Act_Charge_HitWall();
	/// <summary>
	/// �G���Ăԍs��
	/// </summary>
	void Act_Call();
	/// <summary>
	/// �Ă΂ꂽ���̍s��
	/// </summary>
	void Act_Called();
	/// <summary>
	/// ����p���Ƀv���C���[�����݂��Ȃ��Ƃ��̍s��
	/// </summary>
	/// <returns></returns>
	bool Act_CallEnd();
	/// <summary>
	/// ���������Ƃ��̏���
	/// </summary>
	void Act_Loss();
	/// <summary>
	/// �M���e�����������Ƃ��̏���
	/// </summary>
	void Act_HitFlashBullet();
	/// <summary>
	/// �����e�����������Ƃ��̏���
	/// </summary>
	void Act_HitSoundBullet();
	/// <summary>
	/// �s����~
	/// </summary>
	/// <param name="time">��~���鎞��</param>
	/// <param name="i">�g�p����^�C�}�[���w��</param>
	/// <returns></returns>
	bool Act_Stop(float time,int i);
	/// <summary>
	/// �v���C���[�𔭌����鏈��
	/// </summary>
	/// <returns></returns>
	void Act_SeachPlayer();
	/// <summary>
	/// �v���C���[���m�ۂ��鏈��
	/// </summary>
	/// <returns></returns>
	bool Act_CatchPlayer();
	/// <summary>
	/// �v���C���[�������������̏���
	/// ���������ʒu�܂ňʒu���ړ�����
	/// </summary>
	void Act_MoveMissingPosition();
	/// <summary>
	/// �v���C���[�������������̏���
	/// �v���C���[��T��
	/// </summary>
	void Act_SearchMissingPlayer();

	void SpotLight_New(Vector3 position,int num);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	void VigilanceCount();				//

	void Efect_Dizzy();
	void Efect_FIndPlayer();
	void Efect_MissingPlayer();

	enum EnemyType
	{
		Normal,
		Charge,
		Search,
		Clear
	};
	EnemyType m_enemyType;

	// �G�l�~�[�̃A�j���[�V�����X�e�[�g
	// �p�������h���N���X�ŃA�j���[�V������ǂݍ��݁A�֐����ĂԂƍĐ�����܂��B
	enum EnAnimationClip
	{
		m_enAnimation_Idle,		// �ҋ@
		m_enAnimation_Walk,		// ����
		m_enAnimation_Run,		// ����
		m_enAnimation_Attack,	// �U��
		m_enAnimation_Damege,	// ��e
		m_enAnimation_Dizzy,	// �M���e���󂯂��Ƃ�
		m_enAnimation_Loss,		// �v���C���[������������
		m_enAnimation_Num
	};
	// �A�j���[�V�����X�e�[�g
	AnimationClip m_enAnimationClips[m_enAnimation_Num];
	
	// �A�j���[�V�����Đ��p�X�e�[�g
	enum EnAnimationState
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		DAMEGE,
		DIZZY,
		LOSS
	};
	EnAnimationState m_enAnimationState = IDLE;

	// �G�l�~�[�̍s���p�^�[��
	enum EnEnemyActState
	{
		CRAW,					// ����
		TRACKING,				// �ǐ�
		SEARCH,					// ���G
		MISSING_MOVEPOSITON,	// �����������W�܂ňړ�����
		MISSING_SEARCHPLAYER,	// ���������v���C���[��T��
		CALL,					// ����̓G���Ă�
		CALLED,					// CALL����Search�ȊO�����s
		CALLEND,				// ����p���Ƀv���C���[�����Ȃ��Ƃ�����̓G�����̈ʒu�ɖ߂�
		CHARGE,					// �ːi
		CHARGEEND,				// �ːi�I��
		BACKBASEDON,			// �����Ԃɖ߂�
		CONFUSION,				// �M���e�ɂ��������Ƃ�
		LISTEN,					// �����e���g�p�����Ƃ�
		CATCH					// �ߊl����
	};
	/// <summary>
	/// �G�l�~�[�̍s���p�^�[���Bswitch�ŊǗ����Ă�������
	/// </summary>
	/// <param name="CRAW">����</param>
	/// <param name="TRACKING">�ǐ�</param>
	/// <param name="SEARCH">�ҋ@</param>
	/// <param name="MISSING_MOVEPOSITON">�����������W�܂ňړ�����</param>
	/// <param name="CALL">����̓G���Ă�</param>
	/// <param name="CALLED">CALL����Search�ȊO�����s</param>
	/// <param name="CALLEND">����p���Ƀv���C���[�����݂��Ȃ��Ƃ����s</param>
	/// <param name="CHARGE">�ːi</param>
	/// <param name="BACKBASEDON">�����Ԃɖ߂�</param>
	/// <param name="CONFUSION">�M���e�ɂ��������Ƃ�</param>
	/// <param name="LISTEN">�����e���g�p�����Ƃ�</param>
	/// <param name="CATCH">�ߊl</param>
	EnEnemyActState m_ActState = CRAW;

	void SearchPass(EnEnemyActState state);

	// �w��ł���p�X�ړ�
	enum EnEnemyPassState
	{
		LINE_VERTICAL,					// �c
		LINE_HORIZONTAL,				// ��
		SQUARE_RIGHT,					// �E���(�����`)
		SQUARE_LEFT,					// �����(�����`)
		ANGLE_RIGHT,					// �E�ɒ��p
		ANGLE_LEFT,						// ���ɒ��p
		RECTANGLE_RIGHT,				// �E���(�����`)
		RECTANGLE_LEFT,					// �����(�����`)
	};
	EnEnemyPassState PassState;

	/// <summary>
	/// �G�l�~�[�̏���p�^�[�����w��
	/// </summary>
	/// <param name="0">�c</param>
	/// <param name="1">��</param>
	/// <param name="2">�E���(�����`)</param>
	/// <param name="3">�����(�����`)</param>
	/// <param name="4">�E�ɒ��p</param>
	/// <param name="6">�E�ɒ��p</param>
	/// <param name="7">�E���(�����`)</param>
	/// <param name="8">�����(�����`)</param>
	void Pass(int num);

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	void SetPosition(Vector3 pos) {
		m_position = pos;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	void SetRotation(Quaternion rot) {
		m_rotation = rot;
	}

	/// <summary>
	/// �X�P�[����ݒ肷��
	/// </summary>
	void SetScale(Vector3 sca) {
		m_scale = sca;
	}

	/// <summary>
	/// �v���C���[���m�ۂ������ǂ����Ԃ�
	/// </summary>
	/// <returns></returns>
	bool GetChachPlayerFlag() {
		return m_ChachPlayerFlag;
	}

	/// <summary>
	/// �M���e�̔�e�t���O��ݒ�
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHitFlashBullet(bool b) {
		m_HitFlashBulletFlag = b;
	};

	/// <summary>
	/// �M���e�ɓ����������ǂ����Ԃ�
	/// </summary>
	/// <returns>��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</returns>
	bool GetHitFlushBullet() {
		return m_HitFlashBulletFlag;
	}

	/// <summary>
	/// �����e�̔�e�t���O��ݒ�
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHitSoundBullet(bool b) {
		m_HitSoundBulletFlag = b;
	};

	/// <summary>
	/// �����e�ɓ����������ǂ����Ԃ�
	/// </summary>
	/// <returns>��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</returns>
	bool GetHitSoundBullet() {
		return m_HitSoundBulletFlag;
	}

	/// <summary>
	/// �A�C�e���̍��W��n��
	/// </summary>
	/// <returns></returns>
	void SetItemPos(Vector3 pos) {
		m_itemPos = pos;
	}

	/// <summary>
	/// ���W���擾����
	/// </summary>
	const Vector3& GetPosition() const {
		return m_position;
	}

	/// <summary>
	/// �X�P�[�����擾����
	/// </summary>
	const Vector3& GetScale() const {
		return m_scale;
	}

	/// <summary>
	/// ��]���擾����
	/// </summary>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

	// �X�|�b�g���C�g�̔ԍ���������
	void SetSpotLigNum(int num) {
		m_spotNum = num;
	}

	/// <summary>
	/// �`�悷�邩�ǂ������肷��Btrue�̂Ƃ��`�悵�Ȃ�
	/// </summary>
	/// <param name="flag"></param>
	void SetNotDrawFlag(bool flag) {
		m_NotDrawFlag = flag;
	}

	/// <summary>
	/// �s���p�^�[����߂�
	/// </summary>
	void SetTrueChangeDefaultFlag() {
		m_ChangeDefaultFlag = true;
	}

	//------------------------------------------
	//�G�l�~�[�̑O���������߂�
	const Vector3 GetFoward()const
	{
		return m_forward;
	}

	Vector3 m_foward=Vector3::AxisZ;
	//--------------------------------------------

protected:

	// �p�X�ړ��p�̃|�C���g�\����
	struct Point {
		Vector3 s_position;					// �|�C���g�̍��W
		int s_number;						// �|�C���g�̔ԍ�
	};

	std::vector<Point> m_pointList;			// �|�C���g�\���̂̔z��
	Point* m_point = nullptr;				// �|�C���g�\���̂̃|�C���^�A���݂̖ړI�n�ɂȂ�

	TknFile m_tknFile;						// tkn�t�@�C��
	PhysicsStaticObject m_bgObject;			// �ÓI�����I�u�W�F�N�g
	nsAI::NaviMesh m_nvmMesh;				// �i�r���b�V��
	nsAI::Path m_path;						// �p�X
	nsAI::PathFinding m_pathFiding;			// �p�X��T��

	PlayerManagement* m_playerManagement = nullptr;
	Gage* m_gage = nullptr;
	Game* m_game = nullptr;

	EffectEmitter* m_soundEffect = nullptr;		// �G�t�F�N�g

	CharacterController m_characterController;	// �L�����N�^�[�R���g���[���[
	SphereCollider m_sphereCollider;			// �X�t�B�A�R���C�_�[

	FontRender m_fontRender;				// �t�H���g�����_�[

	Vector3 m_position = Vector3::Zero;		// �G�l�~�[�̍��W
	Vector3 m_forward = Vector3::AxisZ;		// �G�l�~�[�̑O����
	Vector3 m_scale = Vector3::One;			// �X�P�[��
	Vector3 m_playerPos = Vector3::Zero;	// �v���C���[�̍��W
	Vector3 m_playerChargePosition = Vector3::Zero;			// �ːi�p�B�v���C���[�̍��W
	Vector3 m_playerMissionPosition = Vector3::Zero;	// �����������p�B�v���C���[�̍��W
	Vector3 m_sumPos = Vector3::Zero;		// ���ړ�����
	Vector3 m_setPos = Vector3::Zero;		// �W��������W
	Vector3 m_itemPos = Vector3::Zero;		// �A�C�e���̍��W

	Quaternion m_rotation = Quaternion::Identity;	// ��]

	ModelRender m_enemyRender;				//�G�l�~�[���f��
	SpotLight m_spotLight;					//�X�|�b�g���C�g

	bool m_HitFlashBulletFlag = false;		// �M���e�������������ǂ���
	bool m_HitSoundBulletFlag = false;		// �����e
	bool m_CountFlag = false;				// �J�E���g����t���O
	bool m_TrakingPlayerFlag = false;		// �v���C���[��ǂ�������t���O
	bool m_ChachPlayerFlag = false;			// �v���C���[���m�ۂ������ǂ���
	bool m_CalculatedFlag = false;			// �ːi�p�t���O�B��x�����Q�Ƃ��s��
	bool m_NotDrawFlag = false;				// �`�悷�邩�ǂ���
	bool m_ChangeDefaultFlag = false;		// �f�t�H���g�ɐ؂�ւ��邩�ǂ���

	std::array<bool,3>m_efectDrawFlag;				// �G�t�F�N�g�`��t���O

	/// <summary>
	/// 0���M���e�B1������B2���ːi�p�B3���v���C���[�������������̏���
	/// </summary>
	std::array<float, 4>m_addTimer;

	float m_NaviTimer = 0.0f;				// �i�r���b�V���p�̃^�C�}�[
	float m_move = 1.0f;
	float m_Vicount;						//�x���x�����񐔑��₷

	int m_spotNum = 0;						// �X�|�b�g���C�g�̌�
};