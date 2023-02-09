#pragma once
#include "../../k2EngineLow/level3D/TklFile.h"


namespace nsBookEngine {
	class MapChipRender;


	struct LevelObjectData {
		Vector3			position;		//座標。
		Quaternion		rotation;		//回転。
		Vector3			scale;			//拡大率。
		const wchar_t* name;			//名前。
		int				number = 0;		//番号。

		/// <summary>
		/// オブジェクト名を検索する(完全一致)
		/// </summary>
		/// <param name="objName">オブジェクト名。</param>
		/// <returns>完全一致ならtrue</returns>
		bool EqualObjectName(const wchar_t*& objName)
		{
			return wcscmp(objName, name) == 0;
		}

		bool ForwardMatchName(const wchar_t*& objName)
		{
			auto len = wcslen(objName);
			auto namelen = wcslen(name);

			if (len > namelen) {
				//名前が長いので不一致
				return false;
			}
			return wcsncmp(objName, name, len) == 0;
		}
	};

	/// <summary>
	/// レベル
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
		/// 更新処理。
		/// </summary>
		void Update();

		/// <summary>
		/// モデルの描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

	private:
		/// <summary>
		/// マップチップを作成。
		/// </summary>
		/// <param name="objData"></param>
		/// <param name="filePath"></param>
		void CreateMapChipRender(const LevelObjectData& objData, const char* filePath);

		/// <summary>
		/// Tklファイルの行列を変換する。
		/// </summary>
		void MatrixTklToLevel();

	private:
		using BonePtr = std::unique_ptr<Bone>;					//ボーンのポインタ
		std::vector<BonePtr>	m_boneList;								//ボーンのリスト
		std::map<std::string, MapChipRenderPtr> m_mapChipRenderPtrs;	//マップチップの可変長配列
		TklFile					m_tklFile;								//tklファイル


	};

}
