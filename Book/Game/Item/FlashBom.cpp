#include "stdafx.h"
#include "FlashBom.h"
#include "Enemy.h"
#include "Game.h"
#include "PlayerManagement.h"
namespace
{
	const Vector3 LIGHTNO = Vector3::Zero;
	const float FLASHRANGE = 300.0f;			//フラッシュの効果範囲
	const float MAXRANGE = 2000.0f;				//ポイントライトの範囲
	const float MAXALPHA = 0.9;					//α値の範囲
	const float MAXAMBIENT = 10.0f;				//環境の強さ
	const float MAXCOLOR = 10.0f;
	
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
	m_sphereCollider.Create(0.1f);
	//ポイントライト
	m_pointLight.SetPointLight(3, m_position, LIGHTNO, MAXRANGE);
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
	}
};


void FlashBom::Update()
{
	ManageState();
	if (m_FlashState == m_enFlash_Flashing)
	{
		FlashEffect();
	}
	m_ambient = max(m_ambient, 0.0f);
	RenderingEngine::GetInstance()->SetAmbient(m_ambient);

}
void FlashBom::ItemHit()
{
	//ポイントライト
	m_position.y = 200.0f;
	m_pointLight.SetPosition(m_position);
	m_pointLight.Update();

	//近くにいるエネミーたちを探す
	for (int i = 0; i<m_game->GetEnemyList().size(); i++)
	{
		Vector3 diff = m_position - m_game->GetEnemyList()[i]->GetPosition();
        if (diff.Length() < pow(FLASHRANGE, 2.0f))
		{
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			Vector3 enemyPosition = m_game->GetEnemyList()[i]->GetPosition();
			//始点はアイテムの座標
			start.setOrigin(btVector3(m_position.x,10.0f, m_position.z));
			//終点はエネミーの座標
			end.setOrigin(btVector3(enemyPosition.x, 10.0f, enemyPosition.z));
			SweepResyltWall callback;
			//コライダーを始点から終点までを動かして。
			//衝突するかどうか調べる
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突していない
			if (callback.isHit == false)
			{
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
	
}

void FlashBom::FlashEffect()
{
	//フラッシュを減少させていく
	m_range -= (MAXRANGE / 100.0f);
	m_alpha-= (MAXALPHA / 100.0f);
	m_ambient -=(MAXAMBIENT / 100.0f);
	m_color -= (MAXCOLOR / 100.0f);
	

	m_flashRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	
	RenderingEngine::GetInstance()->SetAmbient(m_ambient);
	
	m_color = max(m_color, 1.0f);

	m_pointLight.SetPointLight(3, m_position, Vector3(m_color, m_color, m_color), m_range);
	m_pointLight.Update();
}

void FlashBom::SetFlashEffect()
{
	//RenderingEngine::GetInstance()->SetAmbient(10.0f);
	//ポイントライトの初期化
	m_color = MAXCOLOR;

	m_pointLight.SetPointLight(3,m_position, Vector3(m_color, m_color, m_color),m_range);
	m_pointLight.Update();
	
	//フラッシュ時の値を入れる
	m_alpha = MAXALPHA;
	m_range = MAXRANGE;
	m_ambient = MAXAMBIENT;
	RenderingEngine::GetInstance()->GetLightCB().ptNum = 4;
}

void FlashBom::ProcessStartState()
{
	m_flashCount -= 1;
	if (m_flashCount > 0)
	{
		//開始時に必要な物を呼ぶ

		SetFlashEffect();
		ItemHit();
		m_FlashState = m_enFlash_Flashing;
	}
	else
	{
		m_FlashState = m_enFlash_Flashing;
	}
	
}

void FlashBom::ProcessFlashingState()
{
	//フラッシュが終わったらすべてを初期化する
	if (m_alpha <= 0.0f && m_range <= 0.0f)
	{
		m_pointLight.SetPointLight(3, m_position, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
		m_pointLight.Update();
		m_alpha = 0.0f;
		RenderingEngine::GetInstance()->SetAmbient(0.7f);
		m_FlashState = m_enFlash_No;
		RenderingEngine::GetInstance()->GetLightCB().ptNum = 3;
	}
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