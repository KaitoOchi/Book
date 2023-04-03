#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class Enemy :public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();

	bool Start();

	bool WallAndHit(Vector3 pos);		// �ǂƏՓ˂������ǂ����̏���
	void Rotation();			// ��]����
	void Act_Craw();					// ����s��
	void Act_Tracking();				// �ǐՍs��
	void Act_Access();					// �ڋߍs��
	void Act_Loss();					// ���������Ƃ��̏���
	void Act_Limit();					// ���ȓ��ɂ͋߂Â��Ȃ����߂̏���
	void Act_HitFlashBullet();			// �M���e�����������Ƃ��̏���
	bool Act_Stop(float time);			// �s����~
	bool Act_SeachPlayer();				// �v���C���[�𔭌����鏈��
	bool Act_CatchPlayer();				// �v���C���[���m�ۂ��鏈��
	void SpotLight_New(Vector3 position);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);

	// �G�l�~�[�̃A�j���[�V�����X�e�[�g
	// Enemy���Ŏw�肵�Ă���̂ŁA�p�������h���N���X�Ŋ֐����ĂԂ����ōĐ�����܂��B
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

	// �G�l�~�[�̍s���X�e�[�g
	// �K�v�Ȃ���΍폜���Ă��������B
	enum EnEnemyActState
	{
		m_enEnemyActState_Craw,			// ����
		m_enEnemyActState_Tracking,		// �ǐ�
		m_enEnemyActState_Waiting,		// �ҋ@
		m_enEnemyActState_Confusion		// ����
	};
	// �s���X�e�[�g
	EnEnemyActState m_enEnemyActState = m_enEnemyActState_Craw;

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

	TknFile m_tknFile;
	PhysicsStaticObject m_bgObject;
	nsAI::NaviMesh m_nvmMesh;				// �i�r���b�V��
	nsAI::Path m_path;						// �p�X
	nsAI::PathFinding m_pathFiding;			// �p�X��T��

	std::vector<Point> m_pointList;			// �|�C���g�\���̂̔z��
	Point* m_point = nullptr;				// �|�C���g�\���̂̃|�C���^�A���݂̖ړI�n�ɂȂ�

	PlayerManagement* m_playerManagement = nullptr;

	CharacterController m_characterController;

	SphereCollider m_sphereCollider;

	FontRender m_fontRender;				// �t�H���g�����_�[

	Vector3 m_position = Vector3::Zero;		// �G�l�~�[�̍��W
	Vector3 m_forward = Vector3::AxisY;		// �G�l�~�[�̑O����
	Vector3 m_scale = Vector3::One;			// �X�P�[��
	Quaternion m_rotation;					// ��]

	Vector3 m_playerPos = Vector3::Zero;	// �v���C���[�̍��W

	bool HitFlashBulletFlag = false;		// �M���e�������������ǂ���

	float addTimer = 0.0f;					// ���Z����^�C�}�[
	float NaviTimer = 0.0f;					// �i�r���b�V���p�̃^�C�}�[

	ModelRender m_enemyRender;				//�G�l�~�[���f��
	SpotLight m_spotLight;					//�X�|�b�g���C�g
};