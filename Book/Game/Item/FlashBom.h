#pragma once
#include "Item.h"
class Game;

class FlashBom:public Item
{
public:
	FlashBom();
	~FlashBom()override;
	bool Start()override;
	void Update();
	void ItemHit()override;
	void Render(RenderContext& rc);
	/// <summary>
	/// ���l�Ɖe������ݒ肷��
	/// </summary>
	void SetFlashEffect();

	enum EnFlashState
	{
		m_enFlash_No,				//�t���b�V�����Ă��Ȃ�
		m_enFlash_Start,			//�t���b�V���X�^�[�g
		m_enFlash_Flashing,			//�t���b�V����
		m_enFlash_End				//�t���b�V���G���h
	};

	EnFlashState m_FlashState = m_enFlash_No;
	int m_flashCount = 5;						//�t���b�V���{���̏�����
private:
	/// <summary>
	/// �t���b�V���̌���
	/// </summary>
	void FlashEffect();
	/// <summary>
	/// �X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState()override;
	/// <summary>
	/// �J�n���̑J��
	/// </summary>
	void ProcessStartState();

	/// <summary>
	/// �t���b�V�����̑J��
	/// </summary>
	void ProcessFlashingState();

	


	SpriteRender m_flashRender;

	SphereCollider m_sphereCollider;					//�X�t�B�A�R���C�_�[
	RigidBody m_rigidBody;								//����

	PointLight m_pointLight;							//�|�C���g���C�g�̍��W
	float m_range=0;
	float m_alpha=0;
	float m_ambient=0;
	float m_color = 0;

};

