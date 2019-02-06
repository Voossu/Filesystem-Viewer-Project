#include "filesystem.h"

// ---------------------------------------------------------------------------------------------------------------------

#include<vcclr.h>

// ---------------------------------------------------------------------------------------------------------------------

BYTE _time::second() const {
	return (time & 0x0E) * 2;
}

BYTE _time::minute() const {
	return (time >> 0x05) & 0x1F;
}

BYTE _time::hours() const {
	return (time >> 0x0B) & 0x0F;
}

String ^ _time::ToString() const {
	return String::Format("{0:00}:{1:00}:{2:00}", this->hours(), this->minute(), this->second());
}

// ---------------------------------------------------------------------------------------------------------------------

BYTE _date::day() const {
	return data & 0x0F;
}

BYTE _date::month() const {
	return (data >> 0x05) & 0x0F;
}

DWORD _date::year() const {
	return 1980 + (data >> 0x09);
}

String ^_date::ToString() const {
	return String::Format(L"{0}/{1:00}/{2:00}", this->year(), this->month(), this->day());
}

// ---------------------------------------------------------------------------------------------------------------------

String ^_timestump::ToString() const {
	return this->date.ToString() + L" " + this->time.ToString();
}

// ---------------------------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------------------------

String ^ filesystem::fs_auto(const filesystem::boot *Boot) {

	if (filesystem::exfat::verify(&(Boot->exfat))) {
		return L"exFAT";
	}
	if (filesystem::fat32::verify(&(Boot->fat32))) {
		return L"FAT32";
	}
	if (filesystem::fat16::verify(&(Boot->fat16))) {
		return L"FAT16";
	}
	if (filesystem::ntfs::verify(&(Boot->ntfs))) {
		return L"NTFS";
	}
	return L"UNKNOWN";
}

// ---------------------------------------------------------------------------------------------------------------------

bool filesystem::fat16::verify(const filesystem::fat16::boot *Boot) {

	if (Boot->BS_Sign != 0xAA55 /*|| !(gcnew String(Boot->BPB_SystemID))->ToUpper()->Contains(L"FAT16")*/
		|| !Boot->BPB_BytsPerSec || !Boot->BPB_RsvdSecCnt || Boot->BPB_CurrHead) {
		return false;
	}

	DWORD RootDirSect = (Boot->BPB_RootEntCnt * 32) / Boot->BPB_BytsPerSec;
	DWORD TotSec = Boot->BPB_TotSecSm ? Boot->BPB_TotSecSm : Boot->BPB_TotSecLr;
	DWORD FATSize = Boot->BPB_FATSizeSm;
	
	if (!TotSec || !FATSize) {
		return false;
	}

	DWORD DataSec = TotSec - Boot->BPB_RsvdSecCnt - ((DWORD)Boot->BPB_NumFATs * FATSize) - RootDirSect;
	DWORD CountOfClusters = DataSec / Boot->BPB_SecPerClus;

	return CountOfClusters <= 65524;
}

// ---------------------------------------------------------------------------------------------------------------------

bool filesystem::fat32::verify(const filesystem::fat32::boot *Boot) {

	if (Boot->BS_Sign != 0xAA55 /*|| !(gcnew String(Boot->BPB_SystemID))->ToUpper()->Contains(L"FAT32")*/
		|| !Boot->BPB_BytsPerSec || !Boot->BPB_RsvdSecCnt || Boot->BPB_FlSysVers) {
		return false;
	}

	DWORD RootDirSect = (Boot->BPB_RootEntCnt * 32) / Boot->BPB_BytsPerSec;
	DWORD TotSec = Boot->BPB_TotSecSm ? Boot->BPB_TotSecSm : Boot->BPB_TotSecLr;
	DWORD FATSize = Boot->BPB_FATSizeSm ? Boot->BPB_FATSizeSm : Boot->BPB_FATSizeLr;

	if (!TotSec || !FATSize) {
		return false;
	}

	DWORD DataSec = TotSec - Boot->BPB_RsvdSecCnt - ((DWORD)Boot->BPB_NumFATs * FATSize) - RootDirSect;
	DWORD CountOfClusters = DataSec / Boot->BPB_SecPerClus;

	return CountOfClusters > 65524;
}

// ---------------------------------------------------------------------------------------------------------------------

bool filesystem::exfat::verify(const filesystem::exfat::boot *Boot) {

	if (Boot->BS_Sign != 0xAA55 || !(gcnew String(Boot->BS_OEMName))->Contains(L"EXFAT")) {
		return false;
	}

	size_t SysNullBtCount = sizeof(Boot->PBP_AlwaysNull) / sizeof(*Boot->PBP_AlwaysNull);

	for (size_t i = 0; i < SysNullBtCount; i++) {
		if (Boot->PBP_AlwaysNull[i]) {
			return false;
		}
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bool filesystem::ntfs::verify(const filesystem::ntfs::boot *Boot) {

	if (Boot->BS_Sign != 0xAA55 || !(gcnew String(Boot->BS_OEMName))->Contains(L"NTFS")
		|| Boot->BPB_Unused0 || Boot->PBP_Unused1 || Boot->PBP_Unused2 || Boot->PBP_Unused3) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------
