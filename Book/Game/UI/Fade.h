#pragma once


class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//フェードイン
	inline void StartFadeIn() { state = enState_FadeIn; }
	//フェードアウト
	inline void StartFadeOut() { state = enState_FadeOut; }
	//フェード中
	inline const bool IsFade() const { return state != enState_Idle; }

private:
	enum EnState {
		enState_FadeIn,			//フェードイン中
		enState_FadeOut,		//フェードアウト中
		enState_Idle,			//待機中
	};

	SpriteRender	m_spriteRender;
	EnState			state = enState_Idle;		//状態

	float m_alpha = 1.0f;					//透明度
};

