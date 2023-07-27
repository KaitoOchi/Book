#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"
#include "Gage.h"
#include "Game.h"
#include "GameManager.h"

namespace
{
	const Vector3	BOXSIZE = { 60.0f, 80.0f,60.0f };		// CharacterControllerのサイズ
	const Vector3	SCALE = { 2.0f,2.0f,2.0f };				// スケール
	const float		LINEAR_COMPLETION = 1.0f;				// 線形補完
	const float		COLLIDER_SIZE = 20.0f;					// スフィアコライダーのサイズ
	const float		MOVE_SPEED = 3.0f;						// 移動速度
	const float		ADD_SPEED = 1.3f;						// 乗算速度
	const float		CALL_DISTANCE_MAX = 400.0f;				// 呼ぶことができる最大値
	const float		CALL_DISTANCE_MIN = 190.0f;				// 呼ぶことができる最小値
	const float		CHANGING_DISTANCE = 20.0f;				// 目的地を変更する距離
	const float		LENGTH_LOWESTVALUE = 10.0f;				// 移動する距離の限度
	const float		TIMER_CANMOVE = 5.5f;					// 再度行動できるまでの待機時間
	const float		TIMER_WAITING = 3.0f;					// パス移動時の待機時間
	const float		TIMER_SEARCHPLAYER = 7.0f;				// プレイヤーを見失った時の待機時間
	const float		TIMER_PLAYER_LOSTUPTO = 10.0f;			// プレイヤーを見失うまでの時間
	const float		ADD_MOVE_MIN = 250.0f;					// パス移動
	const float		ADD_MOVE_LONG = 400.0f;					// パス移動
	const float		AI_RADIUS = 150.0f;						// AIエージェントの半径
	const float		AI_HIGH = 200.0f;						// AIエージェントの高さ
	const float		CALCULATIONNAVI_TIMER = 0.5f;			// ナビメッシュを再度計算するタイマー
	const float		CATCH_DECISION = 60.0f;					// プレイヤーを確保したことになる範囲
	const float		SEACH_DECISION = 100.0f * 100.0f;		// 索敵範囲
	const float     VIGILANCETIME = 0.3f;					// 警戒度UP時間
	const Vector3	LIGHT_DIRECTION = { 0.0f, 1.0f, 1.0f };	// スポットライトの方向
	const Vector3   LIGHTCOLOR(1.5f, 0.3f, 0.0f);			// スポットライトのカラー
	const float		ANGLE = 45.0f;							// スポットライトの角度
	const float		LIGHTRANGE = 600.0f;					// スポットライトの範囲
	const float		LIGHTPOSITION = 80.0f;					// スポットライトの座標
	const float		EFFECT_Z = 10.0f;						// エフェクトのZ座標加算値
	const float		EFFECT_X = 5.0f;						// エフェクトのX座標加算値
	const float		EFFECT_Y = 100.0f;						// エフェクトのY座標
	const float		EFFECT_SIZE_STAR = 1.0f;				// エフェクトのサイズ
	const float		EFFECT_SIZE_EXCLAMATION = 1.2f;			// エフェクトのサイズ
	const float		EFFECT_SIZE_QUESTION = 1.5f;			// エフェクトのサイズ
}

Enemy::Enemy()
{
	// タイマーのリセット
	for (int i = 0; i < m_TimerState_Num; i++) {
		m_addTimer[i] = 0.0f;
	}

	// エフェクトを生成するフラグのリセット
	for (int i = 0; i < m_EffectState_Num; i++) {
		m_existsEfectDraw[i] = false;
	}

	//警戒度時間を代入
	m_addVicount = VIGILANCETIME;
}

Enemy::~Enemy()
{
	m_pointList.clear();
	m_pointList.shrink_to_fit();

	if (m_Effect != nullptr) {
		// エフェクトの停止と削除
		m_Effect->Stop();
		DeleteGO(m_Effect);
	}
}

bool Enemy::Start()
{
	// スケールを設定
	SetScale(SCALE);

	// 行動パターンを初期化
	if (m_enemyType == TYPE_SEARCH) {
		m_enActionState = m_ActionState_Search;
	}
	else {
		m_enActionState = m_ActionState_Craw;
	}

	// キャラクターコントローラーを初期化する
	m_characterController.Init(BOXSIZE, m_position);

	// スフィアコライダーを設定
	m_sphereCollider.Create(COLLIDER_SIZE);

	// ナビメッシュを構築
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");

	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_gage = FindGO<Gage>("gage");
	m_game = FindGO<Game>("game");

	// 視野を作成
	SpotLight_New(m_position, m_spotNum);

	//----------------------------------------------
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	//----------------------------------------------

	return true;
}

void Enemy::LoadAnimation()
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

	m_enAnimationClips[m_enAnimation_Damage].Load("Assets/animData/enemy/damege.tka");
	m_enAnimationClips[m_enAnimation_Damage].SetLoopFlag(false);

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
	case m_enAnimationState_Idle:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Idle, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Walk:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Walk, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Run:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Run, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Attack:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Attack, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Damage:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Damage, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Dizzy:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Dizzy, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Loss:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Loss, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Call:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Call, LINEAR_COMPLETION);
		break;
	}
}

void Enemy::Efect_Dizzy()
{
	if (m_existsEfectDraw[m_EffectState_Star] == false) {
		// ☆のエフェクトを生成
		m_Effect = NewGO<EffectEmitter>(2);
		m_Effect->Init(2);
		// エフェクトの大きさを指定する
		m_Effect->SetScale(Vector3::One * EFFECT_SIZE_STAR);
		// エフェクトの座標の設定
		m_Effect->SetPosition(Vector3(m_position.x + EFFECT_X, EFFECT_Y, m_position.z + EFFECT_Z));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_existsEfectDraw[m_EffectState_Star] = true;
	}
}

void Enemy::Efect_FindPlayer()
{
	if (m_existsEfectDraw[m_EffectState_ExclamationPoint] == false) {
		// !のエフェクトを生成
		m_Effect = NewGO<EffectEmitter>(3);
		m_Effect->Init(3);
		// エフェクトの大きさを指定する
		m_Effect->SetScale(Vector3::One * EFFECT_SIZE_EXCLAMATION);
		// エフェクトの座標の設定
		m_Effect->SetPosition(Vector3(m_position.x + EFFECT_X, EFFECT_Y, m_position.z + EFFECT_Z));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_existsEfectDraw[m_EffectState_ExclamationPoint] = true;
	}
}

void Enemy::Efect_MissingPlayer()
{
	if (m_existsEfectDraw[m_EffectState_QuestionMark] == false) {
		// ?のエフェクトを生成
		m_Effect = NewGO<EffectEmitter>(4);
		m_Effect->Init(4);
		// エフェクトの大きさを指定する
		m_Effect->SetScale(Vector3::One * EFFECT_SIZE_QUESTION);
		// エフェクトの座標の設定
		m_Effect->SetPosition(Vector3(m_position.x + EFFECT_X, EFFECT_Y, m_position.z + EFFECT_Z));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_existsEfectDraw[m_EffectState_QuestionMark] = true;
	}
}

void Enemy::Rotation(const Vector3& rot)
{
	// 回転
	m_rotation.SetRotationYFromDirectionXZ(rot);
	m_enemyModelRender.SetRotation(m_rotation);

	//---------------------------------------------
	//エネミーの前ベクトルを求める
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	//---------------------------------------------
}

void Enemy::SpecifyPath(int pathNumber)
{
	// パスを指定
	switch (pathNumber){
	case m_PathState_VerticalMovement:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_LONG),2 });
		break;
	case m_PathState_MoveSideways:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_LONG,m_position.y,m_position.z),2 });
		break;
	case m_PathState_ClockwiseRotation:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_MIN,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_MIN,m_position.y,m_position.z - ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_MIN),4 });
		break;
	}
}

void Enemy::CreateNavimesh(const Vector3& pos)
{
	m_naviTimer += g_gameTime->GetFrameDeltaTime();

	// 目標地点へ向かうベクトル
	Vector3 diff = pos - m_position;
	diff.Normalize();

	// 回転
	Rotation(diff);

	// 一定時間以下のときreturn
	if (CALCULATIONNAVI_TIMER >= m_naviTimer) {
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

void Enemy::Action_SeachPlayer()
{
	// 混乱時は何もしない
	if (m_enActionState == m_ActionState_Dizzy) {
		return;
	}

	// 追跡している最中
	if (m_isTrackingPlayer == true) {
		// 衝突判定を行う
		if (WallAndHit(m_playerManagement->GetPosition()) == false) {
			m_isTrackingPlayer = false;
			return;
		}
	}

	// スポットライトの中にプレイヤーがいるとき
	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true) {
		// 衝突判定を行う
		if (WallAndHit(m_playerManagement->GetPosition()) == false) {
			m_isTrackingPlayer = false;
			return;
		}

		m_isTrackingPlayer = true;
		Efect_FindPlayer();
		return;
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

bool Enemy::WallAndHit(const Vector3& pos)
{
	btTransform start, end;

	start.setIdentity();
	end.setIdentity();

	// 始点はエネミーの座標
	// Y軸は少し上に配置する
	start.setOrigin(btVector3(m_position.x,  25.0f, m_position.z));

	// 終点はプレイヤーの座標 (突進時は始点の少し前)
	end.setOrigin(btVector3(pos.x, 25.0f, pos.z));

	SweepResultWall callback;

	// コライダーを始点から終点まで動かして、
	// 衝突するかどうか調べる
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(),
		start, end, callback);

	// 壁と衝突した
	if (callback.isHit == true) {
		return false;
	}

	// 壁と衝突していない
	// プレイヤーを見つけた
	return true;
}

bool Enemy::Action_CatchPlayer()
{
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// 長さを計算する
	float length = diff.Length();

	// プレイヤーを捕まえられる長さのとき
	if (length <= CATCH_DECISION) {

		// プレイヤーの方向を向く
		diff.Normalize();
		Rotation(diff);
		// 捕まえる
		m_enAnimationState = m_enAnimationState_Attack;

		for (int i = 0; i < m_game->GetEnemyList().size(); i++) {
			// ステートを変動させる
			m_game->GetEnemyList()[i]->m_enActionState = m_ActionState_CatchPlayer;
		}
		return true;
	}
	return false;
}

void Enemy::Action_MoveMissingPosition()
{
	// プレイヤーを発見したとき
	if (m_isTrackingPlayer == true) {
		// 再度追跡する
		Efect_FindPlayer();
		m_naviTimer = 0.0f;
		m_addTimer[m_TimerState_MissingPlayer] = 0.0f;			// タイマーをリセット

		if (m_enemyType == TYPE_SEARCH) {
			m_enActionState = m_ActionState_Call_AroundEnemy;
			return;
		}
		else if (m_enemyType == TYPE_CHARGE) {
			m_enActionState = m_ActionState_Charge;
			return;
		}
		m_enActionState = m_ActionState_Tracking;
		return;
	}

	// ベクトルを作成
	Vector3 diff = m_playerMissiongPosition - m_position;

	// 長さを計算
	float length = diff.Length();

	// 移動する
	diff.Normalize();

	// 移動速度に加算
	Vector3 moveSpeed = diff * (MOVE_SPEED * ADD_SPEED);
	m_position += moveSpeed;

	m_enAnimationState = m_enAnimationState_Run;

	// 長さが一定以下だったら
	if (length < LENGTH_LOWESTVALUE) {
		// ステートを移行
		m_enActionState = m_ActionState_Search_MissingPlayer;
		return;
	}

	// 回転を教える
	Rotation(moveSpeed);
}

void Enemy::Action_SearchMissingPlayer()
{
	Efect_MissingPlayer();

	if (m_isTrackingPlayer == true) {
		// 再度追跡する
		Efect_FindPlayer();
		m_naviTimer = 0.0f;
		m_addTimer[m_TimerState_MissingPlayer] = 0.0f;			// タイマーをリセット

		if (m_enemyType == TYPE_SEARCH) {
			m_enActionState = m_ActionState_Call_AroundEnemy;
			return;
		}
		else if (m_enemyType == TYPE_CHARGE) {
			m_enActionState = m_ActionState_Charge;
			return;
		}
		m_enActionState = m_ActionState_Tracking;
		return;
	}

	m_enAnimationState = m_enAnimationState_Loss;

	// モーションを再生
	if (Action_StopMove(TIMER_SEARCHPLAYER, m_TimerState_MissingPlayer) == true) {

		m_existsEfectDraw[m_EffectState_ExclamationPoint] = false;	// エフェクトの描画フラグ
		m_existsEfectDraw[m_EffectState_QuestionMark] = false;

		m_addTimer[m_TimerState_MissingPlayer] = 0.0f;				// タイマーをリセット
		m_sumPosition = Vector3::Zero;								// 移動距離をリセット

		if (m_enemyType == TYPE_SEARCH) {
			// 索敵状態に戻す
			m_enActionState = m_ActionState_Search;
			return;
		}
		m_enActionState = m_ActionState_BackBasedOn;
	}
}

void Enemy::Action_HitFlashBullet()
{
	if (m_isTrackingPlayer == true) {
		// 閃光弾が当たっているときはプレイヤーを追いかけない
		m_isTrackingPlayer = false;
	}

	if (m_isChachPlayer == true) {
		// 閃光弾が当たっているときはプレイヤーを捕まえない
		m_isChachPlayer = false;
	}

	m_enAnimationState = m_enAnimationState_Dizzy;
	Efect_Dizzy();

	if (Action_StopMove(TIMER_CANMOVE, m_TimerState_HitByaFlashbang) == true) {
		// エフェクトの生成フラグをfalseにしておく
		m_existsEfectDraw[m_EffectState_Star] = false;

		// フラグを降ろす
		m_isTrackingPlayer = false;
		m_hitFlashBullet = false;

		// タイマーをリセット
		m_addTimer[m_TimerState_HitByaFlashbang] = 0.0f;

		Efect_MissingPlayer();
		m_enActionState = m_ActionState_Search_MissingPlayer;
	}
	else {
		// エフェクトの生成フラグをfalseにしておく
		m_existsEfectDraw[m_EffectState_QuestionMark] = false;
	}
}

void Enemy::Action_GoLocationListenSound(const Vector3& tergetPosition)
{
	if (m_isTrackingPlayer == true) {
		if (m_enemyType == TYPE_CHARGE) {
			m_enActionState = m_ActionState_Charge;
			return;
		}
		else {
			m_enActionState = m_ActionState_Tracking;
			return;
		}
	}

	Efect_FindPlayer();

	// エネミーからアイテムへ向かうベクトルを作成
	Vector3 diff = tergetPosition - m_position;
	float length = diff.Length();

	// 長さが一定以上のとき
	if (length >= CALL_DISTANCE_MIN) {
		// アイテムの座標を基にしてナビメッシュを作成
		CreateNavimesh(tergetPosition);

		// 経過時間を計測
		m_addTimer[m_TimerState_HitByaSoundbang] += g_gameTime->GetFrameDeltaTime();
		m_enAnimationState = m_enAnimationState_Run;

		// エフェクトの再生フラグをfalseにしておく
		m_existsEfectDraw[m_EffectState_QuestionMark] = false;
	}
	else if (m_addTimer[m_TimerState_HitByaSoundbang] > TIMER_PLAYER_LOSTUPTO) {
		// 一定時間が経過したとき
		m_enActionState = m_ActionState_Search_MissingPlayer;
		m_hearedSoundBullet = false;
		// タイマーをリセット
		m_addTimer[m_TimerState_HitByaSoundbang] = 0.0f;
		m_existsEfectDraw[m_EffectState_ExclamationPoint] = false;
		return;
	}
	else {
		m_enActionState = m_ActionState_Search_MissingPlayer;
		// フラグをリセット
		m_hearedSoundBullet = false;
		m_existsEfectDraw[m_EffectState_ExclamationPoint] = false;
		return;
	}
}

void Enemy::Action_CrawPath()
{
	if (m_isTrackingPlayer == true) {
		if (m_enemyType == TYPE_CHARGE) {
			m_enActionState = m_ActionState_Charge;
			return;
		}
		m_enActionState = m_ActionState_Tracking;
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

		m_addTimer[m_TimerState_StayOnThePath] = 0.0f;	// タイマーをリセット
	}

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 moveSpeed = m_point->s_position - m_position;
	// 正規化
	moveSpeed.Normalize();
	// 回転を教える
	moveSpeed *= MOVE_SPEED;
	Rotation(moveSpeed);

	if (Action_StopMove(TIMER_WAITING, m_TimerState_StayOnThePath) == true) {
		m_enAnimationState = m_enAnimationState_Walk;
		m_position += moveSpeed;
	}
	else {
		m_enAnimationState = m_enAnimationState_Idle;
	}
}

void Enemy::Action_TrackingPlayer()
{
	// ナビメッシュを作成
	CreateNavimesh(m_playerManagement->GetPosition());

	if (m_isChachPlayer == false) {
		m_enAnimationState = m_enAnimationState_Run;
	}

	// 見失った時
	if (m_isTrackingPlayer == false) {
		// プレイヤーの座標を参照する
		m_playerMissiongPosition = m_playerManagement->GetPosition();
		m_enActionState = m_ActionState_Move_MissingPositon;
	}
}

void Enemy::Action_MissingPlayer()
{
	m_addTimer[m_TimerState_StayOnThePath] = 0.0f;	// タイマーをリセット
	m_addTimer[m_TimerState_UntilTheCharge] = 0.0f;	// 突進からの移行時にこちらのタイマーもリセット

	// 直近のパスを検索
	// floatに最大値を格納
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	int NowTargetNum = -1;

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

	CreateNavimesh(m_point->s_position);
	m_enAnimationState = m_enAnimationState_Run;

	// エネミーからパスへ向かうベクトル
	Vector3 diff = m_point->s_position - m_position;
	float length = diff.Length();

	// 長さが一定のとき
	if (length <= CHANGING_DISTANCE) {
		m_naviTimer = 0.0f;
		m_enActionState = m_ActionState_Craw;
	}
}

bool Enemy::Action_StopMove(float time,int timerNumber)
{
	m_addTimer[timerNumber] += g_gameTime->GetFrameDeltaTime();

	// タイマーが一定以上になったら
	if (time <= m_addTimer[timerNumber]) {
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
	if (m_enActionState == m_ActionState_Dizzy) {
		return;
	}

	lightpos.y = LIGHTPOSITION;
	//Y座標を設定
	Vector3 m_Yup = Vector3(0.0f, 1.0f, 0.0f);
	//前方の座標を設定
	Vector3 m_front = Vector3(0.0f, -0.3f, 0.9f);
	lightrotaition.Apply(m_front);
	//上記の座標の外積を計算
	Vector3 m_vertical = Cross(m_Yup, m_front);
	Quaternion m_SitenRot;
	//計算した外積を軸に回転させる
	m_SitenRot.SetRotationDeg(m_vertical, ANGLE);
	//クォータニオンを適応
	m_SitenRot.Apply(m_front);
	m_spotLight.SetDirection(m_front);
	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true)
	{
		VigilanceCount();

		if (m_shouldCount == false) {
			// 発見回数に1を加算
			GameManager::GetInstance()->AddSearchNum();
			// フラグをtrueにして再度行わないようにする
			m_shouldCount = true;
		}
	}
	else {
		if (m_isChachPlayer == false) {
			// プレイヤーが存在しないときはフラグを戻す
			m_shouldCount = false;
		}
	}
	m_spotLight.SetPosition(lightpos);
	m_spotLight.Update();
}

void Enemy::VigilanceCount()
{
	m_addVicount -= g_gameTime->GetFrameDeltaTime();
	if (m_addVicount <= 0.0f)
	{
		//ステートの遷移
		m_gage->GageUp(1, true);
		m_addVicount = VIGILANCETIME;
	}
}