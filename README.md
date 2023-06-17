<a id="anchor1"></a>
# Carta Gatto

<a id="anchor2"></a>
# 目次
+ [Carta Gatto](#anchor1)
+ [目次](#anchor2)
+ [概要](#anchor3)
+ [制作環境](#anchor4)
+ [担当箇所](#anchor5)
+ [技術紹介](#anchor6)

<a id="anchor3"></a>
# 概要

<a id="anchor4"></a>
# 制作環境

+ **使用ツール**<br>
Visual Studio 2022<br>
3ds Max 2023<br>
Adobe Photoshop 2022<br>
<br>

+ **使用言語**<br>
  C++<br>
  HLSL<br>
<br>

+ a

<a id="anchor5"></a>
# 担当箇所

  **作成したコード**
  
  <details><summary>.cpp .h</summary>

  + <details><summary>BookEngine</summary>

    + CollisionObject.cpp
    + CollisionObject.h
    + DirectionLight.cpp
    + DirectionLight.h
    + HemiSphereLight.cpp
    + HemiSphereLight.h
    + PointLight.cpp
    + PointLight.h
    + SpotLight.cpp
    + SpotLight.h
    + Bloom.cpp
    + Bloom.h
    + FontRender.cpp
    + FontRender.h
    + IRenderer.h
    + ModelRender.cpp
    + ModelRender.h
    + RenderingEngine.cpp
    + RenderingEngine.h
    + SpriteRender.cpp
    + SpriteRender.h
    + Level2DRender.cpp
    + Level2DRender.h
    + LevelRender.cpp
    + LevelRender.h
    + SkyCube.cpp
    + SkyCube.h
    + BookEngine.cpp
    + BookEngine.h
    + BookEnginePreCompile.cpp
    + BookEnginePreCompile.h

    </details>

  + <details><summary>Game</summary>
  
    + LightSensor.cpp
    + LightSensor.h
    + Object.cpp
    + Object.h
    + SecurityCamera.cpp
    + SecurityCamera.h
    + Sensor.cpp
    + Sensor.h
    + Event.cpp
    + Event.h
    + Opening.cpp
    + Opening.h
    + Result.cpp
    + Result.h
    + Title.cpp
    + Title.h
    + Title_Guide.cpp
    + Title_Guide.h
    + Title_Setting.cpp
    + Title_Setting.h
    + Painting.cpp
    + Painting.h
    + CountDown.cpp
    + CountDown.h
    + Fade.cpp
    + Fade.h
    + GoalSprite.cpp
    + GoalSprite.h
    + Logo.cpp
    + Logo.h
    + Minimap.cpp
    + Minimap.h
    + Pause.cpp
    + Pause.h
    + Wipe.cpp
    + Wipe.h
    + GameManager.cpp
    + GameManager.h

  </details>

</details>

<details><summary>.fx</summary>

  + gaussianBlur.fx
  + lighting_CB.h
  + model.fx
  + model_register.h
  + postEffect.fx
  + shadowMap.fx
  + SkyCubeMap.fx
  + sprite.fx
  + volumeLight.fx
  + zprepass.fx

</details>

<br>

  **改造したエンジンコード**

  <details><summary>.cpp .h</summary>

  + GameObjectManager.cpp
    + ゲームオブジェクトを全停止する機能の追加(41行目～)
  + EffectEngine.h
    + エフェクトの再生速度を変更する機能の追加(86行目)
  + Effect.h
    + 同上。EffectEmitter->Effect->EffectEngineクラスへと値を順に渡している(95行目)
  + RenderContext.h
    + ビューポートのみの設定を行うSetViewPort関数を追加(83行目)
  + GamePad.h
    + ボタンを離したかどうかを判定するGetKeyUp関数の追加(100行目)

  </details>

<br>

<a id="anchor6"></a>
# 技術紹介