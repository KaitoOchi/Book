#include "stdafx.h"
#include "Treasure.h"
#include "Game.h"
#include "Gage.h"
#include <random>

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
	//お宝の座標をランダムで生成する
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int>dist(0, 2);
	m_randTreasure = dist(mt);

	m_gage = FindGO<Gage>("gage");

	Object::Start();
	m_position = m_game->GetTreasurePositionList()[m_randTreasure];
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/object/takara/treasure.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale*3.0f);
	m_modelRender.SetRotation(m_rotation);
	//コリジョンの作成
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

	if (g_pad[0]->IsPress(enButtonB)&&m_gagecount<=GAGESIZE)
	{
		m_gagecount += g_gameTime->GetFrameDeltaTime();
	}
	if (m_gagecount >= 1.0f)
	{
		m_player3d->m_enPlayer3D_Steal;
		m_game->NotifyGameClearable();
		m_game->GetPointLight().SetColor(Vector3(50.0f, 0.0f, 0.0f));
		m_game->GetPointLight().Update();

		m_game->NotifyEventStart();

		m_gage->m_leverState = m_gage->m_enLever_MAX;

		m_game->SetTresurePosition(m_position);
		
		Deactivate();
	}
}

void Treasure::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}