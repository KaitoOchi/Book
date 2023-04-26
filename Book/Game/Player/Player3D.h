#pragma once
#include "Player.h"
class PlayerManagement;
class FlashBom;
class SoundBom;
class Player3D :public Player
{
public:
	Player3D();
	~Player3D()override;
	bool Start()override;
	void Update()override;
	void Animation()override;
	void Throw()override;
	void PlayerChang();
	void Render(RenderContext& rc);
	/// <summary>
	/// �L�����R���̍쐬�̊֐�
	/// </summary>
	void CreatCharcon();

	void SetPushPosition(const Vector3 pushpos)
	{
		m_pushPosition = pushpos;
	}
	const Vector3 GetPushPosition()const
	{
		return m_pushPosition;
	}

private:
	PlayerManagement* m_playerManagement = nullptr;
	FlashBom* m_flashBom = nullptr;
	SoundBom* m_soundBom = nullptr;

	//�A�j���[�V�����C�x���g�p�̊֐�
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	bool m_isThrowing = false;

	//�������W�ɓ�����
	Vector3 m_pushPosition = Vector3::Zero;
	Vector3 m_pushRotPos = Vector3::Zero;

	float m_pushRot;
	float senkeiPos = 0.0f;
	float senkeiTime = 1.0f;
};

