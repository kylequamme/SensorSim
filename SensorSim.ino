/*
**  Source code derived in part from SerialUI examples.
**  Copyright (C) 2013, 2014 Pat Deegan.  All rights reserved.
**
** http://www.flyingcarsandstuff.com/projects/SerialUI
**
** This program (and accompanying library) is free software;
** you can redistribute it and/or modify it under the terms of
** the GNU Lesser General Public License as published by the
** Free Software Foundation; either version 3 of the License,
** or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**
**
*/

// to use SerialUI, you need to include the main header:
#include <SerialUI.h>
/*
** ***************** Serial Settings ******************
*/
#define serial_baud_rate           115200
#define serial_input_terminator   '\n'

/*
** ********************* Menu Strings ***********************
*/

SUI_DeclareString(device_greeting,
                  "*SensorSim* v1.0 \r\nEnter '?' to list available options.");

SUI_DeclareString(top_menu_title, "*SensorSim* v1.0 Main Menu");

SUI_DeclareString(help_key, "help");

SUI_DeclareString(outs_key, "outputs");
SUI_DeclareString(outs_help, "Control Outputs");

SUI_DeclareString(toggle_out0_key, "output_0");
SUI_DeclareString(toggle_out0_help, "Turn Output On/Off");

SUI_DeclareString(set_long_out0_key, "0_dc");
SUI_DeclareString(set_long_out0_help, "Set the duty cycle %");

SUI_DeclareString(toggle_out1_key, "output_1");
SUI_DeclareString(toggle_out1_help, "Turn Output On/Off");

SUI_DeclareString(set_long_out1_key, "1_dc");
SUI_DeclareString(set_long_out1_help, "Set the duty cycle %");

SUI_DeclareString(toggle_out2_key, "output_2");
SUI_DeclareString(toggle_out2_help, "Turn Output On/Off");

SUI_DeclareString(set_long_out2_key, "2_dc");
SUI_DeclareString(set_long_out2_help, "Set the duty cycle %");

SUI_DeclareString(exit_key, "exit");
SUI_DeclareString(exit_help, "Exit (and close SensorSim)");


SUI_DeclareString(err_cantadd_command, "Could not addCommand to mainMenu?");


// a few labels for our variable auto-tracking
SUI_DeclareString(label_stream, "streaming");
SUI_DeclareString(label_swOut0, "the bool");
SUI_DeclareString(label_swOut1, "the bool");
SUI_DeclareString(label_swOut2, "the bool");
SUI_DeclareString(label_long0, "the int");
SUI_DeclareString(label_long1, "the int");
SUI_DeclareString(label_long2, "the int");
SUI_DeclareString(label_time, "time");

/*
** ********************* SerialUI instance ***********************
*/

SUI::SerialUI mySUI = SUI::SerialUI(device_greeting);


/*
 * Our global tracked variables
 */

unsigned long CurTime = 0;
unsigned long long_out0 = 0;
unsigned long long_out1 = 0;
unsigned long long_out2 = 0;
bool  swOut0 = false;
bool  swOut1 = false;
bool  swOut2 = false;


/*
** ********************* setup() ***********************
**
** The standard Arduino setup() function.  Here we'll
** setup serial comm and the menu structure.
*/

void setup()
{

  // SerialUI acts just like (is actually a facade for)
  // Serial.  Use it, rather than Serial, throughout the
  // program.
  // Setup of SerialUI:
  mySUI.begin(serial_baud_rate); // serial line open/setup
  mySUI.setTimeout(20000);      // timeout for reads (in ms), same as for Serial.
  mySUI.setMaxIdleMs(30000);    // timeout for user (in ms)
  // how we are marking the "end-of-line" (\n, by default):
  mySUI.setReadTerminator(serial_input_terminator);


  // Setup variable state tracking -- will report changes to
  // Druid4Arduino (v >= 1.3.0) so it can automatically display
  // these in GUI.
  // To setup tracking, you need:
  //	* a pointer to a persistent (i.e. global or at least
  //	  guaranteed to be valid) variable to refer to, and
  //	* a label (created with SUI_DeclareString()).
  //
  // Variables will be listed in the order they are added here:
  mySUI.trackState(label_swOut0, &swOut0);
  mySUI.trackState(label_swOut1, &swOut1);
  mySUI.trackState(label_swOut2, &swOut2);
  mySUI.trackState(label_long0, &long_out0);
  mySUI.trackState(label_long1, &long_out1);
  mySUI.trackState(label_long2, &long_out2);
  mySUI.trackState(label_time, &CurTime);
  // and that's it, you're variables will be displayed in the druid GUI



  // The SerialUI menu setup is a bit involved, and it
  // needs to know about the functions we'll be using as
  // callbacks. Instead of having a bunch of function
  // declarations, all the work is contained in a function
  // of its own at the bottom of the program.
  // Yes: *DO* check it out!
  setupMenus();

}



/*
** ********************* loop() ***********************
**
** The standard Arduino loop() function.
*/
void loop()
{

  /* We checkForUser() periodically, to see
  ** if anyone is attempting to send us some
  ** data through the serial port.
  **
  ** This code checks all the time, for 150 ms,
  ** upon entering the loop.  Should you want to
  ** check for user access only once (say have a
  ** 10 second wait on startup, and then forgo
  ** allowing SerialUI access), then increase the
  ** delay parameter and use checkForUserOnce(), e.g.
  **
  **    mySUI.checkForUserOnce(15000);
  **
  ** to allow 15 seconds to connect on startup only.
  */
  
  if (mySUI.checkForUserOnce(10000))
  {
    // we have a user initiating contact, show the
    // greeting message and prompt
    mySUI.enter();
    
    /* Now we keep handling the serial user's
    ** requests until they exit or timeout.
    */
    while (mySUI.userPresent())
    {
      // actually respond to requests, using
      mySUI.handleRequests();
      CurTime = millis();

    }

  } /* end if we had a user on the serial line */

}


/*
** ********************* Callbacks ***********************
*/

void toggle_out0()
{
  swOut0 = !swOut0;
  mySUI.print(F("Output #0 is "));
  mySUI.println(swOut0 ? F("ON") : F("OFF"));

  return mySUI.returnOK();
}

void toggle_out1()
{
  swOut1 = !swOut1;
  mySUI.print(F("Output #1 is "));
  mySUI.println(swOut1 ? F("ON") : F("OFF"));

  return mySUI.returnOK();
}

void toggle_out2()
{
  swOut2 = !swOut2;
  mySUI.print(F("Output #2 is "));
  mySUI.println(swOut2 ? F("ON") : F("OFF"));

  return mySUI.returnOK();
}

void set_long_out0()
{
  // Here, we want some additional (numerical) input from
  // the user, so we show the "enter data prompt"
  // using... showEnterNumericDataPrompt
  mySUI.showEnterNumericDataPrompt();

  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  long_out0 = mySUI.parseInt();

  // provide some feedback
  mySUI.println(long_out0, DEC);
  mySUI.returnOK();
}

void set_long_out1()
{
  // Here, we want some additional (numerical) input from
  // the user, so we show the "enter data prompt"
  // using... showEnterNumericDataPrompt
  mySUI.showEnterNumericDataPrompt();

  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  long_out1 = mySUI.parseInt();

  // provide some feedback
  mySUI.println(long_out1, DEC);
  mySUI.returnOK();
}

void set_long_out2()
{
  // Here, we want some additional (numerical) input from
  // the user, so we show the "enter data prompt"
  // using... showEnterNumericDataPrompt
  mySUI.showEnterNumericDataPrompt();

  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  long_out2 = mySUI.parseInt();

  // provide some feedback
  mySUI.println(long_out2, DEC);
  mySUI.returnOK();
}

void show_help()
{
  mySUI.println(F("SensorSim Help")) ;
  mySUI.println(F("Use the 'outputs' menu to turn the outputs on/off"));
  mySUI.println(F("and adjust the duty cycle."));

  return mySUI.returnOK();
}

void do_exit() {
  // though you can always just use the "quit" command from
  // the top level menu, this demonstrates using exit(), which
  // will automatically close the Druid4Arduino GUI, if
  // being used.
  mySUI.print(F("Exit requested, terminating GUI if present"));
  mySUI.exit();
}

/*
** ********************* setupMenus() ***********************
*/

void setupMenus()
{
  SUI::Menu * mainMenu = mySUI.topLevelMenu();
  if (! mainMenu)
  {
    // what? Could not create :(
    return mySUI.returnError("Something is very wrong, could not get top level menu?");
  }

  mainMenu->setName(top_menu_title);

  if (! mainMenu->addCommand(help_key, show_help) )
  {
    return mySUI.returnError_P(err_cantadd_command);

  }
  
  SUI::Menu * outsMenu = mainMenu->subMenu(outs_key, outs_help);
  if (! outsMenu)
  {
    // ah, could not create :(
    return mySUI.returnError("Couldn't create enable menu!?");
  }

  // output menu created, add our commands
  outsMenu->addCommand(toggle_out0_key, toggle_out0, toggle_out0_help);
  outsMenu->addCommand(set_long_out0_key, set_long_out0, set_long_out0_help);
  outsMenu->addCommand(toggle_out1_key, toggle_out1, toggle_out1_help);
  outsMenu->addCommand(set_long_out1_key, set_long_out1, set_long_out1_help);
  outsMenu->addCommand(toggle_out2_key, toggle_out2, toggle_out2_help);
  outsMenu->addCommand(set_long_out2_key, set_long_out2, set_long_out2_help);


  // exit
  if (! mainMenu->addCommand(exit_key, do_exit, exit_help) )
    return mySUI.returnError_P(err_cantadd_command);


  // Done setting up the menus!

}
