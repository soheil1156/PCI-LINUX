/*
 * main.c
 *
 *  Created on: Jan 23, 2019
 *      Author: root
 */

#include <pci/pci.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rayaPhonePci.h"
#include "testfunc.h"

int debug_counter = 0;
#define Debug(args...) {debug_counter ++; printf("\r\n#DEBUG-%03d\t",debug_counter);printf(args) ;}

#define Debug_sec(args...)  {printf("\r\n*************************************************\r\n*\r\n* DEBUG\t");printf(args);\
							printf("\r\n*\r\n*************************************************");}

int main(int argc, char **argv) {

	Debug_sec("SYSTEM INIT");
	Debug("SYSTEM START");

	RPciDev *RPciDev0 = (RPciDev *) malloc(sizeof(RPciDev));
	RPciDev_setLookupDeviceID(RPciDev0, 0xbbcc);
	RPciDev_setLookupVendorID(RPciDev0, 0x00aa);
	RPciDev_ScanPciBus(RPciDev0);

	Debug("TOTAL_DEVIC_FIND = %03d", RPciDev_getTotalMachedDevNum(RPciDev0));
	if (RPciDev_getTotalMachedDevNum(RPciDev0) == 0) {
		Debug("\r\nNO DEVICE FIND TERMINATE\r\n");
		return -1;
	}

	RPhoneDev *RPhoneDev0 = (RPhoneDev *) malloc(sizeof(RPhoneDev));
	RPhoneCons(RPhoneDev0, RPciDev0->dev[0]);
	RPhone_Init(RPhoneDev0);
	Debug("INIT DONE");
	Debug("PHYSIC_ADD0 = %08x ", RPhone_getBAR0_PhyAdd(RPhoneDev0));
	Debug("PHYSIC_ADD1 = %08x ", RPhone_getBAR1_PhyAdd(RPhoneDev0));
	Debug("VIRT_ADD0 = %08x ", RPhone_getBar0_VirAdd(RPhoneDev0));
	Debug("VIRT_ADD1 = %08x ", RPhone_getBar1_VirAdd(RPhoneDev0));

	Debug_sec("LED TEST");

	for (int i = 0; i < 3; i++) {
		usleep(100000);
		Debug("LED STATE = %01d", RPhone_getLedState(RPhoneDev0));
		RPhone_setLed(RPhoneDev0, 0x3);
		usleep(100000);
		Debug("LED STATE = %01d", RPhone_getLedState(RPhoneDev0));
		RPhone_setLed(RPhoneDev0, 0x0);
	}
	Debug_sec("MEM  TEST");
	if (memCopy8Test(RPhoneDev0, 1, 1) == 0)
		Debug("MEMCOPY 8 SUCCESS\r\n");
	if (memCopy16Test(RPhoneDev0, 0, 1) == 0)
		Debug("MEMCOPY 16 SUCCESS\r\n");
	if (memCopy32Test(RPhoneDev0, 0, 1) == 0)
		Debug("MEMCOPY 32 SUCCESS\r\n");
	if (memCopy64Test(RPhoneDev0, 0, 1) == 0)
		Debug("MEMCOPY 64 SUCCESS\r\n");

	Debug_sec("MEM TEST2");
	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	{
		uint8_t buff8_2[4096 * 4];
		memcpy(buff8_2, RPhone_getBar0_VirAdd(RPhoneDev0), 1024 * 4 * 4);
	}
	Debug(
			"READ 8bit MEM Num Frame Asserted  ---- %08d  Irdyn Asserted  ---- %08d\r\n",
			RPhone_getDbgFramen(RPhoneDev0), RPhone_getDbgIrdyn(RPhoneDev0));
	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	{
		uint32_t buff8_2[4096 * 4];
		memcpy(RPhone_getBar0_VirAdd(RPhoneDev0), buff8_2, 1024 * 4 * 4);
	}

	Debug(
			"WRITE 8bit MEM Num Frame Asserted  ---- %08d  Irdyn Asserted  ---- %08d\r\n",
			RPhone_getDbgFramen(RPhoneDev0), RPhone_getDbgIrdyn(RPhoneDev0));

	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	{
		uint16_t buff16_2[4096 * 2];
		memcpy(buff16_2, RPhone_getBar0_VirAdd(RPhoneDev0), 1024 * 4 * 4);
	}

	Debug(
			"READ 16bit MEM Num Frame Asserted  ---- %08d  Irdyn Asserted  ---- %08d\r\n",
			RPhone_getDbgFramen(RPhoneDev0), RPhone_getDbgIrdyn(RPhoneDev0));
	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	{
		uint32_t buff16_2[4096 * 2];
		memcpy(RPhone_getBar0_VirAdd(RPhoneDev0), buff16_2, 1024 * 4 * 4);
	}

	Debug(
			"WRITE 16bit MEM Num Frame Asserted  ---- %08d  Irdyn Asserted  ---- %08d\r\n",
			RPhone_getDbgFramen(RPhoneDev0), RPhone_getDbgIrdyn(RPhoneDev0));

	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	{
		uint32_t buff32_2[4096];
		memcpy(buff32_2, RPhone_getBar0_VirAdd(RPhoneDev0), 1024 * 4 * 4);
	}

	Debug(
			"READ 32bit MEM Num Frame Asserted  ---- %08d  Irdyn Asserted  ---- %08d\r\n",
			RPhone_getDbgFramen(RPhoneDev0), RPhone_getDbgIrdyn(RPhoneDev0));
	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	{
		uint32_t buff32_2[4096];
		memcpy(RPhone_getBar0_VirAdd(RPhoneDev0), buff32_2, 1024 * 4 * 4);
	}

	Debug(
			"WRITE 32bit MEM Num Frame Asserted  ---- %08d  Irdyn Asserted  ---- %08d\r\n",
			RPhone_getDbgFramen(RPhoneDev0), RPhone_getDbgIrdyn(RPhoneDev0));

	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	{
		uint64_t buff64_2[4096 / 2];
		memcpy(buff64_2, RPhone_getBar0_VirAdd(RPhoneDev0), 1024 * 4 * 4);

	}

	Debug(
			"READ 64bit MEM Num Frame Asserted  ---- %08d  Irdyn Asserted  ---- %08d\r\n",
			RPhone_getDbgFramen(RPhoneDev0), RPhone_getDbgIrdyn(RPhoneDev0));
	RPhone_setDbgReset(RPhoneDev0, 0xffff);

	{
		uint64_t buff64_2[4096 / 2];
		memcpy(RPhone_getBar0_VirAdd(RPhoneDev0), buff64_2, 1024 * 4 * 4);
	}

	Debug(
			"WRITE 64bit MEM Num Frame Asserted  ---- %08d  Irdyn Asserted  ---- %08d\r\n",
			RPhone_getDbgFramen(RPhoneDev0), RPhone_getDbgIrdyn(RPhoneDev0));

	Debug("IRQ = %08x \r\n", RPhone_getIRQ(RPhoneDev0));

	Debug_sec("FALC TEST");

	Debug("STATE = %08x \r\n", RPhone_getF56ReadStat(RPhoneDev0));
	RPhone_settF56ResetStat(RPhoneDev0, 1);
	Debug("STATE = %08x \r\n", RPhone_getF56ReadStat(RPhoneDev0));
	Debug("RESET_STATE = %s \r\n",
			RPhone_gettF56ResetStat(RPhoneDev0) ? "True" : "False");
	usleep(1000);
	RPhone_settF56ResetStat(RPhoneDev0, 0);
	usleep(1000);
	Debug("STATE = %08x \r\n", RPhone_getF56ReadStat(RPhoneDev0));
	Debug("RESET_STATE = %s \r\n",
			RPhone_gettF56ResetStat(RPhoneDev0) ? "True" : "False");

	RPhone_setF56AddRegister(RPhoneDev0, 0x03, 1, 0x00);

	Debug("READ_REGISTER_0x04 = %08x \r\n",
			RPhone_getF56AddRegister(RPhoneDev0, 0x03, 1));
	Debug("STATE = %08x \r\n", RPhone_getF56ReadStat(RPhoneDev0));

	Debug_sec("FALC TEST2");




	Debug_sec("F56 ADD MODE TEST BEGIN");

		usleep(1);
		uint32_t k =  *(uint32_t *)(RPhone_getBar1_VirAdd(RPhoneDev0)+0x4000 + 20  );
		#define READ_DEBUG(x)  *(uint32_t *)(RPhone_getBar1_VirAdd(RPhoneDev0)+0x4800 + x * 4  )
		#define READ_ADD(x)  *(uint32_t *)(RPhone_getBar1_VirAdd(RPhoneDev0) + x )
		#define READ_BRAM(x)  *(uint32_t *)(RPhone_getBar0_VirAdd(RPhoneDev0) + x )

		*(uint32_t *)(RPhone_getBar1_VirAdd(RPhoneDev0)+0x4000 + 16  ) = 0x0A;


		Debug("\r\n\r\n37 - READ_REGISTER_0x05_NORMAL_MODE = %08x \r\n\r\n", RPhone_getF56AddRegister(RPhoneDev0,0x04,0));

		Debug("\r\n\r\n38 - READ_REGISTER_CHECK_DEBUG = %08x \r\n\r\n",  READ_DEBUG(11) );

		Debug("GNT :\r\n \t\t" );
		printBinary(READ_DEBUG(1));


		Debug("REQ :\r\n \t\t" );
		printBinary(READ_DEBUG(2));



		Debug("ALE :\r\n \t\t" );
		printBinary(READ_DEBUG(3));

		Debug("DIR :\r\n \t\t" );
		printBinary(READ_DEBUG(4));

		Debug("RDn :\r\n \t\t" );
		printBinary(READ_DEBUG(5));

		Debug("WRn :\r\n \t\t" );
		printBinary(READ_DEBUG(6));

		Debug("CS0n :\r\n \t\t" );
		printBinary(READ_DEBUG(7));

		Debug("CS1n :\r\n \t\t" );
		printBinary(READ_DEBUG(8));


		Debug("DATA :\r\n \t\t" );
		for (int i = 32; i> 0 ; i--){
			READ_DEBUG(10) = i-1;
			printf(" %02x",READ_DEBUG(9));
		}


		Debug("RDAT :\r\n \t\t" );
		for (int i = 32; i> 0 ; i--){
			/*int j;
			j = READ_DEBUG(9);
			if (j == 99)
				break;*/
			READ_DEBUG(10) = i;
			printf(" %02x",READ_DEBUG(10));
		}

		uint32_t buf_falc[256];
		for (int i = 0 ; i < 256 ; i++){
			buf_falc[i] = *(uint32_t *)(RPhone_getBar1_VirAdd(RPhoneDev0)+0x4200 + i *4);

		}


		Debug_sec(" BUF FALC  ");

		for (int i = 0 ; i < 256 ; i++){
				buf_falc[i] = *(uint32_t *)(RPhone_getBar1_VirAdd(RPhoneDev0)+0x4000 + i *4);

			}
		Debug("DEFULT FALC DAT :\r\n" );
		for (int i = 0 ; i < 256 ; i++){
			if (i % 5 == 0)
				printf("\r\n");
			printf("%03x - %03x\t" ,i,buf_falc[i]);

				}

		Debug_sec(" TYPE01 FULC  ");
		for (int i = 0 ; i < 256 ; i++){
				buf_falc[i] =(RPhone_getF56AddRegister(RPhoneDev0,i,0));

			}
		Debug("DEFULT FALC DAT :\r\n" );
		for (int i = 0 ; i < 256 ; i++){
			if (i % 5 == 0)
				printf("\r\n");
			printf("%03x - %03x\t" ,i,buf_falc[i]);

				}



		Debug_sec("MEMORY SEQUNCER READ ");
		Debug("ADD 0x8000 = %08x",READ_ADD(0x8000) );
		Debug("ADD 0x8004 = %08d",READ_ADD(0x8004) );
		Debug("ADD 0x8008 = %08x",READ_ADD(0x8008) );
		Debug("ADD 0x800C = %08x",READ_ADD(0x800C) );
		Debug("ADD 0x8010 = %08x",READ_ADD(0x8010) );

		READ_ADD(0x8000) = 0;
		Debug_sec("MEMORY SEQUNCER STOP ");
		Debug("ADD 0x8000 = %08x",READ_ADD(0x8000) );
		Debug("ADD 0x8004 = %08d",READ_ADD(0x8004) );
		Debug("ADD 0x8008 = %08x",READ_ADD(0x8008) );
		Debug("ADD 0x800C = %08x",READ_ADD(0x800C) );
		Debug("ADD 0x8010 = %08x",READ_ADD(0x8010) );

		for(int i = 0; i < 10;i++)
		{
		Debug("MEMORY ACTIVE = %01x",READ_ADD(0x8008) );
		//Debug("ADD 0x8010 = %08d",READ_ADD(0x8010) );
		usleep(129);
		}
		Debug("READ BRAM0 = %01x",READ_BRAM(5));

		RPhone_settF56ResetStat(RPhoneDev0,0);
		Debug("SUCCSESS \r\n");


		RPhone_DeInit(RPhoneDev0);
		Debug("RPHONE DEINIT \r\n");
}

