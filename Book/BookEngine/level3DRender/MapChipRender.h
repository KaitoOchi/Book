#pragma once


namespace nsBookEngine {
	struct LevelObjectData;

	struct MapChipData {
		Vector3		position;	//���W�B
		Vector3		scale;		//�g�嗦�B
		Quaternion	rotation;	//��]�B
	};

	/// <summary>
	/// �}�b�v�`�b�v�����_�[�B
	/// </summary>
	class MapChipRender
	{
	public:
		MapChipRender(const LevelObjectData& objData, const char* filePath);
		~MapChipRender() {};

		/// <summary>
		/// �}�b�v�`�b�v�f�[�^��ǉ��B
		/// </summary>
		/// <param name="objData"></param>
		void AddMapChipData(const LevelObjectData& objData);

		/// <summary>
		/// �������B
		/// </summary>
		void Init();

		/// <summary>
		/// ���f�����X�V�B
		/// </summary>
		void Update();

		/// <summary>
		/// ���f����`��B
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// ���f�������_�[���擾�B
		/// </summary>
		/// <returns></returns>
		ModelRender& GetSkinModelRener()
		{
			return m_modelRender;
		}

	private:
		ModelRender											m_modelRender;					//���f�������_�[
		std::vector<MapChipData>							m_mapChipDataVector;			//�}�b�v�`�b�v�f�[�^�̃��X�g
		std::unique_ptr<const char*>						m_filePath;						//tkm�t�@�C���p�X
		std::vector<std::unique_ptr<PhysicsStaticObject>>	m_physicsStaticObjectPtrVector;	//PhysicsStaticObject�̃X�}�[�g�|�C���^�̔z��
		int													m_mapChipDataNum;				//�}�b�v�`�b�v�f�[�^�̐�
	};
}

