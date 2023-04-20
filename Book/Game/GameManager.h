#pragma once

class GameManager
{
public:
	/// <summary>
	/// セーブデータの構造体
	/// </summary>
	struct SaveData {
		float bgm = 0.1f;
		float sfx = 0.1f;
		int frameRate = 60;
	};

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
	/// データのセーブ。
	/// </summary>
	/// <param name="data"></param>
	void DataSave(const SaveData& data)
	{
		//セーブする
		FILE* fp = fopen("saveData.bin", "wb");
		fwrite((void*)&data, sizeof(data), 1, fp);
		fclose(fp);

		m_saveData = data;

		//フレームレートの設定
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, m_saveData.frameRate);
	}

	/// <summary>
	/// データのロード。
	/// </summary>
	/// <returns></returns>
	SaveData& DataLoad()
	{
		FILE* fp = fopen("saveData.bin", "rb");
		if (fp != NULL) {
			fread((void*)&m_saveData, sizeof(m_saveData), 1, fp);
			fclose(fp);
		}

		//フレームレートの設定
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, m_saveData.frameRate);

		return m_saveData;
	}

	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();

	bool m_posState = true;

private:
	static GameManager* m_instance;

	SaveData m_saveData;
};

