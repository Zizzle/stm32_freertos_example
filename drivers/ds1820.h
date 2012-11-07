///////////////////////////////////////////////////////////////////////////////
//Brad Goold 2012
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DS1820_H
#define DS1820_H

#define DS1820_PORT GPIOC
#define DS1820_PIN  GPIO_Pin_10


#define BUS_ERROR      0xFE
#define PRESENCE_ERROR 0xFD
#define NO_ERROR       0x00

// DS1820 Sensor defines: Use these macro's when calling ds1820_get_temp()
#define HLT 0
#define MASH 1
#define CABINET 2
#define AMBIENT 3
#define SPARE 4

//Scheduled task for FreeRTOS
void          vTaskDS1820Convert( void *pvParameters ); //task to

//get temp of a particular sensor
float         ds1820_get_temp(unsigned char sensor);


//Menu functions
void          ds1820_search_applet(void);
void          ds1820_search_key(uint16_t x, uint16_t y);
void          ds1820_display_temps(void);
#endif
