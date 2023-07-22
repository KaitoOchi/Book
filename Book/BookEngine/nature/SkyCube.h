#pragma once

namespace nsBookEngine {

	/// <summary>
	/// スカイキューブクラス。
	/// </summary>
	class SkyCube : public IGameObject
	{
	public:
		SkyCube();
		~SkyCube();
		bool Start();
		void Render(RenderContext& rc);

	public:
		/// <summary>
		/// 種類を設定。
		/// </summary>
		void SetType(const int type)
		{
			m_number = type;
		}

		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// 座標を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// 拡大率を設定。
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// 拡大率を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetScale() const 
		{
			return m_scale;
		}

		/// <summary>
		/// 明るさを設定。
		/// </summary>
		/// <param name="lum"></param>
		void SetLuminance(const float lum)
		{
			m_luminance = lum;
		}

	private:
		Texture			m_texture;								//テクスチャ
		ModelRender		m_modelRender;							//天球モデル
		Vector3			m_position;								//座標　
		Vector3			m_scale = Vector3::One  * 1000.0f;		//拡大率
		int				m_number = 0;							//種類
		float			m_luminance = 0.75f;					//輝度
	};
}
