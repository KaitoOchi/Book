#pragma once


class Menu : public IGameObject
{
public:
	Menu();
	~Menu();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	int m_cursor = 0;		//ÉJÅ[É\Éã
};