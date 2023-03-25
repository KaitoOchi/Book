#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"
#include "GameUI.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 120.0f			// エネミーの視野角(初期:120)
#define SEACH_DECISION 300.0f * 300.0f						// ベクトルを作成する範囲

namespace
{
	const float		MOVE_SPEED = 3.0f;						// 移動速度
	const float		CHANGING_DISTANCE = 20.0f;				// 目的地を変更する距離
	const float		CALCULATIONNAVI_TIMER = 1.0f;			// ナビメッシュを再度計算するタイマー
	const float		CANMOVE_TIMER = 10.0f;					// 再度行動できるまでのタイマー
	const float		WAITING_TIMER = 3.0f;					// パス移動時の待機時間
	const float		AI_RADIUS = 50.0f;						// AIエージェントの半径
	const float		AI_HIGH = 200.0f;						// AIエージェントの高さ
	const float		CATCH_DECISION = 60.0f;					// プレイヤーを確保したことになる範囲
	const float		ACT_LIMIT = 300.0f;						// プレイヤーに近づける範囲
	const float		SCALESIZE = 1.3f;						// SetScaleのサイズ
	const float     VIGILANCETIME = 1.0f;					//警戒度UP時間
	const Vector3	BOXSIZE = { 75.0f, 90.0f,60.0f };		// CharacterControllerのサイズ
	const float		ANGLE = 45.0f;							//��]�p�x
	const Vector3   LIGHTCOLOR(100.0f, 1.0f, 1.0f);			//���C�g�̃J���[
	const float		LIGHTRANGE = 300.0f;						//���C�g�̉e���͈�
	const float		LIGHTPOSITION = 40.0f;						//���C�g�̃|�W�V����
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//警戒度時間を代入
	m_Vicount = VIGILANCETIME;

	// キャラクターコントローラーを初期化する
	m_characterController.Init(BOXSIZE, m_position);
	// スフィアコライダーを初期化
	m_sphereCollider.Create(1.0f);

	// ナビメッシュを構築
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");

	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_gameUI = FindGO<GameUI>("gameUI");

	return true;
}

bool Enemy::SeachPlayer()
{
	// 視野角の処理
	// trueのときプレイヤーを発見している

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	m_playerPos = m_playerManagement->GetPosition();

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerPos - m_position;

	// プレイヤーにある程度近いとき
	if (diff.LengthSq() <= SEACH_DECISION) {
		// エネミーからプレイヤーへ向かうベクトルを正規化
		diff.Normalize();
		// エネミーの正面ベクトル、エネミーからプレイヤーへ向かうベクトルの内積を計算
		float cos = m_forward.Dot(diff);
		// 内積から角度を計算
		float angle = acosf(cos);
		// 角度が視野角より狭いとき
		if (angle <= (FIELDOF_VIEW) {
			// 壁との衝突判定を行う
			return WallAndHit(m_playerPos);
		}
	}

	return false;
}

// 衝突した際に呼ばれる関数オブジェクト(壁用)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;		// 衝突フラグ

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		// 壁と衝突していないとき
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			// 衝突したのは壁ではない
			return 0.0f;
		}

		// 壁と衝突したとき
		// フラグをtrueにする
		isHit = true;
		return 0.0f;
	}
};

bool Enemy::WallAndHit(Vector3 pos)
{
	// 障害物を考慮した視野角の処理
	// プレイヤーを発見したときtrueを返す

	btTransform start, end;

	start.setIdentity();
	end.setIdentity();

	// 始点はエネミーの座標
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	// 終点はプレイヤーの座標
	end.setOrigin(btVector3(pos.x, pos.y + 70.0f, pos.z));

	SweepResultWall callback;

	// コライダーを始点から終点まで動かして、
	// 衝突するかどうかを調べる
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	// 壁と衝突した
	if (callback.isHit == true) {
		// プレイヤーは見つかっていない
		return false;
	}

	// 壁と衝突していない
	return true;
}

bool Enemy::CatchPlayer()
{
	// プレイヤーを確保する処理
	// trueのときプレイヤーを確保している

	// エネミーからプレイヤーへ向かうベクトルを計算する
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// ベクトルの長さを求める
	float length = diff.Length();

	// ベクトルが一定以下のとき
	if (length <= CATCH_DECISION) {
		// 捕まえる処理を行う
		return true;
	}

	return false;
}

void Enemy::HitFlashBullet()
{
	// 閃光弾が当たったとき
	// trueなら当たった
	if (HitFlashBulletFlag == true) {
		// 被弾アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Damege;

		// タイマーがtrueのとき
		if (Act_Stop(CANMOVE_TIMER) == true) {
			HitFlashBulletFlag = false;		// フラグを降ろす
			addTimer = 0.0f;				// 加算用タイマーをリセット

		}
		// そうでないとき
		else {
			// 待機アニメーションを再生
			m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
		}
	}
}

void Enemy::Act_Craw()
{
	// パス移動
	
	// 追跡から状態を切り替えたとき
	//if (ChangeCrawFlag == true) {

	//	// 一番近いパスを探す
	//	for (int i = 1; i <= m_pointList.size(); i++) {

	//		m_point = &m_pointList[i];
	//		Vector3 diff = m_point->s_position - m_position;

	//		for (int j = i + 1; j <= m_pointList.size(); j++) {

	//			m_point = &m_pointList[j];
	//			Vector3 diff2 = m_point->s_position - m_position;

	//			// 長さを比較
	//			// diff2が長いとき
	//			if (diff.Length() < diff2.Length()) {
	//				// numberを格納する
	//				m_point->s_number = j;
	//			}
	//		}
	//	}
	//}
	
	// 目標とするポイントの座標から、現在の座標を引いたベクトル
	Vector3 diff = m_point->s_position - m_position;

	// 距離が一定以内なら目的地とするポイントを変更する
	if (diff.Length() <= CHANGING_DISTANCE) {

		// 現在の目的地のポイントが配列の最後のとき
		if (ChangeCrawFlag == false && m_point->s_number == m_pointList.size()) {
			// 一番最初のポイントを目的地とする
			m_point = &m_pointList[0];
		}
		// そうでないとき
		else {
			m_point = &m_pointList[m_point->s_number];
		}

		addTimer = 0.0f;	// 加算用タイマーをリセット
	}
	
	// フラグを戻す
	//ChangeCrawFlag = false;

	// 目標とするポイントの座標から、現在の座標を引いたベクトル
	Vector3 moveSpeed = m_point->s_position - m_position;
	// 正規化
	moveSpeed.Normalize();
	// ベクトルにスカラーを乗算
	moveSpeed *= MOVE_SPEED;

	// タイマーがtrueのとき
	if (Act_Stop(WAITING_TIMER) == true) {
		// 待機アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
		// 座標に加算する
		m_position += moveSpeed;
	}
	// そうでないとき
	else {
		// 歩きアニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
	}
}

void Enemy::Act_Tracking()
{
	// 経過時間を加算
	NaviTimer += g_gameTime->GetFrameDeltaTime();

	// 一定時間以下のときreturn
	if (CALCULATIONNAVI_TIMER >= NaviTimer) {
		return;
	}

	// ナビメッシュでの移動
	// プレイヤーの座標を獲得する
	m_playerPos = m_playerManagement->GetPosition();

	bool isEnd;							// パス移動が終了した際のフラグ

	// パス検索
	m_pathFiding.Execute(
		m_path,							// 構築されたパスの格納先
		m_nvmMesh,						// ナビメッシュ
		m_position,						// 開始座標
		m_playerPos,					// 目標地点
		PhysicsWorld::GetInstance(),	// 物理エンジン
		AI_RADIUS,						// AIエージェントの半径
		AI_HIGH							// AIエージェントの高さ
	);

	// パス上を移動する
	m_position = m_path.Move(
		m_position,						// 移動させる座標
		MOVE_SPEED,						// 移動速度
		isEnd							// 終了時にtrueを格納するフラグ
	);

	// 歩行アニメーションを再生
	m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
}

void Enemy::Act_Access()
{
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// ベクトルの長さ
	float length = diff.Length();

	// ベクトルが一定以下のとき
	if (length <= SEACH_DECISION) {
		// ベクトルを正規化
		diff.Normalize();
		// エネミーの座標に加算
		m_position += diff * MOVE_SPEED;
		// 歩きアニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
	}
}

void Enemy::Act_Limit()
{
	// 一定の距離には近づかない
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerManagement->GetPosition() - m_position;

	// 長さが一定以下のとき
	if ( diff.Length() <= ACT_LIMIT) {
		// 動かないようにする
		m_position = m_position;
	}
}

bool Enemy::Act_Stop(float time)
{
	// 経過時間を加算
	addTimer += g_gameTime->GetFrameDeltaTime();

	// 加算された時間が一定以上になったとき
	if (time <= addTimer) {
		return true;
	}

	return false;
}

void Enemy::SpotLight_New(Vector3 position)
{
	m_spotLight.SetPosition(position);
	m_spotLight.SetColor(LIGHTCOLOR);
	m_spotLight.SetRange(LIGHTRANGE);
	m_spotLight.SetAngle(ANGLE);
	Vector3 forward = Vector3::AxisY;
	//ライトの方向設定
	m_spotLight.SetDirection(forward);
	m_spotLight.Update();
}
void Enemy::SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos)
{
	lightpos.y = LIGHTPOSITION;
	//Y軸
	Vector3 m_Yup = Vector3(0.0f, 1.0f, 0.0f);
	//プレイヤーの正面
	Vector3 m_front = Vector3(0.0f, 0.0f, 1.0f);
	lightrotaition.Apply(m_front);
	//その二つの垂直なベクトル
	Vector3 m_vertical = Cross(m_Yup, m_front);
	Quaternion m_SitenRot;
	//その垂直なベクトルを元にクォータニオンを作る
	m_SitenRot.SetRotationDeg(m_vertical, ANGLE);
	//ベクトルにクォータニオンを加算する
	m_SitenRot.Apply(m_front);
	m_spotLight.SetDirection(m_front);

	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true)
	{
		VigilanceCount();
	}
	m_spotLight.SetPosition(lightpos);
	m_spotLight.Update();
}
void Enemy::VigilanceCount()
{
	m_Vicount -= g_gameTime->GetFrameDeltaTime();
	if (m_Vicount <= 0.0f) 
	{
		//ステートの遷移
		m_gameUI->Vigilance(1);
		m_Vicount = VIGILANCETIME;
	}
}