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
	const int ENEMY_NUM = 50;							// �\�����邩�ǂ����̃t���O�p�B���߂ɗp�ӂ��Ă܂�
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
	/// �}�b�v�ɕ`�悷�邩�ǂ����̔���
	/// </summary>
	/// <param name="pos">�ϊ�����G�l�~�[�̍��W</param>
	/// <param name="num">�z��ԍ�</param>
	void DrawMap(Vector3 enemyPos,int num);		
	/// <summary>
	/// �����`�悷��
	/// </summary>
	void DrawMap_Treasure(Vector3 TreasurePos);

private:
	/// <summary>
	/// ���[���h���W�n����}�b�v���W�n�ɕϊ�
	/// </summary>
	/// <param name="worldcenterPosition">�}�b�v�̒��S�Ƃ���I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="worldPosition">�}�b�v�ɕ\���������I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="mapPosirion">�ϊ�������̃}�b�v���W</param>
	/// <param name="isTresure">true�Ȃ�͈͊O�ł��\������</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		Vector3 worldcenterPosition,
		Vector3 worldPosition,
		Vector3& mapPosirion,
		const bool isTresure
	);

	SpriteRender m_SpriteRender;							// �X�v���C�g�����_�[�B�~�j�}�b�v�̃x�[�X
	SpriteRender m_OutLineSpriteRender;						// �X�v���C�g�����_�[�B�~�j�}�b�v�̑�������
	SpriteRender m_PlayerSpriteRender;						// �X�v���C�g�����_�[�B�v���C���[
	std::array<SpriteRender,ENEMY_NUM>m_EnemySpriteRender;	// �X�v���C�g�����_�[�B�G�l�~�[
	SpriteRender m_TreasureSpriteRender;					// �X�v���C�g�����_�[�B����

	PlayerManagement* m_playerManagement = nullptr;
	Game* m_game = nullptr;
	Treasure* m_treasure = nullptr;

	std::vector<Enemy*> m_enemyList;				// �G�l�~�[�̃��X�g

	Vector3 m_treasurePos = Vector3::Zero;			// ����̈ʒu

	std::array<bool,ENEMY_NUM>m_isImage;						// �\�����邩�ǂ����̃t���O�B�G�l�~�[�̐����p�ӂ���
};

