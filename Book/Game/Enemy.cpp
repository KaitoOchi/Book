#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"
#include "GameUI.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 75.0f				// エネミーの視野角(初期:120)
#define SEACH_DECISION 200.0f * 200.0f						// ベクトルを作成する範囲

namespace
{
	const float		MOVE_SPEED = 3.0f;						// 移動速度
	const float		MOVING_DISTANCE = 400.0f;				// 移動距離
	const float		CALL_DISTANCE = 300.0f;					// 呼ぶことができる範囲
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
	const float		LIGHTRANGE = 300.0f;					//���C�g�̉e���͈�
	const float		LIGHTPOSITION = 40.0f;					//���C�g�̃|�W�V����
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
	// �X�t�B�A�R���C�_�[�������
	m_sphereCollider.Create(1.0f);
	// ナビメッシュを構築
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");
	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_gameUI = FindGO<GameUI>("gameUI");

	return true;
}

void Enemy::Rotation(Vector3 rot)
{
	// 回転
	m_rotation.SetRotationYFromDirectionXZ(rot);
	m_enemyRender.SetRotation(m_rotation);
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

	// ベクトルが一定の長さ以下のとき
	if (diff.LengthSq() <= SEACH_DECISION) {
		// 正規化
		diff.Normalize();
		// 内積を計算
		float cos = m_forward.Dot(diff);
		// 角度を計算
		float angle = acosf(cos);
		// 角度が視野角内のとき
		if (angle <= (FIELDOF_VIEW) {
			// 衝突判定を行う
			
			//// 衝突しているとき
			//else if (WallAndHit(m_playerPos) == true) {
			//	FindPlayerFlag = true;
			//}

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
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	// 終点はプレイヤーの座標
	end.setOrigin(btVector3(pos.x, pos.y + 70.0f, pos.z));

	SweepResultWall callback;

	// コライダーを始点から終点まで動かして、
	// 衝突するかどうか調べる
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	// 壁と衝突した
	if (callback.isHit == true) {
		// プレイヤーは見つかっていない
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
		return true;
	}

	return false;
}

void Enemy::Act_HitFlashBullet()
{
	// 閃光弾が当たったとき
	// trueのとき当たった

	if (HitFlashBulletFlag == true) {
		// 被弾アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Damege;
		// タイマーがtrueのとき
		if (Act_Stop(CANMOVE_TIMER) == true) {
			HitFlashBulletFlag = false;		// フラグを降ろす
			addTimer = 0.0f;				// タイマーをリセット

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

		addTimer = 0.0f;	// タイマーをリセット
	}

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 moveSpeed = m_point->s_position - m_position;
	// 正規化
	moveSpeed.Normalize();
	// 回転を教える
	moveSpeed *= MOVE_SPEED;
	Rotation(moveSpeed);

	// タイマーがtrueのとき
	if (Act_Stop(WAITING_TIMER) == true) {
		// 歩きアニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
		// 座標に加算
		m_position += moveSpeed;
	}
	// そうでないとき
	else {
		// 待機アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
	}
}

void Enemy::Act_Tracking()
{
	// タイマーを加算
	NaviTimer += g_gameTime->GetFrameDeltaTime();

	// 一定時間以下のときreturn
	if (CALCULATIONNAVI_TIMER >= NaviTimer) {
		return;
	}

	// ナビメッシュ

	// プレイヤーの座標
	m_playerPos = m_playerManagement->GetPosition();

	bool isEnd;							// フラグ

	// パス検索
	m_pathFiding.Execute(
		m_path,							// 構築されたパスの格納先
		m_nvmMesh,						// ナビメッシュ
		m_position,						// 開始座標
		m_playerPos,					// 移動目標座標
		PhysicsWorld::GetInstance(),	// 物理エンジン
		AI_RADIUS,						// AIエージェントの半径
		AI_HIGH							// AIエージェントの高さ
	);

	// パス上を移動する
	m_position = m_path.Move(
		m_position,						// 座標
		MOVE_SPEED,						// 移動速度
		isEnd							// 移動したときtrue
	);

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 moveSpeed = m_playerPos - m_position;

	float angle = atan2(-moveSpeed.x, moveSpeed.z);
	Quaternion rot = Quaternion::Identity;
	rot.SetRotationY(-angle);

	// 回転を教える
	m_enemyRender.SetRotation(rot);

	// 歩きアニメーションを再生
	m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
}

void Enemy::Pass(int PassState)
{
	switch (PassState)
	{
		// 縦
	case LINE_VERTICAL:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		break;
		// 横
	case LINE_HORIZONTAL:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });
		break;
		// 右回り(正方形)
	case SQUARE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),4 });
		break;
		// 左回り(正方形)
	case SQUARE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),4 });
		break;
		// (右に)直角
	case ANGLE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 500.0f),4 });
		break;
		// (左に)直角
	case ANGLE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 500.0f),4 });
		break;
		// 右回り(長方形)
	case RECTANGLE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 300.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f ,m_position.y,m_position.z + 300.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z),4 });
		break;
		// 左回り(長方形)
	case RECTANGLE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 300.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f ,m_position.y,m_position.z + 300.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),4 });
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
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
	}

}

void Enemy::Act_Charge(float time)
{
	// 回転
	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();
	Rotation(rot);

	// 待機アニメーションを再生
	m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;

	// タイマーがtrueのとき
	if (Act_Stop(time) == true) {

		// 一度だけ実行する
		if (CalculatedFlag == false) {

			// 座標を参照
			playerPos = m_playerManagement->GetPosition();
			enemyPos = m_position;

			// 何度も実行しないようにtrueにする
			CalculatedFlag = true;
		}

		// エネミーからプレイヤーへ向かうベクトル
		Vector3 diff = playerPos - enemyPos;
		diff.Normalize();
		// 回転
		Rotation(diff);

		// 移動速度に加算
		Vector3 moveSpeed = diff * MOVE_SPEED;
		m_position += moveSpeed;
		// 装移動距離を計算
		sumPos += moveSpeed;

		// 歩きアニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;

		// 長さが一定以上のとき
		if (sumPos.Length() > MOVING_DISTANCE) {
			
			m_position = m_position;	// 座標を固定

			addTimer = 0.0f;			// タイマーをリセット
			sumPos = Vector3::Zero;		// 移動距離をリセット
			CalculatedFlag = false;		// フラグを降ろす

			// プレイヤーが視野角内にいるとき
			if (Act_SeachPlayer() == true) {
				return;
			}

			// いないときは巡回状態に戻る
			m_ActState = BACKBASEDON;
		}
	}
}

void Enemy::Act_Call()
{
	// 周りの敵を呼ぶ処理

	// えねみーのリストを検索
	// ベクトルを計算して一定以内のとき正規化、移動速度を加算する


}

void Enemy::Act_Loss()
{
	addTimer = 0.0f;	// タイマーをリセット

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

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 moveSpeed = m_point->s_position - m_position;
	// 正規化
	moveSpeed.Normalize();
	// 移動速度に乗算
	moveSpeed *= MOVE_SPEED;
	// 回転
	Rotation(moveSpeed);

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

bool Enemy::Act_Stop(float time)
{
	// フレームを加算
	addTimer += g_gameTime->GetFrameDeltaTime();

	// タイマーが一定以上になったら
	if (time <= addTimer) {
		return true;
	}

	return false;
}

void Enemy::SpotLight_New(Vector3 position)
{
	Vector3 forward = Vector3::AxisY;

	m_spotLight.SetSpotLight(
		0,
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