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
	/// ���[���h���W�n����}�b�v���W�n�ɕϊ�
	/// </summary>
	/// <param name="worldcenterPosition">�}�b�v�̒��S�Ƃ���I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="worldPosition">�}�b�v�ɕ\���������I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="mapPosirion">�ϊ�������̃}�b�v���W</param>
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

