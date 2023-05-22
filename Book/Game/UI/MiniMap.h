#pragma once

class PlayerManagement;
class Enemy;
class PhysicsGhost;

using namespace std;

namespace
{
	const int ENEMY_NUM = 50;	//敵の数。少し多めに
	const int WALL_NUM = 10;	//隙間の数。少し多めに
}

class MiniMap:public IGameObject
{
public:
	MiniMap();
	~MiniMap();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// 永続表示する座標を設定。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetTreasurePos(const Vector3& pos)
	{
		m_treasurePos = pos;
	}

private:
	/// <summary>
	/// 描画共通処理。
	/// </summary>
	bool DrawMap(const Vector3& mapPos, float& alpha);

	/// <summary>
	/// アクタを描画する処理。
	/// </summary>
	void DrawMap_Actor();		

	/// <summary>
	/// ワールド座標系からマップ座標系に変換。
	/// </summary>
	/// <param name="worldcenterPosition">マップの中心とするオブジェクトのワールド座標</param>
	/// <param name="worldPosition">マップに表示したいオブジェクトのワールド座標</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		const Vector3& worldcenterPosition,
		const Vector3& worldPosition
	);

private:
	SpriteRender						m_spriteRender;					//ミニマップの背景画像
	SpriteRender						m_outlineSpriteRender;			//ミニマップの装飾画像
	SpriteRender						m_playerSpriteRender;			//プレイヤー画像
	SpriteRender						m_treasureSpriteRender;			//お宝画像
	array< SpriteRender, ENEMY_NUM >	m_enemySpriteRender;			//敵画像の配列
	array< SpriteRender, WALL_NUM >		m_wallSpriteRender;				//壁画像の配列
	PlayerManagement*					m_playerManagement = nullptr;	//プレイヤーマネジメントクラス
	vector<Enemy*>						m_enemyList;					//敵のリスト
	vector<PhysicsGhost*>				m_physicsGhostList;				//壁のリスト
	Vector3								m_treasurePos;					//お宝の位置
	Vector3								m_playerPos;					//マップ上のプレイヤーの座標
	Vector3								m_mapPos;						//計算したマップに乗せる対象の座標
	bool								m_isTreasure = false;			//お宝かどうか
	array< bool, ENEMY_NUM >			m_enableEnemySprites;			//敵を表示するかどうかのフラグ。敵の数分用意する
	array< bool, WALL_NUM >				m_enableWallSprites;			//壁を表示するかどうかのフラグ。壁の数分用意する
};

