#include "stdafx.h"
#include "Event.h"

#include "Game.h"
#include "Fade.h"
#include "GameManager.h"

namespace
{
	const int SCENE_MAX = 5;													//�V�[���̍ő吔

	const bool PLAYER_ENABLE[SCENE_MAX] = { true, true, true, false, true };	//�v���C���[�̕\�����
	const bool ENEMY_ENABLE[SCENE_MAX] = { false, false, false, true, false };	//�G�l�~�[�̕\�����
	const bool LIGHT_ENABLE[SCENE_MAX] = { false, false, true, false, true };	//���C�g�̕\�����
	const bool CAMERA_SET_POS[SCENE_MAX] = { true, true, false, true, true };	//�J�����̍��W�ύX�����邩
	const float SCENE_TIME[SCENE_MAX] = { 2.0f, 3.0f, 1.5f, 2.7f, 2.0f };		//�V�[���J�ڂ��邽�߂̎���
	const Vector3 CAMERA_POS[SCENE_MAX] = { { 0.0f, 50.0f, 200.0f },
									{ 100.0f, 50.0f, 50.0f }, 
									{ 100.0f, 50.0f, 50.0f },
									{ 285.0f, 10.0f, -50.0f },
									{ 0.0f, 50.0f, 200.0f } };			//�J�����̍��W
	const Vector3 CAMERA_TARGET[5] = { {0.0f, 75.0f, 0.0f},
									{ 0.0f, 50.0f, 50.0f },
									{ 0.0f, 50.0f, 0.0f },
									{ 260.0f, 10.0f, -50.0f },
									{ 0.0f, 75.0f, 0.0f } };			//�J�����̒����_
	const Vector3 CAMERA_SPEED[5] = { {0.0f, -5.0f, 0.0f},
									{ 0.0f, 0.0f, -3.0f },
									{ 0.0f, 0.0f, 0.0f },
									{ 0.0f, 0.0f, -150.0f },
									{ 0.0f, 3.0f, 0.0f } };			//�J�������x

	const Vector3 FILM_POS[4] = { { -750.0f, 0.0f, 0.0f },
								{ 750.0f, 0.0f, 0.0f },
								{ 0.0f, 420.0f, 0.0f },
								{ 0.0f, -420.0f, 0.0f } };			//�t�B�����̍��W
}

Event::Event()
{
}

Event::~Event()
{
	//�x�������폜
	m_alert->Stop();

	//�Q�[���N���X�ɃC�x���g�I����ʒm
	Game* game = FindGO<Game>("game");
	game->NotifyEventEnd();

	GameManager::GetInstance()->SetGameState(GameManager::enState_GetTresure);
}

bool Event::Start()
{
	m_tresurePos.y -= 50.0f;
	m_tresurePos.z -= 10.0f;

	//�v���C���[���f����ݒ�
	m_animationClips[animationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[animationClip_Idle].SetLoopFlag(true);
	m_animationClips[animationClip_Walk].Load("Assets/animData/player/event/walk_start.tka");
	m_animationClips[animationClip_Walk].SetLoopFlag(false);
	m_animationClips[animationClip_HeadUp].Load("Assets/animData/player/event/head_up.tka");
	m_animationClips[animationClip_HeadUp].SetLoopFlag(false);
	m_animationClips[animationClip_HeadStop].Load("Assets/animData/player/event/head_stop.tka");
	m_animationClips[animationClip_HeadStop].SetLoopFlag(false);
	m_animationClips[animationClip_HeadDown].Load("Assets/animData/player/event/head_down.tka");
	m_animationClips[animationClip_HeadDown].SetLoopFlag(false);
	m_animationClips[animationClip_RunAway].Load("Assets/animData/player/event/run_away.tka");
	m_animationClips[animationClip_RunAway].SetLoopFlag(false);
	m_playerModelRender.Init("Assets/modelData/player/player.tkm", m_animationClips, animationClip_Num, enModelUpAxisZ, 0, 0, 0, D3D12_CULL_MODE_BACK);
	m_playerModelRender.SetPosition(m_tresurePos);
	m_playerModelRender.Update();

	//�A�j���[�V�����C�x���g��ݒ�
	m_playerModelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName)
		{
			OnAnimationEvent(clipName, eventName);
		});


	//�G�l�~�[���f����ݒ�
	m_enemyAnimClips[enemyAnimClip_Run].Load("Assets/animData/enemy/run_battle.tka");
	m_enemyAnimClips[enemyAnimClip_Run].SetLoopFlag(true);
	for (int i = 0; i < 3; i++) {
		m_enemyModelRender[i].Init("Assets/modelData/enemy/enemy_normal.tkm", m_enemyAnimClips, enemyAnimClip_Num, enModelUpAxisZ);
		m_enemyModelRender[i].SetPosition(Vector3(260.0f, 0.0f, (-300.0f + (100 * i))));
		m_enemyModelRender[i].Update();
	}

	//�{�����[�����C�g���f���̏�����
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/preset/volumeLight.tkm";
	initData.m_fxFilePath = "Assets/shader/volumeLight.fx";
	initData.m_vsEntryPointFunc = "VSMain";
	initData.m_vsSkinEntryPointFunc = "VSMain";
	initData.m_psEntryPointFunc = "PSMain";
	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;
	initData.m_cullMode = D3D12_CULL_MODE_FRONT;

	//�{�����[�����C�g���f���̐ݒ�
	m_volumeLightModelRender.InitModelData(initData);
	m_volumeLightModelRender.SetPosition(m_tresurePos);
	m_volumeLightModelRender.Update();

	//�t�B�����摜�̐ݒ�
	m_filmSpriteRender[0].Init("Assets/sprite/UI/event/event_outline3.DDS", 200.0f, 960.0f, AlphaBlendMode_Trans);
	m_filmSpriteRender[1].Init("Assets/sprite/UI/event/event_outline3.DDS", 200.0f, 960.0f, AlphaBlendMode_Trans);
	m_filmSpriteRender[2].Init("Assets/sprite/UI/event/event_outline.DDS", 2000.0f, 80.0f, AlphaBlendMode_Trans, 4);
	m_filmSpriteRender[3].Init("Assets/sprite/UI/event/event_outline2.DDS", 2000.0f, 80.0f, AlphaBlendMode_Trans, 4);

	for (int i = 0; i < 4; i++) {
		m_filmSpriteRender[i].SetPosition(FILM_POS[i]);
		m_filmSpriteRender[i].Update();
	}

	//�J�����̐ݒ�
	m_cameraPos = CAMERA_POS[0];
	m_cameraTarget = CAMERA_TARGET[0];
	g_camera3D->SetNear(10.0f);
	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_cameraTarget);
	g_camera3D->Update();

	//BGM�̐ݒ�
	GameManager::GetInstance()->SetBGM(23);

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Event::Update()
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
			m_fade->StartFadeOut();
		}
	}

	Animation();

	Time();

	Camera();
}

void Event::Time()
{
	//���Ԃ��v��
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�t�B�����p�̎��Ԃ��v��
	m_filmTimer += 0.001f;
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.z = m_filmTimer;
}

void Event::Animation()
{	
	switch (m_cameraScene)
	{
	case 0:
		if (m_timer < 1.0f) {
			m_playerModelRender.PlayAnimation(animationClip_Walk, 0.0f);
		}
		else {
			m_playerModelRender.PlayAnimation(animationClip_Idle, 0.0f);
		}
		break;
	case 2:
		m_playerModelRender.PlayAnimation(animationClip_HeadUp, 0.0f);

		if (m_timer == 0.0f) {
			//���C�g�̉����o��
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(11);
			se->Play(false);
			se->SetVolume(GameManager::GetInstance()->GetSFX());
		}
		break;
	case 3:
		m_playerModelRender.PlayAnimation(animationClip_HeadStop, 0.0f);

		if (m_timer == 0.0f) {
			//�������o��
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(12);
			se->Play(false);
			se->SetVolume(GameManager::GetInstance()->GetSFX());

			//�x�������o��
			m_alert = NewGO<SoundSource>(0);
			m_alert->Init(3);
			m_alert->Play(true);
			m_alert->SetVolume(GameManager::GetInstance()->GetSFX());
		}
		break;
	case 4:
		if (m_timer < 1.0f) {
			m_playerModelRender.PlayAnimation(animationClip_HeadDown, 0.0f);
		}
		else {
			m_playerModelRender.PlayAnimation(animationClip_RunAway, 0.3f);
		}
		break;
	default:
		m_playerModelRender.PlayAnimation(animationClip_Idle, 0.5f);
		break;
	}
	m_playerModelRender.Update();

	for (int i = 0; i < 3; i++) {
		m_enemyModelRender[i].PlayAnimation(enemyAnimClip_Run, 0.5f);
		m_enemyModelRender[i].Update();
	}
}

void Event::Camera()
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
			g_camera3D->SetPosition(m_tresurePos + m_cameraPos);
			g_camera3D->SetTarget(m_tresurePos + m_cameraTarget);
		}
		g_camera3D->Update();
	}
}

void Event::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O��Attack_Start�̎�
	if (wcscmp(eventName, L"Step") == 0) {
		//�������Đ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(6);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
		se->Play(false);

	}
}

void Event::Render(RenderContext& rc)
{
	for (int i = 0; i < 4; i++) {
		m_filmSpriteRender[i].Draw(rc);
	}

	if (LIGHT_ENABLE[m_cameraScene] == true) {
		//���C�g���f���̕`��
		m_volumeLightModelRender.Draw(rc);
	}

	if (PLAYER_ENABLE[m_cameraScene] == true) {
		//�v���C���[���f���̕`��
		m_playerModelRender.Draw(rc);
	}

	if (ENEMY_ENABLE[m_cameraScene] == true) {
		for (int i = 0; i < 3; i++) {
			//�G���f���̕`��
			m_enemyModelRender[i].Draw(rc);
		}
	}
}