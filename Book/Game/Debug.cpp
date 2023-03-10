#include "stdafx.h"
#include "Debug.h"

#include "Mirror.h"
#include "level3DRender/LevelRender.h"
#include "graphics/Texture.h"


Debug::Debug()
{
	//当たり判定を有効化
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//フレームレートを固定
	//g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Fix, 60);
}

Debug::~Debug()
{
	//DeleteGO(m_playerCollision);

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

	m_modelRender.Init("Assets/modelData/player/player2D.tkm");
	m_modelRender.SetPosition(Vector3(-20.0f, 190.0f, -370.0f));
	m_modelRender.SetScale(Vector3::One);
	Quaternion rot;
	rot.AddRotationDegY(180.0f);
	m_modelRender.SetRotation(rot);
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

	//m_playerCollision = NewGO<CollisionObject>(0);
	//m_playerCollision->CreateBox(Vector3(0.0f, 50.0f, 0.0f), Quaternion::Identity, Vector3(5.0f, 5.0f, 5.0f));
	//m_playerCollision->SetName("collision1");
	//m_playerCollision->SetIsEnableAutoDelete(false);
	//m_playerCollision->SetIsEnable(true);

	m_levelRender = new LevelRender;

	//レベルデザイン処理
	m_levelRender->Init("Assets/level3D/level0_0.tkl", [&](LevelObjectData& objData) {

		//名前がunityChanなら
		if (objData.ForwardMatchName(L"unityChan") == true) {
			//m_mirror = NewGO<Mirror>(0, "mirror");
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

	m_pointLight.SetPosition(Vector3(-50.0f, 0.0f, 0.0f));
	m_pointLight.SetColor(Vector3(15.0f, 0.0f, 0.0f));
	m_pointLight.SetRange(250.0f);
	m_pointLight.Update();

	m_spotLight.SetPosition(m_position);
	m_spotLight.SetColor(Vector3(10.0f, 0.0f, 0.0f));
	m_spotLight.SetRange(500.0f);
	m_spotLight.SetDirection(Vector3(1.0f, -1.0f, 1.0f));
	m_spotLight.SetAngle(25.0f);
	m_spotLight.Update();

	texture[0].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_1.dds");
	texture[1].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_2.dds");
	texture[2].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_3.dds");

	return true;
}

void Debug::Update()
{
	m_position.x += g_pad[0]->GetLStickXF();

	if (g_pad[0]->IsPress(enButtonB)) {
		m_position.y += g_pad[0]->GetLStickYF();
	}
	else {
		m_position.z += g_pad[0]->GetLStickYF();
	}
	m_spotLight.SetPosition(m_position);

	Quaternion qRotY;
	qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
	qRotY.Apply(m_spotLight.GetDirection());

	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, m_spotLight.GetDirection());
	Quaternion qRotX;
	qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);

	qRotX.Apply(m_spotLight.GetDirection());

	Quaternion qRot;
	qRot.SetRotation({ 0.0f, 0.0f, -1.0f }, m_spotLight.GetDirection());

	m_spotLight.Update();


	m_animModelRender.PlayAnimation(enAnimationClip_Run);
	m_animModelRender.Update();


	//2Dアニメーションをモデルで表示する処理
	int j = i / 10;
	i++;
	if (i >= 30) {
		i = 0;
	}
	m_modelRender.GetModel().ChangeAlbedoMap("", texture[j]);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void Debug::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_animModelRender.Draw(rc);
	m_stageModelRender.Draw(rc);
	m_fontRender.Draw(rc);
}
