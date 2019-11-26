#include <Servo.h>

//PIN-OUT
#define CORRECAO_DIRECAO    A11
#define CORRECAO_ACELERACAO A15

#define ACCEL_X             A8
#define ACCEL_Y             A9
#define ACCEL_Z             A10

#define DIRECAO             12
#define ACELERACAO          13

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
Servo acelerador;
Servo direcacao;

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

void move_robo(int dir, int ace);
void exibeControleMotores();
void exibeSensores();
void testaBordas();

void setup() 
{
  direcacao.attach(DIRECAO); //Direção
  acelerador.attach(ACELERACAO); //Aceleração
  Serial.begin(9600);

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

//  noInterrupts();           // disable all interrupts
//  TCCR1A = 0;
//  TCCR1B = 0;
//  TCNT1  = 0;
//
//  OCR1A = 31250;            // compare match register 16MHz/256/2Hz
//  TCCR1B |= (1 << WGM12);   // CTC mode
//  TCCR1B |= (1 << CS12);    // 256 prescaler 
//  TIMSK1 |= (1 << OCIE3A);  // enable timer compare interrupt
//  
//  interrupts();             // enable all interrupts
}

//ISR(TIMER1_COMPA_vect)      // timer compare interrupt service routine
void f1()
{
  tempoTotal++;
  bloqueiaSensor =0;

  if(tempoTotal > TEMPO_BLOQUEIO)
  {
      bloqueio = 0;
  }

  if((tempoTotal%3) == 0)
  {
    girando ^= 1;
  }

  if(tempoTotal > TEMPO_LUTA)
  {
      bloqueio = 1;
      move_robo(0,0);
      digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
      digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);   // toggle LED pin 
  }
}

void loop() 
{  
    if(digitalRead(S_FRONTAL) == 0)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      valDirecao *= -1;
      valAceleracao=50;
      move_robo(0,valAceleracao);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
        if(girando)
        {
//          move_robo(valDirecao,valAceleracao);
        }   
        else
        {
//          move_robo(0,valAceleracao);  
        }
    }
      
//    x= analogRead(ACCEL_X);
//    y= analogRead(ACCEL_Y);

//    testaBordas();
//    exibeSensores();
    exibeControleMotores();
}

void move_robo(int dir, int ace)
{
    int offSetDirecao = 0;
    int offSetAceleracao = 0;

    int tempDir = map(dir,-90,90, SERVO_MIN, SERVO_MAX);
    int tempAcel = map(ace,-100,100,SERVO_MIN, SERVO_MAX);
    
    offSetDirecao = map(analogRead(CORRECAO_DIRECAO), 0, 1023, SERVO_MIN, SERVO_MAX);
    offSetAceleracao = map(analogRead(CORRECAO_ACELERACAO), 0, 1023,SERVO_MIN, SERVO_MAX);
    
//    if(!bloqueio)
    {
        direcacao.write(tempDir + offSetDirecao);
        acelerador.write(tempAcel + offSetAceleracao);
//        direcacao.write(tempDir);
//        acelerador.write(tempAcel);
    }    
//    else
//    {
//        direcacao.write(offSetDirecao);
//        acelerador.write(offSetAceleracao);      
//    }
}

void testaBordas()
{
    static int ValorAnteriorFrente;
    static int ValorAnteriorTraseira;
  
    if((digitalRead(S_FRONTAL_ESQ) == SENSOR_BORDA_ATIVO)||(digitalRead(S_FRONTAL_DIR) == SENSOR_BORDA_ATIVO))
    {
        if(!bloqueiaSensor)
        {
            valAceleracao *= -1;
            bloqueiaSensor = 1;
        }
    }

    if((digitalRead(S_TRASEIRO_ESQ) == SENSOR_BORDA_ATIVO)||(digitalRead(S_TRASEIRO_DIR) == SENSOR_BORDA_ATIVO))
    {
        if(!bloqueiaSensor)
        {
            valAceleracao *= -1;
            bloqueiaSensor = 1;
        }
    }
}

void exibeControleMotores()
{
    Serial.print(valAceleracao);
    Serial.print("\t");
    Serial.print(valDirecao);
    Serial.println();
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
