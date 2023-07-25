#pragma once
#include "Enemy.h"
class Enemy_Charge : public Enemy
{
public:
	Enemy_Charge();
	~Enemy_Charge()override;

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �ːi�s��
	/// </summary>
	/// <param name="time">�ːi����܂ł̃`���[�W����</param>
	void Action_ChargeStart(float time);
	/// <summary>
	/// �ːi������̍s������
	/// </summary>
	void Action_ChargeEnd();
	/// <summary>
	/// �ǂƂ̏Փ˔���
	/// </summary>
	void Action_ChargeHitWall();

	/// <summary>
	/// ����
	/// </summary>
	void Update_OnCraw();
	/// <summary>
	/// �ːi
	/// </summary>
	void Update_OnCharge();
	/// <summary>
	/// �ːi�I���̏���
	/// </summary>
	void Update_OnChargeEnd();
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
	/// �ߊl����
	/// </summary>
	void Update_OnCatch();

private:
	bool						m_CalculatedFlag = false;			// �ːi�p�t���O�B��x�����Q�Ƃ��s��
	float						m_Chargemove = 1.0f;				// �ːi�X�e�[�g���ɏ�Z���Ă���X�J���[
};