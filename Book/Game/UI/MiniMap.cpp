#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"
#include "Game.h"
#include "Enemy.h"

namespace
{
	const Vector3	CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// マップの中心
	const float		MAP_RADIUS = 140.0f;									// マップの半径
	const float		LIMITED_RANGE_IMAGE = 650.0f;							// マップの範囲
}

MiniMap::MiniMap()
{
	m_enemyList.reserve(ENEMY_NUM);
	m_physicsGhostList.reserve(WALL_NUM);
}

MiniMap::~MiniMap()
{
	m_enemyList.clear();
	m_enemyList.shrink_to_fit();
	m_physicsGhostList.clear();
	m_physicsGhostList.shrink_to_fit();
}

bool MiniMap::Start()
{
	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	Game* game = FindGO<Game>("game");

	// エネミーのリストを持ってくる
	m_enemyList = game->GetEnemyList();
	m_physicsGhostList = game->GetPhysicsGhostList();

	// 背景画像の設定
	m_spriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_spriteRender.SetPosition(CENTER_POSITION);
	m_spriteRender.Update();

	// 装飾画像の設定
	m_outlineSpriteRender.Init("Assets/sprite/UI/miniMap/base_outLine.DDS", 362, 519);
	m_outlineSpriteRender.SetPosition({ 640.0f, -210.0f, 0.0f });
	m_outlineSpriteRender.Update();

	// プレイヤー画像の設定
	m_playerSpriteRender.Init("Assets/sprite/UI/miniMap/player.DDS", 20,40);
	m_playerSpriteRender.SetPosition(CENTER_POSITION);
	m_playerSpriteRender.Update();

	// エネミー画像の設定
	for (int i = 0; i < m_enemyList.size(); i++) {
		m_enemySpriteRender[i].Init("Assets/sprite/UI/miniMap/map_enemy.DDS", 15, 15);
		m_enableEnemySprites[i] = false;
	}

	//壁画像の設定
	for (int i = 0; i < m_physicsGhostList.size(); i++) {
		m_wallSpriteRender[i].Init("Assets/sprite/UI/miniMap/map_wall.DDS", 20.0f, 20.0f);
		m_enableWallSprites[i] = false;
	}

	// お宝画像の設定
	m_treasureSpriteRender.Init("Assets/sprite/UI/miniMap/map_exit.DDS", 32.0f, 32.0f);

	return true;
}

void MiniMap::Update()
{
	// プレイヤーの座標を取得
	m_playerPos = m_playerManagement->GetPosition();

	// マップ座標に変換
	DrawMap_Actor();
}

void MiniMap::DrawMap_Actor()
{
	Vector3 mapPos;
	Vector3 enemyPos;
	float alpha = 0.0f;
	m_isTreasure = false;

	//敵をマップに描画
	for (int i = 0; i < m_enemyList.size(); i++) {

		if (m_enemyList[i]->GetActiveFlag()) {
			continue;
		}

		enemyPos =  m_enemyList[i]->GetPosition();
		m_enableEnemySprites[i] = DrawMap(enemyPos, alpha);
		//敵画像の設定
		m_enemySpriteRender[i].SetPosition(m_mapPos);
		m_enemySpriteRender[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
		m_enemySpriteRender[i].Update();
	}

	//壁をマップに描画
	for (int i = 0; i < m_physicsGhostList.size(); i++) {

		enemyPos = m_physicsGhostList[i]->GetPosition();
		m_enableWallSprites[i] = DrawMap(enemyPos, alpha);
		//壁画像の設定
		m_wallSpriteRender[i].SetPosition(m_mapPos);
		m_wallSpriteRender[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
		m_wallSpriteRender[i].Update();
	}

	//お宝をマップに描画
	m_isTreasure = true;
	DrawMap(m_treasurePos, alpha);
	//お宝画像の設定
	m_treasureSpriteRender.SetPosition(m_mapPos);
	m_treasureSpriteRender.Update();
}

bool MiniMap::DrawMap(const Vector3& enemyPos, float& alpha)
{
	// マップに表示する範囲に敵がいたら
	if (WorldPositionConvertToMapPosition(m_playerPos, enemyPos)) {

		Vector3 diff = enemyPos - m_playerPos;
		diff.y = 0.0f;
		alpha = (pow(LIMITED_RANGE_IMAGE, 2.0f) - diff.LengthSq()) / 100000.0f;

		return true;
	}
	// マップに表示する敵がいなかったら
	else {
		return false;
	}
}

const bool MiniMap::WorldPositionConvertToMapPosition(const Vector3& worldCenterPosition, const Vector3& worldPosition)
{
	// 中心の座標から表示したいオブジェクトの座標へ向かうベクトルを計算
	Vector3 diff = worldPosition - worldCenterPosition;
	diff.y = 0.0f;
	Vector3 diff2 = diff;

	if (!m_isTreasure) {

		// 	計算したベクトルが一定以上離れていたら
		if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
			// 範囲外に存在しているのでマップに表示しない
			return false;
		}
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
	m_mapPos = Vector3(CENTER_POSITION.x + diff.x, CENTER_POSITION.y + diff.z, 0.0f);

	if (m_isTreasure) {

		//お宝がある方向へ回転させる
		rot.SetRotationZ(atan2(m_mapPos.y - CENTER_POSITION.y, m_mapPos.x - CENTER_POSITION.x) - 1.5f);
		m_treasureSpriteRender.SetRotation(rot);

		// 	計算したベクトルが一定以上離れていたら
		if (diff2.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
			//お宝座標の方向を求める
			diff2 =	Vector3(m_mapPos.x - CENTER_POSITION.x, m_mapPos.y - CENTER_POSITION.y, 0.0f);
			length = diff2.Length();

			//マップ上に写す座標を求める
			m_mapPos.x = CENTER_POSITION.x + (diff2.x / length) * MAP_RADIUS;
			m_mapPos.y = CENTER_POSITION.y + (diff2.y / length) * MAP_RADIUS;

			// 範囲外に存在している
			return false;
		}
	}
	return true;
}

void MiniMap::Render(RenderContext& rc)
{
	//背景画像の描画
	m_spriteRender.Draw(rc);

	//枠画像の描画
	m_outlineSpriteRender.Draw(rc);

	//敵画像の描画
	for (int i = 0; i < m_enemyList.size(); i++)
	{
		if (m_enableEnemySprites[i] == true) {
			m_enemySpriteRender[i].Draw(rc);
		}
	}

	//壁画像の描画
	for (int i = 0; i < m_physicsGhostList.size(); i++)
	{
		if (m_enableWallSprites[i]) {
			m_wallSpriteRender[i].Draw(rc);
		}
	}

	//お宝画像の描画
	m_treasureSpriteRender.Draw(rc);

	//プレイヤー画像の描画
	m_playerSpriteRender.Draw(rc);
}