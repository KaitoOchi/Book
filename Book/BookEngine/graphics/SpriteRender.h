#pragma once


namespace nsBookEngine {

	class SpriteRender
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="w">画像の横幅。</param>
		/// <param name="h">画像の縦幅。</param>
		/// <param name="alpha">デフォルトは半透明合成。</param>
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alpha = AlphaBlendMode_Trans);

		/// <summary>
		/// 座標を設定。zは0.0f固定。
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
		/// 回転を設定。
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// 回転を取得。
		/// </summary>
		/// <returns></returns>
		const Quaternion& GetRotation()
		{
			return m_rotation;
		}

		/// <summary>
		/// ピボットを設定。
		/// </summary>
		/// <param name="pivot"></param>
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
		/// 乗算カラーを設定。
		/// </summary>
		/// <param name="mulColor"></param>
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}

		/// <summary>
		/// 乗算カラーを取得。
		/// </summary>
		/// <returns></returns>
		const Vector4& GetMulColor()
		{
			return m_sprite.GetMulColor();
		}

		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update()
		{
			m_sprite.Update(m_position,
				m_rotation,
				m_scale,
				m_pivot);
		}

		/// <summary>
		/// 描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

	private:
		void OnRender2D(RenderContext& rc)
		{
			m_sprite.Draw(rc);
		}

	private:
		Sprite		m_sprite;								//スプライト
		Vector3		m_position = Vector3::Zero;				//座標
		Quaternion	m_rotation = Quaternion::Identity;		//回転
		Vector3		m_scale = Vector3::One;					//拡大率
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;		//ピボット
	};
}
