#include <Servo.h>
#include <Timer.h>

//PIN-OUT
#define CORRECAO_ESQUERDO   11
#define CORRECAO_DIREITO    15

#define ACCEL_X             8
#define ACCEL_Y             9
#define ACCEL_Z             10

#define M_DIREITO           13
#define M_ESQUERDO          12

#define SELF_TEST_PIN       46        
#define SLAVE_SELECT_PIN    50    
#define G_SELECT_PIN        44

#define S_FRONTAL_ESQ       22
#define S_FRONTAL_DIR       24
#define S_TRASEIRO_ESQ      28
#define S_TRASEIRO_DIR      32

#define S_FRONTAL           34
#define S_TRASEIRO          30

#define SERVO_MIN           18
#define SERVO_MAX           158

#define ACCEL_MIN           340
#define ACCEL_MAX           380

#define SENSOR_BORDA_ATIVO  1
#define SENSOR_ATIVO        1

#define TEMPO_LUTA          360
#define TEMPO_BLOQUEIO      7

//Instances
Servo MotorDireito;
Servo MotorEsquerdo;

Timer t;

//Variables
int pos = 0;    
int valDirecao = 30;
int valAceleracao = 50;

int bloqueiaSensor =0;
int girando = 0;
int tempoTotal = 0;
int bloqueio = 1;

int xMin,yMin=1023;
int x,y,xMax,yMax=0;

void move_robo(int motorDireito, int motorEsquerdo,int bloqueio);
void exibeControleMotores();
void exibeSensores();
void testaBordas(int bloqueio);

void trataTimer()
{
  static int tempoTotal=0;
  tempoTotal++;
  if(tempoTotal > TEMPO_LUTA)
  {
      bloqueio = 1;
  }
}

void setup() 
{
  MotorEsquerdo.attach(M_ESQUERDO);
  MotorDireito.attach(M_DIREITO);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(S_FRONTAL_ESQ,INPUT);
  pinMode(S_FRONTAL_DIR,INPUT);
  pinMode(S_TRASEIRO_ESQ,INPUT);
  pinMode(S_TRASEIRO_DIR,INPUT);

  pinMode(S_FRONTAL,INPUT);
  pinMode(S_TRASEIRO,INPUT);

  pinMode(SELF_TEST_PIN, OUTPUT);
  pinMode(SLAVE_SELECT_PIN,OUTPUT);
  pinMode(G_SELECT_PIN,OUTPUT);
  
  digitalWrite(SELF_TEST_PIN, LOW);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
  digitalWrite(G_SELECT_PIN, LOW);

  int timerPrincipal = t.every(1000, trataTimer,0);
}

void loop() 
  {
    t.update();
    
    static int motorDireito = 0;
    static int motorEsquerdo = 0;
    static int bloqueio = 0;

    if(digitalRead(S_FRONTAL) == 0)
    {
      motorDireito =80;
      motorEsquerdo =80;
    }
     else if(digitalRead(S_TRASEIRO) == 1)
    {
     motorDireito =-50;
     motorEsquerdo =-50;
    }
    else
   {
      motorDireito = 30;
      motorEsquerdo = 30;
   }

   move_robo(motorDireito, motorEsquerdo, bloqueio);
   testaBordas(bloqueio);
  }

    
    
//    exibeSensores();
//    exibeControleMotores();

void move_robo(int motorDireito, int motorEsquerdo,int bloqueio)
{
    int correcaoEsq,correcaoDir;
    int posicaoEsq,posicaoDir;

    correcaoEsq = map(analogRead(CORRECAO_ESQUERDO), 0, 1023, -100, 100);     // scale it to use it with the servo (value between 0 and 180)
    correcaoDir = map(analogRead(CORRECAO_DIREITO), 0, 1023, -100, 100);     // scale it to use it with the servo (value between 0 and 180)

    posicaoEsq = motorEsquerdo + correcaoEsq;
    posicaoDir = motorDireito + correcaoDir;

    if (posicaoEsq > 100) posicaoEsq = 100;
    if (posicaoEsq <-100) posicaoEsq = -100;

    if (posicaoDir > 100) posicaoDir = 100;
    if (posicaoDir <-100) posicaoDir = -100;

    if(!bloqueio)
    {
        posicaoEsq = map(posicaoEsq, -100, 100, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
        posicaoDir = map(posicaoDir, -100, 100, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    }else
    {
        posicaoEsq = map(0, -100, 100, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
        posicaoDir = map(0, -100, 100, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    }

    MotorDireito.write(posicaoDir);
    MotorEsquerdo.write(posicaoEsq);
    delay(10);
}

void testaBordas(int bloqueio)
{
    if((digitalRead(S_FRONTAL_ESQ) == SENSOR_BORDA_ATIVO)||(digitalRead(S_FRONTAL_DIR) == SENSOR_BORDA_ATIVO))
    {
      move_robo(-50, -50, bloqueio);
      delay(50);    
    }

    if((digitalRead(S_TRASEIRO_ESQ) == SENSOR_BORDA_ATIVO)||(digitalRead(S_TRASEIRO_DIR) == SENSOR_BORDA_ATIVO))
    {
      move_robo(50, 50, bloqueio);
      delay(100);    
    }
}


void exibeSensores()
{
    Serial.print(digitalRead(S_FRONTAL_ESQ));
    Serial.print("\t");
    Serial.print(digitalRead(S_FRONTAL_DIR));
    Serial.print("\t");
    Serial.print(digitalRead(S_TRASEIRO_ESQ));
    Serial.print("\t");
    Serial.print(digitalRead(S_TRASEIRO_DIR));
    Serial.println();
}
