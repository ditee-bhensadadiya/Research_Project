#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

WebServer server(80);

// MOTOR PINS
#define AIN1 25
#define AIN2 26
#define BIN1 14
#define BIN2 12

#define PWMA 33
#define PWMB 32

// SPEED SETTINGS
int motorSpeed = 140;   // forward speed (reduced)
int turnSpeed  = 200;   // turning speed (reduced)

// TIMING (TUNE THIS)
int tileTime = 800;
int turnTime = 1800;

// PATH STORAGE
String actions[32];
int actionCount = 0;

// DIRECTION: 0=UP,1=RIGHT,2=DOWN,3=LEFT
int direction = 0;

// POSITION TRACKING (DEBUG)
int posX = 0;
int posY = 0;

// ---------------- MOTOR STOP ----------------
void stopCar() {
  ledcWrite(PWMA, 0);
  ledcWrite(PWMB, 0);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

// ---------------- FORWARD ----------------
void forward() {

  Serial.println("Forward");

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(PWMA, motorSpeed);
  ledcWrite(PWMB, motorSpeed);

  delay(tileTime);
  stopCar();

  // UPDATE POSITION
  if(direction == 0) posX--;
  if(direction == 1) posY++;
  if(direction == 2) posX++;
  if(direction == 3) posY--;

  Serial.print("Position: ");
  Serial.print(posX);
  Serial.print(",");
  Serial.println(posY);

  delay(300);
}

// ---------------- LEFT TURN ----------------
void leftTurn() {

  Serial.println("Left Turn");

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  ledcWrite(PWMA, turnSpeed);
  ledcWrite(PWMB, 0);   // pivot turn (very sharp)

  delay(turnTime);
  stopCar();

  delay(500);
}

// ---------------- RIGHT TURN ----------------
void rightTurn() {

  Serial.println("Right Turn");

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(PWMA, 0);          // ✅ change here
  ledcWrite(PWMB, turnSpeed);  // ✅ change here

  delay(turnTime);
  stopCar();

  delay(500);
}

// ---------------- ROTATION ----------------
void rotateLeft() {
  leftTurn();
  direction = (direction + 3) % 4;
}

void rotateRight() {
  rightTurn();
  direction = (direction + 1) % 4;
}

// ---------------- FACE DIRECTION ----------------
void faceDirection(int target) {

  while(direction != target) {

    int diff = (target - direction + 4) % 4;

    if(diff == 1) rotateRight();
    else if(diff == 3) rotateLeft();
    else if(diff == 2) {
      rotateRight();
      rotateRight();
    }
  }
}

// ---------------- RUN POLICY ----------------
void runPolicy() {

  Serial.println("Running Path");

  for(int i = 0; i < actionCount; i++) {

    String action = actions[i];

    Serial.print("Step ");
    Serial.print(i);
    Serial.print(" -> ");
    Serial.println(action);

    if(action == "GOAL") {
      Serial.println("Goal reached");
      stopCar();
      return;
    }

    if(action == "UP") {
      faceDirection(0);
      forward();
    }
    else if(action == "RIGHT") {
      faceDirection(1);
      forward();
    }
    else if(action == "DOWN") {
      faceDirection(2);
      forward();
    }
    else if(action == "LEFT") {
      faceDirection(3);
      forward();
    }

    delay(300);
  }
}

// ---------------- RECEIVE POLICY ----------------
void receivePolicy() {

  String body = server.arg("plain");
  Serial.println("Received JSON:");
  Serial.println(body);

  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, body);

  if(error) {
    Serial.println("JSON ERROR");
    server.send(400, "text/plain", "JSON ERROR");
    return;
  }

  // CLEAR OLD DATA
  for(int i = 0; i < 32; i++) actions[i] = "";
  actionCount = 0;

  // RESET STATE
  direction = 0;
  posX = 0;
  posY = 0;

  int tileSize = doc["tile_size"];
  tileTime = tileSize * 20;

  JsonArray path = doc["path"];

  for(String action : path) {
    actions[actionCount++] = action;
  }

  server.send(200, "text/plain", "OK");

  delay(1000);

  runPolicy();
}

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(115200);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  ledcAttach(PWMA, 5000, 8);
  ledcAttach(PWMB, 5000, 8);

  WiFi.softAP("RL_Robot");

  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());

  server.on("/policy", HTTP_POST, receivePolicy);

  server.begin();
}

// ---------------- LOOP ----------------
void loop() {
  server.handleClient();
}