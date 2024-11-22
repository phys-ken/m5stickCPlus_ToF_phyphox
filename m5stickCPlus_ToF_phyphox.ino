// 必要なライブラリをインクルード
#include <M5StickCPlus.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <phyphoxBle.h>

// VL53L0Xセンサのインスタンスを生成
VL53L0X sensor;

// TFT_eSpriteのインスタンスを生成してスプライトの初期設定
TFT_eSprite sprite = TFT_eSprite(&M5.Lcd);

// 移動平均フィルターの設定パラメータと変数
const int filterSize = 10;
float filterBuffer[filterSize];
int filterIndex = 0;
float preDist = 0;

// 前回の距離を保持するための変数
float lastDist = 0.0;

// 前回の計測時間を保持するための変数
unsigned long lastTime = 0;

// センサの名前を保持するための変数
const char* sensorName = "phys_ken_001";

// 内蔵LEDの状態を記録する変数。trueのときLEDは消灯状態、falseのとき点灯状態を表す。
bool ledState = true;

// ボタンが押されたときのタイムスタンプ（ミリ秒単位）を記録する変数
unsigned long startTime = 0;

void setup() {
  M5.begin();  // M5StickCを初期化

  pinMode(10, OUTPUT);    // 内蔵LEDが接続されているGPIO10ピンを出力モードに設定
  digitalWrite(10, HIGH); // 内蔵LEDを初期状態で消灯する（HIGHで消灯）

  // LCDの初期設定
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  Serial.begin(115200);                  // シリアル通信の開始
  Serial.println("VLX53LOX  started.");  // 開始のメッセージをシリアル出力

  // VL53L0Xセンサの初期設定
  Wire.begin(0, 26, 100000);
  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}  // センサの初期化失敗時はここでループ
  }
  sensor.startContinuous();  // センサの連続計測モードを開始

  // PhyphoxBLEの設定と初期化
  PhyphoxBLE::start(sensorName);

  // Phyphoxでの実験の設定
  PhyphoxBleExperiment experiment;
  experiment.setTitle(sensorName);
  experiment.setCategory(sensorName);
  experiment.setDescription("Plot the distance from a time-of-flight sensor over time.");

  // 距離表示用のグラフの設定
  PhyphoxBleExperiment::View view;
  PhyphoxBleExperiment::Graph graph;
  graph.setLabel("x-tグラフ");
  graph.setUnitX("s");
  graph.setUnitY("m");
  graph.setLabelX("時刻");
  graph.setLabelY("距離");
  graph.setChannel(0, 1);

  // 速度表示用のグラフの設定
  PhyphoxBleExperiment::Graph speedGraph;
  speedGraph.setLabel("v-tグラフ");
  speedGraph.setUnitX("s");
  speedGraph.setUnitY("m/s");
  speedGraph.setLabelX("時刻");
  speedGraph.setLabelY("速度");
  speedGraph.setChannel(0, 2);

  // グラフをビューに追加し、ビューを実験に追加
  view.addElement(graph);
  view.addElement(speedGraph);
  experiment.addView(view);
  PhyphoxBLE::addExperiment(experiment);

  // スプライトの設定
  sprite.setColorDepth(8);
  sprite.setTextSize(2);
  sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
}

void loop() {
  M5.update(); // ボタンの状態を更新

  // ボタンAが押された場合
  if (M5.BtnA.wasPressed()) {
    ledState = !ledState; // LEDの状態をトグル
    if (!ledState) {
      startTime = millis();   // ボタンを押した時刻を記録
      digitalWrite(10, LOW);  // LEDを点灯（LOWで点灯）
      Serial.println("<---!!start!!--->"); // シリアル通信に開始メッセージを送信
      Serial.println("---sec , m---");     // 見出し行を表示
    } else {
      digitalWrite(10, HIGH); // LEDを消灯（HIGHで消灯）
      Serial.println("<---!!stop!!--->");  // シリアル通信に停止メッセージを送信
    }
  }

  int distance = sensor.readRangeSingleMillimeters();  // センサから距離を読み取る

  // 無効な距離のエラーチェック
  if (distance > 3000) {
    distance = preDist;
  }

  // 移動平均フィルターの処理
  filterBuffer[filterIndex] = distance;
  filterIndex = (filterIndex + 1) % filterSize;
  float filteredDist = 0.0;
  for (int i = 0; i < filterSize; i++) {
    filteredDist += filterBuffer[i];
  }
  filteredDist /= filterSize;
  preDist = filteredDist;

  // 経過時間の計算
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;  // lastTimeの更新をここに移動

  // 速度を計算
  float speed = (filteredDist - lastDist) / deltaTime;
  lastDist = filteredDist;

  // 距離と速度をメートルに変換
  filteredDist = filteredDist / 1000.0;
  speed = speed / 1000.0;

  // Phyphoxに計測結果を送信
  PhyphoxBLE::write(filteredDist, speed);

  // LCDに計測結果を表示
  sprite.fillRect(0, 0, sprite.width(), sprite.height(), BLACK);
  sprite.setCursor(0, 0);
  sprite.setTextSize(3);
  sprite.setTextColor(TFT_YELLOW);
  sprite.printf("%s\n", sensorName);  // センサ名

  sprite.setCursor(0, 40);
  sprite.setTextSize(2);
  sprite.setTextColor(TFT_WHITE);
  sprite.printf("x = %.3f m\n", filteredDist);  // 距離
  sprite.printf("v = %.3f m/s\n", speed);       // 速度

  // スプライトを実際のLCDに描画
  sprite.pushSprite(0, 0);

  // LEDが点灯している場合のみシリアル送信を行う
  if (!ledState) {
    // ボタンを押してからの経過時間を秒単位で計算
    float timeDifference = (currentTime - startTime) / 1000.0;
    // シリアル送信（時間と距離をカンマ区切りで表示、単位は省略）
    Serial.print(timeDifference, 3);   // 経過時間（秒）、小数点以下3桁まで表示
    Serial.print(",");                 // カンマ区切り
    Serial.println(filteredDist, 3);   // 距離（メートル）、小数点以下3桁まで表示
  }

  delay(10);  // 10ms待機
}
