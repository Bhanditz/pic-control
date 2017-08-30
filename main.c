/*
 * File:   main.c
 * Author: jkahler
 *
 * Created on January 29, 2017, 3:01 PM
 */


#include <xc.h>
#include "pic18f26j53.h"
#include "SSS.h"
#define _XTAL_FREQ 8000000

#define Flash_DIR TRISAbits.TRISA0
#define Flash_PIN LATAbits.LATA0

#define SensorClear_DIR TRISAbits.TRISA1
#define SensorClear_PIN LATAbits.LATA1

#define SensorInput1_DIR TRISAbits.TRISA2
#define SensorInput1_PIN PORTAbits.RA2

#define AllCamera_DIR TRISBbits.TRISB5
#define AllCamera_PIN LATBbits.LATB5

#define SpeedCamera_DIR TRISBbits.TRISB4
#define SpeedCamera_PIN LATBbits.LATB4

#define AtoP1_DIR TRISCbits.TRISC0
#define AtoP1_PIN PORTCbits.RC0

#define AtoP2_DIR TRISCbits.TRISC1
#define AtoP2_PIN PORTCbits.RC1

#define Laser1Control_DIR TRISCbits.TRISC6
#define Laser1Control_PIN LATCbits.LATC6

#define Laser2Control_DIR TRISCbits.TRISC7
#define Laser2Control_PIN LATCbits.LATC7

#define INPUT 1
#define OUTPUT 0

void delay_ms(int x) {
    for (int i = 0; i < x; i++) {
        __delay_ms(1);
    }
}

void main(void) {
    int PicsPerSecond = 5;
    int Sensing_Plane_Delay = 0;
    int High_Speed_Delay = 1000 / 125;
    int High_Res_Delay = 1000 / (PicsPerSecond*2);
    
    ANCON0bits.PCFG2 = 1;
    VCFG1 = 0; // set vref- to AN2
    VCFG0 = 0; // set vref+ to
    
    Laser1Control_DIR = OUTPUT;
    Laser2Control_DIR = OUTPUT;
    Flash_DIR = OUTPUT;
    SensorClear_DIR = OUTPUT;
    AllCamera_DIR = OUTPUT;
    SpeedCamera_DIR = OUTPUT;
    
    SensorInput1_DIR = INPUT; 
    AtoP1_DIR = INPUT;
    AtoP2_DIR = INPUT;
    
    while(AtoP1_PIN == 1){}
    
    //--------------
    //Uncomment when Arduino controls laserType
    //int laserType = AtoP2_PIN;
    //--------------
    
    int laserType = 0;  
    
    if(laserType == 1){
        Laser2Control_PIN = 1;
        Laser1Control_PIN = 0;
    }
    else{
        Laser1Control_PIN = 1; //This is what we want!
        Laser2Control_PIN = 0;
    }
    
    //Clear Sensors and turn Flash on permanently
    
    SensorClear_PIN = 1;  
    Flash_PIN = 0;
    __delay_ms(40);
    SensorClear_PIN = 0;
    

    while(1){
        if(SensorInput1_PIN == 0){
            //turn laser off
            if(laserType == 1){
                Laser2Control_PIN = 0;
            }
            else{
                Laser1Control_PIN = 0;
            }
            //sensing plane delay
            Flash_PIN = 1;
            delay_ms(Sensing_Plane_Delay);
            //flash turns on
            delay_ms(High_Speed_Delay / 2);
            //All camera pin
            AllCamera_PIN = 1;
            delay_ms(High_Speed_Delay / 2);
            AllCamera_PIN = 0;
            //high speed delay
            delay_ms(High_Speed_Delay / 2);
            //strobe high speed
            SpeedCamera_PIN = 1;
            delay_ms(High_Speed_Delay / 2);
            SpeedCamera_PIN = 0;
            delay_ms(High_Speed_Delay / 2);
            //clear on
            SensorClear_PIN = 1;
            Flash_PIN = 0;
            //high res - high speed delay
            delay_ms(High_Res_Delay - (2 * High_Speed_Delay));
            
            //laser on
            if(laserType == 1){
                Laser2Control_PIN = 1;
            }
            else{
                Laser1Control_PIN = 1;
            }
            //clear off
            SensorClear_PIN = 0;
        }
    }
    
    return;
}