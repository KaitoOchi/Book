#pragma once
class Item :public IGameObject
{
public:
	Item();
	virtual ~Item();
	virtual bool Start();
	virtual void Update();
	/// <summary>
	/// ƒAƒCƒeƒ€‚ÌØ‘Ö
	/// </summary>
	void ItemChange();

	virtual void ItemEffect() = 0;
protected:
	enum EnItemState
	{
		m_enItem_No,			//‰½‚à‚Á‚Ä‚¢‚È‚¢
		m_enItem_Flash,			//‘MŒõ’e			
		m_enItem_SoundBom		//‰¹”š’e
	};
	EnItemState m_enItemState = m_enItemState;


};

