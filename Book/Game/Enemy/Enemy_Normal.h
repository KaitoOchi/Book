#pragma once
#include "Enemy.h"
class Enemy_Normal:public Enemy
{
public:
	Enemy_Normal();
	~Enemy_Normal()override;

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// ����
	/// </summary>
	void Update_OnCraw();		
	/// <summary>
	/// �ǐ�
	/// </summary>
	void Update_OnTracking();
	/// <summary>
	/// Search�ɌĂ΂ꂽ���̏���s
	/// </summary>
	void Update_OnCalled();
	/// <summary>
	/// ���������ʒu�܂ňړ�����
	/// </summary>
	void Update_OnMoveMissingPosition();
	/// <summary>
	/// ���������v���C���[��T��
	/// </summary>
	void Update_OnSearchMissingPlayer();
	/// <summary>
	/// �����Ԃɖ߂�
	/// </summary>
	void Update_OnBackBasedOn();
	/// <summary>
	/// �M���e�ɓ��������Ƃ�
	/// </summary>
	void Update_OnDizzy();
	/// <summary>
	/// �����e���g�p�����Ƃ�
	/// </summary>
	void UpDate_OnListen();
};