#include "stdafx.h"
#include "Opening.h"

#include "Fade.h"
#include "GameManager.h"
#include "Game.h"
#include "nature/SkyCube.h"

namespace
{
	const int		SCENE_MAX = 5;													//�V�[���̍ő吔
	const float		CIRCLE_MAX = 360.0f;											//�~�`�Q�[�W�̍ő吔
	const float		CIRCLE_MIN = 0.0f;												//�~�`�Q�[�W�̍ŏ��l
	const float		CIRCLE_SPEED = 280.0f;											//�Q�[�W�̐i�ޑ���
	const float		CIRCLE_DECREASE = 60.0f;										//�Q�[�W�̌�����
	const double	PI = 3.14159;													//�~����

	const bool		CAMERA_SET_POS[SCENE_MAX] = { true, false, true, true, true };	//�J�����̍��W�ύX�����邩
	const float		SCENE_TIME[SCENE_MAX] = { 3.0f, 4.0f, 5.0f, 2.7f, 4.0f };		//�V�[���J�ڂ��邽�߂̎���

	const Vector3	CAMERA_POS[SCENE_MAX] = { { -50.0f, 20.0f, 50.0f },
										{ -50.0f, 20.0f, 0.0f },
										{ 0.0f, 50.0f, -50.0f },
										{ 0.0f, 50.0f, 100.0f },
										{ 0.0f, 40.0f, 0.0f } };					//�J�����̍��W
	const Vector3	CAMERA_TARGET[5] = { { 0.0f, 20.0f, 50.0f},
											{ 0.0f, 20.0f, 0.0f },
											{ 0.0f, 50.0f, 0.0f },
											{ 0.0f, 50.0f, 0.0f },
											{ 0.0f, 40.0f, 50.0f } };				//�J�����̒����_
	const Vector3	CAMERA_SPEED[5] = { { 0.0f, 0.0f, -16.0f},
											{ 0.0f, 5.0f, 0.0f },
											{ 0.0f, 0.0f, 10.0f },
											{ 0.0f, 0.0f, -10.0f },
											{ 0.0f, 0.0f, 5.0f } };				//�J�������x
	const Vector3	FILM_POS[4] = { { -750.0f, 0.0f, 0.0f },
									{ 750.0f, 0.0f, 0.0f },
									{ 0.0f, 420.0f, 0.0f },
									{ 0.0f, -420.0f, 0.0f } };						//�t�B�����̍��W
}

Opening::Opening()
{
	m_levelModelRender.reserve(16);
}

Opening::~Opening()
{
	DeleteGO(m_skyCube);

	for (auto& model : m_levelModelRender)
	{
		delete model;
	}
	m_levelModelRender.clear();
	m_levelModelRender.shrink_to_fit();
}

bool Opening::Start()
{
	RenderingEngine::GetInstance()->SetBloomThreshold(20.0f);
	g_camera3D->SetFar(10000.0f);

	//�Z�s�A���ɂ���
	RenderingEngine::GetInstance()->SetScreenProcess(1);

	InitModel();

	InitSprite();

	LevelDesign();

	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	m_skyCube->SetType(1);
	m_skyCube->SetScale(Vector3(700.0f, 700.0f, 700.0f));

	//�J�����̐ݒ�
	m_cameraPos = CAMERA_POS[0];
	m_cameraTarget = CAMERA_TARGET[0];
	g_camera3D->SetNear(10.0f);
	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_cameraTarget);
	g_camera3D->Update();

	//BGM�̐ݒ�
	GameManager::GetInstance()->SetBGM(26);

	//�t�F�[�h�̐ݒ�
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Opening::InitModel()
{
	m_animationClips[animationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[animationClip_Idle].SetLoopFlag(true);
	m_animationClips[animationClip_Walk].Load("Assets/animData/player/event/walk_start.tka");
	m_animationClips[animationClip_Walk].SetLoopFlag(false);
	m_animationClips[animationClip_Put].Load("Assets/animData/player/idle_act.tka");
	m_animationClips[animationClip_Put].SetLoopFlag(false);

	//�v���C���[���f���̐ݒ�
	m_playerModelRender.Init("Assets/modelData/player/player.tkm", m_animationClips, animationClip_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK);

	//�A�j���[�V�����C�x���g��ݒ�
	m_playerModelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName)
		{
			OnAnimationEvent(clipName, eventName);
		});

	//�ǃ��f���̐ݒ�
	m_wallModelRender.Init("Assets/modelData/event/wall.tkm", 0, 0, enModelUpAxisZ, true, false);
	m_wallModelRender.SetPosition(Vector3(0.0f, 0.0f, 100.0f));
	m_wallModelRender.Update();

	//�����f���̐ݒ�
	m_backGroundModelRender.Init("Assets/modelData/level_test/tkm/base.tkm", 0, 0, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK);
}

void Opening::InitSprite()
{
	//�t�B�����摜�̐ݒ�
	m_filmSpriteRender[0].Init("Assets/sprite/UI/event/event_outline3.DDS", 200.0f, 960.0f, AlphaBlendMode_Trans);
	m_filmSpriteRender[1].Init("Assets/sprite/UI/event/event_outline3.DDS", 200.0f, 960.0f, AlphaBlendMode_Trans);
	m_filmSpriteRender[2].Init("Assets/sprite/UI/event/event_outline.DDS", 2000.0f, 80.0f, AlphaBlendMode_Trans, SpriteRender::enMode_UVScroll);
	m_filmSpriteRender[3].Init("Assets/sprite/UI/event/event_outline2.DDS", 2000.0f, 80.0f, AlphaBlendMode_Trans, SpriteRender::enMode_UVScroll);
	for (int i = 0; i < 4; i++) {
		m_filmSpriteRender[i].SetPosition(FILM_POS[i]);
		m_filmSpriteRender[i].Update();
	}

	//�X�L�b�v�摜�̐ݒ�
	m_skipSpriteRender[0].Init("Assets/sprite/UI/PressAndHoldGauge/skip_base.DDS", 157.0f, 178.0f);
	m_skipSpriteRender[1].Init("Assets/sprite/UI/PressAndHoldGauge/gauge.DDS", 157.0f, 178.0f, AlphaBlendMode_Trans, SpriteRender::enMode_CircleGauge);
	for (int i = 0; i < 2; i++) {
		m_skipSpriteRender[i].SetPosition(Vector3(700.0f, -350.0f, 0.0f));
		m_skipSpriteRender[i].Update();
	}
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_degree * PI) / 180.0f;
}

void Opening::LevelDesign()
{
	// ���x�������_�[����
	m_levelRender.Init("Assets/level3D/level_opening.tkl", [&](LevelObjectData& objData) {

		if (objData.EqualObjectName(L"House_2") == true) {
			ModelRender* modelRender = new ModelRender;
			modelRender->Init("Assets/modelData/event/house_2.tkm", 0, 0, enModelUpAxisZ, true, true);
			modelRender->SetPosition(objData.position);
			modelRender->SetRotation(objData.rotation);
			modelRender->Update();
			m_levelModelRender.push_back(modelRender);
			return true;
		}
		else if (objData.EqualObjectName(L"House_3") == true) {
			ModelRender* modelRender = new ModelRender;
			modelRender->Init("Assets/modelData/event/house_3.tkm", 0, 0, enModelUpAxisZ, true, true);
			modelRender->SetPosition(objData.position);
			modelRender->SetRotation(objData.rotation);
			modelRender->Update();
			m_levelModelRender.push_back(modelRender);
			return true;
		}
		else if (objData.EqualObjectName(L"House_5") == true) {
			ModelRender* modelRender = new ModelRender;
			modelRender->Init("Assets/modelData/event/house_5.tkm", 0, 0, enModelUpAxisZ, true, true);
			modelRender->SetPosition(objData.position);
			modelRender->SetRotation(objData.rotation);
			modelRender->Update();
			m_levelModelRender.push_back(modelRender);
			return true;
		}
		else if (objData.EqualObjectName(L"lampPost") == true) {
			ModelRender* modelRender = new ModelRender;
			modelRender->Init("Assets/modelData/event/lampPost.tkm", 0, 0, enModelUpAxisZ, true, true);
			modelRender->SetPosition(objData.position);
			modelRender->SetRotation(objData.rotation);
			modelRender->Update();
			m_levelModelRender.push_back(modelRender);
			return true;
		}
		else if (objData.EqualObjectName(L"Church") == true) {
			ModelRender* modelRender = new ModelRender;
			modelRender->Init("Assets/modelData/event/charch.tkm", 0, 0, enModelUpAxisZ, true, true);
			modelRender->SetPosition(objData.position);
			modelRender->SetRotation(objData.rotation);
			modelRender->Update();
		m_levelModelRender.push_back(modelRender);
			return true;
		}
		return true;
	});
}


void Opening::Update()
{
	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I��������
		if (!m_fade->IsFade()) {
			DeleteGO(this);

			//�Q�[�����J�n
			NewGO<Game>(0, "game");

			GameManager::GetInstance()->SetGameState(GameManager::enState_Game);
			RenderingEngine::GetInstance()->SetBloomThreshold(0.2f);

			//�摜���H��ʏ�ɖ߂�
			RenderingEngine::GetInstance()->SetScreenProcess(0);
		}

		//���H���I��������
		m_processTimer -= g_gameTime->GetFrameDeltaTime() * 0.5f;
		m_processTimer = max(m_processTimer, 0.0f);
		RenderingEngine::GetInstance()->GetSpriteCB().processRate = m_processTimer;
	}
	else {
		//�t�F�[�h�A�E�g���n�߂�
		if (m_cameraScene == SCENE_MAX - 1 &&
			m_timer > 3.0f) {
			m_isWaitFadeOut = true;
			m_fade->StartFadeOut();
		}

		if (m_fade->IsFade()) {
			//�Z�s�A���ɉ��H����
			m_processTimer += g_gameTime->GetFrameDeltaTime() * 0.5f;
			m_processTimer = min(m_processTimer, 1.0f);
			RenderingEngine::GetInstance()->GetSpriteCB().processRate = m_processTimer;
		}

		Input();
	}

	Time();

	Animation();

	Camera();
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
		m_degree -= CIRCLE_SPEED * g_gameTime->GetFrameDeltaTime();

		//�Q�[�W���ő�ɂȂ�����X�L�b�v
		if (m_degree < CIRCLE_MIN) {
			m_isWaitFadeOut = true;
			m_fade->StartFadeOut();
		}
	}
	else {
		//������ĂȂ����A���X�Ɍ���
		m_degree += CIRCLE_DECREASE * g_gameTime->GetFrameDeltaTime();
		m_degree = min(m_degree, CIRCLE_MAX);
	}
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_degree * PI) / 180.0f;
}

void Opening::Animation()
{
	switch (m_cameraScene)
	{
	case 0:
		if (m_timer < 2.0f) {
			m_playerModelRender.SetAnimationSpeed(0.5f);
			m_playerModelRender.PlayAnimation(animationClip_Walk, 0.0f);
		}
		else {
			m_playerModelRender.SetAnimationSpeed(1.0f);
			m_playerModelRender.PlayAnimation(animationClip_Idle, 0.2f);
		}
		break;

	case 3:
		m_playerModelRender.PlayAnimation(animationClip_Put, 0.0f);
		break;

	default:
		m_playerModelRender.PlayAnimation(animationClip_Idle, 0.5f);
		break;
	}
	m_playerModelRender.Update();
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

		if (CAMERA_SET_POS[m_cameraScene]) {
			//�J�����̈ʒu��ݒ�
			m_cameraPos = CAMERA_POS[m_cameraScene];
			m_cameraTarget = CAMERA_TARGET[m_cameraScene];
		}
	}

	//�J�����̈ړ�
	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_cameraTarget);
	g_camera3D->Update();
}

void Opening::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void Opening::Render(RenderContext& rc)
{
	m_backGroundModelRender.Draw(rc);

	//�ǂ̕`��
	m_wallModelRender.Draw(rc);

	//�X�e�[�W�̕`��
	for (auto& model : m_levelModelRender)
	{
		model->Draw(rc);
	}

	//�v���C���[�̕`��
	if (m_cameraScene != 2) {
		m_playerModelRender.Draw(rc);
	}

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