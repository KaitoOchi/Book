#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"
#include "Gage.h"
#include "Game.h"
#include "GameManager.h"
#include "Treasure.h"

#define SEACH_DECISION	100.0f * 100.0f									// ベクトルを作成する範囲

namespace
{
	const Vector3	BOXSIZE = { 60.0f, 80.0f,60.0f };		// CharacterControllerのサイズ
	const Vector3	MODEL_SCALE = { 2.0f,2.0f,2.0f };		// モデルのスケール

	const float		LINEAR_COMPLETION = 1.0f;				// 線形補完

	const float		MOVE_SPEED = 3.0f;						// 移動速度
	const float		ADD_SPEED = 1.3f;						// 乗算速度

	const float		MOVING_DISTANCE = 600.0f;				// 突進する移動距離

	const float		CALL_DISTANCE_MAX = 400.0f;				// 呼ぶことができる最大値
	const float		CALL_DISTANCE_MIN = 190.0f;				// 呼ぶことができる最小値

	const float		CHANGING_DISTANCE = 20.0f;				// 目的地を変更する距離

	const float		CANMOVE_TIMER = 5.5f;					// 再度行動できるまでの待機時間
	const float		WAITING_TIMER = 3.0f;					// パス移動時の待機時間
	const float		SEARCHPLAYER_TIMER = 7.0f;				// プレイヤーを見失った時の待機時間

	const float		ADD_MOVE_MIN = 250.0f;					// パス移動
	const float		ADD_MOVE_LONG = 400.0f;					// パス移動

	const float		AI_RADIUS = 50.0f;						// AIエージェントの半径
	const float		AI_HIGH = 200.0f;						// AIエージェントの高さ
	const float		CALCULATIONNAVI_TIMER = 1.0f;			// ナビメッシュを再度計算するタイマー

	const float		CATCH_DECISION = 60.0f;					// プレイヤーを確保したことになる範囲

	const float		ADD_LENGTH = 150.0f;						// 突進時に追加する長さ

	const float     VIGILANCETIME = 0.3f;					// 警戒度UP時間

	const float		ANGLE = 45.0f;							//��]�p�x
	const Vector3   LIGHTCOLOR(1.5f, 0.3f, 0.0f);			//���C�g�̃J���[
	const float		LIGHTRANGE = 600.0f;					//���C�g�̉e���͈�
	const float		LIGHTPOSITION = 80.0f;					//���C�g�̃|�W�V����
	const Vector3	LIGHT_DIRECTION = { 0.0f, 1.0f, 1.0f };
}

Enemy::Enemy()
{
	//m_point = new Point;
}

Enemy::~Enemy()
{
	m_pointList.clear();
	m_pointList.shrink_to_fit();

	if (m_Effect != nullptr) {
		m_Effect->Stop();
		DeleteGO(m_Effect);
	}
}

bool Enemy::Start()
{
	// スケールを設定
	SetScale(MODEL_SCALE);

	// 行動パターンを初期化
	if (m_enemyType == TYPE_SEARCH) {
		m_ActState = SEARCH;
	}
	else {
		m_ActState = CRAW;
	}

	//警戒度時間を代入
	m_Vicount = VIGILANCETIME;

	// キャラクターコントローラーを初期化する
	m_characterController.Init(BOXSIZE, m_position);

	// スフィアコライダーを設定
	m_sphereCollider.Create(17.0f);

	// ナビメッシュを構築
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");

	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_gage = FindGO<Gage>("gage");
	m_game = FindGO<Game>("game");
	m_treasure = FindGO<Treasure>("treasure");

	// 各タイマーのリセット
	for (int i = 0; i < 4; i++) {
		m_addTimer[i] = 0.0f;
	}

	// フラグのリセット
	for (int i = 0; i < 3; i++) {
		m_efectDrawFlag[i] = false;
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

	m_enAnimationClips[m_enAnimation_Dizzy].Load("Assets/animData/enemy/dizzy.tka");
	m_enAnimationClips[m_enAnimation_Dizzy].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Loss].Load("Assets/animData/enemy/search.tka");
	m_enAnimationClips[m_enAnimation_Loss].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Call].Load("Assets/animData/enemy/search_idle.tka");
	m_enAnimationClips[m_enAnimation_Call].SetLoopFlag(true);
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
	case DIZZY:
		m_enemyRender.PlayAnimation(m_enAnimation_Dizzy, LINEAR_COMPLETION);
		break;
	case LOSS:
		m_enemyRender.PlayAnimation(m_enAnimation_Loss, LINEAR_COMPLETION);
		break;
	case CALL:
		m_enemyRender.PlayAnimation(m_enAnimation_Call, LINEAR_COMPLETION);
		break;
	}
}

void Enemy::Efect_Dizzy()
{
	if (m_efectDrawFlag[0] == false) {

		// ☆のエフェクトを生成
		m_Effect = NewGO<EffectEmitter>(2);
		m_Effect->Init(2);
		// エフェクトの大きさを指定する
		m_Effect->SetScale(Vector3::One * 1.0f);
		// エフェクトの座標の設定
		m_Effect->SetPosition(Vector3(m_position.x + 5.0f, 100.0f, m_position.z + 10.0f));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_efectDrawFlag[0] = true;
	}
}

void Enemy::Efect_FindPlayer()
{
	if (m_efectDrawFlag[1] == false) {
		// !のエフェクトを生成
		m_Effect = NewGO<EffectEmitter>(3);
		m_Effect->Init(3);
		// エフェクトの大きさを指定する
		m_Effect->SetScale(Vector3::One * 1.2f);
		// エフェクトの座標の設定
		m_Effect->SetPosition(Vector3(m_position.x + 5.0f, 100.0f, m_position.z + 10.0f));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_efectDrawFlag[1] = true;
	}
}

void Enemy::Efect_MissingPlayer()
{
	if (m_efectDrawFlag[2] == false) {
		// ?のエフェクトを生成
		m_Effect = NewGO<EffectEmitter>(4);
		m_Effect->Init(4);
		// エフェクトの大きさを指定する
		m_Effect->SetScale(Vector3::One * 1.5f);
		// エフェクトの座標の設定
		m_Effect->SetPosition(Vector3(m_position.x + 5.0f, 100.0f, m_position.z + 10.0f));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_efectDrawFlag[2] = true;
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

void Enemy::CreateNavimesh(Vector3 pos)
{
	// タイマーを加算
	m_NaviTimer += g_gameTime->GetFrameDeltaTime();

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = pos - m_position;

	diff.Normalize();
	Rotation(diff);

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
}

void Enemy::Act_SeachPlayer()
{
	// 混乱時は何もしない
	if (m_ActState == CONFUSION) {
		return;
	}

	// スポットライトの中にプレイヤーがいるとき
	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true) {

		m_playerPos = m_playerManagement->GetPosition();

		// 衝突判定を行う
		if (WallAndHit(m_playerPos) == false) {
			// 壁に衝突したとき
			m_TrackingPlayerFlag = false;
			m_efectDrawFlag[1] = false;
			return;
		}

		// 追跡フラグをtrueにする
		m_TrackingPlayerFlag = true;
		// エフェクトを生成
		Efect_FindPlayer();
		return;
	}

	if (m_TrackingPlayerFlag == true) {
		m_playerPos = m_playerManagement->GetPosition();

		// 衝突判定を行う
		if (WallAndHit(m_playerPos) == false) {
			// 壁に衝突したとき
			m_TrackingPlayerFlag = false;
			m_efectDrawFlag[1] = false;
			return;
		}
	}
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
	start.setOrigin(btVector3(m_position.x,  20.0f, m_position.z));

	// 終点はプレイヤーの座標 (突進時は始点の少し前)
	end.setOrigin(btVector3(pos.x, 20.0f, pos.z));

	SweepResultWall callback;

	// コライダーを始点から終点まで動かして、
	// 衝突するかどうか調べる
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	// 壁と衝突した
	if (callback.isHit == true) {
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

		// プレイヤーの方向を向く
		diff.Normalize();
		Rotation(diff);
		// 捕まえる
		m_enAnimationState = ATTACK;

		for (int i = 0; i < m_game->GetEnemyList().size(); i++) {
			// ステートを変動させる
			m_game->GetEnemyList()[i]->m_ActState = CATCH;
		}

		return true;
	}

	return false;
}

void Enemy::Act_MoveMissingPosition()
{
	// ベクトルを作成
	Vector3 diff = m_playerMissionPosition - m_position;

	// 長さを計算
	float length = diff.Length();

	// 移動する
	diff.Normalize();

	// 移動速度に加算
	Vector3 moveSpeed = diff * (MOVE_SPEED * ADD_SPEED);
	m_position += moveSpeed * m_move;

	// 走るアニメーションを再生
	m_enAnimationState = RUN;

	if (length < 10.0f) {
		m_ActState = MISSING_SEARCHPLAYER;
		return;
	}

	// 回転を教える
	Rotation(moveSpeed);
}

void Enemy::Act_SearchMissingPlayer()
{
	// エフェクトを生成
	Efect_MissingPlayer();

	// 見渡すモーションを再生
	m_enAnimationState = LOSS;

	// プレイヤーを発見したとき
	if (m_TrackingPlayerFlag == true) {
		// 再度追跡する
		Efect_FindPlayer();
		m_NaviTimer = 0.0f;
		m_addTimer[3] = 0.0f;			// タイマーをリセット

		// 索敵するタイプなら
		if (m_enemyType == TYPE_SEARCH) {
			// 周りの敵を呼ぶ
			m_ActState = CALLING_AROUND_ENEMY;
			return;
		}
		else if (m_enemyType == TYPE_CHARGE) {
			// 突進する
			m_ActState = CHARGE;
			return;
		}

		m_ActState = TRACKING;
		return;
	}

	// モーションを再生
	if (Act_Stop(SEARCHPLAYER_TIMER, 3) == true) {

		m_efectDrawFlag[1] = false;		// エフェクトの描画フラグ
		m_efectDrawFlag[2] = false;

		m_addTimer[3] = 0.0f;			// タイマーをリセット
		m_sumPos = Vector3::Zero;		// 移動距離をリセット

		// 索敵するタイプなら
		if (m_enemyType == TYPE_SEARCH) {
			// 索敵状態に戻す
			m_ActState = SEARCH;
			return;
		}

		m_ActState = BACKBASEDON;
	}
}

void Enemy::Act_HitFlashBullet()
{
	// プレイヤーを追跡するフラグがtrueになったら
	if (m_TrackingPlayerFlag == true) {
		// falseにする
		m_TrackingPlayerFlag = false;
	}

	// プレイヤーを確保したフラグがtrueになったら
	if (m_ChachPlayerFlag == true) {
		// falseにする
		m_ChachPlayerFlag = false;
	}

	// めまいのアニメーションを再生
	m_enAnimationState = DIZZY;

	Efect_Dizzy();

	// タイマーがtrueのとき
	if (Act_Stop(CANMOVE_TIMER,0) == true) {
		// 生成フラグをリセット
		m_efectDrawFlag[0] = false;

		// フラグを降ろす
		m_TrackingPlayerFlag = false;
		m_HitFlashBulletFlag = false;

		// タイマーをリセット
		m_addTimer[0] = 0.0f;

		// エフェクトを生成
		Efect_MissingPlayer();

		// プレイヤーを探す
		m_ActState = MISSING_SEARCHPLAYER;

	}
	else {
		// エフェクトの生成フラグをfalseにしておく
		m_efectDrawFlag[2] = false;
	}
}

void Enemy::Act_GoLocationListenSound(Vector3 tergetPos)
{
	// プレイヤーを発見したとき
	if (m_TrackingPlayerFlag == true) {
		Efect_FindPlayer();

		// 突進タイプのとき
		if (m_enemyType == TYPE_CHARGE) {
			m_ActState = CHARGE;
			return;
		}
		else {
			m_ActState = TRACKING;
			return;
		}
	}

	Efect_FindPlayer();

	// エネミーからアイテムへ向かうベクトルを作成
	Vector3 diff = tergetPos - m_position;
	float length = diff.Length();

	// 長さが一定以上のとき
	if (length >= CALL_DISTANCE_MIN) {
		// アイテムの座標を基にしてナビメッシュを作成
		CreateNavimesh(tergetPos);

		// 経過時間を計測
		m_addTimer[4] += g_gameTime->GetFrameDeltaTime();

		// 走るアニメーションを再生
		m_enAnimationState = RUN;
		// エフェクトの再生フラグをfalseにしておく
		m_efectDrawFlag[2] = false;
	}
	// 長さが一定以上かつ一定時間が経過したとき(壁にぶつかったときの対処策)
	else if (m_addTimer[4] < 10.0f) {
		// 見失ったプレイヤーを探す
		m_ActState = MISSING_SEARCHPLAYER;
		m_HearedSoundBulletFlag = false;
		// タイマーをリセット
		m_addTimer[4] = 0.0f;
		m_efectDrawFlag[1] = false;
		return;
	}
	else {
		// 見失ったプレイヤーを探す
		m_ActState = MISSING_SEARCHPLAYER;
		m_HearedSoundBulletFlag = false;
		m_efectDrawFlag[1] = false;
		return;
	}
}

void Enemy::Act_Craw()
{
	// プレイヤーを発見したとき
	if (m_TrackingPlayerFlag == true) {
		// 突進タイプのとき
		if (m_enemyType == TYPE_CHARGE) {
			m_ActState = CHARGE;
			return;
		}
		// それ以外
		m_ActState = TRACKING;
		return;
	}
	
	// エネミーからパスへ向かうベクトル
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
		// 待機アニメーションを再生
		m_enAnimationState = IDLE;
	}
}

void Enemy::Act_Tracking()
{
	// プレイヤーの座標
	m_playerPos = m_playerManagement->GetPosition();
	// ナビメッシュを作成
	CreateNavimesh(m_playerPos);

	if (m_ChachPlayerFlag == false) {
		// 走るアニメーションを再生
		m_enAnimationState = RUN;
	}

	// 見失った時
	if (m_TrackingPlayerFlag == false) {
		// プレイヤーの座標を参照する
		m_playerMissionPosition = m_playerManagement->GetPosition();
		m_ActState = MISSING_MOVEPOSITON;
	}
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
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_MIN,m_position.y,m_position.z + ADD_MOVE_MIN),3 });
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
	// 移動距離の長さが一定以上のとき
	if (m_sumPos.Length() >= MOVING_DISTANCE) {
		// 突進を終了する
		m_ActState = CHARGEEND;
		// 総移動距離をリセット
		m_sumPos = Vector3::Zero;
		return;
	}

	// タイマーがtrueのとき
	if (Act_Stop(time,2) == true) {

		// 一度だけ実行する
		if (m_CalculatedFlag == false) {
			// 座標を参照
			m_playerChargePosition = m_playerManagement->GetPosition();

			// 何度も実行しないようにtrueにする
			m_CalculatedFlag = true;

			// エネミーからプレイヤーへ向かうベクトル
			m_chargeDiff = m_playerChargePosition - m_position;
			m_chargeDiff.y = 0.0f;
			m_chargeDiff.Normalize();
		}

		// 移動速度に加算
		Vector3 moveSpeed = m_chargeDiff * (MOVE_SPEED * ADD_SPEED);
		m_position += moveSpeed * m_move;

		// 総移動距離を計算
		m_sumPos += moveSpeed;
		// 走るアニメーションを再生
		m_enAnimationState = RUN;
	}
	else {
		// 回転のみプレイヤーを追尾させる
		m_chargeDiff = m_playerManagement->GetPosition() - m_position;
		m_chargeDiff.Normalize();

		// 待機アニメーションを再生
		m_enAnimationState = IDLE;
	}

	Act_Charge_HitWall();

	// 回転を教える
	Rotation(m_chargeDiff);
}

void Enemy::Act_ChargeEnd()
{
	m_position = m_position;		// 座標を固定

	m_addTimer[2] = 0.0f;			// タイマーをリセット
	m_sumPos = Vector3::Zero;		// 移動距離をリセット
	m_CalculatedFlag = false;		// フラグを降ろす

	m_efectDrawFlag[2] = false;		//　!のエフェクトのフラグを降ろす

	// プレイヤーが視野角内にいるとき
	if (m_TrackingPlayerFlag == true) {
		// 再度突進する
		m_ActState = CHARGE;
		return;
	}
	else {
		// いないときは巡回状態に戻る
		m_ActState = MISSING_SEARCHPLAYER;
	}
}

void Enemy::Act_Charge_HitWall()
{
	// 壁に衝突する判定
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerChargePosition - m_position;
	//// 正規化
	diff.Normalize();

	// 壁に衝突したかどうか
	// プレイヤーの方向へ向かう単位ベクトルにスカラーを乗算したものを加算して渡す
	if (Enemy::WallAndHit(m_position + (diff * ADD_LENGTH)) == false) {
		// 衝突したとき
		m_move = 0.0f;
		m_CalculatedFlag = false;
		// 行動を混乱状態にする
		m_ActState = CONFUSION;
		return;
	}

	// 衝突していないときは続行する
	m_move = 1.0f;
}

void Enemy::Act_Call()
{
	// seを鳴らす
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(17);
	se->SetVolume(GameManager::GetInstance()->GetSFX() * 0.1f);
	se->Play(false);

	// ����p��Ƀv���C���[�����݂��Ȃ��Ƃ�
	if (m_TrackingPlayerFlag == false) {
		// フラグを降ろす
		m_efectDrawFlag[1] = false;
		m_ActState = MISSING_SEARCHPLAYER;
		se->Stop();

		return;
	}

	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();

	// エネミーのリストを検索
	for (int i = 0; i < m_game->GetEnemyList().size(); i++) {

		// 混乱中だったときはそれ以降は実行しない
		if (m_game->GetEnemyList()[i]->m_ActState == CONFUSION) {
			return;
		}

		// 各エネミーから該当エネミーへ向かうベクトル
		Vector3 diff = m_position - m_game->GetEnemyList()[i]->m_position;
		float length = diff.Length();

		// 長さが一定以内のとき かつ まだ呼んでいないとき
		if (length > CALL_DISTANCE_MIN &&
			length < CALL_DISTANCE_MAX &&
			m_game->GetEnemyList()[i]->m_ActState != CALLED) {

			m_game->GetEnemyList()[i]->m_ActState = CALLED;				// 行動パターンを変更する
			m_game->GetEnemyList()[i]->m_setPos = m_position - BOXSIZE;	// 自身の座標-キャラコンを目標地点として渡す
		}
	}

	Rotation(rot);

	m_enAnimationState = CALL;
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

	// ナビメッシュを作成
	CreateNavimesh(m_point->s_position);

	// 走るアニメーションを再生
	m_enAnimationState = RUN;


	// エネミーからパスへ向かうベクトル
	Vector3 diff = m_point->s_position - m_position;
	float length = diff.Length();

	// 長さが一定のとき
	if (length <= CHANGING_DISTANCE) {
		m_NaviTimer = 0.0f;
		m_ActState = CRAW;
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
	m_spotLight.SetSpotLight(
		num,
		position,
		LIGHTCOLOR,
		LIGHTRANGE,
		LIGHT_DIRECTION,
		ANGLE
	);
}

void Enemy::SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos)
{
	// 混乱状態の時は実行しない
	if (m_ActState == CONFUSION) {
		return;
	}

	lightpos.y = LIGHTPOSITION;
	//Y��
	Vector3 m_Yup = Vector3(0.0f, 1.0f, 0.0f);
	//�v���C���[�̐���
	Vector3 m_front = Vector3(0.0f, -0.3f, 0.9f);
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
		m_gage->GageUp(1, true);
		m_Vicount = VIGILANCETIME;
	}
}

void Enemy::Event()
{
	// プレイヤーを発見したとき
	if (m_TrackingPlayerFlag == true) {
		Efect_FindPlayer();

		// 突進タイプのとき
		if (m_enemyType == TYPE_CHARGE) {
			m_ActState = CHARGE;
			return;
		}
		else {
			m_ActState = TRACKING;
			return;
		}
	}

	Efect_FindPlayer();

	// アイテムの座標を基にしてナビメッシュを作成
	CreateNavimesh(m_treasure->GetPosition());

	// 経過時間を計測
	m_addTimer[4] += g_gameTime->GetFrameDeltaTime();

	// 走るアニメーションを再生
	m_enAnimationState = RUN;
	// エフェクトの再生フラグをfalseにしておく
	m_efectDrawFlag[2] = false;

	// エネミーからアイテムへ向かうベクトルを作成
	Vector3 diff = m_treasure->GetPosition() - m_position;
	float length = diff.Length();

	// 長さが一定以下のとき
	if (length < CALL_DISTANCE_MIN) {
		// 見失ったプレイヤーを探す
		m_ActState = MISSING_SEARCHPLAYER;
		m_HearedSoundBulletFlag = false;
		m_efectDrawFlag[1] = false;
		return;
	}
	// 長さが一定以上かつ一定時間が経過したとき(壁にぶつかったときの対処策)
	else if (m_addTimer[4] < 10.0f) {
		// 見失ったプレイヤーを探す
		m_ActState = MISSING_SEARCHPLAYER;
		m_HearedSoundBulletFlag = false;
		// タイマーをリセット
		m_addTimer[4] = 0.0f;
		m_efectDrawFlag[1] = false;
		return;
	}
}