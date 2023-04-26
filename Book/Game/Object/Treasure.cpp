#include "stdafx.h"
#include "Treasure.h"
#include "Game.h"
#include <random>

namespace
{
	const Vector3 BOXSIZE{ 50.0f,50.0f,50.0f };
	const float GAGESIZE = 10.0f;
}
Treasure::Treasure()
{

}
Treasure::~Treasure()
{
	DeleteGO(m_collisionObject);
}
bool Treasure::Start()
{
	Object::Start();
	//ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_modelRender.Init("Assets/modelData/object/takara/item.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotation);
	//ƒRƒŠƒWƒ‡ƒ“‚Ìì¬
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(
		m_position,
		m_rotation,
		BOXSIZE
	);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("otakara");

	


	m_collisionObject->Update();
	m_modelRender.Update();
	return true;
}
void Treasure::Update()
{
	Collision();
	m_modelRender.Update();
}
void Treasure::Hit()
{

	if (g_pad[0]->IsPress(enButtonX)&&m_gagecount<=GAGESIZE)
	{
		m_gagecount += g_gameTime->GetFrameDeltaTime();
	}
	if (m_gagecount >= 1.0f)
	{
		m_player3d->m_enPlayer3D_Steal;
		m_game->m_gameState = m_game->m_enGameState_GameClearable;
		m_game->m_pointLight[m_game->m_lightNumber].SetColor(Vector3(50.0f, 0.0f, 0.0f));
		m_game->m_pointLight[m_game->m_lightNumber].Update();


		DeleteGO(this);
	}
}

void Treasure::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}