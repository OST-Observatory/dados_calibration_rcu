/*
DADOS RCU Microcontroller Program v.0.1
as of 25.04.2022
 */

byte flatlamp = 5;
byte caliblamp = 6;

unsigned long maxtimeF = 600; // in seconds
unsigned long maxtimeW = 600; // in seconds

////////////////////
byte off = 1;
byte on = 0;
String command;
String sub_command;
char first_char;
unsigned long mytime;
unsigned long timerF = 0;
unsigned long timerW = 0;
bool forceF = false;
bool forceW = false;
bool stateF = false;
bool stateW = false;

void lamp_onoff(byte lamp, byte state) { // define the switch-on/-off routine for the lamp
    if (state == off) { // if the user wants to switch off the lamp proceed ...
      if (lamp == flatlamp) { // if the flat lamp is supposed to be switched off ...
        timerF = 0; // reset awake time counter
        stateF = false; // set the flat lamp state variable to off
        digitalWrite(flatlamp, off); // physically switch flat lamp off
      }
      if (lamp == caliblamp) { // if the wavelength calibration lamp is supposed to be switched off ...
        timerW = 0; // reset awake time counter
        stateW = false; // set the wavelength calibration lamp state variable to off
        digitalWrite(caliblamp, off); // physically switch wavelength calibration lamp off
      }
    }
    if (state == on) { // if one of the lamps is to switched ON ...
      if (lamp == flatlamp) {
        timerF = millis(); // measure system time to determine the awake time of the lamp
        stateF = true; // set the lamp state variable to on as the lamp is now switched on
        digitalWrite(flatlamp, on); // physically switch the flat lamp on
      }
      if (lamp == caliblamp) {
        timerW = millis(); // measure system time to determine the awake time of the lamp
        stateW = true; // set the lamp state variable to on as the lamp is now switched on
        digitalWrite(caliblamp, on); // physically switch the wavelength calibration lamp off
      }
    }
}

void setup() { // initialization procedure
  Serial.begin(9600);

  pinMode(flatlamp, OUTPUT);
  pinMode(caliblamp, OUTPUT);

  digitalWrite(flatlamp, off); // initialize BOTH lamps switch off
  digitalWrite(caliblamp, off);
  
  maxtimeF = 1000 * maxtimeF; // convert to milliseconds
  maxtimeW = 1000 * maxtimeW; // convert to milliseconds
  
  delay(10); //necessary to reserve the "0" for the millis function
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if(Serial.available() > 0) { // if something arrives at the read buffer, read it ...
    command = Serial.readStringUntil(';'); ///unitl you arrive at the termination symbol ';'
    first_char = command[0]; // take the first character to identify the respective lamp of interest
    sub_command = command.substring(1); // the rest of the command is then the actual action to take

    if ((first_char == 'f') or (first_char == 'F') or (first_char == char(flatlamp))) {
      //commands for the flatlamp follow here ...
      if(sub_command.equals("on")) {lamp_onoff(flatlamp, on);}
      if(sub_command.equals("off")) {lamp_onoff(flatlamp, off);}
      if(sub_command.equals("get")) {Serial.print(stateF);}
      if(sub_command.equals("forceon")) {forceF = true;}
      if(sub_command.equals("forceoff")) {forceF = false;}
      if(sub_command.equals("forceget")) {Serial.print(forceF);}
      if(sub_command.substring(0, 6).equals("setmax")) {maxtimeF = 1000 * strtoul(sub_command.substring(6).c_str(), NULL, 10);}
      if(sub_command.equals("getmaxtime")) {Serial.print(float(maxtimeF) / 1000.);}
    }
    else if ((first_char == 'w') or (first_char == 'W') or (first_char == char(caliblamp))) {
      //commands for the wavelength calibration lamp follow here ...
      if(sub_command.equals("on")) {lamp_onoff(caliblamp, on);}
      if(sub_command.equals("off")) {lamp_onoff(caliblamp, off);}
      if(sub_command.equals("get")) {Serial.print(stateW);}
      if(sub_command.equals("forceon")) {forceW = true;}
      if(sub_command.equals("forceoff")) {forceW = false;}
      if(sub_command.equals("forceget")) {Serial.print(forceW);}
      if(sub_command.substring(0, 6).equals("setmax")) {maxtimeW = 1000 * strtoul(sub_command.substring(6).c_str(), NULL, 10);}
      if(sub_command.equals("getmaxtime")) {Serial.print(float(maxtimeW) / 1000.);}
    }
    //else {
    //  Serial.println("Unknown command: " + command); // throw back the command if you cannot read it
    //}
  }
  delay(10); // allow the COM port some delay time 
  mytime = millis();
  if (((mytime - timerF) > maxtimeF) and (forceF == false) and (stateF == true)) { // is force mode is off, the flat lamp is ON, and the maxtime has been exceeded, shut-off the lamp to extend their lifetime
    lamp_onoff(flatlamp, off); //
  }
  if (((mytime - timerW) > maxtimeW) and (forceW == false) and (stateW == true)) { // is force mode is off, the wavelength calibration lamp is ON, and the maxtime has been exceeded, shut-off the lamp to extend their lifetime
    lamp_onoff(caliblamp, off);
  }
}
