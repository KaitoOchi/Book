#pragma once

class Player3D;
class Player2D;
class GameCamera;
class MiniMap;
class BackGround;
class Enemy;
class Enemy_Normal;
class Enemy_Search;
class TransparentBox;
class Enemy_Charge;
class PlayerManagement;
class Wall;
class Treasure;
class Gost;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void LevelDesign();			// �X�e�[�W�̃��x���f�U�C���̏���

	//�N���A���W��Ԃ�
	Vector3 GetClearPosition()
	{
		return m_position;
	}
	void SetClearPosition(Vector3 m_pos)
	{
		m_position = m_pos;
	}
	
	Vector3 m_position = Vector3::Zero;
	enum EnGameState
	{
		m_enGameState_DuringGamePlay,//�Q�[���v���C��
		m_enGameState_GameClear,//�Q�[���N���A
		m_enGameState_GameOver,//�Q�[���I�[�o�[
		m_enGameState_GameBuck//�^�C�g���ɖ߂�
	};
private:
	/// <summary>
	/// �N���A�X�e�[�g�ɑJ��
	/// </summary>
	void ClearState();

	/// <summary>
	/// �X�e�[�g�̐ؑ�
	/// </summary>
	void MnageState();
	EnGameState m_gameState=m_enGameState_DuringGamePlay;//���݂̃X�e�[�g�̊Ǘ�
	ModelRender m_stageModelRender;
	PhysicsStaticObject m_demobg;
	Player3D* m_player3D = nullptr;//3D�v���C���[
	Player2D* m_player2D = nullptr;//2D�v���C���[
	GameCamera* m_gamecamera = nullptr;

	LevelRender m_levelRender;
	MiniMap* m_miniMap = nullptr;
	Enemy_Normal* m_enemyNormal = nullptr;
	Enemy_Search* m_enemySearch = nullptr;
	Enemy_Charge* m_enemyCharge = nullptr;
	BackGround* m_backGround = nullptr;

	std::vector<Enemy*> m_enemyList;

	TransparentBox *m_trans = nullptr;
	PlayerManagement* m_playerManagement = nullptr;
	Wall* m_wall = nullptr;
	Treasure* m_treaSure = nullptr;
	Gost* m_gost = nullptr;
	
	PointLight m_pointLight;

};

