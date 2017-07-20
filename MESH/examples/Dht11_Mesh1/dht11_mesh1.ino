//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time betweew 1 and 5 seconds
// 2. prints anything it recieves to Serial.print
// 
//
//************************************************************
#include <Wire.h>
// #include <LCD.h>
// #include <LiquidCrystal_I2C.h> //lcd
#include "DHT.h"
#include "painlessMesh.h"

#define   LED 2       // GPIO number of connected LED, ON ESP-12 IS GPIO2
#define   BLINK_PERIOD    30000 // microseconds until cycle repeat
#define   BLINK_DURATION  1000  // microseconds LED is on for

#define   MESH_PREFIX     "falcotecnologia"
#define   MESH_PASSWORD   "Red_mesh"
#define   MESH_PORT       5555
#define led_tx 5
int cont_led = 0;

#define pin_rst 5
#define pin_led_rx 15
bool error;
int cont_led_rx = 0;

#define DHTTYPE DHT11
#define DHTPIN  4
int temp = 0;
int humid = 0;



DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

// //=======================================================================================================
// // selecciona los pines utilizados en el panel LCD POR I2C

// #define I2C_ADDR    0x27
// LiquidCrystal_I2C lcd(I2C_ADDR,2, 1, 0, 4, 5, 6, 7);
// //=======================================================================================================


painlessMesh  mesh;
int cont = 0;
int contador = 0;
int contador_rst = 0;
//StaticJsonBuffer<512> jsonBuffer;
uint32_t sendMessageTime = 0;

void setup() {
  dht.begin(); 
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
    pinMode(pin_rst, OUTPUT);
  pinMode(pin_led_rx, OUTPUT);
  digitalWrite(pin_rst, HIGH);
  digitalWrite(pin_led_rx, LOW); 
  // pinMode(led_tx,        OUTPUT);

//sincronizar();
// lcd.begin (16,2);        // Inicializar el display con 16 caraceres 2 lineas
//      lcd.setBacklightPin(3,POSITIVE);
//      lcd.setBacklight(HIGH);  
//      lcd.home ();                   // go home
// lcd.print("ESP8266WIFI_MESH");
   
// //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

void loop() {

    bool error;

    mesh.update();

    // Ejecutar el blink led
    bool  onFlag = true;
    uint32_t cycleTime = mesh.getNodeTime() % BLINK_PERIOD;
    for (uint8_t i = 0; i < (mesh.getNodeList().size() + 1); i++) {
        uint32_t onTime = BLINK_DURATION * i * 2;

        if (cycleTime > onTime && cycleTime < onTime + BLINK_DURATION)
            onFlag = false;
    }
    digitalWrite(LED, onFlag);
    
    digitalWrite(LED, onFlag);
          if(error == 1)
          {
            digitalWrite(pin_rst, HIGH);
          }
          else
          {
            
          }
    //sictronizar();
    //ESP.wdtFeed();

contador_rst++;
delay(1);
//Serial.println(error);
//Serial.println(contador_rst);
if(contador_rst >= 30)
{
  contador_rst = 0;
  error = 0;
  digitalWrite(pin_rst, LOW);
  digitalWrite(pin_led_rx, LOW);
  delay(30);
  

  //reconexion();
  //ESP.reset();
}    

  //sensor();

 // // Obtener la próxima vez al azar para enviar un mensaje
 //  if ( sendMessageTime == 0 ) {
 //    sendMessageTime = mesh.getNodeTime() + random( 1000000, 5000000 );
 //  }



  // Si el tiempo es ideal, Enviar a todos un mensaje!
  // if ( sendMessageTime != 0 && sendMessageTime < mesh.getNodeTime() ){

    
    
    String msg = " Humedad_2 = ";
    String msg1 = " temperatura_2 = ";
    msg = msg + humid++ + msg1 + temp++;
    
    //msg += mesh.getNodeId();
    mesh.sendBroadcast( msg );
    //mesh.sendBroadcast( msg1 );
    sendMessageTime = 0;

sensor();
// contador_rst++;
// //Serial.println(contador_rst);
// if(contador_rst >= 120)
// {
//   sincronizar();
//   //ESP.reset();
// }



//sincronizar();
    // String msg = "Humedad: ";
    // String msg1 = "temperatura: ";
    // msg = msg + humid++ ;
    // msg1 = msg1 + temp++ ;
    // //msg += mesh.getNodeId();
    // mesh.sendBroadcast( msg );
    // mesh.sendBroadcast( msg1 );
    // sendMessageTime = 0;

    
  }


void receivedCallback( uint32_t from, String &msg ) {     
  //Serial.printf("Comienza Aqui: contador %u mensaje =%s\n", contador++, msg.c_str());
  Serial.println(msg.c_str());
  error = 1;
  contador_rst = 0;
  digitalWrite(pin_rst, HIGH);
  //digitalWrite(pin_led_rx, HIGH);
     //Comando = (msg.c_str());
   cont_led_rx++;
   //Serial.println(cont_led_rx);
   if(cont_led_rx <= 1) digitalWrite(pin_led_rx, HIGH);
   if(cont_led_rx >= 2) digitalWrite(pin_led_rx, LOW);
   if(cont_led_rx >= 3) cont_led_rx = 0;

}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> Comienza Aquí: Nueva conexion, contador = %u\n", contador++);
    //Serial.printf("--> Comienza Aquí: Nueva conexion, ID_Nodo = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Conexiones cambiadas %s\n",mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Tiempo ajustado %u. Compensar = %d\n", mesh.getNodeTime(),offset);
}

void sensor()
{
  humid = dht.readHumidity();
  // Serial.print("humedad: ");
  // Serial.print(humid);
  temp = dht.readTemperature();
  // Serial.print(" Temperatura: ");
  // Serial.println(temp);
}

// void sincronizar()
// {
//   //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
//   mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

//   mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
//   mesh.onReceive(&receivedCallback);
//   mesh.onNewConnection(&newConnectionCallback);
//   mesh.onChangedConnections(&changedConnectionCallback);
//   mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
// }
