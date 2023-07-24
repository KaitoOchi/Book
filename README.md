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
  + [リファクタリング](#anchor15)
    + [マジックナンバーの除去](#anchor16)

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
+ ## **UVアニメーション**
  
  ![title](portfolio/02_player2D.png)


  2D状態のプレイヤーは、全てのアニメーションを配置した画像を一枚の板ポリゴンに貼り、  
  表示したいアニメーション番号を定数バッファに値を入れ、  
  その値を元にuv座標を計算することでアニメーションを実現しています。  

  ![title](portfolio/03_uvAnimation.png)

<br>

<a id="anchor9"></a>
+ ## **川瀬式ブルーム**
  このエンジンでは、高速で高品質な処理を実現できる川瀬式ブルームを採用しています。  
  通常のブルーム処理は、輝度テクスチャをぼかす処理が一回のみですが、川瀬式では複数回ガウシアンブラーをかけ、それらを合成することで質の高いブルームを表現できます。
  
  ![title](portfolio/04_00_bloom.png)

  1. モデルから輝度抽出を行う。
  2. 輝度抽出した画像からダウンサンプリングを行い、ガウシアンブラーをかける。
  3. 作られた画像から2の工程をする、を複数回行う。90
  4. c

  ▼ブルーム実装前
  ![title](portfolio/04_01_bloom.png)

  ▼ブルーム実装後
  ![title](portfolio/04_02_bloom.png)

<br>

<a id="anchor10"></a>
+ ## **VSM(分散シャドウマップ)**
  VSM(Variance Shadow Map)とは、シャドウマップに書き込まれた「深度値の局所的な分散」を利用してシャドウを実現するアルゴリズムです。  
  局所的な分散とは、シャドウマップをグリッド状にしたときのグループごとの分散で、分散が大きいということは、グループ内の深度値の幅が広いということになります。  
  この分散が大きい部分が影の境界線になるので、影を薄くしてジャギーを目立たないようにする手法がVSMです。  
  あ

<br>

<a id="anchor11"></a>
+ ## **輪郭線**
  このゲームではプレイヤーやエネミーを強調するために輪郭線を実装しています。  
  一般的な輪郭線はポストエフェクトの処理で行いますが、シーン全体に輪郭線が発生したり、輪郭線の色が同じになる問題が発生します。  
  今回は、特定の目立たせたいモデルにのみ、色を分けて実装したいので、フォワードレンダリングによる輪郭線を実装しました。

  1.モデルのプロジェクション空間の深度値をテクスチャに書き込み、持ってくる。  
  2.今から描画するテクセルの深度値と、近傍8テクセルの深度値の平均を比較し、差が一定以上なら、輪郭線を描画する。

<br>

<a id="anchor12"></a>
+ ## **ディザリング**
  「探索時にステルス状態になる敵」を追加するためにディザリングを実装しました。  
  ディザリングとは一部のピクセルをディザパターンに従って描画しないようにする処理で、これにより、不透明オブジェクトを半透明っぽく表現することが出来ます。  

  1.ディザパターンと呼ばれる4x4の二次元配列を定義する。  
    ![title](portfolio/05_00_dithering.png)  

  2.ピクセルのスクリーン座標系のX座標とY座標4で割った余りを計算し、0～3の値を返す。  
    ![title](portfolio/05_01_dithering.png)  

  3.2で求めた値を使ってディザパターン配列から閾値を取得し、一定値以下ならピクセルキルを行う。
    ![title](portfolio/05_02_dithering.png)  
<br>

<a id="anchor13"></a>
+ ## **ビューポート**
  警戒度レベルが上がったとき左下に表示されるワイプは、ビューポートを追加して実装しています。

    ![title](portfolio/06_00_wipe.png)  

  ステージ外に地形を生成し、それをカメラに写すことでワイプを表現している。

    ![title](portfolio/06_01_wipe.png)  

  通常モデルとは別にワイプ用のモデルとカメラを設定し、ビューポートを切り替えて描画している。

  ▼ワイプ用の描画実行処理(赤色)と、通常の描画実行処理(青色)
    ![title](portfolio/06_02_wipe.png)  

<br>

<a id="anchor14"></a>
+ ## **Singleton**
  Sigletonは、2つの機能を提供するデザインパターンです。1つ目は、グローバルなアクセスポイントを確保します。2つ目は、インスタンスの数を一つに制限します。


<br><br>

---
<a id="anchor15"></a>
# リファクタリング

<a id="anchor16"></a>
+ ## **マジックナンバーの除去**

<br>

<a id="anchor17"></a>
+ ## **デフォルトコピーコンストラクタの禁止**