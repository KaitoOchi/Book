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
		b = m_HitFlashBulletFlag;
	};

private:

	ModelRender m_modelRender;			// ���f�������_�[
	FontRender m_fontRender;			// �t�H���g�����_�[

	// �A�j���[�V�����N���b�v
	enum EnEnemyAnimationClip
	{
		enAnimationClip_Idle,			// �ҋ@
		enAnimationClip_Walk,			// ����
		enAnimationClip_Run,			// ����
		enAnimationClip_Attack,			// �U��
		enAnimationClip_Damege,			// ��e
		enAnimationClip_Num
	};

	AnimationClip m_animationClips[enAnimationClip_Num];

	Vector3 m_position;

	bool m_FoundPlayerFlag = false;		// �v���C���[�𔭌��������ǂ���
	bool m_CatchPlayerFlag = false;		// �v���C���[��߂܂������ǂ���
	bool m_MissigPlayerFlag = false;	// �v���C���[�������������ǂ���
	bool m_HitFlashBulletFlag = false;	// �M���e�������������ǂ���
	bool m_ReturnToPassFlag = false;	// ���߂̃p�X�ɖ߂������ǂ���

	float catchDecision = 10.0f;		// �߂܂����ƔF������͈�
};

