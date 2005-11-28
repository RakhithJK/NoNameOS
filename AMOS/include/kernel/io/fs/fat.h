#ifndef _KERNEL_IO_FS_FAT_H_
#define _KERNEL_IO_FS_FAT_H_

#include <sys/types.h>
#include <kernel/io/io.h>

#define FAT_CLUSTER12(c)	( c & 0x00000FFF )	// 12 bits
#define FAT_CLUSTER16(c)	( c & 0x0000FFFF )	// 16 bits
#define FAT_CLUSTER31(c)	( c & 0x0FFFFFFF )	// 28 bits


#define FAT_EOC12			0xFF8
#define FAT_BAD_CLUSTER		0xFF7
#define FAT_EOC16			0xFFF8
#define FAT_EOC32			0xFFFFFFF8

#define FAT_12		12
#define FAT_16		16
#define FAT_32		32

#define FAT_MAGIC	0xAA55

// for FAT 12 and 16
struct FAT_BOOTSECTOR16
{
	BYTE BS_DrvNum;
	BYTE BS_Reserved1;
	BYTE BS_BootSig;
	DWORD BS_VolID;
	BYTE BS_VolLab[11];
	BYTE BS_FilSysType[8];
} PACKED;

// for FAT 32
struct FAT_BOOTSECTOR32
{
	DWORD BPB_FATSz32;
	WORD BPB_ExtFlags;
	WORD BPB_FSVer;
	DWORD BPB_RootClus;
	WORD BPB_FSInfo;
	WORD BPB_BkBootSec;
	BYTE BPB_Reserved[12];
	BYTE BS_DrvNum;
	BYTE BS_Reserved1;
	BYTE BS_BootSig;
	DWORD BS_VolID;
	BYTE BS_VolLab[11];
	BYTE BS_FilSysType[8];
} PACKED;

struct FAT_BOOTSECTOR
{
	BYTE jmp[3];
	BYTE oem_id[8];
	WORD bytes_per_sector;
	BYTE sectors_per_cluster;
	WORD num_boot_sectors;
	BYTE num_fats;
	WORD num_root_dir_ents;
	WORD total_sectors;
	BYTE media_id_byte;
	WORD sectors_per_fat;
	WORD sectors_per_track;
	WORD heads;
	DWORD hidden_sectors;
	DWORD total_sectors_large;
	union
	{
		struct FAT_BOOTSECTOR16 bs16;
		struct FAT_BOOTSECTOR32 bs32;
		BYTE boot_code[474];
	};
	WORD magic;
} PACKED;

struct FAT_DOSTIME
{
	unsigned int twosecs:5;  /* low 5 bits: 2-second increments */
	unsigned int minutes:6;   /* middle 6 bits: minutes */
	unsigned int hours:5;     /* high 5 bits: hours (0-23) */
} PACKED;

struct FAT_DOSDATE
{
	unsigned int date:5;      /* low 5 bits: date (1-31) */
	unsigned int month:4;     /* middle 4 bits: month (1-12) */
	unsigned int year:7;      /* high 7 bits: year - 1980 */
} PACKED;  

struct FAT_ATTRIBUTE
{
	int readonly:1;
	int hidden:1;
	int system:1;
	int volumelabel:1;
	int directory:1;
	int archive:1;
	int reserved:2;
} PACKED;

struct FAT_ENTRY
{
	BYTE name[8];
	BYTE extention[3];
	struct FAT_ATTRIBUTE attribute;
	BYTE reserved[10];
	struct FAT_DOSTIME time;
	struct FAT_DOSDATE date;
	WORD start_cluster;
	DWORD file_size;
} PACKED;

struct FAT_MOUNTPOINT
{
	struct IO_HANDLE * device;
	struct FAT_BOOTSECTOR bootsector;
	BYTE type;
	BYTE * fat_data;
	int fat_size;
	int cluster_size;
};

int fat_mount( char * );

#endif
