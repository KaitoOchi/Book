#pragma once

class PlayerManagement;
class Enemy_Normal;
class Enemy_Search;
class Enemy_Charge;
class Game;
class Enemy;
class Treasure;
class PhysicsGhost;

namespace
{
	const int ENEMY_NUM = 50;							// �\�����邩�ǂ����̃t���O�p�B���߂ɗp�ӂ��Ă܂�
	const int WALL_NUM = 10;
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
	/// �i���\��������W��ݒ�B
	/// </summary>
	void SetTreasurePos(const Vector3& pos)
	{
		m_treasurePos = pos;
		m_TreasureSpriteRender.SetMulColor({ 1.0f,0.0f,0.0f,1.0f });
	}

private:
	/// <summary>
	/// �`�揈���B
	/// </summary>
	bool DrawMap(const Vector3& mapPos, float& alpha);

	/// <summary>
	/// �G��`�悷�鏈���B
	/// </summary>
	void DrawMap_Enemy();		

	/// <summary>
	/// ���[���h���W�n����}�b�v���W�n�ɕϊ��B
	/// </summary>
	/// <param name="worldcenterPosition">�}�b�v�̒��S�Ƃ���I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="worldPosition">�}�b�v�ɕ\���������I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="isTresure">true�Ȃ�͈͊O�ł��\������</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		Vector3 worldcenterPosition,
		Vector3 worldPosition,
		const bool isTreasure
	);

private:

	SpriteRender m_SpriteRender;							// �X�v���C�g�����_�[�B�~�j�}�b�v�̃x�[�X
	SpriteRender m_OutLineSpriteRender;						// �X�v���C�g�����_�[�B�~�j�}�b�v�̑�������
	SpriteRender m_PlayerSpriteRender;						// �X�v���C�g�����_�[�B�v���C���[
	std::array<SpriteRender,ENEMY_NUM>m_EnemySpriteRender;	// �X�v���C�g�����_�[�B�G�l�~�[
	std::array< SpriteRender, WALL_NUM > m_wallSpriteRender;
	SpriteRender m_TreasureSpriteRender;					// �X�v���C�g�����_�[�B����

	PlayerManagement* m_playerManagement = nullptr;
	Game* m_game = nullptr;
	Treasure* m_treasure = nullptr;

	std::vector<Enemy*> m_enemyList;						// �G�l�~�[�̃��X�g
	std::vector<PhysicsGhost*> m_physicsGhostList;	//�ǂ̃��X�g

	Vector3 m_treasurePos;			// ����̈ʒu
	Vector3 m_playerPos;							//�}�b�v��̃v���C���[�̍��W
	Vector3 m_mapPos;								//�v�Z�����}�b�v�ɏ悹��Ώۂ̍��W

	std::array<bool,ENEMY_NUM>m_isImage;						// �\�����邩�ǂ����̃t���O�B�G�l�~�[�̐����p�ӂ���
	std::array< bool, WALL_NUM > m_enableWallSprite;
};

