
/***************************************************************************
 *  ex_ctrl.cpp - KinDrv example - control arm, move it around
 *
 *  Created: Fri Oct 11 00:031:00 2013
 *  Copyright  2013  Bahram Maleki-Fard
 ****************************************************************************/

/*  This file is part of libkindrv.
 *
 *  libkindrv is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser Public License
 *  along with libkindrv.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libkindrv/kindrv.h>

#include <stdio.h>

using namespace KinDrv;

int
goto_retract(JacoArm *arm)
{
  // this can only be achieved from HOME position. Otherwise the arm
  // will move to HOME. You'll probably need to uncomment the gripper movements
  // in order for this to work. Or even better, implement moving to HOME position,
  // which could be triggered before going to RETRACT ;)
  jaco_retract_mode_t mode = arm->get_status();
  switch( mode ) {
    case MODE_READY_TO_RETRACT:
      // is currently on the way to RETRACT. Need 2 button presses,
      // 1st moves towards HOME, 2nd brings it back to its way to RETRACT
      arm->push_joystick_button(2);
      arm->release_joystick();
      arm->push_joystick_button(2);
      break;

    case MODE_READY_STANDBY:
    case MODE_RETRACT_TO_READY:
      // just 1 button press needed
      arm->push_joystick_button(2);
      break;

    case MODE_NORMAL_TO_READY:
    case MODE_NORMAL:
    case MODE_NOINIT:
      printf("cannot go from NORMAL/NOINIT to RETRACT \n");
      return 0;
      break;

    case MODE_ERROR:
      printf("some error?! \n");
      return 0;
      break;

    case MODE_RETRACT_STANDBY:
      printf("nothing to do here \n");
      return 1;
      break;
  }

  while( mode != MODE_RETRACT_STANDBY ) {
    usleep(1000*10); // 10 ms
    mode = arm->get_status();
  }
  arm->release_joystick();

  return 1;
}


int
goto_home(JacoArm *arm)
{
  // going to HOME position is possible from all positions. Only problem is,
  // if there is some kinfo of error
  jaco_retract_mode_t mode = arm->get_status();
  switch( mode ) {
    case MODE_RETRACT_TO_READY:
      // is currently on the way to HOME. Need 2 button presses,
      // 1st moves towards RETRACT, 2nd brings it back to its way to HOME
      arm->push_joystick_button(2);
      arm->release_joystick();
      arm->push_joystick_button(2);
      break;

    case MODE_NORMAL_TO_READY:
    case MODE_READY_TO_RETRACT:
    case MODE_RETRACT_STANDBY:
    case MODE_NORMAL:
    case MODE_NOINIT:
      // just 1 button press needed
      arm->push_joystick_button(2);
      break;

    case MODE_ERROR:
      printf("some error?! \n");
      return 0;
      break;

    case MODE_READY_STANDBY:
      printf("nothing to do here \n");
      return 1;
      break;
  }

  while( mode != MODE_READY_STANDBY ) {
    usleep(1000*10); // 10 ms
    mode = arm->get_status();
    if( mode == MODE_READY_TO_RETRACT ) {
      arm->release_joystick();
      arm->push_joystick_button(2);
    }
  }
  arm->release_joystick();

  return 1;
}



int main()
{
  printf("KinDrv example for controlling the arm \n");

  // explicitly initialize a libusb context; optional
  KinDrv::init_usb();



  printf("Create a JacoArm \n");
  JacoArm *arm;
  try {
    arm = new JacoArm();
    printf("Successfully connected to arm! \n");
  } catch( KinDrvException &e ) {
    printf("error %i: %s \n", e.error(), e.what());
    return 0;
  }

  printf("Gaining API control over the arm \n");
  arm->start_api_ctrl();


  // TEST: Tekin
  jaco_position_t ang_pos = arm->get_ang_pos();
  printf("angular positions: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         ang_pos.joints[0], ang_pos.joints[1], ang_pos.joints[2],
         ang_pos.joints[3], ang_pos.joints[4], ang_pos.joints[5],
	  ang_pos.finger_position[0], ang_pos.finger_position[1], ang_pos.finger_position[2]);
  
  jaco_position_t cart_pos = arm->get_cart_pos();
  printf("cartesian position: x: %f y: %f z: %f Tx: %f Ty: %f Tz: %f fingers: %f  %f  %f\n",
         cart_pos.position[0], cart_pos.position[1], cart_pos.position[2],
         cart_pos.rotation[0], cart_pos.rotation[1], cart_pos.rotation[2],
	  cart_pos.finger_position[0], cart_pos.finger_position[1], cart_pos.finger_position[2]);
  
  jaco_position_t ang_vel = arm->get_ang_vel();
  printf("angular velocities: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         ang_vel.joints[0], ang_vel.joints[1], ang_vel.joints[2],
         ang_vel.joints[3], ang_vel.joints[4], ang_vel.joints[5],
	  ang_vel.finger_position[0], ang_vel.finger_position[1], ang_vel.finger_position[2]);
  
  jaco_position_t ang_command = arm->get_ang_command();
  printf("angular command: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         ang_command.joints[0], ang_command.joints[1], ang_command.joints[2],
         ang_command.joints[3], ang_command.joints[4], ang_command.joints[5],
	  ang_command.finger_position[0], ang_command.finger_position[1], ang_command.finger_position[2]);
  
  jaco_position_t cart_command = arm->get_cart_command();
  printf("cartesian command: x: %f y: %f z: %f Tx: %f Ty: %f Tz: %f fingers: %f  %f  %f\n",
         cart_command.position[0], cart_command.position[1], cart_command.position[2],
         cart_command.rotation[0], cart_command.rotation[1], cart_command.rotation[2],
	  cart_command.finger_position[0], cart_command.finger_position[1], cart_command.finger_position[2]);
  
  jaco_position_t ang_force = arm->get_ang_force();
  printf("angular forces: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         ang_force.joints[0], ang_force.joints[1], ang_force.joints[2],
         ang_force.joints[3], ang_force.joints[4], ang_force.joints[5],
	  ang_force.finger_position[0], ang_force.finger_position[1], ang_force.finger_position[2]);
  
  jaco_position_t cart_force = arm->get_cart_force();
  printf("cartesian force: x: %f y: %f z: %f Tx: %f Ty: %f Tz: %f fingers: %f  %f  %f\n",
         cart_force.position[0], cart_force.position[1], cart_force.position[2],
         cart_force.rotation[0], cart_force.rotation[1], cart_force.rotation[2],
	  cart_force.finger_position[0], cart_force.finger_position[1], cart_force.finger_position[2]);
  
  jaco_position_t ang_current = arm->get_ang_current();
  printf("angular currents: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         ang_current.joints[0], ang_current.joints[1], ang_current.joints[2],
         ang_current.joints[3], ang_current.joints[4], ang_current.joints[5],
	  ang_current.finger_position[0], ang_current.finger_position[1], ang_current.finger_position[2]);
  
  jaco_position_t ang_current_motor = arm->get_ang_current_motor();
  printf("angular motor currents: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         ang_current_motor.joints[0], ang_current_motor.joints[1], ang_current_motor.joints[2],
         ang_current_motor.joints[3], ang_current_motor.joints[4], ang_current_motor.joints[5],
	  ang_current_motor.finger_position[0], ang_current_motor.finger_position[1], ang_current_motor.finger_position[2]);
  
  

  /*
  //check if we need to initialize arm
  jaco_retract_mode_t mode = arm->get_status();
  printf("Arm is currently in state: %i \n", mode);
  if( mode == MODE_NOINIT ) {
    //push the "HOME/RETRACT" button until arm is initialized
    arm->push_joystick_button(2);

    while( mode == MODE_NOINIT ) {
      usleep(1000*10); // 10 ms
      mode = arm->get_status();
    }

    arm->release_joystick();
  }
  printf("Arm is initialized now, state: %i \n", mode);

  float finger_open[3] = {0, 0, 0};
  float finger_close[3] = {5250, 5250, 5250};

  // set control type to angular
  arm->set_control_ang();
  // current joint values + target finger values
  jaco_position_t ang_pos = arm->get_ang_pos();

  printf("angular: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         ang_pos.joints[0], ang_pos.joints[1], ang_pos.joints[2],
         ang_pos.joints[3], ang_pos.joints[4], ang_pos.joints[5],
		 ang_pos.finger_position[0], ang_pos.finger_position[1], ang_pos.finger_position[2]);

  jaco_position_t cart_pos = arm->get_cart_pos();

  printf("cartesian: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         cart_pos.joints[0], cart_pos.joints[1], cart_pos.joints[2],
         cart_pos.joints[3], cart_pos.joints[4], cart_pos.joints[5],
		 cart_pos.finger_position[0], cart_pos.finger_position[1], cart_pos.finger_position[2]);

  printf("Opening gripper 1...");
  arm->set_target_ang(ang_pos.joints, finger_close);
  // wait a little bit, until movement is finished
  usleep(1000*5000);
  printf("DONE\n");
  
  printf("Closing gripper 1...");
  arm->set_target_ang(ang_pos.joints, finger_close);
  usleep(1000*5000);
  printf("DONE\n");
  
  arm->set_target_ang(69.842964, 209.939117, 315.143616, 85.171867, 349.780945, 80.170029, 5250, 5250, 5250);
  
  usleep(1000*5000);
  
  printf("Opening gripper 2...");
  // set target values; this moves the arm towards the target position
  arm->set_target_ang(ang_pos.joints, finger_open);
  // wait a little bit, until movement is finished
  usleep(1000*5000);
  printf("DONE\n");

  printf("Closing gripper 2...");
  //float finger_close[3] = {50, 50, 50};
  arm->set_target_ang(ang_pos.joints, finger_close);
  usleep(1000*2000);
  printf("DONE\n");



  printf("Move arm back to RETRACT position \n");
  if( !goto_retract(arm) ) {
    printf("Failed to go to RETRACT. Go to HOME first \n");
    if( goto_home(arm) ) {

     ang_pos = arm->get_ang_pos();

     printf("angular: joints: %f  %f  %f  %f  %f  %f fingers: %f  %f  %f\n",
         ang_pos.joints[0], ang_pos.joints[1], ang_pos.joints[2],
         ang_pos.joints[3], ang_pos.joints[4], ang_pos.joints[5],
                 ang_pos.finger_position[0], ang_pos.finger_position[1], ang_pos.finger_position[2]);

      printf("Try RETRACT again\n");
      goto_retract(arm);
    } else {
      printf("Also failed going to HOME. Might be some serious problem...hmm \n");
    }
  }
  */
  


  // explicitly close libusb context (only needed if explicitly openede before)
  KinDrv::close_usb();
  return 0;
}
