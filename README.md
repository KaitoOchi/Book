<a id="anchor1"></a>
# Carta Gatto

  ![title](portfolio/00_title.png)

<br><br>

---
<a id="anchor2"></a>
# 目次

  + [Carta Gatto](#anchor1)
  + [目次](#anchor2)
  + [概要](#anchor3)
  + [操作方法](#anchor4)
  + [制作環境](#anchor5)
  + [担当箇所](#anchor6)
  + [技術紹介](#anchor7)
    + [UVアニメーション](#anchor8)
    + [ブルーム](#anchor9)
    + [シャドウイング](#anchor10)
    + [輪郭線](#anchor11)
    + [ディザリング](#anchor12)
    + [ワイプ](#anchor13)
    + [Singleton](#anchor14)

<br><br>

---
<a id="anchor3"></a>
# 概要

  + 制作期間<br>
    2023年2月～5月

<br>

  + 制作人数<br>
    4人

<br>

  + ジャンル<br>
    アクション・ステルスゲーム

<br>

  + プレイ人数<br>
    1人

<br>

  + 対応ハード<br>
    windows11


<br><br>

---
<a id="anchor4"></a>
# 操作方法

![title](portfolio/01_guide.png)

  + Aボタン
    + 決定
    + ダッシュ
    + お宝獲得(長押し)

<br>

  + Bボタン
    + キャンセル
    + ジャンプ

<br>

  + Lボタン
    + 2D/3D切り替え

<br>

  + Rボタン
    + アイテム使用

<br>

  + Lスティック
    + 移動
  
<br>

  + Rスティック
    + カメラ回転
    + プレイヤーの方向へ向く(押し込み)

<br>

  + 十字キー
    + 閃光弾を選択(左キー)
    + 音爆弾を選択(右キー)

<br>

  + Startボタン
    + ポーズ画面を開く


<br><br>

---
<a id="anchor5"></a>
# 制作環境

  + **使用ツール**<br>
    Visual Studio 2022<br>
    Visual Studio Code<br>
    3ds Max 2023<br>
    Adobe Photoshop 2022<br>
    Effekseer<br>
    fork<br>
    Github<br>

<br>

  + **使用言語**<br>
    C++<br>
    HLSL<br>


<br><br>

---
<a id="anchor6"></a>
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


<br><br>

---
<a id="anchor7"></a>
# 技術紹介

<a id="anchor8"></a>
+ ## UVアニメーション
  
  ![title](portfolio/02_uvAnimation.png)

  2D状態のプレイヤーは、全てのアニメーションを配置した画像を一枚の板ポリゴンに貼り、uv座標を変更することでアニメーションを実現しています。

<br>

<a id="anchor9"></a>
+ ## ブルーム

<a id="anchor10"></a>
+ ## シャドウイング

<a id="anchor11"></a>
+ ## 輪郭線

<a id="anchor12"></a>
+ ## ディザリング

<a id="anchor13"></a>
+ ## ワイプ

<a id="anchor14"></a>
+ ## Singleton