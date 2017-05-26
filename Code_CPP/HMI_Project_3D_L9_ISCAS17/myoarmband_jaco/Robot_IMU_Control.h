#pragma once
#include "KinovaTypes.h"
#include "Defined_Macro.h"
#include <string>
#include <Windows.h>
#include <math.h>

//using namespace System;
//using namespace std;


public class Robot_IMU_Control
{
public:	

	int FREQ; int FREQ_REFERENCE; int FREQ_EMG; int sensor_id;	
	
	//DIRECTION - COMMANDES                                                                                  
	float CmdX_new; float CmdY_new; 
	float CmdXout;	float CmdYout;
	float RightLeftComm; float ForBackComm; float UpDownComm;
	float RightLeftComm_ctrl; float ForBackComm_ctrl; float UpDownComm_ctrl;

	//filters
	static const double num_lp_10Hz[3]; static const double den_lp_10Hz[3];
	static const double num_lp_25Hz[3]; static const double den_lp_25Hz[3];
	
		//last update : Fs_EMG = 1125 Hz
	static const double num_bs_55_65Hz[5]; static const double den_bs_55_65Hz[5];
	static const double num_bp_80_450Hz[5]; static const double den_bp_80_450Hz[5];
	static const double num_bp_20_450Hz[5]; static const double den_bp_20_450Hz[5];

	//Calib
	static const double comp_headset[3][3]; static const double hardiron_headset[3];
	static const double comp_reference[3][3]; static const double hardiron_reference[3];

	//RAW DATA HEADSET
	char HEADSET_ON;
	float acc[3];
	float gyr[6]; // 0:2 --> raw values, 3:5 --> calibrated values
	float mag[6]; // 0:2 --> raw values, 3:5 --> calibrated values
	float pitch[3]; 
	float x_pitch[3];
	float roll[3];  
	float x_roll[3];
	float yaw[3];   
	float x_yaw[3];
	float yaw_ctrl[3];
	float x_yaw_ctrl[3];
	float xh_cal, yh_cal, zh_cal;
	float xh_ctrl, yh_ctrl, zh_ctrl;

	//RAW DATA REFERENCE
	char REFERENCE_ON = 0;
	float acc_ref[3];
	float gyr_ref[6]; // 0:2 --> raw values, 3:5 --> calibrated values
	float mag_ref[6]; // 0:2 --> raw values, 3:5 --> calibrated values
	float pitch_ref[3];
	float x_pitch_ref[3];
	float roll_ref[3];
	float x_roll_ref[3];
	float yaw_ref[3];
	float x_yaw_ref[3];
	float xh_cal_ref, yh_cal_ref, zh_cal_ref;

	static const int nb_val_gyr_ref = 30;
	float moy_gyr_ref[nb_val_gyr_ref];
	int index_gyr_ref;
	float MeanVelRef;

	//DATA EMG
	char EMG_ON = 0;
	char index_EMG = 0;
	static const int NB_SAMPLES_IN_PCKT = 9;
	int DATA_EMG[NB_SAMPLES_IN_PCKT];
	int DATA_EMG_FILT[NB_SAMPLES_IN_PCKT];
	double x_emg[5]; double y_emg[5]; double z_emg[5]; //FILTER

	//DATA TKE + ENV
	int DATA_TKE [NB_SAMPLES_IN_PCKT];
	static const int NB_SAMPLES_IN_WINDOW = 81;
	static const int NB_SAMPLES_OVERLAP = 45;
	int INDEX_WINDOW;
	int DATA_ENV [NB_SAMPLES_IN_WINDOW];
	float MEAN_TKE_CURRENT;
	double emg [3];
	char NEW_TKE;

	//DATA CALIB + DETECT
	float MEAN_TKE; float STD_TKE; int K_TH;
	static const int NB_SAMPLES_CALIB = 100 * NB_SAMPLES_IN_PCKT;
	int DATA_TKE_CALIB [NB_SAMPLES_CALIB];
	int CALIB_EMG_INDEX;
	char EMG_DETECTED;
	static const char nb_detect = 10;  char EMG_BUTTON [nb_detect]; char i_button ;
	char NEW_CALIB;

	//Data 2D --> 3D
	float x;									//roll + offset
	float y;									//pitch + offset
	float x0;									//roll brute
	float y0;									//pitch brute	
	float xoff;									//offset - position neutre roll
	float yoff;									//offset - position neutre pitch
	float z0;
	float z_ctrl;
	float zoff;
	float z0_ref;
	int InhibiteZctrl;

	float Amp;
	float Theta;
	float Orientation;

	int ZActive = 1;
	int ReferenceActive = 0;						//indique l'acitivation du noeud de référence
	int Valid = 1;									//indique la validité des marges et zones définies
	int DiagoActive = 1;							//0 = diagonale désactivée, 1 = diagonale activée
	float Theta_offset = 0;							//0 .. 360 degrés, mapping
	float ThRotReference = 30;						//vitesse de rotation seuil maximale du fauteuil
	//////////////////////////////////////////////////////////////////////////////////////////////////
	float Amin_BACK = 3;
	float Amax_BACK = 20;
	float Amin_D = 3;
	float Amax_D = 50;
	float Amax = 50;								//inclinaison maximale
	float Amin = 7;									//seuil d'inclinaison
	float ZoneForw = 90;	float DeltaForw = 20;	//Angle Forward	, Marge Forwad
	float ZoneRight = 0;	float DeltaRight = 30;	//Angle Right	, Marge Right
	float ZoneLeft = 180;	float DeltaLeft = 30;	//Angle Left	, Marge Left
	float ZoneBack = 270;	float DeltaBack = 20;	//Angle Backward, Marge Backward

	float MaxUp = 20;		float DeltaMaxUp = 5;	//Yaw Angle Right (Move Up)	, Marge
	float MaxDown = -20;	float DeltaMaxDown = 5;//Yaw Angle Left (Mode Down), Marge
	float NeutralUpDown = 0; float DeltaNeutralUpDown = 10;
	////////////////////////////////////////////////////////////////////////////////////////////////

	//BUTTONS                                                                                                   
	int PRESSED;	
	int pressedtime;
	int B_INDEX;																						
	bool HOME_PRESSED;
	bool BACK;

	Robot_IMU_Control();
	~Robot_IMU_Control();
	void initialize(void);
	void set_default_calibration(void);
	void initialize_filters(void);
	void read_payload(int data[]);
	void filter_emg(void);
	void process_tke(void);
	void detect_tke(void);
	char min_tabc(char* tab, char size);
	void calibrate_imu(void);
	void sensor_fusion(void);
	int detect_wheelchair_rotation(void);
	void algo(void);
	JoystickCommand PushButton(char buttonindex);
	JoystickCommand ReleaseButton(char buttonindex);
	JoystickCommand Control(int b);
	float C2_2(int var);
	float C2_3(int var);
	void wrap_angle(float * p_angle);
	float rad2deg(float angle_rad);
	float deg2rad(float angle_deg);
	float absolute(float var);
	float mean_tab(float * tab, int taille);
	float mean_tabi(int * tab, int taille);

};