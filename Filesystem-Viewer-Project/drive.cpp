#include"drive.h"

// ---------------------------------------------------------------------------------------------------------------------

#include<iomanip>
using std::fixed;
using std::setw;
using std::hex;

#include<stack>
using std::stack;

#include<vcclr.h> 

// ---------------------------------------------------------------------------------------------------------------------

using namespace System;
using namespace System::Collections;
using namespace System::Data;

// ---------------------------------------------------------------------------------------------------------------------

WCHAR getSysLetter(DWORD Serial);
WCHAR getSysLetter(_guid GUID);

QWORD getPartNumOfFreeByte(WCHAR Letter);
QWORD getFAT32NumOfFreeByte(volume::part ^Volume);

Generic::List<filesystem::object^> ^getDirItems(String ^ Path);
Generic::List<filesystem::object^> ^getFAT32DirIrems(volume::part ^Volume, String ^Path);

// ---------------------------------------------------------------------------------------------------------------------

DriveAccess ^DriveAccess::Open(size_t DriveID) {
	try {
		return gcnew DriveAccess(DriveID);
	} catch (const System::Exception ^) {
		return nullptr;
	}
}

Generic::List<DriveAccess^>^ DriveAccess::OpenAll() {
	Generic::List<DriveAccess^>^ drives = gcnew Generic::List<DriveAccess^>;
	for (size_t i = 0; i < DriveAccess::MaxNumber; i++) {
		DriveAccess ^drive = DriveAccess::Open(i);
		if (drive) {
			drives->Add(drive);
		}
	}
	return drives;
}

// ---------------------------------------------------------------------------------------------------------------------

String ^ DriveAccess::ToString() {
	return this->Address;
}

// ---------------------------------------------------------------------------------------------------------------------

bool DriveAccess::ReadSectors(QWORD LBAPos, LPVOID Buffer, DWORD ToRead, LPDWORD Read) {
	HANDLE handle;
	bool result = this->ReadHandle(handle)
		&& this->ReadSectors(handle, LBAPos, Buffer, ToRead, Read);
	CloseHandle(handle);
	return result;
}

// ---------------------------------------------------------------------------------------------------------------------

DriveAccess::DriveAccess(size_t DriveID) :
	driveID(DriveID), address(L"\\\\.\\PhysicalDrive" + DriveID) {

	HANDLE handle;

	// Reading Drive geometry
	DISK_GEOMETRY geometry;
	if (!this->ReadHandle(handle) ||
		!DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &geometry, sizeof(DISK_GEOMETRY), NULL, NULL)) {
		CloseHandle(handle);
		throw gcnew Exception();
	}
	this->bytesPerSector = geometry.BytesPerSector;
	this->numberOfSectors = geometry.Cylinders.QuadPart * geometry.TracksPerCylinder * geometry.SectorsPerTrack;

	// Reading Drive volumes
	this->ReadVolumes(handle, 0x00);

	CloseHandle(handle);
}

// ---------------------------------------------------------------------------------------------------------------------

bool DriveAccess::ReadHandle(HANDLE &Handle) {
	pin_ptr<const wchar_t> address = PtrToStringChars(this->address);
	Handle = CreateFile(address, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return Handle != INVALID_HANDLE_VALUE;
}

// ---------------------------------------------------------------------------------------------------------------------

bool DriveAccess::ReadSectors(HANDLE &Handle, QWORD LBAPos, LPVOID Buffer, DWORD ToRead, LPDWORD Read) {
	LBAPos *= this->bytesPerSector;
	PLONG overflow = reinterpret_cast<PLONG>(&LBAPos) + 1;
	return SetFilePointer(Handle, (LONG)LBAPos, overflow, FILE_BEGIN) != INVALID_SET_FILE_POINTER
		&& ReadFile(Handle, Buffer, ToRead * this->bytesPerSector, Read, NULL);
}

// ---------------------------------------------------------------------------------------------------------------------

typedef struct dbr_view {
	QWORD shift;
	DWORD index;
	::dbr::note note;
} dbr_view;

// ---------------------------------------------------------------------------------------------------------------------

void DriveAccess::ReadVolumes(HANDLE &Handle, QWORD LBAPos) {

	::dbr::note mbr;

	if (!this->ReadSectors(Handle, LBAPos, &mbr, 1, NULL)) {
		this->errors->Add(L"MBR element Read error!");
	}

	// MBR not found	
	else if (mbr.sign != 0xAA55) {
		this->errors->Add(L"MBR signature not found!");
	}

	// Protect MBR
	if (mbr.part[0].os_type == 0xEE) {

		::gpt::head gpt_head;

		if (!this->ReadSectors(Handle, LBAPos + mbr.part[0].beg_lba, &gpt_head, 1, NULL)) {
			this->errors->Add(L"GPT header Read error!");
		}

		else if (strcmp("EFI PART", gpt_head.head_sign)) {
			this->errors->Add(L"GPT header signature error!");
		}

		else if (0x00010000 != gpt_head.gpt_vers) {
			this->errors->Add(L"GPT version not supported!");
		}

		else if (sizeof(::gpt::head) != gpt_head.head_size) {
			this->errors->Add(L"GPT header Size invalid!");
		}

		else if (sizeof(::gpt::part) != gpt_head.part_size) {
			this->errors->Add(L"GPT NamePart Size not standard!");
		}
		
		else {

			::gpt::part *gpt_parts = new ::gpt::part[gpt_head.parts_num];

			DWORD sectors = sizeof(gpt_parts) / this->bytesPerSector + (sizeof(gpt_parts) % this->bytesPerSector ? 1 : 0);

			if (!this->ReadSectors(Handle, LBAPos + gpt_head.parts_lba, gpt_parts, sectors, NULL)) {
				this->errors->Add(L"GPT parts table Read error!");
			}

			else for (size_t i = 0; i < gpt_head.parts_num; i++) {

				if (gpt_parts[i].part_guid == _guid(0x024DEE41, 0x33E7, 0x11D3, 0x9D69, 0x0008C781F39F)) {
					this->ReadVolumes(Handle, gpt_parts[i].beg_lba);
				}

				else if (gpt_parts[i].part_guid) {
					this->volumes->Add(gcnew volume::part_gpt(this, this->volumes->Count, gpt_head.guid, gpt_parts[i]));
				}
			}

			delete[] gpt_parts;
		}
	}

	// Legacy MBR
	else {
		for (size_t i = 0; i < 4; i++) {

			const ::dbr::info *master_i = ::dbr::getPartInfo(mbr.part[i]);

			if (master_i == nullptr || master_i->is_part) {

				if (mbr.part[i].lba_sct > 0) {
					this->volumes->Add(gcnew volume::part_dbr(this, this->volumes->Count, mbr.part[i], LBAPos));
				}
			}

			// Partition is a container
			else {

				stack<dbr_view *> analysis_stack;

				QWORD shift = mbr.part[i].beg_lba;

				do {

					::dbr::note ebr;

					if (!this->ReadSectors(Handle, LBAPos + shift, &ebr, 1, NULL)) {
						this->errors->Add(L"EBR element Read error!");
					}

					else if (ebr.sign != 0xAA55) {
						this->errors->Add(L"EBR signature error!");
					}

					else {
						dbr_view * view = new dbr_view;
						view->shift = shift;
						view->index = 0;
						view->note = ebr;
						analysis_stack.push(view);
					}

					// EBR stack analysis
					while (!analysis_stack.empty()) {

						dbr_view * current = analysis_stack.top();

						while (current->index < 4) {

							const dbr::info *extend_i = dbr::getPartInfo(current->note.part[current->index]);

							if (extend_i != nullptr && !extend_i->is_part) {
								shift = (QWORD)mbr.part[i].beg_lba + current->note.part[current->index].beg_lba;
								current->index++;
								break;
							}

							if (current->note.part[current->index].lba_sct) {
								this->volumes->Add(gcnew volume::part_dbr(this, this->volumes->Count, current->note.part[current->index], LBAPos + shift));
							}

							current->index++;
						}

						if (current->index < 4) break;

						analysis_stack.pop();
						delete current;
					}

				} while (!analysis_stack.empty());
			}
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------

QWORD volume::part::getNumOfFreeBytes() {

	return this->fs_auto == L"FAT32"
		? getFAT32NumOfFreeByte(this)
		: this->letter != L'?'
		? getPartNumOfFreeByte(this->letter)
		: 0;
}

// ---------------------------------------------------------------------------------------------------------------------

volume::part::part(DriveAccess ^Drive, size_t PartID, QWORD BegLBA, QWORD EndLBA) :
	drive(Drive), beg_lba(BegLBA), end_lba(EndLBA) {
	
	this->name = String::Format(L"V#{0:X2}", PartID);

	filesystem::boot *boot = new filesystem::boot;

	this->ReadSectors(0, boot, 1, NULL);
	this->fs_auto = filesystem::fs_auto(boot);

	if ((gcnew String(L"NTFS"))->Equals(this->fs_auto)) {
		filesystem::ntfs::boot *ntfs_boot = (filesystem::ntfs::boot *)boot;
		this->letter = getSysLetter((DWORD)ntfs_boot->BPB_VolSerial);
	}
	else if ((gcnew String(L"exFAT"))->Equals(this->fs_auto)) {
		filesystem::exfat::boot *exfat_boot = (filesystem::exfat::boot *)boot;
		this->letter = getSysLetter(exfat_boot->BPB_VolSerial);
	}
	else if ((gcnew String(L"FAT32"))->Equals(this->fs_auto)) {
		filesystem::fat32::boot *fat32_boot = (filesystem::fat32::boot *)boot;
		this->letter = getSysLetter(fat32_boot->BPB_VolSerial);
	}
	else if ((gcnew String(L"FAT16"))->Equals(this->fs_auto)) {
		filesystem::fat16::boot *fat16_boot = (filesystem::fat16::boot *)boot;
		this->letter = getSysLetter(fat16_boot->BPB_VolSerial);
	}
	else this->letter = L'?';
	
	delete[] boot;
}

// ---------------------------------------------------------------------------------------------------------------------

String ^ volume::part::ToString() {
	return this->Name;
}

// ---------------------------------------------------------------------------------------------------------------------

bool volume::part::ReadSectors(QWORD LBAPos, LPVOID Buffer, DWORD ToRead, LPDWORD Read) {
	LBAPos += this->beg_lba;
	if (this->end_lba <= LBAPos + ToRead) {
		ToRead = (DWORD)(this->end_lba - LBAPos);
		if (this->end_lba <= LBAPos) {
			return false;
		}
	}
	return this->drive->ReadSectors(LBAPos, Buffer, ToRead, Read);
}

// ---------------------------------------------------------------------------------------------------------------------

Generic::List<filesystem::object^>^ volume::part::getDirItems(String ^Path) {
	if (this->fs_auto == L"FAT32") {
		return getFAT32DirIrems(this, Path);
	} 
	else if (this->letter != L'?') {
		return ::getDirItems(this->letter + L":\\" + Path);
	}
	else {
		throw gcnew NotSupportedException(L"Unsupported file system!");
	}
}

// ---------------------------------------------------------------------------------------------------------------------

volume::part_dbr::part_dbr(DriveAccess ^Drive, size_t PartID, const ::dbr::part &Part, QWORD Offset) :
	volume::part(Drive, PartID, Offset + Part.beg_lba, Offset + Part.beg_lba + Part.lba_sct) {

	this->is_primary = !Offset;
	this->is_boot = Part.boot_fl == 0x80;

	this->os_type = Part.os_type;

	auto info = ::dbr::getPartInfo(Part);

	this->is_hide = info->is_hide;
	
	this->fs_rec = gcnew String(info->fs_name);	
}

// ---------------------------------------------------------------------------------------------------------------------

volume::part_gpt::part_gpt(DriveAccess ^Drive, size_t PartID, const _guid &DriveGUID, const ::gpt::part &Part) :
	volume::part(Drive, PartID, Part.beg_lba, Part.end_lba) {

	this->drive_guid = new _guid(DriveGUID);

	this->part_guid = new _guid(Part.part_guid);
	this->type_guid = new _guid(Part.type_guid);

	auto info = ::gpt::getPartInfo(Part);

	this->desc = gcnew String(info->type_desc);


	this->letter = getSysLetter(Part.part_guid);
}

// ---------------------------------------------------------------------------------------------------------------------

volume::part_gpt::~part_gpt() {

	delete this->type_guid;
	delete this->part_guid;
}

// ---------------------------------------------------------------------------------------------------------------------

String ^getFormatSize(QWORD Size) {
	static LPWSTR sign[] = { L"byte", L"KB", L"MB", L"GB", L"TB", L"PB", L"EB", L"ZB", L"YB" };
	static size_t sign_s = sizeof(sign) / sizeof(LPWSTR);
	double __size = (double)Size;
	size_t i = 0;
	while (i < sign_s && __size / 1024 >= 0.8) {
		__size = __size / 1024; i++;
	}
	return String::Format(L"{0:0.00} ", __size) + gcnew String(sign[i]);
}

// ---------------------------------------------------------------------------------------------------------------------

WCHAR getSysLetter(DWORD Serial) {
	WCHAR address[] = L"A:\\";
	for (DWORD disks = GetLogicalDrives(), vSerial; disks; disks = disks >> 1) {
		if (disks & 0x00000001 
			&& GetVolumeInformation(address, NULL, NULL, &vSerial, NULL, NULL, NULL, NULL)
			&& Serial == vSerial) {
			return address[0];
		}
		address[0]++;
	}
	return L'?';
}

WCHAR getSysLetter(_guid GUID) {
	WCHAR address[] = L"A:\\";
	WCHAR guidStr[50];
	for (DWORD disks = GetLogicalDrives(); disks; disks = disks >> 1) {
		if (disks & 0x00000001
			&& GetVolumeNameForVolumeMountPoint(address, guidStr, 50)
			&& (gcnew String(guidStr))->Contains(GUID.ToString())) {
			return address[0];
		}
		address[0]++;
	}
	return L'?';
}

// ---------------------------------------------------------------------------------------------------------------------

// Get a partition free bytes count
QWORD getPartNumOfFreeByte(WCHAR Letter) {
	WCHAR Name[] = L" :\\\\";
	Name[0] = Letter;
	DWORD SecPerClus = 0, BytsPerSec = 0, NumOfFreeClus = 0;
	return GetDiskFreeSpace(Name, &SecPerClus, &BytsPerSec, &NumOfFreeClus, NULL) 
		? (QWORD)NumOfFreeClus * BytsPerSec * SecPerClus : 0;
}

// ---------------------------------------------------------------------------------------------------------------------

// Get a directory items using winapi
Generic::List<filesystem::object^> ^getDirItems(String ^Path) {
	Generic::List<filesystem::object^>^ objects = gcnew Generic::List<filesystem::object^>;

	Path += "\\*.*";

	pin_ptr<const wchar_t> address = PtrToStringChars(Path);

	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFile(address, &data);

	if (handle == INVALID_HANDLE_VALUE) {
		FindClose(handle);
		throw gcnew ArgumentException(gcnew String(L"Access error."));
	}

	do {

		filesystem::object^ object = gcnew filesystem::object;

		if (!(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) {

			object->name = gcnew String(data.cFileName);

			if (!object->name->Equals(L".") && !object->name->Equals(L"..")) {
				object->size = data.nFileSizeHigh * ((QWORD)MAXDWORD + 1) + data.nFileSizeLow;
				object->attr = data.dwFileAttributes;
				objects->Add(object);
			}
		}

	} while (FindNextFile(handle, &data));

	FindClose(handle);
	return objects;
}

// ---------------------------------------------------------------------------------------------------------------------

// Get FAT32 data clusters cout
DWORD getFAT32DataClusCout(const filesystem::fat32::boot &Boot) {
	DWORD TotSec = Boot.BPB_TotSecSm ? Boot.BPB_TotSecSm : Boot.BPB_TotSecLr;
	DWORD FATSize = Boot.BPB_FATSizeSm ? Boot.BPB_FATSizeSm : Boot.BPB_FATSizeLr;

	return (TotSec - Boot.BPB_RsvdSecCnt - (Boot.BPB_NumFATs * FATSize)) / Boot.BPB_SecPerClus;
}

// Get FAT record per FAT32 cluster  
DWORD getFAT32RecsPerFATSec(const filesystem::fat32::boot &Boot) {
	return Boot.BPB_BytsPerSec / sizeof(DWORD);
}

// Get FAT directory per cluster  
DWORD getFAT32DirRecPerClus(const filesystem::fat32::boot &Boot) {
	return (DWORD)Boot.BPB_BytsPerSec * Boot.BPB_SecPerClus / sizeof(filesystem::fat32::dir_rec);
}

// ---------------------------------------------------------------------------------------------------------------------

// Read FAT32 Boot Sector
BOOL readFAT32BootSect(volume::part ^Volume, filesystem::fat32::boot &Boot) {
	DWORD Read;
	return Volume->ReadSectors(0x00, &Boot, 1, &Read) && Read == sizeof(Boot);
}

// Read FAT32 data cluster
BOOL readFAT32DataClus(volume::part ^Volume, const filesystem::fat32::boot &Boot, DWORD NumClus, LPVOID Buff) {
	if (NumClus < 2 || NumClus > getFAT32DataClusCout(Boot)) {
		return false;
	}
	DWORD FATSize = Boot.BPB_FATSizeSm ? Boot.BPB_FATSizeSm : Boot.BPB_FATSizeLr;
	QWORD ClusPos = Boot.BPB_RsvdSecCnt + (QWORD)Boot.BPB_NumFATs * FATSize + (NumClus - 2) * Boot.BPB_SecPerClus;
	return Volume->ReadSectors(ClusPos, Buff, Boot.BPB_SecPerClus, NULL);
}

// Read FAT32 FAT Value
BOOL readFAT32ClusMark(volume::part ^Volume, const filesystem::fat32::boot &Boot, DWORD NumClus, DWORD &Mark) {
	if (NumClus > getFAT32DataClusCout(Boot)) {
		return false;
	}

	DWORD FATsPerSect = getFAT32RecsPerFATSec(Boot);
	DWORD *FATsSect = new DWORD[FATsPerSect];
	DWORD NumClusFAT = Boot.BPB_RsvdSecCnt + NumClus / FATsPerSect;
	DWORD NumMarkPos = NumClus % FATsPerSect;

	DWORD Read;
	BOOL result = Volume->ReadSectors(NumClusFAT, FATsSect, 1, &Read) && Read == sizeof(Boot);

	Mark = result ? FATsSect[NumMarkPos] : 0;

	delete[] FATsSect;

	return result;
}

// Read FAT32 element clusters number list
BOOL readFAT32ClusList(volume::part ^Volume, const filesystem::fat32::boot &Boot, DWORD NumClus, Generic::List<DWORD> ^ClusList) {
	ClusList->Clear();

	static CONST DWORD SYS = 0x0FFFFFF5;
	static CONST DWORD EOC = 0x0FFFFFF8;

	DWORD FATsPerSect = getFAT32RecsPerFATSec(Boot);
	DWORD *FATsSect = new DWORD[FATsPerSect];
	DWORD Read;

	DWORD ClusCount = getFAT32DataClusCout(Boot);

	DWORD OldNumClus = EOC;
	DWORD NumSectFAT;
	DWORD NumMarkPos;

	while (NumClus > 1 && NumClus < ClusCount) {

		ClusList->Add(NumClus);

		NumSectFAT = Boot.BPB_RsvdSecCnt + NumClus / FATsPerSect;
		NumMarkPos = NumClus % FATsPerSect;

		if (NumSectFAT == OldNumClus) {
			NumClus = FATsSect[NumMarkPos];
		}
		else if (Volume->ReadSectors(NumSectFAT, FATsSect, 1, &Read) && Read == sizeof(Boot)) {
			NumClus = FATsSect[NumMarkPos];
			OldNumClus = NumSectFAT;
		}
		else {
			ClusList->Clear();
			delete[] FATsSect;
			return false;
		}
	}

	delete[] FATsSect;

	if (NumClus == 0 || (NumClus > SYS && NumClus < EOC)) {
		ClusList->Clear();
		return false;
	}

	return true;
}

// Read FAT32 info sector
BOOL readFAT32InfoSec(volume::part ^Volume, const filesystem::fat32::boot &Boot, filesystem::fat32::info &Info) {
	DWORD Read;
	return Volume->ReadSectors((QWORD)Boot.BPB_SecNumInfo, &Info, 1, &Read) && Read == sizeof(filesystem::fat32::info);
}

// ---------------------------------------------------------------------------------------------------------------------

// Get fat32 partition free bytes count
QWORD getFAT32NumOfFreeByte(volume::part ^Volume) {

	filesystem::fat32::boot Boot;

	if (!readFAT32BootSect(Volume, Boot)) {
		return 0;
	}

	filesystem::fat32::info Info;

	if (!readFAT32InfoSec(Volume, Boot, Info)) {
		return 0;
	}

	DWORD ClusPerFAT = getFAT32DataClusCout(Boot);

	if (Info.FSI_FreeCount != 0xFFFFFFFF || Info.FSI_FreeCount > ClusPerFAT) {
		return (QWORD)Info.FSI_FreeCount * Boot.BPB_SecPerClus * Boot.BPB_BytsPerSec;
	}

	DWORD NumClusFAT = Boot.BPB_RsvdSecCnt;
	
	DWORD RecsPerSec = getFAT32RecsPerFATSec(Boot);

	DWORD NumFATSec = ClusPerFAT / RecsPerSec;

	DWORD *FATSec = new DWORD[ClusPerFAT];

	QWORD FreeClust = 0;

	if (!Volume->ReadSectors(NumClusFAT, FATSec, NumFATSec, NULL)) {
		return 0;
	}

	for (size_t i = 0; i < ClusPerFAT; i++) {
		if (FATSec[i] == 0) {
			++FreeClust;
		}
	}

	delete[] FATSec;

	return FreeClust * Boot.BPB_SecPerClus * Boot.BPB_BytsPerSec;
}

// ---------------------------------------------------------------------------------------------------------------------

// Get format small name string
String ^SmallNameString(const CHAR SmallName[11]) {
	CHAR FormatName[13];
	size_t NamePos = 0;
	for (size_t i = 0; i < 8 && SmallName[i] != 0x20; NamePos++, i++) {
		FormatName[NamePos] = SmallName[i];
	}
	if (SmallName[8] != 0x20) {
		FormatName[NamePos] = '.'; NamePos++;
	}
	for (size_t i = 8; i < 11 && SmallName[i] != 0x20; NamePos++, i++) {
		FormatName[NamePos] = SmallName[i];
	}
	FormatName[NamePos] = '\0';
	return gcnew String(FormatName);
}

// Get format small name hashsum
BYTE SmallNameHashsum(const CHAR SmallName[11]) {
	BYTE Sum = 0;
	for (size_t i = 11; i != 0; i--) {
		Sum = ((Sum & 1) ? 0x80 : 0) + (Sum >> 1) + (BYTE)*SmallName++;
	}
	return (Sum);
}



// Get format directory item name
String ^DirItemNameString(CHAR SmallName[11], stack<filesystem::fat32::dir_name> FullName) {

	if (FullName.empty()) {
		return SmallNameString(SmallName);
	}

	BYTE Hashsum = SmallNameHashsum(SmallName);
	String ^LongName = gcnew String(L"");

	for (size_t i = 1; !FullName.empty(); i++, FullName.pop()) {

		filesystem::fat32::dir_name NameItem = FullName.top();

		if ((FullName.size() == 1 ? (NameItem.LN_Ordinal & 0x40) != 0x40 : NameItem.LN_Ordinal != i) || NameItem.LN_Hashsum != Hashsum) {
			return SmallNameString(SmallName);
		}

		WCHAR NamePart[14]; 

		size_t PartPos = 0;

		for (size_t i = 0; i < 5; i++, PartPos++) {
			NamePart[PartPos] = NameItem.LN_Part1[i];
		}

		for (size_t i = 0; i < 6; i++, PartPos++) {
			NamePart[PartPos] = NameItem.LN_Part2[i];
		}

		for (size_t i = 0; i < 2; i++, PartPos++) {
			NamePart[PartPos] = NameItem.LN_Part3[i];
		}

		NamePart[PartPos] = L'\0';
		LongName += gcnew String(NamePart);

	}
	return LongName;
}

// Get FAT32 directory items
Generic::List<filesystem::object^> ^getFAT32DirIrems(volume::part ^ Volume, String ^ Path) {
	Generic::List<filesystem::object^> ^objects = gcnew Generic::List<filesystem::object^>;

	filesystem::fat32::boot Boot;

	if (!readFAT32BootSect(Volume, Boot)) {
		throw gcnew ArgumentException(L"Error reading boot sector.");
	}

	if (!filesystem::fat32::verify(&Boot)) {
		throw gcnew ArgumentException(L"Error boot sector corrupt.");
	}

	Generic::List<DWORD> ^DirClusList = gcnew Generic::List<DWORD>;

	if (!readFAT32ClusList(Volume, Boot, Boot.BPB_ClusNumRt, DirClusList)) {
		throw gcnew Exception(L"Error in directory fat segmentation.");
	}

	DWORD DirRecCount = getFAT32DirRecPerClus(Boot);

	filesystem::fat32::dir_rec *FATClus = new filesystem::fat32::dir_rec[DirRecCount];

	stack<filesystem::fat32::dir_name> *DirLongNameStack = new stack<filesystem::fat32::dir_name>;


	String ^ClearPAth = (Path->Length ? Path->Remove(0, 1) : Path);

	Generic::List<String^> ^Dirs = gcnew Generic::List<String^>(ClearPAth->Split(L'\\'));

	do {

		String ^DirName = Dirs[0];

		BOOL IsExstDir = false;
		BOOL IsStopped = false;

		for each (DWORD DirClusNum in DirClusList) {

			if (!readFAT32DataClus(Volume, Boot, DirClusNum, FATClus)) {
				throw gcnew Exception(L"Error reading exeption.");
			}

			for (size_t i = 0; i < DirRecCount; i++) {

				BYTE Flag = (BYTE)FATClus[i].item.DI_ShortName[0];

				if (Flag == 0x00) {
					IsStopped = true;
					break;
				}
	
				bool isEmpty = Flag == (BYTE)0x05;

				if (isEmpty) {
					isEmpty = true;
					for (size_t i = 1; isEmpty && i < 11; i++) {
						if ((BYTE)FATClus[i].item.DI_ShortName[i] != 0x00) {
							isEmpty = false;
						}
					}
				}

				if (!isEmpty && Flag == (BYTE)0xE5) {
					isEmpty = true;
				}

				if (!isEmpty) {
					
					if (FATClus[i].name.LN_Type == 0 && FATClus[i].name.DI_Attribute == 0x0F && FATClus[i].name.LN_FstClus == 0) {

						if (DirLongNameStack->size()) {
							byte LN_Ordinal = DirLongNameStack->top().LN_Ordinal;
							if (LN_Ordinal != 0x42 && LN_Ordinal - 1 != FATClus[i].name.LN_Ordinal) {
								delete DirLongNameStack;
								DirLongNameStack = new stack<filesystem::fat32::dir_name>;
							}
						}
						DirLongNameStack->push(FATClus[i].name);
					}

					else {

						if (DirName == L"") {

							String ^ItemName = DirItemNameString(FATClus[i].item.DI_ShortName, *DirLongNameStack);

							if (ItemName != L"." && ItemName != L"..") {
								filesystem::object ^object = gcnew filesystem::object;

								object->name = ItemName;
								object->attr = FATClus[i].item.DI_Attributes;
								object->size = FATClus[i].item.DI_FileSize;

								objects->Add(object);
							}
						}

						else if (DirName == DirItemNameString(FATClus[i].item.DI_ShortName, *DirLongNameStack)) {
							DWORD DirClus = FATClus[i].item.DI_NumClusHI * ((QWORD)MAXWORD + 1) + FATClus[i].item.DI_NumClusLO;
							if (!readFAT32ClusList(Volume, Boot, DirClus, DirClusList)) {
								throw gcnew Exception(L"Error in directory fat segmentation.");
							}
							IsStopped = true;
							IsExstDir = true;
						}

						delete DirLongNameStack;
						DirLongNameStack = new stack<filesystem::fat32::dir_name>;

						if (IsStopped) {
							break;
						}
					}
				}
			}
		
			if (IsStopped) {
				break;
			}
		}

		if (DirName != L"" && !IsExstDir) {
			throw gcnew Exception(L"Invalid path.");
		}

		Dirs->RemoveAt(0);

	} while (Dirs->Count);

	delete[] FATClus;
	delete DirLongNameStack;

	return objects;
}