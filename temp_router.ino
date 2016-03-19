#include <SPI.h>
#include <Client.h>
#include <Ethernet.h>
#include <Server.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,10);
EthernetServer server(80);
int PinLed1= 5; //esto sera actuador 1 en algun momento
int PinLed2= 4; //esto sera actuador 2 en algun momento
String readString = String(30);
int analog_pin = 0;
float tempC;
float tempF;
float tempK;

void setup(){
// start the Ethernet connection and the server:
    Ethernet.begin(mac, ip);
    server.begin();
    pinMode(PinLed1,OUTPUT);
    pinMode(PinLed2,OUTPUT);
    pinMode(12, INPUT);
    digitalWrite(12, HIGH); //pullup
    }

  
void loop(){
    tempC = (5.0*analogRead(analog_pin)*100.0)/1024.0;
    tempK = tempC + 273.15;
    tempF = (1.8*tempC)+32;
// listen for incoming clients
EthernetClient client = server.available();
  if (client) {
  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
      char c = client.read();
      if (readString.length()<100) //leer peticion HTTP caracter por caracter
        {
          readString += c;
        }
      
      if (c=='\n'){ //Si la peticion HTTP ha finalizado
         //Determina lo que se recibe mediante GET para ON/OFF
        if(readString.indexOf("Led1=Encender")>0){
          digitalWrite(PinLed1,HIGH);
        }
        if(readString.indexOf("Led1=Apagar")>0){
        digitalWrite(PinLed1,LOW);
        }
        if(readString.indexOf("Led2=Encender")>0){
        digitalWrite(PinLed2,HIGH);
        }
        if(readString.indexOf("Led2=Apagar")>0){
        digitalWrite(PinLed2,LOW);
        }
      
readString=""; //Vaciar el string que se uso para la lectura
//Enviar cabecera HTTP estandar
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
// client.println("Connection: close"); // the connection will be closed after completion of the response
// client.println("Refresh: 1"); // refresh the page automatically every 5 sec
client.println();
//Crear pagina web HTML
client.println("<html>");
client.println("<head>");
client.println("<title>UDISTRITAL</title>");
client.println("</head>");
client.println("<body BGCOLOR=#4B088A>");
client.println("<style> p { color:#FFFFFF; font-size:30px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial}</style>");
client.println("<style> p1 { color:#FFFFFF; font-size:20px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial}</style>");
client.println("<style> p2 { color:#FFFFFF; font-size:13px;font-weight: bold; font-family:Sans-Serif, Helvetica, Arial}</style>");
client.println("<center>");
client.println("<p>PROYECTO IoT USANDO ETHERNET</p>");
client.println("<hr><br>");
//ACTUADORES
client.println("<p1>TABLERO PARA EL CONTROL DE INSTRUMENTOS</p1>");
client.println("<hr><br>");
client.println("<table>");
client.println("<tr><td><p2>ACTUADOR 1</p2></td><td>    </td><td><p2>ACTUADOR 2</p2></td></tr>");
client.println("<tr><td><form method=GET><input type=submit name=Led1 value=Encender></form></td><td>....</td><td><form method=GET><input type=submit name=Led2 value=Encender></form></td></tr>");
client.println("<tr><td><form method=GET><input type=submit name=Led1 value=Apagar ></form></td><td>    </td><td><form method=GET><input type=submit name=Led2 value=Apagar></form></td></tr>");
client.println("</table>");
client.println("<hr><br>");
//SENSORES
client.println("<p1>LECTURA DE SENSORES ANALOGICOS</p1><br>");
client.println("<hr><br>");
client.println("<p2>");
client.println(tempC);
client.println(" Grados Centigrados");
client.println("<br><br>");
client.println(tempF);
client.println("Grados Farenheit");
client.println("<br><br>");
client.println(tempK);
client.println("Grados Kelvin");
client.println("</p2>");
client.println("</center>");
client.println("</body>");
client.println("</html>");
client.stop();
      }
    }
  }
}
}
