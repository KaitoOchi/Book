#pragma once
//Programmer : KaitoOchi


class Menu : public IGameObject
{
public:
	Menu();
	~Menu();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	FontRender m_fontRender;
	FontRender m_cursorFontRender;

	int m_cursor = 0;		//ÉJÅ[É\Éã
};