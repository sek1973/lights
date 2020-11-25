// For webserver needs
#include <WiFi.h>
#include "webserver_settings.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

// those variables are used by effects file
WiFiClient client;
// Variable to store the HTTP request
String header;

#include "effects.h"

// Set web server port number to 80
WiFiServer server(80);

void init() {
  Serial.begin(115200);
}

void connectToWiFi() {
  // Webserver setup
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(NETWORK_SSID, NETWORK_PWD);
    Serial.print("Connecting to ");
    Serial.println(NETWORK_SSID);
    int trials = 0;
    while (WiFi.status() != WL_CONNECTED && trials < 10) {
      delay(500);
      Serial.print(".");
      trials++;
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected.");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());// this will display the Ip address of the board which should be entered into your browser
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);      
      getTimeInfo();
      server.begin();
    } else {
      WiFi.disconnect();
      Serial.println("WiFi connection failed.");
    }
  }
}

void runWebServer(void *parameter) {
  init();
  while(true) {
    connectToWiFi();
    client = server.available();   // Listen for incoming clients
    if (client) {                             // If a new client connects,
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected      
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          header += c;
          if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:           
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              effect_from_header();
         
              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the on/off buttons 
              // Feel free to change the background-color and font-size attributes to fit your preferences
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println("</style></head>");
              
              // Web Page Heading
              client.println("<body><h1>Lights Web Server</h1>");

              create_buttons();

              client.println("<hr>");              
              client.println("<form method=\"POST\">");

              client.println("<div style=\"display: flex; flex-direction: column; width: 100%; padding: 5px\">");
              
              client.println("<div style=\"display: flex; flex-direction: row; width: 100%; justify-content: center;\">");
              client.println("<div style=\"padding: 10px;\">");
              client.println("Start hour: <input type=\"number\" name=\"start_hour1\" /> <br/>");
              client.println("</div><div style=\"padding: 10px;\">");
              client.println("Stop hour: <input type=\"number\" name=\"stop_hour1\" /> <br/>");
              client.println("</div>");
              client.println("</div>");

              client.println("<div style=\"display: flex; flex-direction: row; width: 100%; justify-content: center;\">");              
              client.println("<div style=\"padding: 10px;\">");
              client.println("Start hour: <input type=\"number\" name=\"start_hour2\" /> <br/>");
              client.println("</div><div style=\"padding: 10px;\">");
              client.println("Stop hour: <input type=\"number\" name=\"stop_hour2\" /> <br/>");
              client.println("</div>");
              client.println("</div>");

              client.println("</div>");
                            
              client.println("<input type=\"submit\" name=\"action\" value=\"Submit\" />");
              client.println("</form>");
                              
              client.println("</body></html>");
              
              // The HTTP response ends with another blank line
              client.println();
              // Break out of the while loop
              break;
            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
      }
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }
}
