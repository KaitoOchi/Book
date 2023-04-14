#pragma once
#include "Player.h"
class PlayerManagement;
class Player3D :public Player
{
public:
	Player3D();
	~Player3D()override;
	bool Start()override;
	void Update()override;
	void Animation()override;
	void Throw();
	void PlayerChang();
	void Render(RenderContext& rc);
	/// <summary>
	/// �L�����R���̍쐬�̊֐�
	/// </summary>
	void CreatCharcon();
private:
	ModelRender m_model3DRender;//3D���f��
	PlayerManagement* m_playerManagement = nullptr;

	//�������W�ɓ�����
	Vector3 m_movePosition = Vector3::Zero;
	float senkeiPos = 0.1f;
	float senkeiTime = 1.0f;
};

