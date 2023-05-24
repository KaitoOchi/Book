#include "stdafx.h"
#include "Opening.h"

#include "Fade.h"
#include "GameManager.h"
#include "Game.h"

namespace
{
	const int		SCENE_MAX = 5;													//�V�[���̍ő吔
	const float		CIRCLE_MAX = 360.0f;											//�~�`�Q�[�W�̍ő吔
	const double	PI = 3.14159;													//�~����

	const bool		CAMERA_SET_POS[SCENE_MAX] = { true, false, true, true, true };	//�J�����̍��W�ύX�����邩
	const float		SCENE_TIME[SCENE_MAX] = { 3.0f, 3.0f, 3.0f, 2.7f, 2.0f };		//�V�[���J�ڂ��邽�߂̎���

	const Vector3	CAMERA_POS[SCENE_MAX] = { { 50.0f, 20.0f, 100.0f },
										{ 170.0f, 50.0f, 30.0f },
										{ 100.0f, 50.0f, 50.0f },
										{ 220.0f, 5.0f, 0.0f },
										{ 0.0f, 50.0f, 200.0f } };			//�J�����̍��W
	const Vector3	CAMERA_TARGET[5] = { {0.0f, 20.0f, 0.0f},
											{ 0.0f, 50.0f, 30.0f },
											{ 0.0f, 50.0f, 0.0f },
											{ 200.0f, 5.0f, 0.0f },
											{ 0.0f, 75.0f, 0.0f } };			//�J�����̒����_
	const Vector3	CAMERA_SPEED[5] = { {0.0f, 0.0f, 50.0f},
											{ 0.0f, 50.0f, 0.0f },
											{ 0.0f, 0.0f, 0.0f },
											{ 0.0f, 0.0f, -150.0f },
											{ 0.0f, 3.0f, 0.0f } };			//�J�������x
	const Vector3	FILM_POS[4] = { { -750.0f, 0.0f, 0.0f },
									{ 750.0f, 0.0f, 0.0f },
									{ 0.0f, 420.0f, 0.0f },
									{ 0.0f, -420.0f, 0.0f } };			//�t�B�����̍��W
}

Opening::Opening()
{

}

Opening::~Opening()
{
	//�Q�[�����J�n
	NewGO<Game>(0, "game");
	DeleteGO(this);

	GameManager::GetInstance()->SetGameState(GameManager::enState_Game);
}

bool Opening::Start()
{
	m_animationClips[animationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[animationClip_Idle].SetLoopFlag(true);
	m_animationClips[animationClip_Walk].Load("Assets/animData/player/event/walk_start.tka");
	m_animationClips[animationClip_Walk].SetLoopFlag(false);
	m_animationClips[animationClip_Put].Load("Assets/animData/player/idle_act.tka");
	m_animationClips[animationClip_Put].SetLoopFlag(false);

	//�v���C���[���f���̐ݒ�
	m_playerModelRender.Init("Assets/modelData/player/player.tkm", m_animationClips, animationClip_Num, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK);

	//�t�B�����摜�̐ݒ�
	m_filmSpriteRender[0].Init("Assets/sprite/UI/event/event_outline3.DDS", 200.0f, 960.0f, AlphaBlendMode_Trans);
	m_filmSpriteRender[1].Init("Assets/sprite/UI/event/event_outline3.DDS", 200.0f, 960.0f, AlphaBlendMode_Trans);
	m_filmSpriteRender[2].Init("Assets/sprite/UI/event/event_outline.DDS", 2000.0f, 80.0f, AlphaBlendMode_Trans, 4);
	m_filmSpriteRender[3].Init("Assets/sprite/UI/event/event_outline2.DDS", 2000.0f, 80.0f, AlphaBlendMode_Trans, 4);
	for (int i = 0; i < 4; i++) {
		m_filmSpriteRender[i].SetPosition(FILM_POS[i]);
		m_filmSpriteRender[i].Update();
	}

	//�X�L�b�v�摜�̐ݒ�
	m_skipSpriteRender[0].Init("Assets/sprite/UI/PressAndHoldGauge/skip_base.DDS", 157.0f, 178.0f);
	m_skipSpriteRender[1].Init("Assets/sprite/UI/PressAndHoldGauge/gauge.DDS", 157.0f, 178.0f, AlphaBlendMode_Trans, 5);
	for (int i = 0; i < 2; i++) {
		m_skipSpriteRender[i].SetPosition(Vector3(700.0f, -350.0f, 0.0f));
		m_skipSpriteRender[i].Update();
	}
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_degree * PI) / 180.0f;

	//�J�����̐ݒ�
	m_cameraPos = CAMERA_POS[0];
	m_cameraTarget = CAMERA_TARGET[0];
	g_camera3D->SetNear(10.0f);
	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_cameraTarget);
	g_camera3D->Update();

	//BGM�̐ݒ�
	GameManager::GetInstance()->SetBGM(23);

	//�t�F�[�h�̐ݒ�
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Opening::Update()
{
	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I��������
		if (!m_fade->IsFade()) {
			DeleteGO(this);
		}
	}
	else {
		//�t�F�[�h�A�E�g���n�߂�
		if (m_cameraScene == SCENE_MAX - 1 &&
			m_timer > 1.0f) {
			m_isWaitFadeOut = true;
			m_fade->SetEnableTips(false);
			m_fade->StartFadeOut();
		}
	}

	Time();

	Input();
}

void Opening::Time()
{
	//���Ԃ��v��
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�t�B�����p�̎��Ԃ��v��
	m_filmTimer += 0.0005;
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.z = m_filmTimer;
}

void Opening::Input()
{
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsPress(enButtonA)) {
		m_degree -= 120.0f * g_gameTime->GetFrameDeltaTime();

		//�Q�[�W���ő�ɂȂ�����X�L�b�v
		if (m_degree < 0.0f) {
			m_isWaitFadeOut = true;
			m_fade->SetEnableTips(false);
			m_fade->StartFadeOut();
		}
	}
	else {
		//������ĂȂ����A���X�Ɍ���
		m_degree += 60.0f * g_gameTime->GetFrameDeltaTime();
		m_degree = min(m_degree, CIRCLE_MAX);
	}
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_degree * PI) / 180.0f;
}

void Opening::Camera()
{
	//�J�������x��ݒ�
	Vector3 cameraSpeed;

	cameraSpeed = CAMERA_SPEED[m_cameraScene] * g_gameTime->GetFrameDeltaTime() * 1.0f;
	m_cameraPos += cameraSpeed;
	m_cameraTarget += cameraSpeed;

	//�J�����̑J�ڎ��Ԃ��o�߂�����
	if (m_timer > SCENE_TIME[m_cameraScene]) {
		m_cameraScene += 1;
		m_timer = 0.0f;

		//�J�����̈ʒu��ݒ�
		m_cameraPos = CAMERA_POS[m_cameraScene];
		m_cameraTarget = CAMERA_TARGET[m_cameraScene];
	}

	//�J�������ړ��\�ȃV�[���Ȃ�
	if (CAMERA_SET_POS[m_cameraScene]) {

		//�J�����̈ړ�
		if (m_cameraScene == 3) {
			g_camera3D->SetPosition(m_cameraPos);
			g_camera3D->SetTarget(m_cameraTarget);
		}
		else {
			g_camera3D->SetPosition(m_cameraPos);
			g_camera3D->SetTarget(m_cameraTarget);
		}
		g_camera3D->Update();
	}
}


void Opening::Render(RenderContext& rc)
{
	//�t�B�����摜�̕`��
	for (int i = 0; i < 4; i++) {
		m_filmSpriteRender[i].Draw(rc);
	}

	//�Q�[�W�摜�̕`��
	if (m_degree < 360.0f) {
		m_skipSpriteRender[1].Draw(rc);
		m_skipSpriteRender[0].Draw(rc);
	}
}