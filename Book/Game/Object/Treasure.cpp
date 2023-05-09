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
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int>dist(0, 2);
	m_randTreasure = dist(mt);
	Object::Start();
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/object/takara/item.tkm");
	m_modelRender.SetPosition(m_game->GetTreasurePositionList()[m_randTreasure]);
	m_modelRender.SetScale(m_scale*3.0f);
	m_modelRender.SetRotation(m_rotation);
	//�R���W�����̍쐬
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(
		m_game->GetTreasurePositionList()[m_randTreasure],
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

	if (g_pad[0]->IsPress(enButtonB)&&m_gagecount<=GAGESIZE)
	{
		m_gagecount += g_gameTime->GetFrameDeltaTime();
	}
	if (m_gagecount >= 1.0f)
	{
		m_player3d->m_enPlayer3D_Steal;
		m_game->NotifyGameClearable();
		m_game->m_pointLight[m_game->m_lightNumber].SetColor(Vector3(50.0f, 0.0f, 0.0f));
		m_game->m_pointLight[m_game->m_lightNumber].Update();


		Deactivate();
	}
}

void Treasure::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}