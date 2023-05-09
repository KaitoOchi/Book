#pragma once

class PlayerManagement;
class Game;
class Player2D;
class GameUI : public IGameObject	
{
public:
	GameUI();
	~GameUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �^�C�����擾�B
	/// </summary>
	const float GetTime()
	{
		return m_timer;
	}

private:
	/// <summary>
	/// ���Ԍv�������B
	/// </summary>
	void Time();

	/// <summary>
	/// �Q�[�W�̕ύX�����B
	/// </summary>
	void ChangeGage();

	/// <summary>
	/// ���Ԃ̈ړ������B
	/// </summary>
	void TimeMove(const int& m, const int& s);

private:
	SpriteRender		m_gageFrameSpriteRender;		//�Q�[�W�̘g�摜
	SpriteRender		m_gageSpriteRender;				//�Q�[�W�摜
	FontRender			m_timeFontRender;				//�^�C������
	PlayerManagement*	m_playerManagement;
	Game*				m_game;
	Player2D*			m_player2D;
	bool				m_isTimerEnable = true;			//�^�C�}�[�̕\�����
	int					m_vigilanceGage = 1;			//�x���x
	float				m_timer = 0.0f;					//�^�C�}�[
	float				m_vigilanceTime = 0.0f;			//�x���x�p�̃^�C�}�[
	float				m_gage = 0.0f;					//MP�Q�[�W
	float				m_gaugeTimer = 0.0f;			//�Q�[�W�p�̃^�C�}�[
	float				m_timePosition = 0.0f;			//�^�C���̍��W

};

