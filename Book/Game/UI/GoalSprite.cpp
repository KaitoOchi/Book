#include "stdafx.h"
#include "GoalSprite.h"

namespace
{
	const Vector3	GOALSPRITE_FIRST_POS = Vector3(-0.0f, 300.0f, 0.0f);	//�摜�̍��W�B
	const Vector3	GOALSPRITE_SCALE = Vector3(1.5f, 1.5f, 1.5f);			//�摜�̊g�嗦�B
	const float		SPRITE_WIDTH = 231.0f;									//�摜�̏c���B
	const float		SPRITE_HEIGHT = 60.0f;									//�摜�̉����B
	const float		GOALSPRITE_MOVESPEED = 30.0f;							//�ړ����x�B
	const float		GOALSPRITE_ENABLE_TIME = 4.5f;							//�\�����ԁB
	const float		ALPHA_TIMER_MIN = -0.5f;								//�^�C�}�[�̍ŏ��l�B
	const float		ALPHA_TIMER_MAX = 1.0f;									//�^�C�}�[�̍ő�l�B
}


GoalSprite::GoalSprite()
{

}

GoalSprite::~GoalSprite()
{

}

bool GoalSprite::Start()
{
	m_goalSpriteRender[0].Init("Assets/sprite/UI/Gauge/image_target.DDS", SPRITE_WIDTH, SPRITE_HEIGHT);
	m_goalSpriteRender[1].Init("Assets/sprite/UI/Gauge/image_target2.DDS", SPRITE_WIDTH, SPRITE_HEIGHT);

	for (int i = 0; i < 2; i++) {
		m_goalSpriteRender[i].SetPosition(GOALSPRITE_FIRST_POS);
		m_goalSpriteRender[i].SetScale(GOALSPRITE_SCALE);
		m_goalSpriteRender[i].Update();
	}

	m_goalPos = GOALSPRITE_FIRST_POS;

	return true;
}

void GoalSprite::Update()
{
	//���Ԃ��o�߂�����
	if (m_goalTimer > GOALSPRITE_ENABLE_TIME) {
		DeleteGO(this);
		return;
	}

	m_goalTimer += g_gameTime->GetFrameDeltaTime();

	m_alphaTimer += g_gameTime->GetFrameDeltaTime();

	if (m_alphaTimer > ALPHA_TIMER_MAX) {
		m_alphaTimer = ALPHA_TIMER_MIN;
	}

	//���ߗʂ��v�Z����
	float alpha	= fabsf(-pow(m_alphaTimer, 2.0f) + (2.0f * m_alphaTimer));

	//�摜�̐ݒ�
	m_goalSpriteRender[m_enableNum].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_goalSpriteRender[m_enableNum].Update();
}

void GoalSprite::Render(RenderContext& rc)
{
	m_goalSpriteRender[m_enableNum].Draw(rc);
}