#include <WiFi.h> 
#include <FirebaseESP32.h> 
const int sampleWindow = 50; 
unsigned int sample; 
#define SENSOR_PIN1 35 
#define SENSOR_PIN2 32 
#define trigPin1 13 
#define echoPin1 12 
#define trigPin2 14 
#define echoPin2 27 
#define SOUND_SPEED 0.034 
#define FIREBASE_HOST 
void setup() { 
pinMode(SENSOR_PIN1, INPUT); 
"https://esp32-330bd-default
rtdb.firebaseio.com/" 
#define FIREBASE_AUTH "RTvlDrIh7jA7urm6Q8d0SOxzpRYF9xrKVnWeQzop" 
const char* ssid = "tunarungu"; 
const char* password = "tunarungu"; 
long duration; 
int distanceCm1, distanceCm2; 
float kirimeter = distanceCm1/100; 
float kananmeter = distanceCm2/100; 
FirebaseData firebaseData; 
66 
 
 
 
 
  pinMode(SENSOR_PIN2, INPUT); 
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT); 
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT); 
  Serial.begin(115200); 
 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.println("Connecting to WiFi..."); 
    delay(1000); 
  } 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
} 
 
void loop() { 
  unsigned long startMillis = millis(); 
  float peakToPeak = 0; 
 
  unsigned int signalMax1 = 0; 
  unsigned int signalMin1 = 4095; 
  unsigned int signalMax2 = 0; 
  unsigned int signalMin2 = 4095; 
 
  while (millis() - startMillis < sampleWindow) { 
    sample = analogRead(SENSOR_PIN1); 
    if (sample < 4095) { 
      if (sample > signalMax1) { 
        signalMax1 = sample; 
      } else if (sample < signalMin1) { 
        signalMin1 = sample; 
      } 
    } 
 
    sample = analogRead(SENSOR_PIN2); 
    if (sample < 4095) { 
      if (sample > signalMax2) { 
        signalMax2 = sample; 
      } else if (sample < signalMin2) { 
67 
 
 
 
 
        signalMin2 = sample; 
      } 
    } 
  } 
 
  peakToPeak = signalMax1 - signalMin1; 
  int db1 = map(peakToPeak, 20, 900, 49.5, 90); 
 
  peakToPeak = signalMax2 - signalMin2; 
  int db2 = map(peakToPeak, 20, 900, 49.5, 90); 
 
  Serial.print("ADC Sensor 1: "); 
  Serial.println(analogRead(SENSOR_PIN1)); 
 
  Serial.print("ADC Sensor 2: "); 
  Serial.println(analogRead(SENSOR_PIN2)); 
 
  Serial.print("Loudness Sensor 1: "); 
  Serial.print(db1); 
  Serial.println("dB"); 
 
  Serial.print("Loudness Sensor 2: "); 
  Serial.print(db2); 
  Serial.println("dB"); 
 
  digitalWrite(trigPin1, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin1, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin1, LOW); 
  duration = pulseIn(echoPin1, HIGH); 
  distanceCm1 = duration * SOUND_SPEED / 2; 
 
  digitalWrite(trigPin2, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin2, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin2, LOW); 
  duration = pulseIn(echoPin2, HIGH); 
68 
 
 
 
 
  distanceCm2 = duration * SOUND_SPEED / 2; 
 
  Serial.print("Distance (cm) Sensor 1: "); 
  Serial.println(distanceCm1); 
 
  Serial.print("Distance (cm) Sensor 2: "); 
  Serial.println(distanceCm2); 
 
  kirimeter = distanceCm1/100.0; 
  kananmeter = distanceCm2/100.0; 
 
  Firebase.setInt(firebaseData, "kebisingankiri", db1); 
  Firebase.setInt(firebaseData, "kebisingankanan", db2); 
  Firebase.setInt(firebaseData, "jarakkiri", distanceCm1); 
  Firebase.setInt(firebaseData, "jarakkanan", distanceCm2); 
 
  String kirimeterString = String(kirimeter,1); 
  String kananmeterString = String(kananmeter,1); 
 
  Firebase.setString(firebaseData, "jarakkirimeter", 
kirimeterString); 
  Firebase.setString(firebaseData, "jarakkananmeter", 
kananmeterString); 
}