#include "stdafx.h"
#include "Wipe.h"
#include "Gage.h"

#include "BackGround.h"
#include "Stage/Wall/Wall.h"
#include "Stage/Wall/Wall_Normal.h"
#include "Stage/Wall/Wall_Post.h"
#include "Stage/Wall/Wall_Decoration.h"

namespace
{
	const float	MOVE_TIME = 1.3f;		//�ړ�����
	const float MOVE_SPEED = 0.3f;		//�ړ����x
	const float ENEMY_DURATION = 0.1f;	//�G�̊Ԋu
}

Wipe::Wipe()
{
	RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftX = -290;   //��ʍ����x���W
	RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftY = 662;   //��ʍ����y���W

	m_stage.reserve(16);
}

Wipe::~Wipe()
{
	delete m_enemyAnim;

	//�X�e�[�W���폜
	for (auto& stage : m_stage)
	{
		DeleteGO(stage);
	}
	m_stage.clear();
	m_stage.shrink_to_fit();

	//�����폜
	DeleteGO(m_backGround);
}

bool Wipe::Start()
{
	m_wipePos = WIPE_POS_MIN;
	m_outlinePos = OUTLINE_POS_MIN;

	//�|�C���g���C�g�̐ݒ�
	m_pointLight.SetPointLight(
		1,
		Vector3(11500.0f, 150.0f, 1200.0f),
		Vector3(1.0f, 0.2f, 0.2f),
		1500.0f
	);
	RenderingEngine::GetInstance()->GetLightCB().ptNum = 2;

	//�֊s���̐ݒ�
	m_outlineSpriteRender.Init("Assets/sprite/UI/Gauge/wipe_outline.DDS", 262.0f, 205.0f);
	m_outlineSpriteRender.SetPosition(m_outlinePos);
	m_outlineSpriteRender.Update();

	//�x���摜
	m_warningSpriteRender.Init("Assets/sprite/UI/Gauge/image_warning.DDS", 414.0f, 121.0f);
	m_warningSpriteRender.SetPosition(Vector3(-675.0f, -125.0f, 0.0f));
	m_warningSpriteRender.SetScale(Vector3(0.5f, 0.5f, 0.0f));
	m_warningSpriteRender.Update();

	//�A�j���[�V�����ݒ�
	m_enemyAnim = new AnimationClip;
	m_enemyAnim->Load("Assets/animData/enemy/run_battle.tka");
	m_enemyAnim->SetLoopFlag(true);

	//�G�̏�����
	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {
		m_enemy[i].modelRender.Init("Assets/modelData/enemy/enemy_normal.tkm", m_enemyAnim, 1, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, true);
		m_enemy[i].modelRender.SetPosition(Vector3(10000.0f, 0.0f, 0.0f));
		m_enemy[i].modelRender.SetScale(Vector3(2.0f, 2.0f, 2.0f));
		m_enemy[i].modelRender.Update();
		m_enemy[i].moveSpeed[0] = m_bezierPos[0];
		m_enemy[i].moveSpeed[1] = m_bezierPos[0];
		m_enemy[i].moveSpeed[2] = m_bezierPos[1];
	}

    LevelDesign();

	return true;
}

void Wipe::LevelDesign()
{
	int i = 0;

    // ���x���f�U�C������
    m_levelRender.Init("Assets/level3D/level_wipe.tkl", [&](LevelObjectData& objData) {

		//���O��background�Ȃ�
		if (objData.EqualObjectName(L"base") == true) {
			// �w�i�𐶐�
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);
			return true;
		}
		// ���O��box�Ȃ�
		else if (objData.EqualObjectName(L"box") == true) {
			// �ǂ𐶐�
			Wall_Normal* normal = NewGO<Wall_Normal>(0, "wall_Normal");
			normal->SetPosition(objData.position);
			normal->SetRotation(objData.rotation);
			normal->SetScale(objData.scale);
			m_stage.emplace_back(normal);
			return true;
		}
		// ���O��post�̂Ƃ�
		else if (objData.EqualObjectName(L"post") == true) {
			// ���𐶐�����
			Wall_Post* post = NewGO<Wall_Post>(0, "wall_Post");
			post->SetPosition(objData.position);
			post->SetRotation(objData.rotation);
			post->SetScale(objData.scale);
			m_stage.emplace_back(post);
			return true;
		}
		// ���O��decoration�̂Ƃ�
		else if (objData.EqualObjectName(L"decoration") == true) {
			// �����𐶐�
			Wall_Decoration* decoration = NewGO<Wall_Decoration>(0, "wall_Decoration");
			decoration->SetPosition(objData.position);
			decoration->SetRotation(objData.rotation);
			decoration->SetScale(objData.scale);
			m_stage.emplace_back(decoration);
			return true;
		}
		// ���O��position�̂Ƃ�
		else if (objData.EqualObjectName(L"position") == true) {
			// �x�W�F��̍��W
			m_bezierPos[i] = objData.position;
			i++;
			return true;
		}
    });
}

void Wipe::Update()
{
	EnemyMove();

	WipeOutline();
}

void Wipe::EnemyMove()
{
	if (!m_isWipe) {
		return;
	}

	//���Ԃ��o�߂�����A�ړ����~�߂�
	if (m_timer > MOVE_TIME) {
		m_isWipe = false;
		return;
	}

	//�v������
	m_timer += g_gameTime->GetFrameDeltaTime() * MOVE_SPEED;

	float timer = m_timer;

	//�G�̐�����for���ŉ�
	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {

		//�G�̊Ԋu��ݒ�
		timer = m_timer - (i * ENEMY_DURATION);

		Vector3 moveSpeed = m_enemy[i].moveSpeed[2];

		//�x�W�F�Ȑ��𗘗p���ēG���ړ�������
		m_enemy[i].moveSpeed[0].Lerp(timer, m_bezierPos[0], m_bezierPos[1]);
		m_enemy[i].moveSpeed[1].Lerp(timer, m_bezierPos[1], m_bezierPos[2]);
		m_enemy[i].moveSpeed[2].Lerp(timer, m_enemy[i].moveSpeed[0], m_enemy[i].moveSpeed[1]);

		//��]������
		moveSpeed -= m_enemy[i].moveSpeed[2];
		moveSpeed *= -1.0f;
		Quaternion rot;
		rot.SetRotationYFromDirectionXZ(moveSpeed);

		//�G���f���̐ݒ�
		m_enemy[i].modelRender.SetPosition(m_enemy[i].moveSpeed[2]);
		m_enemy[i].modelRender.SetRotation(rot);
		m_enemy[i].modelRender.Update();
	}
}

void Wipe::WipeOutline()
{
	if (!m_isWipe) {
		//���Ԃ��o�߂�����A���C�v�̈ړ����~�߂�
		if (m_outlineTimer <= 0.0f) {
			m_timer = 0.0f;
			return;
		}

		//���Ԃ��v��
		m_outlineTimer -= g_gameTime->GetFrameDeltaTime();
		m_outlineTimer = max(m_outlineTimer, 0.0f);
	}
	else {
		//���Ԃ��o�߂�����A���C�v�̈ړ����~�߂�
		if (m_outlineTimer > 1.0f) {
			return;
		}

		//���Ԃ��v��
		m_outlineTimer += g_gameTime->GetFrameDeltaTime();
		m_outlineTimer = min(m_outlineTimer, 1.0f);
	}

	//���C�v���ړ�
	m_outlinePos.Lerp(m_outlineTimer, OUTLINE_POS_MIN, OUTLINE_POS_MAX);
	m_wipePos.Lerp(m_outlineTimer, WIPE_POS_MIN, WIPE_POS_MAX);

	//���C�v�̐ݒ�
	RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftX = m_wipePos.x;
	m_outlineSpriteRender.SetPosition(m_outlinePos);
	m_outlineSpriteRender.Update();

	//�����x�����߂�
	m_alpha += g_gameTime->GetFrameDeltaTime();
	if (m_alpha > 1.0f)
		m_alpha = -0.5f;
	float alpha = fabsf(-pow(m_alpha, 2.0f) + (2 * m_alpha));

	Vector3 warningPos = m_outlinePos;
	warningPos.y += 150.0f;

	//�x���摜�̐ݒ�
	m_warningSpriteRender.SetPosition(warningPos);
	m_warningSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_warningSpriteRender.Update();
}

void Wipe::Render(RenderContext& rc)
{
	if (m_timer <= 0.0f) {
		return;
	}

	//�X�e�[�W�̕`��
	for (auto& wall : m_stage)
	{
		wall->WipeRender(rc);
	}

	//�G�̕`��
	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {
		m_enemy[i].modelRender.Draw(rc);
	}

	m_warningSpriteRender.Draw(rc);
	m_outlineSpriteRender.Draw(rc);
}