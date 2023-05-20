#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class Gage;
class Game;
class Enemy :public IGameObject
{
public:
	Enemy();
	virtual ~Enemy()=0;

	bool Start();

	/// <summary>
	/// ƒAƒjƒ[ƒVƒ‡ƒ“
	/// </summary>
	void Animation();
	/// <summary>
	/// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// •Ç‚ÆÕ“Ë‚µ‚½‚©‚Ç‚¤‚©‚Ìˆ—
	/// </summary>
	/// <param name="pos">À•W</param>
	/// <returns></returns>
	bool WallAndHit(Vector3 pos);
	/// <summary>
	/// ‰ñ“]ˆ—
	/// </summary>
	/// <param name="rot">©g‚ªŒü‚©‚¤ƒxƒNƒgƒ‹</param>
	void Rotation(Vector3 rot);
	/// <summary>
	/// ƒiƒrƒƒbƒVƒ…‚ğì¬‚·‚é
	/// </summary>
	void CreateNavimesh(Vector3 pos);
	/// <summary>
	/// „‰ñs“®
	/// </summary>
	void Act_Craw();
	/// <summary>
	/// ’ÇÕs“®
	/// </summary>
	void Act_Tracking();
	/// <summary>
	/// Ú‹ßs“®
	/// </summary>
	void Act_Access();
	/// <summary>
	/// “Ëis“®
	/// </summary>
	/// <param name="time">“Ëi‚·‚é‚Ü‚Å‚Ìƒ`ƒƒ[ƒWŠÔ</param>
	void Act_Charge(float time);
	/// <summary>
	/// “Ëi‚µ‚½Œã‚Ìs“®ˆ—
	/// </summary>
	void Act_ChargeEnd();
	/// <summary>
	/// •Ç‚Æ‚ÌÕ“Ë”»’è
	/// </summary>
	void Act_Charge_HitWall();
	/// <summary>
	/// “G‚ğŒÄ‚Ôs“®
	/// </summary>
	void Act_Call();
	/// <summary>
	/// ŒÄ‚Î‚ê‚½‚Ìs“®
	/// </summary>
	void Act_Called();
	/// <summary>
	/// Œ©¸‚Á‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	void Act_Loss();
	/// <summary>
	/// ‘MŒõ’e‚ª“–‚½‚Á‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	void Act_HitFlashBullet();
	/// <summary>
	/// ‰¹”š’e‚ª“–‚½‚Á‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	void Act_HitSoundBullet();
	/// <summary>
	/// s“®’â~
	/// </summary>
	/// <param name="time">’â~‚·‚éŠÔ</param>
	/// <param name="i">g—p‚·‚éƒ^ƒCƒ}[‚ğw’è</param>
	/// <returns></returns>
	bool Act_Stop(float time,int i);
	/// <summary>
	/// ƒvƒŒƒCƒ„[‚ğ”­Œ©‚·‚éˆ—
	/// </summary>
	/// <returns></returns>
	void Act_SeachPlayer();
	/// <summary>
	/// ƒvƒŒƒCƒ„[‚ğŠm•Û‚·‚éˆ—
	/// </summary>
	/// <returns></returns>
	bool Act_CatchPlayer();
	/// <summary>
	/// ƒvƒŒƒCƒ„[‚ğŒ©¸‚Á‚½‚Ìˆ—
	/// Œ©¸‚Á‚½ˆÊ’u‚Ü‚ÅˆÊ’u‚ğˆÚ“®‚·‚é
	/// </summary>
	void Act_MoveMissingPosition();
	/// <summary>
	/// ƒvƒŒƒCƒ„[‚ğŒ©¸‚Á‚½‚Ìˆ—
	/// ƒvƒŒƒCƒ„[‚ğ’T‚·
	/// </summary>
	void Act_SearchMissingPlayer();

	void SpotLight_New(Vector3 position,int num);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	void VigilanceCount();				//

	void Efect_Dizzy();
	void Efect_FindPlayer();
	void Efect_MissingPlayer();

	// ƒGƒlƒ~[‚Ìí—Ş
	enum EnemyType
	{
		TYPE_NORMAL,
		TYPE_CHARGE,
		TYPE_SEARCH,
		TYPE_CLEAR
	};
	EnemyType m_enemyType;

	// ƒGƒlƒ~[‚ÌƒAƒjƒ[ƒVƒ‡ƒ“ƒXƒe[ƒg
	// Œp³‚µ‚½”h¶ƒNƒ‰ƒX‚ÅƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“Ç‚İ‚İAŠÖ”‚ğŒÄ‚Ô‚ÆÄ¶‚³‚ê‚Ü‚·B
	enum EnAnimationClip
	{
		m_enAnimation_Idle,		// ‘Ò‹@
		m_enAnimation_Walk,		// •à‚­
		m_enAnimation_Run,		// ‘–‚é
		m_enAnimation_Attack,	// UŒ‚
		m_enAnimation_Damege,	// ”í’e
		m_enAnimation_Dizzy,	// ‘MŒõ’e‚ğó‚¯‚½‚Æ‚«
		m_enAnimation_Loss,		// ƒvƒŒƒCƒ„[‚ğŒ©¸‚Á‚½
		m_enAnimation_Num
	};
	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒXƒe[ƒg
	AnimationClip m_enAnimationClips[m_enAnimation_Num];
	
	// ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶—pƒXƒe[ƒg
	enum EnAnimationState
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		DAMEGE,
		DIZZY,
		LOSS
	};
	EnAnimationState m_enAnimationState = IDLE;

	// ƒGƒlƒ~[‚Ìs“®ƒpƒ^[ƒ“
	enum EnEnemyActState
	{
		CRAW,					// „‰ñ
		TRACKING,				// ’ÇÕ
		SEARCH,					// õ“G
		MISSING_MOVEPOSITON,	// Œ©¸‚Á‚½À•W‚Ü‚ÅˆÚ“®‚µ‚½
		MISSING_SEARCHPLAYER,	// Œ©¸‚Á‚½ƒvƒŒƒCƒ„[‚ğ’T‚·
		CALL,					// ü‚è‚Ì“G‚ğŒÄ‚Ô
		CALLED,					// CALL‚ÉSearchˆÈŠO‚ªÀs
		CHARGE,					// “Ëi
		CHARGEEND,				// “ËiI—¹
		BACKBASEDON,			// „‰ñó‘Ô‚É–ß‚é
		CONFUSION,				// ‘MŒõ’e‚É‚ ‚½‚Á‚½‚Æ‚«
		LISTEN,					// ‰¹”š’e‚ğg—p‚µ‚½‚Æ‚«
		CATCH					// •ßŠl‚µ‚½
	};
	/// <summary>
	/// ƒGƒlƒ~[‚Ìs“®ƒpƒ^[ƒ“Bswitch‚ÅŠÇ—‚µ‚Ä‚­‚¾‚³‚¢
	/// </summary>
	/// <param name="CRAW">					F„‰ñ										</param>
	/// <param name="TRACKING">				F’ÇÕ										</param>
	/// <param name="SEARCH">				F‘Ò‹@										</param>
	/// <param name="MISSING_MOVEPOSITON">	FŒ©¸‚Á‚½À•W‚Ü‚ÅˆÚ“®‚µ‚½					</param>
	/// <param name="MISSING_SEARCHPLAYER">	FŒ©¸‚Á‚½ƒvƒŒƒCƒ„[‚ğ’T‚·					</param>
	/// <param name="CALL">					Fü‚è‚Ì“G‚ğŒÄ‚Ô							</param>
	/// <param name="CALLED">				FCALL‚ÉSearchˆÈŠO‚ªÀs					</param>
	/// <param name="CHARGE">				F“Ëi										</param>
	/// <param name="BACKBASEDON">			F„‰ñó‘Ô‚É–ß‚é							</param>
	/// <param name="CONFUSION">			F‘MŒõ’e‚É‚ ‚½‚Á‚½‚Æ‚«						</param>
	/// <param name="LISTEN">				F‰¹”š’e‚ğg—p‚µ‚½‚Æ‚«						</param>
	/// <param name="CATCH">				F•ßŠl										</param>
	EnEnemyActState m_ActState;

	// w’è‚Å‚«‚éƒpƒXˆÚ“®
	enum EnEnemyPassState
	{
		LINE_VERTICAL,					// c
		LINE_HORIZONTAL,				// ‰¡
		SQUARE_RIGHT,					// ‰E‰ñ‚è(³•ûŒ`)
		SQUARE_LEFT,					// ¶‰ñ‚è(³•ûŒ`)
		ANGLE_RIGHT,					// ‰E‚É’¼Šp
		ANGLE_LEFT,						// ¶‚É’¼Šp
		RECTANGLE_RIGHT,				// ‰E‰ñ‚è(’·•ûŒ`)
		RECTANGLE_LEFT,					// ¶‰ñ‚è(’·•ûŒ`)
	};
	EnEnemyPassState PassState;

	/// <summary>
	/// ƒGƒlƒ~[‚Ì„‰ñƒpƒ^[ƒ“‚ğw’è
	/// </summary>
	/// <param name="0">	Fc				</param>
	/// <param name="1">	F‰¡				</param>
	/// <param name="2">	F‰E‰ñ‚è(³•ûŒ`)	</param>
	/// <param name="3">	F¶‰ñ‚è(³•ûŒ`)	</param>
	/// <param name="4">	F‰E‚É’¼Šp		</param>
	/// <param name="6">	F‰E‚É’¼Šp		</param>
	/// <param name="7">	F‰E‰ñ‚è(’·•ûŒ`)	</param>
	/// <param name="8">	F¶‰ñ‚è(’·•ûŒ`)	</param>
	void Pass(int num);

	/// <summary>
	/// À•W‚ğİ’è‚·‚é
	/// </summary>
	void SetPosition(Vector3 pos) {
		m_position = pos;
	}

	/// <summary>
	/// ‰ñ“]‚ğİ’è‚·‚é
	/// </summary>
	void SetRotation(Quaternion rot) {
		m_rotation = rot;
	}

	/// <summary>
	/// ƒXƒP[ƒ‹‚ğİ’è‚·‚é
	/// </summary>
	void SetScale(Vector3 sca) {
		m_scale = sca;
	}

	/// <summary>
	/// ƒvƒŒƒCƒ„[‚ğŠm•Û‚µ‚½‚©‚Ç‚¤‚©•Ô‚·
	/// </summary>
	/// <returns></returns>
	bool GetChachPlayerFlag() {
		return m_ChachPlayerFlag;
	}

	/// <summary>
	/// ‘MŒõ’e‚Ì”í’eƒtƒ‰ƒO‚ğİ’è
	/// </summary>
	/// <param name="">”í’e‚µ‚½‚©‚Ç‚¤‚©‚Ç‚¤‚©”»’è‚·‚éBtrue‚È‚ç”í’e‚µ‚½‚Æ”»’è</param>
	void SetHitFlashBullet(bool b) {
		m_HitFlashBulletFlag = b;
	};

	/// <summary>
	/// ‘MŒõ’e‚É“–‚½‚Á‚½‚©‚Ç‚¤‚©•Ô‚·
	/// </summary>
	/// <returns>”í’e‚µ‚½‚©‚Ç‚¤‚©‚Ç‚¤‚©”»’è‚·‚éBtrue‚È‚ç”í’e‚µ‚½‚Æ”»’è</returns>
	bool GetHitFlushBullet() {
		return m_HitFlashBulletFlag;
	}

	/// <summary>
	/// ‰¹”š’e‚Ì”í’eƒtƒ‰ƒO‚ğİ’è
	/// </summary>
	/// <param name="">”í’e‚µ‚½‚©‚Ç‚¤‚©‚Ç‚¤‚©”»’è‚·‚éBtrue‚È‚ç”í’e‚µ‚½‚Æ”»’è</param>
	void SetHitSoundBullet(bool b) {
		m_HitSoundBulletFlag = b;
	};

	/// <summary>
	/// ‰¹”š’e‚É“–‚½‚Á‚½‚©‚Ç‚¤‚©•Ô‚·
	/// </summary>
	/// <returns>”í’e‚µ‚½‚©‚Ç‚¤‚©‚Ç‚¤‚©”»’è‚·‚éBtrue‚È‚ç”í’e‚µ‚½‚Æ”»’è</returns>
	bool GetHitSoundBullet() {
		return m_HitSoundBulletFlag;
	}

	/// <summary>
	/// ƒAƒCƒeƒ€‚ÌÀ•W‚ğ“n‚·
	/// </summary>
	/// <returns></returns>
	void SetItemPos(Vector3 pos) {
		m_itemPos = pos;
	}

	/// <summary>
	/// À•W‚ğæ“¾‚·‚é
	/// </summary>
	const Vector3& GetPosition() const {
		return m_position;
	}

	/// <summary>
	/// ƒXƒP[ƒ‹‚ğæ“¾‚·‚é
	/// </summary>
	const Vector3& GetScale() const {
		return m_scale;
	}

	/// <summary>
	/// ‰ñ“]‚ğæ“¾‚·‚é
	/// </summary>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

	// ƒXƒ|ƒbƒgƒ‰ƒCƒg‚Ì”Ô†‚ğ‹³‚¦‚é
	void SetSpotLigNum(int num) {
		m_spotNum = num;
	}

	/// <summary>
	/// •`‰æ‚·‚é‚©‚Ç‚¤‚©Œˆ’è‚·‚éBtrue‚Ì‚Æ‚«•`‰æ‚µ‚È‚¢
	/// </summary>
	/// <param name="flag"></param>
	void SetNotDrawFlag(bool flag) {
		m_NotDrawFlag = flag;
	}

	/// <summary>
	/// s“®ƒpƒ^[ƒ“‚ğ–ß‚·
	/// </summary>
	void SetTrueChangeDefaultFlag() {
		m_ChangeDefaultFlag = true;
	}

	//------------------------------------------
	//ƒGƒlƒ~[‚Ì‘O•ûŒü‚ğ‹‚ß‚é
	const Vector3 GetFoward()const
	{
		return m_forward;
	}

	Vector3 m_foward=Vector3::AxisZ;

	/// <summary>
	/// “®‚©‚·‚©‚Ç‚¤‚©Œˆ‚ß‚é
	/// </summary>
	/// <param name="active">true‚È‚ç“®‚¯‚È‚¢</param>
	void SetActiveFlag(bool active)
	{
		m_activeFlag = active;
	}

	const bool GetActiveFlag()
	{
		return m_activeFlag;
	}

	// ï¿½Xï¿½|ï¿½bï¿½gï¿½ï¿½ï¿½Cï¿½gï¿½ï¿½nï¿½ï¿½
	const SpotLight GetSpotLight() {
		return m_spotLight;
	}

protected:

	// ƒpƒXˆÚ“®—p‚Ìƒ|ƒCƒ“ƒg\‘¢‘Ì
	struct Point {
		Vector3 s_position;					// ƒ|ƒCƒ“ƒg‚ÌÀ•W
		int s_number;						// ƒ|ƒCƒ“ƒg‚Ì”Ô†
	};

	std::vector<Point> m_pointList;			// ƒ|ƒCƒ“ƒg\‘¢‘Ì‚Ì”z—ñ
	Point* m_point = nullptr;				// ƒ|ƒCƒ“ƒg\‘¢‘Ì‚Ìƒ|ƒCƒ“ƒ^AŒ»İ‚Ì–Ú“I’n‚É‚È‚é

	TknFile m_tknFile;						// tknƒtƒ@ƒCƒ‹
	PhysicsStaticObject m_bgObject;			// Ã“I•¨—ƒIƒuƒWƒFƒNƒg
	nsAI::NaviMesh m_nvmMesh;				// ƒiƒrƒƒbƒVƒ…
	nsAI::Path m_path;						// ƒpƒX
	nsAI::PathFinding m_pathFiding;			// ƒpƒX‚ğ’T‚·

	PlayerManagement* m_playerManagement = nullptr;
	Gage* m_gage = nullptr;
	Game* m_game = nullptr;

	EffectEmitter* m_soundEffect = nullptr;		// ƒGƒtƒFƒNƒg

	CharacterController m_characterController;	// ƒLƒƒƒ‰ƒNƒ^[ƒRƒ“ƒgƒ[ƒ‰[
	SphereCollider m_sphereCollider;			// ƒXƒtƒBƒAƒRƒ‰ƒCƒ_[

	FontRender m_fontRender;				// ƒtƒHƒ“ƒgƒŒƒ“ƒ_[

	Vector3 m_position = Vector3::Zero;		// ƒGƒlƒ~[‚ÌÀ•W
	Vector3 m_firstPosition = Vector3::Zero;// ƒGƒlƒ~[‚ÌÅ‰‚ÌˆÊ’uî•ñ
	Vector3 m_forward = Vector3::AxisZ;		// ƒGƒlƒ~[‚Ì‘O•ûŒü
	Vector3 m_scale = Vector3::One;			// ƒXƒP[ƒ‹
	Vector3 m_playerPos = Vector3::Zero;	// ƒvƒŒƒCƒ„[‚ÌÀ•W
	Vector3 m_playerChargePosition = Vector3::Zero;			// “Ëi—pBƒvƒŒƒCƒ„[‚ÌÀ•W
	Vector3 m_playerMissionPosition = Vector3::Zero;		// Œ©¸‚Á‚½—pBƒvƒŒƒCƒ„[‚ÌÀ•W
	Vector3 m_sumPos = Vector3::Zero;		// ‘ˆÚ“®‹——£
	Vector3 m_setPos = Vector3::Zero;		// W‡‚·‚éÀ•W
	Vector3 m_itemPos = Vector3::Zero;		// ƒAƒCƒeƒ€‚ÌÀ•W
	Vector3 m_chargeDiff = Vector3::Zero;	// “Ëi‚ÌˆÚ“®—Ê


	Quaternion m_rotation = Quaternion::Identity;		// ‰ñ“]
	Quaternion m_firstRotation = Quaternion::Identity;	// ƒGƒlƒ~[‚ÌÅ‰‚Ì‰ñ“]î•ñ

	ModelRender m_enemyRender;				//ƒGƒlƒ~[ƒ‚ƒfƒ‹
	SpotLight m_spotLight;					//ƒXƒ|ƒbƒgƒ‰ƒCƒg

	bool m_HitFlashBulletFlag = false;		// ‘MŒõ’e‚ª“–‚½‚Á‚½‚©‚Ç‚¤‚©
	bool m_HitSoundBulletFlag = false;		// ‰¹”š’e
	bool m_CountFlag = false;				// ƒJƒEƒ“ƒg‚·‚éƒtƒ‰ƒO
	bool m_TrakingPlayerFlag = false;		// ƒvƒŒƒCƒ„[‚ğ’Ç‚¢‚©‚¯‚éƒtƒ‰ƒO
	bool m_ChachPlayerFlag = false;			// ƒvƒŒƒCƒ„[‚ğŠm•Û‚µ‚½‚©‚Ç‚¤‚©
	bool m_CalculatedFlag = false;			// “Ëi—pƒtƒ‰ƒOBˆê“x‚¾‚¯QÆ‚ğs‚¤
	bool m_NotDrawFlag = false;				// •`‰æ‚·‚é‚©‚Ç‚¤‚©
	bool m_ChangeDefaultFlag = false;		// ƒfƒtƒHƒ‹ƒg‚ÉØ‚è‘Ö‚¦‚é‚©‚Ç‚¤‚©
	bool m_activeFlag = false;				//Å‰‚©‚ç“®‚¯‚é‚©‚»‚¤‚©

	/// <summary>
	/// ƒGƒtƒFƒNƒg‚ğ•`‰æ‚µ‚½‚©‚Ç‚¤‚©‚Ìƒtƒ‰ƒOBtrue‚Ì‚Æ‚«•`‰æ‚µ‚½
	/// </summary>
	/// <param name="0">	F™‚ÌƒGƒtƒFƒNƒg	</param>
	/// <param name="1">	F!‚ÌƒGƒtƒFƒNƒg		</param>
	/// <param name="2">	F?‚ÌƒGƒtƒFƒNƒg		</param>
	std::array<bool, 3>m_efectDrawFlag;

	/// <summary>
	/// </summary>
	/// <param name="0">	F‘MŒõ’e‚ğó‚¯‚½‚Æ‚«‚ÌÄs“®ŠÔ	</param>
	/// <param name="1">	F„‰ñ‚ÌƒpƒX‚É—¯‚Ü‚éŠÔ			</param>
	/// <param name="2">	F“Ëi‚ğs‚¤‚Ü‚Å‚Ì‘Ò‹@ŠÔ			</param>
	/// <param name="3">	FƒvƒŒƒCƒ„[‚ğŒ©¸‚Á‚½‚Ì‘Ò‹@ŠÔ	</param>
	std::array<float, 4>m_addTimer;

	float m_NaviTimer = 0.0f;				// ƒiƒrƒƒbƒVƒ…—p‚Ìƒ^ƒCƒ}[
	float m_move = 1.0f;
	float m_Vicount;						//Œx‰ú“x‚ğˆê’è‰ñ”‘‚â‚·

	int m_spotNum = 0;						// ƒXƒ|ƒbƒgƒ‰ƒCƒg‚ÌŒÂ”
};