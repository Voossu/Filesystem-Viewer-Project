#pragma once

// ---------------------------------------------------------------------------------------------------------------------

#include"partition.h"
#include"filesystem.h"

// ---------------------------------------------------------------------------------------------------------------------

using namespace System;
using namespace System::Collections;
using namespace System::Data;

// ---------------------------------------------------------------------------------------------------------------------

namespace volume {

	ref class part;
	ref class part_dbr;
	ref class part_gpt;

}

// ---------------------------------------------------------------------------------------------------------------------

typedef ref class DriveAccess {

	// -----------------------------------------------------------------------------------------------------------------

protected:

	// -----------------------------------------------------------------------------------------------------------------
	
	bool ReadHandle (HANDLE &Handle);

	bool ReadSectors (HANDLE &Handle, QWORD LBAPos, LPVOID Buffer, DWORD ToRead, LPDWORD Read);

	void ReadVolumes (HANDLE &Handle, QWORD LBAPos);

	DriveAccess (size_t DriveID);
	
	// -----------------------------------------------------------------------------------------------------------------

	size_t  driveID;
	String ^address;
	
	DWORD bytesPerSector;
	QWORD numberOfSectors;

	Generic::List<String ^> ^errors = gcnew Generic::List<String ^>;
	
	Generic::List<volume::part ^> ^volumes = gcnew Generic::List<volume::part ^>;

	// -----------------------------------------------------------------------------------------------------------------

public:

	// -----------------------------------------------------------------------------------------------------------------

	static DriveAccess ^Open(size_t DriveID);

	// -----------------------------------------------------------------------------------------------------------------

	static Generic::List<DriveAccess^> ^OpenAll();

	// -----------------------------------------------------------------------------------------------------------------

	virtual String ^ToString() override;

	// -----------------------------------------------------------------------------------------------------------------

	bool ReadSectors(QWORD LBAPos, LPVOID Buffer, DWORD ToRead, LPDWORD Read);

	// -----------------------------------------------------------------------------------------------------------------

	static property size_t MaxNumber {
		size_t get() {
			return 32;
		}
	}

	property size_t DriveID {
		size_t get() {
			return this->driveID;
		}
	}

	property String ^Address {
		String ^ get() {
			return gcnew String(this->address);
		}
	}

	property DWORD BytesPerSector {
		DWORD get() {
			return this->bytesPerSector;
		}
	}

	property QWORD NumberOfSectors {
		QWORD get() {
			return this->numberOfSectors;
		}
	}

	property QWORD NumberOfBytes {
		QWORD get() {
			return this->numberOfSectors * this->bytesPerSector;
		}
	}

	property Generic::List<String ^> ^Errors {
		Generic::List<String ^> ^ get() {
			return this->errors;
		}
	}

	property Generic::List<volume::part ^> ^Volumes {
		Generic::List<volume::part ^> ^ get() {
			return this->volumes;
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

} DriveAccess;

// ---------------------------------------------------------------------------------------------------------------------

typedef ref class volume::part {

	// -----------------------------------------------------------------------------------------------------------------

protected:

	// -----------------------------------------------------------------------------------------------------------------

	DriveAccess ^drive;

	String ^name;

	WCHAR letter;

	QWORD beg_lba;
	QWORD end_lba;

	String ^fs_auto;

	// -----------------------------------------------------------------------------------------------------------------

	QWORD getNumOfFreeBytes();

	// -----------------------------------------------------------------------------------------------------------------

public:

	// -----------------------------------------------------------------------------------------------------------------

	part(DriveAccess ^Drive, size_t PartID, QWORD BegLBA, QWORD EndLBA);

	// -----------------------------------------------------------------------------------------------------------------

	virtual String ^ToString() override;

	// -----------------------------------------------------------------------------------------------------------------

	bool ReadSectors(QWORD LBAPos, LPVOID Buffer, DWORD ToRead, LPDWORD Read);

	// -----------------------------------------------------------------------------------------------------------------

	Generic::List<filesystem::object^> ^getDirItems(String ^Path);

	// -----------------------------------------------------------------------------------------------------------------

	property DriveAccess ^Drive {
		DriveAccess ^get() {
			return this->drive;
		}
	}

	property String ^Name {
		String ^get() {
			return this->name + L" > " + this->letter;
		}
	}

	property WCHAR Letter {
		WCHAR get() {
			return this->letter;
		}
	}

	property QWORD BegLBA {
		QWORD get() {
			return this->beg_lba;
		}
	};

	property QWORD EndLBA {
		QWORD get() {
			return this->end_lba;
		}
	};

	property String ^FSNameAuto {
		String ^get() {
			return gcnew String(this->fs_auto);
		}
	}

	property QWORD NumberOfSectors {
		QWORD get() {
			return this->end_lba - this->beg_lba;
		}
	}

	property DWORD BytesPerSector {
		DWORD get() {
			return this->drive->BytesPerSector;
		}
	}

	property QWORD NumberOfBytes {
		QWORD get() {
			return this->NumberOfSectors * this->BytesPerSector;
		}
	}

	property QWORD NumberOfFreeBytes {
		QWORD get() {
			return this->getNumOfFreeBytes();
		}
	}

	// -----------------------------------------------------------------------------------------------------------------
	
} part;

// --------------------------------------------------------------------------------------------------------------------

typedef ref class volume::part_dbr : public part {

	// ----------------------------------------------------------------------------------------------------------------

protected:

	// ----------------------------------------------------------------------------------------------------------------

	bool is_primary;
	bool is_boot;

	BYTE os_type;
	bool is_hide;

	String ^fs_rec;

	// ----------------------------------------------------------------------------------------------------------------

public:

	// ----------------------------------------------------------------------------------------------------------------

	part_dbr(DriveAccess ^Drive, size_t PartID, const ::dbr::part &Part, QWORD Offset);

	// ----------------------------------------------------------------------------------------------------------------

	property bool IsPrime {
		bool get() {
			return this->is_primary;
		}
	}

	property bool IsBoot {
		bool get() {
			return this->is_boot;
		}
	}

	property BYTE OSType {
		BYTE get() {
			return this->os_type;
		}
	}

	property bool IsHide {
		bool get() {
			return this->is_hide;
		}
	}

	property String ^ FSNameRec {
		String ^get() {
			return gcnew String(this->fs_rec);
		}
	}


	// ----------------------------------------------------------------------------------------------------------------

} part_dbr;

// --------------------------------------------------------------------------------------------------------------------

typedef ref class volume::part_gpt : public part {

	// ----------------------------------------------------------------------------------------------------------------

protected:

	// ----------------------------------------------------------------------------------------------------------------

	_guid *drive_guid;
	_guid *type_guid;
	_guid *part_guid;

	String ^desc;

	// ----------------------------------------------------------------------------------------------------------------

public:

	// ----------------------------------------------------------------------------------------------------------------

	part_gpt(DriveAccess ^Drive, size_t PartID, const _guid &DriveGUID, const ::gpt::part &Part);
	~part_gpt();

	// ----------------------------------------------------------------------------------------------------------------

	property _guid DriveGUID {
		_guid get() {
			return *this->drive_guid;
		}
	}

	property _guid TypeGUID {
		_guid get() {
			return *this->type_guid;
		}
	};

	property _guid PartGUID {
		_guid get() {
			return *this->part_guid;
		}
	}

	property String ^Descript {
		String ^ get() {
			return gcnew String(this->desc);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

} part_gpt;

// ---------------------------------------------------------------------------------------------------------------------

String ^getFormatSize(QWORD Size);

// ---------------------------------------------------------------------------------------------------------------------