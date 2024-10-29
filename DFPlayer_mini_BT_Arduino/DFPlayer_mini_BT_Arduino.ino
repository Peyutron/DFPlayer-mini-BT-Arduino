
/*
 * Con DFPlayer Mini BT V2 controlaremos el módulo DFPlayer Mini por Bluetooth.
 * Este programa consta de dos partes, el archivo .ino y el .apk con la interfaz para android.
 *
 * Librería https://github.com/DFRobot/DFPlayer-Mini-mp3 
 * 
 * Microcontroladores: Arduino NANO y UNO
 *
 * Nuevas funciones v2:
 * - Repetición automática de canciones.
 * - Diferentes niveles de ecualización Normal, Pop, Rock, Jazz, Clasica, Base.
 * - Para el reproductor al desconectar el Bluetooth
 *
 * Software desarollado por Carlos Muñoz para https://www.infotronikblog.com
 * Fecha: 17/09/2017
 * Revisión: 29/10/2024 
 * - Librería DFPlayer-Mini-mp3 Obsoleta.
 * - APK sin probar
 */

#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial mySerial(10, 11); // RX, TX
SoftwareSerial btSerial(2, 3);

int busy  = 4;  // Pin estado del modulo
int vol = 15; // Volumen Inicial
char command;
String string;
boolean inicio = false;
boolean next_song = true;
boolean play_state = false;

void setup () {
  Serial.begin (115200);
  Serial.println(F("DFPlayerMiniBluetooth www.infotronikblog.com"));
  
  // Inicia SoftwareSerial para el modulo Bluettoh
  btSerial.begin  (9600);

  // Inicia SoftwareSerial para el modulo DFPlayer Mini
  mySerial.begin (9600);

  // Ponemos a la escucha el modulo DFPlayer Mini
  mySerial.listen();      
  
  // Enlaza el SoftwareSerial con DFPlayer Mini
  mp3_set_serial(mySerial); 
  delay(10);

  // Reinicia el módulo DFPlayer Mini
  mp3_reset();
  delay(1000);
  
  mp3_set_volume (vol); // Valor de 0 ~ 30
  delay(10);
  
  // Ecualizacion Normal por defecto
  mp3_set_EQ (0);  
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
  delay(25);
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

    case 'P':   // Reproducción
      reproducir();     
      break;
    
    case 'S':   // Parar
      parar();
      break;

    case  'N':  // Pista siguiente
      pista_siguiente();
      break;

    case 'F':   // Pista anterior
      pista_anterior();
      break;

    case 'D':   // Pausa
      pause();
      break;

    case 'H':   // Siguiente canción automatica
      next_track();   
      break;

    case 'U':   // Ecualizador Normal
      eq0();          
      break;

    case 'V':   // Ecualizador Pop
      eq1();
      break;

    case 'W':   // Ecualizador Rock
      eq2();
      break;

    case 'X':   // Ecualizador Jazz
      eq3();
      break;

    case 'Y':   // Ecualizador Clasica
      eq4();
      break;
    case 'Z':   // Ecualizador Base
      eq5();
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

// Reproducción
void reproducir() {    
  play_state = true;
  inicio = true;
  mp3_play();
  delay(10);
  Serial.println("Reproduciendo");
}

// Parar canción
void parar() {
  play_state = false;
  mp3_stop();
  delay(100);
  Serial.println("Parar");
}

// Pasar a la siguinte pista
void pista_siguiente() {
  mp3_next();
  delay(10);
  Serial.println("Pista siguiente");
}

// Pasar a la pista anterior
void  pista_anterior() {
  mp3_prev();
  delay(10);
  Serial.println("Pista anterior");
}

// Pausa la reproducción 
void pause() {
  play_state = false;
  mp3_pause();
  delay(10);
  Serial.println("Pause, pulse Play para continuar");
}

// Repite la canción
void next_track() {
  if (next_song == true ) {
    next_song = false;
    Serial.println("Sin repeticion");
  } else {
    next_song = true;
    Serial.println("Con repeticion");
  }
}

// Equalización normal
void eq0() {
  mp3_set_EQ (0);
  Serial.println("Ecualizacion Normal");
  delay(10);
}

// Equalización Pop
void eq1() {
  mp3_set_EQ (1);
  Serial.println("Ecualizacion Pop");
  delay(10);
}

// Equalización Rock
void eq2() {
  mp3_set_EQ (2);
  Serial.println("Ecualizacion Rock");
  delay(10);
}

// Equalización Jazz
void eq3() {
  mp3_set_EQ (3);
  Serial.println("Ecualizacion Jazz");
  delay(10);
}

// Equalización Clasica
void eq4() {
  mp3_set_EQ (4);
  Serial.println("Ecualizacion Clasica");
  delay(10);
}

// Equalización Base
void eq5() {
  mp3_set_EQ (5);
  Serial.println("Ecualizacion Base");
  delay(10);
}


