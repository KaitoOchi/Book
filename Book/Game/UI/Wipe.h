#pragma once

class Wall;
class BackGround;

namespace
{
	const int ENEMY_NUM_WIPE = 4;				//�G�̐�
}

class Wipe : public IGameObject
{
public:
	Wipe();
	~Wipe();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// ���x�������_�[�����B
	/// </summary>
	void LevelDesign();

	/// <summary>
	/// �G�̈ړ������B
	/// </summary>
	void EnemyMove();

	/// <summary>
	/// �G�̒n�_�����Z�b�g
	/// </summary>
	void Reset()
	{
		for (int i = 0; i < 3; i++) {
			m_enemy[i].moveSpeed[0] = m_bezierPos[0];
			m_enemy[i].moveSpeed[1] = m_bezierPos[0];
			m_enemy[i].moveSpeed[2] = m_bezierPos[1];
		}

		m_timer = 0.0f;
	}


private:
	struct EnemyStruct
	{
		ModelRender modelRender;	//���f��
		Vector3		moveSpeed[3];	//�ړ����x
	};

private:
	LevelRender			m_levelRender;
	AnimationClip*		m_enemyAnim = nullptr;
	BackGround*			m_backGround = nullptr;
	std::vector<Wall*>	m_stage;
	EnemyStruct			m_enemy[ENEMY_NUM_WIPE];		//�G�̍\����
	Vector3				m_bezierPos[3];	//�G�̈ړ����W
	float				m_timer = 0.0f;
};