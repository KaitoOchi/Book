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

		return m_saveData;
	}


	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();

	bool m_posState = true;

private:
	static GameManager* m_instance;

	SaveData m_saveData;
};

