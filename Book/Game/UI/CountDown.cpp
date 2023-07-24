#include "stdafx.h"
#include "CountDown.h"

#include "Game.h"
#include "PlayerManagement.h"
#include "GameManager.h"
#include "Pause.h"
#include "GoalSprite.h"

namespace
{
	const Vector3	SPRITE_POS = Vector3(0.0f, 150.0f, 0.0f);		//�摜�̍��W
	const int		COUNT_MAX = 3;									//�ő�J�E���g��
	const float		MOVE_MAX_TIME = 0.5f;							//�ړ�����
	const float		TIMER_MAX = 1.0f;								//�ő厞��
}


CountDown::CountDown()
{
	m_count = COUNT_MAX;
	m_timer = TIMER_MAX;
}

CountDown::~CountDown()
{
}

bool CountDown::Start()
{
	//�J�E���g�_�E���摜�̐ݒ�
	m_countDownSpriteRender[0].Init("Assets/sprite/UI/countDown/start.dds", 977.0f, 254.0f);
	m_countDownSpriteRender[1].Init("Assets/sprite/UI/countDown/1.dds", 73.0f, 234.0f);
	m_countDownSpriteRender[2].Init("Assets/sprite/UI/countDown/2.dds", 161.0f, 234.0f);
	m_countDownSpriteRender[3].Init("Assets/sprite/UI/countDown/3.dds", 166.0f, 255.0f);

	for (int i = 0; i < 4; i++) {
		m_countDownSpriteRender[i].SetPosition(SPRITE_POS);
		m_countDownSpriteRender[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
		m_countDownSpriteRender[i].Update();
	}

	return true;
}

void CountDown::Update()
{
	m_timer -= g_gameTime->GetFrameDeltaTime();

	//���Ԃ��o�߂�����
	if (m_timer < 0.0f) {

		//�J�E���g��i�߂ă^�C�������Z�b�g����
		m_count--;
		m_timer = TIMER_MAX;

		CountEnd();
		return;
	}

	//�J�E���g�摜�̐ݒ�
	if (m_timer > MOVE_MAX_TIME) {
		m_countDownSpriteRender[m_count].SetScale(Vector3(m_timer, m_timer, 0.0f));
		m_countDownSpriteRender[m_count].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 2.0f - (m_timer * 2.0f)));
		m_countDownSpriteRender[m_count].Update();
	}
}

void CountDown::CountEnd()
{
	//START�̕\�����I�������
	if (m_count < 0) {

		//�v���C���[�̈ړ����ł���悤�ɂ���
		PlayerManagement* player = FindGO<PlayerManagement>("playerManagement");
		player->SetGameState(true);

		//�Q�[���N���X�ɃQ�[������ʒm
		Game* game = FindGO<Game>("game");
		game->NotifyDuringGamePlay();

		//�ڕW�摜���o��
		GoalSprite* goalSprite = NewGO<GoalSprite>(0, "goalSprite");
		goalSprite->SetSpriteNum(false);

		//�|�[�Y���\�ɂ���
		NewGO<Pause>(2, "pause");

		DeleteGO(this);
		return;
	}
	//1�܂ŃJ�E���g������
	else if (m_count == 0) {
		PlaySE(16);
	}
	else {
		PlaySE(15);
	}
}

void CountDown::Render(RenderContext& rc)
{
	m_countDownSpriteRender[m_count].Draw(rc);
}