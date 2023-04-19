#pragma once
#include "graphics/ModelRender.h"

namespace nsBookEngine {

	class SpotLight
	{
	public:
		/// <summary>
		/// スポットライト用の構造体
		/// </summary>
		struct spotLight
		{
			Vector3 spPosition = Vector3::Zero;		//座標
			float pad0 = 0.0f;				
			Vector3 spColor = Vector3::Zero;		//カラー
			float spRange = 0.0f;			//影響範囲
			Vector3 spDirection = Vector3::Zero;	//方向
			float spAngle = 0.0f;			//射出角度
		};

	public:
		SpotLight();
		~SpotLight();

		/// <summary>
		/// スポットライトを設定。
		/// </summary>
		/// <param name="num">番号</param>
		/// <param name="pos">座標</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		/// <param name="range">方向</param>
		/// <param name="range">射出角度</param>
		void SetSpotLight(const int num, const Vector3& pos, const Vector3& color, const float range, const Vector3& dir, const float angle)
		{
			SetNumber(num);
			SetPosition(pos);
			SetColor(color);
			SetRange(range);
			SetDirection(dir);
			SetAngle(angle);
			Update();
		}

		/// <summary>
		/// 番号を設定。
		/// </summary>
		/// <returns></returns>
		void SetNumber(const int num)
		{
			m_spNum = num;
		}

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

		/// <summary>
		/// 光があたっているかどうか
		/// </summary>
		const bool IsHit(const Vector3& playerPos);

	private:
		spotLight m_spotLig;
		int m_spNum = 0;
	};

}
