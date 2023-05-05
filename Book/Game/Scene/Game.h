#pragma once

class Player3D;
class Player2D;
class GameCamera;
class MiniMap;
class BackGround;
class Enemy;
class Enemy_Normal;
class Enemy_Search;
class Enemy_Charge;
class Enemy_Clear;
class PlayerManagement;
class Wall;
class Wall_Decoration;
class Wall_Door;
class Wall_Gap;
class Wall_Normal;
class Wall_Post;
class Treasure;
class Ghost;
class GameUI;
class FlashBom;
class SoundBom;
class Fade;
class Star;
class GameUI;
class Pause;
class GhostBox;
class PhysicsGhost;
class SenSor;
class SecurityCamera;
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
	enum EnGameSceneState
	{
		m_enGameState_DuringGamePlay,//�Q�[���v���C��
		m_enGameState_GameClearable,//クリア可能
		m_enGameState_GameFade,//�Q�[���N���A
		m_enGameState_GameOver,//�Q�[���I�[�o�[
		m_enGameState_GameBuck//�^�C�g���ɖ߂�
	};
	EnGameSceneState m_gameState = m_enGameState_DuringGamePlay;//���݂̃X�e�[�g�̊Ǘ�

	// �G�l�~�[�̃��X�g��Ԃ�
	std::vector<Enemy*> GetEnemyList()
	{
		return m_enemyList;
	}

	/// <summary>
	/// 星の情報を取得
	/// </summary>
	/// <returns></returns>
	std::vector<Star*> GetStarList()
	{
		return m_starList;
	}


	//クリア座標の取得
	std::vector<Vector3>m_clearList;

	//ライトの生成
	PointLight m_pointLight[4];
	int lights = 0;
	int m_lightNumber = 0;
public:

	/// <summary>
	/// 別のところで消去する
	/// </summary>
	void GameDelete(const int nextScene);
	/// <summary>
	/// ポーズ画面の切替
	/// </summary>
	void GamePos();
private:
	/// <summary>
	/// �N���A�X�e�[�g�ɑJ��
	/// </summary>
	void ClearState();
	/// <summary>
	/// �N���A�\�X�e�[�g�ɑJ��
	/// </summary>
	void Clearable();

	/// <summary>
	/// �X�e�[�g�̐ؑ�
	/// </summary>
	void MnageState();

	

	
	ModelRender m_stageModelRender;
	PhysicsStaticObject m_demobg;
	Player3D* m_player3D = nullptr;//3D�v���C���[
	Player2D* m_player2D = nullptr;//2D�v���C���[
	GameCamera* m_gamecamera = nullptr;

	LevelRender m_levelRender;
	MiniMap* m_miniMap = nullptr;

	// エネミー
	Enemy_Normal* m_enemyNormal = nullptr;
	Enemy_Search* m_enemySearch = nullptr;
	Enemy_Charge* m_enemyCharge = nullptr;
	Enemy_Clear* m_enemyClear = nullptr;
	
	BackGround* m_backGround = nullptr;
	
	FlashBom* m_flahBom = nullptr;
	SoundBom* m_soundBom = nullptr;
	
	std::vector<Enemy*> m_enemyList;
	
	std::vector<Star*>m_starList;

	std::vector<Wall*>m_wallList;

	PlayerManagement* m_playerManagement = nullptr;

	// ステージ
	Wall* m_wall = nullptr;
	Wall_Decoration* m_decoration = nullptr;
	Wall_Door* m_door = nullptr;
	Wall_Gap* m_gap = nullptr;
	Wall_Normal* m_normal = nullptr;
	Wall_Post* m_post = nullptr;
	Treasure* m_treaSure = nullptr;
	Ghost* m_ghost = nullptr;
	PhysicsGhost* m_physicsGhost = nullptr;
	GhostBox* m_ghostBox = nullptr;
	SenSor* m_senSor = nullptr;
	SecurityCamera* m_securityCamera = nullptr;


	Fade* m_fade = nullptr;
	GameUI* m_gameUI = nullptr;
	Star* m_star = nullptr;

	bool m_isWaitFadeOut = false;		//フェード状態かどうか
	int	m_nextScene = 0;				//次に移行するシーン

	int m_spotLigNum = 0;				// エネミー用スポットライトの数
};	

