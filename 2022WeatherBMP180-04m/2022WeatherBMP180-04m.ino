//Arduino Wemos R1 D1 (ESP8266 Boards 2.5.2) & GY-68 BMP180
//SCL=Wemos pin D1 (GPIO5 SCL) SDA=Wemos pin D2 (GPIO4 SDA)
//Outputs data to COM Monitor screen.
//BMP180 delivers uncompensated pressure & temperature values
//startPressure(3)--> 0 to 3. 3=hi rez, but slower
//0=low rez, but faster. Routine returns msecs to wait b4 pressure read

#include <Wire.h>                         //Serial library
#include <SFE_BMP180.h>                   //Include BMP180 library
SFE_BMP180 BMP180;                        //Pressure routine
#define ALTITUDE 4.0                      //Device altitude, meters
#define S_prt Serial.print                //Short name for Serial.print
#define S_pln Serial.println              //Short name for Serial.println

char status;                              //Status returned from BMP180
double T, P;                              //Absolute Temp, Pressure
double p0, a;                             //Relative press sealevel, alt
double Tf;                                //Temporary temp F

void(* resetFunc) (void) = 0;             //Reset Arduino function at addr 0

void setup() {
  Serial.begin(9600);                     //Open Com
  delay(500);                             //Allow serial to open
  if (BMP180.begin())                     //Init & if pressure available
    S_pln("BMP180 opened");               //Tell user success
  else {
    S_pln("BMP180 open failed\n");        //Tell user fail
    resetFunc();                          //Restart Arduino  
  }
}
void loop() {
  status = BMP180.startTemperature();     //Start temp reading
  if (status != 0){                       //If start success
    delay(1000);                          //Wait>=4.5 mSec after start b4 get
    status = BMP180.getTemperature(T);    //Start temp reading
    if (status != 0){                     //If temp gotten
      S_prt("Temperature: ");S_prt(T, 3); //Temp returned, places
      S_prt("°C; ");
      Tf=((T * 1.8) + 32);                //Calc F
      S_prt(Tf, 3);S_pln("°F");           //Temp F, places
      status = BMP180.startPressure(3);   //0-3 0=fast, low rez. 3=slow, hi rez
      if (status != 0){                   //If success
        delay(status);                    //Above function returns needed delay
        status = BMP180.getPressure(P,T); //Get P(press), Pass T(temp)
        if (status != 0){                 //If success
          S_pln("Barometric Pressure-Absolute: ");
          S_prt("  ");S_prt(P, 3);S_prt(" Pa (mbar); ");  //Print Pressure in Pa mbar
          S_prt(P * 100, 3);S_pln(" hPa");                //Print Pressure in hPa
          S_prt("  ");
          S_prt(P * 0.750061683, 3);S_pln(" mmHg");       //Print Pressure in mmHg
          S_prt("  ");S_prt(P * 0.000986923, 6);          //Print Pressure in atmospheres
          S_pln(" Atm; ");
          p0 = BMP180.sealevel(P, ALTITUDE);              //Pressure based on input alt
          S_prt("Barometric Pressure-Comp'ed To Sealevel(");
          S_prt(T, 2);S_prt("°C, ");                      //Print temp
          a = BMP180.altitude(P, p0);                     //Get comp'ed pressure
          S_prt(a, 2);S_prt(" meters");S_pln("): ");      //
          S_prt("  ");S_prt(p0, 3);S_prt(" Pa (mbar); "); //Comp'ed Pressure in Pa,mbar
          S_prt(p0 * 100, 3);S_pln(" hPa");               //Comp'ed Pressure in hPa
          S_prt("  ");
          S_prt(p0 * 0.750061683, 3);S_pln(" mmHg");      //Comp'ed Pressure in mmHg
          S_prt("  ");S_prt(p0 * 0.000986923, 6);         //Comp'ed Pressure in Atm
          S_pln(" Atm; ");
        }
        else S_pln("Temp reading start fail\n");
      }
      else S_pln("Get temperature fail\n");
    }
    else S_pln("Start pressure reading fail\n");
  }
  else S_pln("Get pressure fail\n");
  S_pln("==========================================");
  delay(3000);
}
