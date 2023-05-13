#include "stdafx.h"
#include "Enemy_Clear.h"

#include "GameManager.h"
namespace
{
	const float		LINEAR_COMPLETION = 0.2f;		// üŒ`•âŠ®‚ÌƒtƒŒ[ƒ€”
}

Enemy_Clear::Enemy_Clear()
{

}
Enemy_Clear::~Enemy_Clear()
{
	m_clearFlag = true;
}
bool Enemy_Clear::Start()
{
	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì“Ç‚İ‚İ
	Animation();

	// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_enemyRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 3);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	// ƒpƒXˆÚ“®
	m_point = &m_pointList[0];

	return true;
}
void Enemy_Clear::Update()
{
	// •`‰æ‚µ‚È‚¢ƒtƒ‰ƒO‚ªtrue‚Ì‚Æ‚«
	if (m_NotDrawFlag == true) {
		return;
	}
	// ƒfƒtƒHƒ‹ƒg‚É–ß‚·ƒtƒ‰ƒO‚ªtrue‚Ì‚Æ‚«
	if (m_ChangeDefaultFlag == true) {
		m_ActState = CRAW;
		m_ChangeDefaultFlag = false;
	}

	// ï¿½Mï¿½ï¿½ï¿½eï¿½É“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ğ•·‚ï¿½ï¿½ï¿½ï¿½Æ‚ï¿½
	if (m_HitSoundBulletFlag == true && m_HitFlashBulletFlag == true) {
		// ï¿½Mï¿½ï¿½ï¿½eï¿½ï¿½Dï¿½æ‚·ï¿½ï¿½
		m_HitSoundBulletFlag = false;
	}

	// ‘MŒõ’e‚É“–‚½‚Á‚½
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
	// ‰¹”š’e‚ğg—p‚µ‚½
	if (m_HitSoundBulletFlag == true) {
		m_ActState = LISTEN;
	}

	switch (m_ActState) {
		// „‰ñ
	case CRAW:
		Update_OnCraw();
		break;
		// ’ÇÕ
	case TRACKING:
		Update_OnTracking();
		break;
		// ŒÄ‚Î‚ê‚½‚Æ‚«
	case CALLED:
		Update_OnCalled();
		break;
		// „‰ñó‘Ô‚É–ß‚é
	case BACKBASEDON:
		Update_OnBackBasedOn();
		break;
		// ‘MŒõ’e‚É“–‚½‚Á‚½
	case CONFUSION:
		Update_OnConfusion();
		break;
		// ‰¹”š’e‚ğg—p‚µ‚½‚Æ‚«
	case LISTEN:
		UpDate_OnListen();
		break;
	}

	Enemy::PlayAnimation();		// ƒAƒjƒ[ƒVƒ‡ƒ“
	ClearChange();
	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// ƒLƒƒƒ‰ƒNƒ^[ƒRƒ“ƒgƒ[ƒ‰[‚ğƒ‚ƒfƒ‹‚ÌˆÊ’u‚Æ“¯Šú
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);
	// ‹–ìŠp
	Enemy::Act_SeachPlayer();

	m_enemyRender.Update();	// XV
}
void Enemy_Clear::Update_OnCraw()
{
	if (m_ChachPlayerFlag == true) {
		m_enAnimationState = IDLE;
		return;
	}

	Enemy::Act_Craw();				// „‰ñs“®

	// ‹–ìŠp‚ÉƒvƒŒƒCƒ„[‚ª‚¢‚é‚Æ‚«
	if (m_TrakingPlayerFlag == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Clear::Update_OnTracking()
{
	// ƒvƒŒƒCƒ„[‚ğ•ß‚Ü‚¦‚½‚Æ‚«
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
		return;
	}


	Enemy::Act_Tracking();			// ’ÇÕs“®

	// ‹–ìŠp‚ÉƒvƒŒƒCƒ„[‚ª‚¢‚È‚¢‚Æ‚«
	if (m_TrakingPlayerFlag == false) {
		Enemy::Act_MissingPlayer();
	}
}

void Enemy_Clear::Update_OnCalled()
{
	Enemy::Act_Called();

	// ‹–ìŠp‚ÉƒvƒŒƒCƒ„[‚ª‚¢‚é‚Æ‚«
	if (m_TrakingPlayerFlag == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Clear::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();					// ’ÇÕs“®‚©‚ç‚ÌØ‚è‘Ö‚¦
	m_ActState = CRAW;
}

void Enemy_Clear::Update_OnConfusion()
{
	// ‘MŒõ’e‚É“–‚½‚Á‚½‚Æ‚«

	Enemy::Act_HitFlashBullet();

	// d’¼‚ª‰ğ‚¯‚Ä‚¢‚é‚Æ‚«
	if (m_HitFlashBulletFlag == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Clear::UpDate_OnListen()
{
	// ‰¹”š’e‚ğg‚Á‚½‚Æ‚«

	// ‘MŒõ’e‚ğH‚ç‚Á‚Ä‚¢‚é‚Æ‚«‚ÍÀs‚µ‚È‚¢
	if (m_HitFlashBulletFlag == true) {
		m_HitFlashBulletFlag = false;
		return;
	}

	Enemy::Act_HitSoundBullet();

	// Œø‰Ê‚ªI—¹‚µ‚½‚Æ‚«
	if (m_HitSoundBulletFlag == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Clear::ClearChange()

{
	if (m_SetActState != m_ActState&&m_clearChangeTime>=0.0f)
	{
		m_clearChangeTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//„‰ñ’†‚È‚ç“§–¾‰»‚É‚·‚é
	if (m_ActState == CRAW)
	{
		m_clearFlag = true;
		m_SetActState = CRAW;
		m_clearChangeTime = 0.0f;
	}
	//’ÇÕ’†‚È‚ç“§–¾‰»‚ğ‰ğœ
	else if (m_ActState == TRACKING)
	{
		m_clearFlag = false;
		m_SetActState = TRACKING;
		m_clearChangeTime = 1.0f;
	
	}
}

void Enemy_Clear::Render(RenderContext& rc)
{
	//“§–¾‰»‰ğœ
	if (m_clearFlag == false)
	{
		if (m_NotDrawFlag == false) {
			m_enemyRender.Draw(rc);
		}
	}
	
}