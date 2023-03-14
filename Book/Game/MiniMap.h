#pragma once

class Player;
class Player2D;
class Player3D;
class Enemy;
class MiniMap:public IGameObject
{
public:
	MiniMap();
	~MiniMap();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// ワールド座標系からマップ座標系に変換
	/// </summary>
	/// <param name="worldcenterPosition">マップの中心とするオブジェクトのワールド座標</param>
	/// <param name="worldPosition">マップに表示したいオブジェクトのワールド座標</param>
	/// <param name="mapPosirion">変換した後のマップ座標</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		Vector3 worldcenterPosition,
		Vector3 worldPosition,
		Vector3& mapPosirion
	);

	SpriteRender m_SpriteRender;
	SpriteRender m_PlayerSpriteRender;
	SpriteRender m_EnemySpriteRender;

	Player* m_player = nullptr;
	Player2D* player2D = nullptr;
	Player3D* player3D = nullptr;
	Enemy* m_enemy = nullptr;

	bool m_isImage = false;
};

