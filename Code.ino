//adding required libraries
#include <SPI.h>        
#include <Ethernet.h>
//Defining the port for current sensor
#define ACS712 A0
//Voltage in one point of the input from the current sensor
float voltagePerPoint=0.0048828125;
//Sensity of the ACS712 30A module
float sensitivity=0.066;
//intializing the variables used in project
float sum=0;
float AverageCurrent=0;
int count=1;
//intializing pins for different ports
int mainload = 5;//consist of Raspberry pi, WiFi router and arduino mega
int disp=6; // The monitor used for the project
int light=7;// Light bulbs
int charger=8;//mobile charger
//default mac address for the ethernet server
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x3F, 0xFE, 0xED };   
//Ip address of the ethernet sheild assigned by the Wi-Fi router
byte ip[] = { 192,168,1,100 };                        
//Starting the server using port 80                  
EthernetServer server(80);                                
// defining a string 
String readString;           

void setup() {
//starting the serial pin at 9600
  Serial.begin(9600);
//setting the pins as input or out put
  pinMode(mainload, OUTPUT);   
  pinMode(disp, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(charger, OUTPUT);
  pinMode(ACS712, INPUT);
  /Starting the server 
  Ethernet.begin(mac, ip);    
  server.begin();  
  //printing the ip at which ethernet server is running           
  Serial.print("Server is running at ");          
  Serial.println(Ethernet.localIP());  
  //setting the initial states of pins as high as required 
  digitalWrite(charger,HIGH);
  digitalWrite(disp,HIGH);
  digitalWrite(mainload,HIGH);
  
}


void loop() {
  //using delay to make the loop run once in a second
  delay(1000);
  //Reading the input from the current sensor
  int value=analogRead(ACS712)+1;
  //converting the value from the sensor to voltage
  float voltage=voltagePerPoint*value;
  voltage=voltage-2.5; 
  //using the sensitivitey of the sensor, converting the voltage into amperes
  float Ampere=voltage/sensitivity; 
  //print the value of the current
  //Serial.println("Current is ="+String(Ampere));
  //adding the values of current after each loop
  sum+=Ampere;
  //print the sum
  Serial.println("Sum is ="+String(sum));
  //calculting the mode of the count by 30
  float mod = count%30;
  //when count%30 is 0 entering the if statement
  if (mod==0 ){
    Serial.println("Sum is ="+String(sum));
    //taking averge current for 30 counts
    AverageCurrent=sum/30;  
    Serial.println("Current is ="+String(Ampere));
    Serial.println("Count is ="+String(count));
    //if the average of the sum is lessthan 0.2 then relay will be turned off
    if(AverageCurrent<0.2){
    Serial.println("relay closed");
    digitalWrite(8, LOW);
    delay(1000);
    }
    //setting the sum zero after every 30 counts
    sum=0;
  }
  
count++;

  //creating an ethernet client
  EthernetClient client = server.available();
  //if the ethernet client is avaible we will check whether it is connected
  if (client) {                   
    while (client.connected()) {  
    //checking if the client is availble    
      if (client.available()) {
      //if it is availbe it will return something which will stored in in c
        char c = client.read();
     //adding the value of the c into the readString
        if (readString.length() < 100) {  
          readString += c;  
         }
     // checking for new line as the request from the HTTP when finished creates a new line
         if (c == '\n') {          
           Serial.println(readString); 
           //html file 
           client.println("HTTP/1.1 200 OK"); 
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           //giving the title for the webpage
           client.println("<TITLE>Energy Efficient Library</TITLE>");
           client.println("</HEAD>");
           //setting styles for the body
           client.println("<BODY style=background-color:lightgreen>");
           client.println("<h1 style = 'text-align: center; font-weight: 900; font-family: Courier'>Energy Efficient Library</h1>");
           client.println("<div style = 'margin-left: 40%;'>");
           //creating and styling buttons
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
           //stoping the client
           client.stop();
           //now we willcheck what we recieved in readString and make decision based on it
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
            //at last we will set the readString empty
            readString="";  
           
         }
       }
    }
}
}
