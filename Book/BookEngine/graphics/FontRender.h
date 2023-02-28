#pragma once


namespace nsBookEngine {

	class FontRender
	{
	public:
		static const int MAX_TEXT_SIZE = 256;

		~FontRender()
		{

		}

		/// <summary>
		/// 表示する文字を設定。
		/// </summary>
		/// <param name="text"></param>
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}

		/// <summary>
		/// 表示する文字を取得。
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetText()
		{
			return m_text;
		}

		/// <summary>
		/// 座標を設定。zは0.0f。
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
		/// 大きさを設定。
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const float scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// 大きさを取得。
		/// </summary>
		/// <returns></returns>
		const float GetScale()
		{
			return m_scale;
		}

		/// <summary>
		/// 色を設定。
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}

		/// <summary>
		/// 色を取得。
		/// </summary>
		/// <returns></returns>
		const Vector4& GetColor()
		{
			return m_color;
		}

		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const float rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// 回転を取得。
		/// </summary>
		/// <returns></returns>
		const float GetRotation()
		{
			return m_rotation;
		}

		/// <summary>
		/// ピボットを設定。
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}

		/// <summary>
		/// ピボットを取得。
		/// </summary>
		/// <returns></returns>
		const Vector2& GetPivot()
		{
			return m_pivot;
		}

		/// <summary>
		/// 描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// 影のパラメータを設定。
		/// </summary>
		/// <param name="isDrawShadow">影を描画するかどうか。</param>
		/// <param name="shadowOffset">ピクセルのオフセット量。</param>
		/// <param name="shadowColor">影の色。</param>
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}

	private:
		/// <summary>
		/// 2D描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		void OnRender2D(RenderContext& rc)
		{
			m_font.Begin(rc);
			m_font.Draw(m_text, Vector2(m_position.x, m_position.y), m_color, m_rotation, m_scale, m_pivot);
			m_font.End(rc);
		}

	private:
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;	//ピボット
		Vector3		m_position = Vector3::Zero;			//座標
		Vector4		m_color = g_vec4White;				//色
		Font		m_font;								//フォント
		wchar_t		m_text[MAX_TEXT_SIZE];				//文字
		float		m_scale = 1.0f;						//大きさ
		float		m_rotation = 0.0f;					//回転

	};
}

