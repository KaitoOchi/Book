#pragma once
class Player3D;
class Player2D;
class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
private:
	Player3D* m_player3D = nullptr;//�v���C���[
	Player2D* m_player2D = nullptr;//�v���C���[
	Vector3 m_toCameraPos = Vector3::Zero;//�����_���王�_�Ɍ������x�N�g��
	Vector3 m_toCameraPosOld = Vector3::Zero;//�����_���王�_�Ɍ������x�N�g���ύX�O���W
	Vector3 target=Vector3::Zero;//�����_
	Quaternion qRot;//Y������̉�]
	Vector3 axisX;//X������̉�]


};

