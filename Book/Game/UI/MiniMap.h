#pragma once

class PlayerManagement;
class Enemy_Normal;
class Enemy_Search;
class Enemy_Charge;
class Game;
class Enemy;
class Treasure;

namespace
{
	const int ENEMY_NUM = 50;							// 表示するかどうかのフラグ用。多めに用意してます
}

class MiniMap:public IGameObject
{
public:
	MiniMap();
	~MiniMap();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// マップに描画するかどうかの判定
	/// </summary>
	/// <param name="pos">変換するエネミーの座標</param>
	/// <param name="num">配列番号</param>
	void DrawMap(Vector3 enemyPos,int num);		
	/// <summary>
	/// お宝を描画する
	/// </summary>
	void DrawMap_Treasure(Vector3 TreasurePos);

private:
	/// <summary>
	/// ワールド座標系からマップ座標系に変換
	/// </summary>
	/// <param name="worldcenterPosition">マップの中心とするオブジェクトのワールド座標</param>
	/// <param name="worldPosition">マップに表示したいオブジェクトのワールド座標</param>
	/// <param name="mapPosirion">変換した後のマップ座標</param>
	/// <param name="isTresure">trueなら範囲外でも表示する</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		Vector3 worldcenterPosition,
		Vector3 worldPosition,
		Vector3& mapPosirion,
		const bool isTresure
	);

	SpriteRender m_SpriteRender;							// スプライトレンダー。ミニマップのベース
	SpriteRender m_OutLineSpriteRender;						// スプライトレンダー。ミニマップの装飾部分
	SpriteRender m_PlayerSpriteRender;						// スプライトレンダー。プレイヤー
	std::array<SpriteRender,ENEMY_NUM>m_EnemySpriteRender;	// スプライトレンダー。エネミー
	SpriteRender m_TreasureSpriteRender;					// スプライトレンダー。お宝

	PlayerManagement* m_playerManagement = nullptr;
	Game* m_game = nullptr;
	Treasure* m_treasure = nullptr;

	std::vector<Enemy*> m_enemyList;				// エネミーのリスト

	Vector3 m_treasurePos = Vector3::Zero;			// お宝の位置

	std::array<bool,ENEMY_NUM>m_isImage;						// 表示するかどうかのフラグ。エネミーの数分用意する
};

