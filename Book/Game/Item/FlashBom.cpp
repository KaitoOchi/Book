#include "stdafx.h"
#include "FlashBom.h"
#include "Enemy.h"
#include "Game.h"
#include "PlayerManagement.h"
namespace
{
	const float FLASHRANGE = 300.0f;			//�t���b�V���̌��ʔ͈�
	const float MAXRANGE = 2000.0f;				//�|�C���g���C�g�͈̔�
	const float MAXALPHA = 0.9;					//���l�͈̔�
	const float MAXAMBIENT = 10.0f;				//���̋���
}
FlashBom::FlashBom()
{

}
FlashBom::~FlashBom()
{

}
bool FlashBom::Start()
{
	Item::Start();
	m_sphereCollider.Create(1.0f);
	//�|�C���g���C�g
	m_pointLight.SetColor(Vector3(0.0f,0.0f,0.0f));
	m_pointLight.SetRange(MAXRANGE);
	m_pointLight.Update();
	
	//�t���b�V��
	m_flashRender.Init("Assets/sprite/Flash/flash.DDS", 1920.0f, 1080.0f);
	return true;
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g�i�Ǘp�j
struct SweepResyltWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����ĂȂ�������
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
		{
			//�Փ˂����͕̂ǂł͂Ȃ�
			return 0.0f;
		}
		//�ǂƂԂ�������
		//�t���O��true��
		isHit = true;
		return 0.0f;
		g_pad[0]->IsTrigger(enButtonB);
	}
};


void FlashBom::Update()
{
	ManageState();
	if (m_FlashState != m_enFlash_No&&m_FlashState!=m_enFlash_Start)
	{
		FlashEffect();
	}
}
void FlashBom::ItemHit()
{
	//�|�C���g���C�g
	m_position.y = 200.0f;
	m_pointLight.SetPosition(m_position);
	m_pointLight.Update();

	//�߂��ɂ���G�l�~�[������T��
	for (int i = 0; m_game->GetEnemyList().size(); i++)
	{
		Vector3 diff = m_position - m_game->GetEnemyList()[i]->GetPosition();
        if (diff.Length() < pow(FLASHRANGE, 2.0f))
		{
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			Vector3 enemyPosition = m_game->GetEnemyList()[i]->GetPosition();
			Vector3 playerPosition = m_playerManagement->GetPosition();
			//�n�_�̓v���C���[�̍��W
			start.setOrigin(btVector3(playerPosition.x, playerPosition.y, playerPosition.z));
			//�I�_�̓G�l�~�[�̍��W
			end.setOrigin(btVector3(enemyPosition.x, enemyPosition.y, enemyPosition.z));
			SweepResyltWall callback;
			//�R���C�_�[���n�_����I�_�܂ł𓮂����āB
			//�Փ˂��邩�ǂ������ׂ�
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
            //�ǂƏՓ˂���
			if (callback.isHit == true)
			{
				//�v���C���[�͌������Ă��Ȃ�
				return;
			}
			//�ǂƏՓ˂��Ă��Ȃ�
			//���ɃG�l�~�[��������̖@�������Ă��邩���ׂ�
			//�A�C�e������G�l�~�[�Ɍ������x�N�g���𐳋K������
			diff.Normalize();
			//�G�l�~�[�̐��ʃx�N�g���ƃA�C�e������G�l�~�[�Ɍ������x�N�g���̓���(cos��)�����߂�
			float cos = m_game->GetEnemyList()[i]->GetFoward().Dot(diff);
			//���ς���p�x�����߂�
			float angle = acosf(cos);
			if (angle <= (Math::PI / 180.0f) * 90.0f)
			{
				//�G�l�~�[�t���O��true�ɁB
				m_game->GetEnemyList()[i]->SetHitFlashBullet(true);
			}
		}
	}
	
}

void FlashBom::FlashEffect()
{
	//�t���b�V�������������Ă���
	m_range -= (MAXRANGE / 100.0f);
	m_alpha-= (MAXALPHA / 100.0f);
	m_ambient -=(MAXAMBIENT / 100.0f+0.7);
	

	m_flashRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	
	RenderingEngine::GetInstance()->SetAmbient(m_ambient);
	
	m_pointLight.SetRange(m_range);
	m_pointLight.Update();
}

void FlashBom::SetFlashEffect()
{
	RenderingEngine::GetInstance()->SetAmbient(10.0f);
	//�|�C���g���C�g�̏�����
	m_pointLight.SetColor(Vector3(10.0f, 10.0f, 10.0f));
	m_pointLight.Update();
	//�t���b�V�����̒l������
	m_alpha = MAXALPHA;
	m_range = MAXRANGE;
	m_ambient = MAXAMBIENT;
}

void FlashBom::ProcessStartState()
{
	//�J�n���ɕK�v�ȕ����Ă�
	SetFlashEffect();
	ItemHit();
	m_FlashState = m_enFlash_Flashing;
}

void FlashBom::ProcessFlashingState()
{
	if (m_alpha <= 0.0f && m_range <= 0.0f)
	{
		m_pointLight.SetRange(0.0f);
		m_pointLight.SetColor(Vector3(0.0f, 0.0f, 0.0f));
		m_pointLight.Update();
		m_alpha = 0.0f;
		RenderingEngine::GetInstance()->SetAmbient(0.7f);
		m_FlashState = m_enFlash_No;
	}

}

void FlashBom::ProcessEndState()
{

}

void FlashBom::ManageState()
{
	switch (m_FlashState)
	{
	case FlashBom::m_enFlash_Start:
		ProcessStartState();
		break;
	case FlashBom::m_enFlash_Flashing:
		ProcessFlashingState();
		break;
	case FlashBom::m_enFlash_No:
		break;
	default:
		break;
	}
}

void FlashBom::Render(RenderContext& rc)
{
	m_flashRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f,m_alpha));
	m_flashRender.Draw(rc);
}