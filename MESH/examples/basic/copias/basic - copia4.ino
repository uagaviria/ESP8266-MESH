//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time betweew 1 and 5 seconds
// 2. prints anything it recieves to Serial.print
//
//
//************************************************************


//#include <Esp.h>
#include "painlessMesh.h"

#define   BLINK_PERIOD    3000000 // microseconds until cycle repeat
#define   BLINK_DURATION  100000  // microseconds LED is on for

#define   MESH_PREFIX     "falcotecnologia"
#define   MESH_PASSWORD   "Red_mesh"
#define   MESH_PORT       5555

#define pin_rst 5
#define pin_led_rx 4
bool error;

//LiquidCrystal_I2C lcd(0x3F,16,2); // Check I2C address of LCD, normally 0x27 or 0x3F
String Comando;
int h1;
int h2;
int t1;
int t2;
int valor_hum;
int valor_temp;
word contador_rst = 0;
int cont_led_rx = 0;
#define   LED             2       // GPIO number of connected LED, ON ESP-12 IS GPIO2

void sendMessage() ; // Prototype so PlatformIO doesn't complain

painlessMesh  mesh;
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
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
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
 //ESP.wdtDisable();
  
  pinMode(LED, OUTPUT);
  pinMode(pin_rst, OUTPUT);
  pinMode(pin_led_rx, OUTPUT);
  digitalWrite(pin_rst, HIGH);
  digitalWrite(pin_led_rx, LOW);


//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  mesh.scheduler.addTask( taskSendMessage );
  taskSendMessage.enable() ;

     // wdt_enable(8000);
     // Serial.println("Reset WDT");

}

void loop() {
    

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
if(contador_rst >= 30000)
{
  contador_rst = 0;
  error = 0;
  digitalWrite(pin_rst, LOW);
  digitalWrite(pin_led_rx, LOW);
  delay(30);
  

  //reconexion();
  //ESP.reset();
}   
Serial.println(mesh.getNodeList().size()); 
  
 }

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );


  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// void reconexion()
// {
//   //mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
//   // mesh.onReceive(&receivedCallback);
//    mesh.onNewConnection(&newConnectionCallback);
//   // mesh.onChangedConnections(&changedConnectionCallback);
//   // mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);


// }





// //************************************************************
// // this is a simple example that uses the painlessMesh library
// //
// // 1. sends a silly message to every node on the mesh at a random time betweew 1 and 5 seconds
// // 2. prints anything it recieves to Serial.print
// // 
// //
// //************************************************************
// //#include <LiquidCrystal_I2C.h>
// #include "painlessMesh.h"


// //LiquidCrystal_I2C lcd(0x3F,16,2); // Check I2C address of LCD, normally 0x27 or 0x3F

// #define   MESH_PREFIX     "falcotecnologia"
// #define   MESH_PASSWORD   "Red_mesh"
// #define   MESH_PORT       5555

// painlessMesh  mesh;
// char tmp = 25;

// uint32_t sendMessageTime = 0;

// void setup() {
//   Serial.begin(115200);
//   // lcd.begin(0,2);      // In ESP8266-01, SDA=0, SCL=2               
//   // lcd.backlight();
    
// //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
//   mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

//   mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
//   mesh.onReceive(&receivedCallback);
//   mesh.onNewConnection(&newConnectionCallback);
//   mesh.onChangedConnections(&changedConnectionCallback);
//    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
// }

// void loop() {
//   mesh.update();

//  // get next random time to send a message
//   if ( sendMessageTime == 0 ) {
//     sendMessageTime = mesh.getNodeTime() + random( 1000000, 5000000 );
//   }

//   // if the time is ripe, send everyone a message!
//   if ( sendMessageTime != 0 && sendMessageTime < mesh.getNodeTime() ){

//     String msg = "Hola desde el nodo ";
//     msg += mesh.getNodeId();
//     mesh.sendBroadcast( msg );

//     sendMessageTime = 0;
//     //display();

//   }
// }

// void receivedCallback( uint32_t from, String &msg ) {
//   Serial.printf("Comienza Aqui: Recibido de %u mensaje =%s\n", from, msg.c_str());
// }

// void newConnectionCallback(uint32_t nodeId) {
//     Serial.printf("--> Comienza Aquí: Nueva conexion, ID_Nodo = %u\n", nodeId);
// }

// void changedConnectionCallback() {
//     Serial.printf("Conexiones cambiadas %s\n",mesh.subConnectionJson().c_str());
// }

// void nodeTimeAdjustedCallback(int32_t offset) {
//     Serial.printf("Tiempo ajustado %u. Compensar = %d\n", mesh.getNodeTime(),offset);
// }

// // void display()
// // {
// //     lcd.home();                // At column=0, row=0
// //     lcd.print("ESP8266");   
// //     lcd.setCursor(0, 1);
// //     lcd.print("LiquidCrystalI2C");
// //     delay(500);
// // }