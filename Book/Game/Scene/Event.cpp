#include "stdafx.h"
#include "Event.h"

#include "Fade.h"

namespace
{
	const float SCENE_TIME[5] = { 3.0f, 3.0f, 2.75f, 5.0f, 3.0f };		//�V�[���J�ڂ��邽�߂̎���
	const Vector3 CAMERA_POS[5] = { { 0.0f, 0.0f, 0.0f },
									{ 100.0f, 50.0f, 50.0f }, 
									{ 25.0f, 10.0f, 150.0f }, 
									{ 0.0f, 50.0f, 200.0f }, 
									{ 0.0f, 0.0f, 0.0f }, };					//�J�����̍��W
	const Vector3 CAMERA_TARGET[5] = { {0.0f, 50.0f, 0.0f},
									{ 0.0f, 50.0f, 50.0f },
									{ 0.0f, 10.0f, 150.0f },
									{ 0.0f, 75.0f, 0.0f },
									{ 0.0f, 50.0f, 0.0f }, };					//�J�����̒����_
	const Vector3 CAMERA_SPEED[5] = { {0.0f, 1.0f, 0.0f},
									{ 0.0f, 0.0f, 1.0f },
									{ 0.0f, 0.0f, -25.0f },
									{ 0.0f, 1.0f, 0.0f },
									{ 0.0f, 0.0f, 0.0f }, };					//�J�������x
}

Event::Event()
{

}

Event::~Event()
{

}

bool Event::Start()
{
	//�v���C���[���f����ݒ�
	m_animationClips[animationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[animationClip_Idle].SetLoopFlag(true);
	m_playerModelRender.Init("Assets/modelData/player/player.tkm", m_animationClips, animationClip_Num, enModelUpAxisZ, 0, 0, 0, D3D12_CULL_MODE_BACK);

	//�G�l�~�[���f����ݒ�
	m_enemyAnimClips[enemyAnimClip_Run].Load("Assets/animData/enemy/run_battle.tka");
	m_enemyAnimClips[enemyAnimClip_Run].SetLoopFlag(true);
	for (int i = 0; i < 3; i++) {
		m_enemyModelRender[i].Init("Assets/modelData/enemy/enemy_normal.tkm", m_enemyAnimClips, enemyAnimClip_Num, enModelUpAxisZ);
		m_enemyModelRender[i].SetPosition(Vector3(0.0f, 0.0f, 50.0f * i));
		m_enemyModelRender[i].Update();
	}

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	m_cameraPos = { 0.0f, 150.0f, 200.0f };
	m_cameraTarget = { 0.0f, 50.0f, 0.0f };

	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_cameraTarget);
	g_camera3D->Update();

	return true;
}

void Event::Update()
{
	m_playerModelRender.PlayAnimation(animationClip_Idle, 0.5f);
	m_playerModelRender.Update();

	for (int i = 0; i < 3; i++) {
		m_enemyModelRender[i].PlayAnimation(enemyAnimClip_Run, 0.5f);
		m_enemyModelRender[i].Update();
	}

	m_timer += g_gameTime->GetFrameDeltaTime();

	Camera();
}

void Event::Camera()
{
	float move = m_timer * 0.05f;

	//�J�������x��ݒ�
	Vector3 cameraSpeed;
	cameraSpeed = CAMERA_SPEED[m_cameraScene] * move;

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

	//�J�����̈ړ�
	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_cameraTarget);
	g_camera3D->Update();
}

void Event::Render(RenderContext& rc)
{
	if (m_cameraScene == 0 ||
		m_cameraScene == 1 ||
		m_cameraScene == 3
		) {
		//�v���C���[���f���̕`��
		m_playerModelRender.Draw(rc);
	}
	else if (m_cameraScene == 2) {
		for (int i = 0; i < 3; i++) {
			//�G���f���̕`��
			m_enemyModelRender[i].Draw(rc);
		}
	}
}