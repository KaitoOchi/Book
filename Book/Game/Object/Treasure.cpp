#include "stdafx.h"
#include "Treasure.h"

#include "Game.h"
#include "Gage.h"
#include "GameUI.h"

namespace
{
	const Vector3 BOXSIZE{ 150.0f,50.0f,150.0f };
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
	m_gage = FindGO<Gage>("gage");
	m_gameUI = FindGO<GameUI>("gameUI");
	Object::Start();

	//ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_modelRender.Init("Assets/modelData/object/takara/treasure.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale * 3.0f);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();

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

	return true;
}

void Treasure::Update()
{
	m_gameUI->SetCircleState(false);
	Collision();
	m_modelRender.Update();
}

void Treasure::Hit()
{

	if (g_pad[0]->IsPress(enButtonB))
	{
		m_gameUI->SetCircleState(true);
	}
	if (m_gameUI->GetCircleMAXState())
	{
		m_player3d->m_enPlayer3D_Steal;
		m_game->NotifyGameClearable();
		m_game->GetPointLight().SetColor(Vector3(2.0f, 0.0f, 0.0f));
		m_game->GetPointLight().Update();

		m_game->NotifyEventStart();

		m_game->SetTresurePosition(m_position);

		Deactivate();
	}
}

void Treasure::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}