#include "stdafx.h"
#include "Item.h"
#include "Game.h"
Item::Item()
{

}
Item::~Item()
{

}
bool Item::Start()
{
	m_game = FindGO<Game>("game");
	return true;
}
void Item::Update()
{

}
void Item::ItemChange()
{
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		m_enItemState = m_enItem_Flash;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		m_enItemState = m_enItem_SoundBom;
	}
}

void Item::ItemRange()
{
	
}

void Item::ItemEffect()
{
}