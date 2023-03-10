#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::Update()
{
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		GameObjectManager::GetInstance()->SetStop(true);
	}

	if (g_pad[0]->IsTrigger(enButtonY))
	{
		GameObjectManager::GetInstance()->SetStop(false);
	}
}