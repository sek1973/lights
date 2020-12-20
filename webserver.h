// For webserver needs
#include <WiFi.h>
#include "webserver_settings.h"

#include "HttpRequest.h"
HttpRequest httpReq;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

// those variables are used by effects file
WiFiClient client;
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

void createHoursForm() {
  client.println("<form method=\"POST\" action=\"\">");
  client.println("  <div style=\"display: flex; flex-direction: column; width: 100%; padding: 5px\">");
  client.println("    <div style=\"display: flex; flex-direction: row; width: 100%; justify-content: center;\">");  
  client.println("      <div style=\"padding: 10px;\">");
  client.println("        Brightness (0 - 255): <input type=\"number\" name=\"brightness\" min=\"0\" max=\"255\" step=\"1\" value=\"" + String(settings.brightness) + "\"/> <br/>");
  client.println("      </div>");  
  client.println("    </div>");
  client.println("    <div style=\"display: flex; flex-direction: column; width: 100%; padding: 5px\">");
  client.println("      <div style=\"display: flex; flex-direction: row; width: 100%; justify-content: center;\">");
  client.println("        <div style=\"padding: 10px;\">");
  client.println("          Start hour: <input type=\"number\" name=\"start_hour1\" min=\"0\" max=\"23\" step=\"1\" value=\"" + String(settings.spans[0].startHour) + "\"/> <br/>");
  client.println("        </div>");
  client.println("        <div style=\"padding: 10px;\">");
  client.println("          Stop hour: <input type=\"number\" name=\"stop_hour1\" min=\"0\" max=\"23\" step=\"1\" value=\"" + String(settings.spans[0].stopHour) + "\"/> <br/>");
  client.println("        </div>");
  client.println("      </div>");
  client.println("      <div style=\"display: flex; flex-direction: row; width: 100%; justify-content: center;\">");              
  client.println("        <div style=\"padding: 10px;\">");  
  client.println("          Start hour: <input type=\"number\" name=\"start_hour2\" min=\"0\" max=\"23\" step=\"1\" value=\"" + String(settings.spans[1].startHour) + "\"/> <br/>");
  client.println("        </div>");
  client.println("        <div style=\"padding: 10px;\">");
  client.println("          Stop hour: <input type=\"number\" name=\"stop_hour2\" min=\"0\" max=\"23\" step=\"1\" value=\"" + String(settings.spans[1].stopHour) + "\"/> <br/>");
  client.println("        </div>");
  client.println("      </div>");
  client.println("    </div>");
  client.println("  </div>");            
  client.println("  <input type=\"submit\" name=\"action\" value=\"Submit\" style=\"background-color: darkred; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;\"/>");
  client.println("</form>");
}

String getString(char *value, int length) {
  String result = "";
  for (int i = 0; i < length; i++) {
    if (byte(value[i]) == 0) { return result; }
    if (byte(value[i]) > 32) { result += value[i]; }
  }
  return result;
}

void handleRequestParams() {
  char name[HTTP_REQ_PARAM_NAME_LENGTH], value[HTTP_REQ_PARAM_VALUE_LENGTH];
  // list received parameters GET and POST
  Serial.println("Parameters:");
  for (int i = 1; i <= httpReq.paramCount; i++) {
    httpReq.getParam(i, name, value);                  
    Serial.print(name);    
    Serial.print(" - ");
    Serial.println(value);     
    writeToMemory(getString(name, HTTP_REQ_PARAM_NAME_LENGTH),
      getString(value, HTTP_REQ_PARAM_VALUE_LENGTH).toInt());
  }
  readParamsFromFlash();
}

void runWebServer(void *parameter) {  
  init();
  while(true) {
    connectToWiFi();
    client = server.available();   // Listen for incoming clients
    if (client) {                             // If a new client connects      
      Serial.println("New client connected...");
      while (client.connected()) {            // loop while the client's connected      
        if (client.available()) {             // if there's bytes to read from the client,          
          char c = client.read();             // read a byte, then
          httpReq.parseRequest(c);            // parse the received caracter
          Serial.write(c);                    // and print it out the serial monitor                    
          
          if (httpReq.endOfRequest()) {        // if request has ended -> handle response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            effect_from_header(httpReq.uri);
         
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

            createButtons();

            client.println("<hr>");              

            handleRequestParams();
            
            createHoursForm();
                            
            client.println("</body></html>");
              
            // The HTTP response ends with another blank line
            client.println();

            Serial.print("Method: ");
            Serial.println(httpReq.method);          
            Serial.print("Uri: ");
            Serial.println(httpReq.uri);            
            Serial.print("Version: ");
            Serial.println(httpReq.version);            
            Serial.print("paramCount: ");
            Serial.println(httpReq.paramCount);
          
            httpReq.resetRequest(); // Reset object and free dynamic allocated memory
            
            break; // Break out of the while loop
          }
        }
      }
      delay(1); // give the web browser time to receive the data
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }      
  }
}
