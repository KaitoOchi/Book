#pragma once
#include "Item.h"
class Game;
class SoundBom:public Item
{
public:
	SoundBom();
	~SoundBom()override;
	bool Start()override;
	void Update();
	void ItemHit()override;
	void ItemStart();
private:
	EffectEmitter* m_soundEffect=nullptr;
};

