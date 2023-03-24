#include "stdafx.h"
#include "Enemy_Normal.h"

namespace
{
	const float		LINEAR_COMPLETION = 0.2f;		// 線形補完のフレーム数
	const float		MOVING_DISTANCE = 500.0f;		// 移動量
}

Enemy_Normal::Enemy_Normal()
{
}

Enemy_Normal::~Enemy_Normal()
{
}

bool Enemy_Normal::Start()
{
	// アニメーションの読み込み
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Attack].Load("Assets/animData/enemy/attack1.tka");
	m_animationClips[m_enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Damege].Load("Assets/animData/enemy/damege.tka");
	m_animationClips[m_enAnimationClip_Damege].SetLoopFlag(false);

	// モデルの読み込み
	m_NormalModelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ);
	//m_NormalModelRender.Init("Assets/modelData/unityChan.tkm");
	m_NormalModelRender.SetScale(m_scale);
	m_NormalModelRender.SetPosition(m_position);
	m_NormalModelRender.SetRotation(m_rotation);

	Enemy::Start();

	m_point = &m_pointList[0];

	return true;
}

void Enemy_Normal::Pass(int PassState)
{
	switch (PassState)
	{
		// 縦
	case Line:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		break;
		// 横
	case Horizontal:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });
		break;
		// 右回り
	case RightRotation:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),4 });
		break;
		// 左回り
	case LeftRotation:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),4 });
		break;
		// (右に)直角
	case RightAngle:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 500.0f),4 });
		break;
	}
}

void Enemy_Normal::Update()
{
	Act();							// 行動パターン
	Animation();					// アニメーション

	m_NormalModelRender.SetPosition(m_position);
	m_NormalModelRender.SetRotation(m_rotation);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーをモデルの位置と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	m_NormalModelRender.Update();	// 更新
}

void Enemy_Normal::Act()
{
	HitFlashBullet();		// 閃光弾に当たったときの処理

	// プレイヤーを見つけたとき
	if (Enemy::SeachPlayer() == true) {
		Enemy::Act_Tracking();

		// 捕まえたとき
		if (CatchPlayer() == true) {

			m_fontRender.SetText(L"つかまえた");
			m_fontRender.SetPosition(Vector3(-500.0f, 0.0f, 0.0f));
		}

		// 追跡を停止する
		if (HitFlashBulletFlag == true || Enemy::SeachPlayer() == false) {
			ChangeCrawFlag = true;
			Enemy::Act_Craw();		// 巡回行動
		}
	}
	else {
		Enemy::Act_Craw();			// 巡回行動
	}
}

void Enemy_Normal::Animation()
{
	// アニメーションのステート
	switch (m_enEnemyAnimationState) {
		// 待機
	case Enemy::m_enEnemyAnimationState_Idle:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Idle, LINEAR_COMPLETION);
		break;
		// 歩く
	case Enemy::m_enEnemyAnimationState_Walk:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Walk, LINEAR_COMPLETION);
		break;
		// 走る
	case Enemy::m_enEnemyAnimationState_Run:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Run, LINEAR_COMPLETION);
		break;
		// 攻撃
	case Enemy::m_enEnemyAnimationState_Attack:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Attack, LINEAR_COMPLETION);
		break;
		// 被弾
	case Enemy::m_enEnemyAnimationState_Damege:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Damege, LINEAR_COMPLETION);
		break;
	}
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// 描画
	m_NormalModelRender.Draw(rc);

	if (Enemy::CatchPlayer() == true) {
		m_fontRender.Draw(rc);
	}
}