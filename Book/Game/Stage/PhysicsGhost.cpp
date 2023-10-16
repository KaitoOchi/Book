#include "stdafx.h"
#include "PhysicsGhost.h"
#include "PlayerManagement.h"
#include "UI/TutorialUI.h"

namespace
{
	const float EFFECT_SIZE = 1.5f;				//エフェクトサイズ
	const float DISTANCE = pow(1200.0f, 2.0f);	//エフェクトが見える距離
	const float LENGTH = 500.0f;				//判定する距離
}

PhysicsGhost::PhysicsGhost()
{

}

PhysicsGhost::~PhysicsGhost()
{
	m_kirakiraEffect->SetDeleteState(false);
	m_kirakiraEffect->Stop();
	DeleteGO(m_kirakiraEffect);
}

bool PhysicsGhost::Start()
{
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_turorialUI = FindGO<TutorialUI>("turorialUI");

	Ghost::Start();
	Vector3 boxSize = CreateGhostBox();

	//当たり判定を作成
	m_physicsGhostObj.CreateBox(
		m_position,
		m_rotation,
		boxSize
	);
	m_physicsGhostObj.GetbtCollisionObject().setUserIndex(enCollisionAttr_Ground);

	if (m_isDraw == true) {
		//エフェクトの読み込み
		m_kirakiraEffect = NewGO<EffectEmitter>(5);
		m_kirakiraEffect->SetDeleteState(true);
		m_kirakiraEffect->Init(5);
		m_kirakiraEffect->SetPosition(Vector3{ m_position.x,m_position.y + 50.0f,m_position.z });
		m_kirakiraEffect->SetScale(Vector3::One * EFFECT_SIZE);
		m_kirakiraEffect->Play();
		m_kirakiraEffect->Update();
	}
	return true;
}
void PhysicsGhost::Update()
{
	IsHitPlayer();

	if (m_kirakiraEffect == nullptr) {
		return;
	}

	if (m_isDraw == true) {
		EffectDraw();
	}
}

void PhysicsGhost::EffectDraw()
{
	Vector3 diff = m_position - m_playerManagement->GetPosition();

	if (diff.LengthSq() < DISTANCE) {

		if (!m_kirakiraEffect->IsPlay()) {
			m_kirakiraEffect->Play();
		}
	}
	else {
		m_kirakiraEffect->Stop();
	}
	
}

bool PhysicsGhost::IsHitPlayer()
{
	// プレイヤーから自身へ向かうベクトル
	Vector3 diff = m_position - m_playerManagement->GetPosition();

	// 一定以上のときは実行しない
	if (diff.LengthSq() > LENGTH) {
		return false;
	}

	// 衝突判定を返す
	if (IsHit(m_playerManagement->GetPosition()) == true) {
		// チュートリアルのテキスト画像を描画しているとき
		if (m_turorialUI!=nullptr && m_turorialUI->GetTutorialDrawState() == true) {
			// 描画しないようにする
			m_turorialUI->SetTutorialDrawState(false);
		}

		return true;
	}

	return false;
}