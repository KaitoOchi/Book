#include "stdafx.h"
#include "Enemy_Search.h"

#include "GameManager.h"
#include "Game.h"

namespace
{
	const Vector3	BOXSIZE = { 60.0f, 80.0f,60.0f };		// CharacterControllerのサイズ
	const float		KEEPTIME = 2.0f;						// タイマーをリセットする定数
	const float		ANGLE = 45.0f;							// 角度
	const float		RODADD = 0.01f;							// 追加する角度
	const float		DOWN_VOLUME = 0.1f;						// SEの音量減少値
	const float		CALL_DISTANCE_MAX = 400.0f;				// 呼ぶことができる最大値
	const float		CALL_DISTANCE_MIN = 190.0f;				// 呼ぶことができる最小値
}

Enemy_Search::Enemy_Search()
{
}

Enemy_Search::~Enemy_Search()
{
}

bool Enemy_Search::Start()
{
	Enemy::LoadAnimation();

	// モデルの読み込み
	m_enemyModelRender.Init("Assets/modelData/enemy/enemy_search.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_Enemy);

	Enemy::Start();

	m_enemyModelRender.SetPosition(m_position);
	m_enemyModelRender.SetRotation(m_rotation);
	m_enemyModelRender.SetScale(m_scale);

	return true;
}

void Enemy_Search::Action_CallAroundEnemy()
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(17);
	se->SetVolume(GameManager::GetInstance()->GetSFX() * DOWN_VOLUME);	// 音量を下げる
	se->Play(false);

	if (GetTrackingPlayerFlag() == false) {
		// フラグを降ろす
		SetEffectDrawFlag(false, m_EffectState_ExclamationPoint);
		SetEnemyActionState(m_ActionState_Search_MissingPlayer);
		se->Stop();
		return;
	}

	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();

	// エネミーのリストを検索
	for (int i = 0; i < m_game->GetEnemyList().size(); i++) {

		// 混乱中だったときはそれ以降は実行しない
		if (m_game->GetEnemyList()[i]->GetEnemyActionState() == m_ActionState_Dizzy) {
			return;
		}

		// 各エネミーから該当エネミーへ向かうベクトル
		Vector3 diff = m_position - m_game->GetEnemyList()[i]->GetPosition();
		float length = diff.Length();

		// 長さが一定以内のとき かつ まだ呼んでいないとき
		if (length > CALL_DISTANCE_MIN &&
			length < CALL_DISTANCE_MAX &&
			m_game->GetEnemyList()[i]->GetEnemyActionState() != m_ActionState_Called) {

			// 自身の座標-キャラコン分の座標を目標地点にする
			Vector3 position = m_position - BOXSIZE;

			m_game->GetEnemyList()[i]->SetEnemyActionState(m_ActionState_Called);		// 行動パターンを変更する
			m_game->GetEnemyList()[i]->SetGatherPosition(position);						// 目標地点を渡す
		}
	}

	Rotation(rot);
	SetEnemyAnimationState(m_enAnimationState_Call);
}

void Enemy_Search::Update()
{
	// 行動できるか調べる
	if (GetActiveFlag() == true) {
		// イベント時の処理
		Vector3 move = m_position;
		move.y -= 30000.0f;
		m_characterController.Execute(move, 1.0f);
		m_spotLight.SetPosition(move);
		m_spotLight.Update();
		return;
	}

	// 描画しない状態なら
	if (GetNotDrawFlag() == true) {
		// エフェクトを削除する
		if (m_Effect != nullptr) {
			m_Effect->Stop();
			DeleteGO(m_Effect);
		}
		return;
	}

	// 閃光弾に当たったとき
	if (GetHitFlashBulletFlag() == true) {
		// 混乱状態への移行を優先する
		SetEnemyActionState(m_ActionState_Dizzy);
	}

	// 行動パターン
	switch (GetEnemyActionState()) {
	case m_ActionState_Craw:
	case m_ActionState_Search:
		Update_OnSearch();
		break;
	case m_ActionState_Call_AroundEnemy:
		Update_OnCallAroundEnemy();
		break;
	case m_ActionState_Search_MissingPlayer:
		Update_OnMissingPlayer();
		break;
	case m_ActionState_Dizzy:
		Update_OnDizzy();
		break;
	}

	Enemy::PlayAnimation();
	Enemy::SpotLight_Serch(m_rotation, m_position);
	Enemy::Action_SeachPlayer();

	m_characterController.SetPosition(m_position);

	m_enemyModelRender.SetPosition(m_position);
	m_enemyModelRender.SetRotation(m_rotation);
	m_enemyModelRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	Rotaition();

	// プレイヤーが視野角内に存在するとき
	if (GetTrackingPlayerFlag() == true) {
		SetEffectDrawFlag(false, m_EffectState_QuestionMark);
		SetEnemyActionState(m_ActionState_Call_AroundEnemy);
	}
}

void Enemy_Search::Update_OnCallAroundEnemy()
{
	Action_CallAroundEnemy();
}

void Enemy_Search::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}

void Enemy_Search::Update_OnMissingPlayer()
{
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Search::Rotaition()
{
	m_rotTime -= g_gameTime->GetFrameDeltaTime();

	if (m_rotTime < 0.0f) {
		m_rotTime = KEEPTIME;

		// 回転方向を切り替える
		switch (m_rotState)
		{
		case true:
			m_rotState = false;
			break;
		case false:
			m_rotState = true;
			break;
		default:
			break;
		}
	}

	// 回転処理
	switch (m_rotState) {
	case true:
		m_rotation.AddRotationY(RODADD);
		break;
	case false:
		m_rotation.AddRotationY(-RODADD);
		break;
	default:
		break;
	}
	
	SetEnemyAnimationState(m_enAnimationState_Idle);
}

void Enemy_Search::Render(RenderContext& rc)
{
	// 描画
	if (GetNotDrawFlag() == false &&
		GetActiveFlag() == false) {
		m_enemyModelRender.Draw(rc);
	}
}