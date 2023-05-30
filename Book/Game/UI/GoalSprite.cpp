#include "stdafx.h"
#include "GoalSprite.h"

namespace
{
	const Vector3	GOALSPRITE_FIRST_POS = { -1100.0f, -50.0f, 0.0f };		//�ڕW�摜�̏������W
	const float		GOALSPRITE_MOVESPEED = 10.5f;							//�ړ����x
	const float		GOALSPRITE_MAX_POS_X = -670.0f;							//X���W�̍ő�l
	const float		GOALSPRITE_ENABLE_TIME = 4.0f;							//�\������(1/2)
}


GoalSprite::GoalSprite()
{

}

GoalSprite::~GoalSprite()
{

}

bool GoalSprite::Start()
{
	m_goalSpriteRender[0].Init("Assets/sprite/UI/Gauge/image_target.DDS", 231.0f, 60.0f);
	m_goalSpriteRender[1].Init("Assets/sprite/UI/Gauge/image_target2.DDS", 231.0f, 60.0f);

	for (int i = 0; i < 2; i++) {
		m_goalSpriteRender[i].SetPosition(GOALSPRITE_FIRST_POS);
		m_goalSpriteRender[i].Update();
	}

	m_goalPos = GOALSPRITE_FIRST_POS;

	return true;
}

void GoalSprite::Update()
{
	m_goalTimer += g_gameTime->GetFrameDeltaTime();

	//���Ԃ��o�߂�����
	if (m_goalTimer > GOALSPRITE_ENABLE_TIME) {
		if (m_reverse) {
			DeleteGO(this);
		}
		else {
			m_reverse = true;
			m_goalTimer = 0.0f;
		}
	}

	if (m_goalTimer < GOALSPRITE_ENABLE_TIME / 2.0f) {

		float time = m_goalTimer / 2.0f;

		//�ړ��ʂ��v�Z����
		if (m_reverse) {
			// t^2
			m_goalPos.x -= pow(time, 2.0f) * GOALSPRITE_MOVESPEED;
		}
		else {
			// -t^2 + 2t
			m_goalPos.x += (((pow(time, 2.0f) * -2.0f) + (2.0f * time))) * GOALSPRITE_MOVESPEED;
		}
		m_goalPos.x = min(m_goalPos.x, GOALSPRITE_MAX_POS_X);

		//�摜�̐ݒ�
		m_goalSpriteRender[m_enableNum].SetPosition(m_goalPos);
		m_goalSpriteRender[m_enableNum].Update();
	}
}

void GoalSprite::Render(RenderContext& rc)
{
	m_goalSpriteRender[m_enableNum].Draw(rc);
}