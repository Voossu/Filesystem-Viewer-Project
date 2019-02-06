#include "partition.h"

// ---------------------------------------------------------------------------------------------------------------------

#include<iomanip>
using std::setw;
using std::setfill;
using std::hex;
using std::dec;

// ---------------------------------------------------------------------------------------------------------------------

using namespace System;

// ---------------------------------------------------------------------------------------------------------------------

_chs::_chs() {
	for (size_t i = 0; i < 3; ++i) {
		this->bin[i] = 0;
	}
}

WORD _chs::cyl() {
	return ((this->bin[1] >> 6) << 8) | this->bin[2];
}

BYTE _chs::head() {
	return this->bin[0];
}

BYTE _chs::sect() {
	return this->bin[1] & 0x3F;
}

// ---------------------------------------------------------------------------------------------------------------------

// Type partition information map
const dbr::info _dbr_info_map[] = {

	{ 0x00,    true,	false,	  L"EMPTY" },

	{ 0x01,    true,	false,    L"FAT12" },
	{ 0x8D,    true,	false,	  L"FAT12" },

	{ 0x04,    true,	false,	  L"FAT16" },
	{ 0x06,    true,	false,	  L"FAT16" },
	{ 0x16,    true,	 true,	  L"FAT16" },
	{ 0x0E,    true,	false,	  L"FAT16" },
	{ 0x1E,    true,	 true,	  L"FAT16" },

	{ 0x0B,    true,	false,	  L"FAT32" },
	{ 0x1B,    true,	 true,	  L"FAT32" },
	{ 0x0C,    true,	false,	  L"FAT32" },
	{ 0x1C,    true,	 true,	  L"FAT32" },

	{ 0x07,    true,	false,	  L"exFAT/NTFS" },

	{ 0x82,    true,	 true,	  L"LINUX SWAP" },
	{ 0x83,    true,	false,	  L"LINUX EXT*" },

	{ 0x05,   false,    false,	  L"Container" },
	{ 0x15,   false,	 true,	  L"Container", },
	{ 0x0F,   false,	false,	  L"Container", },
	{ 0x0F,   false,	 true,	  L"Container", },
	{ 0x85,   false,	false,	  L"Container", }

};

// ---------------------------------------------------------------------------------------------------------------------

const dbr::info *dbr::getPartInfo(dbr::part part) {
	size_t length = sizeof(_dbr_info_map) / sizeof(dbr::info);
	for (size_t i = 0; i < length; i++) {
		if (_dbr_info_map[i].os_type == part.os_type) {
			return _dbr_info_map + i;
		}
	}
	return nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------

_guid::_guid() :
	part_a(0),
	part_b(0),
	part_c(0) {
	for (size_t i = 0; i < 8; ++i) {
		part_d[i] = 0;
	}
}

_guid::_guid(DWORD part1, WORD part2, WORD part3, WORD part4, QWORD part5) :
	part_a(part1),
	part_b(part2),
	part_c(part3) {
	BYTE *ptr;
	ptr = reinterpret_cast<BYTE*>(&part4);
	for (size_t i = 0; i < 2; i++) {
		this->part_d[i] = ptr[1 - i];
	}
	ptr = reinterpret_cast<BYTE*>(&part5);
	for (size_t i = 2; i < 8; i++) {
		this->part_d[i] = ptr[7 - i];
	}
}

_guid::operator bool() const {
	if (!this->part_a || !this->part_b || !this->part_c) {
		return false;
	}
	for (size_t i = 0; i < 8; ++i) {
		if (!this->part_d[i]) {
			return false;
		}
	}
	return true;
}

String ^_guid::ToString() const {
	String ^ result = String::Format(L"{0:x8}-{1:x4}-{2:x4}-", this->part_a, this->part_b, this->part_c);
	for (size_t i = 0; i < 2; ++i) {
		result += String::Format(L"{0:x2}", (WORD)this->part_d[i]);
	}
	result += L'-';
	for (size_t i = 2; i < 8; ++i) {
		result += String::Format(L"{0:x2}", this->part_d[i]);
	}
	return L"{" + result + L"}";
}

bool operator==(const _guid &left, const _guid &right) {
	if (left.part_a != right.part_a || left.part_b != right.part_b || left.part_c != right.part_c) {
		return false;
	}
	for (size_t i = 0; i < 8; ++i) {
		if (left.part_d[i] != left.part_d[i]) {
			return false;
		}
	}
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------

const gpt::info _gpt_info_map[] = {

	{ { 0x00000000, 0x0000, 0x0000, 0x0000, 0x000000000000 },  L"EMPTY\0                                   " },
	{ { 0x024DEE41, 0x33E7, 0x11D3, 0x9D69, 0x0008C781F39F },  L"MBR Partitions Scheme\0                   " },
	{ { 0xC12A7328, 0xF81F, 0x11D2, 0xBA4B, 0x00A0C93EC93B },  L"EFI System\0                              " },
	{ { 0x21686148, 0x6449, 0x6E6F, 0x744E, 0x656564454649 },  L"BIOS Boot\0                               " },
	{ { 0xD3BFE2DE, 0x3DAF, 0x11DF, 0xBA40, 0xE3A556D89593 },  L"Intel Fast Flash (iFFS)\0                 " },

	{ { 0xE3C9E316, 0x0B5C, 0x4DB8, 0x817D, 0xF92DF00215AE },  L"Microsoft Reserved Partition (MSR)\0      " },
	{ { 0xEBD0A0A2, 0xB9E5, 0x4433, 0x87C0, 0x68B6B72699C7 },  L"Basic data Manager (LDM) metadata\0       " },
	{ { 0xAF9B60A0, 0x1431, 0x4F62, 0xBC68, 0x3311714A69AD },  L"Logical Disk Manager data\0               " },
	{ { 0xDE94BBA4, 0x06D1, 0x4D40, 0xA16A, 0xBFD50179D6AC },  L"Windows Recovery Environment\0            " },
	{ { 0x37AFFC90, 0xEF7D, 0x4E96, 0x91C3, 0x2D7AE055B174 },  L"IBM General Parallel File System (GPFS)\0 " },
	{ { 0xE75CAF8F, 0xF680, 0x4CEE, 0xAFA3, 0xB001E56EFC2D },  L"Storage Spaces partition\0                " },

	{ { 0x0FC63DAF, 0x8483, 0x4772, 0x8E79, 0x3D69D8477DE4 },  L"Linux filesystem data\0                   " },
	{ { 0xA19D880F, 0x05FC, 0x4D3B, 0xA006, 0x743F0F84911E },  L"RAID partition\0                          " },
	{ { 0x0657FD6D, 0xA4AB, 0x43C4, 0x84E5, 0x0933C84B4F4F },  L"/swap\0                                   " },
	{ { 0x44479540, 0xF297, 0x41B2, 0x9AF7, 0xD131D5F0458A },  L"/root (x86)\0                             " },
	{ { 0x4F68BCE3, 0xE8CD, 0x4DB1, 0x96E7, 0xFBCAF984B709 },  L"/root (x86-64)\0                          " },
	{ { 0x69DAD710, 0x2CE4, 0x4E3C, 0xB16C, 0x21A1D49ABED3 },  L"/root (ARM x86)\0                         " },
	{ { 0xB921B045, 0x1DF0, 0x41C3, 0xAF44, 0x4C6F280D3FAE },  L"/root (ARM x64)\0                         " },
	{ { 0xE6D6D379, 0xF507, 0x44C2, 0xA23C, 0x238F2A3DF928 },  L"Logical Volume Manager (LVM)\0            " },
	{ { 0x933AC7E1, 0x2EB4, 0x4F13, 0xB844, 0x0E14E2AEF915 },  L"/home\0                                   " },
	{ { 0x3B8F8425, 0x20E0, 0x4F3B, 0x907F, 0x1A25A76F98E8 },  L"/srv (server data)\0                      " },
	{ { 0x7FFEC5C9, 0x2D00, 0x49B7, 0x8941, 0x3EA10A5586B7 },  L"Plain dm-crypt partition\0                " },
	{ { 0xCA7D7CCB, 0x63ED, 0x4C53, 0x861C, 0x1742536059CC },  L"LUKS partition\0                          " },
	{ { 0x8DA63339, 0x0007, 0x60C0, 0xC436, 0x083AC8230908 },  L"RESERVE\0                                " }

};

// ---------------------------------------------------------------------------------------------------------------------

const gpt::info *gpt::getPartInfo(gpt::part part) {
	size_t length = sizeof(_gpt_info_map) / sizeof(gpt::info);
	for (size_t i = 0; i < length; i++) {
		if (_gpt_info_map[i].type_guid == part.type_guid) {
			return _gpt_info_map + i;
		}
	}
	return nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
