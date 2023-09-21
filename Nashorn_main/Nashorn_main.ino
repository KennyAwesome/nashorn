// #define ARDUINOOSC_DEBUGLOG_ENABLE

// Please include ArduinoOSCWiFi.h to use ArduinoOSC on the platform
// which can use both WiFi and Ethernet
#include <ArduinoOSCWiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
Adafruit_MPU6050 mpu;
int scl = 12;
int sda = 11;
// this is also valid for other platforms which can use only WiFi
// #include <ArduinoOSC.h>

// WiFi stuff
const char* ssid = "Nashorn";
const char* pwd = "113eeac34f";
const IPAddress ip(192, 168, 0, 100);
const IPAddress gateway(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

// for ArduinoOSC
const char* host = "192.168.0.101";
const int recv_port = 9999;
const int bind_port = 9998; //?
const int send_port = 9998; //?
const int publish_port = 9998;
// send / receive varibales
int mid;
int back;
int val;
float f;
String s;
bool blinkState = true;
int vibratorPin = 16;
//int tipPin = 11;
int midPin = 5; // 23180 min / 24000 trigger
int backPin = 3;
/*
int midPin = 5;
int stemPin = ;
int backPin = 3;
*/

int midThreshold = 24000;
int midMax = 24500;
int backThreshold = 19000;
int backMax = 24000;
double ax,ay,az,gx, gy, gz;
void onOscReceived(const OscMessage& m) {
    Serial.print(m.remoteIP());
    Serial.print(" ");
    Serial.print(m.remotePort());
    Serial.print(" ");
    Serial.print(m.size());
    Serial.print(" ");
    Serial.print(m.address());
    Serial.print(" ");
    Serial.print(m.arg<int>(0));
    Serial.print(" ");
    Serial.print(m.arg<float>(1));
    Serial.print(" ");
    Serial.print(m.arg<String>(2));
    Serial.println();
    blinkState = !blinkState;
    digitalWrite(LED_BUILTIN, blinkState);
    //val = m.arg<int>(0);
    val = m.size();
    if(val > 0){
      analogWrite(vibratorPin, val);
    } else {
      analogWrite(vibratorPin, 0);
    }
    
    
}

void setup() {
  Wire.begin(scl, sda);
  pinMode(vibratorPin, OUTPUT);
    Serial.begin(115200);
    delay(2000);

    // WiFi stuff (no timeout setting for WiFi)
#ifdef ESP_PLATFORM
    WiFi.disconnect(true, true);  // disable wifi, erase ap info
    delay(1000);
    WiFi.mode(WIFI_STA);
#endif
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
     pinMode(LED_BUILTIN, OUTPUT);
    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(50);
      blinkState = !blinkState;
      digitalWrite(LED_BUILTIN, blinkState);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  }
  /*
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
        blinkState = !blinkState;
      digitalWrite(LED_BUILTIN, blinkState);
    }
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.localIP());
    
    OscWiFi.publish(host, publish_port, "/touch/mid", mid)
        ->setFrameRate(10.f);
    OscWiFi.publish(host, publish_port, "/touch/back", back)
        ->setFrameRate(10.f);
    OscWiFi.publish(host, publish_port, "/mpu/ax", ax)//pitch
        ->setFrameRate(50.f);
    OscWiFi.publish(host, publish_port, "/mpu/ay", ay)//dunno
        ->setFrameRate(50.f);
    OscWiFi.publish(host, publish_port, "/mpu/az", az)//roll
        ->setFrameRate(50.f);
    OscWiFi.publish(host, publish_port, "/mpu/gx", gx)//
        ->setFrameRate(50.f);
    OscWiFi.publish(host, publish_port, "/mpu/gy", gy)//
        ->setFrameRate(50.f);
    OscWiFi.publish(host, publish_port, "/mpu/gz", gz)//
        ->setFrameRate(50.f);
    
     OscWiFi.publish(host, publish_port, "/val", val)//
        ->setFrameRate(50.f);
    // publish osc messages (default publish rate = 30 [Hz])
    //OscWiFi.subscribe(recv_port, "/vibrator", onOscReceived);

    OscWiFi.subscribe(recv_port, "/vibrator", [](const OscMessage& m) {

        if(val < 255){
          //val++;
          val = m.size();
          if(val > 0){
            analogWrite(vibratorPin, val);
          } else {
            analogWrite(vibratorPin, 0);
          }
        }

        OscWiFi.send(host, send_port, "/reply", val);
    });
    */
}

void loop() {
    //OscWiFi.update();  // should be called to receive + send osc
    mid = touchRead(midPin);
    int midMap = map(mid, midThreshold, midMax, 0, 255 );
    if(mid > midThreshold){
      analogWrite(vibratorPin, midMap);
      delay(100);
      analogWrite(vibratorPin, 0);
      delay(100);
    } else {
      analogWrite(vibratorPin, 0);
    }
    back = touchRead(backPin);
    int backMap = map(back, backThreshold, backMax, 0, 255 );
    if(back > backThreshold){
      analogWrite(vibratorPin, backMap);
    } else {
      analogWrite(vibratorPin, 0);
    }

    sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  ax = a.acceleration.x;
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  ay = a.acceleration.y;
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  az = a.acceleration.z;
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  gx = g.gyro.x;
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  gy = g.gyro.y;
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  gz = g.gyro.z;
  Serial.println(" rad/s");
    // or do that separately
    // OscWiFi.parse(); // to receive osc
    // OscWiFi.post(); // to publish osc
}
