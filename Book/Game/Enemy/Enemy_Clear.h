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

	/// <summary>
	/// ����
	/// </summary>
	void Update_OnCraw();
	/// <summary>
	/// �ǐ�
	/// </summary>
	void Update_OnTracking();
	/// <summary>
	/// ���������ʒu�܂ňړ�����
	/// </summary>
	void Update_OnMoveMissingPosition();
	/// <summary>
	/// ���������v���C���[��T��
	/// </summary>
	void Update_OnSearchMissingPlayer();
	/// <summary>
	/// Search�ɌĂ΂ꂽ���̏���
	/// </summary>
	void Update_OnCalled();
	/// <summary>
	/// �����Ԃɖ߂�
	/// </summary>
	void Update_OnBackBasedOn();
	/// <summary>
	/// �M���e�ɓ��������Ƃ��̏���
	/// </summary>
	void Update_OnDizzy();
	/// <summary>
	/// �����e���g�p���ꂽ���̏���
	/// </summary>
	void UpDate_OnListen();

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
	EnEnemyActionState	m_setActionState = GetEnemyActionState();	//���݂̏�Ԃ��L�^����

	bool				m_clearFlag = true;							//true�Ȃ瓧����false�Ȃ����
	float				m_clearChangeTime = 0.0f;					//�؂�ւ��܂ł̎���
};
