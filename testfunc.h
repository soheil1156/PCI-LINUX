/*
 * testfunc.h
 *
 *  Created on: Jan 23, 2019
 *      Author: root
 */

#ifndef TESTFUNC_H_
#define TESTFUNC_H_

#include <stdint.h>
#include <string.h>

void printBinary(unsigned int myNumber);
int memCopy8Test(RPhoneDev *RPhoneDev, uint8_t printTest, uint8_t printTime);
int memCopy16Test(RPhoneDev *RPhoneDev, uint8_t printTest, uint8_t printTime);
int memCopy32Test(RPhoneDev *RPhoneDev, uint8_t printTest, uint8_t printTime);
int memCopy64Test(RPhoneDev *RPhoneDev, uint8_t printTest, uint8_t printTime);


#endif /* TESTFUNC_H_ */
