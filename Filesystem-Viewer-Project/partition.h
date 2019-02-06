#pragma once

// ---------------------------------------------------------------------------------------------------------------------

#include<Windows.h>

// ---------------------------------------------------------------------------------------------------------------------

using namespace System;

// ---------------------------------------------------------------------------------------------------------------------

typedef unsigned long long QWORD;
typedef QWORD *LPQWORD;

// ---------------------------------------------------------------------------------------------------------------------

#pragma pack (push, 1)

// ---------------------------------------------------------------------------------------------------------------------

// Cylinder-Head-Sector address
typedef struct _chs {

public:

	_chs();

	WORD cyl();
	BYTE head();
	BYTE sect();

protected:

	BYTE bin[3];

} _chs;

// ---------------------------------------------------------------------------------------------------------------------

// Globally Unique Identifier
typedef struct _guid {

public:

	_guid();
	_guid(DWORD part_a, WORD part_b, WORD part_c, WORD part_d, QWORD part5);

	// convert guid_t
	operator bool() const;
	String ^ToString() const;

	// guid_t compare
	friend bool operator==(const _guid &left, const _guid &right);

protected:

	DWORD part_a;    // GUID part 1
	WORD  part_b;    // GUID part 2
	WORD  part_c;    // GUID part 3
	BYTE  part_d[8]; // GUID part 4

} _guid;

// ---------------------------------------------------------------------------------------------------------------------

#pragma pack (pop)

// ---------------------------------------------------------------------------------------------------------------------

// Drive boot record
namespace dbr {

	// -----------------------------------------------------------------------------------------------------------------

#pragma pack (push, 1)

	// -----------------------------------------------------------------------------------------------------------------

	// DBR partition record
	typedef struct part {

	public:

		BYTE boot_fl;  // Boot flag: 0x80 is boot, 0x00 is no boot, else is incorrect

		_chs beg_chs;  // Begin CHS address
		BYTE os_type;  // Partition type id
		_chs end_chs;  // End CHS address

		DWORD beg_lba; // Begin LBA address
		DWORD lba_sct; // LBA sectors count

	} part;

	// -----------------------------------------------------------------------------------------------------------------

	// Drive boot record
	typedef struct note {

		BYTE boot[446]; // Boot sector bytes
		part part[4];	// Partition table
		WORD sign;		// Magic number 0xAA55

	} note;

	// -----------------------------------------------------------------------------------------------------------------

#pragma pack (pop)

	// -----------------------------------------------------------------------------------------------------------------

	// DBR partition information
	typedef struct info {
		
		BYTE	os_type;

		bool    is_part;
		bool    is_hide;

		LPCWSTR fs_name;

	} info;

	// -----------------------------------------------------------------------------------------------------------------

	// get part info
	const info *getPartInfo(part part);

	// -----------------------------------------------------------------------------------------------------------------

};

// ---------------------------------------------------------------------------------------------------------------------

// GUID Partition Table
namespace gpt {

	// -----------------------------------------------------------------------------------------------------------------

#pragma pack (push, 1)

	// -----------------------------------------------------------------------------------------------------------------

	// GPT header record
	typedef struct head {

		CHAR  head_sign[8]; // Gpt header signature = "EFI PART"
		DWORD gpt_vers;     // Version drive GPT
		DWORD head_size;    // GPT header size = 92 byte
		DWORD check_sum;    // Checksum of header with this * field zeroed during calculation

		DWORD nt_reserve;     // Reserved value = 0

		QWORD this_lba;     // Location of this header
		QWORD copy_lba;     // Location of other header
		QWORD use_beg_lba;  // First usable LBA for partitions
		QWORD use_end_lba;  // Last usable LBA for partitions

		_guid guid;         // Drive GUID indicator

		QWORD parts_lba;    // GPT partition table
		DWORD parts_num;    // Reserved partition number
		DWORD part_size;    // Partition size
		DWORD parts_sum;    // Partition checksum

	} head;

	// -----------------------------------------------------------------------------------------------------------------

	// GPT part record
	typedef struct part {

		_guid type_guid;  // GUID type for GPT
		_guid part_guid;  // GUID id

		QWORD beg_lba;	  // Partition begin
		QWORD end_lba;    // Partition end

		QWORD options;    // Options

		WCHAR name[36];   // System drive name

	} part;

	// -----------------------------------------------------------------------------------------------------------------

#pragma pack (pop)

	// -----------------------------------------------------------------------------------------------------------------

	// GPT part table info struct
	typedef struct info {

		_guid	type_guid;
		LPCWSTR type_desc;

	} info;


	// -----------------------------------------------------------------------------------------------------------------

	// get part info
	const info *getPartInfo(part part);

	// -----------------------------------------------------------------------------------------------------------------

};

// ---------------------------------------------------------------------------------------------------------------------