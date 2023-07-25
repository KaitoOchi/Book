#pragma once
#include "Enemy.h"
class Enemy_Search:public Enemy
{
public:
	Enemy_Search();
	~Enemy_Search()override;

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ��]����
	/// </summary>
	void Rotaition();
	/// <summary>
	/// �G���Ăԍs��
	/// </summary>
	void Action_CallAroundEnemy();
	/// <summary>
	/// ���G
	/// </summary>
	void Update_OnSearch();
	/// <summary>
	/// ����̓G���Ă�
	/// </summary>
	void Update_OnCallAroundEnemy();
	/// <summary>
	/// �M���e�ɓ��������Ƃ�
	/// </summary>
	void Update_OnDizzy();
	/// <summary>
	/// �v���C���[������������
	/// </summary>
	void Update_OnMissingPlayer();

private:
	bool	m_rotState = true;				// ��]�̌����̐ؑ�
	float	m_rotTime = 2.0f;				// ��]��؂�ւ���^�C�}�[
};