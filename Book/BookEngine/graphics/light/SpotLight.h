#pragma once

namespace nsBookEngine {

	class SpotLight
	{
	public:
		/// <summary>
		/// スポットライト用の構造体
		/// </summary>
		struct spotLight
		{
			Vector3 spPosition;		//座標
			float pad0;				
			Vector3 spColor;		//カラー
			float spRange;			//影響範囲
			Vector3 spDirection;	//方向
			float spAngle;			//射出角度
		};

	public:
		SpotLight();
		~SpotLight();

		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_spotLig.spPosition = pos;
		}

		/// <summary>
		/// 座標を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPositoin()
		{
			return m_spotLig.spPosition;
		}

		/// <summary>
		/// カラーを設定。
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_spotLig.spColor = color;
		}

		/// <summary>
		/// カラーを取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetColor()
		{
			return m_spotLig.spColor;
		}

		/// <summary>
		/// 影響範囲を設定。
		/// </summary>
		/// <param name="range"></param>
		void SetRange(const float range)
		{
			m_spotLig.spRange = range;
		}

		/// <summary>
		/// 影響範囲を取得。
		/// </summary>
		/// <returns></returns>
		const float GetRange()
		{
			return m_spotLig.spRange;
		}

		/// <summary>
		/// 方向を設定。
		/// </summary>
		/// <param name="dir"></param>
		void SetDirection(const Vector3& dir)
		{
			m_spotLig.spDirection = dir;
			m_spotLig.spDirection.Normalize();
		}

		/// <summary>
		/// 方向を取得。
		/// </summary>
		/// <returns></returns>
		Vector3& GetDirection()
		{
			return m_spotLig.spDirection;
		}

		/// <summary>
		/// 射出角度を設定。
		/// </summary>
		/// <param name="angle"></param>
		void SetAngle(const float angle)
		{
			m_spotLig.spAngle = Math::DegToRad(angle);
		}

		/// <summary>
		/// 射出角度を取得。
		/// </summary>
		/// <returns></returns>
		const float GetAngle()
		{
			return m_spotLig.spAngle;
		}

		/// <summary>
		/// 構造体を取得。
		/// </summary>
		/// <returns></returns>
		spotLight& GetSpotLight()
		{
			return m_spotLig;
		}

		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();

	private:
		spotLight m_spotLig;
	};

}
