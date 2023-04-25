#include "stdafx.h"
#include "Wall.h"

#include "PlayerManagement.h"

namespace
{
	const int GAP_SUM = 1;		// 隙間ありの種類
	const int PAINT_SUM = 1;	// 絵画ありの種類
}

Wall::Wall()
{

}
Wall::~Wall()
{

}
bool Wall::Start()
{
	m_player = FindGO<PlayerManagement>("playerManagement");

	//SetWallModel(ModelState);

	m_wallRender.Init("Assets/modelData/level_test/box.tkm", 0, 0, enModelUpAxisZ, 1, 1);
	m_wallRender.SetPosition(m_position);
	m_wallRender.SetScale(m_scale);
	m_wallRender.SetRotation(m_rotation);
	
	m_physicsBoxObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix(),m_scale);
	m_physicsBoxObj.Create(m_position, m_rotation);
	m_physicsBoxObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// コリジョン属性を設定

	//m_physicsStaticObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix());
	//m_physicsStaticObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// コリジョン属性を設定
	//m_physicsStaticObj.GetRigidBody()->SetPositionAndRotation(m_position, m_rotation);

	m_wallRender.Update();

	return true;
}

void Wall::SetModel(int num)
{
	// 変数を初期化
	int hoge = 0;

	// 壁のモデルを読み込む
	switch (num)
	{
	case NORMAL:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	case WITH_PANINTING:
		// ランダムに値を返す
		hoge = rand() % PAINT_SUM;
		SetModel_withPainting(hoge);
		break;
	case WITH_GAP:
		// ランダムに値を返す
		hoge = rand() % GAP_SUM;
		SetModel_withGap(hoge);
		break;
	case START:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	case GOAL:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	}
}

void Wall::SetModel_withPainting(int num)
{
	// モデル用意してないのでまだ。
}

void Wall::SetModel_withGap(int num)
{
	switch (num) {
	case 0:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	case 1:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	case 2:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	}
}

void Wall::Update()
{
}
void Wall::Render(RenderContext& rc)
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.Length() < 1500.0f) {
		m_wallRender.Draw(rc);
	}
}