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

	void Update_OnCraw();					// ����
	void Update_OnTracking();				// �ǐ�
	void Update_OnMoveMissingPosition();	// ���������ʒu�܂ňړ�����
	void Update_OnSearchMissingPlayer();	// ���������ʒu�܂ňړ�����
	void Update_OnCalled();					// CALL���Ɏ��s
	void Update_OnBackBasedOn();			// �����Ԃɖ߂�
	void Update_OnDizzy();					// �M���e�ɓ��������Ƃ�
	void UpDate_OnListen();					// �����e���g�p�����Ƃ�

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

	ModelRender			m_clearModelRender;							//���������̃��f��

	//��������؂�ւ���
	bool				m_clearFlag = true;							//true�Ȃ瓧����false�Ȃ����
	EnEnemyActionState	m_SetActionState = GetEnemyActionState();	//�ǂ���̏�ԉ����L������

	float				m_clearChangeTime = 0.0f;					//�؂�ւ��܂ł̎���
};

