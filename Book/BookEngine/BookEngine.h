#pragma once


namespace nsBookEngine {

	class BookEngine
	{
	private:
		BookEngine() {};
		~BookEngine();

	public:
		/// <summary>
		/// �������f�[�^�B
		/// </summary>
		struct InitData {
			HWND hwnd;					//�E�B���h�E�n���h���B
			UINT frameBufferWidth;		//�t���[���o�b�t�@�̕��B
			UINT frameBufferHeight;		//�t���[���o�b�t�@�̍����B
			bool isSoftShadow;			//�\�t�g�V���h�E���s�����ǂ����B
		};

		/// <summary>
		/// �C���X�^���X�̍쐬�B
		/// </summary>
		/// <param name="initData"></param>
		static void CreateInstance(const InitData& initData)
		{
			m_instance = new BookEngine;
			m_instance->Init(initData);
		}

		/// <summary>
		/// �C���X�^���X�̔j���B
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
		}

		/// <summary>
		/// �C���X�^���X�̎擾�B
		/// </summary>
		/// <returns></returns>
		static BookEngine* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// bookEngine���������B
		/// </summary>
		void Init(const InitData& initData);

		/// <summary>
		/// �G���W���̏��������s�B
		/// </summary>
		void Execute();

	private:
		K2EngineLow m_k2EngineLow;
		//CollisionObjectManager m_collisionObjectManager;
		//RenderingEngine m_renderingEngine;		//�����_�����O�G���W��

		static BookEngine* m_instance;			//�C���X�^���X
	};

	//�O���[�o���A�N�Z�X�|�C���g
	extern BookEngine* g_bookEngine;
	//extern RenderingEngine* g_renderingEngine;
	//extern CollisionObjectManager* g_collisionObjectManager;
}

