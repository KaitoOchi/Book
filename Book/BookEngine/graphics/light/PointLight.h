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
			Vector3 ptPosition;		//座標
			float pad0 = 0.0f;
			Vector3 ptColor;		//カラー
			float ptRange = 0.0f;	//影響範囲
		};

		PointLight();
		~PointLight();

		/// <summary>
		/// 座標を設定。
		/// </summary>
		void SetPosition(const Vector3& pos)
		{
			m_pointLig.ptPosition = pos;
			//g_camera3D->GetViewMatrix().Apply(m_pointLig.ptPosition);
		}

		/// <sSummary>
		/// 座標を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition()
		{
			return m_pointLig.ptPosition;
		}

		/// <summary>
		/// カラーを設定。
		/// </summary>
		void SetColor(const Vector3& color)
		{
			m_pointLig.ptColor = color;
		}

		/// <summary>
		/// カラーを取得。
		/// </summary>
		/// <param name="range"></param>
		const Vector3& GetColor()
		{
			return m_pointLig.ptColor;
		}

		/// <summary>
		/// 影響範囲を設定。
		/// </summary>
		void SetRange(float range)
		{
			m_pointLig.ptRange = range;
		}

		/// <summary>
		/// 影響範囲を取得。
		/// </summary>
		const float& GetRange()
		{
			return m_pointLig.ptRange;
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