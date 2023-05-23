#pragma once

class Fade;
class Title;

class Title_Guide : public IGameObject
{
public:
	Title_Guide();
	~Title_Guide();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// �^�C�g���N���X�̃|�C���^��ݒ�B
	/// </summary>
	void SetTitlePtr(Title* title)
	{
		m_title = title;
	}

private:
	/// <summary>
	/// ���͏����B
	/// </summary>
	void Input();

	/// <summary>
	/// �X�e�[�g�̑J�ڏ����B
	/// </summary>
	void StateChange();

	/// <summary>
	/// �A�j���[�V���������B
	/// </summary>
	void Animation();

private:
	SpriteRender						m_guideBackSpriteRender;	//�V�ѕ��w�i�摜
	std::array< SpriteRender, 2 >		m_guideSpriteRender;		//�K�C�h�摜
	std::array< SpriteRender, 2 >		m_buttonSpriteRender;		//�{�^���摜
	std::vector< SpriteRender* >		m_sprites;					//SpriteRender�̃x�N�^�[�^

	Fade*								m_fade = nullptr;
	Title*								m_title = nullptr;

	Vector3								m_bezierPos[4];				//�x�W�F�Ȑ��̍��W
	Vector3								m_guidePos[2];				//�K�C�h�̍��W

	bool								m_isWaitState = false;		//�X�e�[�g�̑J�ڑ҂����ǂ���
	bool								m_isSceneChange = false;	//�V�[����J�ڂ��邩�ǂ���
	bool								m_isWaitFadeOut = false;	//�t�F�[�h�����ǂ���
	bool								m_forward = false;			//�K�C�h�摜�̑S��
	int									m_cursor = 0;				//�J�[�\��
	float								m_timer = 0.0f;				//����
	float								m_alpha = 0.0f;				//�����x
};