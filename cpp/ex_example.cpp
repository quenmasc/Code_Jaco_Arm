#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <libkindrv/kindrv.h>
#include <sys/select.h>

using namespace KinDrv;

ModeChange CM;
int c;
int newclassLabel;
int OldClass;
OldClass=0;
// goto arm
int goto_home(JacoArm *arm)
{
  // going to HOME position is possible from all positions. Only problem is,
  // if there is some kinfo of error
  jaco_retract_mode_t mode = arm->get_status();
  switch( mode ) {
    case MODE_RETRACT_TO_READY:
      // is currently on the way to HOME. Need 2 button presses,
      // 1st moves towards RETRACT, 2nd brings it back to its way to HOME
      arm->push_joystick_button(2);
      .
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

// int main
int main(){
	while(1){
		c=0;
		CM.set_conio_terminal_mode();
		if (CM.kbhit()){
			c=CM.getch();
		}
		CM.reset_terminal_mode();
		
		// keyboard
		if (c==3){
				KinDrv::close_usb();
				break;
		}
		if (c==32){
					KinDrv::init_usb();
					printf("Create a JacoArm \n");
					JacoArm *arm;
					try {
						arm = new JacoArm();
						printf("Successfully connected to JACO arm! \n");
					} catch( KinDrvException &e ) {
						printf("error %i: %s \n", e.error(), e.what());
						return 0;
					}

					printf("Gaining API control over the arm \n");
					arm->start_api_ctrl();
					
					//check if we need to initialize arm
					jaco_retract_mode_t mode = arm->get_status();
					printf("Arm is currently in state: %i \n", mode);
					if( mode == MODE_NOINIT ) {
					//push the "HOME/RETRACT" button until arm is initialized
						jaco_joystick_button_t buttons = {0};
						buttons[2] = 1;
						arm->push_joystick_button(buttons);

						while( mode == MODE_NOINIT ) {
							usleep(1000*10); // 10 ms
							mode = arm->get_status();
						}	

						arm->release_joystick();
					}
					printf("Arm is initialized now, state: %i \n", mode);

					// we want to start from home_position
					goto_home(arm);
					
					arm->set_control_cart();
					
		}
		newclassLabel=ModeChange::classPCA(c)
		ModeChange::ModeChange(newclassLabel,OldClass,arm);
		ModeChange::MovementMode(newclassLabel,flag)
		OldClass=newclassLabel;
		
		
	}
	return 0;
}
