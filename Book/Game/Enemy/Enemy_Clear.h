#pragma once
#include "Enemy.h"
class Enemy_Clear:public Enemy 
{
public:
	Enemy_Clear();
	~Enemy_Clear()override;
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);

	void Update_OnCraw();				// „‰ñ
	void Update_OnTracking();			// ’ÇÕ
	void Update_OnCalled();				// CALL‚ÉÀs
	void Update_OnBackBasedOn();		// „‰ñó‘Ô‚É–ß‚é
	void Update_OnConfusion();			// ‘MŒõ’e‚É“–‚½‚Á‚½‚Æ‚«
	void UpDate_OnListen();				// ‰¹”š’e‚ğg—p‚µ‚½‚Æ‚«
	void Update_OnCatch();				// •ßŠl

	/// <summary>
	/// “§–¾‰»‚·‚é‚©İ’è‚Å‚«‚é
	/// </summary>
	/// <param name="cle">true‚È‚ç“§–¾‰»false‚È‚ç‰ğœ</param>
	/// <returns></returns>
	const void SetClearFlag(const bool cle)
	{
		m_clearFlag = cle;
	}
	/// <summary>
	/// “§–¾‰»‚µ‚Ä‚¢‚é‚©î•ñ‚ğ“¾‚ç‚ê‚é
	/// </summary>
	/// <returns></returns>
	const bool GetClearFlag()const
	{
		return m_clearFlag;
	}
private:
	/// <summary>
	/// “§–¾‰»‚ğØ‚è‘Ö‚¦‚é
	/// </summary>
	void ClearChange();

	//“§–¾‰»‚ğØ‚è‘Ö‚¦‚é
	bool m_clearFlag=true;							//true‚È‚ç“§–¾‰»false‚È‚ç‰ğœ
	EnEnemyActState m_SetActState=m_ActState;		//‚Ç‚¿‚ç‚Ìó‘Ô‰»‚ğ‹L‰¯‚·‚é

	float m_clearChangeTime = 0.0f;					//Ø‚è‘Ö‚í‚é‚Ü‚Å‚ÌŠÔ

};

