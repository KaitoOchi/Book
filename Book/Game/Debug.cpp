#include "stdafx.h"
#include "Debug.h"

#include "Mirror.h"
#include "level3DRender/LevelRender.h"


Debug::Debug()
{
	//当たり判定を有効化
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//フレームレートを固定
	//g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Fix, 60);
}

Debug::~Debug()
{
	DeleteGO(m_playerCollision);

	delete m_levelRender;
}

bool Debug::Start()
{
	//アニメーションクリップ
	animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	animationClips[enAnimationClip_Jump].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/player/player.tkm");
	m_modelRender.SetPosition(Vector3(-20.0f, 100.0f, -300.0f));
	Quaternion rotation;
	rotation.AddRotationDegY(220.0f);
	m_modelRender.SetRotation(rotation);
	m_modelRender.SetScale(Vector3::One);
	m_modelRender.Update();

	m_stageModelRender.Init("Assets/modelData/stage1.tkm");
	m_stageModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_stageModelRender.SetRotation(Quaternion::Identity);
	m_stageModelRender.SetScale(Vector3::One);
	m_stageModelRender.Update();

	m_animModelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_animModelRender.SetPosition(Vector3(100.0f, 0.0f, 0.0f));
	m_animModelRender.SetRotation(Quaternion::Identity);
	m_animModelRender.SetScale(Vector3::One);
	m_animModelRender.Update();

	m_fontRender.SetText(L"たちつてと");
	m_fontRender.SetPosition(Vector3(-500.0f, 0.0f, 0.0f));

	m_playerCollision = NewGO<CollisionObject>(0);
	m_playerCollision->CreateBox(Vector3(0.0f, 50.0f, 0.0f), Quaternion::Identity, Vector3(5.0f, 5.0f, 5.0f));
	m_playerCollision->SetName("collision1");
	m_playerCollision->SetIsEnableAutoDelete(false);
	m_playerCollision->SetIsEnable(true);

	m_levelRender = new LevelRender;

	//レベルデザイン処理
	m_levelRender->Init("Assets/level3D/level0_0.tkl", [&](LevelObjectData& objData) {

		//名前がunityChanなら
		if (objData.ForwardMatchName(L"unityChan") == true) {
			m_mirror = NewGO<Mirror>(0, "mirror");
			return true;
		}

		//名前がbackgroundなら
		if (objData.EqualObjectName(L"background") == true) {

			return false;
		}
		return true;
		}
	);

	m_position.y = 50.0f;

	m_pointLight.SetPosition(m_position);
	m_pointLight.SetColor(Vector3(5.0f, 0.0f, 0.0f));
	m_pointLight.SetRange(150.0f);
	m_pointLight.Update();

	return true;
}

void Debug::Update()
{
	m_position.x += g_pad[0]->GetLStickXF();
	m_position.z += g_pad[0]->GetLStickYF();

	m_pointLight.SetPosition(m_position);
	m_pointLight.Update();

	m_animModelRender.PlayAnimation(enAnimationClip_Run);
	m_animModelRender.Update();
}

void Debug::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_animModelRender.Draw(rc);
	m_stageModelRender.Draw(rc);
	m_fontRender.Draw(rc);
}
