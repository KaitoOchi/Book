#pragma once

class PlayerManagement;
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

	PlayerManagement* m_playerManagement = nullptr;
	Enemy* m_enemy = nullptr;

	bool m_isImage = false;
};

