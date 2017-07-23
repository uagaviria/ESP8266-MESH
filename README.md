EspMesh

Implementación de la librería serial de software Arduino para el ESP8266
mas detalles en https://github.com/uagaviria/EspSoftwareSerial

Documentación técnica de EspMesh

EspMesh crea una red de auto-organización, donde todos los nodos están conectados. Todos los nodos en la malla son iguales. La red utiliza una topología en estrella, evitando rutas circulares. Los mensajes entre los diferentes nodos se envían en formato JSON, haciéndolos fáciles de entender y producir.

![Screenshot](espmesh.png)


Información de enrutamiento

La información de enrutamiento se comparte en forma de mensajes de sincronización de nodos. Cada nodo informa a sus vecinos acerca de todos los otros nodos a los que está conectado directamente ya todas sus respectivas subconnexiones. De esta manera, cada nodo tiene una imagen en tiempo real de toda la malla y sabe qué nodos están conectados a la malla. Esta información se actualiza  cada 3 segundos aproximadamente.

Casos en los que se realiza una nueva sincronización de nodo

-La petición de sub-conexiones se realiza periódicamente

-Cuando se detecta cualquier actualización en la red, es decir,  cualquier subconección de un nodo ha cambiado

-Cuando un nuevo nodo se conecta a un nuevo AP



