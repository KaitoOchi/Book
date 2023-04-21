#pragma once
#include "Enemy.h"
class Enemy_Clear:public Enemy 
{
public:
	Enemy_Clear();
	~Enemy_Clear()override;
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);


	void Animation();					// �A�j���[�V����
	void Update_OnCraw();				// ����
	void Update_OnTracking();			// �ǐ�
	void Update_OnCalled();				// CALL���Ɏ��s
	void Update_OnBackBasedOn();		// �����Ԃɖ߂�
	void Update_OnConfusion();			// ����
	void Update_OnCatch();				// �ߊl

	/// <summary>
	/// ���������邩�ݒ�ł���
	/// </summary>
	/// <param name="cle">true�Ȃ瓧����false�Ȃ����</param>
	/// <returns></returns>
	const void SetClearFlag(const bool cle)
	{
		m_clearFlag = cle;
	}
	/// <summary>
	/// ���������Ă��邩���𓾂���
	/// </summary>
	/// <returns></returns>
	const bool GetClearFlag()const
	{
		return m_clearFlag;
	}
private:
	/// <summary>
	/// ��������؂�ւ���
	/// </summary>
	void ClearChange();


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

	//��������؂�ւ���
	bool m_clearFlag=true;							//true�Ȃ瓧����false�Ȃ����
	EnEnemyActState m_SetActState=m_ActState;		//�ǂ���̏�ԉ����L������

	float m_clearChangeTime = 0.0f;					//�؂�ւ��܂ł̎���

};

