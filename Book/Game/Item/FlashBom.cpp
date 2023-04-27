#include "stdafx.h"
#include "FlashBom.h"
#include "Enemy.h"
#include "Game.h"
#include "PlayerManagement.h"
namespace
{
	const float FLASHRANGE = 300.0f;			//フラッシュの効果範囲
	const float MAXRANGE = 2000.0f;				//ポイントライトの範囲
	const float MAXALPHA = 0.9;					//α値の範囲
	const float MAXAMBIENT = 10.0f;				//環境の強さ
}
FlashBom::FlashBom()
{

}
FlashBom::~FlashBom()
{

}
bool FlashBom::Start()
{
	Item::Start();
	m_sphereCollider.Create(1.0f);
	//ポイントライト
	m_pointLight.SetColor(Vector3(0.0f,0.0f,0.0f));
	m_pointLight.SetRange(MAXRANGE);
	m_pointLight.Update();
	
	//フラッシュ
	m_flashRender.Init("Assets/sprite/Flash/flash.DDS", 1920.0f, 1080.0f);
	return true;
}

//衝突したときに呼ばれる関数オブジェクト（壁用）
struct SweepResyltWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
		{
			//衝突したのは壁ではない
			return 0.0f;
		}
		//壁とぶつかったら
		//フラグをtrueに
		isHit = true;
		return 0.0f;
		g_pad[0]->IsTrigger(enButtonB);
	}
};


void FlashBom::Update()
{
	ManageState();
	if (m_FlashState != m_enFlash_No&&m_FlashState!=m_enFlash_Start)
	{
		FlashEffect();
	}
}
void FlashBom::ItemHit()
{
	//ポイントライト
	m_position.y = 200.0f;
	m_pointLight.SetPosition(m_position);
	m_pointLight.Update();

	//近くにいるエネミーたちを探す
	for (int i = 0; m_game->GetEnemyList().size(); i++)
	{
		Vector3 diff = m_position - m_game->GetEnemyList()[i]->GetPosition();
        if (diff.Length() < pow(FLASHRANGE, 2.0f))
		{
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			Vector3 enemyPosition = m_game->GetEnemyList()[i]->GetPosition();
			Vector3 playerPosition = m_playerManagement->GetPosition();
			//始点はプレイヤーの座標
			start.setOrigin(btVector3(playerPosition.x, playerPosition.y, playerPosition.z));
			//終点はエネミーの座標
			end.setOrigin(btVector3(enemyPosition.x, enemyPosition.y, enemyPosition.z));
			SweepResyltWall callback;
			//コライダーを始点から終点までを動かして。
			//衝突するかどうか調べる
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
            //壁と衝突した
			if (callback.isHit == true)
			{
				//プレイヤーは見つかっていない
				return;
			}
			//壁と衝突していない
			//次にエネミーがこちらの法を向いているか調べる
			//アイテムからエネミーに向かうベクトルを正規化する
			diff.Normalize();
			//エネミーの正面ベクトルとアイテムからエネミーに向かうベクトルの内積(cosθ)を求める
			float cos = m_game->GetEnemyList()[i]->GetFoward().Dot(diff);
			//内積から角度を求める
			float angle = acosf(cos);
			if (angle <= (Math::PI / 180.0f) * 90.0f)
			{
				//エネミーフラグをtrueに。
				m_game->GetEnemyList()[i]->SetHitFlashBullet(true);
			}
		}
	}
	
}

void FlashBom::FlashEffect()
{
	//フラッシュを減少させていく
	m_range -= (MAXRANGE / 100.0f);
	m_alpha-= (MAXALPHA / 100.0f);
	m_ambient -=(MAXAMBIENT / 100.0f+0.7);
	

	m_flashRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	
	RenderingEngine::GetInstance()->SetAmbient(m_ambient);
	
	m_pointLight.SetRange(m_range);
	m_pointLight.Update();
}

void FlashBom::SetFlashEffect()
{
	RenderingEngine::GetInstance()->SetAmbient(10.0f);
	//ポイントライトの初期化
	m_pointLight.SetColor(Vector3(10.0f, 10.0f, 10.0f));
	m_pointLight.Update();
	//フラッシュ時の値を入れる
	m_alpha = MAXALPHA;
	m_range = MAXRANGE;
	m_ambient = MAXAMBIENT;
}

void FlashBom::ProcessStartState()
{
	//開始時に必要な物を呼ぶ
	SetFlashEffect();
	ItemHit();
	m_FlashState = m_enFlash_Flashing;
}

void FlashBom::ProcessFlashingState()
{
	if (m_alpha <= 0.0f && m_range <= 0.0f)
	{
		m_pointLight.SetRange(0.0f);
		m_pointLight.SetColor(Vector3(0.0f, 0.0f, 0.0f));
		m_pointLight.Update();
		m_alpha = 0.0f;
		RenderingEngine::GetInstance()->SetAmbient(0.7f);
		m_FlashState = m_enFlash_No;
	}

}

void FlashBom::ProcessEndState()
{

}

void FlashBom::ManageState()
{
	switch (m_FlashState)
	{
	case FlashBom::m_enFlash_Start:
		ProcessStartState();
		break;
	case FlashBom::m_enFlash_Flashing:
		ProcessFlashingState();
		break;
	case FlashBom::m_enFlash_No:
		break;
	default:
		break;
	}
}

void FlashBom::Render(RenderContext& rc)
{
	m_flashRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f,m_alpha));
	m_flashRender.Draw(rc);
}