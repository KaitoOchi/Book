#pragma once

class GameManager
{
public:
	/// <summary>
	/// �Z�[�u�f�[�^�̍\����
	/// </summary>
	struct SaveData {
		float bgm = 0.1f;
		float sfx = 0.1f;
		int frameRate = 60;
	};

public:
	/// <summary>
	/// �Q�[����Ԃ̗񋓌^
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
	/// �C���X�^���X�̐����B
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new GameManager;
	}

	/// <summary>
	/// �C���X�^���X�̍폜�B
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
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
	void DataSave(const SaveData& data)
	{
		//�Z�[�u����
		FILE* fp = fopen("saveData.bin", "wb");
		fwrite((void*)&data, sizeof(data), 1, fp);
		fclose(fp);

		m_saveData = data;

		//�t���[�����[�g�̐ݒ�
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, m_saveData.frameRate);
	}

	/// <summary>
	/// �f�[�^�̃��[�h�B
	/// </summary>
	/// <returns></returns>
	SaveData& DataLoad()
	{
		FILE* fp = fopen("saveData.bin", "rb");
		if (fp != NULL) {
			fread((void*)&m_saveData, sizeof(m_saveData), 1, fp);
			fclose(fp);
		}

		//�t���[�����[�g�̐ݒ�
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, m_saveData.frameRate);

		return m_saveData;
	}

	/// <summary>
	/// BGM��炷�����B
	/// </summary>
	/// <param name="num">�炵�������̔ԍ�</param>
	void SetBGM(const int num)
	{
		m_bgm = NewGO<SoundSource>(0);
		m_bgm->Init(num);
		m_bgm->SetVolume(m_saveData.bgm);
		m_bgm->Play(true);
	}

	/// <summary>
	/// SFX�̉��ʂ��擾�B
	/// </summary>
	/// <returns></returns>
	const float& GetSFX()
	{
		return m_saveData.sfx;
	}

	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();

private:
	static GameManager*		m_instance;			//�C���X�^���X
	SaveData				m_saveData;			//�Z�[�u�f�[�^�̍\����
	EnGameState				m_gameState;		//�Q�[���X�e�[�g
	SoundSource*			m_bgm = nullptr;	//BGM

	bool					m_posState = true;	//�|�[�Y�����ǂ���
};

