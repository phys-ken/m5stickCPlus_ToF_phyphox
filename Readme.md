# M5StickC Plusで測定した値をphyphoxアプリで表示する

このプロジェクトでは、M5StickC Plusを使用して、ToF HAT(VL53L0Xセンサ)を通じて距離を測定します。計測されたデータは、スマートフォン等にインストールされているPhyphoxアプリに、Bluetoothで送信されます。

## 必要なハードウェア

- M5StickC Plus
- ToF HAT(VL53L0Xセンサ)

## 必要なライブラリ

- M5StickCPlus
- Wire
- VL53L0X
- phyphoxBle

## 設定

1. M5StickC PlusとToF HATを接続します。
2. Arduino IDEや類似の環境でこのコードをM5StickC Plusにアップロードします。

## 使い方

1. M5StickC Plusを起動します。
2. 起動後、M5StickC PlusのLCDにセンサ名、距離、速度が表示されます。
3. スマートフォン等でPhyphoxアプリを起動し、新規実験追加からBluetoothを選択すると、画面と同じ名前のデバイスが見つかるので、選択してください。

## 注意事項

- センサからの距離の読み取り範囲は、3000mmを超える場合無効とされます。
- 距離の読み取りは移動平均フィルターを使用して滑らかにされています。


[![](https://img.youtube.com/vi/RDGyMiHUAPI/0.jpg)](https://www.youtube.com/watch?v=RDGyMiHUAPI)




##  使用ライブラリ一覧

1. **staacks/phyphox-BLE@^1.2.3**
   - **概要**: Phyphox（物理実験を支援するアプリ）のBLE通信をサポートするライブラリ。
   - **用途**: Phyphoxアプリを通じてセンサーデータを取得し、BLE（Bluetooth Low Energy）でデバイスと通信するために使用。
   - **バージョン**: ^1.2.3

2. **m5stack/M5StickCPlus@^0.1.0**
   - **概要**: M5StackのM5StickC Plusデバイスを操作するための公式ライブラリ。
   - **用途**: ディスプレイ描画や内蔵センサー（ボタン、IMUなど）の制御を行うために使用。
   - **バージョン**: ^0.1.0

3. **pololu/VL53L0X@^1.3.1**
   - **概要**: STMicroelectronics製のVL53L0X ToF（Time of Flight）距離センサー用のライブラリ。
   - **用途**: 距離計測や障害物検出などの機能を実装するために使用。
   - **バージョン**: ^1.3.1

## Visual Studio Codeのplatform io拡張機能で書き込む方法
[参考サイト](https://stprec.co.jp/products/m5stickcplus-%E3%81%A7-hello-world/)