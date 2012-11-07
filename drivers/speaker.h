///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2012, Brad Goold, All Rights Reserved.
//
// Authors: Brad Goold
//
// Date: 15 Feb 2012
//
// RCS $Date$
// RCS $Revision$
// RCS $Source$
// RCS $Log$///////////////////////////////////////////////////////////////////////////////

#ifndef SPEAKER_H
#define SPEAKER_H

extern xQueueHandle xBeepQueue;

void vBeepTask( void * pvParameters); 
#define TOUCH_BEEP 1
#define ALARM_BEEP 2
#endif
