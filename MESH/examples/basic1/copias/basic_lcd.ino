//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time betweew 1 and 5 seconds
// 2. prints anything it recieves to Serial.print
//
//
//************************************************************
#include <LiquidCrystal_I2C.h>
#include "painlessMesh.h"

#define   MESH_PREFIX     "falcotecnologia"
#define   MESH_PASSWORD   "Red_mesh"
#define   MESH_PORT       5555

LiquidCrystal_I2C lcd(0x3F,16,2); // Check I2C address of LCD, normally 0x27 or 0x3F
String Comando;
int h1;
int h2;
int t1;
int t2;
int valor_hum;
int valor_temp;

void sendMessage() ; // Prototype so PlatformIO doesn't complain

painlessMesh  mesh;
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  Serial.println(msg.c_str());
     Comando = (msg.c_str());
      //Serial.println(Comando);
         Serial.print(Comando[0]); //H
         Serial.print(Comando[1]); // digito1 de valor
         Serial.print(Comando[2]); // digito0 de valor
         Serial.print(Comando[3]); // dos puntos de la hora
         Serial.print(Comando[4]); // digito1 de minutos
         Serial.print(Comando[5]); // digito0 de minutos
         Serial.print(Comando[6]); // dos puntos de la hora
         Serial.print(Comando[7]); // digito1 de segundos
          Serial.print(Comando[8]); // digito0 de segundos
          Serial.print(Comando[9]); // dos puntos de dias
          Serial.print(Comando[10]); // digito1 de dia
          Serial.print(Comando[11]); // digito0 de dia
          Serial.println(Comando[12]); // dos puntos de mes
         // Serial.print(Comando[13]); // digito1 de mes
         // Serial.print(Comando[14]); // digito0 de mes
         // Serial.print(Comando[15]); // dos puntos de año
         // Serial.print(Comando[16]); // digito1 de año
         // Serial.print(Comando[17]); // digito0 de año
         // Serial.println(Comando[18]); // digito1 de año

       h1 = Comando[11];
       h2 = Comando[12]; 
       //valor = (h1 - 48)*10 + (h2 - 48);
       valor_hum = (h1 - 48)*10 + (h2 - 48);
       Serial.println(valor_temp);

       t1 = Comando[28];
       t2 = Comando[29]; 
       //valor = (h1 - 48)*10 + (h2 - 48);
       valor_temp = (t1 - 48)*10 + (t2 - 48);
       Serial.println(valor_temp);



  //   if (Comando.substring(1) == "Humedad") {
  //   Serial.println("It's an html file");
  // }   

  // Comando.trim();
  // if (Comando.length() >0) {
  //   if (Comando == humedad){
  //     Serial.println("Encendiste el tanque # 1");
      
  //   }
  //  } 

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
  lcd.begin(2,4);      // In ESP8266-01, SDA=2, SCL=4               
  lcd.backlight();
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  mesh.scheduler.addTask( taskSendMessage );
  taskSendMessage.enable() ;



}

void loop() {
  mesh.update();
  display();
  
}

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

 void display()
{
    lcd.home();                // At column=0, row=0
    lcd.setCursor(0, 0);
    lcd.print("Hum : "); 
    lcd.setCursor(6, 0);
    lcd.print(valor_hum);
    lcd.setCursor(8, 0);
    lcd.print("         ");


    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.setCursor(6, 1);
    lcd.print(valor_temp);
    lcd.setCursor(8, 1);
    lcd.print("         ");
    //delay(500);
 }








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