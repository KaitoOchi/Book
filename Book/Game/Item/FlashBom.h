#pragma once
#include "Item.h"
class Game;
class FlashBom:public Item
{
public:
	FlashBom();
	~FlashBom()override;
	bool Start()override;
	void Update()override;
	void ItemEffect()override;
private:

};

