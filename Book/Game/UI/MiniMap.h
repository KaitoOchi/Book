#pragma once

class PlayerManagement;
class Enemy;
class PhysicsGhost;

using namespace std;

namespace
{
	const int ENEMY_NUM = 50;	//�G�̐��B�������߂�
	const int WALL_NUM = 10;	//���Ԃ̐��B�������߂�
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
	/// <param name="pos">���W�B</param>
	void SetTreasurePos(const Vector3& pos)
	{
		m_treasurePos = pos;
	}

private:
	/// <summary>
	/// �`�拤�ʏ����B
	/// </summary>
	bool DrawMap(const Vector3& mapPos, float& alpha);

	/// <summary>
	/// �A�N�^��`�悷�鏈���B
	/// </summary>
	void DrawMap_Actor();		

	/// <summary>
	/// ���[���h���W�n����}�b�v���W�n�ɕϊ��B
	/// </summary>
	/// <param name="worldcenterPosition">�}�b�v�̒��S�Ƃ���I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="worldPosition">�}�b�v�ɕ\���������I�u�W�F�N�g�̃��[���h���W</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		const Vector3& worldcenterPosition,
		const Vector3& worldPosition
	);

private:
	SpriteRender						m_spriteRender;					//�~�j�}�b�v�̔w�i�摜
	SpriteRender						m_outlineSpriteRender;			//�~�j�}�b�v�̑����摜
	SpriteRender						m_playerSpriteRender;			//�v���C���[�摜
	SpriteRender						m_treasureSpriteRender;			//����摜
	array< SpriteRender, ENEMY_NUM >	m_enemySpriteRender;			//�G�摜�̔z��
	array< SpriteRender, WALL_NUM >		m_wallSpriteRender;				//�ǉ摜�̔z��
	PlayerManagement*					m_playerManagement = nullptr;	//�v���C���[�}�l�W�����g�N���X
	vector<Enemy*>						m_enemyList;					//�G�̃��X�g
	vector<PhysicsGhost*>				m_physicsGhostList;				//�ǂ̃��X�g
	Vector3								m_treasurePos;					//����̈ʒu
	Vector3								m_playerPos;					//�}�b�v��̃v���C���[�̍��W
	Vector3								m_mapPos;						//�v�Z�����}�b�v�ɏ悹��Ώۂ̍��W
	bool								m_isTreasure = false;			//���󂩂ǂ���
	array< bool, ENEMY_NUM >			m_enableEnemySprites;			//�G��\�����邩�ǂ����̃t���O�B�G�̐����p�ӂ���
	array< bool, WALL_NUM >				m_enableWallSprites;			//�ǂ�\�����邩�ǂ����̃t���O�B�ǂ̐����p�ӂ���
};

