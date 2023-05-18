#pragma once
class Game;
class Enemy_Increase :public IGameObject
{
public:
	Enemy_Increase();
	~Enemy_Increase();
	bool Start();
	void Update();
	void NewEnemy(int number);
public:
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ポジションの取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="scl">大きさ</param>
	void SetScale(Vector3 scl)
	{
		m_scale = scl;
	}
	/// <summary>
	/// 大きさの取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()
	{
		return m_scale;
	}
private:


private:
	Vector3 m_position = Vector3::Zero;				//座標
	Vector3 m_scale = Vector3::Zero;				//大きさ
	Quaternion m_rotation;							//回転

	Game* m_game = nullptr;
};

