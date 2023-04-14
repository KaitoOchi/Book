#pragma once
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

	virtual void ItemEffect() = 0;
protected:
	enum EnItemState
	{
		m_enItem_No,			//���������Ă��Ȃ�
		m_enItem_Flash,			//�M���e			
		m_enItem_SoundBom		//�����e
	};
	EnItemState m_enItemState = m_enItemState;


};

