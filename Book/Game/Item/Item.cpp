#include "stdafx.h"
#include "Item.h"

#include "Game.h"
#include "PlayerManagement.h"
#include "Player3D.h"
#include "Enemy.h"
#include "GameUI.h"

Item::Item()
{

}

Item::~Item()
{

}

bool Item::Start()
{
	m_game = FindGO<Game>("game");
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_player3D = FindGO<Player3D>("player3D");
	m_gameUI = FindGO<GameUI>("gameUI");

	return true;
}

void Item::Update()
{
	ManageState();
}
