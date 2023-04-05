#pragma once
#include "Enemy.h"
class Enemy_Normal:public Enemy
{
public:
	Enemy_Normal();
	~Enemy_Normal();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Act();							// �G�l�~�[�̍s���p�^�[��
	void Animation();					// �A�j���[�V����

	/// <summary>
	/// �G�l�~�[�̏���p�^�[�����w��
	/// </summary>
	/// <param name="Line">�c�Ɉړ�</param>
	/// <param name="Horizontal">���Ɉړ�</param>
	/// <param name="RightRotation">�E���Ɉړ�</param>
	/// <param name="LeftRotation">�����Ɉړ�</param>
	/// <param name="RightAngle">���p�Ɉړ�</param>
	void Pass(int PassState);

private:

	Enemy* enemy;

	ModelRender m_NormalModelRender;	// ���f�������_�[

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

	// �w��ł���p�X�ړ�
	enum EnEnemyPassState
	{
		Line,			// �c
		Horizontal,		// ��
		RightRotation,	// �E���
		LeftRotation,	// �����
		RightAngle,		// ���p
	};

	EnEnemyPassState PassState;
};