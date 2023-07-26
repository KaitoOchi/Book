#pragma once

/// <summary>
/// ゲームマネージャークラス。
/// </summary>
class GameManager : Noncopyable
{
public:
	/// <summary>
	/// セーブデータの構造体
	/// </summary>
	struct SaveData {
		float bgm = 0.5f;
		float sfx = 0.5f;
		int frameRate = 60;
	};

	/// <summary>
	/// ゲーム状態の列挙型
	/// </summary>
	enum EnGameState
	{
		enState_Game,
		enState_GetTreasure,
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
		//既に生成されているなら。
		if (m_instance != nullptr) {
			std::abort();
		}

		m_instance = new GameManager;
	}

	/// <summary>
	/// インスタンスの削除。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
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
	void Save(const SaveData& data);

	/// <summary>
	/// データのロード。
	/// </summary>
	/// <returns></returns>
	SaveData& Load();

	/// <summary>
	/// BGMを鳴らす。
	/// </summary>
	/// <param name="num">鳴らしたい音の番号</param>
	void SetBGM(const int num);

	/// <summary>
	/// 音量を変更する。
	/// </summary>
	void SetVolume()
	{
		m_bgm->SetVolume(m_saveData.bgm * 0.5f);
	}

	/// <summary>
	/// BGMを削除する
	/// </summary>
	void DeleteBGM()
	{
		if (m_isDeleteBGM) {
			return;
		}
		m_isDeleteBGM = true;
		m_timer = m_saveData.bgm;
	}

	/// <summary>
	/// BGMが削除されたかどうか
	/// </summary>
	/// <returns></returns>
	const bool IsDeleteBGM() const
	{
		if (m_timer <= 0.0f) {
			return true;
		}
		return false;
	}

	/// <summary>
	/// SFXの音量を取得。
	/// </summary>
	/// <returns></returns>
	const float GetSFX() const
	{
		return m_saveData.sfx;
	}

	/// <summary>
	/// フレームレートを取得。
	/// </summary>
	/// <returns></returns>
	const int GetFrameRate() const
	{
		return m_saveData.frameRate;
	}

	/// <summary>
	/// 発見された数を加算。
	/// </summary>
	void AddSearchNum()
	{
		m_searchNum++;
	}

	/// <summary>
	/// 発見された数を取得。
	/// </summary>
	/// <returns></returns>
	const int GetSearchNum() const
	{
		return m_searchNum;
	}

	/// <summary>
	/// 発見された数を削除。
	/// </summary>
	void ResetSearchNum()
	{
		m_searchNum = 0;
	}

	/// <summary>
	/// ステートの切替
	/// </summary>
	/// <param name="gamestate"></param>
	void SetGameState(const EnGameState	gamestate)
	{
		m_gameState = gamestate;
	}

	/// <summary>
	/// ステートの取得。
	/// </summary>
	EnGameState GetGameState()const
	{
		return m_gameState;
	}

	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();

private:
	/// <summary>
	/// BGMの削除処理。
	/// </summary>
	void DeletingBGM();

private:
	static GameManager*		m_instance;						//インスタンス
	SoundSource*			m_bgm = nullptr;				//BGM
	SaveData				m_saveData;						//セーブデータの構造体
	EnGameState				m_gameState = enState_Game;		//ゲームステート
	bool					m_isDeleteBGM = false;			//BGMの削除中かどうか
	int						m_searchNum = 0;				//敵に見つかった回数
	float					m_timer = 0.0f;					//タイマー
};

