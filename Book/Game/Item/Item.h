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
	virtual void Update();
	/// <summary>
	/// �A�C�e���̐ؑ�
	/// </summary>
	void ItemChange();
	/// <summary>
	/// �A�C�e���͈̔�
	/// </summary>
	void ItemRange();
	virtual void ItemEffect() = 0;
protected:
	enum EnItemState
	{
		m_enItem_No,			//���������Ă��Ȃ�
		m_enItem_Flash,			//�M���e			
		m_enItem_SoundBom		//�����e
	};
	EnItemState m_enItemState = m_enItemState;
	Game* m_game=nullptr;
	PlayerManagement* m_playerManagement=nullptr;
	Player3D* m_player3D = nullptr;
	Vector3 EffecrRange = Vector3::Zero;

	std::vector<Enemy*> GetHitEnemyList()
	{
		return m_HitEnemyList;
	}

	std::vector<Enemy*> m_HitEnemyList;

};

