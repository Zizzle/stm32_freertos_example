///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2012, Brad Goold, All Rights Reserved.
//
// Authors: Brad Goold
//
// Date:  7 Feb 2012
//
// RCS $Date$
// RCS $Revision$
// RCS $Source$
// RCS $Log$///////////////////////////////////////////////////////////////////////////////

#ifndef LEDS_H
#define LEDS_H

//#define D1_PIN GPIO_Pin_6 cant be used as its the IRQ pin for the TP 
#define D2_PIN GPIO_Pin_7
#define D3_PIN GPIO_Pin_13
#define D4_PIN GPIO_Pin_6

//#define D1_PORT GPIOC
#define D2_PORT GPIOC
#define D3_PORT GPIOD
#define D4_PORT GPIOD

#define ON  1
#define OFF 0


void vLEDSet( GPIO_TypeDef *GPIO_PORT , 
              uint16_t GPIO_Pin, 
              unsigned portBASE_TYPE uxValue);

void vLEDToggle( GPIO_TypeDef *GPIO_PORT , uint16_t GPIO_Pin);

void vLEDInit( void );

#endif
