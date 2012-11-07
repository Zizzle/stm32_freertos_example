///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2012, Brad Goold, All Rights Reserved.
//
// Authors: Brad Goold
//
// Date: 16 Feb 2012
//
// RCS $Date$
// RCS $Revision$
// RCS $Source$
// RCS $Log$///////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H
#define TIMER_H
#define SetTIM3Duty( val )    TIM3->CCR3 = val 
void vTimerSetupTask( void * pvParameters);

#endif
