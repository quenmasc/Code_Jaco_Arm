#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <libkindrv/kindrv.h>
#include <sys/select.h>

#define SPACEBAR 32
#define U 85
namespace KinDrv{
	class  ModeChange{
		public :
			jaco_joystick_button_t buttons;
			jaco_joystick_axis_t axes;
			bool flag;
			int OldClass;
			int classPCA(int classLabel);
			void MovementMode(int classLabel,int OldClass,bool flag);
			ModeChange(int classLabel,JacoArm *arm);
			~ModeChange();
			void reset_terminal_mode();
			void set_conio_terminal_mode();
			int kbhit();
			int getch()
			
	};
	
}

