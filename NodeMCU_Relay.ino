/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Token de Blynk
char auth[] = "SX2e8gA2n3WPpB_x_ohAm88bvhO7OfFg";

//Credenciales del WiFi
char ssid[] = "MOVISTAR_9005";
char pass[] = "LUW96BxtnH7pW2EAhQU4";

bool boton_on_off; //Botón de Inicio Virtual de la App V0.
int delta_temperatura; //Ingreso del Aumento de Temperatura Virtual de la App V1.

long tiempo_calentamiento; //Tiempo en ms para calentar el agua.

BLYNK_WRITE(V0)/*Función para leer el status del botón de encendido*/
{  int pinValue_V0=param.asInt();
  boton_on_off=pinValue_V0;}
  
BLYNK_WRITE(V1) /*Función para leer la temperatura de input en el app*/
{  int pinValue_V1 = param.asInt();
  delta_temperatura=pinValue_V1;}

BlynkTimer timer; //Inicializamos el timer.

void ControlTerma()
{  int masa=80000; //Masa del agua a calentar en gramos.
  int Ce=1; //Calor Específico del Agua.
  int Pot=21496; //Potencia eléctrica de la resistencia para calentar el agua.
  if (boton_on_off==true){
    digitalWrite(D0,LOW); //Encendemos el Relay.
    //Calculamos el tiempo de calentamiento en milisengundos.
    tiempo_calentamiento=masa*Ce*delta_temperatura*1.21*60*1000/Pot;
    delay(tiempo_calentamiento); //Dejamos encendido el Relay.
    digitalWrite(D0,HIGH); //Pasado el tiempo, apagamos el Relay.
    boton_on_off=false; //Variable de encendido a falso.        }    
  else {    digitalWrite(D0,HIGH); //Si el botón de encendido está apagado, el Relay también.
    Blynk.virtualWrite(V0,false); //Variable de encendido virtual VO lo apagamos.    }
}

void setup()
{
  Serial.begin(9600); //Iniciamos comunicación serial con el computador.
  Blynk.begin(auth, ssid, pass); //Iniciamos comunicación Serial con Blynk.
  pinMode(D0,OUTPUT); //Configuramos el pin del Relay como salida.
  //Ejecutar cada segundo la función "ApagarTerma"
  timer.setInterval(1000L,ControlTerma);
}

void loop()
{
  Blynk.run();
  timer.run(); //Inicializa el BlynkTimer.
}

