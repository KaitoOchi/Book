#include "stdafx.h"
#include "Wipe.h"
#include "Gage.h"

#include "BackGround.h"
#include "Stage/Wall/Wall.h"
#include "Stage/Wall/Wall_Normal.h"
#include "Stage/Wall/Wall_Post.h"
#include "Stage/Wall/Wall_Decoration.h"

namespace
{
	const float	MOVE_TIME = 1.3f;		//移動時間
	const float MOVE_SPEED = 0.3f;		//移動速度
	const float ENEMY_DURATION = 0.1f;	//敵の間隔
}

Wipe::Wipe()
{
	RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftX = -290;   //画面左上のx座標
	RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftY = 662;   //画面左上のy座標

	m_stage.reserve(16);
}

Wipe::~Wipe()
{
	delete m_enemyAnim;

	//ステージを削除
	for (auto& stage : m_stage)
	{
		DeleteGO(stage);
	}
	m_stage.clear();
	m_stage.shrink_to_fit();

	//床を削除
	DeleteGO(m_backGround);
}

bool Wipe::Start()
{
	m_wipePos = WIPE_POS_MIN;
	m_outlinePos = OUTLINE_POS_MIN;

	//ポイントライトの設定
	m_pointLight.SetPointLight(
		1,
		Vector3(11500.0f, 150.0f, 1200.0f),
		Vector3(1.0f, 0.2f, 0.2f),
		1500.0f
	);
	RenderingEngine::GetInstance()->GetLightCB().ptNum = 2;

	//輪郭線の設定
	m_outlineSpriteRender.Init("Assets/sprite/UI/Gauge/wipe_outline.DDS", 262.0f, 205.0f);
	m_outlineSpriteRender.SetPosition(m_outlinePos);
	m_outlineSpriteRender.Update();

	//警告画像
	m_warningSpriteRender.Init("Assets/sprite/UI/Gauge/image_warning.DDS", 414.0f, 121.0f);
	m_warningSpriteRender.SetPosition(Vector3(-675.0f, -125.0f, 0.0f));
	m_warningSpriteRender.SetScale(Vector3(0.5f, 0.5f, 0.0f));
	m_warningSpriteRender.Update();

	//アニメーション設定
	m_enemyAnim = new AnimationClip;
	m_enemyAnim->Load("Assets/animData/enemy/run_battle.tka");
	m_enemyAnim->SetLoopFlag(true);

	//敵の初期化
	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {
		m_enemy[i].modelRender.Init("Assets/modelData/enemy/enemy_normal.tkm", m_enemyAnim, 1, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, true);
		m_enemy[i].modelRender.SetPosition(Vector3(10000.0f, 0.0f, 0.0f));
		m_enemy[i].modelRender.SetScale(Vector3(2.0f, 2.0f, 2.0f));
		m_enemy[i].modelRender.Update();
		m_enemy[i].moveSpeed[0] = m_bezierPos[0];
		m_enemy[i].moveSpeed[1] = m_bezierPos[0];
		m_enemy[i].moveSpeed[2] = m_bezierPos[1];
	}

    LevelDesign();

	return true;
}

void Wipe::LevelDesign()
{
	int i = 0;

    // レベルデザイン処理
    m_levelRender.Init("Assets/level3D/level_wipe.tkl", [&](LevelObjectData& objData) {

		//名前がbackgroundなら
		if (objData.EqualObjectName(L"base") == true) {
			// 背景を生成
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);
			return true;
		}
		// 名前がboxなら
		else if (objData.EqualObjectName(L"box") == true) {
			// 壁を生成
			Wall_Normal* normal = NewGO<Wall_Normal>(0, "wall_Normal");
			normal->SetPosition(objData.position);
			normal->SetRotation(objData.rotation);
			normal->SetScale(objData.scale);
			m_stage.emplace_back(normal);
			return true;
		}
		// 名前がpostのとき
		else if (objData.EqualObjectName(L"post") == true) {
			// 柱を生成する
			Wall_Post* post = NewGO<Wall_Post>(0, "wall_Post");
			post->SetPosition(objData.position);
			post->SetRotation(objData.rotation);
			post->SetScale(objData.scale);
			m_stage.emplace_back(post);
			return true;
		}
		// 名前がdecorationのとき
		else if (objData.EqualObjectName(L"decoration") == true) {
			// 装飾を生成
			Wall_Decoration* decoration = NewGO<Wall_Decoration>(0, "wall_Decoration");
			decoration->SetPosition(objData.position);
			decoration->SetRotation(objData.rotation);
			decoration->SetScale(objData.scale);
			m_stage.emplace_back(decoration);
			return true;
		}
		// 名前がpositionのとき
		else if (objData.EqualObjectName(L"position") == true) {
			// ベジェ先の座標
			m_bezierPos[i] = objData.position;
			i++;
			return true;
		}
    });
}

void Wipe::Update()
{
	EnemyMove();

	WipeOutline();
}

void Wipe::EnemyMove()
{
	if (!m_isWipe) {
		return;
	}

	//時間が経過したら、移動を止める
	if (m_timer > MOVE_TIME) {
		m_isWipe = false;
		return;
	}

	//計測処理
	m_timer += g_gameTime->GetFrameDeltaTime() * MOVE_SPEED;

	float timer = m_timer;

	//敵の数だけfor文で回す
	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {

		//敵の間隔を設定
		timer = m_timer - (i * ENEMY_DURATION);

		Vector3 moveSpeed = m_enemy[i].moveSpeed[2];

		//ベジェ曲線を利用して敵を移動させる
		m_enemy[i].moveSpeed[0].Lerp(timer, m_bezierPos[0], m_bezierPos[1]);
		m_enemy[i].moveSpeed[1].Lerp(timer, m_bezierPos[1], m_bezierPos[2]);
		m_enemy[i].moveSpeed[2].Lerp(timer, m_enemy[i].moveSpeed[0], m_enemy[i].moveSpeed[1]);

		//回転させる
		moveSpeed -= m_enemy[i].moveSpeed[2];
		moveSpeed *= -1.0f;
		Quaternion rot;
		rot.SetRotationYFromDirectionXZ(moveSpeed);

		//敵モデルの設定
		m_enemy[i].modelRender.SetPosition(m_enemy[i].moveSpeed[2]);
		m_enemy[i].modelRender.SetRotation(rot);
		m_enemy[i].modelRender.Update();
	}
}

void Wipe::WipeOutline()
{
	if (!m_isWipe) {
		//時間が経過したら、ワイプの移動を止める
		if (m_outlineTimer <= 0.0f) {
			m_timer = 0.0f;
			return;
		}

		//時間を計測
		m_outlineTimer -= g_gameTime->GetFrameDeltaTime();
		m_outlineTimer = max(m_outlineTimer, 0.0f);
	}
	else {
		//時間が経過したら、ワイプの移動を止める
		if (m_outlineTimer > 1.0f) {
			return;
		}

		//時間を計測
		m_outlineTimer += g_gameTime->GetFrameDeltaTime();
		m_outlineTimer = min(m_outlineTimer, 1.0f);
	}

	//ワイプを移動
	m_outlinePos.Lerp(m_outlineTimer, OUTLINE_POS_MIN, OUTLINE_POS_MAX);
	m_wipePos.Lerp(m_outlineTimer, WIPE_POS_MIN, WIPE_POS_MAX);

	//ワイプの設定
	RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftX = m_wipePos.x;
	m_outlineSpriteRender.SetPosition(m_outlinePos);
	m_outlineSpriteRender.Update();

	//透明度を求める
	m_alpha += g_gameTime->GetFrameDeltaTime();
	if (m_alpha > 1.0f)
		m_alpha = -0.5f;
	float alpha = fabsf(-pow(m_alpha, 2.0f) + (2 * m_alpha));

	Vector3 warningPos = m_outlinePos;
	warningPos.y += 150.0f;

	//警告画像の設定
	m_warningSpriteRender.SetPosition(warningPos);
	m_warningSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_warningSpriteRender.Update();
}

void Wipe::Render(RenderContext& rc)
{
	if (m_timer <= 0.0f) {
		return;
	}

	//ステージの描画
	for (auto& wall : m_stage)
	{
		wall->WipeRender(rc);
	}

	//敵の描画
	for (int i = 0; i < ENEMY_NUM_WIPE; i++) {
		m_enemy[i].modelRender.Draw(rc);
	}

	m_warningSpriteRender.Draw(rc);
	m_outlineSpriteRender.Draw(rc);
}