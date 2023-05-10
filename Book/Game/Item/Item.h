#pragma once
class Game;
class Player3D;
class PlayerManagement;
class Enemy;
class Item :public IGameObject
{
public:
	Item();
	virtual ~Item();
	virtual bool Start();
	void Update();
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
	Vector3 m_position = Vector3::Zero;					//�A�C�e�����g�������W
	
	Game* m_game=nullptr;
	PlayerManagement* m_playerManagement=nullptr;
	Player3D* m_player3D = nullptr;
	Vector3 EffecrRange = Vector3::Zero;
	
	

	std::vector<Enemy*> GetHitEnemyList()				//�A�C�e���͈͂̃G�l�~�[���擾����
	{
		return m_HitEnemyList;
	}

	std::vector<Enemy*> m_HitEnemyList;
protected:
	/// <summary>
	/// �X�e�[�g�̑J��
	/// </summary>
	virtual void ManageState() = 0;
};

