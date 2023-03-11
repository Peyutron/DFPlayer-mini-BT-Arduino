/*
  DFPlayer Mini BT V2 es un programa para controlar el modulo DFPlayer Mini por Bluetooth.
  Este programa consta de dos partes, el archivo .ino y el apk para android.
  Puedes encontrar el programa para su descarga en la seccion de descargas de la web
  www.infotronikblog.com http://www.infotronikblog.com/p/descargas.html

  Nuevas funciones respecto a la version anterior:
  -Repeticion automatica de canciones.
  -Diferentes niveles de ecualizacion (Normal, Pop, Rock, Jazz, Clasica, Base).
  -Para el reproductor al desconectar el Bluetooth

  Software desarollado por Carlos Muños para www.infotronikblog.com
  Fecha: 17/09/2017
*/



#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial mySerial(10, 11); // RX, TX
SoftwareSerial btSerial(2, 3);
int busy  = 4;  //Pin estado del modulo
int vol = 15; // Volumen Inicial
char command;
String string;
boolean inicio = false;
boolean next_song = true;
boolean play_state = false;

void setup () {
  Serial.begin (9600);
  btSerial.begin  (9600); //Inicia SoftwareSerial para el modulo Bluettoh
  mySerial.begin (9600);  //Inicia SoftwareSerial para el modulo DFPlayer Mini
  mySerial.listen();      //Pone a la escucha el modulo DFPlayer Mini
  mp3_set_serial (mySerial); //Enlaza el SoftwareSerial con DFPlayer Mini
  delay(10);              
  mp3_reset();
  Serial.println("DFPlayerMiniBluetooth www.infotronikblog.com");
  delay(1000);
  mp3_set_volume (vol); // Valor de 0 ~ 30
  delay(10);
  mp3_set_EQ (0);  //Ecualizacion Normal por defecto
  delay(10);
}

void loop () {

  boolean busy_state = digitalRead(busy);
  if ((busy_state) == HIGH && (next_song == true) && (play_state == true)) {
    if (inicio == true) {
      mp3_next();
      delay(10);
      Serial.println("Siguiente Cancion");
    }
  }

  btSerial.listen();
  delay(50);
  if (btSerial.available() > 0)
  {
    string = "";
  }
  // Serial.println(string);
  while (btSerial.available() > 0) {
    command = ((byte) btSerial.read());
    Serial.println(command);
    if (command == ':')
    {
      break;
    }
    else {
      string += command;
    }
    delay(10);
  }
  mySerial.listen();
  switch (command) {

    case 'P':
      reproducir();     // Reproduccion
      break;
    case 'S':
      parar();          //Parar
      break;
    case  'N':          
      pista_siguiente();//Pista siguiente
      break;
    case 'F':          
      pista_anterior();//Pista anterior
      break;
    case 'D':
      pause();        //Pausa
      break;
    case 'H':
      next_track();   //Siguiente cancion automatica
      break;
    case 'U':
      eq0();          //Ecualizador Normal
      break;
    case 'V':
      eq1();          //Ecualizador Pop
      break;
    case 'W':
      eq2();          //Ecualizador Rock
      break;
    case 'X':
      eq3();          //Ecualizador Jazz  
      break;
    case 'Y':
      eq4();          //Ecualizador Clasica
      break;
    case 'Z':
      eq5();          //Ecualizador Base
      break;
    default:
      break;
  }
  if ((string.toInt() >= 1) && (string.toInt() <= 30))
  {
    int volumen;
    volumen = string.toInt();
    mp3_set_volume(volumen);
    Serial.print("Nivel del volumen: ");
    Serial.println(volumen);
    delay(10);
  }

  string  = (":");
  command = (":");
  mp3_single_loop(false);

}
void reproducir() {    // Reproduccion
  play_state = true;
  inicio = true;
  mp3_play();
  delay(10);
  Serial.println("Reproduciendo");
}
void parar() {
  play_state = false;
  mp3_stop();
  delay(100);
  Serial.println("Parar");
}
void pista_siguiente() {
  mp3_next();
  delay(10);
  Serial.println("Pista siguiente");
}

void  pista_anterior() {
  mp3_prev();
  delay(10);
  Serial.println("Pista anterior");
}
void pause() {
  play_state = false;
  mp3_pause();
  delay(10);
  Serial.println("Pause, pulse Play para continuar");
}
void next_track() {
  if (next_song == true ) {
    next_song = false;
    Serial.println("Sin repeticion");
  } else {
    next_song = true;
    Serial.println("Con repeticion");
  }
}
void eq0() {
  mp3_set_EQ (0);
  Serial.println("Ecualizacion Normal");
  delay(10);
}
void eq1() {
  mp3_set_EQ (1);
  Serial.println("Ecualizacion Pop");
  delay(10);
}
void eq2() {
  mp3_set_EQ (2);
  Serial.println("Ecualizacion Rock");
  delay(10);
}
void eq3() {
  mp3_set_EQ (3);
  Serial.println("Ecualizacion Jazz");
  delay(10);
}
void eq4() {
  mp3_set_EQ (4);
  Serial.println("Ecualizacion Clasica");
  delay(10);
}
void eq5() {
  mp3_set_EQ (5);
  Serial.println("Ecualizacion Basel");
  delay(10);
}


