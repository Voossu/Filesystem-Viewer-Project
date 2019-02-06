#pragma once

// ---------------------------------------------------------------------------------------------------------------------

#include<Windows.h>

// ---------------------------------------------------------------------------------------------------------------------

using namespace System;
using namespace System::Collections;

// ---------------------------------------------------------------------------------------------------------------------

typedef unsigned long long QWORD;
typedef QWORD *LPQWORD;

// ---------------------------------------------------------------------------------------------------------------------

#pragma pack (push, 1)

// ---------------------------------------------------------------------------------------------------------------------

// Time
typedef struct _time {

	BYTE second() const;
	BYTE minute() const;
	BYTE hours() const;

	String ^ToString() const;

protected:

	WORD time;

} _time;

// ---------------------------------------------------------------------------------------------------------------------

// Date
typedef struct _date {

	BYTE day() const;
	BYTE month() const;
	DWORD year() const;

	String ^ToString() const;

protected:

	WORD data;

} _date;

// ---------------------------------------------------------------------------------------------------------------------

// Data & Time
typedef struct _timestump {

	_time time;
	_date date;

	String ^ToString() const;

} _timestump;

// ---------------------------------------------------------------------------------------------------------------------

#pragma pack (pop)

// ---------------------------------------------------------------------------------------------------------------------

// Filesystem
namespace filesystem {

	// -----------------------------------------------------------------------------------------------------------------

	// Directory object 
	typedef ref struct object {

		String ^name;	// Object name
		DWORD attr;		// Object attributes
		QWORD size;		// Object size

	} object;

	// -----------------------------------------------------------------------------------------------------------------

	// exFAT
	namespace exfat {

		// -------------------------------------------------------------------------------------------------------------

#pragma pack (push, 1)

		// -------------------------------------------------------------------------------------------------------------

		// exFAT boot record
		typedef struct boot {

			BYTE  BS_JmpBoot[3];	 // JMP instruction
			CHAR  BS_OEMName[8];	 // OEM ID

			BYTE  PBP_AlwaysNull[53]; // Must be zero

			QWORD BPB_PrtOffset;	 // Partition offset
			QWORD BPB_LengthVol;	 // Volume length

			DWORD BPB_FATOffset;	 // Fat offset
			DWORD BPB_FATLength;	 // Fat length

			DWORD BPB_ClusHepOff;	 // Cluster heap offset
			DWORD BPB_ClusCnt;		 // Cluster count

			DWORD BPB_RtDirClus;	 // Root directory cluster

			DWORD BPB_VolSerial;	 // Volume serial number

			WORD  PBP_FlSysVer;		 // File system revision
			WORD  PBP_VolFlags;		 // Volume flags

			BYTE  PBP_BytsPerSec;	 // Bytes per sector shift

			BYTE  PBP_SecPerClus;	 // Sectors per cluster shift

			BYTE  BPB_NumFATs;		 // Number ofFats

			BYTE  BPB_DrvNum;		 // Drive select

			BYTE  BPB_PrcntInUse;	 // Percent in use

			BYTE  BPB_Reserved[7];	 // Reserved

			BYTE  BS_Boot[390];		 // Boot sector bytes
			WORD  BS_Sign;			 // Magic number 0xAA55

		} boot;

		// -------------------------------------------------------------------------------------------------------------

#pragma pack (pop)

		// -------------------------------------------------------------------------------------------------------------

		// check boot record
		bool verify(const filesystem::exfat::boot *boot);

		// -------------------------------------------------------------------------------------------------------------

	};

	// -----------------------------------------------------------------------------------------------------------------

	namespace fat32 {

		// -------------------------------------------------------------------------------------------------------------

#pragma pack (push, 1)

		// -------------------------------------------------------------------------------------------------------------

		// FAT32 boot record
		typedef struct boot {

			BYTE  BS_JmpBoot[3];	 // JMP instruction
			CHAR  BS_OEMName[8];	 // OEM ID

			WORD  BPB_BytsPerSec;	 // Bytes per sector
			BYTE  BPB_SecPerClus;	 // Sectors Per Cluster
			WORD  BPB_RsvdSecCnt;	 // Reserved Sectors

			BYTE  BPB_NumFATs;	 // Number of Copies of FAT
			WORD  BPB_RootEntCnt;	 // Maximum number of files in the root directory

			WORD  BPB_TotSecSm;	     // 16-bit count of sectors

			BYTE  BPB_MediaDesc;	 // Media descriptor

			WORD  BPB_FATSizeSm;	 // Sectors per file allocation table (FAT)

			WORD  BPB_SecPerTrk;	 // Sectors per Track
			WORD  BPB_NumOfHead;	 // Number of Head
			DWORD BPB_HiddenSec;	 // Hidden Sectors

			DWORD BPB_TotSecLr;	     // Sectors Count in Volume

			DWORD BPB_FATSizeLr;	 // Sectors per file allocation table (FAT)

			WORD  BPB_ExtFlags;		 // Flags 
			WORD  BPB_FlSysVers;	 // Version of FAT32 Drive

			DWORD BPB_ClusNumRt;	 // Root cluster number

			WORD  BPB_SecNumInfo;	 // FSInfo struct cluster
			WORD  BPB_BkBootSec;	 // Sector Number of the BackupBoot Sector

			BYTE  BPB_Reserved[12];  // Not used by FAT

			BYTE  BPB_DriveNum;		 // Logical Drive Number of Partition
			BYTE  BPB_CurrHead;		 // Not used by FAT
			BYTE  BPB_BootSign;		 // Extended Signature (Must be either 0x28 or 0x29)

			DWORD BPB_VolSerial;	 // Serial Number of Partition

			CHAR  BPB_VolLabel[11];  // Partition Volume Name ANSI String  

			CHAR  BPB_SystemID[8];   // System ID ANSI String

			BYTE  BS_Boot[420];		 // Boot sector bytes
			WORD  BS_Sign;			 // Magic number 0xAA55

		} boot;

		// -------------------------------------------------------------------------------------------------------------

		// FAT32 info record
		typedef struct info {

			DWORD FSI_LeadSign;		  // Always = 0x41615252
			BYTE  FSI_Reserved1[480]; // Reserved area
			DWORD FSI_StructSign;	  // Always = 0x61417272
			DWORD FSI_FreeCount;	  // Free sectors count
			DWORD FSI_FirstFree;	  // First free cluster
			BYTE  FSI_Reserved2[12];  // Reserved area
			DWORD FSI_TrailSign;	  // Always = 0xAA550000

		} info;

		// -------------------------------------------------------------------------------------------------------------

		// Directory item struct
		typedef struct dir_item {

			CHAR DI_ShortName[11];
			BYTE DI_Attributes;
			BYTE DI_NTReserved;
			BYTE DI_CreateMS;
			_timestump DI_Create;
			_date DI_Access;
			WORD  DI_NumClusHI;
			_timestump DI_Modify;
			WORD  DI_NumClusLO;
			DWORD DI_FileSize;

		} dir_item;

		// -------------------------------------------------------------------------------------------------------------

		// Directory name struct
		typedef struct dir_name {

			BYTE LN_Ordinal;
			WCHAR LN_Part1[5];
			BYTE DI_Attribute;
			BYTE LN_Type;
			BYTE LN_Hashsum;
			WCHAR LN_Part2[6];
			WORD LN_FstClus;
			WCHAR LN_Part3[2];

		} dir_name;

		// -------------------------------------------------------------------------------------------------------------

		// Directory note
		typedef union dir_rec {
			dir_item item;
			dir_name name;
		} dir_rec;

		// -------------------------------------------------------------------------------------------------------------

#pragma pack (pop)

		// -------------------------------------------------------------------------------------------------------------

		// check boot record
		bool verify(const filesystem::fat32::boot *Boot);

		// -------------------------------------------------------------------------------------------------------------

	};

	// -----------------------------------------------------------------------------------------------------------------

	namespace fat16 {

		// -------------------------------------------------------------------------------------------------------------

#pragma pack (push, 1)

		// -------------------------------------------------------------------------------------------------------------

		// FAT16 boot record
		typedef struct boot {

			BYTE  BS_JmpBoot[3];	 // JMP instruction
			CHAR  BS_OEMName[8];	 // OEM ID

			WORD  BPB_BytsPerSec;	 // Bytes per sector
			BYTE  BPB_SecPerClus;	 // Sectors Per Cluster
			WORD  BPB_RsvdSecCnt;	 // Reserved Sectors

			BYTE  BPB_NumFATs;	 // Number of Copies of FAT
			WORD  BPB_RootEntCnt;	 // Maximum number of files in the root directory

			WORD  BPB_TotSecSm;		 // 16-bit count of sectors

			BYTE  BPB_MediaDesc;	 // Media descriptor

			WORD  BPB_FATSizeSm;	 // Sectors per file allocation table (FAT)

			WORD  BPB_SecPerTrk;	 // Sectors per Track
			WORD  BPB_NumOfHead;	 // Number of Head
			DWORD BPB_HiddenSec;	 // Hidden Sectors

			DWORD BPB_TotSecLr;		 // Sectors Count in Volume

			BYTE  BPB_DriveNum;		 // Logical Drive Number of Partition
			BYTE  BPB_CurrHead;		 // Not used by FAT
			BYTE  BPB_BootSign;		 // Extended Signature (Must be either 0x28 or 0x29)

			DWORD BPB_VolSerial;	 // Serial Number of Partition

			CHAR  BPB_VolLabel[11];  // Partition Volume Name ANSI String  

			CHAR  BPB_SystemID[8];   // System ID ANSI String

			BYTE  BS_Boot[448];		 // Boot sector bytes
			WORD  BS_Sign;			 // Magic number 0xAA55

		} boot;

		// -------------------------------------------------------------------------------------------------------------

#pragma pack (pop)

		// -------------------------------------------------------------------------------------------------------------

		// check boot record
		bool verify(const filesystem::fat16::boot *Boot);

		// -------------------------------------------------------------------------------------------------------------

	};

	// -----------------------------------------------------------------------------------------------------------------

	namespace ntfs {

		// -------------------------------------------------------------------------------------------------------------

#pragma pack (push, 1)

		// -------------------------------------------------------------------------------------------------------------

		// NTFS boot record
		typedef struct boot {

			BYTE  BS_JmpBoot[3];	 // JMP instruction
			CHAR  BS_OEMName[8];	 // OEM ID ANSI String

			WORD  BPB_BytsPerSec;	 // Bytes per sector
			BYTE  BPB_SecPerClus;	 // Sectors Per Cluster
			WORD  BPB_RsvdSecCnt;	 // Reserved Sectors

			BYTE  BPB_Unused0;		 // Always = 0
			WORD  PBP_Unused1;		 // Always = 0
			WORD  PBP_Unused2;		 // Always = 0

			BYTE  BPB_MediaDesc;	 // Media descriptor

			WORD  PBP_Unused3;		 // Always = 0

			WORD  BPB_SecPerTrk;	 // Sectors per Track
			WORD  BPB_NumOfHead;	 // Number of Head
			DWORD BPB_HiddenSec;	 // Hidden Sectors

			DWORD PBP_Unused4;		 // Not used by NTFS
			DWORD PBP_Unused5;		 // Not used by NTFS

			QWORD BPB_TotalSec;	 // Total sectors in Volume

			QWORD PBP_MFTPrimSecNum; // Logical Cluster Number for the file $MFT
			QWORD PBP_MFTMirrSecNum; // Logical Cluster Number for the file $MFTMirr

			BYTE  PBP_BytsPerMTFRec; // Clusters Per MFT Record

			BYTE  PBP_Unused6[3];	 // Not used by NTFS

			BYTE  PBP_ClusPerIdxBuf; // Clusters Per Index Buffer

			BYTE  PBP_Unused7[3];	 // Not used by NTFS

			QWORD BPB_VolSerial;	 // Serial Number of Partition

			DWORD PBP_VolHashSum;	 // Volume Boot Sector Checksum

			BYTE  BS_Boot[426];		 // Boot sector bytes
			WORD  BS_Sign;			 // Magic number 0xAA55

		} boot;

		// -------------------------------------------------------------------------------------------------------------

#pragma pack (pop)

		// -------------------------------------------------------------------------------------------------------------

		// check boot record
		bool verify(const filesystem::ntfs::boot *Boot);

		// -------------------------------------------------------------------------------------------------------------

	};

	// -----------------------------------------------------------------------------------------------------------------

	typedef union boot {

		filesystem::exfat::boot exfat;
		filesystem::fat32::boot fat32;
		filesystem::fat16::boot fat16;
		filesystem::ntfs::boot ntfs;

	} boot;

	// -----------------------------------------------------------------------------------------------------------------

	String ^fs_auto(const filesystem::boot *Boot);

	// -----------------------------------------------------------------------------------------------------------------

};

// ---------------------------------------------------------------------------------------------------------------------