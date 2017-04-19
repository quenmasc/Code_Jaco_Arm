#define _USE_MATH_DEFINES

#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include "stdafx.h"
#include "KinovaTypes.h"
#include "omp.h"
#include "Robot_IMU_Control.h"
#include "Defined_Macro.h"

#define SPACEBAR 32
#define U 85
#define Q 81
#define W 87
#define E 69
#define D 68
#define F 70
#define Z 90
#define O 79
#define P 80
#define u 117
#define q 113
#define w 119
#define e 101
#define d 100
#define f 102
#define z 122
#define o 111
#define p 112

namespace Control_IMU_JACO
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;
	using namespace System::Threading;
	using namespace std;

	HINSTANCE commandLayer_handle;
	static int(*MyInitAPI)();
	static int(*MyCloseAPI)();
	static int(*MyMoveHome)();
	static int(*MyInitFingers)();
	static int(*MySendAdvanceTrajectory)(TrajectoryPoint);
	static int(*MyGetQuickStatus)(QuickStatus &);
	static int(*MySendJoystickCommand)(JoystickCommand);
	static int(*MySetCartesianControl)();
	static int(*MyStartControlAPI)();
	static int(*MyStopControlAPI)();
	
	const int Packet_Size = 32;
	int PCKT[Packet_Size - 1];
	static int S_Of_F = 51;
	static int E_Of_F = 52;
	int Baudrate = 115200;
	Robot_IMU_Control CJACO;
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
	double GLOBAL_TIME;
	double INIT_TIME;
	int Sec, Min, Hr;
	int refresh = 0;
	int elapsed = 0;

	//KEYPAD
	int in_kyp; char ch_kyp;
	bool kyp_ready = 0;
	int kyp_delay = 0;
	
	float keypressed[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	//PRINTING
	bool PRINT = 1;

//#pragma intrinsic(radians)

	[STAThread]
	int main()
	{
		std::random_device                  rand_dev;
		std::mt19937                        generator(rand_dev());
		
		system("start /min /b XBOX\\JoyToKey.exe");
		
		Console::WriteLine("JoyToKey started");

#pragma region PORT
		int cpt = 0; int int_kyp = 0;
		String^ Port_COM;
		SerialPort^  serialPort1;
		serialPort1 = gcnew SerialPort();
		
		//TEST
		//Console::WriteLine(Convert::ToString(omp_get_wtime()));

		Console::WriteLine("Available Ports:");
		for each (String^ s in SerialPort::GetPortNames())
		{
			Console::WriteLine("   {0}", s);
		}
		Console::Write("Entrez le port COM: \n");
		Port_COM = Convert::ToString(Console::ReadLine());
		
		serialPort1->BaudRate = 1998848;
		serialPort1->PortName = Port_COM;
		serialPort1->ReadTimeout = 100000;
		serialPort1->Open();
#pragma endregion		
		if (serialPort1->IsOpen)
		{
			Console::WriteLine("Port COM ouvert \nAppuyez sur W pour démarrer l'acquisition \n");
			while ((ch_kyp != 'W') && (ch_kyp != 'w'))
			{
				std::cin >> ch_kyp;
			}
			Console::WriteLine("ACQUISITION DÉMARRÉE\n");
		}
		//////////////
		elapsed = 20;
		/////////////
		#pragma region LOOP		
		
		while (1)
		{
			#pragma region READ PACKET
			while (serialPort1->ReadByte() != S_Of_F);
			refresh++;
			//READING BUFFER 
			for (cpt = 0; cpt < Packet_Size - 1; cpt++)
			{
				PCKT[cpt] = serialPort1->ReadByte();
			}

			if (PCKT[Packet_Size - 2] == E_Of_F)
			{
				if (Acq_Started == 0)
				{
					Acq_Started = 1;
					Console::WriteLine("Acquisition Started Succesfully\n");
				}
				
				CJACO.read_payload(PCKT);
				CJACO.sensor_fusion();
				CJACO.algo();

				if (refresh == elapsed)
				{
					if (PRINT)
					{
						Console::WriteLine("_______________________________________");
						
						if (ControlJACO == 1)
							Console::WriteLine("API OUVERTE");
						else
							Console::WriteLine("API FERMÉE");
						if (Bouton_Hold == 1)
							Console::WriteLine("CONTROLE DÉMARRÉ");
						else
							Console::WriteLine("CONTROLE ARRETÉE");
						if (ControlJACO == 1)
							Console::WriteLine("PERIODE ENVOI CMD = " + Convert::ToString(END_TIME * 1000) + " ms");

						Console::WriteLine("FREQUENCE CAPTEUR = " + Convert::ToString(CJACO.FREQ) + " Hz");
						//Console::WriteLine("FREQUENCE BASE    = " + Convert::ToString(CJACO.FREQ_BASE) + " Hz");
						//Console::WriteLine("PITCH = " + Convert::ToString(CJACO.y) + " ____ ROLL = " + Convert::ToString(CJACO.x) + " ____ YAW = " + Convert::ToString(CJACO.yaw));
						Console::WriteLine("PITCH = " + Convert::ToString(CJACO.y) + " ____ ROLL = " + Convert::ToString(CJACO.x));
						Console::WriteLine("FORBACK = " + Convert::ToString(CJACO.ForBackComm) + " ____ RIGHTLEFT = " + Convert::ToString(CJACO.RightLeftComm));
						Console::WriteLine("UpDownComm = " + Convert::ToString(CJACO.UpDownComm));
						
						//Console::WriteLine("YAW_FUSED_SENSOR = " + Convert::ToString(CJACO.yaw_fused_sensor) + " ____ YAW_FUSED_CPP = " + Convert::ToString(CJACO.yaw));
						//Console::WriteLine("GYROX = " + Convert::ToString(CJACO.gyrox));
						
						//Console::WriteLine("TEST RAD = " + Convert::ToString(radians(90)));
						//Console::WriteLine("CmdXout = " + Convert::ToString(CJACO.CmdXout) + " ____ CmdYout = " + Convert::ToString(CJACO.CmdXout));
						//Console::WriteLine("CmdX_new = " + Convert::ToString(CJACO.CmdX_new) + " ____ CmdY_new = " + Convert::ToString(CJACO.CmdX_new));
						//Console::WriteLine("Theta_offset = " + Convert::ToString(CJACO.Theta_offset));
					}
					refresh = 0;
				}
			}
			#pragma endregion
			#pragma region CONTROL
			if (Bouton_Hold == 1 && ControlJACO == 1)
			{
				//Prendre en compte l'état du bouton avant d'arrêter un mouvement
				int result = (*MySendJoystickCommand)(CJACO.Control(1));
				if (result != 1)
					Console::WriteLine("Command not transmitted to JACO, please retry...");
			}
			else if (Bouton_Hold == 0 && ControlJACO == 1)
			{
				int result = (*MySendJoystickCommand)(CJACO.Control(0));
				if (result != 1)
					Console::WriteLine("Command not transmitted to JACO, please retry...");
			}
			/*if (ControlJACO == 1)
			{
				if (START_TIME != 0)
					END_TIME = omp_get_wtime() - START_TIME;
				START_TIME = omp_get_wtime();
			}*/
			#pragma endregion
			#pragma region KEYPRESS CALIBRATION/SETTING
			#pragma region REFRESH KYP
			if (kyp_delay != 0)
			{
				kyp_delay++;
				if (kyp_delay == 5)
					kyp_delay = 0;
			}
			#pragma endregion


			if (GetAsyncKeyState(SPACEBAR) && kyp_ready && (keypressed[0] == 0))//STOP/START API
			{
				keypressed[0] = 1;

				#pragma region STOP/START API
				kyp_delay = 1;
				if (ControlJACO == 1)
				{
					(*MySendJoystickCommand)(CJACO.Control(0));
					(*MyMoveHome)();
					(*MyStopControlAPI)();
					int result = (*MyCloseAPI)();
					Console::WriteLine("JACO's control has been stopped...");
					ControlJACO = 0;
					serialPort1->ReadExisting();
				}
				else
				{
					ControlJACO = 1;

					//1st step = creer, instancier, activer les instances de contrôle de JACO
					//commandLayer_handle = LoadLibrary(L"Kinova.API.UsbCommandLayer.dll");
					commandLayer_handle = LoadLibrary(L"CommandLayerWindows.dll");
					if (commandLayer_handle == NULL)
						std::cout << "Error while loading library. Cannot perform test. Leaving,,, \n";
					else
						std::cout << "API loaded Successfully \n";

					//Initialise the function pointer from the API
					MyInitAPI = (int(*)()) GetProcAddress(commandLayer_handle, "InitAPI");
					MyCloseAPI = (int(*)()) GetProcAddress(commandLayer_handle, "CloseAPI");
					MyMoveHome = (int(*)()) GetProcAddress(commandLayer_handle, "MoveHome");
					MySendJoystickCommand = (int(*)(JoystickCommand)) GetProcAddress(commandLayer_handle, "SendJoystickCommand");
					MySetCartesianControl = (int(*)()) GetProcAddress(commandLayer_handle, "SetCartesianControl");
					MyStartControlAPI = (int(*)()) GetProcAddress(commandLayer_handle, "StartControlAPI");
					MyStopControlAPI = (int(*)()) GetProcAddress(commandLayer_handle, "StopControlAPI");
					MyGetQuickStatus = (int(*)(QuickStatus &)) GetProcAddress(commandLayer_handle, "GetQuickStatus");

					//Verify that all functions has been loaded correctly
					if ((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyMoveHome == NULL) || (MySendJoystickCommand == NULL) || (MyStartControlAPI == NULL) || (MyStopControlAPI == NULL) || (MySetCartesianControl == NULL) || (MyGetQuickStatus == NULL))
						std::cout << "Cannot load all the functions from the API. Please reinit the system \n";
					else
					{
						int result = (*MyInitAPI)();
						if (result == 1)
						{
							std::cout << "The result of InitAPI = 1 \n";
							(*MyMoveHome)();						// Puts JACO in init position
							(*MySetCartesianControl)();
							(*MyStartControlAPI)();
							std::cout << "JACO is ready... have fun ;) \n";
						}
						else
							std::cout << "The result of InitAPI != 1 \n";
					}
				}
				#pragma endregion
			}
			else
			{
				keypressed[0] = 0;
			}
			if ((GetAsyncKeyState(U) || GetAsyncKeyState(u)))//LOCK/UNLOCK KEYPAD
			{
				if (keypressed[1] == 0)
				{
					#pragma region LOCK/UNLOCK KEYPAD
					kyp_delay = 1;
					if (kyp_ready == 0)
					{
						kyp_ready = 1;
						Console::WriteLine("Keyboard Unlocked \n");
					}
					else
					{
						kyp_ready = 0;
						Console::WriteLine("Keyboard Locked \n");
					}
					#pragma endregion
				}
				keypressed[1] = 1;
			}
			else
			{
				keypressed[1] = 0;
			}
			if ((GetAsyncKeyState(D) || GetAsyncKeyState(d)) && kyp_ready)
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
						CJACO.xoff = CJACO.x0;
						CJACO.yoff = CJACO.y0;
						Console::WriteLine("NEUTRAL PITCH = " + CJACO.x0 + "\n");
						Console::WriteLine("NEUTRAL ROLL  = " + CJACO.x0 + "\n");
					}
					Console::WriteLine("BOUTON HOLD = " + Bouton_Hold + "\n");
					#pragma endregion
				}

				keypressed[2] = 1;
			}
			else
			{
				keypressed[2] = 0;
			}
			if ((GetAsyncKeyState(O) || GetAsyncKeyState(o)) && kyp_ready)//CHANGE MODE 2 (BOUTON 1)
			{
				if (keypressed[3] == 0)
				{
					#pragma region CHANGE MODE 2
					kyp_delay = 1;
					CJACO.PRESSED = 1;
					CJACO.B_INDEX = XY_BUTTON1;
					#pragma endregion
				}
				keypressed[3] = 1;
			}
			else
			{
				keypressed[3] = 0;
			}
			if ((GetAsyncKeyState(P) || GetAsyncKeyState(p)) && kyp_ready)//CHANGE MODE 3 (BOUTON 3)
			{
				if (keypressed[4] == 0)
				{
					#pragma region CHANGE MODE 3
					kyp_delay = 1;
					CJACO.PRESSED = 1;
					CJACO.B_INDEX = XY_BUTTON3;
					#pragma endregion
				}
				keypressed[4] = 1;
			}
			else
			{
				keypressed[4] = 0;
			}
			if ((GetAsyncKeyState(Z) || GetAsyncKeyState(z)) && kyp_ready)//Calibrate NEUTRAL (BOUTON 5)
			{
				if (keypressed[5] == 0)
				{
					#pragma region CALIB NEUTRAL POSITION
					kyp_delay = 1;
					CJACO.xoff = CJACO.x0;
					CJACO.yoff = CJACO.y0;
					//CJACO.yawoff = CJACO.yaw;
					Console::WriteLine("NEUTRAL PITCH = " + CJACO.x0 + "\n");
					Console::WriteLine("NEUTRAL ROLL  = " + CJACO.y0 + "\n");
					#pragma endregion
				}

				keypressed[5] = 1;
			}
			else
			{
				keypressed[5] = 0;
			}
			if ((GetAsyncKeyState(F) || GetAsyncKeyState(f)) && kyp_ready)//ENABLE/DISABLE PRINTING
			{
				if (keypressed[6] == 0)
				{
					#pragma region PRINTING
					kyp_delay = 1;
					if (PRINT == 1)
					{
						PRINT = 0;
						Console::WriteLine("AFFICHAGE INACTIF \n");
					}
					else
					{
						PRINT = 1;
						Console::WriteLine("AFFICHAGE ACTIVÉ \n");
					}
					#pragma endregion
				}

				keypressed[6] = 1;
			}
			else
			{
				keypressed[6] = 0;
			}



		#pragma endregion
		}
#pragma endregion
		return 0;
	}
}


