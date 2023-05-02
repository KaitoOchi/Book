#pragma once

class PlayerManagement;

class Wall :public IGameObject
{
public:
	Wall();
	~Wall();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetModel(int num);
	void CreateBoxObj();		// 当たり判定を作成

	//座標の取得
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	void SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}

	//大きさの取得
	const Vector3& GetScale()const
	{
		return m_scale;
	}
	void SetScale(const Vector3& m_scl)
	{
		m_scale = m_scl;
	}

	//回転の取得
	const Quaternion& GetRotation()const
	{
		return m_rotation;
	}
	void SetRotation(const Quaternion& m_rot)
	{
		m_rotation = m_rot;
	}
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation;


	/// <summary>
	/// 壁のモデルを設定する
	/// </summary>
	/// <param name="0">基本</param>
	/// <param name="1">絵画あり</param>
	/// <param name = "2">隙間あり</param>
	/// <param name = "3">スタート</param>
	/// <param name="4">ゴール</param>
	void SetWallModel(int num) {
		ModelState = num;
	}

protected:
	PhysicsBoxObject m_physicsBoxObj;
	PhysicsGhostObject m_physicsGhostObj;
	ModelRender m_wallRender;							//壁モデルの作成
	CollisionObject m_colliob;	

	PlayerManagement* m_player = nullptr;
	LevelRender m_levelRender;

	float posXmax = 0;
	float posXmin = 1000000.0;
	float posYmax = 0;
	float posYmin = 1000000.0;
	float posZmax = 0;
	float posZmin = 1000000.0;
	float posX;
	float posY;
	float posZ;

	Vector3 m_boxSize = Vector3::Zero;					//ボックスコライダーの作成

	int ModelState = 0;									// モデルを指定する
};

