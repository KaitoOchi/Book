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
	void CreateNavimesh(Vector3 pos);
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
	/// ���������Ƃ��̏���
	/// </summary>
	void Act_Loss();
	/// <summary>
	/// �M���e�����������Ƃ��̏���
	/// </summary>
	void Act_HitFlashBullet();
	/// <summary>
	/// �������������ꏊ�ɍs������
	/// </summary>
	/// <param name="pos">�ڕW�n�_</param>
	void Act_GoLocationListenSound(Vector3 pos);
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
	void Efect_FindPlayer();
	void Efect_MissingPlayer();

	// �G�l�~�[�̎��
	enum EnemyType
	{
		TYPE_NORMAL,
		TYPE_CHARGE,
		TYPE_SEARCH,
		TYPE_CLEAR
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
	/// <param name="CRAW">					�F����										</param>
	/// <param name="TRACKING">				�F�ǐ�										</param>
	/// <param name="SEARCH">				�F�ҋ@										</param>
	/// <param name="MISSING_MOVEPOSITON">	�F�����������W�܂ňړ�����					</param>
	/// <param name="MISSING_SEARCHPLAYER">	�F���������v���C���[��T��					</param>
	/// <param name="CALL">					�F����̓G���Ă�							</param>
	/// <param name="CALLED">				�FCALL����Search�ȊO�����s					</param>
	/// <param name="CHARGE">				�F�ːi										</param>
	/// <param name="BACKBASEDON">			�F�����Ԃɖ߂�							</param>
	/// <param name="CONFUSION">			�F�M���e�ɂ��������Ƃ�						</param>
	/// <param name="LISTEN">				�F�����e���g�p�����Ƃ�						</param>
	/// <param name="CATCH">				�F�ߊl										</param>
	EnEnemyActState m_ActState;

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
	/// <param name="0">	�F�c				</param>
	/// <param name="1">	�F��				</param>
	/// <param name="2">	�F�E���(�����`)	</param>
	/// <param name="3">	�F�����(�����`)	</param>
	/// <param name="4">	�F�E�ɒ��p		</param>
	/// <param name="6">	�F�E�ɒ��p		</param>
	/// <param name="7">	�F�E���(�����`)	</param>
	/// <param name="8">	�F�����(�����`)	</param>
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
	void SetHearedSoundBullet(bool b) {
		m_HearedSoundBulletFlag = b;
	};

	/// <summary>
	/// �����e�ɓ����������ǂ����Ԃ�
	/// </summary>
	/// <returns>��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</returns>
	bool GetHitSoundBullet() {
		return m_HearedSoundBulletFlag;
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

	const int GetSpotLigNum()
	{
		return m_spotNum;
	}

	/// <summary>
	/// �`�悷�邩�ǂ������肷��Btrue�̂Ƃ��`�悵�Ȃ�
	/// </summary>
	/// <param name="flag"></param>
	void SetNotDrawFlag(bool b) {
		m_NotDrawFlag = b;

		if (b == false) {
			return;
		}

		// �X�e�[�g�������Ԃɖ߂�
		m_ActState = CRAW;
		// �ǐՁE�ߊl�t���O��false�ɂ���
		m_TrackingPlayerFlag = false;
		m_ChachPlayerFlag = false;
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
		m_activeFlag = active;
	}

	const bool GetActiveFlag()
	{
		return m_activeFlag;
	}

	// �X�|�b�g���C�g��n��
	SpotLight GetSpotLight() {
		return m_spotLight;
	}

	//--------------------------------------------
	CharacterController& GetCharCon()
	{
		return m_characterController;
	}

	const int GetNumber()
	{
		return m_spotNum;
	}

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
	Vector3 m_playerMissionPosition = Vector3::Zero;		// �����������p�B�v���C���[�̍��W
	Vector3 m_sumPos = Vector3::Zero;		// ���ړ�����
	Vector3 m_setPos = Vector3::Zero;		// �W��������W
	Vector3 m_itemPos = Vector3::Zero;		// �A�C�e���̍��W
	Vector3 m_chargeDiff = Vector3::Zero;	// �ːi�̈ړ���


	Quaternion m_rotation = Quaternion::Identity;		// ��]

	ModelRender m_enemyRender;				//�G�l�~�[���f��
	SpotLight m_spotLight;					//�X�|�b�g���C�g

	bool m_HitFlashBulletFlag = false;		// �M���e�������������ǂ���
	bool m_HearedSoundBulletFlag = false;		// �����e
	bool m_CountFlag = false;				// �J�E���g����t���O
	bool m_TrackingPlayerFlag = false;		// �v���C���[��ǂ�������t���O
	bool m_ChachPlayerFlag = false;			// �v���C���[���m�ۂ������ǂ���
	bool m_CalculatedFlag = false;			// �ːi�p�t���O�B��x�����Q�Ƃ��s��
	bool m_NotDrawFlag = false;				// �`�悷�邩�ǂ���
	bool m_activeFlag = false;				//�ŏ����瓮���邩������

	/// <summary>
	/// �G�t�F�N�g��`�悵�����ǂ����̃t���O�Btrue�̂Ƃ��`�悵��
	/// </summary>
	/// <param name="0">	�F���̃G�t�F�N�g	</param>
	/// <param name="1">	�F!�̃G�t�F�N�g		</param>
	/// <param name="2">	�F?�̃G�t�F�N�g		</param>
	std::array<bool, 3>m_efectDrawFlag;

	/// <summary>
	/// </summary>
	/// <param name="0">	�F�M���e���󂯂��Ƃ��̍čs������	</param>
	/// <param name="1">	�F���񎞂̃p�X�ɗ��܂鎞��			</param>
	/// <param name="2">	�F�ːi���s���܂ł̑ҋ@����			</param>
	/// <param name="3">	�F�v���C���[�������������̑ҋ@����	</param>
	std::array<float, 4>m_addTimer;

	float m_NaviTimer = 0.0f;				// �i�r���b�V���p�̃^�C�}�[
	float m_move = 1.0f;
	float m_Vicount;						//�x���x�����񐔑��₷

	int m_spotNum = 0;						// �X�|�b�g���C�g�̌�
};