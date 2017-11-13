
//LIBRARIES
#include <DualMC33926MotorShield.h>
#include <Triskar.h>
#include <Encoder.h>
#include <NewPing.h>
#include <Filters.h>
#include <FastRunningMedian.h>
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>
#include <DFRobotDFPlayerMini.h>
#include <ArduinoSort.h>


#define WAIT_BT_CONN 60000
unsigned long int firstSoundTime = 0;
#define TIME_TO_SETUP 5000

// MULTITHREADING
#define PING_INTERVAL 33 //71 con capacitivi // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

//BLUETOOTH
//buttons
boolean triangolo;
boolean quadrato;
boolean cerchio;
boolean croce;
boolean startbutton;
boolean select;
//boolean noneB=true;
char b = ' ';
//interpreter states
enum btStates {choose_modality, fam_modality, choose_game,choose_scenario, sg_waiting, game_modality, test_modality, discharge};
btStates interpreterState = choose_modality;
boolean btMov = false;

//HARDWARE TESTS
enum testStates { tests_descr, choose_test, start_test, test_exe};
testStates testState = tests_descr;

enum testTypes { no_one, sonar_t, head_capacitives_t, body_capacitives_t, fotores_t, speaker_t, micro_t};
testTypes testType = no_one;


//FOTORESISTOR
#define FOTORES_PIN A2
int fotores_value;

//VOLTAGE CHECKER
int battery_indicator=0;
#define MIN_INDICATOR_VOLTAGE 11.0f
#define MAX_INDICATOR_VOLTAGE 12.5f
#define MIN_INDICATOR_VALUE 0.0f
#define MAX_INDICATOR_VALUE 5.0f
#define BATTERY_UPDATE_TIME 20000
unsigned long int lastBatteryUpdate=0;
#define NUM_SAMPLES 10
#define VOLT_CHECKER_PIN A4
int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage
unsigned long int lastWarning = 0; //last time warning advice

//SOUND VARIABLES
#define AUDIO_SCARED_ROUND 10
#define AUDIO_SCARED_BEHIND 11
#define DONT_WONNA_AUDIO 22
#define DOMANDA_COLORE_LED_AUDIO 10 
#define AVVICINATI_AUDIO 33
#define MiChiamoTeo_AUDIO 01
#define firstGameAudioNumber 3
#define firstScenarioAudioNumber 6
boolean firstSound = false;
boolean speakers = true;
boolean bodyCapacitor=true;
boolean fotoresistor=true;
unsigned long int startPlayTime = 0;
int lastPlayed = 0;
DFRobotDFPlayerMini myDFPlayer;
#define BUSY_PIN 5

//GAME VARIABLES
#define N_GAMES 3
enum gameStates {no_game, setting, make_question, wait_answer, right_answer, wrong_answer, end_game, mov};
gameStates gameState = no_game;
int currentGameI=0;
int currentScenarioI=0;

//0-blu 1-giallo 2-verde 3-rosso
unsigned long int startWaitingTime = 0;

//CAPACITIVES
#define N_HEAD_SENSORS 4
#define N_BODY_SENSORS 3
#define RESET_PAT_TIME 5000
#define RESET_HIT_TIME 20000
#define RESET_HUG_TIME 10000
#define N_PATS 3
#define N_HITS 3
int abbraccioN = 0;
enum warKingsCapacitives {noOne, head, body, both, body_t};
warKingsCapacitives workingCapacitives = body;
warKingsCapacitives previousWorkingCapacitives = noOne;
enum touched_parts {noWhere, leftT, rightT, frontT};
touched_parts touched = noWhere;
enum touch_types {nothing, hugT, patT, hitT};
touch_types touch_type = nothing;
int pressedButton = -1;
int buttonToTouch=-1;
boolean headInterpreter = false;
int pat[N_BODY_SENSORS] = {0, 0, 0};
int hit[N_BODY_SENSORS+1] = {0, 0, 0, 0};
int pats = 0;
int hits = 0;
int hugsCount = 0;
unsigned long int lastPatTime[N_BODY_SENSORS]={0,0,0};
unsigned long int lastHitTime[N_BODY_SENSORS]={0,0,0};
unsigned long int lastHugRecivedTime=0;


//FRONT LEDS PINS, CONSTANT AND VARIABLES
#define FRONT_LEDPIN 9
#define FRONT_LED_NUMBER 9
Adafruit_NeoPixel head_strip = Adafruit_NeoPixel(FRONT_LED_NUMBER, FRONT_LEDPIN, NEO_GRB + NEO_KHZ800);
//states
enum colors {redC, greenC, blueC, yellowC, lightBlueC, orangeC};
colors body_color = greenC;
enum ledStates {led_off, rainbow_cycle, color_wipe, color_pulse};
boolean body_leds=true;
boolean head_leds=true;
ledStates led_state = rainbow_cycle;
ledStates body_led_state = led_off;
uint32_t head_color;
const uint32_t blue = head_strip.Color(0, 0, 255);
const uint32_t red = head_strip.Color(255, 0, 0);
const uint32_t green = head_strip.Color(0, 255, 0);
const uint32_t light_blue = head_strip.Color(0, 255, 255);
const uint32_t violet = head_strip.Color(255, 0, 255);
const uint32_t yellow = head_strip.Color(255, 170, 0);
const uint32_t orange = head_strip.Color(255, 100, 0);

//MICRO PINS, CONSTANT AND VARIABLES
boolean micro = true;
#define soundPin  A3 //sound sensor attach to A11
#define microISequence 250
#define microISequenceShortMin 20
#define microISequenceShortMax 100
#define microSoglia 250.00f
int microI = 0;
float microFilterFrequency = 1.0;
float micro_f=0.0;
FilterOnePole microLowpassFilter( LOWPASS, microFilterFrequency );
unsigned long int lastadd = 0;
unsigned long int movementFinishTime = TIME_TO_SETUP + 1000;

//MOTORS AND ENCODERS PINS AND OBJECTS
//-------SHIELD 1-------
#define nD2m1 10
#define nSFm1 11
//--------Motor 1-----
#define M1DIR 46
#define M1PWM 12
#define M1FB  A6
//-------SHIELD 2-------
#define nD2m23 48
#define nSFm23 52
//--------Motor 2-----
#define M2DIR 50
#define M2PWM 4 //17
#define M2FB A7
//--------Motor 3-----
#define M3DIR 40
#define M3PWM 13 //16
#define M3FB A5

//-------ENCODER 1-------
#define EncA1 2
#define EncB1 3
//-------ENCODER 2-------
#define EncA2 18
#define EncB2 19
//-------ENCODER 3-------
#define EncA3 20
#define EncB3 21

Encoder posReader1(EncA1, EncB1), posReader2(EncA2, EncB2), posReader3(EncA3, EncB3);
DualMC33926MotorShield m1(M1DIR, M1PWM, nD2m1, nSFm1);
DualMC33926MotorShield m2m3(M2DIR, M2PWM, M3DIR, M3PWM, nD2m23, nSFm23);
Triskar triskar(m1, m2m3, posReader1, posReader2, posReader3);

//PRINT-TIMING VARIABLE
unsigned long lastMilliPrint = 0;

// MOVEMENTS VARIABLES
boolean move = false;
#define no_movement         0
#define make_eight          1
#define turn180r            2  //rotazione 180 gradi USANDO RUOTA DESTRA COME CENTRO DI ROTAZIONE
#define turn180l            3  //rotazione 180 gradi USANDO RUOTA SINISTRA COME CENTRO DI ROTAZIONE
#define turnAlphaR          4  //rotazione di alpha(variabile globale) gradi a destra USANDO IL CENTRO DEL ROBOT COME CENTRO DI ROTAZIONE
#define turnAlphaL          5
#define makeOnemF           6
#define makeOnemB           7
#define make_circle         8
#define scared_round        9
#define dontwonna           10
#define scared_behind       11
#define make_happy0         12
#define make_happy1         13
#define make_happy2         14
#define make_happy3         15
#define make_sad0           16
#define make_sad1           17
#define scared_hit          18
#define make_sad2           19
#define make_sad2L          20
#define make_sad2R          21
#define scared_hitL         22
#define scared_hitR         23
#define angrymov            24
#define dance_mov           25
#define idle                26
#define follow              27
#define autonomous_movement 28
#define brokeIce            29
#define autonomous_capa     30
#define colorGame           31

double alpha = 0;
byte next_movement = make_circle;
byte actual_movement = no_movement;
byte prev_movement = no_movement;
boolean follow2 = false;
boolean aut_mov = false;
boolean idle_mov = false;
int movementI = 0;
unsigned long int randomTurnTime = 15000 + rand() % (20000);
unsigned long int randomIdleTurnTime = 3000 + rand() % (7000);
unsigned long int lastObstacleTime = 0;
int obstacleCount = 0;
int dir = rand() % 2;

long int movStartTime = 0;
double startPosX = 0;
double startPosY = 0;
double startPosTh = 0;

int speed_trg = 30;


//---------SONARS---------
//-----constants-----
#define SONAR_NUM     4 // Number or sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define FAR_DISTANCE 100.0f
#define CLOSE_DISTANCE 60.0f
#define VERYCLOSE_DISTANCE 45.0f
#define MEDIAN_NUMBER 7
#define FILTERFREQUENCY 1.0f        //PB filter frequency
#define COUNTER 50                  //when targetPos reach COUNTER(right) or -COUNTER(left) the robot is sure about where obstacle is(that's for avoid sonars false readings)
#define MAX_COUNTER 2*COUNTER             //maximum COUNTER value
//-----variables-----
unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

float f_front = 400;                //filtered values
float f_right = 400;
float f_left = 400;
float f_back = 400;

int backI = 0;
int targetPos = 0;                  //variable for suppose where the person to follow/obstacle is. Positive is right, negative is left.
float previous_distance = 400;
float actual_distance = 400;        //viariable for store the distance of the closest object saw by sonars
enum obstacle {left, right, front, none};
obstacle last_obstacle = none;      //variable to define the last sonar that saw an obstacle
obstacle actual_obstacle = none;    //variable to define the sonar that is seeing the closest thing
//-----filters-----
FastRunningMedian<unsigned int, MEDIAN_NUMBER, 400> frontMedian, rightMedian, leftMedian, backMedian;
FilterOnePole front_sonar_f( LOWPASS, FILTERFREQUENCY );
FilterOnePole left_sonar_f( LOWPASS, FILTERFREQUENCY );
FilterOnePole right_sonar_f( LOWPASS, FILTERFREQUENCY );
FilterOnePole back_sonar_f( LOWPASS, FILTERFREQUENCY );
//-----objects-----
NewPing sonar[SONAR_NUM] = {      // Sensor object array.
  NewPing(24, 26, MAX_DISTANCE),  // front sonar
  NewPing(28, 30, MAX_DISTANCE),  //right sonar
  NewPing(32, 34, MAX_DISTANCE),  //left sonar
  NewPing(38, 36, MAX_DISTANCE)   //back sonar
};
//-----booleans-----
boolean front_obstacle = false;
boolean right_obstacle = false;
boolean left_obstacle = false;
boolean back_obstacle = false;
boolean close_front_obstacle = false;
boolean close_right_obstacle = false;
boolean close_left_obstacle = false;
boolean close_back_obstacle = false;
boolean veryclose_front_obstacle = false;
boolean veryclose_right_obstacle = false;
boolean veryclose_left_obstacle = false;
boolean veryclose_back_obstacle = false;
boolean no_obstacle = true;



void setup() {
  Serial.begin(38400);
  srand(millis());
  // HC-05 default serial speed for AT mode is 38400
  Serial3.begin(38400);
  dfPlayerSetup();
  bodyLedSetup();
  fotoresSetup();
  voltageCheckSetup();
  sonarSetup();
  headLedSetup();
}

void loop() {
  //FirstSound();
  //sensori
  btInterpreter();
  capacitiveSerialLoop();
  //headCapacitiveInterpreter();
  headCapacitiveLoop();
  voltageCheckloop();
  bodyLedLoop();
  sonarLoop();
  fotoresLoop();
  microLoop();
  headLedLoop();
  //print();
  //attuatori
  if (interpreterState != test_modality && interpreterState != choose_modality) {
    pidLoop();
    makeMovement();
    gameModality();
  }
}

void pidLoop() {
  if (move) {
    triskar.PIDLoop();
  }
}
#define switchToIdleTime 15000
void print()  {

  if ((millis() - lastMilliPrint) >= 50)   {
    lastMilliPrint = millis();


    /*    Serial.print("DONTWONNA:  ");
        Serial.println(movementI);
        Serial.print("MOV START TIME: ");
        Serial.println(movStartTime);
        Serial.print("START POS TH: ");
        Serial.println(startPosTh);
    */
    /*Serial.print("SP:  ");     Serial.print(speed_req[0]); Serial.print("    "); Serial.print(speed_req[1]); Serial.print("    "); Serial.println(speed_req[2]);
      Serial.print("RPM: ");     Serial.print(speed_act[0]); Serial.print("    "); Serial.print(speed_act[1]); Serial.print("    "); Serial.println(speed_act[2]);
      Serial.print("PWM: ");     Serial.print(PWM_val[0]);  Serial.print("   "); Serial.print(PWM_val[1]);   Serial.print("    "); Serial.println(PWM_val[2]);
      Serial.print("POS: ");   Serial.print(posReader1.read());Serial.print("    "); Serial.print(posReader2.read());Serial.print("    "); Serial.println(posReader3.read());
    */
    /*
      if(targetPos==0) Serial.println("FRONT");
      else if (targetPos<0) Serial.println("LEFT");
      else if (targetPos>0) Serial.println("RIGHT");
    */

    //    Serial.print("  TargetPos:  ");
    //    Serial.print(targetPos);
    //    Serial.print("  Actual Obstacle:  ");
    //    switch(actual_obstacle){
    //      case 0: Serial.print("left");break;
    //      case 1: Serial.print("right");break;
    //      case 2: Serial.print("front");break;
    //      case 3: Serial.print("none");break;
    //    }
    //    Serial.print("  Last Obstacle:  ");
    //    switch(last_obstacle){
    //      case 0: Serial.println("left");break;
    //      case 1: Serial.println("right");break;
    //      case 2: Serial.println("front");break;
    //      case 3: Serial.println("none");break;
    //    }


    /*    Serial.print("  BACK: ");
      Serial.println(cm[3]);
    */
    /*  Serial.print("triskar PosX  "); Serial.println(triskar.getPosX());
      Serial.print("triskar PosY "); Serial.println(triskar.getPosY());
      Serial.print("triskar PosTh "); Serial.println(triskar.getPosTh());

      Serial.print("triskar SpeedX  "); Serial.println(triskar.getSpeedX());
      Serial.print("triskar SpeedY "); Serial.println(triskar.getSpeedY());
      Serial.print("triskar SpeedTh "); Serial.println(triskar.getPosTh());
      /*
      Serial.println(voltage);

      Serial.println(fotores_value);

      /*Serial.print("millis()-movementFinishTime:  "); Serial.println(millis()-movementFinishTime);

      Serial.print("actual_movement  "); Serial.println(actual_movement);
      Serial.print("digitalRead(BUSY_PIN)  "); Serial.println(digitalRead(BUSY_PIN));
      Serial.print("gameState  "); Serial.println(gameState);
      Serial.print("triskar.isStopped()  "); Serial.println(triskar.isStopped());*/
    /*    Serial.print("micro val: "); Serial.println(microLowpassFilter.output());
        Serial.print("microI: "); Serial.println(microI);


      /*  Serial.print("actual_movement:  "); Serial.println(actual_movement);
      Serial.print("myDFPlayer.available():  "); Serial.println(myDFPlayer.available());
      /* Serial.print("gameState:  "); Serial.println(gameState);


      //Serial.print("led val: "); Serial.println(ledI);*/
    /*Serial.print("obstacleCount: "); Serial.print(obstacleCount);
      //Serial.print("  millis: "); Serial.print(millis());
      Serial.print("  millis - mov Start Time: "); Serial.print(millis()-movStartTime);
      Serial.print("  rand turn Time: "); Serial.print(randomTurnTime);
      Serial.print("lastObstacleTime"); Serial.println(lastObstacleTime);
      //millis()-movStartTime>=randomTurnTime || movementI>=5
      /*Serial.print("movement I2: "); Serial.println(movementI2);

      Serial.print("actual pos: "); Serial.println(triskar.getPosTh());
      /*Serial.print("target pos: "); Serial.println(startPosTh+PI/12.0);

      Serial.print(gameState);
      /*
      Serial3.print("left: ");Serial3.print(bodySensorValue[0]);
      /*if(bodySensorValue[0]>maxSensor)
      maxSensor=bodySensorValue[0];
      Serial3.print("   max: ");Serial3.println(maxSensor);
      Serial3.print("    right: ");Serial3.print(bodySensorValue[1]);
      Serial3.print("    behind: ");Serial3.print(bodySensorValue[2]);Serial3.print("    front: ");Serial3.print(bodySensorValue[3]);
      Serial3.print("    Pressed Button: "); Serial3.println(pressedButton);

      Serial3.print(stateStartTime[1] - previousStateStartTime[1]);Serial3.print("    ");
      Serial3.print(bodySensorValue[1]); Serial3.print("    ");Serial3.print(capacitiveState[1]); Serial3.print("    ");
      Serial3.print(previousDynamicCapacitiveState[1]); Serial3.print("    "); Serial3.print(previousCapacitiveState[1]); Serial3.print("    ");
      Serial3.print("    ");*/
    /*
          Serial3.print(bodySensorValue[0]); Serial3.print("    "); Serial3.print(bodySensorValue[1]); Serial3.print("    ");
        Serial3.print(bodySensorValue[2]); Serial3.print("    "); Serial3.println(pressedButton);

        Serial3.print(headSensorValue[0]); Serial3.print("    "); Serial3.print(headSensorValue[1]); Serial3.print("    ");
        Serial3.print(headSensorValue[2]); Serial3.print("    "); Serial3.print(headSensorValue[3]); Serial3.print("    "); Serial3.println(pressedButton);
      /*
       Serial3.print(bodySensorValue[0]); Serial3.print("    "); Serial3.print(capacitiveState[0]);Serial3.print("    ");
       Serial3.print(bodySensorValue[1]); Serial3.print("    "); Serial3.print(capacitiveState[1]);Serial3.print("    ");
       Serial3.print(bodySensorValue[2]); Serial3.print("    "); Serial3.print(capacitiveState[2]);Serial3.print("    ");
       Serial3.println(touchState);

      /*  Serial3.print(f_right);
        Serial3.print(" ");
        Serial3.print(f_front);
        Serial3.print(" ");
        Serial3.print(f_left);
        Serial3.print(" ");
        Serial3.println(f_back);


      /* Serial3.print("PosX:  "); Serial3.print(triskar.getPosX());
       Serial3.print(" PosY:  "); Serial3.print(triskar.getPosY());
       Serial3.print(" PosTh:  "); Serial3.print(triskar.getPosTh());
       Serial3.print(" SpeedX:  "); Serial3.print(triskar.getSpeedX());
       Serial3.print(" SpeedY:  "); Serial3.print(triskar.getSpeedY());
       Serial3.print(" SpeedTh:  "); Serial3.println(triskar.getSpeedTh());

        /*
          Serial3.print(millis() - stateStartTime[0]); Serial3.print("    ");
          Serial3.print(millis() - stateStartTime[1]); Serial3.print("    ");
          Serial3.print(millis() - stateStartTime[2]);Serial3.print("    ");
          Serial3.println(bodySensorValue[2]);

          /*
            Serial.print("Actual_movement:   "); Serial.print(actual_movement); Serial.print("  prec_movement:   "); Serial.println(prec_movement);
    */


    /*Serial.print(millis()); Serial.print(",");
      Serial.print(speedX); Serial.print(",");
      Serial.print(speedY); Serial.print(",");
      Serial.print(speedTh); Serial.print(",");*/

    /*Serial3.print(millis()); Serial3.print(",");
      Serial3.print(speedX); Serial3.print(",");
      Serial3.print(speedY); Serial3.print(",");
      Serial3.print(speedTh); Serial3.print(",");*/

    /*
        Serial.print(posX); Serial.print(",");
        Serial.print(posY); Serial.print(",");
        Serial.print(posTh);Serial.print(",");
        Serial.println(k);
    */

    /*Serial3.print(posX); Serial3.print(",");
      Serial3.print(posY); Serial3.print(",");
      Serial3.print(posTh);Serial3.print(",");
      Serial3.println(k);*/
  }
}




