#include "stdafx.h"
#include "FlashBom.h"
#include "Enemy.h"
FlashBom::FlashBom()
{

}
FlashBom::~FlashBom()
{

}
bool FlashBom::Start()
{
	Item::Start();
	return true;
}
void FlashBom::Update()
{
	Item::Update();

}
void FlashBom::ItemEffect()
{
	for (int i = 0; GetHitEnemyList().size(); i++)
	{
		if (GetHitEnemyList()[i]->GetHitFlushBullet() == true)
		{
		}
	}
}