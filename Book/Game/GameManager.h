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
	/// <summary>
	/// ゲーム状態の列挙型
	/// </summary>
	enum EnGameState
	{
		enState_Title,
		enState_Game,
		enState_Result
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
	/// BGMを鳴らす処理。
	/// </summary>
	/// <param name="num">鳴らしたい音の番号</param>
	void SetBGM(const int num)
	{
		m_bgm = NewGO<SoundSource>(0);
		m_bgm->Init(num);
		m_bgm->SetVolume(m_saveData.bgm);
		m_bgm->Play(true);
	}

	/// <summary>
	/// SFXの音量を取得。
	/// </summary>
	/// <returns></returns>
	const float& GetSFX()
	{
		return m_saveData.sfx;
	}

	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();

private:
	static GameManager*		m_instance;			//インスタンス
	SaveData				m_saveData;			//セーブデータの構造体
	EnGameState				m_gameState;		//ゲームステート
	SoundSource*			m_bgm = nullptr;	//BGM

	bool					m_posState = true;	//ポーズ中かどうか
};

