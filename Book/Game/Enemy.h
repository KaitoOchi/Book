#pragma once

class Player3D;
class Player2D;
class Enemy:public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void HeadToDestination();			// �ړI�n�Ɍ���������
	void SeachPlayer();					// �v���C���[�𔭌����鏈��
	void CatchPlayer();					// �v���C���[���m�ۂ��鏈��
	void HitFlashBullet();				// �M���e�����������Ƃ��̏���

	void Act();							// �s���p�^�[��
	void Animation();					// �A�j���[�V����

	/// <summary>
	/// ��e�t���O�ݒ�p�B�K�v�Ȃ���Ώ���
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHitFlashBullet(bool b) {
		// ������n��
		b = HitFlashBulletFlag;
	};

private:

	CharacterController m_characterController;

	ModelRender m_modelRender;			// ���f�������_�[
	FontRender m_fontRender;			// �t�H���g�����_�[

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
	EnEnemyAnimationState m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;

	// �G�l�~�[�̍s���X�e�[�g
	enum EnEnemyActState
	{
		m_enEnemyActState_Craw,			// ����
		m_enEnemyActState_Tracking		// �ǐ�
	};
	// �s���X�e�[�g
	EnEnemyActState m_enEnemyActState = m_enEnemyActState_Craw;

	Vector3 m_position = Vector3::Zero;		// �G�l�~�[�̍��W
	Vector3 m_float = Vector3::AxisZ;		// �G�l�~�[�̑O����

	bool FoundPlayerFlag = false;		// �v���C���[�𔭌��������ǂ���
	bool CatchPlayerFlag = false;		// �v���C���[��߂܂������ǂ���
	bool MissigPlayerFlag = false;		// �v���C���[�������������ǂ���
	bool HitFlashBulletFlag = false;	// �M���e�������������ǂ���
	bool ReturnToPassFlag = false;		// ���߂̃p�X�ɖ߂������ǂ���
};

