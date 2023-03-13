#include "stdafx.h"
#include "Player2D.h"
#include "Player3D.h"
#include "GameCamera.h"
namespace
{
	const Vector3 BOXSIZE{ 80.0f,120.0f,2.0f };//ボックスコライダーの大きさ
	const Vector3 MODELSIZE{ 1.0f,1.0f,1.0f };
}
Player2D::Player2D()
{

}
Player2D::~Player2D()
{
	delete(m_characon);
	delete(m_modelRender);
}
bool Player2D::Start()
{
	Player::Start();
	m_modelRender = new ModelRender;
	m_characon = new CharacterController;
	m_player3D = FindGO<Player3D>("player3d");
	//モデルの読み込み
	m_modelRender->Init("Assets/modelData/player/player2D.tkm");
	m_modelRender->SetScale(MODELSIZE);
	m_characon->Init(BOXSIZE, m_position);
	m_player2D[0].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_1.DDS");
	m_player2D[1].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_2.DDS");
	m_player2D[2].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_3.DDS");
	Deactivate();
	delete(m_characon);
	return true;
}
void Player2D::Update(bool m_newcharacon)
{
	Player::Update();
	Animation();

	//キャラコンが生成されているかどうか
	if (m_newcharacon != true)
	{
		//一度しか生成されないようにする
		m_characon = new CharacterController;
		m_newcharacon = false;
	}
	//プレイヤーを切替る
	if (g_pad[0]->IsTrigger(enButtonLB1))
	{
		Player::Change(true);
	}

	m_characon->SetPosition(m_position);
	m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime()/2.0f);
	m_modelRender->SetPosition(m_position);
	gamecamera->SetPosition(m_position);
	m_modelRender->Update();	
	//切り替える
	PlayerChang();
}

void Player2D::PlayerChang()
{
	if (m_playerState == m_enPlayer_3DChanging)
	{
		m_player3D->Activate();
		//プレイヤー２Dに３Dの座標を与える
		m_player3D->SetPosition(m_position);
		//ステートを遷移する。
		ProcessCommonStateTransition();
		Deactivate();
	}
}
void Player2D::Animation()
{
	int j = i / 10;
	i++;
	if (i >= 30)
	{
		i = 0;
	}
	m_modelRender->GetModel().ChangeAlbedoMap("", m_player2D[j]);
}
void Player2D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}