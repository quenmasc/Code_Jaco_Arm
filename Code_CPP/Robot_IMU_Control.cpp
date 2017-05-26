#include "Robot_IMU_Control.h"
namespace KinDrv{
#pragma region FILTERS/CALIB
const double Robot_IMU_Control::num_lp_10Hz[3] = { 0.355, 0.355, 0.0 };
const double Robot_IMU_Control::num_lp_25Hz[3] = { 0.755, 0.755, 0.0 };
const double Robot_IMU_Control::den_lp_10Hz[3] = { 1.0, -0.2905, 0.0 };
const double Robot_IMU_Control::den_lp_25Hz[3] = { 1.0, 0.5095, 0.0 };

//Fs_EMG = 1125 Hz
const double Robot_IMU_Control::den_bp_20_450Hz[5] = { 1.0, -0.706332727403145, -0.823332488588912, 0.205925223216761, 0.354276046972334 };
const double Robot_IMU_Control::num_bp_20_450Hz[5] = { 0.588587106635191, 0.0, -1.17717421327038, 0.0, 0.588587106635191 };

const double Robot_IMU_Control::den_bp_80_450Hz[5] = { 1.0, -0.250198547923143, -0.566207313245305, 0.0213077489872791, 0.233041463693116 };
const double Robot_IMU_Control::num_bp_80_450Hz[5] = { 0.454876458653252, 0.0, -0.909752917306504, 0.0, 0.454876458653252 };

const double Robot_IMU_Control::den_bs_55_65Hz[5] = { 1.0, -3.70439515632780, 5.35297699455272, -3.56089634373912, 0.924054185873028 };
const double Robot_IMU_Control::num_bs_55_65Hz[5] = { 0.961277079385909, -3.63264575003346, 5.35447702165393, -3.63264575003346, 0.961277079385909 };

const double Robot_IMU_Control::comp_headset[3][3] = { { 0.000168679716274983, -5.17710741425967e-06, -8.77884394210979e-07 },
{ -5.17710741425967e-06, 0.000159571222440490, -3.27611632378113e-06 },
{ -8.77884394210986e-07, -3.27611632378113e-06, 0.000166456977421752 } };

const double Robot_IMU_Control::hardiron_headset[3] = { -725.100317519760, 491.703440889862, -53.3108814506022 };

const double Robot_IMU_Control::comp_reference[3][3] = { { 0.000161495931045042, -4.43127013254319e-06, -2.96331601906327e-07 },
{ -4.43127013254319e-06, 0.000165988374412387, -1.96475753260921e-06 },
{ -2.96331601906334e-07, -1.96475753260921e-06, 0.000164100497167486 } };

const double Robot_IMU_Control::hardiron_reference[3] = { -863.903533142847, 179.123382820601, 2556.53911843545 };
#pragma endregion

Robot_IMU_Control::Robot_IMU_Control(void)
{}

Robot_IMU_Control::~Robot_IMU_Control(void)
{}

void Robot_IMU_Control::initialize(void)
{
	FREQ = 0;
	FREQ_REFERENCE = 0;
	FREQ_EMG = 0;
	sensor_id = 0;//	

	//DIRECTION - COMMANDES                                                                                  
	CmdX_new = 0; float CmdY_new = 0;
	CmdXout = 0;	float CmdYout = 0;
	RightLeftComm = 0.0;	                                                                        
	ForBackComm = 0.0;	                                                                        
	UpDownComm = 0.0;
	RightLeftComm_ctrl = 0.0;	                                                                        
	ForBackComm_ctrl = 0.0;	
	UpDownComm_ctrl = 0.0;

	//RAW DATA HEADSET
	HEADSET_ON = 0;

	//RAW DATA REFERENCE
	char REFERENCE_ON = 0;

	int index_gyr_ref = 0;
	float MeanVelRef = 0.0;

	//DATA EMG
	char EMG_ON = 0;
	char index_EMG = 0;
	static const int NB_SAMPLES_IN_PCKT = 9;

	//DATA TKE + ENV
	int DATA_TKE[NB_SAMPLES_IN_PCKT];
	static const int NB_SAMPLES_IN_WINDOW = 81;
	static const int NB_SAMPLES_OVERLAP = 45;
	int INDEX_WINDOW = 0;
	float MEAN_TKE_CURRENT = 0;
	double emg[3];
	char NEW_TKE = 0;

	//DATA CALIB + DETECT
	/*float MEAN_TKE = 0; float STD_TKE = 0; int K_TH = 1;
	static const int NB_SAMPLES_CALIB = 100 * NB_SAMPLES_IN_PCKT;
	int DATA_TKE_CALIB[NB_SAMPLES_CALIB];
	//int CALIB_EMG_INDEX = -1;
	char EMG_DETECTED = 0;
	static const char nb_detect = 10;  char EMG_BUTTON[nb_detect]; char i_button = 0;
	char NEW_CALIB = 0;*/

	//Data 2D --> 3D
	float x = 0;									//roll + offset
	float y = 0;									//pitch + offset
	float x0 = 0;									//roll brute
	float y0 = 0;									//pitch brute	
	float xoff = 0;									//offset - position neutre roll
	float yoff = 0;									//offset - position neutre pitch
	float z0 = 0;
	float z_ctrl = 0;
	float zoff = 0;
	float z0_ref = 0;
	int InhibiteZctrl = 0;

	float Amp = 0.0;
	float Theta = 0.0;
	float Orientation = 0.0;

	//BUTTONS                                                                                                   
	int PRESSED = 0;
	int pressedtime = 0;
	int B_INDEX = 0;
	bool HOME_PRESSED = 0;
	bool BACK = 0;

	set_default_calibration();
	//initialize_filters();
}

void Robot_IMU_Control::set_default_calibration(void)
{
	int ZActive = 1;
	int ReferenceActive = 0;						//indique l'acitivation du noeud de référence
	int Valid = 1;									//indique la validité des marges et zones définies
	int DiagoActive = 1;							//0 = diagonale désactivée, 1 = diagonale activée
	float Theta_offset = 0;							//0 .. 360 degrés, mapping
	float ThRotReference = 30;						//vitesse de rotation seuil maximale du fauteuil

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
}


void Robot_IMU_Control::initialize_filters(void)
{
	
}

void Robot_IMU_Control::read_payload(int data[])
{
	sensor_id = data[29];
	if (sensor_id == HEADSET)
	{
		HEADSET_ON = 1;
		if (data[1] != 0)
		{
			FREQ = 1000 / data[1];											// MSP430 Sensor Acquisition Frequency
		}

		acc[0] = C2_2((data[14] << 8) + data[15]);
		acc[1] = C2_2((data[16] << 8) + data[17]);
		acc[2] = C2_2((data[18] << 8) + data[19]);

		gyr[0] = C2_2((data[8] << 8) + data[9]);
		gyr[1] = C2_2((data[10] << 8) + data[11]);
		gyr[2] = C2_2((data[12] << 8) + data[13]);

		mag[0] = C2_2((data[2] << 8) + data[3]);
		mag[1] = C2_2((data[4] << 8) + data[5]);
		mag[2] = C2_2((data[6] << 8) + data[7]);
	}
	else if (sensor_id == REFERENCE)
	{
		REFERENCE_ON = 1;
		if (data[1] != 0)
		{
			FREQ_REFERENCE = 1000 / data[1];											// MSP430 Sensor Acquisition Frequency
		}
		acc_ref[0] = C2_2((data[14] << 8) + data[15]);
		acc_ref[1] = C2_2((data[16] << 8) + data[17]);
		acc_ref[2] = C2_2((data[18] << 8) + data[19]);

		gyr_ref[0] = C2_2((data[8] << 8) + data[9]);
		gyr_ref[1] = C2_2((data[10] << 8) + data[11]);
		gyr_ref[2] = C2_2((data[12] << 8) + data[13]);

		mag_ref[0] = C2_2((data[2] << 8) + data[3]);
		mag_ref[1] = C2_2((data[4] << 8) + data[5]);
		mag_ref[2] = C2_2((data[6] << 8) + data[7]);
	}
	else if (sensor_id == EMG)
	{
		//read EMG payload
		EMG_ON = 1;
		if (data[1] != 0)
		{
			FREQ_EMG = 1000 / data[1];											// MSP430 Sensor Acquisition Frequency
		}
		for (int i = 1; i <= NB_SAMPLES_IN_PCKT; i++)
		{
			int j = (3 * i) - 1;
			DATA_EMG[i - 1] = C2_3((data[j] << 16) + (data[j + 1] << 8) + (data[j + 2])) / 1000;
			index_EMG = index_EMG + 1;
		}
	}
}

void Robot_IMU_Control::filter_emg()
{
	for (int i = 0; i < NB_SAMPLES_IN_PCKT; i++)
	{
		x_emg[0] = DATA_EMG[i];
		//y_emg[0] = x_emg[0];
		// bs filter
		y_emg[0] = num_bs_55_65Hz[0] * x_emg[0] + num_bs_55_65Hz[1] * x_emg[1] + num_bs_55_65Hz[2] * x_emg[2] + num_bs_55_65Hz[3] * x_emg[3] + num_bs_55_65Hz[4] * x_emg[4] - (den_bs_55_65Hz[1] * y_emg[1] + den_bs_55_65Hz[2] * y_emg[2] + den_bs_55_65Hz[3] * y_emg[3] + den_bs_55_65Hz[4] * y_emg[4]);
		x_emg[4] = x_emg[3]; x_emg[3] = x_emg[2]; x_emg[2] = x_emg[1]; x_emg[1] = x_emg[0];
		//y_emg[4] = y_emg[3]; y_emg[3] = y_emg[2]; y_emg[2] = y_emg[1]; y_emg[1] = y_emg[0];
		// bp filter
		z_emg[0] = num_bp_20_450Hz[0] * y_emg[0] + num_bp_20_450Hz[1] * y_emg[1] + num_bp_20_450Hz[2] * y_emg[2] + num_bp_20_450Hz[3] * y_emg[3] + num_bp_20_450Hz[4] * y_emg[4] - (den_bp_20_450Hz[1] * z_emg[1] + den_bp_20_450Hz[2] * z_emg[2] + den_bp_20_450Hz[3] * z_emg[3] + den_bp_20_450Hz[4] * z_emg[4]);
		y_emg[4] = y_emg[3]; y_emg[3] = y_emg[2]; y_emg[2] = y_emg[1]; y_emg[1] = y_emg[0];
		z_emg[4] = z_emg[3]; z_emg[3] = z_emg[2]; z_emg[2] = z_emg[1]; z_emg[1] = z_emg[0];

		DATA_EMG_FILT[i] = z_emg[0];
	}
}

void Robot_IMU_Control::process_tke()
{
	int var;
	for (int itke = 0; itke < NB_SAMPLES_IN_PCKT; itke++)
	{
		emg[2] = DATA_EMG_FILT[itke];
		DATA_TKE[itke] = absolute(emg[1] * emg[1] - emg[0] * emg[2]);
		emg[0] = emg[1];
		emg[1] = emg[2];

		DATA_ENV[INDEX_WINDOW] = DATA_TKE[itke];
		//var = DATA_TKE[itke];
		INDEX_WINDOW += 1;
		if (INDEX_WINDOW >= NB_SAMPLES_IN_WINDOW)
		{
			MEAN_TKE_CURRENT = mean_tabi(DATA_ENV, NB_SAMPLES_IN_WINDOW);
			int var_nb = NB_SAMPLES_IN_WINDOW - NB_SAMPLES_OVERLAP;
			for (int j = 0; j < NB_SAMPLES_OVERLAP; j++)
				DATA_ENV[j] = DATA_ENV[var_nb + j];
			INDEX_WINDOW = NB_SAMPLES_OVERLAP;
			NEW_TKE = 1;
		}

		if (CALIB_EMG_INDEX != -1 && CALIB_EMG_INDEX != -2)
		{
			DATA_TKE_CALIB[CALIB_EMG_INDEX] = DATA_TKE[itke];
			CALIB_EMG_INDEX++;
			if (CALIB_EMG_INDEX >= NB_SAMPLES_CALIB)
			{
				CALIB_EMG_INDEX = -2;
				MEAN_TKE = mean_tabi(DATA_TKE_CALIB, NB_SAMPLES_CALIB);
				double SUM_STD = 0;
				for (int h = 0; h < NB_SAMPLES_CALIB; h++)
					SUM_STD += (DATA_TKE_CALIB[h] - MEAN_TKE)*(DATA_TKE_CALIB[h] - MEAN_TKE);
				STD_TKE = sqrt(SUM_STD / NB_SAMPLES_CALIB);
				NEW_CALIB = 1;
			}
		}
	}
}

void Robot_IMU_Control::detect_tke()
{
	int i = 0;
	if (MEAN_TKE_CURRENT >= MEAN_TKE + K_TH*STD_TKE)
	{
		EMG_DETECTED = 1;
		//Beep(523, 500); // 523 hertz (C5) for 500 milliseconds
	}
	else
		EMG_DETECTED = 0;
}

char Robot_IMU_Control::min_tabc(char* tab, char size)
{
	char pt = tab[0];
	for (int g = 0; g < size; g++)
	{
		if (tab[g] <= pt)
			pt = tab[g];
	}
	return pt;
}

void Robot_IMU_Control::calibrate_imu()
{
	float magx_off, magy_off, magz_off;
	if (sensor_id == HEADSET)
	{
		//gyr
		gyr[3] = gyr[0];
		gyr[4] = gyr[1];
		gyr[5] = gyr[2];

		//magnet
		magx_off = mag[0] - hardiron_headset[0];
		magy_off = mag[1] - hardiron_headset[1];
		magz_off = -mag[2] - hardiron_headset[2];

		mag[3] = comp_headset[0][0] * magx_off + comp_headset[0][1] * magy_off + comp_headset[0][2] * magz_off;
		mag[4] = comp_headset[1][0] * magx_off + comp_headset[1][1] * magy_off + comp_headset[1][2] * magz_off;
		mag[5] = comp_headset[2][0] * magx_off + comp_headset[2][1] * magy_off + comp_headset[2][2] * magz_off;
	}
	else if (sensor_id == REFERENCE)
	{
		//gyr
		gyr_ref[3] = gyr_ref[0];
		gyr_ref[4] = gyr_ref[1];
		gyr_ref[5] = gyr_ref[2];
		moy_gyr_ref[index_gyr_ref] = abs(gyr_ref[0]);
		index_gyr_ref = index_gyr_ref + 1;
		if (index_gyr_ref >= nb_val_gyr_ref)
			index_gyr_ref = 0;

		//magnet
		magx_off = mag_ref[0] - hardiron_reference[0];
		magy_off = mag_ref[1] - hardiron_reference[1];
		magz_off = -mag_ref[2] - hardiron_reference[2];

		mag_ref[3] = comp_reference[0][0] * magx_off + comp_reference[0][1] * magy_off + comp_reference[0][2] * magz_off;
		mag_ref[4] = comp_reference[1][0] * magx_off + comp_reference[1][1] * magy_off + comp_reference[1][2] * magz_off;
		mag_ref[5] = comp_reference[2][0] * magx_off + comp_reference[2][1] * magy_off + comp_reference[2][2] * magz_off;
	}
}

void Robot_IMU_Control::sensor_fusion()
{
	if (sensor_id == HEADSET)
	{
		//pitch
		pitch[0] = ALPHA_ACC*rad2deg(atan2(-acc[1], -acc[0])) + ALPHA_GYRO*(pitch[0] + (-gyr[2]) * GAIN_GYRO * (1 / FREQ_GYRO));
		x_pitch[0] = pitch[0];
		pitch[1] = num_lp_10Hz[0] * x_pitch[0] + num_lp_10Hz[1] * x_pitch[1] - den_lp_10Hz[1] * pitch[2];
		x_pitch[1] = x_pitch[0];
		pitch[2] = pitch[1];

		//roll
		roll[0] = ALPHA_ACC*rad2deg(atan2(-acc[2], -acc[0])) + ALPHA_GYRO*(roll[0] + gyr[1] * GAIN_GYRO * (1 / FREQ_GYRO));
		x_roll[0] = roll[0];
		roll[1] = num_lp_10Hz[0] * x_roll[0] + num_lp_10Hz[1] * x_roll[1] - den_lp_10Hz[1] * roll[2];
		x_roll[1] = x_roll[0];
		roll[2] = roll[1];

		//yaw
		float cP, sP, cR, sR, cPo, sPo, cRo, sRo, cYo, sYo;
		cP = cos(deg2rad(-pitch[2]));
		sP = sin(deg2rad(-pitch[2]));
		cR = cos(deg2rad(roll[2]));
		sR = sin(deg2rad(roll[2]));

		cPo = 1;// cos(deg2rad(yoff));
		sPo = 0;// sin(deg2rad(yoff));
		cRo = 1;// cos(deg2rad(-xoff));
		sRo = 0;// sin(deg2rad(-xoff));
		cYo = cos(deg2rad(-zoff));
		sYo = sin(deg2rad(-zoff));

		xh_cal = cP*cR*mag[3] - sP*mag[4] + cP*sR*mag[5];
		yh_cal = sP*cR*mag[3] + cP*mag[4] + sP*sR*mag[5];
		zh_cal = -sR*mag[3] + 0 * mag[4] + cR*mag[5];

		xh_ctrl = cPo*cRo			  *	xh_cal + sPo*cRo			  *	yh_cal - sRo	* zh_cal;
		yh_ctrl = -(sYo*cPo*sRo + cYo*sPo)*	xh_cal + (cYo*cPo - sYo*sPo*sRo)*	yh_cal - sYo*cRo* zh_cal;
		zh_ctrl = (cYo*cPo*sRo - sYo*sPo) *	xh_cal + (cYo*sPo*sRo + sYo*cPo)*	yh_cal + cYo*cRo* zh_cal;

		yaw[0] = ALPHA_MAG*rad2deg(atan2(-zh_cal, -yh_cal)) + ALPHA_GYRO*(yaw[0] + gyr[0] * GAIN_GYRO * (1 / FREQ_GYRO));
		x_yaw[0] = yaw[0];
		yaw[1] = num_lp_10Hz[0] * x_yaw[0] + num_lp_10Hz[1] * x_yaw[1] - den_lp_10Hz[1] * yaw[2];
		x_yaw[1] = x_yaw[0];
		yaw[2] = yaw[1];

		yaw_ctrl[0] = ALPHA_MAG*rad2deg(atan2(-zh_ctrl, -yh_ctrl)) + ALPHA_GYRO*(yaw_ctrl[0] + gyr[0] * GAIN_GYRO * (1 / FREQ_GYRO));
		x_yaw_ctrl[0] = yaw_ctrl[0];
		yaw_ctrl[1] = num_lp_10Hz[0] * x_yaw_ctrl[0] + num_lp_10Hz[1] * x_yaw_ctrl[1] - den_lp_10Hz[1] * yaw_ctrl[2];
		x_yaw_ctrl[1] = x_yaw_ctrl[0];
		yaw_ctrl[2] = yaw_ctrl[1];
	}
	else if (sensor_id == REFERENCE)
	{
		//pitch
		pitch_ref[0] = ALPHA_ACC*rad2deg(atan2(-acc_ref[1], -acc_ref[0])) + ALPHA_GYRO*(pitch_ref[0] + (-gyr_ref[2]) * GAIN_GYRO * (1 / FREQ_GYRO));
		x_pitch_ref[0] = pitch_ref[0];
		pitch_ref[1] = num_lp_10Hz[0] * x_pitch_ref[0] + num_lp_10Hz[1] * x_pitch_ref[1] - den_lp_10Hz[1] * pitch_ref[2];
		x_pitch_ref[1] = x_pitch_ref[0];
		pitch_ref[2] = pitch_ref[1];

		//roll
		roll_ref[0] = ALPHA_ACC*rad2deg(atan2(-acc_ref[2], -acc_ref[0])) + ALPHA_GYRO*(roll_ref[0] + gyr_ref[1] * GAIN_GYRO * (1 / FREQ_GYRO));
		x_roll_ref[0] = roll_ref[0];
		roll_ref[1] = num_lp_10Hz[0] * x_roll_ref[0] + num_lp_10Hz[1] * x_roll_ref[1] - den_lp_10Hz[1] * roll_ref[2];
		x_roll_ref[1] = x_roll_ref[0];
		roll_ref[2] = roll_ref[1];

		//yaw
		float cP, sP, cR, sR;
		cP = cos(deg2rad(-pitch_ref[2]));
		sP = sin(deg2rad(-pitch_ref[2]));
		cR = cos(deg2rad(roll_ref[2]));
		sR = sin(deg2rad(roll_ref[2]));

		xh_cal_ref = cP*cR*mag_ref[3] - sP*mag_ref[4] + cP*sR*mag_ref[5];
		yh_cal_ref = sP*cR*mag_ref[3] + cP*mag_ref[4] + sP*sR*mag_ref[5];
		zh_cal_ref = -sR*mag_ref[3] + 0 * mag_ref[4] + cR*mag_ref[5];

		yaw_ref[0] = ALPHA_MAG*rad2deg(atan2(-zh_cal_ref, -yh_cal_ref)) + ALPHA_GYRO*(yaw_ref[0] + gyr_ref[0] * GAIN_GYRO * (1 / FREQ_GYRO));
		x_yaw_ref[0] = yaw_ref[0];
		yaw_ref[1] = num_lp_10Hz[0] * x_yaw_ref[0] + num_lp_10Hz[1] * x_yaw_ref[1] - den_lp_10Hz[1] * yaw_ref[2];
		x_yaw_ref[1] = x_yaw_ref[0];
		yaw_ref[2] = yaw_ref[1];
	}
}

int Robot_IMU_Control::detect_wheelchair_rotation()
{
	//test rotation chaise
	//ThRotReference = mean_tab(moy_gyr_ref, nb_val_gyr_ref);
	MeanVelRef = mean_tab(moy_gyr_ref, nb_val_gyr_ref);
	InhibiteZctrl = 0;
	if (MeanVelRef >= ThRotReference)
		InhibiteZctrl = 1;
	return InhibiteZctrl;
}

void Robot_IMU_Control::algo()
{
	Amin_D = Amin;
	Amax_D = Amax;
	BACK = 0;

	x0 = roll[2];
	y0 = pitch[2];
	z0 = yaw[2];
	z0_ref = yaw_ref[2];

	y = y0 - yoff;
	x = x0 - xoff;
	z_ctrl = yaw_ctrl[2];

	if (ZoneRight > 180)
	{
		ZoneRight = ZoneRight - 180;
	}
	float Ri1 = ZoneRight - DeltaRight / 2;
	float Ri2 = Ri1 + 360;

	if ((ZoneRight + DeltaRight / 2) > (ZoneForw - DeltaForw / 2) || (ZoneForw + DeltaForw / 2) > (ZoneLeft - DeltaLeft / 2) || (ZoneLeft + DeltaLeft / 2) > (ZoneBack - DeltaBack / 2) || (ZoneBack + DeltaBack / 2) > (Ri2))
	{
		Valid = 0;
		ZoneForw = 90;
		ZoneRight = 10;
		ZoneLeft = 170;
		ZoneBack = 270;
		DeltaForw = 20;
		DeltaRight = 60;
		DeltaLeft = 60;
		DeltaBack = 20;
	}

	Amp = sqrt(x*x + y*y);
	Theta = atan2(y, x) * 180 / M_PI;

	// 0 .. 360 degrés ////////////////
	if (Theta < 0)
	{
		Theta = Theta + 360;
	}

	float Theta2 = Theta;
	if (Theta2 >= 180)
	{
		Theta2 = Theta2 - 360;
	}

	float Theta3 = Theta;
	if (Theta3 < 180)
	{
		Theta3 = Theta3 + 360;
	}
	///////////////////////////////////

	float AmpF = 0; float CmdY = 0; float CmdX = 0;

	AmpF = Amp;

	float PortY = 0; float PortX = 0;

	if (Theta > ZoneForw - DeltaForw / 2 && Theta < ZoneForw + DeltaForw / 2)
	{
		//Forward
		CmdY = AmpF;
	}
	else if (Theta > ZoneBack - DeltaBack / 2 && Theta < ZoneBack + DeltaBack / 2)
	{
		//Backward
		BACK = 1;
		Amin_D = Amin_BACK;
		Amax_D = Amax_BACK;
		CmdY = -AmpF;
	}
	else if (Theta > ZoneLeft - DeltaLeft / 2 && Theta < ZoneLeft + DeltaLeft / 2)
	{
		//Left
		CmdX = -AmpF;
	}
	else if (Theta < ZoneForw - DeltaForw / 2 && Theta > ZoneRight + DeltaRight / 2)
	{
		//For-Right
		if (DiagoActive == 1)
		{
			PortY = 1 - abs(Theta - (ZoneForw - DeltaForw / 2)) / abs((ZoneForw - DeltaForw / 2) - (ZoneRight + DeltaRight / 2));
			PortX = 1 - abs(Theta - (ZoneRight + DeltaRight / 2)) / abs((ZoneForw - DeltaForw / 2) - (ZoneRight + DeltaRight / 2));
			CmdY = AmpF * PortY;
			CmdX = AmpF * PortX;
		}
		else
		{
			CmdY = 0;
			CmdX = 0;
		}
	}
	else if (Theta < ZoneLeft - DeltaLeft / 2 && Theta > ZoneForw + DeltaForw / 2)
	{
		//For-Left
		if (DiagoActive == 1)
		{
			PortX = 1 - abs(Theta - (ZoneLeft - DeltaLeft / 2)) / abs((ZoneLeft - DeltaLeft / 2) - (ZoneForw + DeltaForw / 2));
			PortY = 1 - abs(Theta - (ZoneForw + DeltaForw / 2)) / abs((ZoneLeft - DeltaLeft / 2) - (ZoneForw + DeltaForw / 2));
			CmdY = AmpF * PortY;
			CmdX = -AmpF * PortX;
		}
		else
		{
			CmdY = 0;
			CmdX = 0;
		}
	}
	else if (Theta < ZoneBack - DeltaBack / 2 && Theta > ZoneLeft + DeltaLeft / 2)
	{
		//Back-Left
		BACK = 1;
		Amin_D = Amin_BACK;
		Amax_D = Amax_BACK;
		if (DiagoActive == 1)
		{
			PortY = 1 - abs(Theta - (ZoneBack - DeltaBack / 2)) / abs((ZoneBack - DeltaBack / 2) - (ZoneLeft + DeltaLeft / 2));
			PortX = 1 - abs(Theta - (ZoneLeft + DeltaLeft / 2)) / abs((ZoneBack - DeltaBack / 2) - (ZoneLeft + DeltaLeft / 2));
			CmdY = -AmpF * PortY;
			CmdX = -AmpF * PortX;
		}
		else
		{
			CmdY = 0;
			CmdX = 0;
		}
	}
	else if (Theta2 > Ri1 && Theta2 < (ZoneRight + DeltaRight / 2))
	{
		//Right
		CmdX = AmpF;
	}
	else if (Theta3 < Ri2 && Theta3 > ZoneBack + DeltaBack / 2)
	{
		//Back-Right
		BACK = 1;
		Amin_D = Amin_BACK;
		Amax_D = Amax_BACK;
		if (DiagoActive == 1)
		{
			PortX = 1 - abs(Theta3 - (Ri2)) / abs((Ri2)-(ZoneBack + DeltaBack / 2));
			PortY = 1 - abs(Theta3 - (ZoneBack + DeltaBack / 2)) / abs((Ri2)-(ZoneBack + DeltaBack / 2));
			CmdY = -AmpF * PortY;
			CmdX = AmpF * PortX;
		}
		else
		{
			CmdY = 0;
			CmdX = 0;
		}
	}

	if (fabsf(CmdX) < Amin)
	{
		CmdXout = 0;
	}
	else if (CmdX > Amax - 0.15)
	{
		CmdXout = 1;
	}
	else if (CmdX < -Amax + 0.15)
	{
		CmdXout = -1;
	}
	else if (CmdX >= 0)
	{
		CmdXout = ((CmdX - Amin) / (Amax - Amin)) + 0.15;
	}
	else if (CmdX < 0)
	{
		CmdXout = ((CmdX + Amin) / (Amax - Amin)) - 0.15;
	}

	if (BACK == 1 && abs(CmdY) < Amin_D)
	{
		CmdYout = 0;
	}
	else if (BACK == 0 && abs(CmdY) < Amin)
	{
		CmdYout = 0;
	}
	else if (CmdY > Amax - 0.15)
	{
		CmdYout = 1;
	}
	else if (CmdY < -Amax_D + 0.15)
	{
		CmdYout = -1;
	}
	else if (CmdY >= 0)
	{
		CmdYout = ((CmdY - Amin) / (Amax - Amin)) + 0.15;
	}
	else if (CmdY < 0)
	{
		CmdYout = ((CmdY + Amin_D) / (Amax - Amin_D)) - 0.15;
	}
	CmdYout = -CmdYout;

	if (REFERENCE_ON && ReferenceActive)
		Theta_offset = z0 - z0_ref;

	CmdX_new = CmdXout * cos(deg2rad(Theta_offset)) + CmdYout * sin(deg2rad(Theta_offset));
	CmdY_new = CmdXout * -sin(deg2rad(Theta_offset)) + CmdYout * cos(deg2rad(Theta_offset));
	/////////////////////////////////////
	/*if (abs(CmdX_new) > 0.3)
		CmdX_new = CmdX_new / abs(CmdX_new);// 0;// 
	if (abs(CmdY_new) > 0.3)
		CmdY_new = CmdY_new / abs(CmdY_new);// 0;//*/ 
	/////////////////////////////////////
	RightLeftComm = CmdX_new;//CmdXout;//
	ForBackComm = CmdY_new;//CmdYout;//


	// Commande Z
	UpDownComm = 0;
	if ((ZActive == 1 && InhibiteZctrl == 0) && (CmdX_new == 0 && CmdY_new == 0))
		//if ((ZActive == 1) && (CmdX_new == 0 && CmdY_new == 0))
	{
		if (z_ctrl >= NeutralUpDown - DeltaNeutralUpDown / 2 && z_ctrl <= NeutralUpDown + DeltaNeutralUpDown / 2)
			UpDownComm = 0;
		else if (z_ctrl > NeutralUpDown + DeltaNeutralUpDown / 2 && z_ctrl < MaxUp)
			UpDownComm = (z_ctrl - NeutralUpDown) / (MaxUp - NeutralUpDown);
		else if (z_ctrl >= MaxUp)
			UpDownComm = (MaxUp - NeutralUpDown) / (MaxUp - NeutralUpDown);
		else if (z_ctrl < NeutralUpDown - DeltaNeutralUpDown / 2 && z_ctrl > MaxDown)
			UpDownComm = -1 * (NeutralUpDown - z_ctrl) / (NeutralUpDown - MaxDown);
		else if (z_ctrl <= MaxDown)
			UpDownComm = -1 * (NeutralUpDown - MaxDown) / (NeutralUpDown - MaxDown);
	}
}

JoystickCommand Robot_IMU_Control::PushButton(char buttonindex)
{
	JoystickCommand button_Command;
	button_Command.InitStruct();
	button_Command.ButtonValue[B_INDEX] = 1;
	return button_Command;
}

JoystickCommand Robot_IMU_Control::ReleaseButton(char buttonindex)
{
	JoystickCommand button_Command;
	button_Command.InitStruct();
	button_Command.ButtonValue[B_INDEX] = 0;
	return button_Command;
}

JoystickCommand Robot_IMU_Control::Control(int b)
{
	JoystickCommand control_Command;
	control_Command.InitStruct();
	if (b == 1)
	{
		RightLeftComm_ctrl = -RightLeftComm;
		ForBackComm_ctrl = ForBackComm;
		UpDownComm_ctrl = UpDownComm;
	}
	else
	{
		RightLeftComm_ctrl = 0.0;
		ForBackComm_ctrl = 0.0;
		UpDownComm_ctrl = 0.0;
	}
	control_Command.InclineLeftRight = RightLeftComm_ctrl;
	control_Command.InclineForwardBackward = ForBackComm_ctrl;
	control_Command.Rotate = UpDownComm_ctrl;
	return control_Command;
}

float Robot_IMU_Control::C2_2(int var)
{
	float var1, var2;
	int mask = 32767; //mask 0x7FFF
	var1 = var & mask;
	var2 = (var >> 15)*(-32768);
	return (var1 + var2);
}

float Robot_IMU_Control::C2_3(int var)
{
	float var1, var2;
	int mask = 8388607; //mask 0x7FFF
	var1 = var & mask;
	var2 = (var >> 23)*(-8388608);
	return (var1 + var2);
}

void Robot_IMU_Control::wrap_angle(float * p_angle)
{
	if (*p_angle > M_PI)
		*p_angle -= (2 * M_PI);
	if (*p_angle < M_PI)
		*p_angle += (2 * M_PI);
	if (*p_angle < 0)
		*p_angle += (2 * M_PI);
}

float Robot_IMU_Control::rad2deg(float angle_rad)
{
	return angle_rad *(180 / M_PI);
}

float Robot_IMU_Control::deg2rad(float angle_deg)
{
	return angle_deg *(M_PI / 180);
}

float Robot_IMU_Control::absolute(float var)
{
	if (var <= 0)
		return -var;
	else
		return var;
}

float Robot_IMU_Control::mean_tab(float * tab, int taille)
{
	int g; float sum = 0.0;
	for (g = 0; g < taille; g++)
		sum = sum + tab[g];
	return (sum / taille);
}

float Robot_IMU_Control::mean_tabi(int * tab, int taille)
{
	int g; float sum = 0.0;
	for (g = 0; g < taille; g++)
		sum = sum + tab[g];
	return (sum / taille);
}
}
