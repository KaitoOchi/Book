#include "stdafx.h"
#include "PhysicsGhost.h"
#include "PlayerManagement.h"
#include "UI/TutorialUI.h"

namespace
{
	const float EFFECT_SIZE = 1.5f;				//�G�t�F�N�g�T�C�Y
	const float DISTANCE = pow(1200.0f, 2.0f);	//�G�t�F�N�g�������鋗��
	const float LENGTH = 500.0f;				//���肷�鋗��
}

PhysicsGhost::PhysicsGhost()
{

}

PhysicsGhost::~PhysicsGhost()
{
	m_kirakiraEffect->SetDeleteState(false);
	m_kirakiraEffect->Stop();
	DeleteGO(m_kirakiraEffect);
}

bool PhysicsGhost::Start()
{
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_turorialUI = FindGO<TutorialUI>("turorialUI");

	Ghost::Start();
	Vector3 boxSize = CreateGhostBox();

	//�����蔻����쐬
	m_physicsGhostObj.CreateBox(
		m_position,
		m_rotation,
		boxSize
	);
	m_physicsGhostObj.GetbtCollisionObject().setUserIndex(enCollisionAttr_Ground);

	if (m_isDraw == true) {
		//�G�t�F�N�g�̓ǂݍ���
		m_kirakiraEffect = NewGO<EffectEmitter>(5);
		m_kirakiraEffect->SetDeleteState(true);
		m_kirakiraEffect->Init(5);
		m_kirakiraEffect->SetPosition(Vector3{ m_position.x,m_position.y + 50.0f,m_position.z });
		m_kirakiraEffect->SetScale(Vector3::One * EFFECT_SIZE);
		m_kirakiraEffect->Play();
		m_kirakiraEffect->Update();
	}
	return true;
}
void PhysicsGhost::Update()
{
	IsHitPlayer();

	if (m_kirakiraEffect == nullptr) {
		return;
	}

	if (m_isDraw == true) {
		EffectDraw();
	}
}

void PhysicsGhost::EffectDraw()
{
	Vector3 diff = m_position - m_playerManagement->GetPosition();

	if (diff.LengthSq() < DISTANCE) {

		if (!m_kirakiraEffect->IsPlay()) {
			m_kirakiraEffect->Play();
		}
	}
	else {
		m_kirakiraEffect->Stop();
	}
	
}

bool PhysicsGhost::IsHitPlayer()
{
	// �v���C���[���玩�g�֌������x�N�g��
	Vector3 diff = m_position - m_playerManagement->GetPosition();

	// ���ȏ�̂Ƃ��͎��s���Ȃ�
	if (diff.LengthSq() > LENGTH) {
		return false;
	}

	// �Փ˔����Ԃ�
	if (IsHit(m_playerManagement->GetPosition()) == true) {
		// �`���[�g���A���̃e�L�X�g�摜��`�悵�Ă���Ƃ�
		if (m_turorialUI!=nullptr && m_turorialUI->GetTutorialDrawState() == true) {
			// �`�悵�Ȃ��悤�ɂ���
			m_turorialUI->SetTutorialDrawState(false);
		}

		return true;
	}

	return false;
}