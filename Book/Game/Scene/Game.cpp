#include "stdafx.h"
#include "Game.h"
#include"Player3D.h"
#include"Player2D.h"
#include"GameCamera.h"
#include "PlayerManagement.h"
#include "GameUI.h"
#include "Title.h"
#include "SenSor.h"
#include "MiniMap.h"
#include "Enemy.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"
#include "BackGround.h"
#include "LightSensor.h"
#include "Wall.h"
#include "Treasure.h"
#include "Ghost.h"
#include "FlashBom.h"
#include "SoundBom.h"
#include "Fade.h"
Game::Game()
{
	//�E��E��E��E��E�蔻�E��E��E�L�E��E��E��E�
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

Game::~Game()
{
	DeleteGO(m_player3D);
	DeleteGO(m_player2D);
	DeleteGO(m_playerManagement);
	//�G�l�~�[
	DeleteGO(m_enemyNormal);
	DeleteGO(m_enemyCharge);
	DeleteGO(m_enemySearch);

	//�I�u�W�F�N�g
	DeleteGO(FindGO<Sensor>("sensor"));
	DeleteGO(FindGO<GameUI>("gameUI"));
	DeleteGO(m_miniMap);
	DeleteGO(m_gamecamera);
	DeleteGO(m_backGround);
	//�A�C�e��
	DeleteGO(m_soundBom);
	DeleteGO(m_flahBom);
}

bool Game::Start()
{
	m_player2D=NewGO<Player2D>(0,"player2d");
	m_player3D = NewGO<Player3D>(0, "player3d");
	m_gamecamera=NewGO<GameCamera>(0, "gameCamera");
	NewGO<Sensor>(0, "sensor");
	m_playerManagement=NewGO<PlayerManagement>(0,"playerManagement");
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	NewGO<GameUI>(0, "gameUI");
	//NewGO<LightSensor>(0, "lightSensor");
	//m_stageModelRender.Init("Assets/modelData/stage1.tkm");
	//m_stageModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	//m_stageModelRender.SetRotation(Quaternion::Identity);
	//m_stageModelRender.SetScale(Vector3::One);
	//m_stageModelRender.Update();
	/*m_demobg.CreateFromModel(m_stageModelRender.GetModel(), m_stageModelRender.GetModel().GetWorldMatrix());*/
	m_flahBom = NewGO<FlashBom>(0, "flashBom");
	m_soundBom = NewGO<SoundBom>(0, "soundBom");
	m_pointLight[0].SetPointLight(
		0,
		Vector3::Zero,
		{ 10.0f, 0.0f, 0.0f },
		100.0f
	);

	m_pointLight[1].SetPointLight(
		1,
		Vector3::Zero,
		{ 00.0f, 10.0f, 0.0f },
		50.0f
	);

	m_pointLight[2].SetPointLight(
		2,
		Vector3::Zero,
		{ 0.0f, 0.0f, 10.0f },
		100.0f
	);

	m_pointLight[3].SetPointLight(
		3,
		Vector3::Zero,
		{ 10.0f, 0.0f, 0.0f },
		150.0f
	);

	LevelDesign();

	for (int i = 0; i < 4; i++) {
		m_pointLight[i].Update();
	}

	m_spotLight.SetSpotLight(
		0,
		{ 0.0f, 40.0f, 0.0f },
		{ 0.0f, 10.0f, 0.0f },
		400.0f,
		Vector3::AxisZ,
		45.0f
	);

	m_miniMap = NewGO<MiniMap>(0, "miniMap");


	//�t�F�[�h�̏���
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void Game::LevelDesign()
{
	// ���x���f�U�C������
	m_levelRender.Init("Assets/modelData/level/debug_1.tkl", [&](LevelObjectData& objData) {

		// ���O��unityChan�Ȃ�
		if (objData.ForwardMatchName(L"FootmanHP") == true) {
			//m_mirror = NewGO<Mirror>(0, "mirror");

			m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			m_enemyNormal->SetPosition(objData.position);
			m_enemyNormal->SetRotation(objData.rotation);
			m_enemyNormal->SetScale(objData.scale);
			// Enemy�̃��X�g�ɒǉ�
			m_enemyList.push_back(m_enemyNormal);

			// �p�X�ړ��̎w��
			m_enemyNormal->Pass(0);

			m_enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
			m_enemyCharge->SetPosition(objData.position);
			m_enemyCharge->SetRotation(objData.rotation);
			m_enemyCharge->SetScale(objData.scale);
			// Enemy�̃��X�g�ɒǉ�
			m_enemyList.push_back(m_enemyCharge);

			// �p�X�ړ��̎w��
			m_enemyCharge->Pass(7);

			return true;
		}

		//// ���O�� EnemyNormal �Ȃ�
		//if (objData.ForwardMatchName(L"EnemyNormal") == true) {

		//	// �G�l�~�[�𐶐�
		//	m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
		//	// ���g�� Normal �ł���Ƌ�����
		//	m_enemyNormal->m_enemyType = Enemy::Normal;
		//	// ���W�A��]�A�X�P�[���̐ݒ�
		//	m_enemyNormal->SetPosition(objData.position);
		//	m_enemyNormal->SetRotation(objData.rotation);
		//	m_enemyNormal->SetScale(objData.scale);
		//	// �p�X�ړ��̎w��
		//	m_enemyNormal->Pass(0);
		//	// Enemy�̃��X�g�ɒǉ�
		//	m_enemyList.push_back(m_enemyNormal);
		//}

		//// ���O�� EnemyCharge �Ȃ�
		//if (objData.ForwardMatchName(L"EnemyCharge") == true) {

		//	// �G�l�~�[�𐶐�
		//	m_enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
		//	// ���g�� Charge �ł���Ƌ�����
		//	m_enemyCharge->m_enemyType = Enemy::Charge;
		//	// ���W�A��]�A�X�P�[���̐ݒ�
		//	m_enemyCharge->SetPosition(objData.position);
		//	m_enemyCharge->SetRotation(objData.rotation);
		//	m_enemyCharge->SetScale(objData.scale);
		//	// �p�X�ړ��̎w��
		//	m_enemyCharge->Pass(7);
		//	// Enemy�̃��X�g�ɒǉ�
		//	m_enemyList.push_back(m_enemyCharge);
		//}

		//// ���O�� EnemySearch �Ȃ�
		//if(objData.ForwardMatchName(L"EnemySearch") == true) {

		//	// �G�l�~�[�𐶐�
		//	m_enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
		//	// ���g�� Charge �ł���Ƌ�����
		//	m_enemySearch->m_enemyType = Enemy::Search;
		//	// ���W�A��]�A�X�P�[���̐ݒ�
		//	m_enemySearch->SetPosition(objData.position);
		//	m_enemySearch->SetRotation(objData.rotation);
		//	m_enemySearch->SetScale(objData.scale);
		//	// Enemy�̃��X�g�ɒǉ�
		//	m_enemyList.push_back(m_enemySearch);
		//}

		//���O��background�Ȃ�
		if (objData.EqualObjectName(L"debug") == true) {

			// �w�i�𐶐�
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		// ���O��box�̂Ƃ�
		if (objData.EqualObjectName(L"box") == true) {

			m_wall = NewGO<Wall>(0, "wall");
			m_wall ->SetPosition(objData.position);
			m_wall->SetRotation(objData.rotation);
			m_wall->SetScale(objData.scale);

			return true;
		}
		if (objData.EqualObjectName(L"unityChan") == true) {

			m_enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
			m_enemySearch->SetPosition(objData.position);
			m_enemySearch->SetRotation(objData.rotation);
			m_enemySearch->SetScale(objData.scale);
			// Enemy�̃��X�g�ɒǉ�
			m_enemyList.push_back(m_enemySearch);

			return true;
		}
		if (objData.EqualObjectName(L"debugtoumei") == true) {

			m_player3D->m_ghostpositions.push_back(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"item") == true) {

			m_treaSure = NewGO<Treasure>(0, "treaSure");
			m_treaSure->SetPosition(objData.position);
			m_treaSure->SetScale(objData.scale);
			m_treaSure->SetRotation(objData.rotation);
			return true;
		}
		if (objData.EqualObjectName(L"gost") == true) {

			m_ghost = NewGO<Ghost>(0, "ghost");
			m_ghost->SetPosition(objData.position);
			m_ghost->SetScale(objData.scale);
			m_ghost->SetRotation(objData.rotation);
			return true;
		}
		if (objData.EqualObjectName(L"clear") == true) {

			SetClearPosition(objData.position);

			for (int i = 0; i < 4; i++) {
				m_pointLight[i].SetPosition(Vector3(m_position.x + (i * 100), m_position.y + 10.0f, m_position.z));
			}
			return true;
		}

		return true;
		}
	);
}

void Game::Update()
{
	Vector3 diff = m_playerManagement->GetPosition()- GetClearPosition();
	if (diff.LengthSq() <= 120.0f*120.0f)
	{
		m_gameState = m_enGameState_GameClear;
	}
		
	MnageState();

	for (int i = 0; i < 4; i++) {
		m_pointLight[i].Update();
	}

	m_spotLight.Update();
}
void Game::ClearState()
{
	NewGO<Title>(0, "title");
	DeleteGO(this);
	int a = 0;
}

void Game::MnageState()
{
	switch (m_gameState)
	{
	case Game::m_enGameState_DuringGamePlay:
		break;
	case Game::m_enGameState_GameClear:
		ClearState();
		break;
	case Game::m_enGameState_GameOver:
		break;
	case Game::m_enGameState_GameBuck:
		break;
	default:
		break;
	}
}
void Game::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
}
