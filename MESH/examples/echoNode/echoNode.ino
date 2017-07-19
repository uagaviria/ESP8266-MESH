//************************************************************
// this is a simple example that uses the painlessMesh library and echos any
// messages it receives
//
//************************************************************
#include "painlessMesh.h"

#define   LED 2       // GPIO number of connected LED, ON ESP-12 IS GPIO2
#define   BLINK_PERIOD    3000000 // microseconds until cycle repeat
#define   BLINK_DURATION  100000  // microseconds LED is on for

#define   MESH_PREFIX     "falcotecnologia"
#define   MESH_PASSWORD   "Red_mesh"
#define   MESH_PORT       5555

painlessMesh  mesh;



void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT); 

  // sincronizar(); 
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
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
    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE );
    Serial.println(mesh.getNodeList().size());
    // sincronizar();
}

void receivedCallback( uint32_t from, String &msg ) {
  //Serial.printf("Nodo de eco: Recibido de %u msg=%s\n", from, msg.c_str());
  //Serial.printf("STARTUP %u msg=%s\n", STARTUP);
  mesh.sendSingle(from, msg);
}


// void sincronizar()
// {
//   mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

//   mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
//   mesh.onReceive(&receivedCallback);
// }
