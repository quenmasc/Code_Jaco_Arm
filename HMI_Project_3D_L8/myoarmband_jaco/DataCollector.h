#pragma once

#define _USE_MATH_DEFINES

#include "stdafx.h"
#include <typeinfo>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <time.h>
#include <Windows.h>
#include <string>
#include <random>
#include <vector>
#include <myo/myo.hpp>


namespace MyoArmBand_JACO
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
	class DataCollector : public myo::DeviceListener
	{
		std::string path_to_save;
		int number_example;
		int number_of_emg_canals;
	public:
		DataCollector()
		{
			path_to_save = "";

			number_example = 0;
			number_of_emg_canals = 8;
		}

		void set_path_to_save(std::string new_path_to_save)
		{
			path_to_save = new_path_to_save;
		}

		void set_number_example(int new_number_example)
		{
			number_example = new_number_example;
		}

		void onOrientationData(myo::Myo *myo, uint64_t timestamp, const myo::Quaternion< float > &rotation)
		{
			using std::atan2;
			using std::asin;
			using std::sqrt;
			using std::max;
			using std::min;

			// Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
			orientation_data[0] = atan2(2.0f * (rotation.w() * rotation.x() + rotation.y() * rotation.z()),//ROLL
				1.0f - 2.0f * (rotation.x() * rotation.x() + rotation.y() * rotation.y()));
			orientation_data[1] = asin(max(-1.0f, min(1.0f, 2.0f * (rotation.w() * rotation.y() - rotation.z() * rotation.x()))));//PITCH
			orientation_data[2] = atan2(2.0f * (rotation.w() * rotation.z() + rotation.x() * rotation.y()),//YAW
				1.0f - 2.0f * (rotation.y() * rotation.y() + rotation.z() * rotation.z()));
		}

		// onEmgData() is called whenever a paired Myo has provided new EMG data, and EMG streaming is enabled.
		void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
		{
			//EMG DATA COLLECTION
		}

		int get_number_of_emg_vector_available_currently()
		{
			return number_of_emg_vector_available_currently;
		}

		void reset_number_of_emg_vector_available_currently()
		{
			number_of_emg_vector_available_currently = 0;
		}

		void onConnect(myo::Myo *myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
			//Reneable streaming
			myo->setStreamEmg(myo::Myo::streamEmgEnabled);
		}

		float *get_orientation_data()
		{
			return orientation_data;
		}

		int classe_actual = 0;
		int number_of_emg_vector_available_currently = 0;
		float orientation_data[3];
		//float orientation_data[3] = { 0.0, 0.0, 0.0 };
	};
}