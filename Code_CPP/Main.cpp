#define _USE_MATH_DEFINES
#include <libkindrv/kindrv.h>
#include <sys/select.h>

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include "omp.h"
#include "Defined_Macro.h"
#include "IMU_Sensor.h"
#include "Control_JACO.h"
#include "Routine.h"

	using namespace KinDrv;
	using namespace std;

	/* USB DEVICE */
	int result;  
	const char* usb_dev;
	std::string usb_name;
	/* END USB DEVICE DEFINITION */
	
	const int Packet_Size = 32;
	int PCKT[Packet_Size - 1];
	static int S_Of_F = 51;
	static int E_Of_F = 52;
	int Baudrate = 115200;
	int JACO_PREVIOUS_MODE = 0;
	static bool ControlJACO = 0; //ON ou OFF
	static bool Acq_Started = 0; //ON ou OFF
	static bool Bouton_Hold = 0; //ON ou OFF
	Byte Data_received = System::Convert::ToByte(0);
	Byte Test;
	/////
	Byte INIT_KY[256];
	/////
	//TIMING
	double START_TIME = 0;
	double END_TIME = 0;
	double delay_TIME = 0;
	double GLOBAL_TIME;
	double INIT_TIME;
	int Sec, Min, Hr;
	int refresh = 0;
	int elapsed = 20;

	//KEYPAD
	int in_kyp; char ch_kyp;
	bool kyp_ready = 0;
	int kyp_delay = 0;
	
	const int nb_keys = 23;
	float keypressed [nb_keys];

	//PRINTING
	bool PRINT = ON;
	bool PRINT_CALIB = ON;

	//PLOT
	bool PLOT_DATA = ON;

	const int NB_SAMPLES_BY_PCKT = 9;
	const int NB_SAMPLES_TO_PLOT = 11 * NB_SAMPLES_BY_PCKT;
	const int NB_SCOPE_SAMPLES = 50 * NB_SAMPLES_TO_PLOT;
	
	const int NB_TKE_TO_PLOT = 15;
	const int NB_SCOPE_TKE = 50 * NB_TKE_TO_PLOT;

	const int NB_IMU_TO_PLOT = 5;
	const int NB_SCOPE_IMU = 50 * NB_IMU_TO_PLOT;

	Control_JACO CTRL_IMU;
	float new_emg[NB_SAMPLES_TO_PLOT]; int i_nemg = 0; 
	float new2_emg[NB_SAMPLES_TO_PLOT]; int i_n2emg = 0;
	float new_tke[NB_TKE_TO_PLOT]; int i_ntke = 0;
	float th = 0;
	float new_imu[4];

	void update_plot(void);
	void update_console(void);
	int send_cmd_JACO(void);
	int check_keypad(void);
	void change_mode_A(char value);
	int ReadJACOMode(void);

	//PRINTING
	bool PRINT = 1;
	int flag=0;
	int c;
	
	
	
	/* SOME DEFINITION */ 
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
/* keyboard */
struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}
void set_conio_terminal_mode()
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
int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

	/* END */
	[STAThread]
	int main()
	{
		
		printf(" ____     ___  ____  _____    _____  _____  _____  ____  ____  _____ _ \n");
		printf("|_   |   /   ||  __||  _  |  |  _  ||  _  \|  _  ||_   ||  _ ||  __|| |_\n");
		printf("  |  |  / _  || |   | | | |  | |_| || |_| || | | |  |  || |_  | |   |  _|\n");
		printf("  |  | / |_| || |   | | | |  |  ___||  _  /| | | |  |  ||  _| | |   | | \n");
		printf(" _/  |/  / | || |__ | |_| |  | |    | | \ \| |_| | _/  || |__ | |__ | \__\n");
		printf("|___/|__/  |_||____||_____|  |_|    |_|  \_\_____||___/ |____||____| \___|\n");
		//system("start /min /b XBOX\\JoyToKey.exe");
		//Console::WriteLine("JoyToKey started");

		CTRL_IMU.initialize();
		if (PLOT_DATA)
		{
			PLOT_EMG.initialize(EMG_SCOPE, 1, NB_SCOPE_SAMPLES, NB_SAMPLES_TO_PLOT, 2);
			PLOT_TKE.initialize(TKE_SCOPE, 2, NB_SCOPE_TKE, NB_TKE_TO_PLOT, 2);
			PLOT_IMU.initialize(IMU_SCOPE, 3, NB_SCOPE_IMU, NB_IMU_TO_PLOT, 2);
		}

		for (int i_k = 0; i_k < nb_keys; i_k++)
			keypressed[i_k] = 0;

		int cpt = 0; int int_kyp = 0;
		result=system("ls -l /dev/ttyUSB*");
		if (result!=0){
			std::cout <<"\n no USB devices were found\n" << std::endl;
			return 0;
		}
		else {
			printf("\nEnter /dev/ttyUSB* wished \n");
			std::cin >> usb_name;
			usb_dev=usb_name.c_str();
		}
		mySerial serial1(usb_dev,Baudrate);
		if (serial1.IsOpen()){
			printf("USB Devices is opened... \nStart Acquisition?  YES[Y]  |  NO[N]\n");
			while ((ch_kyp != 'Y') && (ch_kyp != 'y') && (ch_kyp != 'N') && (ch_kyp != 'n'))
			{
				std::cin >> ch_kyp;
			}
			if (ch_kyp == 'Y' || ch_kyp == 'y')
			{
				system("cls");
				printf("Acquisition STARTED...\n");
			}
		}	
		
		while (1)
		{
			set_conio_terminal_mode();
			if (kbhit()){
				c=getch();
			}
			reset_terminal_mode();
			while (serialPort1->ReadByte() != S_Of_F);
			refresh++;

			for (cpt = 0; cpt < Packet_Size - 1; cpt++)
			{
				PCKT[cpt] = serialPort1->ReadByte();
			}

			if (PCKT[Packet_Size - 2] == E_Of_F)
			{
				/*if (Acq_Started == 0)
				{
					Acq_Started = 1;
					printf("Acquisition Started Succesfully\n");
				}*/
				
				CTRL_IMU.process_payload(PCKT);
				int res_update = CTRL_IMU.update_calibration();//renvoie 1 à la fin d'une calibration
				if (res_update)
					PRINT = 1;
				update_plot();
				update_console();				
			}
			change_mode_A(CTRL_IMU.EMG_1.EMG_DETECTED);
			//CTRL_IMU.EMG_1.EMG_DETECTED = 0;
			//serialPort1->ReadExisting();
			int etat = send_cmd_JACO ();
			int touch = check_keypad ();
		}
#pragma endregion
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void change_mode_A(char value)
	{
		if (value == 1)
		{
			if (ControlJACO == 1 && CTRL_IMU.EMG_CALIBRATED)
			{
				CTRL_IMU.MODE_JACO += 1;
				if (CTRL_IMU.MODE_JACO == NB_MODES_JACO)
					CTRL_IMU.MODE_JACO = 0;
				
				CTRL_IMU.B_INDEX1 = XYZ_BUTTON2;
				CTRL_IMU.B_INDEX2 = XYZ_BUTTON3;

				if (EMG_CONTRACTION_IS_SHORT)
				{
					printf("short contraction detected...\n");
					(*MySendJoystickCommand)(CTRL_IMU.PushButton(CTRL_IMU.B_INDEX1));
					Sleep(50);
					(*MySendJoystickCommand)(CTRL_IMU.ReleaseButton(CTRL_IMU.B_INDEX1));
				}
				else if (EMG_CONTRACTION_IS_LONG)
				{
					printf("long contraction detected...\n");
					(*MySendJoystickCommand)(CTRL_IMU.PushButton(CTRL_IMU.B_INDEX2));
					Sleep(50);
					(*MySendJoystickCommand)(CTRL_IMU.ReleaseButton(CTRL_IMU.B_INDEX2));
				}
				RESET_CONTRACT_CHRONO;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void update_plot(void)
	{
		if (PLOT_DATA)
		{
			if (PCKT[29] == HEADSET)
			{
				new_imu[0] = CTRL_IMU.HEAD_IMU.pitchctrl;
				new_imu[1] = CTRL_IMU.HEAD_IMU.rollctrl;
				new_imu[2] = CTRL_IMU.HEAD_IMU.yawctrl;
				PLOT_IMU.update(new_imu, 3, 1);
			}
			else if (PCKT[29] == REFERENCE)
			{
			}
			else if (PCKT[29] == EMG1)
			{
				if (CTRL_IMU.EMG_1.NEW_CALIB)
				{
					CTRL_IMU.EMG_1.NEW_CALIB = 0;
					PLOT_TKE.update(CTRL_IMU.EMG_1.THRESHOLD, PLOT_TKE.nb_data_to_plot, 2);
					PLOT_TKE.update(CTRL_IMU.EMG_1.UPPER_THRESHOLD, CTRL_IMU.EMG_1.LOWER_THRESHOLD, PLOT_TKE.nb_data_to_plot, 3);
					CTRL_IMU.EMG_1.NEW_CALIB = 0;
				}
				for (int pemg = 0; pemg < NB_SAMPLES_BY_PCKT; pemg++)
				{
					new_emg[i_nemg + pemg] = CTRL_IMU.EMG_1.DATA_EMG_FILT[pemg] * 5;
					if (new_emg[i_nemg + pemg] >= 100)
						new_emg[i_nemg + pemg] = 100;
					else if (new_emg[i_nemg + pemg] <= -100)
						new_emg[i_nemg + pemg] = -100;
				}
				i_nemg += NB_SAMPLES_BY_PCKT;

				new_tke[i_ntke] = CTRL_IMU.EMG_1.MEAN_TKE_CURRENT;
				i_ntke += 1;

				if (i_nemg >= PLOT_EMG.nb_data_to_plot)
				{
					PLOT_EMG.update(new_emg, PLOT_EMG.nb_data_to_plot, 1);
					i_nemg = 0;
				}
				if (i_ntke >= PLOT_TKE.nb_data_to_plot)
				{
					PLOT_TKE.update(new_tke, PLOT_TKE.nb_data_to_plot, 1);
					PLOT_TKE.update(CTRL_IMU.EMG_1.THRESHOLD, PLOT_TKE.nb_data_to_plot, 2);
					PLOT_TKE.update(CTRL_IMU.EMG_1.UPPER_THRESHOLD, CTRL_IMU.EMG_1.LOWER_THRESHOLD, PLOT_TKE.nb_data_to_plot, 3);
					i_ntke = 0;
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void update_console(void)
	{
		if (START_TIME != 0)
			END_TIME = omp_get_wtime() - START_TIME;
		START_TIME = omp_get_wtime();

		if (refresh == elapsed)
		{
			if (PRINT)
			{
				system("cls");
				printf("_______________________________________\n");

				if (ControlJACO == 1)
					printf("API SET\n");
				else
					printf("API CLOSED\n");

				if (Bouton_Hold == 1)
					printf("CONTROL SET\n");
				else
					printf("CONTROL STOPPED\n");

				printf("***************************************\n");
				if (CTRL_IMU.HEAD_IMU_STATE < CTRL_IMU.limit_missing)
				{
					printf("FREQ HEADSET = %d Hz\n", CTRL_IMU.HEAD_IMU.FREQ);
					printf("RAW  : PITCH = %d __ ROLL  = %d __ YAW  = %d | [deg]\n", (int)CTRL_IMU.HEAD_IMU.pitchraw, (int)CTRL_IMU.HEAD_IMU.rollraw, (int)CTRL_IMU.HEAD_IMU.yawraw);
					printf("CTRL : PITCH = %d __ ROLL  = %d __ YAW  = %d | [deg]\n", (int)CTRL_IMU.HEAD_IMU.pitchctrl, (int)CTRL_IMU.HEAD_IMU.rollctrl, (int)CTRL_IMU.HEAD_IMU.yawctrl);
					printf("     : AMP   = %f __ THETA = %f | [deg]\n", (float)CTRL_IMU.Amp, (float)CTRL_IMU.Theta);
					if (CTRL_IMU.IMU_CALIBRATED)
						printf("HEADSET CALIBRATED\n");
					else
						printf("HEADSET NOT CALIBRATED\n");
				}
				else
					printf("HEADSET SENSOR NOT DETECTED\n");
				printf("***************************************\n");
				if (CTRL_IMU.CHAIR_IMU_STATE < CTRL_IMU.limit_missing)
				{
					printf("FREQ CHAIR  = %d Hz\n", CTRL_IMU.CHAIR_IMU.FREQ);
					printf("HEADING JACO  : %d [deg]\n", (int)CTRL_IMU.CHAIR_IMU.yawraw);
					printf("CHAIR VELOCITY = %d deg/s   |  THRESHOLD = %d deg/s\n", (int)CTRL_IMU.CHAIR_IMU.MeanVelYaw, (int)CTRL_IMU.CHAIR_IMU.ThRotReference);
				}
				else
					printf("REFERENCE SENSOR NOT DETECTED\n");
				printf("***************************************\n");
				if (CTRL_IMU.EMG_1_STATE < CTRL_IMU.limit_missing)
				{
					printf("FREQ EMG  = %d Hz\n", CTRL_IMU.EMG_1.FREQ * 9);
					printf("CURRENT TKE  = %f  |  THRESHOLD TKE = %f\n", CTRL_IMU.EMG_1.MEAN_TKE_CURRENT, CTRL_IMU.EMG_1.THRESHOLD);
					printf("UPPER THRES. = %f  |  LOWER THRES   = %f\n", CTRL_IMU.EMG_1.UPPER_THRESHOLD, CTRL_IMU.EMG_1.LOWER_THRESHOLD);
					printf("DETECTION EMG = %d |  CONTRACTION DURATION = %d\n ", CTRL_IMU.EMG_1.EMG_DETECTED, CTRL_IMU.EMG_1.time_contraction);
					if (CTRL_IMU.EMG_CALIBRATED)
						printf("EMG CALIBRATED\n");
					else
						printf("EMG NOT CALIBRATED\n");
				}
				else
					printf("EMG SENSOR NOT DETECTED\n");
				printf("***************************************\n");
				//printf("PARAMS : Ctrl Z = %d | Ref active = %d | TH ROT CHAIR = %d deg/s (window = %d)\n", CTRL_IMU.ZActive, CTRL_IMU.ReferenceActive, (int)CTRL_IMU.CHAIR_IMU.ThRotReference, CTRL_IMU.CHAIR_IMU.nb_val_gyr);
				if (CTRL_IMU.CHAIR_IMU_STATE < CTRL_IMU.limit_missing)
					printf("HEADING OFFSET = %d\n", (int)CTRL_IMU.Theta_offset);
				//Console::WriteLine("DIRECTION = " + Convert::ToString(CTRL_IMU.DIRECTION)); //+ " ____ CmdYout = " + Convert::ToString(CJACO.CmdXout));
				printf("CMDX  = %f | CMDY = %f | CMDZ = %f \n", CTRL_IMU.CmdX_new, CTRL_IMU.CmdY_new, CTRL_IMU.CmdZ_new);
				if (PRINT_CALIB)
				{
					printf("***************************************\n");
					Console::WriteLine("THETA     = " + Convert::ToString(CTRL_IMU.Theta) + "|  AMP = " + Convert::ToString(CTRL_IMU.Amp)); 
					Console::WriteLine("MODE ALGO = " + Convert::ToString(CTRL_IMU.MODE_JACO) + "|  MODE ROBOT = " + Convert::ToString(CTRL_IMU.MODE_JACO_RETURNED));
					printf("_______________________________________\n");
					Console::WriteLine("MIN FORW  = " + Convert::ToString(CTRL_IMU.AminForw) + "|  MIN BACK  = " + Convert::ToString(CTRL_IMU.AminBack));
					Console::WriteLine("MAX FORW  = " + Convert::ToString(CTRL_IMU.AmaxForw) + "|  MAX BACK  = " + Convert::ToString(CTRL_IMU.AmaxBack));
					Console::WriteLine("YAW FORW  = " + Convert::ToString(CTRL_IMU.YawCtrlForw) + "|  YAW BACK  = " + Convert::ToString(CTRL_IMU.YawCtrlBack));
					printf("_______________________________________\n");
					Console::WriteLine("MIN RIGHT = " + Convert::ToString(CTRL_IMU.AminRight) + "|  MIN LEFT  = " + Convert::ToString(CTRL_IMU.AminLeft));
					Console::WriteLine("MAX RIGHT = " + Convert::ToString(CTRL_IMU.AmaxRight) + "|  MAX LEFT  = " + Convert::ToString(CTRL_IMU.AmaxLeft));
					Console::WriteLine("YAW RIGHT  = " + Convert::ToString(CTRL_IMU.YawCtrlRight) + "|  YAW LEFT  = " + Convert::ToString(CTRL_IMU.YawCtrlLeft));
					printf("_______________________________________\n");
					Console::WriteLine("ROT MIN RIGHT = " + Convert::ToString(CTRL_IMU.RotminRight) + "|  ROT MIN LEFT  = " + Convert::ToString(CTRL_IMU.RotminLeft));
					Console::WriteLine("ROT MAX RIGHT = " + Convert::ToString(CTRL_IMU.RotmaxRight) + "|  ROT MAX LEFT  = " + Convert::ToString(CTRL_IMU.RotmaxLeft));
					Console::WriteLine("AMP ROT RIGHT = " + Convert::ToString(CTRL_IMU.AmpRotRight) + "|  AMP ROT LEFT   = " + Convert::ToString(CTRL_IMU.AmpRotLeft));
					printf("_______________________________________\n"); 
					Console::WriteLine("ZONE FORW  = " + Convert::ToString(CTRL_IMU.ZoneForw) + "|  ZONE BACK  = " + Convert::ToString(CTRL_IMU.ZoneBack));
					Console::WriteLine("ZONE RIGHT = " + Convert::ToString(CTRL_IMU.ZoneRight) + "|  ZONE LEFT  = " + Convert::ToString(CTRL_IMU.ZoneLeft));
				}
			}
			refresh = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	int send_cmd_JACO (void)
	{
		int result;
		if (Bouton_Hold == 1 && ControlJACO == 1 && CTRL_IMU.IMU_CALIBRATED)
		{
			//Prendre en compte l'état du bouton avant d'arrêter un mouvement
			result = (*MySendJoystickCommand)(CTRL_IMU.CommandJACO(1));
			if (result != 1)
				printf("Command not transmitted to JACO, please retry...\n");
		}
		else if (Bouton_Hold == 0 && ControlJACO == 1)
		{
			result = (*MySendJoystickCommand)(CTRL_IMU.CommandJACO(0));
			if (result != 1)
				printf("Command not transmitted to JACO, please retry...\n");
		}
		return result;
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	int check_keypad(void)
	{
		int num_kyp = 0;
		if (kyp_delay != 0)
		{
			kyp_delay++;
			if (kyp_delay == 5)
				kyp_delay = 0;
		}
		/*__________________________________________________________________________________*/
		if (GetAsyncKeyState(SPACEBAR_KY) && kyp_ready && (keypressed[0] == 0))//STOP/START API
		{
			keypressed[0] = 1;
			num_kyp = 1;
			#pragma region STOP/START API
			kyp_delay = 1;
			if (ControlJACO == 1)
			{
				(*MySendJoystickCommand)(CTRL_IMU.CommandJACO(0));
				(*MyMoveHome)();
				(*MyStopControlAPI)();
				int result = (*MyCloseAPI)();
				Console::WriteLine("JACO's control has been stopped...");
				ControlJACO = 0;
				//serialPort1->ReadExisting();
			}
			else
			{
				ControlJACO = 1;
				//1st step = creer, instancier, activer les instances de contrôle de JACO
				//commandLayer_handle = LoadLibrary(L"Kinova.API.UsbCommandLayer.dll");
				commandLayer_handle = LoadLibrary(L"CommandLayerWindows.dll");
				if (commandLayer_handle == NULL)
					printf("Error while loading library. Cannot perform test. Leaving... \n");
				else
					printf("API loaded Successfully... \n");

				//Initialise the function pointer from the API
				MyInitAPI = (int(*)()) GetProcAddress(commandLayer_handle, "InitAPI");
				MyCloseAPI = (int(*)()) GetProcAddress(commandLayer_handle, "CloseAPI");
				MyMoveHome = (int(*)()) GetProcAddress(commandLayer_handle, "MoveHome");
				MySendJoystickCommand = (int(*)(JoystickCommand)) GetProcAddress(commandLayer_handle, "SendJoystickCommand");
				MySetCartesianControl = (int(*)()) GetProcAddress(commandLayer_handle, "SetCartesianControl");
				MyStartControlAPI = (int(*)()) GetProcAddress(commandLayer_handle, "StartControlAPI");
				MyStopControlAPI = (int(*)()) GetProcAddress(commandLayer_handle, "StopControlAPI");
				MyGetQuickStatus = (int(*)(QuickStatus &)) GetProcAddress(commandLayer_handle, "GetQuickStatus");
				MyGetControlMapping = (int(*)(ControlMappingCharts &)) GetProcAddress(commandLayer_handle, "GetControlMapping");
				//Verify that all functions has been loaded correctly
				if ((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyMoveHome == NULL) || (MySendJoystickCommand == NULL) || (MyStartControlAPI == NULL) || (MyStopControlAPI == NULL) || (MySetCartesianControl == NULL) || (MyGetQuickStatus == NULL) || (MyGetControlMapping == NULL))
					printf("Cannot load all the functions from the API. Please reinit the system...\n");
				else
				{
					int result = (*MyInitAPI)();
					if (result == 1)
					{
						printf("The result of InitAPI = 1 \n");
						(*MyMoveHome)();						// Puts JACO in init position
						(*MySetCartesianControl)();
						(*MyStartControlAPI)();
						printf("JACO is ready... have fun ;) \n");
					}
					else
						printf("The result of InitAPI != 1 \n");
				}
			}
		#pragma endregion
		}
		else {keypressed[0] = 0;}
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(U_KY)))//LOCK/UNLOCK KEYPAD
		{
			if (keypressed[1] == 0)
			{
				#pragma region LOCK/UNLOCK KEYPAD
				kyp_delay = 1;
				if (kyp_ready == 0)
				{
					kyp_ready = 1;
					printf("Keyboard Unlocked \n");
				}
				else
				{
					kyp_ready = 0;
					printf("Keyboard Locked \n");
				}
				#pragma endregion
			}
			keypressed[1] = 1;
			num_kyp = 2;
		}
		else {keypressed[1] = 0;}
		/*__________________________________________________________________________________*/
		if (GetAsyncKeyState(D_KY) && kyp_ready)
		{
			if (keypressed[2] == 0)
			{
				#pragma region EN/DISABLE - SET 0
				kyp_delay = 1;
				if (Bouton_Hold == 1)
					Bouton_Hold = 0;
				else
				{
					Bouton_Hold = 1;
					CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
					if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
					{
						PRINT = 0;
						printf("NEUTRAL IMU CALIBRATION...\n");
						CTRL_IMU.HEAD_IMU.CALIB_TYPE = NEUTRAL;
					}
					CTRL_IMU.InhibiteZctrl = 0;
				}
				Console::WriteLine("BOUTON HOLD = " + Bouton_Hold + "\n");
				#pragma endregion
			}
			keypressed[2] = 1;
			num_kyp = 3;
		}
		else {keypressed[2] = 0;}
		/*__________________________________________________________________________________*/
		if (GetAsyncKeyState(O_KY) && kyp_ready)//CHANGE MODE B (BOUTON 3)
		{
			if (keypressed[3] == 0)
			{
				#pragma region CHANGE MODE 2
				kyp_delay = 1;
				if (ControlJACO)
				{
					CTRL_IMU.MODE_JACO += 1;
					if (CTRL_IMU.MODE_JACO == NB_MODES_JACO)
						CTRL_IMU.MODE_JACO = 0;
					
					CTRL_IMU.B_INDEX1 = XYZ_BUTTON3;
					CTRL_IMU.B_INDEX2 = 0;
					(*MySendJoystickCommand)(CTRL_IMU.PushButton(CTRL_IMU.B_INDEX));
					Sleep(50);
					(*MySendJoystickCommand)(CTRL_IMU.ReleaseButton(CTRL_IMU.B_INDEX));
				}
				#pragma endregion
			}
			keypressed[3] = 1;
			num_kyp = 4;
		}
		else {keypressed[3] = 0;}
		/*__________________________________________________________________________________*/
		if (GetAsyncKeyState(P_KY) && kyp_ready)//CHANGE MODE 3 (BOUTON 3)
		{
			if (keypressed[4] == 0)
			{
				#pragma region CHANGE MODE 3
				kyp_delay = 1;
				if (ControlJACO)
				{
					CTRL_IMU.MODE_JACO += 1;
					if (CTRL_IMU.MODE_JACO == NB_MODES_JACO)
						CTRL_IMU.MODE_JACO = 0;

					CTRL_IMU.B_INDEX1 = XYZ_BUTTON2;
					CTRL_IMU.B_INDEX2 = 0;
					(*MySendJoystickCommand)(CTRL_IMU.PushButton(CTRL_IMU.B_INDEX1));
					Sleep(50);
					(*MySendJoystickCommand)(CTRL_IMU.ReleaseButton(CTRL_IMU.B_INDEX1));
				}
				#pragma endregion
			}
			keypressed[4] = 1;
			num_kyp = 5;
		}
		else {keypressed[4] = 0;}
		/*__________________________________________________________________________________*/
		if (GetAsyncKeyState(Z_KY) && kyp_ready)//Calibrate NEUTRAL (BOUTON 5)
		{
			if (keypressed[5] == 0)
			{
				#pragma region CALIB NEUTRAL POSITION
				kyp_delay = 1;
				CTRL_IMU.EMG_1.CALIB_EMG_INDEX = 0;
				if (CTRL_IMU.EMG_1.CALIB_EMG_INDEX != -1)
				{
					PRINT = 0;
					printf("NEUTRAL EMG CALIBRATION...\n");
				}
				#pragma endregion
			}

			keypressed[5] = 1;
			num_kyp = 6;
		}
		else {keypressed[5] = 0;}
		/*__________________________________________________________________________________*/
		if (GetAsyncKeyState(F_KY) && kyp_ready)//ENABLE/DISABLE PRINTING
		{
			if (keypressed[6] == 0)
			{
				kyp_delay = 1;
				if (PRINT == 1)
				{
					PRINT = 0;
					printf("AFFICHAGE INACTIF \n");
				}
				else
				{
					PRINT = 1;
					printf("AFFICHAGE ACTIVF \n");
				}
			}

			keypressed[6] = 1;
			num_kyp = 7;
		}
		else {keypressed[6] = 0;}
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(add_KY) && kyp_ready))//UP ARROW --> INCREASE EMG THRESHOLD
		{
			if (keypressed[7] == 0)
			{
				kyp_delay = 1;
				if (CTRL_IMU.EMG_1.SENSOR_ON)
				{
					if (CTRL_IMU.EMG_1.STD_TKE == 0)
						CTRL_IMU.EMG_1.STD_TKE = 1;
					else
						CTRL_IMU.EMG_1.K_TH++;
					printf("threshold increased...");
					CTRL_IMU.EMG_1.NEW_CALIB = 1;
				}
				keypressed[7] = 1;
				num_kyp = 8;
			}
		}
		else { keypressed[7] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(sub_KY) && kyp_ready))//DOWN ARROW -->	DECREASE EMG THRESHOLD
		{
			if (keypressed[8] == 0)
			{
				kyp_delay = 1;
				if (CTRL_IMU.EMG_1.SENSOR_ON)
				{
					if (CTRL_IMU.EMG_1.K_TH == 1 && CTRL_IMU.EMG_1.STD_TKE != 0)
					{
						CTRL_IMU.EMG_1.STD_TKE--;
						CTRL_IMU.EMG_1.NEW_CALIB = 1;
						printf("threshold decreased...");
					}
					else if (CTRL_IMU.EMG_1.K_TH != 1)
					{
						CTRL_IMU.EMG_1.K_TH--;
						CTRL_IMU.EMG_1.NEW_CALIB = 1;
						printf("threshold decreased...");
					}
					else
						printf("not valid...");
				}
				keypressed[8] = 1;
				num_kyp = 9;
			}
		}
		else { keypressed[8] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(up_arrow) && kyp_ready))//MAX FORTH CALIBRATION
		{
			if (keypressed[9] == 0)
			{
				CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
				if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
				{
					PRINT = 0;
					printf("MAX FORWARD CALIBRATION...\n");
					CTRL_IMU.HEAD_IMU.CALIB_TYPE = MAX_FORTH;
				}
				keypressed[9] = 1;
				num_kyp = 10;
			}
		}
		else { keypressed[9] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(down_arrow) && kyp_ready))//MAX BACK CALIBRATION
		{
			if (keypressed[10] == 0)
			{
				CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
				if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
				{
					PRINT = 0;
					printf("MAX BACKWARD CALIBRATION...\n");
					CTRL_IMU.HEAD_IMU.CALIB_TYPE = MAX_BACK;
				}
				keypressed[10] = 1;
				num_kyp = 11;
			}
		}
		else { keypressed[10] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(right_arrow) && kyp_ready))//MAX RIGHT CALIBRATION
		{
			if (keypressed[11] == 0)
			{
				CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
				if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
				{
					PRINT = 0;
					printf("MAX RIGHT CALIBRATION...\n");
					CTRL_IMU.HEAD_IMU.CALIB_TYPE = MAX_RIGHT;
				}
				keypressed[11] = 1;
				num_kyp = 12;
			}
		}
		else { keypressed[11] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(left_arrow) && kyp_ready))//MAX LEFT CALIBRATION
		{
			if (keypressed[12] == 0)
			{
				CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
				if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
				{
					PRINT = 0;
					printf("MAX LEFT CALIBRATION...\n");
					CTRL_IMU.HEAD_IMU.CALIB_TYPE = MAX_LEFT;
				}
				keypressed[12] = 1;
				num_kyp = 13;
			}
		}
		else { keypressed[12] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(numpad0) && kyp_ready))//NEUTRAL IMU CALIBRATION
		{
			if (keypressed[13] == 0)
			{
				CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
				if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
				{
					PRINT = 0;
					printf("NEUTRAL IMU CALIBRATION...\n");
					CTRL_IMU.HEAD_IMU.CALIB_TYPE = NEUTRAL;
				}
				keypressed[13] = 1;
				num_kyp = 12;
			}
		}
		else { keypressed[13] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(numpad4) && kyp_ready))//NEUTRAL IMU CALIBRATION
		{
			if (keypressed[14] == 0)
			{
				CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
				if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
				{
					PRINT = 0;
					printf("ROT MAX LEFT CALIBRATION...\n");
					CTRL_IMU.HEAD_IMU.CALIB_TYPE = MAX_ROT_LEFT;
				}
				keypressed[14] = 1;
				num_kyp = 15;
			}
		}
		else { keypressed[14] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(numpad6) && kyp_ready))//NEUTRAL IMU CALIBRATION
		{
			if (keypressed[15] == 0)
			{
				CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
				if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
				{
					PRINT = 0;
					printf("ROT MAX RIGHT CALIBRATION...\n");
					CTRL_IMU.HEAD_IMU.CALIB_TYPE = MAX_ROT_RIGHT;
				}
				keypressed[15] = 1;
				num_kyp = 16;
			}
		}
		else { keypressed[15] = 0; }
		/*__________________________________________________________________________________*/
		if (GetAsyncKeyState(E_KY) && kyp_ready)//Z Control disabled/enabled
		{
			if (keypressed[16] == 0)
			{
				kyp_delay = 1;
				if (CTRL_IMU.ZActive == 1)
				{
					CTRL_IMU.ZActive = 0;
					printf("Z Control disabled...\n");
				}
				else if (CTRL_IMU.ZActive == 0)
				{
					CTRL_IMU.ZActive = 1;
					printf("Z Control enabled...\n");
				}
			}

			keypressed[16] = 1;
			num_kyp = 17;
		}
		else { keypressed[16] = 0; }
		/*__________________________________________________________________________________*/
		if (GetAsyncKeyState(V_KY) && kyp_ready)//Validate control heading offset
		{
			if (keypressed[17] == 0)
			{
				CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX = 0;
				if (CTRL_IMU.HEAD_IMU.CALIB_IMU_INDEX != -1)
				{
					PRINT = 0;
					printf("HEADING OFFSET CALIBRATION...\n");
					CTRL_IMU.HEAD_IMU.CALIB_TYPE = HEADING_OFFSET;
					SENSOR_HEADING_COMPUTATION;
					if (CTRL_IMU.CHAIR_IMU.SENSOR_ON)
						CTRL_IMU.ReferenceActive = 1;
				}
				keypressed[17] = 1;
				num_kyp = 18;
			}
		}
		else { keypressed[17] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(numpad8) && kyp_ready))//LOWER_THRESHOLD INCREASE
		{
			if (keypressed[18] == 0)
			{
				if (CTRL_IMU.EMG_1.SENSOR_ON)
				{
					if (CTRL_IMU.EMG_1.LOWER_THRESHOLD + 1 < CTRL_IMU.EMG_1.UPPER_THRESHOLD)
					{
						CTRL_IMU.EMG_1.LOWER_THRESHOLD += 1;
						printf("threshold increased...");
						CTRL_IMU.EMG_1.NEW_CALIB = 1;
					}
					else
						printf("not valid...");
				}
				keypressed[18] = 1;
				num_kyp = 19;
			}
		}
		else { keypressed[18] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(numpad2) && kyp_ready))//LOWER_THRESHOLD DECREASE
		{
			if (keypressed[19] == 0)
			{
				if (CTRL_IMU.EMG_1.SENSOR_ON)
				{
					if (CTRL_IMU.EMG_1.LOWER_THRESHOLD - 1 >= 0)
					{
						CTRL_IMU.EMG_1.LOWER_THRESHOLD -= 1;
						printf("threshold decreased...");
						CTRL_IMU.EMG_1.NEW_CALIB = 1;
					}
					else
						printf("not valid...");
				}
				keypressed[19] = 1;
				num_kyp = 20;
			}
		}
		else { keypressed[19] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(numpad9) && kyp_ready))//UPPER_THRESHOLD INCREASE
		{
			if (keypressed[20] == 0)
			{
				if (CTRL_IMU.EMG_1.SENSOR_ON)
				{
					CTRL_IMU.EMG_1.UPPER_THRESHOLD += 1;
					printf("upper-threshold increased...");
					CTRL_IMU.EMG_1.NEW_CALIB = 1;
				}
				keypressed[20] = 1;
				num_kyp = 21;
			}
		}
		else { keypressed[20] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(numpad3) && kyp_ready))//UPPER_THRESHOLD DECREASE
		{
			if (keypressed[21] == 0)
			{
				if (CTRL_IMU.EMG_1.SENSOR_ON)
				{
					if (CTRL_IMU.EMG_1.UPPER_THRESHOLD - 1 > CTRL_IMU.EMG_1.LOWER_THRESHOLD)
					{
						CTRL_IMU.EMG_1.UPPER_THRESHOLD -= 1;
						printf("upper-threshold decreased...\n");
						CTRL_IMU.EMG_1.NEW_CALIB = 1;
					}
					else
						printf("not valid...");
				}
				keypressed[21] = 1;
				num_kyp = 20;
			}
		}
		else { keypressed[21] = 0; }
		/*__________________________________________________________________________________*/
		if ((GetAsyncKeyState(W_KY) && kyp_ready))//UPPER_THRESHOLD DECREASE
		{
			if (keypressed[22] == 0)
			{
				PLOT_DATA = 0;
				keypressed[22] = 1;
				num_kyp = 21;
			}
		}
		else { keypressed[22] = 0; }
		return num_kyp;
	}

	int ReadJACOMode(void)
	{
		int status_jaco, mode_jaco;
		mode_jaco = 0;
		ControlMappingCharts MappingJACO;
		(*MyGetControlMapping)(MappingJACO);
		mode_jaco = MappingJACO.Mapping[2].ActualModeA;
		return mode_jaco;
	}



