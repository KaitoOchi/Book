#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"
#include "Game.h"
#include "Enemy.h"
#include "Treasure.h"

namespace
{
	const Vector3	CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// マップの中心
	const float		MAP_RADIUS = 140.0f;									// マップの半径
	const float		LIMITED_RANGE_IMAGE = 650.0f;							// マップの範囲
	const float		ALPHA = 0.75f;											// α値
}

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
}

bool MiniMap::Start()
{
	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_game = FindGO<Game>("game");
	m_treasure = FindGO<Treasure>("treaSure");
	// エネミーのリストを持ってくる
	m_enemyList = m_game->GetEnemyList();
	// お宝の位置
	m_treasurePos = m_treasure->GetPosition();

	// 画像を用意する
	// マップ画像
	m_SpriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_SpriteRender.SetPosition(CENTER_POSITION);
	//m_SpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, ALPHA });

	// アウトライン
	m_OutLineSpriteRender.Init("Assets/sprite/UI/miniMap/base_outLine.DDS", 362, 519);
	m_OutLineSpriteRender.SetPosition({ 640.0f, -210.0f, 0.0f });
	m_OutLineSpriteRender.Update();

	// プレイヤー
	m_PlayerSpriteRender.Init("Assets/sprite/UI/miniMap/player.DDS", 20,40);
	m_PlayerSpriteRender.SetPosition(CENTER_POSITION);

	// エネミー
	for (int i = 0; i < m_enemyList.size(); i++) {
		m_EnemySpriteRender[i].Init("Assets/sprite/UI/miniMap/map_2.DDS", 15, 15);
		// フラグも初期化しておく
		m_isImage[i] = false;
	}

	// お宝
	m_TreasureSpriteRender.Init("Assets/sprite/UI/miniMap/map_2.DDS", 10,10);
	// マップ上の色を黄色に設定。エネミーより少し大きく描画する
	m_TreasureSpriteRender.SetMulColor({ 5.0f,3.0f,0.0f,1.0f });

return true;
}

void MiniMap::Update()
{
	for (int i = 0; i < m_enemyList.size(); i++) {
		// マップ座標に変換
		DrawMap(m_enemyList[i]->GetPosition(), i);
		// 更新
		m_EnemySpriteRender[i].Update();
	}

	// お宝を描画
	DrawMap_Treasure(m_treasurePos);
	m_TreasureSpriteRender.Update();

	m_PlayerSpriteRender.Update();
	m_SpriteRender.Update();
}

void MiniMap::DrawMap(Vector3 enemyPos, int num)
{
	// プレイヤーの座標
	Vector3 playerPos = m_playerManagement->GetPosition();

	Vector3 mapPos;

	// マップに表示する範囲に敵がいたら
	if (WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos)) {

		// spriteRenderに座標を設定
		m_EnemySpriteRender[num].SetPosition(mapPos);
		// マップに表示する
		m_isImage[num] = true;
	}
	// マップに表示する敵がいなかったら
	else {
		m_isImage[num] = false;
	}
}

void MiniMap::DrawMap_Treasure(Vector3 TreasurePos)
{
	// プレイヤーの座標
	Vector3 playerPos = m_playerManagement->GetPosition();

	Vector3 mapPos;

	// 長さが範囲内だったら
	if (WorldPositionConvertToMapPosition(playerPos, TreasurePos, mapPos)) {
		// spriteRenderに座標を設定
		m_TreasureSpriteRender.SetPosition(mapPos);
		m_isImage2 = true;
	}
	else {
		m_isImage2 = false;
	}
}

const bool MiniMap::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition)
{
	// Y座標はマップとは関係ないので0.0fを設定
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;

	// 中心の座標から表示したいオブジェクトの座標へ向かうベクトルを計算
	Vector3 diff = worldPosition - worldCenterPosition;

	// 	計算したベクトルが一定以上離れていたら
	if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
		// 範囲外に存在しているのでマップに表示しない
		return false;
	}

	// ベクトルの長さを取得する
	float length = diff.Length();

	// カメラの前方向ベクトルから、
	// Y軸方向の回転クォータニオンを生成する
	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	// ベクトルにカメラの回転を適用
	rot.Apply(diff);

	// ベクトルを正規化
	diff.Normalize();

	// マップの大きさ/範囲で、
	// ベクトルをマップ座標系に変換する
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	// マップの中央座標と上記ベクトルを加算する
	mapPosition = Vector3(CENTER_POSITION.x + diff.x, CENTER_POSITION.y + diff.z, 0.0f);
	return true;
}

void MiniMap::Render(RenderContext& rc)
{
	// 描画
	m_SpriteRender.Draw(rc);
	m_OutLineSpriteRender.Draw(rc);
	m_PlayerSpriteRender.Draw(rc);

	if (m_isImage2 == true) {
		m_TreasureSpriteRender.Draw(rc);
	}

	for (int i = 0; i < m_enemyList.size(); i++) {
		// 範囲内のとき
		if (m_isImage[i] == true) {
			// 描画する
			m_EnemySpriteRender[i].Draw(rc);
		}
	}
}