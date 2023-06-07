#pragma once

class Game;
class Player3D;
class PlayerManagement;
class Enemy;
class GameUI;

/// <summary>
/// �A�C�e�����N���X�B
/// </summary>
class Item :public IGameObject
{
public:
	Item();
	virtual ~Item();
	virtual bool Start();
	void Update();

public:
	/// <summary>
	/// �A�C�e���������������ǂ���
	/// </summary>
	virtual void ItemHit() = 0;

	/// <summary>
	/// �A�C�e�����g�p�������W
	/// </summary>
	/// <param name="pos">�g�������̍��W</param>
	/// <returns></returns>
	const void SetItemPosition(const Vector3 pos)
	{
		m_position = pos;
	}

protected:
	/// <summary>
	/// �X�e�[�g�̑J��
	/// </summary>
	virtual void ManageState() = 0;

	/// <summary>
	/// �A�C�e���͈͓��̃G�l�~�[���擾�B
	/// </summary>
	std::vector<Enemy*> GetHitEnemyList()
	{
		return m_HitEnemyList;
	}

protected:	
	Game*					m_game = nullptr;				//�Q�[��
	PlayerManagement*		m_playerManagement = nullptr;	//�v���C���[�}�l�W�����g
	Player3D*				m_player3D = nullptr;			//�v���C���[
	GameUI*					m_gameUI = nullptr;				//�Q�[��UI
	std::vector< Enemy* >	m_HitEnemyList;					//���������G�l�~�[�̃��X�g
	Vector3					m_position;						//�A�C�e�����g�������W
	Vector3					EffecrRange;					//�G�t�F�N�g�͈�
};

