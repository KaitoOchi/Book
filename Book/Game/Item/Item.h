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
protected:

	Game* m_game=nullptr;
	PlayerManagement* m_playerManagement=nullptr;
	Player3D* m_player3D = nullptr;
	Vector3 EffecrRange = Vector3::Zero;

	

	std::vector<Enemy*> GetHitEnemyList()				//�A�C�e���͈͂̃G�l�~�[���擾����
	{
		return m_HitEnemyList;
	}

	std::vector<Enemy*> m_HitEnemyList;

};

