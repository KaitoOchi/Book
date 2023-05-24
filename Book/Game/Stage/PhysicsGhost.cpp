#include "stdafx.h"
#include "PhysicsGhost.h"
#include "PlayerManagement.h"

namespace
{
	const float EFFECT_SIZE = 1.5f;				//エフェクトサイズ
	const float DISTANCE = pow(1450.0f, 2.0f);	//エフェクトが見える距離
}

PhysicsGhost::PhysicsGhost()
{

}

PhysicsGhost::~PhysicsGhost()
{
	m_kirakiraEffect->SetDeleteState(false);
	m_kirakiraEffect->Stop();
}

bool PhysicsGhost::Start()
{
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");

	Ghost::Start();

	m_physicsGhostObj.CreateBox(
		m_position,
		m_rotation,
		m_boxSize
	);
	m_physicsGhostObj.GetbtCollisionObject().setUserIndex(enCollisionAttr_Ground);

	//エフェクトの読み込み
	m_kirakiraEffect = NewGO<EffectEmitter>(5);
	m_kirakiraEffect->SetDeleteState(true);
	m_kirakiraEffect->Init(5);
	m_kirakiraEffect->SetPosition(Vector3{ m_position.x,m_position.y + 50.0f,m_position.z });
	m_kirakiraEffect->SetScale(Vector3::One * EFFECT_SIZE);
	m_kirakiraEffect->Play();
	m_kirakiraEffect->Update();

	return true;
}
void PhysicsGhost::Update()
{
	if (m_kirakiraEffect == nullptr) {
		return;
	}

	EffectDraw();
}

void PhysicsGhost::EffectDraw()
{
	Vector3 diff = m_position - m_playerManagement->GetPosition();

	if (diff.LengthSq() < DISTANCE)
	{
		if (!m_kirakiraEffect->IsPlay()) {
			m_kirakiraEffect->Play();
		}
	}
	else
	{
		m_kirakiraEffect->Stop();
	}
	
}