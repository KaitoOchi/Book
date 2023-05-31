#include "stdafx.h"
#include "Treasure.h"

#include "Game.h"
#include "Gage.h"
#include "GameUI.h"
#include "Player2D.h"
#include "Player3D.h"
namespace
{
	const Vector3	BOXSIZE = { 80.0f, 80.0f, 80.0f };		//�R���W�����̃T�C�Y
}

Treasure::Treasure()
{
	m_treasurePositions.reserve(3);
}

Treasure::~Treasure()
{
	m_treasurePositions.clear();
	m_treasurePositions.shrink_to_fit();

	if (m_kirakiraEffect != nullptr) {
		m_kirakiraEffect->SetDeleteState(false);
		m_kirakiraEffect->Stop();
	}
	DeleteGO(m_kirakiraEffect);

	DeleteGO(m_collisionObject);
}

bool Treasure::Start()
{
	m_gameUI = FindGO<GameUI>("gameUI");
	m_player2d = FindGO<Player2D>("player2d");
	Object::Start();

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/object/takara/treasure.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale * 3.0f);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();

	//�R���W�����̍쐬
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(
		m_position,
		m_rotation,
		BOXSIZE
	);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("otakara");
	m_collisionObject->Update();

	////����̃G�t�F�N�g�̐ݒ�
	m_kirakiraEffect = NewGO<EffectEmitter>(0);
	m_kirakiraEffect->Init(6);
	m_kirakiraEffect->SetDeleteState(true);
	m_kirakiraEffect->SetPosition(m_position);
	m_kirakiraEffect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
	m_kirakiraEffect->Play();
	m_kirakiraEffect->Update();

	return true;
}

void Treasure::Update()
{
	m_gameUI->SetCircleState(false);
	m_hitState = false;
	m_gameUI->SetCircleDrawState(false);

	if (m_gameUI->GetDegree() >= 360.0f)
	{
		m_gameUI->SetCircleDrawState(false);
	}

	Collision();

	m_modelRender.Update();
}

void Treasure::Hit()
{
	//�~�`�Q�[�W�𑝂₷
	m_gameUI->SetCircleDrawState(true);
	m_hitState = true;
	//A�{�^����������Ă���Ȃ�
	if (g_pad[0]->IsPress(enButtonA)) {
		//���₹���Ԃɂ���
		m_gameUI->SetCircleState(true);
	}

	//�Q�[�W���ő�܂ł�������
	if (m_gameUI->GetCircleMAXState())
	{
		m_hitState = false;

		m_gameUI->SetCircleDrawState(false);

		m_player3d->m_enPlayer3D_Steal;

		m_player3d->m_Player_Act = false;
		m_player2d->m_Player_Act = false;
		m_player2d->SetStamina(10.0f);
		m_player3d->SetStamina(10.0f);
		m_player3d->SetMoveSpeed(Vector3::Zero);
		m_gameUI->SetStaminaDrawState(false);
		//�C�x���g�̊J�n
		m_game->NotifyEventStart();

		//�G�t�F�N�g�̒�~
		m_kirakiraEffect->SetDeleteState(false);
		m_kirakiraEffect->Stop();

		Deactivate();
	}
}

void Treasure::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}