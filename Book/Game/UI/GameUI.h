#pragma once

class PlayerManagement;
class Game;
class GameUI : public IGameObject	
{
public:
	GameUI();
	~GameUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �x���l���㏸����B
	/// </summary>
	/// <param name="GageUp"></param>
	void Vigilance(int GageUp);

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
	/// �x���x�̕ύX�����B
	/// </summary>
	void VigilanceChange();
	void TimeMove();
private:
	SpriteRender		m_gageFrameSpriteRender;		//�Q�[�W�̘g�摜
	SpriteRender		m_gageSpriteRender;				//�Q�[�W�摜
	SpriteRender		m_vigilanceRender;				//�x���x�摜
	FontRender			m_timeFontRender;				//�^�C������
	PlayerManagement*	m_playerManagement;
	Game*				m_game;
	int					m_vigilanceGage = 1;			//�x���x
	int					m_Gitgage = 0;
	float				m_timer = 0.0f;					//�^�C�}�[
	float				m_vigilanceTime = 0.0f;			//�x���x�p�̃^�C�}�[
	float				m_gage = 0.0f;					//MP�Q�[�W
	float				m_gaugeTimer = 0.0f;			//�Q�[�W�p�̃^�C�}�[

	float				m_timePosition = 0.0f;			//�^�C���̍��W

};

