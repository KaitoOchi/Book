#pragma once
class Player3D;
class Player2D;
class GameCamera;
class Enemy;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void LevelDesign();			// �X�e�[�W�̃��x���f�U�C���̏���
	enum EnGameState
	{
		m_enGameState_DuringGamePlay,//�Q�[���v���C��
		m_enGameState_GameClear,//�Q�[���N���A
		m_enGameState_GameOver,//�Q�[���I�[�o�[
		m_enGameState_GameBuck//�^�C�g���ɖ߂�
	};
private:
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

	LevelRender* m_levelRender = nullptr;
};

