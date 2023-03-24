#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"

namespace
{
	const Vector3 CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// マップの中心
	const float MAP_RADIUS = 150.0f;									// マップの半径
	const float LIMITED_RANGE_IMAGE = 600.0f;							// マップの範囲
	const float ALPHA = 0.75f;
}

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
}

bool MiniMap::Start()
{
	// マップの黒い画像
	m_SpriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_SpriteRender.SetPosition(CENTER_POSITION);
	//m_SpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, ALPHA });

	// 飾りの画像
	m_OutLineSpriteRender.Init("Assets/sprite/UI/miniMap/base_outLine.DDS", 362, 519);
	m_OutLineSpriteRender.SetPosition({ 635.0f, -200.0f, 0.0f });
	m_OutLineSpriteRender.Update();

	// 中心の画像
	m_PlayerSpriteRender.Init("Assets/sprite/UI/miniMap/player.DDS", 20,40);
	m_PlayerSpriteRender.SetPosition(CENTER_POSITION);

	// エネミーを表す画像
	m_EnemySpriteRender.Init("Assets/sprite/UI/miniMap/map_2.DDS", 15, 15);

	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_enemyNormal = FindGO<Enemy_Normal>("enemyNormal");

	return true;
}

void MiniMap::Update()
{
	DrawMap();

	m_EnemySpriteRender.Update();
	m_PlayerSpriteRender.Update();
	m_SpriteRender.Update();
}

void MiniMap::DrawMap()
{
	// 座標を取得
	Vector3 playerPos = m_playerManagement->GetPosition();
	Vector3 enemy_NormalPos = m_enemyNormal->GetPosition();

	Vector3 mapPos;

	// マップに表示する範囲に敵がいたら
	if (WorldPositionConvertToMapPosition(playerPos, enemy_NormalPos, mapPos)) {
		// マップに表示する
		m_isImage = true;
		// spriteRenderに座標を設定
		m_EnemySpriteRender.SetPosition(mapPos);
	}
	// マップに表示する敵がいなかったら
	else {
		m_isImage = false;
	}
}

const bool MiniMap::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition,Vector3 worldPosition,Vector3& mapPosition) 
{
	// Y座標はマップとは関係ないので0.0fを設定
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;

	// 中心座標から表示したいオブジェクトの座標へ向かうベクトルを計算
	Vector3 diff = worldPosition - worldCenterPosition;

	// 計算したベクトルが一定以上離れていたら
	if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
		// 範囲外に存在しているのでマップに表示しない
		return false;
	}

	/*以下コピペ。
	敵の向きが反映されていないらしいので反映させること。*/


	//ベクトルの長さを取得する
	float length = diff.Length();

	//カメラの前方向ベクトルから。
	//クォータニオンを生成。
	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	//ベクトルにカメラの回転を適用。
	rot.Apply(diff);

	//ベクトルを正規化。
	diff.Normalize();

	//マップの大きさ/距離制限で。
	//ベクトルをマップ座標系に変換する。
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//マップの中央座標と上記ベクトルを加算する。
	mapPosition = Vector3(CENTER_POSITION.x + diff.x, CENTER_POSITION.y + diff.z, 0.0f);
	return true;
}

void MiniMap::Render(RenderContext& rc)
{
	// 描画
	m_SpriteRender.Draw(rc);
	m_OutLineSpriteRender.Draw(rc);
	m_PlayerSpriteRender.Draw(rc);

	// エネミーが近くにいないとき
	if (m_isImage == false) {
		// ここで処理を終了
		return;
	}

	m_EnemySpriteRender.Draw(rc);
}