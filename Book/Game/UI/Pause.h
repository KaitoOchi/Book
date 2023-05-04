#pragma once
#include "GameManager.h"
class Game;

class Pause : public IGameObject
{
public:
	enum PauseState
	{
		enState_FadeIn,
		enState_FadeOut,
		enState_Pause,
		enState_Game
	};

public:
	Pause();
	~Pause();
	bool Start();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �|�[�Y���̃A�b�v�f�[�g�����B
	/// </summary>
	void PauseUpdate();

	/// <summary>
	/// �|�[�Y���̏����B
	/// </summary>
	void PauseScreen();

	/// <summary>
	/// �t�F�[�h�C�������B
	/// </summary>
	void FadeIn();

	/// <summary>
	/// �t�F�[�h�A�E�g�����B
	/// </summary>
	void FadeOut();

	/// <summary>
	/// �X�v���C�g�̕ύX�����B
	/// </summary>
	void SetSprite();

	/// <summary>
	/// SE���o���B
	/// </summary>
	void PlaySE(const int num)
	{
		SoundSource* se = NewGO<SoundSource>(0, "sound");
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
	}

private:
	SpriteRender	m_backGroundSpriteRender;		//�w�i�摜
	SpriteRender	m_pauseSpriteRender;			//�|�[�Y���j���[�摜
	SpriteRender	m_cursorSpriteRender;			//�J�[�\���摜
	PauseState		m_pauseState = enState_FadeOut;	//�|�[�Y�X�e�[�g
	Game*			m_game = nullptr;				//�Q�[���N���X
	bool			m_isPause = false;				//�|�[�Y�������ǂ���
	int				m_cursor = 0;					//�J�[�\��
	float			m_timer = 0.0f;					//�^�C�}�[
};