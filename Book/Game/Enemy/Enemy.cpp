#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"
#include "Gage.h"
#include "Game.h"
#include "GameManager.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 75.0f				// エネミーの視野角(初期:120)
#define SEACH_DECISION 200.0f * 200.0f						// ベクトルを作成する範囲

namespace
{
	const Vector3	MODEL_SCALE = { 2.0f,2.0f,2.0f };		// モデルのスケール

	const float		LINEAR_COMPLETION = 0.5f;				// 線形補完

	const float		MOVE_SPEED = 3.0f;						// 移動速度
	const float		ADD_SPEED = 3.0f;						// 乗算速度
	const float		MOVING_DISTANCE = 400.0f;				// 移動距離
	const float		CALL_DISTANCE_MAX = 350.0f;				// 呼ぶことができる最大値
	const float		CALL_DISTANCE_MIN = 70.0f;				// 呼ぶことができる最小値
	const float		CHANGING_DISTANCE = 20.0f;				// 目的地を変更する距離
	const float		CALCULATIONNAVI_TIMER = 1.0f;			// ナビメッシュを再度計算するタイマー
	const float		CANMOVE_TIMER = 10.0f;					// 再度行動できるまでのタイマー
	const float		WAITING_TIMER = 3.0f;					// パス移動時の待機時間
	const float		AI_RADIUS = 50.0f;						// AIエージェントの半径
	const float		AI_HIGH = 200.0f;						// AIエージェントの高さ
	const float		CATCH_DECISION = 60.0f;					// プレイヤーを確保したことになる範囲
	const float		ACT_LIMIT = 300.0f;						// プレイヤーに近づける範囲
	const float		SCALESIZE = 1.3f;						// SetScaleのサイズ
	const float		ADD_LENGTH = 150.0f;					// 突進時に追加する長さ

	const float     VIGILANCETIME = 0.3f;					//警戒度UP時間
	const Vector3	BOXSIZE = { 60.0f, 80.0f,60.0f };		// CharacterControllerのサイズ
	const float		ANGLE = 45.0f;							//��]�p�x
	const Vector3   LIGHTCOLOR(25.0f, 1.0f, 0.0f);		//���C�g�̃J���[
	const float		LIGHTRANGE = 300.0f;					//���C�g�̉e���͈�
	const float		LIGHTPOSITION = 80.0f;					//���C�g�̃|�W�V����

	const float		ADD_MOVE_MIN = 250.0f;
	const float		ADD_MOVE_LONG = 400.0f;
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	SetScale(MODEL_SCALE);

	//警戒度時間を代入
	m_Vicount = VIGILANCETIME;

	// キャラクターコントローラーを初期化する
	m_characterController.Init(BOXSIZE, m_position);

	// スフィアコライダーを設定
	m_sphereCollider.Create(18.0f);

	// ナビメッシュを構築
	m_nvmMesh.Init("Assets/modelData/level_test/nav_test.tkn");

	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_gage = FindGO<Gage>("gage");
	m_game = FindGO<Game>("game");

	// gameで設定したエネミーのリストを取得する
	enemyList = m_game->GetEnemyList();

	// 各タイマーのリセット
	for (int i = 0; i < TIMER_NUM; i++) {
		m_addTimer[i] = 0.0f;
	}

	// 視野を作成
	SpotLight_New(m_position, m_spotNum);

	//----------------------------------------------
	m_foward = Vector3::AxisZ;
	m_rotation.Apply(m_foward);
	//----------------------------------------------

	return true;
}

void Enemy::Animation()
{
	// アニメーションの読み込み
	m_enAnimationClips[m_enAnimation_Idle].Load("Assets/animData/enemy/idle.tka");
	m_enAnimationClips[m_enAnimation_Idle].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Walk].Load("Assets/animData/enemy/walk_battle.tka");
	m_enAnimationClips[m_enAnimation_Walk].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Run].Load("Assets/animData/enemy/run_battle.tka");
	m_enAnimationClips[m_enAnimation_Run].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Attack].Load("Assets/animData/enemy/attack.tka");
	m_enAnimationClips[m_enAnimation_Attack].SetLoopFlag(false);

	m_enAnimationClips[m_enAnimation_Damege].Load("Assets/animData/enemy/damege.tka");
	m_enAnimationClips[m_enAnimation_Damege].SetLoopFlag(false);

	m_enAnimationClips[m_enAnimation_Flash].Load("Assets/animData/enemy/dizzy.tka");
	m_enAnimationClips[m_enAnimation_Flash].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Loss].Load("Assets/animData/enemy/search.tka");
	m_enAnimationClips[m_enAnimation_Loss].SetLoopFlag(true);
}

void Enemy::PlayAnimation()
{
	// 行動パターンで再生アニメーションを変動
	switch (m_enAnimationState)
	{
	case IDLE:
		m_enemyRender.PlayAnimation(m_enAnimation_Idle, LINEAR_COMPLETION);
		break;
	case WALK:
		m_enemyRender.PlayAnimation(m_enAnimation_Walk, LINEAR_COMPLETION);
		break;
	case RUN:
		m_enemyRender.PlayAnimation(m_enAnimation_Run, LINEAR_COMPLETION);
		break;
	case ATTACK:
		m_enemyRender.PlayAnimation(m_enAnimation_Attack, LINEAR_COMPLETION);
		break;
	case DAMEGE:
		m_enemyRender.PlayAnimation(m_enAnimation_Damege, LINEAR_COMPLETION);
		break;
	case FLASH:
		m_enemyRender.PlayAnimation(m_enAnimation_Flash, LINEAR_COMPLETION);
		break;
	case LOSS:
		m_enemyRender.PlayAnimation(m_enAnimation_Loss, LINEAR_COMPLETION);
		// 再生速度を速くする
		m_enemyRender.SetAnimationSpeed(20.0f);
		break;
	}
}

void Enemy::Rotation(Vector3 rot)
{
	// 回転
	m_rotation.SetRotationYFromDirectionXZ(rot);
	m_enemyRender.SetRotation(m_rotation);

	//---------------------------------------------
	//エネミーの前ベクトルを求める
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	//---------------------------------------------

}

void Enemy::Nav(Vector3 pos)
{
	// タイマーを加算
	m_NaviTimer += g_gameTime->GetFrameDeltaTime();

	// 一定時間以下のときreturn
	if (CALCULATIONNAVI_TIMER >= m_NaviTimer) {
		return;
	}

	// ナビメッシュ
	bool isEnd;							// フラグ

	// パス検索
	m_pathFiding.Execute(
		m_path,							// 構築されたパスの格納先
		m_nvmMesh,						// ナビメッシュ
		m_position,						// 開始座標
		pos,							// 移動目標座標
		PhysicsWorld::GetInstance(),	// 物理エンジン
		AI_RADIUS,						// AIエージェントの半径
		AI_HIGH							// AIエージェントの高さ
	);

	// パス上を走って移動する
	m_position = m_path.Move(
		m_position,						// 座標
		MOVE_SPEED * ADD_SPEED,			// 移動速度（パス移動よりも速め）
		isEnd							// 移動したときtrue
	);

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 moveSpeed = pos - m_position;

	// 回転を教える
	float angle = atan2(-moveSpeed.x, moveSpeed.z);
	Quaternion rot = Quaternion::Identity;
	rot.SetRotationY(-angle);

	// 回転を教える
	m_enemyRender.SetRotation(rot);
}

bool Enemy::Act_SeachPlayer()
{
	// 索敵
	// trueのときプレイヤーを発見している

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	m_playerPos = m_playerManagement->GetPosition();
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerPos - m_position;

	// スポットライトの中にプレイヤーがいるとき
	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true) {
		// 正規化
		diff.Normalize();
		// 内積を計算
		float cos = m_forward.Dot(diff);
		// 角度を計算
		float angle = acosf(cos);
		// 角度が視野角内のとき
		if (angle <= (FIELDOF_VIEW) {
			// 衝突判定を行う
			return WallAndHit(m_playerPos);
		}
	}

	return false;
}

// 衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;		// 衝突フラグ

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		// 壁とぶつかっていなかったら
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			// 衝突したのは壁ではない
			return 0.0f;
		}

		// 壁とぶつかったら
		// フラグをtrueにする
		isHit = true;
		return 0.0f;
	}
};

bool Enemy::WallAndHit(Vector3 pos)
{
	btTransform start, end;

	start.setIdentity();
	end.setIdentity();

	// 始点はエネミーの座標
	start.setOrigin(btVector3(m_position.x, m_position.y + 20.0f, m_position.z));

	// 終点はプレイヤーの座標 (突進時は始点の少し前)
	end.setOrigin(btVector3(pos.x, m_position.y + 20.0f, pos.z));

	SweepResultWall callback;

	// コライダーを始点から終点まで動かして、
	// 衝突するかどうか調べる
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	// 壁と衝突した
	if (callback.isHit == true) {
		// プレイヤーは見つかっていない
		//if (m_ActState == EnEnemyActState::CHARGE) {
		//	int hoge = 0;
		//}
		return false;
	}

	// 壁と衝突していない
	// プレイヤーを見つけた
	return true;
}

bool Enemy::Act_CatchPlayer()
{
	// プレイヤーを捕まえる処理
	// trueのときプレイヤーを捕まえた

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// 長さを計算する
	float length = diff.Length();

	// 一定の長さのとき
	if (length <= CATCH_DECISION) {
		// 捕まえる
		m_fontRender.SetText(L"catch");
		m_fontRender.SetPosition(Vector3(-500.0f, 0.0f, 0.0f));

		for (int i = 0; i < enemyList.size(); i++) {
			// 捕まえたのでフラグをtrueにする
			enemyList[i]->m_ChachPlayerFlag = true;
		}

		return true;
	}

	return false;
}

void Enemy::Act_MissingPlayer()
{
	// プレイヤーを見失った時の処理

	if (m_FindPlayerFlag == false) {
		// 座標を参照
		m_playerPos3 = m_playerManagement->GetPosition();
		// 何度も参照しないようにする
		m_FindPlayerFlag = true;
	}

	// ベクトルを作成
	Vector3 moveSpeed = m_playerPos3 - m_position;

	// 長さを計算
	float length = moveSpeed.Length();

	// 移動する
	moveSpeed.Normalize();
	//移動速度に加算
	moveSpeed *= MOVE_SPEED;

	// 座標が一定範囲内になるまで
	if (length < 50.0f) {
		m_position += moveSpeed;
	}

	// 回転を教える
	Rotation(moveSpeed);

	// 歩きモーションを再生
	m_enAnimationState = WALK;

	// モーションを再生
	if (Act_Stop(3.0f, 4) == false) {
		// 見渡すモーションを再生
		m_enAnimationState = LOSS;
	}
}

void Enemy::Act_HitFlashBullet()
{
	// 閃光弾が当たったとき
	// trueのとき当たった

	// 当たったとき
	if (m_HitFlashBulletFlag == true) {

		// 被弾アニメーションを再生
		m_enAnimationState = DAMEGE;

		// タイマーがtrueのとき
		if (Act_Stop(CANMOVE_TIMER,0) == true) {
			m_HitFlashBulletFlag = false;		// フラグを降ろす
			m_addTimer[0] = 0.0f;				// タイマーをリセット

			// 見渡すアニメーションを再生
			m_enAnimationState = LOSS;
		}
	}
}

bool Enemy::Act_HitSoundBullet()
{
	// 音爆弾の処理
	// trueのとき当たった

	// 当たった(プレイヤーが使用した)とき
	if (m_HitSoundBulletFlag == true) {

		// エネミーからアイテムへ向かうベクトルを作成
		Vector3 diff = m_itemPos - m_position;
		float length = diff.Length();

		// 一定範囲内より小さいとき
		if (length < CALL_DISTANCE_MAX) {
			// アイテムの座標を基にしてナビメッシュを作成
			Nav(m_itemPos);
			// 走るアニメーションを再生
			m_enAnimationState = RUN;

			// アイテムを使用した位置についたとき
			if (length > 20.0f && length < 500.0f) {
				// 見渡すアニメーションを再生
				m_enAnimationState = LOSS;
				return true;
			}
		}
	}

	return false;
}

void Enemy::Act_Craw()
{
	// 巡回
	
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_point->s_position - m_position;

	// 長さが一定のとき
	if (diff.Length() <= CHANGING_DISTANCE) {

		// パスが最後のとき
		if (m_point->s_number == m_pointList.size()) {
			// 最初の値を渡す
			m_point = &m_pointList[0];
		}
		// そうでないとき
		else {
			m_point = &m_pointList[m_point->s_number];
		}

		m_addTimer[1] = 0.0f;	// タイマーをリセット
	}

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 moveSpeed = m_point->s_position - m_position;
	// 正規化
	moveSpeed.Normalize();
	// 回転を教える
	moveSpeed *= MOVE_SPEED;
	Rotation(moveSpeed);

	// タイマーがtrueのとき
	if (Act_Stop(WAITING_TIMER,1) == true) {
		// 歩きアニメーションを再生
		m_enAnimationState = WALK;
		// 座標に加算
		m_position += moveSpeed;
	}
	else {
		// 歩きアニメーションを再生
		m_enAnimationState = IDLE;
	}
}

void Enemy::Act_Tracking()
{
	// プレイヤーの座標
	m_playerPos = m_playerManagement->GetPosition();
	// ナビメッシュを作成
	Nav(m_playerPos);

	// 走るアニメーションを再生
	m_enAnimationState = RUN;
}

void Enemy::Pass(int PassState)
{
	switch (PassState)
	{
		// 縦
	case LINE_VERTICAL:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_LONG),2 });
		break;
		// 横
	case LINE_HORIZONTAL:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_LONG,m_position.y,m_position.z),2 });
		break;
		// 右回り(正方形)
	case SQUARE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_MIN,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_MIN,m_position.y,m_position.z - ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_MIN),4 });
		break;
		// 左回り(正方形)
	case SQUARE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_MIN,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_MIN,m_position.y,m_position.z - ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z+ ADD_MOVE_MIN),4 });
		break;
		// (右に)直角
	case ANGLE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_LONG),2 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_LONG,m_position.y,m_position.z - ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_LONG),4 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),5 });
		break;
		// (左に)直角
	case ANGLE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_LONG),2 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_LONG,m_position.y,m_position.z - ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + ADD_MOVE_LONG),4 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),5 });
		break;
		// 右回り(長方形)
	case RECTANGLE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + ADD_MOVE_MIN),2 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_LONG ,m_position.y,m_position.z + ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_LONG,m_position.y,m_position.z),4 });
		break;
		// 左回り(長方形)
	case RECTANGLE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + ADD_MOVE_MIN),2 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_LONG ,m_position.y,m_position.z + ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_LONG,m_position.y,m_position.z),4 });
	}
}

void Enemy::Act_Access()
{
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// 長さを計算
	float length = diff.Length();
	// 回転を教える
	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();
	Rotation(rot);

	// 長さが一定のとき
	if (length <= SEACH_DECISION) {
		// 正規化
		diff.Normalize();
		// 移動速度に加算
		Vector3 moveSpeed = diff * MOVE_SPEED;
		m_position += moveSpeed;

		// 歩きアニメーションを再生
		m_enAnimationState = WALK;
	}

}

void Enemy::Act_Charge(float time)
{
	// 壁に衝突する判定
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerPos - m_position;
	// 正規化
	diff.Normalize();

	// 壁に衝突したとき
	// プレイヤーの方向へ向かう単位ベクトルにスカラーを乗算したものを加算して渡す
	if (Enemy::WallAndHit(m_position + (diff * ADD_LENGTH)) == false) {
		m_move = 0.0f;
		m_CalculatedFlag = false;

		m_ActState = BACKBASEDON;		// 状態を移行
		return;
	}
	else {
		m_move = 1.0f;
	}

	// 回転
	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();
	Rotation(rot);

	// 待機アニメーションを再生
	m_enAnimationState = IDLE;

	// タイマーがtrueのとき
	if (Act_Stop(time,2) == true) {

		// 一度だけ実行する
		if (m_CalculatedFlag == false) {

			// 座標を参照
			m_enemyPos = m_position;
			m_playerPos2 = m_playerManagement->GetPosition();

			// 何度も実行しないようにtrueにする
			m_CalculatedFlag = true;
		}

		// エネミーからプレイヤーへ向かうベクトル
		Vector3 diff = m_playerPos2 - m_enemyPos;
		diff.Normalize();
		// 回転
		Rotation(diff);

		// 移動速度に加算
		Vector3 moveSpeed = diff * (MOVE_SPEED * ADD_SPEED);
		m_position += moveSpeed * m_move;
		// 総移動距離を計算
		m_sumPos += moveSpeed;

		// 走るアニメーションを再生
		m_enAnimationState = RUN;

		// 長さが一定以上のとき
		if (m_sumPos.Length() > MOVING_DISTANCE) {
			
			m_position = m_position;	// 座標を固定

			m_addTimer[2] = 0.0f;			// タイマーをリセット
			m_sumPos = Vector3::Zero;		// 移動距離をリセット
			m_CalculatedFlag = false;		// フラグを降ろす

			// プレイヤーが視野角内にいるとき
			if (Act_SeachPlayer() == true) {
				return;
			}
			// いないときは巡回状態に戻る
			m_ActState = BACKBASEDON;
		}
	}
}

void Enemy::SearchPass(EnEnemyActState state)
{
	// 経路探索をしない
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	if (diff.Length() < 1500.0f) {
		m_ActState = state;
	}
	else {
		m_ActState = NOOP;
	}
}

void Enemy::Act_Call()
{
	// 周りの敵を呼ぶ処理

	// エネミーのリストを検索
	for (int i = 0; i < enemyList.size(); i++) {

		// 各エネミーから該当エネミーへ向かうベクトル
		Vector3 diff = m_position - enemyList[i]->m_position;
		float length = diff.Length();

		// 長さが一定以内のとき
		if (length > CALL_DISTANCE_MIN && length < CALL_DISTANCE_MAX) {

			enemyList[i]->m_ActState = CALLED;				// 行動パターンを変更する
			enemyList[i]->m_setPos = m_position - BOXSIZE;	// 自身の座標-キャラコンを目標地点として渡す

			// 長さが一定上のとき
			if (length <= CALL_DISTANCE_MIN) {

				m_ActState = CALLEND;
				return;
			}

			// 正規化
			diff.Normalize();
			// 移動速度を加算
			enemyList[i]->m_position += diff * MOVE_SPEED;

			m_fontRender.SetText(L"call");
		}
	}
}

void Enemy::Act_Called()
{
	// 呼ばれた時の処理

	// 目標地点へ向かうナビメッシュを作成
	Nav(m_setPos);

	// 走るアニメーションを再生
	m_enAnimationState = RUN;

	// 自身から目標へ向かうベクトル
	Vector3 diff = m_setPos - m_position;

	//　長さが一定になったとき
	if (diff.Length() <= CALL_DISTANCE_MIN) {
		// 移動を停止する
		m_position = m_position;
		// 待機アニメーションを再生
		m_enAnimationState = IDLE;
	}
}

bool Enemy::Act_CallEnd()
{
	// エネミーのリストを検索
	for (int i = 0; i < enemyList.size(); i++) {

		//// 各エネミーから該当エネミーへ向かうベクトル
		//Vector3 diff = m_position - enemyList[i]->m_position;
		//float length = diff.Length();

		m_fontRender.SetText(L"callend");

		// 行動パターンがCALLのとき
		if (enemyList[i]->m_ActState == CALLED) {

			enemyList[i]->m_ActState = BACKBASEDON;		// 行動パターンを変更する
		}
	}

	return true;
}

void Enemy::Act_Loss()
{
	m_addTimer[1] = 0.0f;	// タイマーをリセット
	m_addTimer[2] = 0.0f;	// 突進からの移行時にこちらのタイマーもリセット

	// 直近のパスを検索
	// floatに最大値を格納
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	int NowTargetNum = -1;

	// パスの個数文回す
	for (int i = 0; i < m_pointList.size(); i++) {

		m_point = &m_pointList[i];
		Vector3 diff = m_point->s_position - m_position;
		float length = diff.Length();

		// 再度計算した長さが現在保存している長さより短いとき
		if (NowTargetDiff > length) {
			// 値を更新する
			NowTargetNum = i;
			NowTargetDiff = length;
		}
	}

	// 最短のパスの情報を渡す
	m_point = &m_pointList[NowTargetNum];

	Nav(m_point->s_position);

	//// エネミーからプレイヤーへ向かうベクトル
	//Vector3 moveSpeed = m_point->s_position - m_position;
	//// 正規化
	//moveSpeed.Normalize();
	//// 移動速度に乗算
	//moveSpeed *= MOVE_SPEED;
	//// 回転
	//Rotation(moveSpeed);

	// 歩くアニメーションを再生
	m_enAnimationState = RUN;

	return;
}

void Enemy::Act_Limit()
{
	// 近づける距離
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerManagement->GetPosition() - m_position;

	// 長さが一定以内のとき
	if ( diff.Length() < ACT_LIMIT) {
		// 座標を固定
		m_position = m_position;
	}
}

bool Enemy::Act_Stop(float time,int i)
{
	// フレームを加算
	m_addTimer[i] += g_gameTime->GetFrameDeltaTime();

	// タイマーが一定以上になったら
	if (time <= m_addTimer[i]) {
		return true;
	}

	return false;
}

void Enemy::SpotLight_New(Vector3 position, int num)
{
	Vector3 forward = Vector3::AxisY;

	m_spotLight.SetSpotLight(
		num,
		position,
		LIGHTCOLOR,
		LIGHTRANGE,
		forward,
		ANGLE
	);
}

void Enemy::SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos)
{
	lightpos.y = LIGHTPOSITION;
	//Y��
	Vector3 m_Yup = Vector3(0.0f, 1.0f, 0.0f);
	//�v���C���[�̐���
	Vector3 m_front = Vector3(0.0f, 0.0f, 1.0f);
	lightrotaition.Apply(m_front);
	//���̓�̐����ȃx�N�g��
	Vector3 m_vertical = Cross(m_Yup, m_front);
	Quaternion m_SitenRot;
	//���̐����ȃx�N�g������ɃN�H�[�^�j�I������
	m_SitenRot.SetRotationDeg(m_vertical, ANGLE);
	//�x�N�g���ɃN�H�[�^�j�I������Z����
	m_SitenRot.Apply(m_front);
	m_spotLight.SetDirection(m_front);
	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true)
	{
		VigilanceCount();

		// カウントするフラグがfalseのとき
		if (m_CountFlag == false) {
			// 発見回数に1を加算
			GameManager::GetInstance()->AddSearchNum();
			// フラグをtrueにして再度行わないようにする
			m_CountFlag = true;
		}
	}
	else {
		// プレイヤーを確保していないとき
		if (m_ChachPlayerFlag == false) {
			// プレイヤーが存在しないときはフラグを戻す
			m_CountFlag = false;
		}
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
		m_gage->GageUp(1);
		m_Vicount = VIGILANCETIME;
	}
}