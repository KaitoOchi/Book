#pragma once
class Enemy:public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void MoveState();					// �s���p�^�[���̏���
	void HeadToDestination();			// �ړI�n�Ɍ���������
	void Seach();						// ����p�̏���
	void CatchPlayer();					// �v���C���[���m�ۂ��鏈��
	void HitFlashBullet();				// �M���e�����������Ƃ��̏���

	// ��e�t���O�ݒ�p�B�K�v�Ȃ���Ώ���
	// true�̂Ƃ���e
	void SetHitFlashBullet(bool b) {
		// ������n��
		b = HitFlashBulletFlag;
	};

private:

	ModelRender m_modelRender;			// ���f�������_�[
	FontRender m_fontRender;			// �t�H���g�����_�[

	// �A�j���[�V�����N���b�v
	enum EnEnemyAnimationClip
	{
		enEnemyAnimationClip_Idle,		// �ҋ@
		enEnemyAnimationClip_Walk,		// ����
		enEnemyAnimationClip_Run,		// ����
		enEnemyAnimationClip_Attack,	// �U��
		enEnemyAnimationClip_Damege,	// ��e
		enEnemyAnimationClip_Num
	};

	AnimationClip m_animationClips[enEnemyAnimationClip_Num];

	Vector3 m_position;

	bool FoundPlayerFlag = false;		// �v���C���[�𔭌��������ǂ���
	bool CatchPlayerFlag = false;		// �v���C���[��߂܂������ǂ���
	bool MissigPlayerFlag = false;		// �v���C���[�������������ǂ���
	bool HitFlashBulletFlag = false;	// �M���e�������������ǂ���
	bool ReturnToPassFlag = false;		// ���߂̃p�X�ɖ߂������ǂ���
};

