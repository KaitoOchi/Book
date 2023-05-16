#include "stdafx.h"
#include "Wipe.h"

#include "BackGround.h"
#include "Stage/Wall/Wall.h"
#include "Stage/Wall/Wall_Normal.h"
#include "Stage/Wall/Wall_Post.h"
#include "Stage/Wall/Wall_Decoration.h"

namespace
{
	const float	MOVE_TIME = 1.2f;			//�ړ�����
	const float MOVE_SPEED = 0.3f;			//�ړ����x
	const float ENEMY_DURATION = 0.1f;		//�G�̊Ԋu
}

Wipe::Wipe()
{

}

Wipe::~Wipe()
{
	delete m_enemyAnim;

	for (auto stage : m_stage)
	{
		DeleteGO(stage);
	}

	DeleteGO(m_backGround);
}

bool Wipe::Start()
{
	//�A�j���[�V�����ݒ�
	m_enemyAnim = new AnimationClip;
	m_enemyAnim->Load("Assets/animData/enemy/run_battle.tka");
	m_enemyAnim->SetLoopFlag(true);

	//�G�̏�����
	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {
		m_enemy[i].modelRender.Init("Assets/modelData/enemy/enemy_normal.tkm", m_enemyAnim, 1, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
		m_enemy[i].modelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
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
    if (g_pad[0]->IsTrigger(enButtonA)) {

        RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftY += 10;
        RenderingEngine::GetInstance()->GetWipeViewPort().Height += 10;
    }
    if (g_pad[0]->IsTrigger(enButtonB)) {
        RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftX += 10;
        RenderingEngine::GetInstance()->GetWipeViewPort().Width += 10;
    }

	if (g_pad[0]->IsTrigger(enButtonX)) {
		Reset();
	}

	EnemyMove();
}

void Wipe::EnemyMove()
{
	if (m_timer > MOVE_TIME) {
		return;
	}

	//�v�������B
	m_timer += g_gameTime->GetFrameDeltaTime() * MOVE_SPEED;

	float timer = m_timer;

	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {

		timer = m_timer - (i * ENEMY_DURATION);

		Vector3 moveSpeed = m_enemy[i].moveSpeed[2];

		//�x�W�F�Ȑ��𗘗p���ēG���ړ�������
		m_enemy[i].moveSpeed[0].Lerp(timer, m_bezierPos[0], m_bezierPos[1]);
		m_enemy[i].moveSpeed[1].Lerp(timer, m_bezierPos[1], m_bezierPos[2]);
		m_enemy[i].moveSpeed[2].Lerp(timer, m_enemy[i].moveSpeed[0], m_enemy[i].moveSpeed[1]);

		moveSpeed -= m_enemy[i].moveSpeed[2];
		moveSpeed *= -1.0f;

		//��]������
		Quaternion rot;
		rot.SetRotationYFromDirectionXZ(moveSpeed);

		//�G���f���̐ݒ�
		m_enemy[i].modelRender.SetPosition(m_enemy[i].moveSpeed[2]);
		m_enemy[i].modelRender.SetRotation(rot);
		m_enemy[i].modelRender.Update();
	}
}

void Wipe::Render(RenderContext& rc)
{
	for (auto& wall : m_stage)
	{
		wall->WipeRender(rc);
	}

	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {
		m_enemy[i].modelRender.Draw(rc);
	}
}