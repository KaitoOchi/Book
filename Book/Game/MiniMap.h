#pragma once

class PlayerManagement;
class Enemy_Normal;
class MiniMap:public IGameObject
{
public:
	MiniMap();
	~MiniMap();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void DrawMap();		// �}�b�v�ɕ`�悷�邩�ǂ����̔���

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
	SpriteRender m_OutLineSpriteRender;		// ����̕���
	SpriteRender m_PlayerSpriteRender;
	SpriteRender m_EnemySpriteRender;

	PlayerManagement* m_playerManagement = nullptr;
	Enemy_Normal* m_enemyNormal = nullptr;

	bool m_isImage = false;
};

