#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(10, 11);
DFRobotDFPlayerMini player;

const int ledPin = 7;
bool played = false;

unsigned long lastPlayTime = 0;
const unsigned long playDelay = 3000; // 3 ثواني بين كل تشغيل

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  player.begin(mySerial);
  player.volume(30); // أعلى صوت
}

void loop() {
  int s0 = analogRead(A0);
  int s1 = analogRead(A1);
  int s2 = analogRead(A2);
  int s3 = analogRead(A3);
  int s4 = analogRead(A4);

  Serial.print(s0); Serial.print(" | ");
  Serial.print(s1); Serial.print(" | ");
  Serial.print(s2); Serial.print(" | ");
  Serial.print(s3); Serial.print(" | ");
  Serial.println(s4);

  bool error =
    (s0 < 100 || s0 > 400) ||
    (s1 < 100 || s1 > 400) ||
    (s2 < 100 || s2 > 400) ||
    (s3 < 100 || s3 > 400) ||
    (s4 < 100 || s4 > 400);

  if (error) {
    digitalWrite(ledPin, HIGH);
    played = false;
  } else {
    digitalWrite(ledPin, LOW);

    bool thumbStraight = s0 > 200;
    bool thumbBent     = s0 < 170;

    bool indexStraight  = s1 > 250;
    bool middleStraight = s2 > 280;
    bool ringStraight   = s3 > 250;
    bool pinkyStraight  = s4 > 230;

    bool indexBent  = s1 < 190;
    bool middleBent = s2 < 200;
    bool ringBent   = s3 < 175;
    bool pinkyBent  = s4 < 180;

    bool gesture1 = thumbStraight && indexBent && middleBent && ringBent && pinkyBent;
    bool gesture2 = thumbBent && indexStraight && middleStraight && ringStraight && pinkyStraight;
    bool gesture3 = thumbBent && indexBent && middleBent && ringBent && pinkyBent;
    bool gesture4 = thumbBent && indexBent && middleStraight && ringStraight && pinkyStraight;
    bool gesture5 = indexStraight && thumbBent && middleBent && ringBent && pinkyBent;

    bool anyGesture = gesture1 || gesture2 || gesture3 || gesture4 || gesture5;

    if (anyGesture && !played && millis() - lastPlayTime >= playDelay) {
      if (gesture1) {
        player.play(1);
      }
      else if (gesture2) {
        player.play(2);
      }
      else if (gesture3) {
        player.play(3);
      }
      else if (gesture4) {
        player.play(4);
      }
      else if (gesture5) {
        player.play(5);
      }

      played = true;
      lastPlayTime = millis();
    }

    if (!anyGesture) {
      played = false;
    }
  }

  delay(300);
}