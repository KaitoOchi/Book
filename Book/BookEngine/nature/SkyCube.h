#pragma once

namespace nsBookEngine {

	class SkyCube : public IGameObject
	{
	public:
		SkyCube();
		~SkyCube();
		bool Start();
		void Render(RenderContext& rc);

	public:
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
		const Vector3& GetPosition()
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
		const Vector3& GetScale()
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
		Texture			m_texture;
		ModelRender		m_modelRender;
		Vector3			m_position;
		Vector3			m_scale = Vector3::One  * 1000.0f;
		float			m_luminance = 0.75f;
	};
}
