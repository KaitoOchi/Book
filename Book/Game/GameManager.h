#pragma once

class GameManager
{
public:
	GameManager();
	~GameManager();

	/// <summary>
	/// インスタンスの生成。
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new GameManager;
	}

	/// <summary>
	/// インスタンスの削除。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}

	/// <summary>
	/// インスタンスの取得。
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance()
	{
		return m_instance;
	}


	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();

private:
	static GameManager* m_instance;
};

