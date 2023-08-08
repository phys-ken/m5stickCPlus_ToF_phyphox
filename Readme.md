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