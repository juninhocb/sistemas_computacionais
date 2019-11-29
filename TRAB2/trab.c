#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
struct FAT32_volume_ID{
unsigned char unused1[11];
unsigned short BPB_BytsPerSec; /* Bytes Per Sector   - offset 0x0B - 16 Bits - Always 512 Bytes */
unsigned char BPB_SecPerClus; /* Sectors Per Cluster - offset 0x0D - 8 Bits - 1,2,4,8,16,32,64,128 */
unsigned short BPB_RsvdSecCnt; /* # Reserved Sectors - offset 0x0E - 16 Bits - Usually 0x20 */
unsigned char BPB_NumFATs; /* Number of FATs - offset 0x10 - 8 Bits - Always 2 */
unsigned char unused2[19];
unsigned int BPB_FATSz32;    /* Sectors Per FAT - offset 0x24 - 32 Bits - Depends on disk size */
unsigned char unused3[4];
unsigned int BPB_RootClus; /* Root Directory - offset 0x2C - 32 Bits - Usually 0x00000002 */
unsigned char   unused4[23];
unsigned char BPB_Label[11]; /* Volume label - ofsset 0x47 - 12 bytes    -   String name */
unsigned char BPB_Fat[8]; /* Type of fat - ofsset 0x52 - 8 bytes     -   String name */
unsigned char   unused5[420];
unsigned short  signature; /* Signature (none) - offset 0x1FE - 16 Bits - Always 0xAA55 */
};

struct directory_entry{
unsigned char  DIR_Name[8]; /* Short Filename 0x00 11 Bytes*/
unsigned char  File_ext[3]; /* File Extension 0x00 11 Bytes*/
unsigned char  Attrib; /* Byte DIR_Attr 0x0B 8 Bits */
unsigned char  unused1[8];
unsigned short DIR_FstClusHI; /* First Cluster High 0x14 16 Bits */
unsigned char  unused2[4];
unsigned short DIR_FstClusLO; /* First Cluster Low 0x1A 16 Bits */
unsigned int   DIR_FileSize; /* File Size 0x1C 32 Bits*/
};
#pragma pack(pop)
FILE* dev;
/*
Attrib mean:
0x01: read only
0x02: hidden
0x04: system
0x08: volume label
0x10: directory
0x20: archive
*/


struct directory_table{
struct directory_entry entry[16];
};


struct FAT32_volume_ID FAT32_vol;
struct directory_table dt;


/** Print caracters. */
void print_name(unsigned char *n, int sz){
int i;

for(i=0; i<sz; i++){
printf("%c", n[i]);
}

printf("\n");
}


/* Print FAT32 voluem ID. */
void print_FAT32_volume_ID(struct FAT32_volume_ID *ft){
printf("--------------- FAT32 Volume ID --------------------\n");
printf("Volume name: ");
print_name(ft->BPB_Label, 11);
printf("FAT Type: ");
print_name(ft->BPB_Fat, 8);
printf("Bytes Per Sector: %i\n", ft->BPB_BytsPerSec);
printf("Sectors Per Cluster: %i\n", ft->BPB_SecPerClus);
printf("Number of Reserved Sectors: %i\n", ft->BPB_RsvdSecCnt);
printf("Number of FATs: %i\n", ft->BPB_NumFATs);
printf("Sectors Per FAT: %i\n", ft->BPB_FATSz32);
printf("Root Directory: %i\n", ft->BPB_RootClus);
printf("Signature: 0x%x\n", ft->signature);
printf("----------------------------------------------------\n");
}


/** Print directory table entry . */
void print_dir_entry_name(struct directory_entry *de){
int i;

if(de->Attrib & 0x10){
printf("[Diretório]- ");
}else{
printf("[Arquivo]-");
}

for(i=0; i<8 && de->DIR_Name[i] != '~' && de->DIR_Name[i] != ' '; i++){
printf("%c", de->DIR_Name[i]);
}

if(de->Attrib & 0x10){
printf("\n");
return;
}

printf(".");

for(i=0; i<3; i++){
printf("%c", de->File_ext[i]);
}

printf(" %d\n", de->DIR_FileSize);

}

struct directory_table dts[16]; //data save
char key[512];

/* Printf all valid entries in a directory table. */
void print_dir(struct directory_table *dt){
int i;
unsigned int dir_pos;
struct directory_table dtl;


for(i=0; i<16; i++){
if( dt->entry[i].DIR_Name[0] != 0 &&
dt->entry[i].DIR_Name[0] != 0xE5 &&
dt->entry[i].DIR_Name[0] != 0x2E){

if(dt->entry[i].DIR_Name[0] == 0x5){
dt->entry[i].DIR_Name[0] = 0xE5;
}

if( (dt->entry[i].Attrib & 0x8) || (dt->entry[i].Attrib & 0x4)){
continue;
}
	print_dir_entry_name(&dt->entry[i]);
	if (dt->entry[i].Attrib & 0x10) { //se for diretório
		dir_pos = dt->entry[i].DIR_FstClusHI<<16 | dt->entry[i].DIR_FstClusLO;	
            	dir_pos =  (FAT32_vol.BPB_RsvdSecCnt + (FAT32_vol.BPB_NumFATs * FAT32_vol.BPB_FATSz32) + (dir_pos - 2)) * FAT32_vol.BPB_BytsPerSec;
            	read_data(dev, dir_pos, &dtl, 512);
		dts[i] = dtl;
            	print_dir(&dtl); //chama recursivamente
		if(procura_removidos(key))
		{
			print_dir(retorno);
			//mkdir estrutura
		}
			

            }}
            }

}

void procura_removidos(char key){
	
	for (int i; i < 16; i++){
		if(key == dts->entry[i].DIR_Name[0]){
			return dts->entry[i];
		}

				}
	return 0;
}
/*
Attrib mean:
0x01: read only
0x02: hidden
0x04: system
0x08: volume label
0x10: directory
0x20: archive
*/

/* Read a chunk of data. */
int read_data(FILE* f, unsigned int pos, void *bf, unsigned int sz){
int ret;

if (fseek(f, pos, SEEK_SET)){
perror("fseek()");
}

ret = fread(bf, 1, sz, f);
return ret;
}


/* Calc a cluster position in the disk. */
unsigned int calc_cluster_pos(struct FAT32_volume_ID *ft, unsigned int cluster){
return (ft->BPB_RsvdSecCnt + (ft->BPB_NumFATs * ft->BPB_FATSz32) + (cluster - 2)) * ft->BPB_BytsPerSec;
}

int main(int argc, char** argv){

unsigned int fat1_pos, root_dir_pos;
unsigned int *fat1, fatsz;
int opt;

printf("Digite 1 para recuperar um aqrquivo: \n");
printf("Digite 2 para listar os diretórios e arquivos: \n");
printf("Opção: ");
scanf("%d", opt);


if(argc < 2){
printf("USAGE: %s <filename>\n", argv[0]);
}

if( (dev = fopen(argv[1], "r")) == NULL){
perror("fopen()");
return -1;
}

printf("Digite 1 para recuperar um aqrquivo: \n");
printf("Digite 2 para listar os diretórios e arquivos: \n");
printf("Opção: ");
scanf("%d", opt);

switch(opt){

	case 1: 
		printf("Digite o nome do Arquivo: ");
		gets(key);
		printf("Arquivo a ser procurado ");
		puts(key);
		break;
	case 2: 
		break;

	default:
		printf("Número inválido");

}

/*Read FAT32 Volume ID. */
fread(&FAT32_vol, 1, 512, dev);

print_FAT32_volume_ID(&FAT32_vol);
 
if(strncmp((char*)FAT32_vol.BPB_Fat, "FAT32", 5)){
printf("File system not suported.\n");
return 0;
}

/* Basic variables to access the file system.

  This numbers are giving in cluster number, to use fseek()/fread() you
  need to multiply them by BPB_BytsPerSec.
*/

/* Fat1 offset */
fat1_pos = FAT32_vol.BPB_RsvdSecCnt * FAT32_vol.BPB_BytsPerSec;

/* Fat size */
fatsz = FAT32_vol.BPB_FATSz32 * FAT32_vol.BPB_BytsPerSec;
fat1 = malloc(fatsz);

/* Root Dir offset */
root_dir_pos = calc_cluster_pos(&FAT32_vol, FAT32_vol.BPB_RootClus);

printf("\n VALOR DO CLUSTER DO ROOT: %ld \n", sizeof(root_dir_pos));

printf("\n\n");
printf("FAT1 offset: 0x%x\n", fat1_pos);
printf("FAT size: %d bytes\n", fatsz);
printf("Root Dir offset: 0x%x\n", root_dir_pos);

/* Read fat1 table - Read only the first sector of FAT. The FAT is BPB_FATSz32 sectors. */
read_data(dev, fat1_pos, fat1, fatsz);

/* Read root dir table */
read_data(dev, root_dir_pos, &dt, 512);

/* Show root directory */
printf("\n\nROOT DIR: \n");
int enderecoraiz = root_dir_pos;
print_dir(&dt);

free(fat1);
fclose(dev);

return 0;
}
