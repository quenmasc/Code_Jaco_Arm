#include "ModeChange.h"
using namespace KinDrv{
	
	
	//parameters
int OldClass =0;
bool flag=true ;
jaco_joystick_button_t buttons={0};
jaco_joystick_axis_t axes = {0};
	// methods
struct termios orig_termios;

void ModeChange::reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}
void ModeChange::set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}
int ModeChange::kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int ModeChange::getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}
int ModeChange::classPCA(int classLabel){
	switch (classLabel){
			case 1 :
				return 1
			case 2 :
				return 1
			case 3 :
				return 1
			case 4 :
				return 1
			case 5 :
				return 2
			case 6 :
				return 3
			case 7 :
				return 4
			default :
				buttons={0}
		}
}

void ModeChange::ModeChange(int classLabel,int OldClass,JacoArm *arm){
		switch (classLabel){
			/*case 1 :
				if(OldClass!=1){
					buttons[2]=1;
					arm->push_joystick_button(buttons)
					arm->release_joystick();
				}*/
			case 2 :
				if(OldClass!=2){
					buttons[7]=1;
					arm->push_joystick_button(buttons)
					arm->release_joystick();
				}
			
			case 3 :
				if(OldeClass!=3){
					jaco_retract_mode_t mode = arm->get_status();
					while( mode != MODE_READY_STANDBY ) {
						usleep(1000*10); // 10 ms
						mode = arm->get_status();
						if( mode == MODE_READY_TO_RETRACT ) {
							arm->release_joystick();
							arm->push_joystick_button(2);
						}
					}
					arm->release_joystick();
				}
			case 4 :
				if(OldClass!=4){
					buttons[2]=1;
					arm->push_joystick_button(buttons)
					arm->release_joystick();
				}
			default :
				break;
	}
}
void ModeChange::MovementMode(int classLabel,bool flag){
	switch (classLabel){
			case 1 :
				while(flag){
					if (axes.trans_fb>=-3){
						axes.trans_fb=axes.trans_fb-0.1
						arm->move_joystick_axis(axes);
					}
				}
				arm->release_joystick();
			case 2 :
				while(flag){
					if (axes.trans_fb<=3){
						axes.trans_fb=axes.trans_fb+0.1
						arm->move_joystick_axis(axes);
					}
				}
				arm->release_joystick();
			case 3 :
				while(flag){
					if (axes.trans_fb<=3f){
						axes.trans_fb=axes.trans_fb+0.1f
						arm->move_joystick_axis(axes);
					}
				}
				arm->release_joystick();
			case 4 :
				while(flag){
					if (axes.trans_fb>=-3f){
						axes.trans_fb=axes.trans_fb-0.1f
						arm->move_joystick_axis(axes);
					}
				}
				arm->release_joystick();
			default :
				arm->release_joystick();
		}
}
void ModeChange::~ModeChange(void){}
}
