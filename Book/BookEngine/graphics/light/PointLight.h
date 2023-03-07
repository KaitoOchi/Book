#pragma once

namespace nsBookEngine {

	/// <summary>
	/// ポイントライト。
	/// </summary>
	class PointLight
	{
	public:
		/// <summary>
		/// ポイントライト用の構造体
		/// </summary>
		struct pointLight
		{
			Vector3 ptPosition = Vector3::Zero;		//座標
			float pointNum = 0.0f;					//ポイントライトの数
			Vector3 ptColor = Vector3::Zero;		//カラー
			float ptRange = 0.0f;					//影響範囲
		};

		PointLight();
		~PointLight();

		/// <summary>
		/// 初期化処理。
		/// </summary>
		void Init();

		/// <summary>
		/// 座標を設定。
		/// </summary>
		void SetPosition(const Vector3& pos)
		{
			m_pointLig.ptPosition = pos;
		}

		/// <summary>
		/// カラーを設定。
		/// </summary>
		void SetColor(const Vector3& color)
		{
			m_pointLig.ptColor = color;
		}

		/// <summary>
		/// 影響範囲を設定。
		/// </summary>
		void SetRange(float range)
		{
			m_pointLig.ptRange = range;
		}

		/// <summary>
		/// 構造体を取得
		/// </summary>
		pointLight& GetPointLig()
		{
			return m_pointLig;
		}

		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();

	private:
		pointLight m_pointLig;
	};
}