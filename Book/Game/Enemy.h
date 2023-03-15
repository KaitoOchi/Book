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
	bool HitFlashBullet();				// �M���e�����������Ƃ��̏���
	void HitAfterFlashBullet();			// �M���e������������̏���

	// �A�j���[�V�����N���b�v
	enum EnEnemyAnimationClip
	{
		m_enAnimationClip_Idle,			// �ҋ@
		m_enAnimationClip_Walk,			// ����
		m_enAnimationClip_Run,			// ����
		m_enAnimationClip_Attack,		// �U��
		m_enAnimationClip_Damege,		// ��e
		m_enAnimationClip_Num
	};
	// �A�j���[�V�����N���b�v
	AnimationClip m_animationClips[m_enAnimationClip_Num];


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

private:
	PlayerManagement* m_playerManagement = nullptr;

	CharacterController m_characterController;

	FontRender m_fontRender;			// �t�H���g�����_�[

	Vector3 m_position = Vector3::Zero;		// �G�l�~�[�̍��W
	Vector3 m_forward = Vector3::AxisZ;		// �G�l�~�[�̑O����
	bool MissigPlayerFlag = false;			// �v���C���[�������������ǂ���
	bool HitFlashBulletFlag = false;		// �M���e�������������ǂ���
	bool ReturnToPassFlag = false;			// ���߂̃p�X�ɖ߂������ǂ���
};