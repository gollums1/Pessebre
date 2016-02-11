//------------------------------
// Macros
//------------------------------
#define UPDATE_TIME_SKY 39  // 10 segons (aprox) / 255 (pwm)
#define FIRE_ON_MIN 200
#define FIRE_ON_MAX 255
#define FIRE_ON_T_MIN 100
#define FIRE_ON_T_MAX 500
#define FIRE_OFF_MIN 100
#define FIRE_OFF_MAX 155
#define FIRE_OFF_T_MIN 50
#define FIRE_OFF_T_MAX 100

//------------------------------
// Definicions structs/typedefs
//------------------------------
typedef struct {
  byte output;
  unsigned long time;
  byte pwm_value;
  bool fade;
} Sky;

typedef struct {
  byte output;
  byte pwm_value;
  unsigned long time;
  unsigned int delay;
  byte state;
} Fire;

//------------------------------
// Funcions
//------------------------------
void sky_function(Sky *sky);
void fire_function(Fire *fire);

//------------------------------
// Declaracions - Arduino MEGA ADK
//------------------------------
Sky sky_01 = {8, 0, 0x00, true};
Sky sky_02 = {9, 0, 0xFF, false};
Fire fire = {10, 255, 0, 0, 0};

void setup() {
  pinMode(sky_01.output, OUTPUT);
  pinMode(sky_02.output, OUTPUT);
  pinMode(fire.output, OUTPUT);

  randomSeed(analogRead(0));
}

void loop() {
  sky_function(&sky_01);
  sky_function(&sky_02);
  fire_function(&fire);
}

void sky_function(Sky *sky) {
  if ((millis() - sky->time) > UPDATE_TIME_SKY) {
    if (sky->fade) {
      if (sky->pwm_value < 255) {
        sky->pwm_value += 1;
      } else {
        sky->fade = false;
        sky->pwm_value = 255;
      }
    } else {
      if (sky->pwm_value > 0) {
        sky->pwm_value -= 1;
      } else {
        sky->fade = true;
        sky->pwm_value = 0;
      }
    }
    analogWrite(sky->output, sky->pwm_value);
    sky->time = millis();
  }
}

void fire_function(Fire *fire) {
  if ((millis() - fire->time) > fire->delay) {
    switch (fire->state) {
      case 0:
        fire->pwm_value = random(FIRE_ON_MIN, FIRE_ON_MAX);
        fire->delay = random(FIRE_ON_T_MIN, FIRE_ON_T_MAX);
        fire->state = 1;
        break;
      case 1:
        fire->pwm_value = random(FIRE_OFF_MIN, FIRE_OFF_MAX);
        fire->delay = random(FIRE_OFF_T_MIN, FIRE_OFF_T_MAX);
        fire->state = 0;
        break;
    }
    analogWrite(fire->output, fire->pwm_value);
    fire->time = millis();
  }
}

