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
	void SetModel_withPainting(int num);	// 絵画あり
	void SetModel_withGap(int num);			// 隙間あり

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

private:
	PhysicsBoxObject m_physicsBoxObj;
	PhysicsGhostObject m_physicsGhostObj;
	ModelRender m_wallRender;					//壁モデルの作成
	CollisionObject m_colliob;	

	PlayerManagement* m_player = nullptr;

	Vector3 m_boxSize = Vector3::Zero;//ボックスコライダーの作成

	/// <summary>
	/// 壁の種類を設定する。
	/// </summary>
	enum enWallModel {
		NORMAL,				// 基本の壁
		WITH_PANINTING,		// 絵画あり
		WITH_GAP,			// 隙間あり
		START,				// スタート
		GOAL				// ゴール
	};
	enWallModel m_enWallModel = NORMAL;

	int ModelState = 0;						// モデルを指定する
};

