#pragma once
class Game;
class Item :public IGameObject
{
public:
	Item();
	virtual ~Item();
	virtual bool Start();
	virtual void Update();
	/// <summary>
	/// アイテムの切替
	/// </summary>
	void ItemChange();
	/// <summary>
	/// アイテムの範囲
	/// </summary>
	void ItemRange();
	virtual void ItemEffect() = 0;
protected:
	enum EnItemState
	{
		m_enItem_No,			//何も持っていない
		m_enItem_Flash,			//閃光弾			
		m_enItem_SoundBom		//音爆弾
	};
	EnItemState m_enItemState = m_enItemState;
	Game* m_game;

	Vector3 EffecrRange = Vector3::Zero;

};

