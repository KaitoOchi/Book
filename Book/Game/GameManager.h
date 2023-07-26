#pragma once

/// <summary>
/// �Q�[���}�l�[�W���[�N���X�B
/// </summary>
class GameManager : Noncopyable
{
public:
	/// <summary>
	/// �Z�[�u�f�[�^�̍\����
	/// </summary>
	struct SaveData {
		float bgm = 0.5f;
		float sfx = 0.5f;
		int frameRate = 60;
	};

	/// <summary>
	/// �Q�[����Ԃ̗񋓌^
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
	/// �C���X�^���X�̐����B
	/// </summary>
	static void CreateInstance()
	{
		//���ɐ�������Ă���Ȃ�B
		if (m_instance != nullptr) {
			std::abort();
		}

		m_instance = new GameManager;
	}

	/// <summary>
	/// �C���X�^���X�̍폜�B
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// �C���X�^���X�̎擾�B
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance()
	{
		return m_instance;
	}


	/// <summary>
	/// �f�[�^�̃Z�[�u�B
	/// </summary>
	/// <param name="data"></param>
	void Save(const SaveData& data);

	/// <summary>
	/// �f�[�^�̃��[�h�B
	/// </summary>
	/// <returns></returns>
	SaveData& Load();

	/// <summary>
	/// BGM��炷�B
	/// </summary>
	/// <param name="num">�炵�������̔ԍ�</param>
	void SetBGM(const int num);

	/// <summary>
	/// ���ʂ�ύX����B
	/// </summary>
	void SetVolume()
	{
		m_bgm->SetVolume(m_saveData.bgm * 0.5f);
	}

	/// <summary>
	/// BGM���폜����
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
	/// BGM���폜���ꂽ���ǂ���
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
	/// SFX�̉��ʂ��擾�B
	/// </summary>
	/// <returns></returns>
	const float GetSFX() const
	{
		return m_saveData.sfx;
	}

	/// <summary>
	/// �t���[�����[�g���擾�B
	/// </summary>
	/// <returns></returns>
	const int GetFrameRate() const
	{
		return m_saveData.frameRate;
	}

	/// <summary>
	/// �������ꂽ�������Z�B
	/// </summary>
	void AddSearchNum()
	{
		m_searchNum++;
	}

	/// <summary>
	/// �������ꂽ�����擾�B
	/// </summary>
	/// <returns></returns>
	const int GetSearchNum() const
	{
		return m_searchNum;
	}

	/// <summary>
	/// �������ꂽ�����폜�B
	/// </summary>
	void ResetSearchNum()
	{
		m_searchNum = 0;
	}

	/// <summary>
	/// �X�e�[�g�̐ؑ�
	/// </summary>
	/// <param name="gamestate"></param>
	void SetGameState(const EnGameState	gamestate)
	{
		m_gameState = gamestate;
	}

	/// <summary>
	/// �X�e�[�g�̎擾�B
	/// </summary>
	EnGameState GetGameState()const
	{
		return m_gameState;
	}

	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();

private:
	/// <summary>
	/// BGM�̍폜�����B
	/// </summary>
	void DeletingBGM();

private:
	static GameManager*		m_instance;						//�C���X�^���X
	SoundSource*			m_bgm = nullptr;				//BGM
	SaveData				m_saveData;						//�Z�[�u�f�[�^�̍\����
	EnGameState				m_gameState = enState_Game;		//�Q�[���X�e�[�g
	bool					m_isDeleteBGM = false;			//BGM�̍폜�����ǂ���
	int						m_searchNum = 0;				//�G�Ɍ���������
	float					m_timer = 0.0f;					//�^�C�}�[
};

