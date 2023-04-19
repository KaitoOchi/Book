#pragma once
#include "Item.h"
class Game;

class SoundBom:public Item
{
public:
	SoundBom();
	~SoundBom()override;
	bool Start()override;
	void Update()override;
	void ItemHit()override;
private:

};

