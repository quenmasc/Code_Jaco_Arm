#pragma once
#include "KinovaTypes.h"
#include "omp.h"
#include "Robot_IMU_Control.h"
#include "Defined_Macro.h"

namespace Interface_Controle_JACO {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	/// <summary>
	/// Description résumée de IMU_Control_UI
	/// </summary>
	public ref class IMU_Control_UI : public System::Windows::Forms::Form
	{

	public:
		// Constructeur publique
		IMU_Control_UI(void)
		{
			InitializeComponent();
		}

		// Attributs interface
	public:
		String^ Port_COM = "COM9";
		static int Packet_Size = 8;
		static array<int>^ PCKT = gcnew array<int>(Packet_Size - 1);
		static int SOF = 51;
		static int EOF = 52;
		int Baudrate = 115200;
		Robot_IMU_Control CJACO;
		int JACO_PREVIOUS_MODE = 0;
		static bool ControlJACO = 0; //ON ou OFF
		static bool Bonton_Hold = 0; //ON ou OFF
		static Byte^ Data_received = System::Convert::ToByte(0);
		//TEST
		int var_test  = 0;
		int var_test2 = 0;
		//TIMING
		double START_TIME;
		double END_TIME;
		double GLOBAL_TIME;
		double INIT_TIME;
		int Sec, Min, Hr;
	private:
		HINSTANCE commandLayer_handle;
		static int(*MyInitAPI)();
		static int(*MyCloseAPI)();
		static int(*MyMoveHome)();


	private: System::Windows::Forms::NumericUpDown^  numericUpDown6;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown5;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown4;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Button^  button25;
	private: System::Windows::Forms::CheckBox^  checkBox7;
	private: System::Windows::Forms::CheckBox^  checkBox6;
	private: System::Windows::Forms::CheckBox^  checkBox5;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::PictureBox^  pictureBox7;
	private: System::Windows::Forms::PictureBox^  pictureBox8;
	private: System::Windows::Forms::PictureBox^  pictureBox9;
	private: System::Windows::Forms::PictureBox^  pictureBox10;
	private: System::Windows::Forms::TextBox^  HOURS;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  MINUTES;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::TextBox^  SECONDS;


			 static int(*MyInitFingers)();
		static int(*MySendAdvanceTrajectory)(TrajectoryPoint);
		static int(*MyGetQuickStatus)(QuickStatus &);
		static int(*MySendJoystickCommand)(JoystickCommand);
		static int(*MySetCartesianControl)();
		static int(*MyStartControlAPI)();
		static int(*MyStopControlAPI)();


	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::GroupBox^  groupBox6;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::Button^  button20;
	private: System::Windows::Forms::Button^  button17;
	private: System::Windows::Forms::Button^  button18;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::Button^  button24;
	private: System::Windows::Forms::Button^  button11;
	private: System::Windows::Forms::TextBox^  textBox13;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::HScrollBar^  hScrollBar8;
	private: System::Windows::Forms::HScrollBar^  hScrollBar5;
	private: System::Windows::Forms::HScrollBar^  hScrollBar7;
	private: System::Windows::Forms::HScrollBar^  hScrollBar6;
	private: System::Windows::Forms::Button^  button22;
	private: System::Windows::Forms::Button^  button12;
	private: System::Windows::Forms::Button^  button21;
	private: System::Windows::Forms::Button^  button19;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::GroupBox^  groupBox25;
	private: System::Windows::Forms::Button^  button23;
	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::PictureBox^  pictureBox5;
	private: System::Windows::Forms::PictureBox^  pictureBox6;
	private: System::Windows::Forms::GroupBox^  groupBox10;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton2;
	private: System::Windows::Forms::ToolStripSplitButton^  toolStripSplitButton1;
	private: System::Windows::Forms::ToolStripMenuItem^  mappingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::GroupBox^  groupBox7;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  button15;
	private: System::Windows::Forms::Button^  button16;
	private: System::Windows::Forms::Button^  button13;
	private: System::Windows::Forms::Button^  button14;
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::HScrollBar^  hScrollBar2;
	private: System::Windows::Forms::HScrollBar^  hScrollBar1;
	private: System::Windows::Forms::HScrollBar^  hScrollBar4;
	private: System::Windows::Forms::HScrollBar^  hScrollBar3;
	private: System::Windows::Forms::TextBox^  textBox12;
	private: System::Windows::Forms::TextBox^  textBox11;
	private: System::Windows::Forms::TextBox^  textBox10;
	private: System::Windows::Forms::TextBox^  textBox9;

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~IMU_Control_UI()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::Label^  label13;
	
	protected:

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::IO::Ports::SerialPort^  serialPort2;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(IMU_Control_UI::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->serialPort2 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox7 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox6 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->button25 = (gcnew System::Windows::Forms::Button());
			this->numericUpDown6 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown5 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown4 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->button24 = (gcnew System::Windows::Forms::Button());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->button17 = (gcnew System::Windows::Forms::Button());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->textBox13 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->button20 = (gcnew System::Windows::Forms::Button());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->button18 = (gcnew System::Windows::Forms::Button());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->hScrollBar8 = (gcnew System::Windows::Forms::HScrollBar());
			this->hScrollBar5 = (gcnew System::Windows::Forms::HScrollBar());
			this->textBox12 = (gcnew System::Windows::Forms::TextBox());
			this->hScrollBar2 = (gcnew System::Windows::Forms::HScrollBar());
			this->hScrollBar7 = (gcnew System::Windows::Forms::HScrollBar());
			this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->hScrollBar6 = (gcnew System::Windows::Forms::HScrollBar());
			this->button22 = (gcnew System::Windows::Forms::Button());
			this->hScrollBar3 = (gcnew System::Windows::Forms::HScrollBar());
			this->textBox11 = (gcnew System::Windows::Forms::TextBox());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->button15 = (gcnew System::Windows::Forms::Button());
			this->button21 = (gcnew System::Windows::Forms::Button());
			this->button16 = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->button13 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button14 = (gcnew System::Windows::Forms::Button());
			this->button19 = (gcnew System::Windows::Forms::Button());
			this->hScrollBar4 = (gcnew System::Windows::Forms::HScrollBar());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox25 = (gcnew System::Windows::Forms::GroupBox());
			this->button23 = (gcnew System::Windows::Forms::Button());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox5 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox6 = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSplitButton1 = (gcnew System::Windows::Forms::ToolStripSplitButton());
			this->mappingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->pictureBox7 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox8 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox9 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox10 = (gcnew System::Windows::Forms::PictureBox());
			this->HOURS = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->MINUTES = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->SECONDS = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			this->groupBox4->SuspendLayout();
			this->groupBox7->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			this->groupBox25->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox6))->BeginInit();
			this->groupBox10->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox9))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox10))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 27);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(59, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Port COM :";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 55);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(56, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Baudrate :";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->textBox3);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->textBox2);
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(17, 34);
			this->groupBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox1->Size = System::Drawing::Size(209, 161);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Parameters:";
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(127, 85);
			this->textBox3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(76, 20);
			this->textBox3->TabIndex = 9;
			this->textBox3->Text = L"size in bytes";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(127, 127);
			this->button1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 3;
			this->button1->Text = L"SET";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button1_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 89);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(70, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Packet Size :";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(127, 53);
			this->textBox2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(76, 20);
			this->textBox2->TabIndex = 8;
			this->textBox2->Text = L"BD";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(127, 27);
			this->textBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(76, 20);
			this->textBox1->TabIndex = 7;
			this->textBox1->Text = L"COMX";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(14, 22);
			this->button2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(56, 20);
			this->button2->TabIndex = 4;
			this->button2->Text = L"ON";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(17, 493);
			this->button3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 24);
			this->button3->TabIndex = 5;
			this->button3->Text = L"RESET";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(16, 56);
			this->button4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(56, 20);
			this->button4->TabIndex = 6;
			this->button4->Text = L"OFF";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(16, 22);
			this->button5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(56, 20);
			this->button5->TabIndex = 7;
			this->button5->Text = L"ON";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(14, 56);
			this->button6->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(56, 20);
			this->button6->TabIndex = 8;
			this->button6->Text = L"OFF";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button6_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->button6);
			this->groupBox2->Controls->Add(this->button2);
			this->groupBox2->Location = System::Drawing::Point(17, 202);
			this->groupBox2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox2->Size = System::Drawing::Size(94, 87);
			this->groupBox2->TabIndex = 9;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Acquisition:";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->button5);
			this->groupBox3->Controls->Add(this->button4);
			this->groupBox3->Location = System::Drawing::Point(17, 301);
			this->groupBox3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox3->Size = System::Drawing::Size(94, 86);
			this->groupBox3->TabIndex = 10;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Control:";
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 115200;
			this->serialPort1->ReadBufferSize = 100000;
			// 
			// timer1
			// 
			this->timer1->Interval = 5;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::SystemColors::Control;
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(561, 115);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(84, 80);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 12;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(388, 115);
			this->pictureBox2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(84, 80);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 13;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->BackColor = System::Drawing::SystemColors::Control;
			this->pictureBox3->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(475, 33);
			this->pictureBox3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(84, 80);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox3->TabIndex = 14;
			this->pictureBox3->TabStop = false;
			// 
			// pictureBox4
			// 
			this->pictureBox4->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox4.Image")));
			this->pictureBox4->Location = System::Drawing::Point(475, 115);
			this->pictureBox4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(84, 80);
			this->pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox4->TabIndex = 15;
			this->pictureBox4->TabStop = false;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(34, 48);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(32, 13);
			this->label8->TabIndex = 20;
			this->label8->Text = L"Right";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(34, 74);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(25, 13);
			this->label9->TabIndex = 21;
			this->label9->Text = L"Left";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(34, 106);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(45, 13);
			this->label10->TabIndex = 22;
			this->label10->Text = L"Forward";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(34, 131);
			this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(55, 13);
			this->label11->TabIndex = 23;
			this->label11->Text = L"Backward";
			// 
			// button7
			// 
			this->button7->BackColor = System::Drawing::SystemColors::Control;
			this->button7->Location = System::Drawing::Point(8, 54);
			this->button7->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(75, 23);
			this->button7->TabIndex = 24;
			this->button7->Text = L"HOLD";
			this->button7->UseVisualStyleBackColor = false;
			this->button7->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button7_Click);
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->label13);
			this->groupBox4->Location = System::Drawing::Point(128, 460);
			this->groupBox4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox4->Size = System::Drawing::Size(666, 57);
			this->groupBox4->TabIndex = 27;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Message:";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label13->Location = System::Drawing::Point(4, 24);
			this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(560, 17);
			this->label13->TabIndex = 0;
			this->label13->Text = L"sEMG+ Control Interface, hello! :) Please entre the acquisition parameters...";
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->checkBox7);
			this->groupBox7->Controls->Add(this->checkBox6);
			this->groupBox7->Controls->Add(this->checkBox5);
			this->groupBox7->Controls->Add(this->checkBox4);
			this->groupBox7->Controls->Add(this->button25);
			this->groupBox7->Controls->Add(this->numericUpDown6);
			this->groupBox7->Controls->Add(this->numericUpDown5);
			this->groupBox7->Controls->Add(this->numericUpDown4);
			this->groupBox7->Controls->Add(this->numericUpDown3);
			this->groupBox7->Controls->Add(this->numericUpDown2);
			this->groupBox7->Controls->Add(this->numericUpDown1);
			this->groupBox7->Controls->Add(this->button24);
			this->groupBox7->Controls->Add(this->checkBox3);
			this->groupBox7->Controls->Add(this->button11);
			this->groupBox7->Controls->Add(this->button17);
			this->groupBox7->Controls->Add(this->checkBox2);
			this->groupBox7->Controls->Add(this->textBox13);
			this->groupBox7->Controls->Add(this->textBox4);
			this->groupBox7->Controls->Add(this->button20);
			this->groupBox7->Controls->Add(this->textBox6);
			this->groupBox7->Controls->Add(this->textBox10);
			this->groupBox7->Controls->Add(this->button18);
			this->groupBox7->Controls->Add(this->textBox5);
			this->groupBox7->Controls->Add(this->label19);
			this->groupBox7->Controls->Add(this->textBox9);
			this->groupBox7->Controls->Add(this->hScrollBar8);
			this->groupBox7->Controls->Add(this->hScrollBar5);
			this->groupBox7->Controls->Add(this->textBox12);
			this->groupBox7->Controls->Add(this->hScrollBar2);
			this->groupBox7->Controls->Add(this->hScrollBar7);
			this->groupBox7->Controls->Add(this->hScrollBar1);
			this->groupBox7->Controls->Add(this->hScrollBar6);
			this->groupBox7->Controls->Add(this->button22);
			this->groupBox7->Controls->Add(this->hScrollBar3);
			this->groupBox7->Controls->Add(this->textBox11);
			this->groupBox7->Controls->Add(this->button12);
			this->groupBox7->Controls->Add(this->button15);
			this->groupBox7->Controls->Add(this->button21);
			this->groupBox7->Controls->Add(this->button16);
			this->groupBox7->Controls->Add(this->button10);
			this->groupBox7->Controls->Add(this->button13);
			this->groupBox7->Controls->Add(this->button9);
			this->groupBox7->Controls->Add(this->button14);
			this->groupBox7->Controls->Add(this->button19);
			this->groupBox7->Controls->Add(this->hScrollBar4);
			this->groupBox7->Controls->Add(this->label11);
			this->groupBox7->Controls->Add(this->label20);
			this->groupBox7->Controls->Add(this->label10);
			this->groupBox7->Controls->Add(this->label9);
			this->groupBox7->Controls->Add(this->label17);
			this->groupBox7->Controls->Add(this->label8);
			this->groupBox7->Location = System::Drawing::Point(387, 206);
			this->groupBox7->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox7->Size = System::Drawing::Size(407, 250);
			this->groupBox7->TabIndex = 29;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"Speed:";
			// 
			// checkBox7
			// 
			this->checkBox7->AutoSize = true;
			this->checkBox7->Checked = true;
			this->checkBox7->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox7->Location = System::Drawing::Point(14, 193);
			this->checkBox7->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox7->Name = L"checkBox7";
			this->checkBox7->Size = System::Drawing::Size(15, 14);
			this->checkBox7->TabIndex = 61;
			this->checkBox7->UseVisualStyleBackColor = true;
			this->checkBox7->CheckedChanged += gcnew System::EventHandler(this, &IMU_Control_UI::checkBox7_CheckedChanged);
			// 
			// checkBox6
			// 
			this->checkBox6->AutoSize = true;
			this->checkBox6->Checked = true;
			this->checkBox6->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox6->Location = System::Drawing::Point(14, 170);
			this->checkBox6->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox6->Name = L"checkBox6";
			this->checkBox6->Size = System::Drawing::Size(15, 14);
			this->checkBox6->TabIndex = 60;
			this->checkBox6->UseVisualStyleBackColor = true;
			this->checkBox6->CheckedChanged += gcnew System::EventHandler(this, &IMU_Control_UI::checkBox6_CheckedChanged);
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Checked = true;
			this->checkBox5->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox5->Location = System::Drawing::Point(14, 118);
			this->checkBox5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(15, 14);
			this->checkBox5->TabIndex = 59;
			this->checkBox5->UseVisualStyleBackColor = true;
			this->checkBox5->CheckedChanged += gcnew System::EventHandler(this, &IMU_Control_UI::checkBox5_CheckedChanged);
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Checked = true;
			this->checkBox4->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox4->Location = System::Drawing::Point(14, 60);
			this->checkBox4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(15, 14);
			this->checkBox4->TabIndex = 58;
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &IMU_Control_UI::checkBox4_CheckedChanged);
			// 
			// button25
			// 
			this->button25->Location = System::Drawing::Point(355, 221);
			this->button25->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button25->Name = L"button25";
			this->button25->Size = System::Drawing::Size(40, 23);
			this->button25->TabIndex = 39;
			this->button25->Text = L"SET";
			this->button25->UseVisualStyleBackColor = true;
			this->button25->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button25_Click);
			// 
			// numericUpDown6
			// 
			this->numericUpDown6->Location = System::Drawing::Point(355, 191);
			this->numericUpDown6->Name = L"numericUpDown6";
			this->numericUpDown6->Size = System::Drawing::Size(40, 20);
			this->numericUpDown6->TabIndex = 57;
			// 
			// numericUpDown5
			// 
			this->numericUpDown5->Location = System::Drawing::Point(355, 166);
			this->numericUpDown5->Name = L"numericUpDown5";
			this->numericUpDown5->Size = System::Drawing::Size(40, 20);
			this->numericUpDown5->TabIndex = 56;
			// 
			// numericUpDown4
			// 
			this->numericUpDown4->Location = System::Drawing::Point(355, 130);
			this->numericUpDown4->Name = L"numericUpDown4";
			this->numericUpDown4->Size = System::Drawing::Size(40, 20);
			this->numericUpDown4->TabIndex = 55;
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(355, 104);
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(40, 20);
			this->numericUpDown3->TabIndex = 54;
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(355, 74);
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(40, 20);
			this->numericUpDown2->TabIndex = 53;
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(355, 47);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(40, 20);
			this->numericUpDown1->TabIndex = 39;
			// 
			// button24
			// 
			this->button24->Enabled = false;
			this->button24->Location = System::Drawing::Point(217, 118);
			this->button24->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button24->Name = L"button24";
			this->button24->Size = System::Drawing::Size(34, 20);
			this->button24->TabIndex = 51;
			this->button24->Text = L"0";
			this->button24->UseVisualStyleBackColor = true;
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Checked = true;
			this->checkBox3->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox3->Location = System::Drawing::Point(178, 21);
			this->checkBox3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(107, 17);
			this->checkBox3->TabIndex = 43;
			this->checkBox3->Text = L"Speed control v2";
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &IMU_Control_UI::checkBox3_CheckedChanged);
			// 
			// button11
			// 
			this->button11->Location = System::Drawing::Point(217, 118);
			this->button11->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(34, 20);
			this->button11->TabIndex = 51;
			this->button11->Text = L"0";
			this->button11->UseVisualStyleBackColor = true;
			// 
			// button17
			// 
			this->button17->Enabled = false;
			this->button17->Location = System::Drawing::Point(217, 191);
			this->button17->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button17->Name = L"button17";
			this->button17->Size = System::Drawing::Size(34, 20);
			this->button17->TabIndex = 42;
			this->button17->Text = L"0";
			this->button17->UseVisualStyleBackColor = true;
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(37, 21);
			this->checkBox2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(107, 17);
			this->checkBox2->TabIndex = 32;
			this->checkBox2->Text = L"Speed control v1";
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &IMU_Control_UI::checkBox2_CheckedChanged);
			// 
			// textBox13
			// 
			this->textBox13->Location = System::Drawing::Point(293, 131);
			this->textBox13->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox13->Name = L"textBox13";
			this->textBox13->Size = System::Drawing::Size(48, 20);
			this->textBox13->TabIndex = 52;
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(293, 131);
			this->textBox4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(48, 20);
			this->textBox4->TabIndex = 52;
			// 
			// button20
			// 
			this->button20->Location = System::Drawing::Point(217, 60);
			this->button20->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button20->Name = L"button20";
			this->button20->Size = System::Drawing::Size(34, 20);
			this->button20->TabIndex = 39;
			this->button20->Text = L"0";
			this->button20->UseVisualStyleBackColor = true;
			this->button20->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button20_Click);
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(293, 105);
			this->textBox6->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(48, 20);
			this->textBox6->TabIndex = 47;
			// 
			// textBox10
			// 
			this->textBox10->Location = System::Drawing::Point(293, 73);
			this->textBox10->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox10->Name = L"textBox10";
			this->textBox10->Size = System::Drawing::Size(48, 20);
			this->textBox10->TabIndex = 39;
			// 
			// button18
			// 
			this->button18->Enabled = false;
			this->button18->Location = System::Drawing::Point(217, 166);
			this->button18->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button18->Name = L"button18";
			this->button18->Size = System::Drawing::Size(34, 20);
			this->button18->TabIndex = 41;
			this->button18->Text = L"0";
			this->button18->UseVisualStyleBackColor = true;
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(293, 105);
			this->textBox5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(48, 20);
			this->textBox5->TabIndex = 47;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(34, 172);
			this->label19->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(21, 13);
			this->label19->TabIndex = 44;
			this->label19->Text = L"Up";
			// 
			// textBox9
			// 
			this->textBox9->Location = System::Drawing::Point(293, 47);
			this->textBox9->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(48, 20);
			this->textBox9->TabIndex = 32;
			// 
			// hScrollBar8
			// 
			this->hScrollBar8->Location = System::Drawing::Point(91, 129);
			this->hScrollBar8->Maximum = 360;
			this->hScrollBar8->Minimum = -360;
			this->hScrollBar8->Name = L"hScrollBar8";
			this->hScrollBar8->Size = System::Drawing::Size(83, 19);
			this->hScrollBar8->TabIndex = 50;
			// 
			// hScrollBar5
			// 
			this->hScrollBar5->Location = System::Drawing::Point(91, 129);
			this->hScrollBar5->Maximum = 360;
			this->hScrollBar5->Minimum = -360;
			this->hScrollBar5->Name = L"hScrollBar5";
			this->hScrollBar5->Size = System::Drawing::Size(83, 19);
			this->hScrollBar5->TabIndex = 50;
			// 
			// textBox12
			// 
			this->textBox12->Location = System::Drawing::Point(293, 193);
			this->textBox12->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox12->Name = L"textBox12";
			this->textBox12->Size = System::Drawing::Size(48, 20);
			this->textBox12->TabIndex = 41;
			// 
			// hScrollBar2
			// 
			this->hScrollBar2->Location = System::Drawing::Point(91, 71);
			this->hScrollBar2->Maximum = 360;
			this->hScrollBar2->Minimum = -360;
			this->hScrollBar2->Name = L"hScrollBar2";
			this->hScrollBar2->Size = System::Drawing::Size(83, 19);
			this->hScrollBar2->TabIndex = 33;
			// 
			// hScrollBar7
			// 
			this->hScrollBar7->Location = System::Drawing::Point(91, 106);
			this->hScrollBar7->Maximum = 360;
			this->hScrollBar7->Minimum = -360;
			this->hScrollBar7->Name = L"hScrollBar7";
			this->hScrollBar7->Size = System::Drawing::Size(83, 19);
			this->hScrollBar7->TabIndex = 48;
			// 
			// hScrollBar1
			// 
			this->hScrollBar1->Location = System::Drawing::Point(91, 48);
			this->hScrollBar1->Maximum = 360;
			this->hScrollBar1->Minimum = -360;
			this->hScrollBar1->Name = L"hScrollBar1";
			this->hScrollBar1->Size = System::Drawing::Size(83, 19);
			this->hScrollBar1->TabIndex = 32;
			// 
			// hScrollBar6
			// 
			this->hScrollBar6->Location = System::Drawing::Point(91, 106);
			this->hScrollBar6->Maximum = 360;
			this->hScrollBar6->Minimum = -360;
			this->hScrollBar6->Name = L"hScrollBar6";
			this->hScrollBar6->Size = System::Drawing::Size(83, 19);
			this->hScrollBar6->TabIndex = 48;
			// 
			// button22
			// 
			this->button22->Location = System::Drawing::Point(255, 118);
			this->button22->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button22->Name = L"button22";
			this->button22->Size = System::Drawing::Size(34, 20);
			this->button22->TabIndex = 49;
			this->button22->Text = L"max";
			this->button22->UseVisualStyleBackColor = true;
			this->button22->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button22_Click);
			// 
			// hScrollBar3
			// 
			this->hScrollBar3->Location = System::Drawing::Point(91, 166);
			this->hScrollBar3->Maximum = 360;
			this->hScrollBar3->Minimum = -360;
			this->hScrollBar3->Name = L"hScrollBar3";
			this->hScrollBar3->Size = System::Drawing::Size(83, 19);
			this->hScrollBar3->TabIndex = 33;
			// 
			// textBox11
			// 
			this->textBox11->Location = System::Drawing::Point(293, 168);
			this->textBox11->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox11->Name = L"textBox11";
			this->textBox11->Size = System::Drawing::Size(48, 20);
			this->textBox11->TabIndex = 40;
			// 
			// button12
			// 
			this->button12->Location = System::Drawing::Point(255, 118);
			this->button12->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(34, 20);
			this->button12->TabIndex = 49;
			this->button12->Text = L"max";
			this->button12->UseVisualStyleBackColor = true;
			// 
			// button15
			// 
			this->button15->Location = System::Drawing::Point(255, 191);
			this->button15->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button15->Name = L"button15";
			this->button15->Size = System::Drawing::Size(34, 20);
			this->button15->TabIndex = 38;
			this->button15->Text = L"max";
			this->button15->UseVisualStyleBackColor = true;
			this->button15->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button15_Click);
			// 
			// button21
			// 
			this->button21->Location = System::Drawing::Point(178, 118);
			this->button21->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button21->Name = L"button21";
			this->button21->Size = System::Drawing::Size(34, 20);
			this->button21->TabIndex = 46;
			this->button21->Text = L"min";
			this->button21->UseVisualStyleBackColor = true;
			this->button21->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button21_Click);
			// 
			// button16
			// 
			this->button16->Location = System::Drawing::Point(178, 191);
			this->button16->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button16->Name = L"button16";
			this->button16->Size = System::Drawing::Size(34, 20);
			this->button16->TabIndex = 37;
			this->button16->Text = L"min";
			this->button16->UseVisualStyleBackColor = true;
			this->button16->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button16_Click);
			// 
			// button10
			// 
			this->button10->Location = System::Drawing::Point(255, 60);
			this->button10->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(34, 20);
			this->button10->TabIndex = 32;
			this->button10->Text = L"max";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button10_Click);
			// 
			// button13
			// 
			this->button13->Location = System::Drawing::Point(255, 166);
			this->button13->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button13->Name = L"button13";
			this->button13->Size = System::Drawing::Size(34, 20);
			this->button13->TabIndex = 36;
			this->button13->Text = L"max";
			this->button13->UseVisualStyleBackColor = true;
			this->button13->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button13_Click);
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(178, 60);
			this->button9->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(34, 20);
			this->button9->TabIndex = 24;
			this->button9->Text = L"min";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button9_Click);
			// 
			// button14
			// 
			this->button14->Location = System::Drawing::Point(178, 166);
			this->button14->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button14->Name = L"button14";
			this->button14->Size = System::Drawing::Size(34, 20);
			this->button14->TabIndex = 35;
			this->button14->Text = L"min";
			this->button14->UseVisualStyleBackColor = true;
			this->button14->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button14_Click);
			// 
			// button19
			// 
			this->button19->Location = System::Drawing::Point(178, 118);
			this->button19->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button19->Name = L"button19";
			this->button19->Size = System::Drawing::Size(34, 20);
			this->button19->TabIndex = 46;
			this->button19->Text = L"min";
			this->button19->UseVisualStyleBackColor = true;
			// 
			// hScrollBar4
			// 
			this->hScrollBar4->Location = System::Drawing::Point(91, 192);
			this->hScrollBar4->Maximum = 360;
			this->hScrollBar4->Minimum = -360;
			this->hScrollBar4->Name = L"hScrollBar4";
			this->hScrollBar4->Size = System::Drawing::Size(83, 19);
			this->hScrollBar4->TabIndex = 34;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(34, 192);
			this->label20->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(35, 13);
			this->label20->TabIndex = 45;
			this->label20->Text = L"Down";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(34, 192);
			this->label17->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(25, 13);
			this->label17->TabIndex = 45;
			this->label17->Text = L"Left";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(8, 25);
			this->checkBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(66, 17);
			this->checkBox1->TabIndex = 31;
			this->checkBox1->Text = L"Reverse";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &IMU_Control_UI::checkBox1_CheckedChanged);
			// 
			// groupBox25
			// 
			this->groupBox25->Controls->Add(this->button23);
			this->groupBox25->Controls->Add(this->textBox7);
			this->groupBox25->Location = System::Drawing::Point(17, 397);
			this->groupBox25->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox25->Name = L"groupBox25";
			this->groupBox25->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox25->Size = System::Drawing::Size(94, 78);
			this->groupBox25->TabIndex = 30;
			this->groupBox25->TabStop = false;
			this->groupBox25->Text = L"Test";
			// 
			// button23
			// 
			this->button23->Location = System::Drawing::Point(34, 43);
			this->button23->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button23->Name = L"button23";
			this->button23->Size = System::Drawing::Size(46, 23);
			this->button23->TabIndex = 28;
			this->button23->Text = L"Test1";
			this->button23->UseVisualStyleBackColor = true;
			this->button23->Click += gcnew System::EventHandler(this, &IMU_Control_UI::button23_Click);
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(10, 18);
			this->textBox7->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(71, 20);
			this->textBox7->TabIndex = 26;
			// 
			// pictureBox5
			// 
			this->pictureBox5->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox5.Image")));
			this->pictureBox5->Location = System::Drawing::Point(304, 100);
			this->pictureBox5->Name = L"pictureBox5";
			this->pictureBox5->Size = System::Drawing::Size(62, 64);
			this->pictureBox5->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox5->TabIndex = 32;
			this->pictureBox5->TabStop = false;
			this->pictureBox5->Click += gcnew System::EventHandler(this, &IMU_Control_UI::pictureBox5_Click);
			// 
			// pictureBox6
			// 
			this->pictureBox6->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox6.Image")));
			this->pictureBox6->Location = System::Drawing::Point(304, 167);
			this->pictureBox6->Name = L"pictureBox6";
			this->pictureBox6->Size = System::Drawing::Size(62, 64);
			this->pictureBox6->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox6->TabIndex = 33;
			this->pictureBox6->TabStop = false;
			this->pictureBox6->Click += gcnew System::EventHandler(this, &IMU_Control_UI::pictureBox6_Click);
			// 
			// groupBox10
			// 
			this->groupBox10->Controls->Add(this->checkBox1);
			this->groupBox10->Controls->Add(this->button7);
			this->groupBox10->Location = System::Drawing::Point(160, 255);
			this->groupBox10->Name = L"groupBox10";
			this->groupBox10->Size = System::Drawing::Size(94, 83);
			this->groupBox10->TabIndex = 35;
			this->groupBox10->TabStop = false;
			this->groupBox10->Text = L"Control buttons:";
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->toolStripButton2,
					this->toolStripSplitButton1, this->toolStripButton1
			});
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(809, 25);
			this->toolStrip1->TabIndex = 36;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripButton2
			// 
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton2.Image")));
			this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton2->Name = L"toolStripButton2";
			this->toolStripButton2->Size = System::Drawing::Size(60, 22);
			this->toolStripButton2->Text = L"Home";
			// 
			// toolStripSplitButton1
			// 
			this->toolStripSplitButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->toolStripSplitButton1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->mappingToolStripMenuItem,
					this->toolStripMenuItem2
			});
			this->toolStripSplitButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripSplitButton1.Image")));
			this->toolStripSplitButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripSplitButton1->Name = L"toolStripSplitButton1";
			this->toolStripSplitButton1->Size = System::Drawing::Size(65, 22);
			this->toolStripSplitButton1->Text = L"Options";
			// 
			// mappingToolStripMenuItem
			// 
			this->mappingToolStripMenuItem->Name = L"mappingToolStripMenuItem";
			this->mappingToolStripMenuItem->Size = System::Drawing::Size(122, 22);
			this->mappingToolStripMenuItem->Text = L"Mapping";
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(122, 22);
			this->toolStripMenuItem2->Text = L"Margins";
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(36, 22);
			this->toolStripButton1->Text = L"Help";
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->label5);
			this->groupBox5->Controls->Add(this->label4);
			this->groupBox5->Location = System::Drawing::Point(698, 34);
			this->groupBox5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox5->Size = System::Drawing::Size(96, 44);
			this->groupBox5->TabIndex = 37;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Frequency:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(57, 18);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(20, 13);
			this->label5->TabIndex = 1;
			this->label5->Text = L"Hz";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(21, 18);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(13, 13);
			this->label4->TabIndex = 0;
			this->label4->Text = L"0";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->label6);
			this->groupBox6->Controls->Add(this->label7);
			this->groupBox6->Location = System::Drawing::Point(698, 90);
			this->groupBox6->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox6->Size = System::Drawing::Size(96, 44);
			this->groupBox6->TabIndex = 38;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"RT Delay:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(57, 18);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(20, 13);
			this->label6->TabIndex = 1;
			this->label6->Text = L"ms";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(21, 18);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(13, 13);
			this->label7->TabIndex = 0;
			this->label7->Text = L"0";
			// 
			// pictureBox7
			// 
			this->pictureBox7->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox7.Image")));
			this->pictureBox7->Location = System::Drawing::Point(304, 233);
			this->pictureBox7->Name = L"pictureBox7";
			this->pictureBox7->Size = System::Drawing::Size(62, 64);
			this->pictureBox7->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox7->TabIndex = 39;
			this->pictureBox7->TabStop = false;
			// 
			// pictureBox8
			// 
			this->pictureBox8->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox8.Image")));
			this->pictureBox8->Location = System::Drawing::Point(304, 34);
			this->pictureBox8->Name = L"pictureBox8";
			this->pictureBox8->Size = System::Drawing::Size(62, 64);
			this->pictureBox8->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox8->TabIndex = 40;
			this->pictureBox8->TabStop = false;
			// 
			// pictureBox9
			// 
			this->pictureBox9->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox9.Image")));
			this->pictureBox9->Location = System::Drawing::Point(304, 300);
			this->pictureBox9->Name = L"pictureBox9";
			this->pictureBox9->Size = System::Drawing::Size(62, 64);
			this->pictureBox9->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox9->TabIndex = 41;
			this->pictureBox9->TabStop = false;
			// 
			// pictureBox10
			// 
			this->pictureBox10->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox10.Image")));
			this->pictureBox10->Location = System::Drawing::Point(304, 366);
			this->pictureBox10->Name = L"pictureBox10";
			this->pictureBox10->Size = System::Drawing::Size(62, 64);
			this->pictureBox10->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox10->TabIndex = 42;
			this->pictureBox10->TabStop = false;
			// 
			// HOURS
			// 
			this->HOURS->Location = System::Drawing::Point(573, 11);
			this->HOURS->Margin = System::Windows::Forms::Padding(2);
			this->HOURS->Name = L"HOURS";
			this->HOURS->Size = System::Drawing::Size(46, 20);
			this->HOURS->TabIndex = 10;
			this->HOURS->Text = L"00";
			this->HOURS->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(621, 14);
			this->label12->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(23, 13);
			this->label12->TabIndex = 10;
			this->label12->Text = L"Hrs";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(694, 14);
			this->label14->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(29, 13);
			this->label14->TabIndex = 43;
			this->label14->Text = L"Mins";
			// 
			// MINUTES
			// 
			this->MINUTES->Location = System::Drawing::Point(646, 11);
			this->MINUTES->Margin = System::Windows::Forms::Padding(2);
			this->MINUTES->Name = L"MINUTES";
			this->MINUTES->Size = System::Drawing::Size(46, 20);
			this->MINUTES->TabIndex = 44;
			this->MINUTES->Text = L"00";
			this->MINUTES->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(772, 14);
			this->label15->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(26, 13);
			this->label15->TabIndex = 45;
			this->label15->Text = L"Sec";
			// 
			// SECONDS
			// 
			this->SECONDS->Location = System::Drawing::Point(724, 11);
			this->SECONDS->Margin = System::Windows::Forms::Padding(2);
			this->SECONDS->Name = L"SECONDS";
			this->SECONDS->Size = System::Drawing::Size(46, 20);
			this->SECONDS->TabIndex = 46;
			this->SECONDS->Text = L"00";
			this->SECONDS->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// IMU_Control_UI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(809, 529);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->SECONDS);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->MINUTES);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->HOURS);
			this->Controls->Add(this->pictureBox10);
			this->Controls->Add(this->pictureBox9);
			this->Controls->Add(this->pictureBox8);
			this->Controls->Add(this->pictureBox7);
			this->Controls->Add(this->groupBox6);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->groupBox10);
			this->Controls->Add(this->pictureBox6);
			this->Controls->Add(this->pictureBox5);
			this->Controls->Add(this->groupBox25);
			this->Controls->Add(this->groupBox7);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->groupBox1);
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"IMU_Control_UI";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"IMU_Control_UI";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			this->groupBox25->ResumeLayout(false);
			this->groupBox25->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox6))->EndInit();
			this->groupBox10->ResumeLayout(false);
			this->groupBox10->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox9))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox10))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion

		//CLIC on SET*****************************************************************************************
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 if (this->textBox1->Text != "COMX")
					 this->Port_COM = this->textBox1->Text;
				 else
					 this->textBox1->Text = this->Port_COM;

				 if (this->textBox2->Text != "BD")
					 this->Baudrate = System::Convert::ToInt32(this->textBox2->Text);
				 else
					 this->textBox2->Text = this->Baudrate.ToString();

				 if (this->textBox3->Text != "size in bytes")
					 this->Packet_Size = System::Convert::ToInt32(this->textBox3->Text);
				 else
					 this->textBox3->Text = this->Packet_Size.ToString();

				 CJACO.Marges[0] = 5;  CJACO.Marges[1] = 5;
				 this->numericUpDown1->Value = CJACO.Marges[0];
				 this->numericUpDown2->Value = CJACO.Marges[1];
				 CJACO.Marges[2] = 5;  CJACO.Marges[3] = 5;
				 this->numericUpDown3->Value = CJACO.Marges[2];
				 this->numericUpDown4->Value = CJACO.Marges[3];
				 CJACO.Marges[4] = 5;  CJACO.Marges[5] = 5;
				 this->numericUpDown5->Value = CJACO.Marges[4];
				 this->numericUpDown6->Value = CJACO.Marges[5];

				 this->label13->Text = "Please start the acquisition...";
	}
		//*********************************************************************************************************
		//CLIC on CONTROLE ON**************************************************************************************
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 // 1st step = creer, instancier, activer les instances de contrôle de JACO

				 commandLayer_handle = LoadLibrary(L"Kinova.API.UsbCommandLayer.dll");

				 if (commandLayer_handle == NULL)
				 {
					 this->label13->Text = "Error while loading library. Cannot perform test. Leaving,,,";
				 }
				 else
				 {
					 this->label13->Text = "API loaded Successfully";
				 }

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
				 if ((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyMoveHome == NULL) || (MySendJoystickCommand == NULL) ||
					 (MyStartControlAPI == NULL) || (MyStopControlAPI == NULL) || (MySetCartesianControl == NULL) || (MyGetQuickStatus == NULL))
				 {
					 this->label13->Text = "Cannot load all the functions from the API. Please reinit the system";
				 }
				 else
				 {
					 int result = (*MyInitAPI)();
					 //this->textBox3->Text = Convert::ToString(result);
					 if (result == 1)
					 {
						 this->label13->Text = "The result of InitAPI = 1";
						 // Puts JACO in init position
						 (*MyMoveHome)();
						 (*MySetCartesianControl)();
						 
						 //JACO_PREVIOUS_MODE = XYZ;
						 
						 (*MyStartControlAPI)();
						 this->label13->Text = "JACO is ready... have fun ;)";
						 this->ControlJACO = 1;
					 }
					 else
						 this->label13->Text = "The result of InitAPI != 1";
					 // 2nd step = modifier la variable booléenne ControlJACO
				 }
				 this->button4->BackColor = System::Drawing::SystemColors::Control;
				 this->button5->BackColor = System::Drawing::SystemColors::Highlight;
				 this->serialPort1->ReadExisting(); //Clear the buffer for new packets
	}
		//*********************************************************************************************************
		//CLIC on CONTROLE OFF*************************************************************************************
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Click on Control OFF
				 (*MySendJoystickCommand)(CJACO.Control(0));
				 (*MyMoveHome)();
				 (*MyStopControlAPI)();
				 int result = (*MyCloseAPI)();
				 this->label13->Text = "JACO's control has been stopped...";
				 this->ControlJACO = 0;
				 this->button5->BackColor = System::Drawing::SystemColors::Control;
				 this->button4->BackColor = System::Drawing::SystemColors::Highlight;
				 this->serialPort1->ReadExisting(); //Clear the buffer for new packets
	}
		//*********************************************************************************************************
		//CLIC ON ACQUISITION ON***********************************************************************************
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Click on Acquisition ON
				 this->serialPort1->BaudRate = this->Baudrate;
				 this->serialPort1->PortName = this->Port_COM;
				 this->serialPort1->Open();
				 this->timer1->Tick += gcnew System::EventHandler(this, &IMU_Control_UI::Timer1Event);
				 this->timer1->Start();
				 this->label13->Text = "Acquisition started...";
				 this->button6->BackColor = System::Drawing::SystemColors::Control;
				 this->button2->BackColor = System::Drawing::SystemColors::Highlight;

				 INIT_TIME = omp_get_wtime();
	}
		//*********************************************************************************************************
		//CLIC on ACQUISITION OFF**********************************************************************************
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Click on Acquisition OFF
				 this->timer1->Stop();
				 this->serialPort1->Close();
				 this->label13->Text = "Acquisition stopped...";
				 this->button2->BackColor = System::Drawing::SystemColors::Control;
				 this->button6->BackColor = System::Drawing::SystemColors::Highlight;
	}
		//*********************************************************************************************************
		//ROUTINE**************************************************************************************************
	public:
		void Timer1Event(System::Object^ sender, System::EventArgs^ e)
		{
			QuickStatus JACO_STATUS;
			int cpt = 0;
			var_test++;
			this->timer1->Stop();
			while (serialPort1->ReadByte() != SOF)
			{
				this->textBox7->Text = "Waiting USB Packet";
			}
			START_TIME = omp_get_wtime();
			
			//READING BUFFER 
			for (cpt = 0; cpt < Packet_Size - 1; cpt++)
			{
				PCKT[cpt] = serialPort1->ReadByte();
			}
			
			//INTERPRETER********************************************
			if (PCKT[Packet_Size-2] == EOF)
				CJACO.Interpret(PCKT);
			
			///BUTTON CHECKING***************************************
			//*******************************************************

			//ACQUISTION FREQUENCY***********************************
			this->label4->Text = Convert::ToString(CJACO.FREQ);
			
			//SEND COMMANDS
			if (this->Bonton_Hold == 1 && this->ControlJACO == 1)
			{
				//Prendre en compte l'état du bouton avant d'arrêter un mouvement
				int result = (*MySendJoystickCommand)(CJACO.Control(1));
				if (result == 1)
				{
					this->label13->Text = "Command transmitted to JACO";
				}
				else
					this->label13->Text = "Command not transmitted to JACO, please retry...";
			}
			else if (this->Bonton_Hold == 0 && this->ControlJACO == 1)
			{
				int result = (*MySendJoystickCommand)(CJACO.Control(0));
				if (result == 1)
				{
					this->label13->Text = "Command transmitted to JACO";
				}
				else
					this->label13->Text = "Command not transmitted to JACO, please retry...";
			}
			//END OF COMMAND SENDING

			//SETTING SCROLLBARS
			if ((CJACO.PitchS1p + CJACO.PitchS1p_0) <= this->hScrollBar1->Maximum && (CJACO.PitchS1p + CJACO.PitchS1p_0) >= this->hScrollBar1->Minimum)
				this->hScrollBar1->Value = (CJACO.PitchS1p + CJACO.PitchS1p_0);
			else if ((CJACO.PitchS1p + CJACO.PitchS1p_0) >= this->hScrollBar1->Maximum)
				this->hScrollBar1->Value = this->hScrollBar1->Maximum;						//PitchS1p > 0 ALWAYS TRUE
			else if ((CJACO.PitchS1p + CJACO.PitchS1p_0) <= this->hScrollBar1->Minimum)
				this->hScrollBar1->Value = this->hScrollBar1->Minimum;

			if ((CJACO.PitchS1m + CJACO.PitchS1m_0) <= this->hScrollBar2->Maximum && (CJACO.PitchS1m + CJACO.PitchS1m_0) >= this->hScrollBar2->Minimum)
				this->hScrollBar2->Value = (CJACO.PitchS1m + CJACO.PitchS1m_0);
			else if ((CJACO.PitchS1m + CJACO.PitchS1m_0) >= this->hScrollBar2->Maximum)
				this->hScrollBar2->Value = this->hScrollBar2->Maximum;						//PitchS1m > 0 ALWAYS TRUE
			else if ((CJACO.PitchS1m + CJACO.PitchS1m_0) <= this->hScrollBar2->Minimum)
				this->hScrollBar2->Value = this->hScrollBar2->Minimum;

			if ((CJACO.RollS1p + CJACO.RollS1p_0) <= this->hScrollBar7->Maximum && (CJACO.RollS1p + CJACO.RollS1p_0) >= this->hScrollBar7->Minimum)
				this->hScrollBar7->Value = (CJACO.RollS1p + CJACO.RollS1p_0);
			else if ((CJACO.RollS1p + CJACO.RollS1p_0) >= this->hScrollBar7->Maximum)
				this->hScrollBar7->Value = this->hScrollBar7->Maximum;						//PitchS1p > 0 ALWAYS TRUE
			else if ((CJACO.RollS1p + CJACO.RollS1p_0) <= this->hScrollBar7->Minimum)
				this->hScrollBar7->Value = this->hScrollBar7->Minimum;

			if ((CJACO.RollS1m + CJACO.RollS1m_0) <= this->hScrollBar8->Maximum && (CJACO.RollS1m + CJACO.RollS1m_0) >= this->hScrollBar8->Minimum)
				this->hScrollBar8->Value = (CJACO.RollS1m + CJACO.RollS1m_0);
			else if ((CJACO.RollS1m + CJACO.RollS1m_0) >= this->hScrollBar8->Maximum)
				this->hScrollBar8->Value = this->hScrollBar8->Maximum;						//PitchS1m > 0 ALWAYS TRUE
			else if ((CJACO.RollS1m + CJACO.RollS1m_0) <= this->hScrollBar8->Minimum)
				this->hScrollBar8->Value = this->hScrollBar8->Minimum;

			if (CJACO.PitchS2 <= this->hScrollBar3->Maximum && CJACO.PitchS2 >= this->hScrollBar3->Minimum)
				this->hScrollBar3->Value = CJACO.PitchS2;
			else if (CJACO.PitchS2 >= this->hScrollBar3->Maximum)
				this->hScrollBar3->Value = this->hScrollBar3->Maximum;
			else if (CJACO.PitchS2 <= this->hScrollBar3->Minimum)
				this->hScrollBar3->Value = this->hScrollBar3->Minimum;

			if (CJACO.PitchS3 <= this->hScrollBar4->Maximum && CJACO.PitchS3 >= this->hScrollBar4->Minimum)
				this->hScrollBar4->Value = CJACO.PitchS3;
			else if (CJACO.PitchS3 >= this->hScrollBar4->Maximum)
				this->hScrollBar4->Value = this->hScrollBar4->Maximum;
			else if (CJACO.PitchS3 <= this->hScrollBar4->Minimum)
				this->hScrollBar4->Value = this->hScrollBar4->Minimum;
			//END OF SCROLLBAR SETTING

			//TEXTBOXES SETTING
				//PitchS1
			this->textBox9->Text = Convert::ToString(CJACO.PitchS1p);
			if (CJACO.PitchS1p <= CJACO.PitchS1p_0 + CJACO.PitchS1_Mp)
				this->textBox9->BackColor = System::Drawing::SystemColors::Highlight;
			else
				this->textBox9->BackColor = System::Drawing::SystemColors::Control;

			this->textBox10->Text = Convert::ToString(CJACO.PitchS1m);
			if (CJACO.PitchS1m <= CJACO.PitchS1m_0 + CJACO.PitchS1_Mm)
				this->textBox10->BackColor = System::Drawing::SystemColors::Highlight;
			else
				this->textBox10->BackColor = System::Drawing::SystemColors::Control;
				//RollS1
			this->textBox6->Text = Convert::ToString(CJACO.RollS1p);
			if (CJACO.RollS1p  <= CJACO.RollS1p_0 + CJACO.RollS1_Mp)
				this->textBox6->BackColor = System::Drawing::SystemColors::Highlight;
			else
				this->textBox6->BackColor = System::Drawing::SystemColors::Control;

			this->textBox13->Text = Convert::ToString(CJACO.RollS1m);
			if (CJACO.RollS1m <= CJACO.RollS1m_0 + CJACO.RollS1_Mm)
				this->textBox13->BackColor = System::Drawing::SystemColors::Highlight;
			else
				this->textBox13->BackColor = System::Drawing::SystemColors::Control;
				//PitchS2
			this->textBox11->Text = Convert::ToString(CJACO.PitchS2);
			if (CJACO.PitchS2 <= CJACO.PitchS2_0 + CJACO.PitchS2_M)
				this->textBox11->BackColor = System::Drawing::SystemColors::Highlight;
			else
				this->textBox11->BackColor = System::Drawing::SystemColors::Control;

			this->textBox12->Text = Convert::ToString(CJACO.PitchS3);
			if (CJACO.PitchS3 <= CJACO.PitchS3_0 + CJACO.PitchS3_M)
				this->textBox12->BackColor = System::Drawing::SystemColors::Highlight;
			else
				this->textBox12->BackColor = System::Drawing::SystemColors::Control;
			//END OF TEXTBOXES SETTING

			//ARROWS' SETTINGS
				//RL
			if (CJACO.RightLeftComm > 0.3)
			{
				this->pictureBox1->BackColor = System::Drawing::SystemColors::Highlight;
				this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
				this->pictureBox2->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox2->BorderStyle = System::Windows::Forms::BorderStyle::None;
			}
			else if (CJACO.RightLeftComm < -0.3)
			{
				this->pictureBox1->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox2->BackColor = System::Drawing::SystemColors::Highlight;
				this->pictureBox2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			}
			else
			{
				this->pictureBox1->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox2->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox2->BorderStyle = System::Windows::Forms::BorderStyle::None;
			}
				//FB
			if (CJACO.ForBackComm > 0.3)
			{
				this->pictureBox3->BackColor = System::Drawing::SystemColors::Highlight;
				this->pictureBox3->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
				this->pictureBox4->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox4->BorderStyle = System::Windows::Forms::BorderStyle::None;
			}
			else if (CJACO.ForBackComm < -0.3)
			{
				this->pictureBox3->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox3->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox4->BackColor = System::Drawing::SystemColors::Highlight;
				this->pictureBox4->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			}
			else
			{
				this->pictureBox3->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox3->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox4->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox4->BorderStyle = System::Windows::Forms::BorderStyle::None;
			}
				//BUTTON2, BUTTON3, BUTTON HOME
			if (CJACO.HOME_PRESSED)
			{
				this->pictureBox7->BackColor = System::Drawing::SystemColors::Highlight;
				this->pictureBox7->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
				this->pictureBox5->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox5->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox6->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox6->BorderStyle = System::Windows::Forms::BorderStyle::None;
			}
			else if (CJACO.PitchS2 - CJACO.PitchS2_0 >= CJACO.PitchS2_M)
			{
				this->pictureBox5->BackColor = System::Drawing::SystemColors::Highlight;
				this->pictureBox5->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
				this->pictureBox6->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox6->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox7->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox7->BorderStyle = System::Windows::Forms::BorderStyle::None;
			}
			else if (CJACO.PitchS3 - CJACO.PitchS3_0 >= CJACO.PitchS3_M)
			{
				this->pictureBox5->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox5->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox6->BackColor = System::Drawing::SystemColors::Highlight;
				this->pictureBox6->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
				this->pictureBox7->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox7->BorderStyle = System::Windows::Forms::BorderStyle::None;
			}
			else
			{
				this->pictureBox5->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox5->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox6->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox6->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->pictureBox7->BackColor = System::Drawing::SystemColors::Control;
				this->pictureBox7->BorderStyle = System::Windows::Forms::BorderStyle::None;
			}
			///////////////////////////////////////////////////////////////////////////////
			END_TIME = omp_get_wtime() - START_TIME;
			this->label7->Text = System::Convert::ToString((float)(END_TIME * 1000));
			this->timer1->Enabled = true;
			///////////////////////////////////////////////////////////////////////////////
			GLOBAL_TIME = (omp_get_wtime() - INIT_TIME);
			Hr  = (int)	GLOBAL_TIME / 3600;
			double var1 = GLOBAL_TIME - (Hr * 3600);
			Min = (int)	var1 / 60;
			double var2 = var1 - (Min * 60);
			Sec = (int)	var2;
			this->SECONDS->Text = System::Convert::ToString(Sec);
			this->MINUTES->Text = System::Convert::ToString(Min);
			this->HOURS->Text = System::Convert::ToString(Hr);
		}
		//FIN ROUTINE**********************************************************************************************
		//CLIC on HOLD*********************************************************************************************
	private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Click on HOLD
				 //Modification de la variable Bouton_Hold et arret du Robot
				 if (this->Bonton_Hold == 0)
				 {
					 this->Bonton_Hold = 1;
					 this->CJACO.Hold_State = 1;
					 this->button7->BackColor = System::Drawing::SystemColors::Highlight;
				 }
				 else
				 {
					 this->Bonton_Hold = 0;
					 this->CJACO.Hold_State = 1;
					 //Arreter le robot
					 this->button7->BackColor = System::Drawing::SystemColors::Control;
				 }
	}
		//*********************************************************************************************************
		//SENS*****************************************************************************************************
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
				 if (this->checkBox1->Checked == 0)
					 this->CJACO.Sens = 1;
				 else
					 this->CJACO.Sens = -1;
	}
		//**********************************************************************************************************
		//CONTROLE EN VITESSE***************************************************************************************
	private: System::Void checkBox2_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
				 if (this->checkBox2->Checked == 0)
				 {
					 this->CJACO.Speed_Ctrl_v1 = 0;
				 }
				 else
				 {
					 this->CJACO.Speed_Ctrl_v1 = 1;
					 this->CJACO.Speed_Ctrl_v2 = 0;
					 this->checkBox3->Checked = 0;
				 }
	}
	private: System::Void checkBox3_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
				 if (this->checkBox3->Checked == 0)
				 {
					 this->CJACO.Speed_Ctrl_v2 = 0;
				 }
				 else
				 {
					 this->CJACO.Speed_Ctrl_v2 = 1;
					 this->CJACO.Speed_Ctrl_v1 = 0;
					 this->checkBox2->Checked = 0;
				 }
	}
		//**********************************************************************************************************
		//SET MARGINS***********************************************************************************************
	private: System::Void button25_Click(System::Object^  sender, System::EventArgs^  e) 
	{
				 CJACO.Marges[0] = Convert::ToInt16(this->numericUpDown1->Value);
				 CJACO.Marges[1] = Convert::ToInt16(this->numericUpDown2->Value);
				 CJACO.Marges[2] = Convert::ToInt16(this->numericUpDown3->Value);
				 CJACO.Marges[3] = Convert::ToInt16(this->numericUpDown4->Value);
				 CJACO.Marges[4] = Convert::ToInt16(this->numericUpDown5->Value);
				 CJACO.Marges[5] = Convert::ToInt16(this->numericUpDown6->Value);
	}
		//**********************************************************************************************************
		//CALIBRAGE ANGLES******************************************************************************************
	private: System::Void button20_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on 0, hScrollBar1-2 and hScrollBar7-8
					//PitchS1
				 if (this->CJACO.PitchS1p > this->CJACO.PitchS1m)
				 {
					 this->hScrollBar1->Minimum = this->CJACO.PitchS1p;
					 this->hScrollBar2->Minimum = -1 * this->CJACO.PitchS1p;
					 this->CJACO.PitchS1p_0		= this->CJACO.PitchS1p;
					 this->CJACO.PitchS1m_0		= -1 * this->CJACO.PitchS1p;
					 this->textBox9->Text		= Convert::ToString(CJACO.PitchS1p_0);	//PitchS1p
					 this->textBox10->Text		= Convert::ToString(CJACO.PitchS1m_0);	//PitchS1m
				 }
				 else
				 {
					 this->hScrollBar1->Minimum = -1 * this->CJACO.PitchS1m;
					 this->hScrollBar2->Minimum = this->CJACO.PitchS1m;
					 this->CJACO.PitchS1p_0		= -1 * this->CJACO.PitchS1m;
					 this->CJACO.PitchS1m_0		= this->CJACO.PitchS1m;
					 this->textBox9->Text		= Convert::ToString(CJACO.PitchS1p_0);	//PitchS1p
					 this->textBox10->Text		= Convert::ToString(CJACO.PitchS1m_0);	//PitchS1m				 
				 }
				 CJACO.Config_RL = CJACO.Config_RL + 1;
					
					//RollS1
				 if (this->CJACO.RollS1p > this->CJACO.RollS1m)
				 {
					 this->hScrollBar7->Minimum = this->CJACO.RollS1p;
					 this->hScrollBar8->Minimum = -1 * this->CJACO.RollS1p;
					 this->CJACO.RollS1p_0 = this->CJACO.RollS1p;
					 this->CJACO.RollS1m_0 = -1 * this->CJACO.RollS1p;
					 this->textBox6->Text = Convert::ToString(CJACO.RollS1p_0);		//RollS1p
					 this->textBox13->Text = Convert::ToString(CJACO.RollS1m_0);	//RollS1m
				 }
				 else
				 {
					 this->hScrollBar7->Minimum = -1 * this->CJACO.RollS1m;
					 this->hScrollBar8->Minimum = this->CJACO.RollS1m;
					 this->CJACO.RollS1p_0 = -1 * this->CJACO.RollS1m;
					 this->CJACO.RollS1m_0 = this->CJACO.RollS1m;
					 this->textBox6->Text = Convert::ToString(CJACO.RollS1p_0);		//RollS1p
					 this->textBox13->Text = Convert::ToString(CJACO.RollS1m_0);	//RollS1m
				 }
				 CJACO.Config_FB				= CJACO.Config_FB + 1;
	}
	private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on min, hScrollBar1-2
				 this->hScrollBar2->Maximum = this->CJACO.PitchS1m + this->CJACO.PitchS1m_0; //Left Speed - PitchS1m's real value < PitchS1_0 
				 this->CJACO.PitchS1_min = this->hScrollBar2->Maximum;
				 this->textBox9->Text = Convert::ToString(CJACO.PitchS1p_0);		//PitchS1p
				 this->textBox10->Text = Convert::ToString(CJACO.PitchS1_min);		//PitchS1m
				 CJACO.Config_RL = CJACO.Config_RL + 1;
	}
	private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on max, hScrollBar1-2
				 this->hScrollBar1->Maximum = this->CJACO.PitchS1p + this->CJACO.PitchS1p_0; //Right Speed - PitchS1p's real value < PitchS1_0
				 this->CJACO.PitchS1_max = this->hScrollBar1->Maximum;
				 this->textBox9->Text = Convert::ToString(CJACO.PitchS1_max);		//PitchS1p
				 this->textBox10->Text = Convert::ToString(CJACO.PitchS1m_0);		//PitchS1m
				 CJACO.Config_RL = CJACO.Config_RL + 1;
	}
	private: System::Void button21_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on min, hScrollBar7-8
				 this->hScrollBar8->Maximum = this->CJACO.RollS1m + this->CJACO.RollS1m_0; //Forward - RollS1m's real value < RollS1_0 
				 this->CJACO.RollS1_min = this->hScrollBar8->Maximum;
				 this->textBox6->Text = Convert::ToString(CJACO.RollS1p_0);		//RollS1p
				 this->textBox13->Text = Convert::ToString(CJACO.RollS1_min);	//RollS1m
				 CJACO.Config_FB = CJACO.Config_FB + 1;
	}
	private: System::Void button22_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on max, hScrollBar7-8
				 this->hScrollBar7->Maximum = this->CJACO.RollS1p + this->CJACO.RollS1p_0; //Backward - RollS1p's real value < RollS1_0
				 this->CJACO.RollS1_max = this->hScrollBar7->Maximum;
				 this->textBox6->Text = Convert::ToString(CJACO.RollS1_max);	//RollS1p
				 this->textBox13->Text = Convert::ToString(CJACO.RollS1m_0);		//RollS1m
				 CJACO.Config_FB = CJACO.Config_FB + 1;
	}
	private: System::Void button14_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on min, hScrollBar3
				 this->hScrollBar3->Minimum = this->hScrollBar3->Value;
				 this->CJACO.PitchS2_0 = this->hScrollBar3->Value;
				 this->textBox11->Text = Convert::ToString(CJACO.PitchS2_0);
				 CJACO.Config_U = CJACO.Config_U + 1;
	}
	private: System::Void button13_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on max, hScrollBar3
				 this->hScrollBar3->Maximum = this->hScrollBar3->Value;
				 this->CJACO.PitchS2_max = this->hScrollBar3->Value;
				 this->textBox11->Text = Convert::ToString(CJACO.PitchS2_max);
				 CJACO.Config_U = CJACO.Config_U + 1;
	}
	private: System::Void button16_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on min, hScrollBar4
				 this->hScrollBar4->Minimum = this->hScrollBar4->Value;
				 this->CJACO.PitchS3_0 = this->hScrollBar4->Value;
				 this->textBox12->Text = Convert::ToString(CJACO.PitchS3_0);
				 CJACO.Config_D = CJACO.Config_D + 1;
	}
	private: System::Void button15_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 //Press on max, hScrollBar4
				 this->hScrollBar4->Maximum = this->hScrollBar4->Value;
				 this->CJACO.PitchS3_max = this->hScrollBar4->Value;
				 this->textBox12->Text = Convert::ToString(CJACO.PitchS3_max);
				 CJACO.Config_D = CJACO.Config_D + 1;
	}
		//FIN CALIBRAGE ANGLES****************************************************************************************	
	private: System::Void pictureBox5_Click(System::Object^  sender, System::EventArgs^  e) 
	{
				 JoystickCommand CMD;
				 QuickStatus STATE;
				 CMD.InitStruct();
				 CMD.ButtonValue[XY_BUTTON2] = 1;
				 (*MySendJoystickCommand)(CMD);
				 this->pictureBox5->BackColor = System::Drawing::SystemColors::Highlight;
				 Sleep(200);
				 CMD.ButtonValue[XY_BUTTON2] = 0;
				 (*MySendJoystickCommand)(CMD);
				 this->pictureBox5->BackColor = System::Drawing::SystemColors::Control;
				 //STATUS READING
				 //(*MyGetQuickStatus)(STATE);
	}
	private: System::Void pictureBox6_Click(System::Object^  sender, System::EventArgs^  e) 
	{
				 JoystickCommand CMD;
				 QuickStatus STATE;
				 CMD.InitStruct();
				 CMD.ButtonValue[XY_BUTTON3] = 1;
				 (*MySendJoystickCommand)(CMD);
				 this->pictureBox6->BackColor = System::Drawing::SystemColors::Highlight;
				 Sleep(200);
				 CMD.ButtonValue[XY_BUTTON3] = 0;
				 (*MySendJoystickCommand)(CMD);
				 this->pictureBox6->BackColor = System::Drawing::SystemColors::Control;
				 //STATUS READING
				 //(*MyGetQuickStatus)(STATE);
	}
	private: System::Void checkBox4_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
				 this->CJACO.PitchS1_active = (bool) this->checkBox4->Checked;
	}
	private: System::Void checkBox5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
				 this->CJACO.RollS1_active = (bool) this->checkBox5->Checked;
	}
	private: System::Void checkBox6_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
				 this->CJACO.PitchS2_active = (bool) this->checkBox6->Checked;
	}
	private: System::Void checkBox7_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
				 this->CJACO.PitchS3_active = (bool) this->checkBox7->Checked;
	}
		//TEST BUTTON
	private: System::Void button23_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		JoystickCommand CMD_Test;
		CMD_Test.InitStruct();
		var_test++;
		if (var_test == 1)
			CMD_Test.InclineForwardBackward = 1;
		else if (var_test == 2)
			CMD_Test.InclineForwardBackward = -1;
		else if (var_test == 3)
		{
			CMD_Test.InclineForwardBackward = 0;
			var_test = 0;
		}
		(*MySendJoystickCommand)(CMD_Test);
	}
	private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		QuickStatus STATE_TEST;
		//STATUS READING
		(*MyGetQuickStatus)(STATE_TEST);
		//this->label14->Text = Convert::ToString(STATE_TEST.ControlActiveModule);
	}
};
}
