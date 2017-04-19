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
#include "Controle.h"
#include "DataCollector.h"
#include "KinovaTypes.h"

#define KEY_Q 81
#define KEY_T 84
#define KEY_0 48
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57

namespace MyoArmBand_JACO {

	using namespace std;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Description résumée de Controle
	/// </summary>
	public ref class Controle : public System::Windows::Forms::Form
	{
	public:
		Controle(void)
		{
			InitializeComponent();
			//
			//TODO: ajoutez ici le code du constructeur
			//
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~Controle()
		{
			if (components)
			{
				delete components;
			}
		}
	private: int res;
	private: System::IO::Ports::SerialPort^  COM_Serial;
	private: System::Windows::Forms::Button^  Start_JACO;
	private: System::Windows::Forms::Button^  Stop_JACO;
	private: System::Windows::Forms::Label^  Message_JACO;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Label^  Message_MYO;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  Message_SYST;
	private: System::Windows::Forms::Button^  button13;
	private: System::Windows::Forms::Button^  button14;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::TextBox^  textBox10;
	private: System::Windows::Forms::TextBox^  textBox9;
	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox17;
	private: System::Windows::Forms::TextBox^  textBox18;
	private: System::Windows::Forms::TextBox^  textBox15;
	private: System::Windows::Forms::TextBox^  textBox16;
	private: System::Windows::Forms::TextBox^  textBox13;
	private: System::Windows::Forms::TextBox^  textBox14;
	private: System::Windows::Forms::TextBox^  textBox12;
	private: System::Windows::Forms::TextBox^  textBox11;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label4;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Variable nécessaire au concepteur.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Controle::typeid));
			this->COM_Serial = (gcnew System::IO::Ports::SerialPort(this->components));
			this->Start_JACO = (gcnew System::Windows::Forms::Button());
			this->Stop_JACO = (gcnew System::Windows::Forms::Button());
			this->Message_JACO = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->Message_MYO = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->Message_SYST = (gcnew System::Windows::Forms::Label());
			this->button13 = (gcnew System::Windows::Forms::Button());
			this->button14 = (gcnew System::Windows::Forms::Button());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->textBox11 = (gcnew System::Windows::Forms::TextBox());
			this->textBox12 = (gcnew System::Windows::Forms::TextBox());
			this->textBox13 = (gcnew System::Windows::Forms::TextBox());
			this->textBox14 = (gcnew System::Windows::Forms::TextBox());
			this->textBox15 = (gcnew System::Windows::Forms::TextBox());
			this->textBox16 = (gcnew System::Windows::Forms::TextBox());
			this->textBox17 = (gcnew System::Windows::Forms::TextBox());
			this->textBox18 = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->SuspendLayout();
			// 
			// Start_JACO
			// 
			this->Start_JACO->Location = System::Drawing::Point(7, 437);
			this->Start_JACO->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Start_JACO->Name = L"Start_JACO";
			this->Start_JACO->Size = System::Drawing::Size(112, 33);
			this->Start_JACO->TabIndex = 52;
			this->Start_JACO->Text = L"Start JACO";
			this->Start_JACO->UseVisualStyleBackColor = true;
			// 
			// Stop_JACO
			// 
			this->Stop_JACO->Enabled = false;
			this->Stop_JACO->Location = System::Drawing::Point(144, 437);
			this->Stop_JACO->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Stop_JACO->Name = L"Stop_JACO";
			this->Stop_JACO->Size = System::Drawing::Size(113, 33);
			this->Stop_JACO->TabIndex = 51;
			this->Stop_JACO->Text = L"Stop JACO";
			this->Stop_JACO->UseVisualStyleBackColor = true;
			// 
			// Message_JACO
			// 
			this->Message_JACO->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Message_JACO->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Message_JACO->Location = System::Drawing::Point(7, 404);
			this->Message_JACO->Name = L"Message_JACO";
			this->Message_JACO->Size = System::Drawing::Size(250, 25);
			this->Message_JACO->TabIndex = 50;
			this->Message_JACO->Text = L"JACO Arm not connected";
			this->Message_JACO->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox1->Enabled = false;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(7, 121);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(250, 267);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 49;
			this->pictureBox1->TabStop = false;
			// 
			// Message_MYO
			// 
			this->Message_MYO->AutoSize = true;
			this->Message_MYO->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Message_MYO->Location = System::Drawing::Point(61, 41);
			this->Message_MYO->Name = L"Message_MYO";
			this->Message_MYO->Size = System::Drawing::Size(118, 18);
			this->Message_MYO->TabIndex = 0;
			this->Message_MYO->Text = L"No Myo found!";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->Message_MYO);
			this->groupBox1->Location = System::Drawing::Point(7, 11);
			this->groupBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox1->Size = System::Drawing::Size(250, 93);
			this->groupBox1->TabIndex = 49;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Myo Arm:";
			// 
			// button8
			// 
			this->button8->Enabled = false;
			this->button8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button8->Location = System::Drawing::Point(423, 155);
			this->button8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(93, 86);
			this->button8->TabIndex = 60;
			this->button8->Text = L"STOP";
			this->button8->UseVisualStyleBackColor = true;
			// 
			// button7
			// 
			this->button7->Enabled = false;
			this->button7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button7->Location = System::Drawing::Point(315, 155);
			this->button7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(93, 86);
			this->button7->TabIndex = 59;
			this->button7->Text = L"LEFT";
			this->button7->UseVisualStyleBackColor = true;
			// 
			// button6
			// 
			this->button6->Enabled = false;
			this->button6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button6->Location = System::Drawing::Point(423, 279);
			this->button6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(93, 86);
			this->button6->TabIndex = 58;
			this->button6->Text = L"BACK- WARD";
			this->button6->UseVisualStyleBackColor = true;
			// 
			// button5
			// 
			this->button5->Enabled = false;
			this->button5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button5->Location = System::Drawing::Point(531, 155);
			this->button5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(93, 86);
			this->button5->TabIndex = 57;
			this->button5->Text = L"RIGHT";
			this->button5->UseVisualStyleBackColor = true;
			// 
			// button4
			// 
			this->button4->Enabled = false;
			this->button4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button4->Location = System::Drawing::Point(423, 32);
			this->button4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(93, 86);
			this->button4->TabIndex = 56;
			this->button4->Text = L"FOR- WARD";
			this->button4->UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(607, 260);
			this->button1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(93, 86);
			this->button1->TabIndex = 62;
			this->button1->Text = L"DOWN";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Enabled = false;
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(607, 32);
			this->button2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(93, 86);
			this->button2->TabIndex = 61;
			this->button2->Text = L"UP";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// textBox4
			// 
			this->textBox4->BackColor = System::Drawing::SystemColors::Control;
			this->textBox4->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox4->Location = System::Drawing::Point(446, 381);
			this->textBox4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(45, 15);
			this->textBox4->TabIndex = 63;
			this->textBox4->Text = L"00";
			this->textBox4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::Control;
			this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox1->Location = System::Drawing::Point(446, 6);
			this->textBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(45, 15);
			this->textBox1->TabIndex = 64;
			this->textBox1->Text = L"00";
			this->textBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox2
			// 
			this->textBox2->BackColor = System::Drawing::SystemColors::Control;
			this->textBox2->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox2->Location = System::Drawing::Point(264, 191);
			this->textBox2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(45, 15);
			this->textBox2->TabIndex = 65;
			this->textBox2->Text = L"00";
			this->textBox2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox3
			// 
			this->textBox3->BackColor = System::Drawing::SystemColors::Control;
			this->textBox3->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox3->Location = System::Drawing::Point(630, 191);
			this->textBox3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(45, 15);
			this->textBox3->TabIndex = 66;
			this->textBox3->Text = L"00";
			this->textBox3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->Message_SYST);
			this->groupBox2->Location = System::Drawing::Point(263, 400);
			this->groupBox2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox2->Size = System::Drawing::Size(648, 70);
			this->groupBox2->TabIndex = 48;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Message:";
			// 
			// Message_SYST
			// 
			this->Message_SYST->AutoSize = true;
			this->Message_SYST->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Message_SYST->Location = System::Drawing::Point(11, 30);
			this->Message_SYST->Name = L"Message_SYST";
			this->Message_SYST->Size = System::Drawing::Size(76, 18);
			this->Message_SYST->TabIndex = 0;
			this->Message_SYST->Text = L"Message";
			// 
			// button13
			// 
			this->button13->Enabled = false;
			this->button13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button13->Location = System::Drawing::Point(607, 32);
			this->button13->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button13->Name = L"button13";
			this->button13->Size = System::Drawing::Size(93, 86);
			this->button13->TabIndex = 61;
			this->button13->Text = L"UP";
			this->button13->UseVisualStyleBackColor = true;
			// 
			// button14
			// 
			this->button14->Enabled = false;
			this->button14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button14->Location = System::Drawing::Point(607, 260);
			this->button14->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button14->Name = L"button14";
			this->button14->Size = System::Drawing::Size(93, 86);
			this->button14->TabIndex = 62;
			this->button14->Text = L"DOWN";
			this->button14->UseVisualStyleBackColor = true;
			// 
			// textBox5
			// 
			this->textBox5->BackColor = System::Drawing::SystemColors::Control;
			this->textBox5->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox5->Location = System::Drawing::Point(630, 381);
			this->textBox5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(45, 15);
			this->textBox5->TabIndex = 67;
			this->textBox5->Text = L"00";
			this->textBox5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox6
			// 
			this->textBox6->BackColor = System::Drawing::SystemColors::Control;
			this->textBox6->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox6->Location = System::Drawing::Point(630, 6);
			this->textBox6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(45, 15);
			this->textBox6->TabIndex = 68;
			this->textBox6->Text = L"00";
			this->textBox6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->textBox10);
			this->groupBox3->Controls->Add(this->textBox9);
			this->groupBox3->Controls->Add(this->textBox8);
			this->groupBox3->Controls->Add(this->textBox7);
			this->groupBox3->Controls->Add(this->label7);
			this->groupBox3->Controls->Add(this->label6);
			this->groupBox3->Controls->Add(this->label5);
			this->groupBox3->Controls->Add(this->label3);
			this->groupBox3->Location = System::Drawing::Point(730, 11);
			this->groupBox3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox3->Size = System::Drawing::Size(181, 171);
			this->groupBox3->TabIndex = 50;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Angles:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(6, 30);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(70, 15);
			this->label3->TabIndex = 0;
			this->label3->Text = L"Pitch_Myo :";
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->textBox17);
			this->groupBox5->Controls->Add(this->textBox18);
			this->groupBox5->Controls->Add(this->textBox15);
			this->groupBox5->Controls->Add(this->textBox16);
			this->groupBox5->Controls->Add(this->textBox13);
			this->groupBox5->Controls->Add(this->textBox14);
			this->groupBox5->Controls->Add(this->textBox12);
			this->groupBox5->Controls->Add(this->textBox11);
			this->groupBox5->Controls->Add(this->label13);
			this->groupBox5->Controls->Add(this->label15);
			this->groupBox5->Controls->Add(this->label11);
			this->groupBox5->Controls->Add(this->label12);
			this->groupBox5->Controls->Add(this->label10);
			this->groupBox5->Controls->Add(this->label9);
			this->groupBox5->Controls->Add(this->label8);
			this->groupBox5->Controls->Add(this->label4);
			this->groupBox5->Location = System::Drawing::Point(730, 186);
			this->groupBox5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox5->Size = System::Drawing::Size(181, 219);
			this->groupBox5->TabIndex = 51;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Margins:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(6, 65);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(71, 15);
			this->label5->TabIndex = 1;
			this->label5->Text = L"Raw_Myo  :";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(8, 104);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(68, 15);
			this->label6->TabIndex = 2;
			this->label6->Text = L"Roll_Myo  :";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(8, 144);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(70, 15);
			this->label7->TabIndex = 3;
			this->label7->Text = L"Pitch_IMU :";
			// 
			// textBox7
			// 
			this->textBox7->BackColor = System::Drawing::SystemColors::Control;
			this->textBox7->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox7->Location = System::Drawing::Point(112, 32);
			this->textBox7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(45, 15);
			this->textBox7->TabIndex = 69;
			this->textBox7->Text = L"00";
			this->textBox7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox8
			// 
			this->textBox8->BackColor = System::Drawing::SystemColors::Control;
			this->textBox8->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox8->Location = System::Drawing::Point(112, 67);
			this->textBox8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(45, 15);
			this->textBox8->TabIndex = 70;
			this->textBox8->Text = L"00";
			this->textBox8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox9
			// 
			this->textBox9->BackColor = System::Drawing::SystemColors::Control;
			this->textBox9->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox9->Location = System::Drawing::Point(112, 104);
			this->textBox9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(45, 15);
			this->textBox9->TabIndex = 71;
			this->textBox9->Text = L"00";
			this->textBox9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox10
			// 
			this->textBox10->BackColor = System::Drawing::SystemColors::Control;
			this->textBox10->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox10->Location = System::Drawing::Point(112, 144);
			this->textBox10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox10->Name = L"textBox10";
			this->textBox10->Size = System::Drawing::Size(45, 15);
			this->textBox10->TabIndex = 72;
			this->textBox10->Text = L"00";
			this->textBox10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(8, 30);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(62, 15);
			this->label4->TabIndex = 73;
			this->label4->Text = L"PM_plus :";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(104, 30);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(73, 15);
			this->label8->TabIndex = 74;
			this->label8->Text = L"PM_minus :";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(6, 74);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(63, 15);
			this->label9->TabIndex = 75;
			this->label9->Text = L"RM_plus :";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label10->Location = System::Drawing::Point(102, 74);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(74, 15);
			this->label10->TabIndex = 76;
			this->label10->Text = L"RM_minus :";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(105, 119);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(72, 15);
			this->label11->TabIndex = 78;
			this->label11->Text = L"YM_minus :";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label12->Location = System::Drawing::Point(6, 119);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(61, 15);
			this->label12->TabIndex = 77;
			this->label12->Text = L"YM_plus :";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label13->Location = System::Drawing::Point(107, 166);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(65, 15);
			this->label13->TabIndex = 80;
			this->label13->Text = L"PI_minus :";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label15->Location = System::Drawing::Point(8, 166);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(54, 15);
			this->label15->TabIndex = 79;
			this->label15->Text = L"PI_plus :";
			// 
			// textBox11
			// 
			this->textBox11->BackColor = System::Drawing::SystemColors::Control;
			this->textBox11->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox11->Location = System::Drawing::Point(17, 53);
			this->textBox11->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox11->Name = L"textBox11";
			this->textBox11->Size = System::Drawing::Size(45, 15);
			this->textBox11->TabIndex = 73;
			this->textBox11->Text = L"00";
			this->textBox11->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox12
			// 
			this->textBox12->BackColor = System::Drawing::SystemColors::Control;
			this->textBox12->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox12->Location = System::Drawing::Point(112, 53);
			this->textBox12->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox12->Name = L"textBox12";
			this->textBox12->Size = System::Drawing::Size(45, 15);
			this->textBox12->TabIndex = 81;
			this->textBox12->Text = L"00";
			this->textBox12->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox13
			// 
			this->textBox13->BackColor = System::Drawing::SystemColors::Control;
			this->textBox13->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox13->Location = System::Drawing::Point(112, 99);
			this->textBox13->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox13->Name = L"textBox13";
			this->textBox13->Size = System::Drawing::Size(45, 15);
			this->textBox13->TabIndex = 83;
			this->textBox13->Text = L"00";
			this->textBox13->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox14
			// 
			this->textBox14->BackColor = System::Drawing::SystemColors::Control;
			this->textBox14->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox14->Location = System::Drawing::Point(17, 99);
			this->textBox14->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox14->Name = L"textBox14";
			this->textBox14->Size = System::Drawing::Size(45, 15);
			this->textBox14->TabIndex = 82;
			this->textBox14->Text = L"00";
			this->textBox14->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox15
			// 
			this->textBox15->BackColor = System::Drawing::SystemColors::Control;
			this->textBox15->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox15->Location = System::Drawing::Point(112, 145);
			this->textBox15->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox15->Name = L"textBox15";
			this->textBox15->Size = System::Drawing::Size(45, 15);
			this->textBox15->TabIndex = 85;
			this->textBox15->Text = L"00";
			this->textBox15->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox16
			// 
			this->textBox16->BackColor = System::Drawing::SystemColors::Control;
			this->textBox16->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox16->Location = System::Drawing::Point(17, 145);
			this->textBox16->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox16->Name = L"textBox16";
			this->textBox16->Size = System::Drawing::Size(45, 15);
			this->textBox16->TabIndex = 84;
			this->textBox16->Text = L"00";
			this->textBox16->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox17
			// 
			this->textBox17->BackColor = System::Drawing::SystemColors::Control;
			this->textBox17->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox17->Location = System::Drawing::Point(112, 195);
			this->textBox17->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox17->Name = L"textBox17";
			this->textBox17->Size = System::Drawing::Size(45, 15);
			this->textBox17->TabIndex = 87;
			this->textBox17->Text = L"00";
			this->textBox17->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox18
			// 
			this->textBox18->BackColor = System::Drawing::SystemColors::Control;
			this->textBox18->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox18->Location = System::Drawing::Point(17, 195);
			this->textBox18->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox18->Name = L"textBox18";
			this->textBox18->Size = System::Drawing::Size(45, 15);
			this->textBox18->TabIndex = 86;
			this->textBox18->Text = L"00";
			this->textBox18->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// Controle
			// 
			this->ClientSize = System::Drawing::Size(923, 497);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->textBox6);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->button14);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button13);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->Start_JACO);
			this->Controls->Add(this->Stop_JACO);
			this->Controls->Add(this->Message_JACO);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->groupBox2);
			this->Name = L"Controle";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

			//res = Run_MYO(/*this*/);
		}
#pragma endregion
		
		int Run_MYO(/*System::Object^ sender*/)
		{
			/*const std::string Hand_Position[4] = { "Close_Fist", "Finger_Pinch", "Neutral_Position", "Spread_Finger" };
			int number_example_Hand_Position[4] = { 0, 0, 0, 0 };
			const int range_Hand_Position_max = 3;
			const int range_Hand_Position_min = 0;

			const std::string Wrist_Movement[3] = { "Neutral", "Clockwise_Rotation", "Counter_Clockwise_Rotation" };
			int number_example_Wrist_Movement[3] = { 0, 0, 0 };
			const int range_Wrist_Movement_max = 2;
			const int range_Wrist_Movement_min = 0;*/

			//std::random_device                  rand_dev;
			//std::mt19937                        generator(rand_dev());
			//std::uniform_int_distribution<int>  distr(range_Hand_Position_min, range_Hand_Position_max);
			// We catch any exceptions that might occur below -- see the catch statement for more details.

			////try
			////{
				// First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
				// publishing your application. The Hub provides access to one or more Myos.

				myo::Hub hub("com.undercoveryeti.myo-data-capture");
				//this->Visible = 1;

				//std::cout << "Attempting to find a Myo..." << std::endl;
				this->Message_MYO->Text = L"Attempting to find a Myo...";

				// Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
				// immediately.
				// waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
				// if that fails, the function will return a null pointer.

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				myo::Myo* myo = hub.waitForMyo(300);

				// If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
				if (!myo)
				{
					//throw std::runtime_error("Unable to find a Myo!");
					this->Message_MYO->Text = "Unable to find a Myo!";
				}

				// We've found a Myo.
				//std::cout << "Connected to a Myo armband! Logging to the file system. Check the folder this appliation lives in." << std::endl << std::endl;
				//Interface_Visuel->Message_MYO->Text = "Connected to a Myo armband!";

				// Next we enable EMG streaming on the found Myo.
				//myo->setStreamEmg(myo::Myo::streamEmgEnabled);

				// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
				DataCollector collector;

				// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
				// Hub::run() to send events to all registered device listeners.
				hub.addListener(&collector);

				bool recorded = false;
				int current_class = 0;
				while (true)
				{
					hub.run(500);//Run the hub for a second
					// Next we enable EMG streaming on the found Myo.
					//myo->setStreamEmg(myo::Myo::streamEmgDisabled);
					//int classe_found = collector.classify_data();
					//myo->setStreamEmg(myo::Myo::streamEmgEnabled);
					//cout << "CLASS FOUND : " << classe_found << "\n";
					float *orientation_data = collector.get_orientation_data();
					//cout << "Roll : " << orientation_data[0] << " Pitch : " << orientation_data[1] << " Yaw : " << orientation_data[2] << "\n";
				}
				// If a standard exception occurred, we print out its message and exit.
			////}
			////catch (const std::exception& e)
			////{
				//std::cerr << "Error: " << e.what() << std::endl;
				//std::cerr << "Press enter to continue.";
				//std::cin.ignore();
				//this->Message_MYO->Text = "No Myo found___The Application will stop!";
				//return 1;
			////}

			return 0;
		}
};
}
