#pragma once

class PlayerManagement;
class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();

	bool SeachPlayer();					// �v���C���[�𔭌����鏈��
	bool CatchPlayer();					// �v���C���[���m�ۂ��鏈��
	void HitFlashBullet();				// �M���e�����������Ƃ��̏���
	void HitAfterFlashBullet();			// �M���e������������̏���

	// �G�l�~�[�̃A�j���[�V�����X�e�[�g
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
	enum EnEnemyActState
	{
		m_enEnemyActState_Craw,			// ����
		m_enEnemyActState_Tracking,		// �ǐ�
		m_enEnemyActState_Waiting,		// �ҋ@
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
		// ������n��
		b = HitFlashBulletFlag;
	};

	/// <summary>
	/// ���W���擾����
	/// </summary>
	const Vector3 GetPosition() const {
		return m_position;
	}

	const Vector3 GetScale() const {
		return m_scale;
	}

	const Quaternion GetRotation() const {
		return m_rotation;
	}

protected:
	PlayerManagement* m_playerManagement = nullptr;

	CharacterController m_characterController;
	FontRender m_fontRender;				// �t�H���g�����_�[

	Vector3 m_position = Vector3::Zero;		// �G�l�~�[�̍��W
	Vector3 m_forward = Vector3::AxisZ;		// �G�l�~�[�̑O����
	Vector3 m_scale = Vector3::One;			// �X�P�[��
	Quaternion m_rotation;					// ��]

	bool MissigPlayerFlag = false;			// �v���C���[�������������ǂ���
	bool HitFlashBulletFlag = false;		// �M���e�������������ǂ���
	bool ReturnToPassFlag = false;			// ���߂̃p�X�ɖ߂������ǂ���
};