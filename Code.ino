#include <SPI.h>       
#include <Ethernet.h>
#define ACS712 A0
float voltagePerPoint=0.0048828125;
float sensitivity=0.066;
int count=1;
int mainload = 5;
int disp=6;
int light=7;
int charger=8;
int pos = 0; 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x3F, 0xFE, 0xED };   
byte ip[] = { 192,168,1,100 };                        
//byte gateway[] = { 192, 168, 1, 1 };                   
//byte subnet[] = { 255, 255, 255, 0 };                  
EthernetServer server(80);                                 
 
String readString;            

void setup() {

  Serial.begin(9600);
  pinMode(mainload, OUTPUT);   
  pinMode(disp, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(charger, OUTPUT);
 
  Ethernet.begin(mac, ip);    
  server.begin();             
  Serial.print("server is working at ");          
  Serial.println(Ethernet.localIP());   
  digitalWrite(charger,HIGH);
}


void loop() {
  delay(1000);
  int value=analogRead(ACS712)+1;
  float voltage=voltagePerPoint*value;
  voltage=voltage-2.5; 
  float Ampere=voltage/sensitivity; 
  float mod = count%10;
  if (mod==0 ){  
    Serial.println("Current is ="+String(Ampere));
    Serial.println("Count is ="+String(count));
    if(Ampere<0.5){
    Serial.println("relay closed");
    digitalWrite(8, LOW);
    delay(1000);
    }
  }
  
count++;

  
  EthernetClient client = server.available();
  
  if (client) {                   
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        if (readString.length() < 100) {  
          readString += c;  
         }

 // Server WEB PAGE if requested by user      
         if (c == '\n') {          
           Serial.println(readString); 
           //html file 
           client.println("HTTP/1.1 200 OK"); 
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           //client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           //client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<TITLE>Energy Efficient Library</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY style=background-color:lightgreen>");
           client.println("<h1 style = 'text-align: center; font-weight: 900; font-family: Courier'>Energy Efficient Library</h1>");
           client.println("<div style = 'margin-left: 40%;'>");
           client.println("<a href=\"/?button1on\"\"><button style = 'height: 40px; width: 130px; background-color:black; color: white; border-radius: 10px; font-weight: 900; font-size: 15px'>MainLoadOn</button></a>");
           client.println("<a href=\"/?button1off\"\"><button style = 'margin-left: 20px; height: 40px; width: 130px; background-color:black; color: white; border-radius: 10px; font-weight: 900; font-size: 15px'>MainLoadOff</button></a>");
           client.println("</div>");
           client.println("</br>");
           client.println("<div style = 'margin-left: 40%;'>");
           client.println("<a href=\"/?button2on\"\"><button style = 'height: 40px; width: 130px; background-color:black; color: white; border-radius: 10px; font-weight: 900; font-size: 15px'>DisplayOn</button></a>");
           client.println("<a href=\"/?button2off\"\"><button style = ' margin-left: 20px; height: 40px; width: 130px; background-color:black; color: white; border-radius: 10px; font-weight: 900; font-size: 15px'>DisplayOff</button></a>");
           client.println("</div>");
           client.println("</br>");
           client.println("<div style = 'margin-left: 40%;'>");
           client.println("<a href=\"/?button3on\"\"><button style = 'height: 40px; width: 130px; background-color:black; color: white; border-radius: 10px; font-weight: 900; font-size: 15px'>LightsOn</button></a>");
           client.println("<a href=\"/?button3off\"\"><button style = 'margin-left: 20px; height: 40px; width: 130px; background-color:black; color: white; border-radius: 10px; font-weight: 900; font-size: 15px'>LightsOff</button></a>");
           client.println("</div>");
           client.println("</br>");
           client.println("<div style = 'margin-left: 40%;'>");
           client.println("<a href=\"/?button4on\"\"><button style = 'height: 40px; width: 130px; background-color:black; color: white; border-radius: 10px; font-weight: 900; font-size: 15px'>ChargerOn</button></a>");
           client.println("<a href=\"/?button4off\"\"><button style = 'margin-left: 20px; height: 40px; width: 130px; background-color:black; color: white; border-radius: 10px; font-weight: 900; font-size: 15px'>ChargerOff</button></a>");
           client.println("</div>");
           client.println("</br>");
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           //stopping client
           client.stop();
           //Translate the user request and check to switch on or off the fan
           if (readString.indexOf("?button1on") >0){
               digitalWrite(mainload, HIGH);
           }
           if (readString.indexOf("?button1off") >0){
               digitalWrite(mainload, LOW);
           }
           if (readString.indexOf("?button2on") >0){
               digitalWrite(disp, HIGH);
           }
           if (readString.indexOf("?button2off") >0){
               digitalWrite(disp, LOW);
           }      
           if (readString.indexOf("?button3on") >0){
               digitalWrite(light, HIGH);
           }
           if (readString.indexOf("?button3off") >0){
               digitalWrite(light, LOW);
           }
           if (readString.indexOf("?button4on") >0){
               digitalWrite(charger, HIGH);
           }
           if (readString.indexOf("?button4off") >0){
               digitalWrite(charger, LOW);
           }
            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}
