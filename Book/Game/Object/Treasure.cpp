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
	DeleteGO(m_kirakiraEffect);

	DeleteGO(m_collisionObject);
}

bool Treasure::Start()
{
	m_gameUI = FindGO<GameUI>("gameUI");
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

	//����̃G�t�F�N�g�̐ݒ�
	m_kirakiraEffect = NewGO<EffectEmitter>(0);
	m_kirakiraEffect->Init(6);
	m_kirakiraEffect->SetPosition(m_position);
	m_kirakiraEffect->Play();
	m_kirakiraEffect->Update();

	return true;
}

void Treasure::Update()
{
	m_gameUI->SetCircleState(false);
	if (m_gameUI->GetDegree() <= 0.0f)
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
	//B�{�^����������Ă���Ȃ�
	if (g_pad[0]->IsPress(enButtonA))
	{
		//���₹���Ԃɂ���
		m_gameUI->SetCircleState(true);
	}
	if (m_gameUI->GetCircleMAXState())
	{
		m_player3d->m_enPlayer3D_Steal;
		m_game->NotifyGameClearable();

		m_game->NotifyEventStart();

		m_game->SetTresurePosition(m_position);

		Deactivate();
	}
}

void Treasure::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}