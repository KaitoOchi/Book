#pragma once

class PlayerManagement;
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
	void Time();
	void ChangeGage();
	void VigilanceChange();

private:
	SpriteRender		m_gageFrameSpriteRender;
	SpriteRender		m_gageSpriteRender;
	SpriteRender		m_vigilanceRender;
	FontRender			m_timeFontRender;
	PlayerManagement*	m_playerManagement;
	int					m_vigilanceGage = 1;			//�x���x
	int					m_Gitgage = 0;
	float				m_timer = 0.0f;					//�^�C�}�[
	float				m_vigilanceTime = 0.0f;			//�x���x�p�̃^�C�}�[
	float				m_gage = 0.0f;					//MP�Q�[�W
	float				m_gaugeTimer = 0.0f;			//�Q�[�W�p�̃^�C�}�[
};

