#pragma once

#include"drive.h"

namespace Drive {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text::RegularExpressions;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  reloadButton;
	private: System::Windows::Forms::ComboBox^  driveCBox;


	private: System::Windows::Forms::ComboBox^  volumeCBox;
	private: System::Windows::Forms::TextBox^  pathTBox;
	private: System::Windows::Forms::DataGridView^  itemGView;
	private: System::Windows::Forms::TextBox^  infoTBox;
	private: System::Windows::Forms::Button^  upButton;
	private: System::Windows::Forms::Label^  searchIcon;


	private: System::Windows::Forms::TextBox^  searchTBox;
	private: System::Windows::Forms::Label^  searchErrorLabel;
	private: System::Windows::Forms::StatusStrip^  statusStrip;
	private: System::Windows::Forms::ToolStripStatusLabel^  statusLabel;




	private: System::ComponentModel::IContainer^  components;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->reloadButton = (gcnew System::Windows::Forms::Button());
			this->driveCBox = (gcnew System::Windows::Forms::ComboBox());
			this->volumeCBox = (gcnew System::Windows::Forms::ComboBox());
			this->pathTBox = (gcnew System::Windows::Forms::TextBox());
			this->itemGView = (gcnew System::Windows::Forms::DataGridView());
			this->infoTBox = (gcnew System::Windows::Forms::TextBox());
			this->upButton = (gcnew System::Windows::Forms::Button());
			this->searchIcon = (gcnew System::Windows::Forms::Label());
			this->searchTBox = (gcnew System::Windows::Forms::TextBox());
			this->searchErrorLabel = (gcnew System::Windows::Forms::Label());
			this->statusStrip = (gcnew System::Windows::Forms::StatusStrip());
			this->statusLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->itemGView))->BeginInit();
			this->statusStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// reloadButton
			// 
			this->reloadButton->Font = (gcnew System::Drawing::Font(L"Courier New", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->reloadButton->Location = System::Drawing::Point(5, 4);
			this->reloadButton->Name = L"reloadButton";
			this->reloadButton->Size = System::Drawing::Size(60, 25);
			this->reloadButton->TabIndex = 0;
			this->reloadButton->Text = L"↻";
			this->reloadButton->UseVisualStyleBackColor = true;
			this->reloadButton->Click += gcnew System::EventHandler(this, &MainForm::loadDrive);
			// 
			// driveCBox
			// 
			this->driveCBox->DropDownHeight = 110;
			this->driveCBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->driveCBox->FormattingEnabled = true;
			this->driveCBox->IntegralHeight = false;
			this->driveCBox->Location = System::Drawing::Point(135, 5);
			this->driveCBox->Name = L"driveCBox";
			this->driveCBox->Size = System::Drawing::Size(155, 23);
			this->driveCBox->TabIndex = 3;
			this->driveCBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::driveCBox_SelectAction);
			// 
			// volumeCBox
			// 
			this->volumeCBox->DropDownHeight = 110;
			this->volumeCBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->volumeCBox->FormattingEnabled = true;
			this->volumeCBox->IntegralHeight = false;
			this->volumeCBox->Location = System::Drawing::Point(296, 5);
			this->volumeCBox->Name = L"volumeCBox";
			this->volumeCBox->Size = System::Drawing::Size(83, 23);
			this->volumeCBox->TabIndex = 4;
			// 
			// pathTBox
			// 
			this->pathTBox->Location = System::Drawing::Point(385, 6);
			this->pathTBox->Name = L"pathTBox";
			this->pathTBox->ReadOnly = true;
			this->pathTBox->Size = System::Drawing::Size(494, 21);
			this->pathTBox->TabIndex = 5;
			// 
			// itemGView
			// 
			this->itemGView->AllowUserToAddRows = false;
			this->itemGView->AllowUserToDeleteRows = false;
			this->itemGView->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->itemGView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->itemGView->Location = System::Drawing::Point(5, 31);
			this->itemGView->MultiSelect = false;
			this->itemGView->Name = L"itemGView";
			this->itemGView->ReadOnly = true;
			this->itemGView->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			this->itemGView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->itemGView->Size = System::Drawing::Size(513, 394);
			this->itemGView->TabIndex = 6;
			// 
			// infoTBox
			// 
			this->infoTBox->CausesValidation = false;
			this->infoTBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.5F));
			this->infoTBox->Location = System::Drawing::Point(524, 56);
			this->infoTBox->Multiline = true;
			this->infoTBox->Name = L"infoTBox";
			this->infoTBox->ReadOnly = true;
			this->infoTBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->infoTBox->Size = System::Drawing::Size(355, 369);
			this->infoTBox->TabIndex = 10;
			// 
			// upButton
			// 
			this->upButton->Location = System::Drawing::Point(70, 4);
			this->upButton->Name = L"upButton";
			this->upButton->Size = System::Drawing::Size(60, 25);
			this->upButton->TabIndex = 1;
			this->upButton->Text = L"▲";
			this->upButton->UseVisualStyleBackColor = true;
			this->upButton->Click += gcnew System::EventHandler(this, &MainForm::levelUP);
			// 
			// searchIcon
			// 
			this->searchIcon->AutoSize = true;
			this->searchIcon->Font = (gcnew System::Drawing::Font(L"Courier New", 15));
			this->searchIcon->Location = System::Drawing::Point(520, 31);
			this->searchIcon->Name = L"searchIcon";
			this->searchIcon->Size = System::Drawing::Size(25, 22);
			this->searchIcon->TabIndex = 7;
			this->searchIcon->Text = L"🔍";
			// 
			// searchTBox
			// 
			this->searchTBox->Location = System::Drawing::Point(554, 31);
			this->searchTBox->Name = L"searchTBox";
			this->searchTBox->Size = System::Drawing::Size(325, 21);
			this->searchTBox->TabIndex = 8;
			this->searchTBox->TextChanged += gcnew System::EventHandler(this, &MainForm::itemsSearch);
			// 
			// searchErrorLabel
			// 
			this->searchErrorLabel->AutoSize = true;
			this->searchErrorLabel->BackColor = System::Drawing::Color::Transparent;
			this->searchErrorLabel->Font = (gcnew System::Drawing::Font(L"Courier New", 10));
			this->searchErrorLabel->ForeColor = System::Drawing::Color::Red;
			this->searchErrorLabel->Location = System::Drawing::Point(536, 28);
			this->searchErrorLabel->Margin = System::Windows::Forms::Padding(0);
			this->searchErrorLabel->Name = L"searchErrorLabel";
			this->searchErrorLabel->Size = System::Drawing::Size(16, 17);
			this->searchErrorLabel->TabIndex = 9;
			this->searchErrorLabel->Text = L"✘";
			this->searchErrorLabel->Visible = false;
			// 
			// statusStrip
			// 
			this->statusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->statusLabel });
			this->statusStrip->Location = System::Drawing::Point(0, 429);
			this->statusStrip->Name = L"statusStrip";
			this->statusStrip->Size = System::Drawing::Size(884, 22);
			this->statusStrip->TabIndex = 11;
			// 
			// statusLabel
			// 
			this->statusLabel->Name = L"statusLabel";
			this->statusLabel->Size = System::Drawing::Size(56, 17);
			this->statusLabel->Text = L"infoLabel";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(884, 451);
			this->Controls->Add(this->statusStrip);
			this->Controls->Add(this->searchErrorLabel);
			this->Controls->Add(this->searchTBox);
			this->Controls->Add(this->searchIcon);
			this->Controls->Add(this->upButton);
			this->Controls->Add(this->infoTBox);
			this->Controls->Add(this->itemGView);
			this->Controls->Add(this->pathTBox);
			this->Controls->Add(this->volumeCBox);
			this->Controls->Add(this->driveCBox);
			this->Controls->Add(this->reloadButton);
			this->Font = (gcnew System::Drawing::Font(L"Courier New", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->MaximumSize = System::Drawing::Size(900, 490);
			this->MinimumSize = System::Drawing::Size(900, 490);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->Load += gcnew System::EventHandler(this, &MainForm::loadDrive);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->itemGView))->EndInit();
			this->statusStrip->ResumeLayout(false);
			this->statusStrip->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		// ---------------------------------------------------------------------------------------------------------------------

		// Drives list
		Generic::List<DriveAccess ^> ^drives = nullptr;

		// Current drive
		DriveAccess ^cDrive = nullptr;

		// Current volume 
		volume::part ^cVolume = nullptr;

		// FS elements
		Generic::List<filesystem::object ^> ^cFSObjects = nullptr;

		// ---------------------------------------------------------------------------------------------------------------------

		// Set drives list
		void driveCBox_set(Generic::List<DriveAccess ^> ^Drives) {

			this->driveCBox->Items->Clear();
			this->driveCBox->Items->Add(L"");
			this->volumeCBox_set(nullptr);
			if (Drives != nullptr) {
				this->driveCBox->Items->AddRange(Drives->ToArray());
				this->driveCBox->SelectedIndex = 0;
			}
		}

		// Get select drive
		DriveAccess ^driveCBox_selected() {

			System::Object ^object = this->driveCBox->SelectedItem;
			if (object != nullptr && DriveAccess::typeid->IsAssignableFrom(object->GetType())) {
				return (DriveAccess ^)object;
			}
			return nullptr;
		}

		// Set select drive
		bool driveCBox_select(String ^Address) {

			volumeCBox_set(nullptr);
			if (Address == nullptr) {
				this->driveCBox->SelectedIndex = 0;
				return true;
			}
			for (size_t i = 1; i < this->driveCBox->Items->Count; i++) {
				if (DriveAccess::typeid->IsAssignableFrom(this->driveCBox->Items[i]->GetType())
					&& Address == ((DriveAccess ^)this->driveCBox->Items[i])->Address) {
					this->driveCBox->SelectedIndex = i;
					return true;
				}
			}
			return false;
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Set drive volumes list
		void volumeCBox_set(Generic::List<volume::part ^> ^Volumes) {

			this->volumeCBox->Items->Clear();
			this->volumeCBox->Items->Add(L"");
			this->pathTBox_set(nullptr);
			if (Volumes != nullptr) {
				this->volumeCBox->Items->AddRange(Volumes->ToArray());
			}
			this->volumeCBox->SelectedIndex = 0;
		}

		// Get select volume
		volume::part ^volumeCBox_selected() {

			System::Object ^object = this->volumeCBox->SelectedItem;
			if (volume::part::typeid->IsAssignableFrom(object->GetType())) {
				return (volume::part ^)object;
			}
			return nullptr;
		}

		// Set select volume
		bool volumeCBox_select(String ^Name) {

			this->pathTBox_set(nullptr);
			if (Name == nullptr) {
				this->volumeCBox->SelectedIndex = 0;
				return true;
			}
			for (size_t i = 1; i < this->volumeCBox->Items->Count; i++) {
				if (volume::part::typeid->IsAssignableFrom(this->volumeCBox->Items[i]->GetType())
					&& Name == ((volume::part ^)this->volumeCBox->Items[i])->Name) {
					this->volumeCBox->SelectedIndex = i;
					return true;
				}
			}
			return false;
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Set path string
		void pathTBox_set(String ^Path) {
			this->pathTBox->Text = Path ? Path : L"";
		}

		// Get path string
		String ^pathTBox_get() {
			return this->pathTBox->Text;
		}

		// Path level up
		bool pathTBox_up() {
			String ^path = this->pathTBox->Text;
			if (path != L"" && path != L"\\") {
				size_t newAddress = path->Remove(path->Length - 1)->LastIndexOf(L'\\');
				this->pathTBox->Text = !newAddress ? L"\\" : path->Remove(newAddress + 1);
				return true;
			}
			else if (this->cVolume) {
				this->volumeCBox_select(nullptr);
				return true;
			}
			return false;
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Set status label text
		void statusLabelSet(String ^Label) {
			this->statusLabel->Text = Label ? Label : L"";
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Show info to info text box
		void infoTBox_showInfo(DriveAccess ^ drive, ::volume::part ^ volume) {

			this->infoTBox->Text = L">>> Search " + this->drives->Count + L" drive(s).";

			if (drive) {

				this->infoTBox->Text += L"\r\n\r\n>> Drive info";

				this->infoTBox->Text += L"\r\n";

				this->infoTBox->Text += L"\r\n Drive Address   : " + drive->Address;
				this->infoTBox->Text += L"\r\n Sector size     : " + getFormatSize(drive->BytesPerSector);
				this->infoTBox->Text += L"\r\n Number sectors  : " + drive->NumberOfSectors;
				this->infoTBox->Text += L"\r\n Full drive size : " + getFormatSize(drive->NumberOfBytes);
				this->infoTBox->Text += L"\r\n Volumes count   : " + drive->Volumes->Count;

				Generic::List<String ^> ^errors = drive->Errors;
				if (errors->Count) {
					this->infoTBox->Text += L"\r\n\r\n>> Warnings";
					for each (String ^error in errors) {
						this->infoTBox->Text += L"\r\n " + error;
					}
				}
			}

			if (volume) {

				this->infoTBox->Text += L"\r\n\r\n>> Volume info";

				this->infoTBox->Text += L"\r\n";

				this->infoTBox->Text += L"\r\n Volume Name : " + volume->Name;
				this->infoTBox->Text += L"\r\n Full size   : " + getFormatSize(volume->NumberOfBytes);

				QWORD free = volume->NumberOfFreeBytes;

				this->infoTBox->Text += L"\r\n Free size   : " + (free ? getFormatSize(free) : L"-");

				this->infoTBox->Text += L"\r\n Begin LBA   : " + String::Format(L"0x{0:X8}", volume->BegLBA);
				this->infoTBox->Text += L"\r\n End LBA     : " + String::Format(L"0x{0:X8}", volume->EndLBA);

				this->infoTBox->Text += L"\r\n Filesystem  : " + volume->FSNameAuto;

				this->infoTBox->Text += L"\r\n";

				Type ^type = volume->GetType();

				if (type == ::volume::part_dbr::typeid) {

					::volume::part_dbr ^dbr_volume = (::volume::part_dbr ^)volume;

					this->infoTBox->Text += L"\r\n Volume type  : DBR";

					this->infoTBox->Text += L"\r\n Part record  : " + (dbr_volume->IsPrime ? L"primary" : L"extend");
					this->infoTBox->Text += L"\r\n Boot type    : " + (dbr_volume->IsBoot ? L"bootable" : L"no boot");
					this->infoTBox->Text += L"\r\n Part type    : " + String::Format(L"0x{0:X2}", dbr_volume->OSType);
					this->infoTBox->Text += L"\r\n Visibility   : " + (dbr_volume->IsHide ? L"hidden" : L"visible");
					this->infoTBox->Text += L"\r\n FS record    : " + dbr_volume->FSNameRec;
				}

				else if (type == ::volume::part_gpt::typeid) {

					::volume::part_gpt ^gpt_volume = (::volume::part_gpt ^)volume;

					this->infoTBox->Text += L"\r\n Volume type  : GPT";
					this->infoTBox->Text += L"\r\n Drive : " + gpt_volume->DriveGUID.ToString();
					this->infoTBox->Text += L"\r\n GUID  : " + gpt_volume->PartGUID.ToString();
					this->infoTBox->Text += L"\r\n Type  : " + gpt_volume->TypeGUID.ToString() +
						L"\r\n         " + gpt_volume->Descript;
				}

				else {

					this->infoTBox->Text += L"\r\n Volume type  : Unknown[" + type + L"]";

				}
			}
		}

		// ---------------------------------------------------------------------------------------------------------------------

		System::EventHandler ^ itemGView_handler_select = nullptr;
		System::Windows::Forms::DataGridViewCellEventHandler ^ itemGView_handler_choose = nullptr;

		String ^searchColume = nullptr;

		// ---------------------------------------------------------------------------------------------------------------------

		// Display drives and set actions
		void itemGView_displayDrives(Generic::List<DriveAccess ^> ^ drives) {

			this->cDrive = nullptr;

			this->volumeCBox_set(nullptr);

			System::Windows::Forms::DataGridViewTextBoxColumn ^ address, ^ size, ^ sectors, ^ bytesInSector;

			address = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			address->HeaderText = L"Address";
			address->Name = L"Address";
			address->ReadOnly = true;
			address->Width = 160;

			size = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			size->HeaderText = L"Size";
			size->Name = L"Size";
			size->ReadOnly = true;
			size->SortMode = DataGridViewColumnSortMode::NotSortable;
			size->Width = 100;

			sectors = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			sectors->HeaderText = L"Sectors";
			sectors->Name = L"Sectors";
			sectors->ReadOnly = true;
			sectors->Width = 100;

			bytesInSector = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			bytesInSector->HeaderText = L"Sector size";
			bytesInSector->Name = L"SectorSize";
			bytesInSector->ReadOnly = true;
			bytesInSector->Width = 110;

			this->itemGView->SelectionChanged -= this->itemGView_handler_select;
			this->itemGView->CellContentDoubleClick -= this->itemGView_handler_choose;

			this->itemGView->Rows->Clear();
			this->itemGView->Columns->Clear();

			this->itemGView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^ >(4) { address, size, sectors, bytesInSector });

			this->searchColume = L"Address";
			
			this->itemGView_handler_select = gcnew System::EventHandler(this, &MainForm::itemGView_selectDrive);			
			this->itemGView_handler_choose = gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::itemGView_chooseDrive);
			
			this->itemGView->SelectionChanged += this->itemGView_handler_select;
			this->itemGView->CellContentDoubleClick += this->itemGView_handler_choose;

			for each (auto drive in drives) {
				array<String ^> ^ driveItem = { drive->Address,  getFormatSize(drive->NumberOfBytes), Convert::ToString(drive->NumberOfSectors), getFormatSize(drive->BytesPerSector) };
				this->itemGView->Rows->Add(driveItem);
			}

			this->infoTBox_showInfo(drives->Count ? drives[0] : nullptr, nullptr);
		}

		// Action : Double item click - Select drive
		System::Void itemGView_selectDrive(System::Object ^ sender, System::EventArgs ^ e) {

			if (this->itemGView->Rows->Count) {
				String ^address = (String ^)this->itemGView->SelectedRows[0]->Cells[0]->Value;
				if (address != nullptr && address != L"") {
					auto drives = this->drives;
					for each (auto drive in drives) {
						if (drive->Address == address) {
							this->infoTBox_showInfo(drive, nullptr);
							break;
						}
					}
				}
			}
		}

		// Action : Double item click - Choose drive
		System::Void itemGView_chooseDrive(System::Object ^ sender, System::Windows::Forms::DataGridViewCellEventArgs ^ e) {

			if (this->itemGView->Rows->Count) {
				String ^address = (String ^)this->itemGView->SelectedRows[0]->Cells[L"Address"]->Value;
				driveCBox_select(address);
			}
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Display volumes and set actions
		void itemGView_displayVolumes(DriveAccess ^ drive) {

			this->cVolume = nullptr;

			this->volumeCBox_set(drive->Volumes);

			System::Windows::Forms::DataGridViewTextBoxColumn ^ name, ^ size, ^ beg, ^ end, ^ sectors;

			name = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());

			name->HeaderText = L"Name";
			name->Name = L"Name";
			name->ReadOnly = true;
			name->Width = 80;

			size = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());

			size->HeaderText = L"Size";
			size->Name = L"Size";
			size->ReadOnly = true;
			size->SortMode = DataGridViewColumnSortMode::NotSortable;
			size->Width = 85;

			beg = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());

			beg->HeaderText = L"Begin LBA";
			beg->Name = L"BegLBA";
			beg->ReadOnly = true;
			beg->Width = 100;

			end = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());

			end->HeaderText = L"End LBA";
			end->Name = L"EndLBA";
			end->ReadOnly = true;
			end->Width = 100;

			sectors = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());

			sectors->HeaderText = L"Sectors";
			sectors->Name = L"Sectors";
			sectors->ReadOnly = true;
			sectors->Width = 100;

			this->itemGView->SelectionChanged -= this->itemGView_handler_select;
			this->itemGView->CellContentDoubleClick -= this->itemGView_handler_choose;

			this->itemGView->Rows->Clear();
			this->itemGView->Columns->Clear();

			this->itemGView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^ >(5) { name, size, beg, end, sectors });

			this->searchColume = L"Name";

			this->itemGView_handler_select = gcnew System::EventHandler(this, &MainForm::itemGView_selectVolume);
			this->itemGView_handler_choose = gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::itemGView_chooseVolume);
			
			this->itemGView->SelectionChanged += this->itemGView_handler_select;
			this->itemGView->CellContentDoubleClick += this->itemGView_handler_choose;

			auto volumes = drive->Volumes;

			for each (auto volume in volumes) {

				array<String ^> ^ driveItem = { volume->Name, getFormatSize(volume->NumberOfBytes), String::Format(L"0x{0:X8}", volume->BegLBA), String::Format(L"0x{0:X8}", volume->EndLBA), Convert::ToString(volume->NumberOfSectors) };
				this->itemGView->Rows->Add(driveItem);
			}

		

			this->infoTBox_showInfo(this->cDrive, volumes->Count ? volumes[0] : nullptr);
		}

		// Action : Double item click - Select volume
		System::Void itemGView_selectVolume(System::Object ^ sender, System::EventArgs ^ e) {

			if (this->cDrive && this->itemGView->SelectedRows->Count) {
				String ^name = (String ^)this->itemGView->SelectedRows[0]->Cells[L"Name"]->Value;
				if (name != nullptr && name != L"") {
					auto volumes = this->cDrive->Volumes;
					for each (auto volume in volumes) {
						if (volume->Name == name) {
							this->infoTBox_showInfo(this->cDrive, volume);
							break;
						}
					}
				}
			}
		}

		// Action : Double item click - Choose volume
		System::Void itemGView_chooseVolume(System::Object ^ sender, System::Windows::Forms::DataGridViewCellEventArgs ^ e) {

			if (this->cDrive && this->itemGView->Rows->Count) {
				String ^name = (String ^)this->itemGView->SelectedRows[0]->Cells[L"Name"]->Value;
				volumeCBox_select(name);
			}
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Display path records and set actions
		void itemGView_displayPath(volume::part ^ volume, String ^ path) {
			
			try {

				this->cFSObjects = volume->getDirItems(path);
				
				this->itemGView->Rows->Clear();
				this->itemGView->Columns->Clear();

				System::Windows::Forms::DataGridViewTextBoxColumn ^ icon, ^ name, ^ size;

				icon = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());

				icon->HeaderText = L"";
				icon->Name = L"Icon";
				icon->ReadOnly = true;
				icon->Width = 22;
				icon->Resizable = DataGridViewTriState::False;

				name = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());

				name->HeaderText = L"Name";
				name->Name = L"Name";
				name->ReadOnly = true;
				name->Width = 270;

				size = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());

				size->HeaderText = L"Size";
				size->Name = L"Size";
				size->ReadOnly = true;
				size->SortMode = DataGridViewColumnSortMode::NotSortable;
				size->Width = 100;

				this->itemGView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^ >(3) { icon, name, size });

				this->searchColume = L"Name";

				this->itemGView->SelectionChanged -= this->itemGView_handler_select;
				this->itemGView->CellContentDoubleClick -= this->itemGView_handler_choose;

				this->itemGView_handler_select = nullptr;
				this->itemGView_handler_choose = gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::itemGView_choosePath);

				this->itemGView->SelectionChanged += this->itemGView_handler_select;
				this->itemGView->CellContentDoubleClick += this->itemGView_handler_choose;
				
				// 📄  📂

				for each (filesystem::object ^ object in cFSObjects) {
					array<String ^> ^driveItem = { object->attr & FILE_ATTRIBUTE_DIRECTORY ? L"📂" : L"📄", object->name, object->size ? getFormatSize(object->size) : L"-" };
					this->itemGView->Rows->Add(driveItem);
				}

				this->infoTBox_showInfo(this->cVolume->Drive, this->cVolume);
				statusLabelSet(L"Volume " + this->cVolume->Name);
			}
			catch (NotSupportedException ^ex) {
				this->volumeCBox_select(nullptr);
				statusLabelSet(gcnew String(ex->Message));
			}
			catch (Exception ^ex) {
				MessageBox::Show(gcnew String(ex->Message));
				this->pathTBox_up();
			}

		}

		// System::Void itemGView_selectPath(System::Object ^ sender, System::EventArgs ^  e) {}

		// Action : Double item click - Choose path
		System::Void itemGView_choosePath(System::Object ^ sender, System::Windows::Forms::DataGridViewCellEventArgs ^ e) {

			statusLabelSet(nullptr);
			if (this->cDrive && this->cVolume && this->itemGView->Rows->Count) {
				auto select = this->itemGView->SelectedRows[0];
				if (select->Cells[L"Icon"]->Value->Equals(L"📂")) {
					String ^name = (String ^)select->Cells[L"Name"]->Value;
					this->pathTBox_set(this->pathTBox_get() + name + L"\\");
				}
			}
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Action : Load form - Get drives
		System::Void loadDrive(System::Object ^ sender, System::EventArgs ^ e) {

			statusLabelSet(nullptr);

			this->Enabled = false;
			this->drives = DriveAccess::OpenAll();
			this->driveCBox_set(this->drives);
			this->searchTBox->Text = "";
			this->Enabled = true;
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Action : Click UP button - Up level
		System::Void levelUP(System::Object ^ sender, System::EventArgs ^ e) {

			if (!this->pathTBox_up()) {
				if (this->cDrive) {
					this->driveCBox_select(nullptr);
				}
			}
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Action : Change searche value - Searche in items
		System::Void itemsSearch(System::Object ^ sender, System::EventArgs ^ e) {

			statusLabelSet(nullptr);

			this->searchErrorLabel->Visible = false;
			try {
				infoTBox->Text = L"";
				if (this->searchTBox->Text != L"") {
					Regex ^ search = gcnew Regex(this->searchTBox->Text);
					if (this->searchColume != nullptr) {
						for (size_t i = 0; i < this->itemGView->Rows->Count; i++) {
							Object ^ object = this->itemGView->Rows[i]->Cells[this->searchColume]->Value;
							String ^ value = (String::typeid->IsAssignableFrom(object->GetType())) ? (String ^)object : object->ToString();
							//this->itemGView->Rows[i]->Visible = search->IsMatch(value);
							if (search->IsMatch(value)) {
								infoTBox->Text += L"> " + value + L"\r\n";
							}
						}
					}
				}
				else this->infoTBox_showInfo(this->cDrive, this->cVolume);
			}
			catch (const ArgumentException ^) {
				this->searchErrorLabel->Visible = true;
				statusLabelSet(L"Invalid expression format!");
			}

		}

		// ---------------------------------------------------------------------------------------------------------------------

		System::EventHandler ^volumeSelect = gcnew System::EventHandler(this, &MainForm::volumeCBox_SelectAction);

		// Action : Select drive - Display volumes
		System::Void driveCBox_SelectAction(System::Object ^ sender, System::EventArgs ^ e) {

			statusLabelSet(nullptr);

			this->volumeCBox->SelectedIndexChanged -= this->volumeSelect;

			this->cDrive = this->driveCBox_selected();
			if (this->cDrive != nullptr) {
				statusLabelSet(L"Drive " + this->cDrive->Address);
				this->itemGView_displayVolumes(this->cDrive);
				this->volumeCBox->SelectedIndexChanged += this->volumeSelect;
			}
			else {
				this->itemGView_displayDrives(this->drives);
			}
		}

		// ---------------------------------------------------------------------------------------------------------------------

		System::EventHandler ^pathChange = gcnew System::EventHandler(this, &MainForm::pathTBox_ChangePath);

		// Action : Select volume - Display root items
		System::Void volumeCBox_SelectAction(System::Object ^ sender, System::EventArgs ^ e) {

			statusLabelSet(nullptr);

			this->pathTBox->TextChanged -= pathChange;
			pathTBox_set(L"");

			this->cVolume = this->volumeCBox_selected();

			if (this->cVolume != nullptr) {
				statusLabelSet(L"Drive " + this->cDrive->Address);
				this->pathTBox->TextChanged += pathChange;
				pathTBox_set(L"\\");
			}
			else this->driveCBox_SelectAction(sender, e);
		}

		// ---------------------------------------------------------------------------------------------------------------------

		// Action : Change Label Value - Display path items
		System::Void pathTBox_ChangePath(System::Object ^ sender, System::EventArgs ^ e) {

			this->cFSObjects = nullptr;

			String ^path = this->pathTBox->Text;

			if (path != nullptr) {
				this->itemGView_displayPath(this->cVolume, path);
			}
		}

		// ---------------------------------------------------------------------------------------------------------------------

	};
}
