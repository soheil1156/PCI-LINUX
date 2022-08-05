/*
 * rayaPhonePci.c
 *
 *  Created on: Jan 23, 2019
 *      Author: root
 */

#ifndef RAYAPHONEPCI_C_
#define RAYAPHONEPCI_C_

#include "rayaPhonePci.h"





int RPciDev_ScanPciBus(RPciDev *pRPciDev) {
	int i = 0;
	if (pRPciDev->lookupVendorID == -1)
		return -1;
	if (pRPciDev->lookupVendorID == -1)
		return -1;
	pRPciDev->pacc = pci_alloc();
	pci_init(pRPciDev->pacc);
	pci_scan_bus(pRPciDev->pacc);
	struct pci_dev *p;
	for (p = pRPciDev->pacc->devices; p; p = p->next) {
		if (p->device_id == pRPciDev->lookupDeviceID) {
			if (pRPciDev->numFinedDev == -1)
				pRPciDev->numFinedDev = 0;
			else
				pRPciDev->numFinedDev++;
			pRPciDev->dev[i] = p;
			i++;
		}

	}
	return 0;
}

void RPciDev_setLookupDeviceID(RPciDev *pRPciDev, uint16_t DeciceID) {
	pRPciDev->lookupDeviceID = DeciceID;
}
uint16_t RPciDev_getLookupDeviceID(RPciDev *pRPciDev) {
	return pRPciDev->lookupDeviceID;
}
void RPciDev_setLookupVendorID(RPciDev *pRPciDev, uint16_t VendorID) {
	pRPciDev->lookupVendorID = VendorID;
}
uint16_t RPciDev_getLookupVendorID(RPciDev *pRPciDev) {
	return pRPciDev->lookupVendorID;
}
uint16_t RPciDev_getTotalMachedDevNum(RPciDev *pRPciDev) {
	return pRPciDev->numFinedDev;
}

void_RPciDev_getDevice(RPciDev *pRPciDev, uint8_t DeviceNumber ,struct pci_dev *RPciDev) {
	RPciDev = pRPciDev->dev[DeviceNumber];

}

void RPhoneCons(RPhoneDev *pRPhoneDev, struct pci_dev *PciDevice) {

	pRPhoneDev->Init_flage = -1;
	pRPhoneDev->fd = -1;
	pRPhoneDev->virtAdd1 = (void *) -1;
	pRPhoneDev->virtAdd0 = (void *) -1;

	pRPhoneDev->dev = PciDevice;

}
int RPhone_Init(RPhoneDev *pRPhoneDev) {
	if ((pRPhoneDev->fd = open("/dev/mem", O_RDWR | O_SYNC  )) == -1) {
		pRPhoneDev->Init_flage = -1;
		return -1;
	}

	pRPhoneDev->virtAdd0 = mmap(0, 1048576UL, PROT_READ | PROT_WRITE,
			MAP_SHARED   , pRPhoneDev->fd, pRPhoneDev->dev->base_addr[0] & 0xFFFFFFF0);

	if (pRPhoneDev->virtAdd0 == (void *) -1)
	{
		  int errsv = errno;
		  //printf(stderr, "partial write");
		  printf("\n\rmmap() failed ErrNo %08d", errsv);
		return -1;
	}
	pRPhoneDev->virtAdd1 = mmap(0, 1048576UL, PROT_READ | PROT_WRITE,
			MAP_SHARED  , pRPhoneDev->fd, pRPhoneDev->dev->base_addr[1]);
	if (pRPhoneDev->virtAdd1 == (void *) -1)
		return -1;

	return 0;

}
int RPhone_DeInit(RPhoneDev *pRPhoneDev) {

	if(munmap(pRPhoneDev->virtAdd0, 1048576UL) == -1)
		return -1;
	if(munmap(pRPhoneDev->virtAdd1, 1048576UL) == -1)
			return -1;
	close(pRPhoneDev->fd);
	return 0;
}
uint32_t RPhone_getBAR0_PhyAdd(RPhoneDev *pRPhoneDev) {
	return pRPhoneDev->dev->base_addr[0];

}
uint32_t RPhone_getBAR1_PhyAdd(RPhoneDev *pRPhoneDev) {
	return pRPhoneDev->dev->base_addr[1];
}
void * RPhone_getBar0_VirAdd(RPhoneDev *pRPhoneDev) {
	return pRPhoneDev->virtAdd0;
}
void * RPhone_getBar1_VirAdd(RPhoneDev *pRPhoneDev) {
	return pRPhoneDev->virtAdd1;
}
void RPhone_setLed(RPhoneDev *pRPhoneDev, uint8_t ledState) {
	*(uint32_t *) (pRPhoneDev->virtAdd1) = ((uint32_t) (ledState & 0x03) << 24);
}
uint8_t RPhone_getLedState(RPhoneDev *pRPhoneDev) {
	return (((*(uint32_t *) (pRPhoneDev->virtAdd1)) >> 24) & 0x3);
}
void RPhone_setHPMem(RPhoneDev *pRPhoneDev, uint16_t offset, uint16_t BuffSize,
		uint32_t *buff) {
	uint16_t i;
	for (i = 0; i < BuffSize; i++) {
		(*(uint32_t *) (pRPhoneDev->virtAdd0 + offset + i * 4)) = *(buff + i);
	}
}
void RPhone_setHPMem64(RPhoneDev *pRPhoneDev, uint16_t offset, uint16_t BuffSize,
		uint32_t *buff) {
	uint16_t i;
	uint64_t dbuff;
	while (i < BuffSize) {
		if (i + 1 < BuffSize)
			(*(uint64_t *) (pRPhoneDev->virtAdd0 + offset + (i + 1) * 4)) =
					((*(buff + i) << 32) | *(buff + i + 1));
		else
			(*(uint32_t *) (pRPhoneDev->virtAdd0 + offset + i * 4)) =
					*(buff + i);
		i = i + 2;
	}
}
void RPhone_getHPMem(RPhoneDev *pRPhoneDev, uint16_t offset, uint16_t BuffSize,
		uint32_t *buff) {
	uint16_t i;

	for (i = 0; i < BuffSize; i++) {
		*(buff + i) = (*(uint32_t *) (pRPhoneDev->virtAdd0 + offset + i * 4));

	}
}

void RPhone_setHPMem8(RPhoneDev *pRPhoneDev, uint16_t offset, uint16_t BuffSize,
		uint8_t *buff) {
	uint16_t i;
	for (i = 0; i < BuffSize; i++) {
		(*(uint8_t *) (pRPhoneDev->virtAdd0 + offset + i)) = *(buff + i);
	}
}

void RPhone_getHPMem8(RPhoneDev *pRPhoneDev, uint16_t offset, uint16_t BuffSize,
		uint8_t *buff) {
	uint16_t i;

	for (i = 0; i < BuffSize; i++) {
		*(buff + i) = (*(uint8_t *) (pRPhoneDev->virtAdd0 + offset + i));

	}
}

uint16_t RPhone_getDbgFramen(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1004) & 0xffff);
}
uint16_t RPhone_getDbgDevseln(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1004) >> 16);
}
uint16_t RPhone_getDbgIrdyn(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1008) & 0xffff);
}
uint16_t RPhone_getDbgtrdyn(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1008) >> 16);
}
uint16_t RPhone_getDbgStopn(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x100C) & 0xffff);
}
uint16_t RPhone_getDbgidsel(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x100C) >> 16);
}
uint16_t RPhone_getDbgGntn(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1010) & 0xffff);
}
uint16_t RPhone_getDbgReqn(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1010) >> 16);
}
uint16_t RPhone_getDbgParn(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1014) & 0xffff);
}
uint16_t RPhone_getDbgPerrn(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1014) >> 16);
}
void RPhone_setDbgReset(RPhoneDev *pRPhoneDev, uint16_t resetVector) {
	(*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x1000)) = resetVector;
}
uint32_t RPhone_getIRQ(RPhoneDev *pRPhoneDev) {
	return (pRPhoneDev->dev->irq);
}
uint32_t RPhone_getF56ReadStat(RPhoneDev *pRPhoneDev) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2000));
}
/*
 CMD_COMMAND { cmd[7:0] ,add[7:0],data[15:0]};
 CMD :
 [7] falcReset        = 1
 [6] chipselect 0 = cs0 1 = cs1
 [5] reserved for other chip
 [4] reserved for other chip
 [3] read = 0 / write = 1
 [2] address re latch = 1
 [1] reserved
 [0] reserved

 F56_REG_ADD [7:0] = FL56_CMD[23:16];
 F56_REG_WDATA[15:0] = FL56_CMD[15:0];
 F56_REG_CMD[7:0] = FL56_CMD[31:24];

 0:	WB_DATA_O = {FL56_STATE[15:0],WRITE_COUNTER[7:0],READ_COUNTER[7:0]};
 F56_RSTn_O = F56_REG_WDATA[0] ;


 0x0C		 FL56_CMD

 */
void RPhone_settF56ResetStat(RPhoneDev *pRPhoneDev, uint8_t Reset) {

	if (Reset == 0)
		(*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2008)) = 0x80000001;
	else
		(*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2008)) = 0x80000000;

}
uint8_t RPhone_gettF56ResetStat(RPhoneDev *pRPhoneDev) {
	if (((*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2000)) & 0x01000000) == 0)
		return 0;
	else
		return 1;
}
uint32_t RPhone_getF56AddRegister(RPhoneDev *pRPhoneDev,
		uint8_t regAdd, uint8_t chipSel) {
	uint16_t nIdleRead = 0;
	uint32_t dummy;
	if (chipSel == 0)
		(*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2008)) = ((uint32_t) regAdd)
				<< 16 | 0x1 << 26;
	else if (chipSel == 1)
		(*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2008)) = ((uint32_t) regAdd)
				<< 16 | 0x1 << 26 | 0x1 << 30;
	else
		return 0xFFFFFFFF;
	//readstate

	usleep(1);
	while (0x02000000 & (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2000)) == 0)
		nIdleRead++;
	//return 0;
	return ((*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2004)) | nIdleRead << 16);

}
uint32_t RPhone_setF56AddRegister(RPhoneDev *pRPhoneDev, uint8_t regAdd,
		uint8_t chipSel, uint8_t data) {
	uint16_t nIdleRead = 0;
	if (chipSel == 0)
		(*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2008)) = ((uint32_t) regAdd)
				<< 16 | 0x3 << 26 | data;
	else if (chipSel == 1)
		(*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2008)) = ((uint32_t) regAdd)
				<< 16 | 0x3 << 26 | 0x1 << 30 | data;
	else
		return 0xFFFFFFFF;
	//readstate
	usleep(1);
	while (0x00010000 & (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2000)) == 0)
		nIdleRead++;
	return ((*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2004)) | nIdleRead << 16);
}
uint32_t RPhone_getF56ReadCapture(RPhoneDev *pRPhoneDev, uint8_t Capture) {
	return (*(uint32_t *) (pRPhoneDev->virtAdd1 + 0x2000 + Capture));
}
#endif /* RAYAPHONEPCI_C_ */
