#pragma once
#include "Player.h"
#include "Player2D.h"
#include"Player3D.h"
class Player2D;
class Player3D;
class PlayerManagement:public Player
{
public:
	PlayerManagement();
	~PlayerManagement();
	bool Start();
	void Update();
	void PlayerChange();
	/// <summary>
	/// �|�W�V�����̎擾
	/// </summary>
	const Vector3& GetPosition()const
	{
		// ���A�N�e�B�u�ȃv���C���[�̍��W��Ԃ�
		if (m_enMnanagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetPosition();
		}
		else {
			return m_player2D->GetPosition();
		}
	}
	//�L�����R���̎擾
	void SetCharacon(CharacterController* m_characon)
	{
		m_setChara = m_characon;
	}
	const CharacterController* GetCharacon()
	{
		return m_characon;
	}
	//�|�W�V�����̎擾
	void  SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();
private:
	enum EnMnagementState
	{
		m_enPlayer_2DChanging,//2D�ɐؑ֒�
		m_enPlayer_3DChanging,//3D�ɐؑ֒�
	};
	EnMnagementState m_enMnanagementState = m_enPlayer_3DChanging;//�RD���
	Vector3 m_position = Vector3::Zero;
	CharacterController* m_setChara=nullptr;
	Player2D* m_player2D = nullptr;
	Player3D* m_player3D = nullptr;
};

