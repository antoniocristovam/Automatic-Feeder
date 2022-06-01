
/*
 Alimenador de pet 

 Um projeto simples mas muito eficiente, cuja finalidade é você
 poder colocar comida para o seu melhor amigo mesmo longe de casa. 

By Antonio Cristovam, Geyzon Costa, Luis Felipe
 */

#include <WiFi.h>
#include <ESP32_Servo.h>

const char* ssid     = "esp";
const char* password = "felipe1234";

WiFiServer server(80);

Servo servomotor; // Cria objeto para controlar o servo
int Abertura = 180;
int Fechamento = 46;
int UltimoEstado = 0;
int servoPin = 13; // Especifica o pino usado para controlar o servo
int botaoPin = 5; // Variável para armazenar o valor lido do potenciometro

void setup()
{
    Serial.begin(115200);

    pinMode(botaoPin, INPUT);
    servomotor.attach(servoPin); // Associa o pino 7 ao objeto servomotor
    servomotor.write(Fechamento); // Inicia servomotor na posição zero
    
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
  
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<!DOCTYPE html><html>");
            client.println("<head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" type=\"image/x-icon\" href=\"/https://raw.githubusercontent.com/antoniocristovam/projetoiot/main/cool.ico\">");
            client.println("<title>Automatic Feeder</title>");
            client.println("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.0-beta1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-0evHe/X+R7YkIZDRvuzKMRqM+OrBnVFBL6DOitfPri4tjfHxaWutUpFmBp4vmVor\" crossorigin=\"anonymous\">");
            client.println("<style>");
            client.println("@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@100;200;300;400;500;600;700;800&display=swap');");
            client.println("body {background: var(--bs-body-bg);}");
            client.println("p {font-size: 22px;font-family: 'Poppins', sans-serif;font-weight: 600;}");
            client.println("a {margin: 1rem;padding: 0.7rem 1rem;}");
            client.println(".navbar-brand {margin: 5px;font-size: 22px;font-family: 'Poppins', sans-serif;font-weight: 500;}");
            client.println(".center {width: 400px;height: 200px;text-align: center;position: absolute;bottom: 0;right: 0;left: 0;top: 0;margin: auto;}");
            client.println(".img-pet {-webkit-transition: -webkit-transform .5s ease;transition: transform .5s ease;margin-top: 15px;}");
            client.println(".img-pet:hover {-webkit-transform: scale(1.1);transform: scale(1.1);}");
            client.println("footer {font-family: 'Poppins', sans-serif;font-weight: 600;font-size: 15px;text-align: center;width: 100%;position: absolute;bottom: 0;left: 0;}");
            client.println("</style>");
            client.println("</head><body>");
            client.println("<nav class=\"navbar bg-light\"><div class=\"container-fluid\"><a class=\"navbar-brand\" href=\"index.html\"><img src=\"https://github.com/antoniocristovam/projetoiot/blob/main/cool.png?raw=true\" width=\"32\" height=\"32\" class=\"d-inline-block align-text-top\">Automatic Feeder</a></div></nav>");
            client.println("<div class=\"center\"><p>Clique no PET para Alimentar</p><a href=\"http://192.168.43.79/alimentar\"><img class=\"img-pet\" width=\"160px\" src=\"https://github.com/antoniocristovam/projetoiot/blob/main/cool.png?raw=true\"</a></a></div>");
            client.println("<footer>Todos os Direitos Reservador &copy;Automatic Feeder <img height=\"12px\" src=\"https://raw.githubusercontent.com/antoniocristovam/projetoiot/main/cool.ico\"></footer>");
            client.println("</body></html>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /alimentar")) {
          servo_wifi();              // GET /H turns the LED on
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void servo_wifi() {
  if (digitalRead(botaoPin) == HIGH && UltimoEstado == 0){
    servomotor.write(Abertura);
    UltimoEstado = 1;
    delay(500);
    servomotor.write(Fechamento);
    UltimoEstado = 0;
    delay(500);
  }
}
