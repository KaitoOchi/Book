#include "stdafx.h"
#include "Logo.h"

#include "Title.h"


namespace
{
	const float SPRITE_SCALE = 1.5f;		//�g�嗦�B
	const float LOGO_TIME = 3.0f;			//���S�̕\�����ԁB
	const float ANIM_CURVE_SPEED = 0.75;	//�A�j���[�V�������x�B
	const float ALPHA_MIN = 0.0f;			//�����x�̍ŏ��l�B
	const float ALPHA_MAX = 1.0f;			//�����x�̍ő�l�B
}

Logo::Logo()
{

}

Logo::~Logo()
{

}

bool Logo::Start()
{
	//���S�摜�̐ݒ�
	m_logoSpriteRender.Init("Assets/sprite/UI/title/image_rogo.DDS", 216.0f, 129.0f);
	m_logoSpriteRender.SetScale(Vector3(SPRITE_SCALE, SPRITE_SCALE, 0.0f));

	return true;
}

void Logo::Update()
{
	m_timer += g_gameTime->GetFrameDeltaTime();

	//���Ԃ��o�߂�����
	if (m_timer > LOGO_TIME) {
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}

	float alpha = ((pow(m_timer, 2.0f) * -ANIM_CURVE_SPEED) + (2.0f * m_timer));
	alpha = max(min(alpha, ALPHA_MAX), ALPHA_MIN);

	//���S�摜�̐ݒ�
	m_logoSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_logoSpriteRender.Update();
}

void Logo::Render(RenderContext& rc)
{
	m_logoSpriteRender.Draw(rc);
}