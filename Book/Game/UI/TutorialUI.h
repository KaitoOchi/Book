#pragma once

class GameUI;
class TutorialUI:public IGameObject
{
public:
	TutorialUI();
	~TutorialUI();

private:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// �`���[�g���A���̉摜�̕`���Ԃ�ݒ�B
	/// </summary>
	/// <param name="state1">true�Ȃ�`�悷��B</param>
	void SetTutorialDrawState(const bool state)
	{
		m_tutorialDrawState = state;
	}

	/// <summary>
	/// ���݂̕`���Ԃ��擾����B
	/// </summary>
	/// <returns></returns>
	const bool GetTutorialDrawState()
	{
		return m_tutorialDrawState;
	}

private:
	SpriteRender		m_guideSpriteRender;			//�`���[�g���A���K�C�h�̃e�L�X�g�摜
	bool				m_tutorialDrawState = true;		//�`���[�g���A���̃e�L�X�g�摜��`�悷�邩�ǂ���
};

