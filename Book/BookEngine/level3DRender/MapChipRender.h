#pragma once


namespace nsBookEngine {
	struct LevelObjectData;

	struct MapChipData {
		Vector3		position;	//座標。
		Vector3		scale;		//拡大率。
		Quaternion	rotation;	//回転。
	};

	/// <summary>
	/// マップチップレンダー。
	/// </summary>
	class MapChipRender
	{
	public:
		MapChipRender(const LevelObjectData& objData, const char* filePath);
		~MapChipRender() {};

		/// <summary>
		/// マップチップデータを追加。
		/// </summary>
		/// <param name="objData"></param>
		void AddMapChipData(const LevelObjectData& objData);

		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();

		/// <summary>
		/// モデルを更新。
		/// </summary>
		void Update();

		/// <summary>
		/// モデルを描画。
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// モデルレンダーを取得。
		/// </summary>
		/// <returns></returns>
		ModelRender& GetSkinModelRener()
		{
			return m_modelRender;
		}

	private:
		ModelRender											m_modelRender;					//モデルレンダー
		std::vector<MapChipData>							m_mapChipDataVector;			//マップチップデータのリスト
		std::unique_ptr<const char*>						m_filePath;						//tkmファイルパス
		std::vector<std::unique_ptr<PhysicsStaticObject>>	m_physicsStaticObjectPtrVector;	//PhysicsStaticObjectのスマートポインタの配列
		int													m_mapChipDataNum;				//マップチップデータの数
	};
}

