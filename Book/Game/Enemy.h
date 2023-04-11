#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class GameUI;
class Enemy :public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();

	bool Start();

	bool WallAndHit(Vector3 pos);		// �ǂƏՓ˂������ǂ����̏���
	void Rotation(Vector3 rot);			// ��]����
	void Act_Craw();					// ����s��
	void Act_Tracking();				// �ǐՍs��
	void Act_Access();					// �ڋߍs��
	void Act_Charge(float time);		// �ːi�s��
	void Act_Loss();					// ���������Ƃ��̏���
	void Act_Limit();					// ���ȓ��ɂ͋߂Â��Ȃ����߂̏���
	void Act_HitFlashBullet();			// �M���e�����������Ƃ��̏���
	bool Act_Stop(float time);			// �s����~
	bool Act_SeachPlayer();				// �v���C���[�𔭌����鏈��
	bool Act_CatchPlayer();				// �v���C���[���m�ۂ��鏈��
	void Act_Call();					// ����̓G���Ă�
	void SpotLight_New(Vector3 position);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	void VigilanceCount();				//


	enum EnemyType
	{
		Normal,
		Charge,
		Search
	};
	EnemyType m_enemyType;

	// �G�l�~�[�̃A�j���[�V�����X�e�[�g
	// �p�������h���N���X�ŃA�j���[�V������ǂݍ��݁A�֐����ĂԂƍĐ�����܂��B
	enum EnEnemyAnimationState
	{
		m_enEnemyAnimationState_Idle,	// �ҋ@
		m_enEnemyAnimationState_Walk,	// ����
		m_enEnemyAnimationState_Run,	// ����
		m_enEnemyAnimationState_Attack,	// �U��
		m_enEnemyAnimationState_Damege,	// ��e
	};
	// �A�j���[�V�����X�e�[�g
	EnEnemyAnimationState m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;

	// �G�l�~�[�̍s���p�^�[��
	enum EnEnemyActState
	{
		CRAW,			// ����
		TRACKING,		// �ǐ�
		SEARCH,			// ���G
		CHARGE,			// �ːi
		BACKBASEDON,	// �����Ԃɖ߂�
		CONFUSION,		// ����
		CATCH			// �ߊl
	};
	/// <summary>
	/// �G�l�~�[�̍s���p�^�[���Bswitch�ŊǗ����Ă�������
	/// </summary>
	/// <param name="CRAW">����</param>
	/// <param name="TRACKING">�ǐ�</param>
	/// <param name="SEARCH">�ҋ@</param>
	/// <param name="CHARGE">�ːi</param>
	/// <param name="BACKBASEDON">�����Ԃɖ߂�</param>
	/// <param name="CONFUSION">����</param>
	EnEnemyActState m_ActState = CRAW;

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
	/// ��e�t���O�ݒ�p�B�K�v�Ȃ���Ώ���
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHitFlashBullet(bool b) {
		b = HitFlashBulletFlag;
	};

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

protected:

	// �p�X�ړ��p�̃|�C���g�\����
	struct Point {
		Vector3 s_position;					// �|�C���g�̍��W
		int s_number;						// �|�C���g�̔ԍ�
	};

	TknFile m_tknFile;						// tkn�t�@�C��
	PhysicsStaticObject m_bgObject;			// �ÓI�����I�u�W�F�N�g
	nsAI::NaviMesh m_nvmMesh;				// �i�r���b�V��
	nsAI::Path m_path;						// �p�X
	nsAI::PathFinding m_pathFiding;			// �p�X��T��

	std::vector<Point> m_pointList;			// �|�C���g�\���̂̔z��
	Point* m_point = nullptr;				// �|�C���g�\���̂̃|�C���^�A���݂̖ړI�n�ɂȂ�

	PlayerManagement* m_playerManagement = nullptr;
	GameUI* m_gameUI = nullptr;

	CharacterController m_characterController;

	SphereCollider m_sphereCollider;

	FontRender m_fontRender;				// �t�H���g�����_�[

	Vector3 m_position = Vector3::Zero;		// �G�l�~�[�̍��W
	Vector3 m_forward = Vector3::AxisZ;		// �G�l�~�[�̑O����
	Vector3 m_scale = Vector3::One;			// �X�P�[��
	Quaternion m_rotation=Quaternion::Identity;	// ��]

	Vector3 m_playerPos = Vector3::Zero;	// �v���C���[�̍��W

	bool HitFlashBulletFlag = false;		// �M���e�������������ǂ���
	bool FindPlayerFlag = false;			
	bool CalculatedFlag = false;			// �ːi�p�t���O

	float addTimer = 0.0f;					// ���Z����^�C�}�[
	float NaviTimer = 0.0f;					// �i�r���b�V���p�̃^�C�}�[

	Vector3 playerPos = Vector3::Zero;		// �ːi�p
	Vector3 enemyPos = Vector3::Zero;
	Vector3 sumPos = Vector3::Zero;			// ���ړ�����

	ModelRender m_enemyRender;				//�G�l�~�[���f��
	SpotLight m_spotLight;					//�X�|�b�g���C�g

	float m_Vicount;						//�x���x�����񐔑��₷
};