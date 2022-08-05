/*
 * testfunc.c
 *
 *  Created on: Jan 23, 2019
 *      Author: root
 */
#include <stdint.h>
#include <stdlib.h>
#include "rayaPhonePci.h"

int debug_test_counter = 0;
#define Debug_test(args...) {debug_test_counter ++; printf("\r#DEBUG TEST-%03d\t",debug_test_counter);printf(args) ;}

void printBinary(unsigned int myNumber)
{
    int numberOfBits = sizeof(unsigned int)*8;
    for (int i=numberOfBits-1; i>=0; i--) {
        uint32_t isBitSet = (myNumber & (1<<i));
        if (isBitSet == 0) {
            printf("  0");
        } else {
        	printf("  1");
        }
    }
}


int memCopy8Test(RPhoneDev *RPhoneDev, uint8_t printTest, uint8_t printTime) {
	uint32_t err;
	uint32_t buff[4096], buff2[4096];
	uint8_t buff8[4096 * 4], buff8_2[4096 * 4];
	struct timeval tv1Write, tv2Write, tv1Read, tv2Read;
	for (int i = 0; i < 4 * 4096; i++) {
		buff8[i] = rand();
		buff2[i / 4] = i;
	}
	RPhone_setHPMem(RPhoneDev,0, 4096, buff2);
	gettimeofday(&tv1Write, NULL);
	memcpy(RPhone_getBar0_VirAdd(RPhoneDev), buff8, 1024 * 4 * 4);
	gettimeofday(&tv2Write, NULL);

	RPhone_getHPMem(RPhoneDev,0, 4096, buff);

	gettimeofday(&tv1Read, NULL);
	memcpy(buff8_2, RPhone_getBar0_VirAdd(RPhoneDev), 1024 * 4 * 4);
	gettimeofday(&tv2Read, NULL);
	if (printTest) {
		for (int i = 0; i < 1024 * 4 * 4; i++) {
			if (i % 4 == 0)
				printf("\r\n");
			printf("%05d ---- %08x = %08x  ", i, buff8[i], buff8_2[i]);
			if (i % 4 == 0)
				printf("--- BUF32 = %08x ", buff[i / 4]);
			if (buff8[i] != buff8_2[i]) {
				err++;
				printf(" ***  \r\n");
			} else
				printf("\r\n");
		}
		Debug_test("nmemcopy 1byte error ---- %08d\r\n", err);
	}
	if (printTime) {
		Debug_test("MEMCOPY 1BYTE WRITE TIME = %f seconds\n",
				(double) (tv2Write.tv_usec - tv1Write.tv_usec) / 1000000
						+ (double) (tv2Write.tv_sec - tv1Write.tv_sec));
		Debug_test("MEMCOPY 1BYTE READ TIME = %f seconds\n",
				(double) (tv2Read.tv_usec - tv1Read.tv_usec) / 1000000
						+ (double) (tv2Read.tv_sec - tv1Read.tv_sec));
	}

	return err;
}


int memCopy16Test(RPhoneDev *RPhoneDev, uint8_t printTest, uint8_t printTime) {
	uint32_t err;
	uint32_t buff[4096], buff2[4096];
	uint16_t buff16[4096 * 2], buff16_2[4096 * 2];
	struct timeval tv1Write, tv2Write, tv1Read, tv2Read;
	for (int i = 0; i < 2 * 4096; i++) {
		buff16[i] = rand();
		buff2[i / 2] = i;
	}
	RPhone_setHPMem(RPhoneDev,0, 4096, buff2);
	gettimeofday(&tv1Write, NULL);
	memcpy(RPhone_getBar0_VirAdd(RPhoneDev), buff16, 1024 * 4 * 4);
	gettimeofday(&tv2Write, NULL);

	gettimeofday(&tv1Read, NULL);
	memcpy(buff16_2, RPhone_getBar0_VirAdd(RPhoneDev), 1024 * 4 * 4);
	gettimeofday(&tv2Read, NULL);
	RPhone_getHPMem(RPhoneDev,0, 4096, buff);
	if (printTest) {
		for (int i = 0; i < 1024 * 4 * 2; i++) {
			if (i % 2 == 0)
				printf("\r\n");
			printf("%05d ---- %08x = %08x  ", i, buff16[i], buff16_2[i]);
			if (i % 2 == 0)
				printf("--- BUF32 = %08x ", buff[i / 2]);
			if (buff16[i] != buff16_2[i]) {
				err++;
				printf(" ***  \r\n");
			} else
				printf("\r\n");
		}
		Debug_test("\r\nmemcopy 2byte error ---- %08d\r\n", err);
	}
	if (printTime) {
		Debug_test("MEMCOPY 2BYTE WRITE TIME = %f seconds\n",
				(double) (tv2Write.tv_usec - tv1Write.tv_usec) / 1000000
						+ (double) (tv2Write.tv_sec - tv1Write.tv_sec));
		Debug_test("MEMCOPY 2BYTE READ TIME = %f seconds\n",
				(double) (tv2Read.tv_usec - tv1Read.tv_usec) / 1000000
						+ (double) (tv2Read.tv_sec - tv1Read.tv_sec));
	}

	return err;
}
int memCopy32Test(RPhoneDev *RPhoneDev, uint8_t printTest, uint8_t printTime) {
	uint32_t err;
	uint32_t buff[4096], buff2[4096];
	uint32_t buff32[4096], buff32_2[4096];
	struct timeval tv1Write, tv2Write, tv1Read, tv2Read;
	for (int i = 0; i < 1 * 4096; i++) {
		buff32[i] = rand();
		buff2[i] = i;
	}
	RPhone_setHPMem(RPhoneDev,0, 4096, buff2);
	gettimeofday(&tv1Write, NULL);
	memcpy(RPhone_getBar0_VirAdd(RPhoneDev), buff32, 1024 * 4 * 4);
	gettimeofday(&tv2Write, NULL);

	gettimeofday(&tv1Read, NULL);
	memcpy(buff32_2, RPhone_getBar0_VirAdd(RPhoneDev), 1024 * 4 * 4);
	gettimeofday(&tv2Read, NULL);
	RPhone_getHPMem(RPhoneDev,0, 4096, buff);
	if (printTest) {
		for (int i = 0; i < 1024 * 4 * 1; i++) {
			//if(i%2 == 0)
			printf("\r\n");
			printf("%05d ---- %08x = %08x  ", i, buff32[i], buff32_2[i]);
			//if(i%2 == 0)
			printf("--- BUF32 = %08x ", buff[i]);
			if (buff32[i] != buff32_2[i]) {
				err++;
				printf(" ***  \r\n");
			} else
				printf("\r\n");
		}
		Debug_test("\r\nmemcopy 4byte error ---- %08d\r\n", err);
	}
	if (printTime) {
		Debug_test("MEMCOPY 4BYTE WRITE TIME = %f seconds\n",
				(double) (tv2Write.tv_usec - tv1Write.tv_usec) / 1000000
						+ (double) (tv2Write.tv_sec - tv1Write.tv_sec));
		Debug_test("MEMCOPY 4BYTE READ TIME = %f seconds\n",
				(double) (tv2Read.tv_usec - tv1Read.tv_usec) / 1000000
						+ (double) (tv2Read.tv_sec - tv1Read.tv_sec));
	}

	return err;
}

int memCopy64Test(RPhoneDev *RPhoneDev, uint8_t printTest, uint8_t printTime) {
	uint32_t err;
	uint32_t buff[4096], buff2[4096];
	uint64_t buff64[4096 / 2], buff64_2[4096 / 2];
	struct timeval tv1Write, tv2Write, tv1Read, tv2Read;
	for (int i = 0; i < 4096 / 2; i++) {
		buff64[i] = rand();
		buff2[i] = i;
		buff2[2 * i] = i;
	}
	RPhone_setHPMem(RPhoneDev,0, 4096, buff2);
	gettimeofday(&tv1Write, NULL);
	memcpy(RPhone_getBar0_VirAdd(RPhoneDev), buff64, 1024 * 4 * 4);
	gettimeofday(&tv2Write, NULL);

	gettimeofday(&tv1Read, NULL);
	memcpy(buff64_2, RPhone_getBar0_VirAdd(RPhoneDev), 1024 * 4 * 4);
	gettimeofday(&tv2Read, NULL);
	RPhone_getHPMem(RPhoneDev,0, 4096, buff);
	if (printTest) {
		for (int i = 0; i < 1024 * 4 * 1; i = i + 2) {
			//if(i%2 == 0)
			printf("\r\n");
			printf("%05d ---- %08x = %08x  ", i, buff64[i / 2],
					buff64_2[i / 2]);
			//if(i%2 == 0)
			printf("--- BUF32[n] BUF32[n+1]  = %08x  %08x", buff[i],
					buff[i + 1]);
			if (buff64[i / 2] != buff64_2[i / 2]) {
				err++;
				printf(" ***  \r\n");
			} else
				printf("\r\n");
		}
		Debug_test("\r\nmemcopy 4byte error ---- %08d\r\n", err);
	}
	if (printTime) {
		Debug_test("MEMCOPY 4BYTE WRITE TIME = %f seconds\n",
				(double) (tv2Write.tv_usec - tv1Write.tv_usec) / 1000000
						+ (double) (tv2Write.tv_sec - tv1Write.tv_sec));
		Debug_test("MEMCOPY 4BYTE READ TIME = %f seconds\n",
				(double) (tv2Read.tv_usec - tv1Read.tv_usec) / 1000000
						+ (double) (tv2Read.tv_sec - tv1Read.tv_sec));
	}

	return err;
}
