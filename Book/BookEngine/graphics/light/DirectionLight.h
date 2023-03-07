#pragma once

namespace nsBookEngine {

	class DirectionLight
	{
	public:
		//ディレクションライト用の構造体
		struct directionLight
		{
			Vector3 ligDirection = Vector3::Zero;	//ライトの方向
			float pad = 0.0f;
			Vector3 ligColor = Vector3::Zero;		//ライトの色
			float pad2 = 0.0f;
			Vector3 eyePos = Vector3::Zero;			//視点の位置
			float pad3 = 0.0f;
			Vector3 ambient = Vector3::Zero;
		};

		DirectionLight();
		~DirectionLight();

		/// <summary>
		/// ライトの方向を設定
		/// </summary>
		void SetDirection(const Vector3 dir)
		{
			m_directionLig.ligDirection = dir;
			//ライトを正規化
			m_directionLig.ligDirection.Normalize();
		}

		/// <summary>
		/// ライトの色を設定
		/// </summary>
		void SetColor(const Vector3 dir)
		{
			m_directionLig.ligColor = dir;
		}

		/// <summary>
		/// 環境光を設定
		/// </summary>
		void SetAmbientLight(const float amb)
		{
			m_directionLig.ambient.x = amb;
			m_directionLig.ambient.y = amb;
			m_directionLig.ambient.z = amb;
		}

		/// <summary>
		/// 構造体を取得
		/// </summary>
		directionLight& GetDirectionLig()
		{
			return m_directionLig;
		}

	private:
		directionLight m_directionLig;

	};
}