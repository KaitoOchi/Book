#pragma once
#include "../../k2EngineLow/level3D/TklFile.h"


namespace nsBookEngine {
	class MapChipRender;


	struct LevelObjectData {
		Vector3			position;		//���W�B
		Quaternion		rotation;		//��]�B
		Vector3			scale;			//�g�嗦�B
		const wchar_t* name;			//���O�B
		int				number = 0;		//�ԍ��B

		/// <summary>
		/// �I�u�W�F�N�g������������(���S��v)
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g���B</param>
		/// <returns>���S��v�Ȃ�true</returns>
		bool EqualObjectName(const wchar_t*& objName)
		{
			return wcscmp(objName, name) == 0;
		}

		bool ForwardMatchName(const wchar_t*& objName)
		{
			auto len = wcslen(objName);
			auto namelen = wcslen(name);

			if (len > namelen) {
				//���O�������̂ŕs��v
				return false;
			}
			return wcsncmp(objName, name, len) == 0;
		}
	};

	/// <summary>
	/// ���x��
	/// </summary>
	class LevelRender
	{
	private:
		using MapChipRenderPtr = std::shared_ptr<MapChipRender>;

	public:
		LevelRender();
		~LevelRender();

		void Init(const char* filePath, std::function<bool(LevelObjectData& objData)> hookFunc);

		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();

		/// <summary>
		/// ���f���̕`�揈���B
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

	private:
		/// <summary>
		/// �}�b�v�`�b�v���쐬�B
		/// </summary>
		/// <param name="objData"></param>
		/// <param name="filePath"></param>
		void CreateMapChipRender(const LevelObjectData& objData, const char* filePath);

		/// <summary>
		/// Tkl�t�@�C���̍s���ϊ�����B
		/// </summary>
		void MatrixTklToLevel();

	private:
		using BonePtr = std::unique_ptr<Bone>;					//�{�[���̃|�C���^
		std::vector<BonePtr>	m_boneList;								//�{�[���̃��X�g
		std::map<std::string, MapChipRenderPtr> m_mapChipRenderPtrs;	//�}�b�v�`�b�v�̉ϒ��z��
		TklFile					m_tklFile;								//tkl�t�@�C��


	};

}
