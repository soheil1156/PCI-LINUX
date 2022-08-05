/*
 * rayaPhonePci.h
 *
 *  Created on: Jan 23, 2019
 *      Author: root
 */

#ifndef RAYAPHONEPCI_H_
#define RAYAPHONEPCI_H_

#include <pci/pci.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


typedef struct {
	int lookupDeviceID;
	int lookupVendorID ;

	struct pci_access *pacc;
	struct device *pdev;
	struct pci_dev *dev[64];
	int numFinedDev ;

}
RPciDev;


int RPciDev_ScanPciBus(RPciDev *pRPciDev);

void RPciDev_setLookupDeviceID(RPciDev *pRPciDev,uint16_t DeciceID);
uint16_t RPciDev_getLookupDeviceID(RPciDev *pRPciDev);
void RPciDev_setLookupVendorID(RPciDev *pRPciDev,uint16_t VendorID) ;
uint16_t RPciDev_getLookupVendorID(RPciDev *pRPciDev);
uint16_t RPciDev_getTotalMachedDevNum(RPciDev *pRPciDev);
void RPciDev_getDevice(RPciDev *pRPciDev,uint8_t DeviceNumber);


typedef struct {

	struct pci_dev *dev;
	int Init_flage;
	int fd;
	void *virtAdd0, *virtAdd1;

}
RPhoneDev;

void 	RPhoneCons(RPhoneDev *pRPhoneDev,struct pci_dev *PciDevice);

int RPhone_Init(RPhoneDev *pRPhoneDev);
int RPhone_DeInit(RPhoneDev *pRPhoneDev);
	uint32_t RPhone_getBAR0_PhyAdd(RPhoneDev *pRPhoneDev);
	uint32_t RPhone_getBAR1_PhyAdd(RPhoneDev *pRPhoneDev);
	void * RPhone_getBar0_VirAdd(RPhoneDev *pRPhoneDev);
	void * RPhone_getBar1_VirAdd(RPhoneDev *pRPhoneDev);
	void RPhone_setLed(RPhoneDev *pRPhoneDev,uint8_t ledState);
	void RPhone_setHPMem(RPhoneDev *pRPhoneDev,uint16_t offset, uint16_t BuffSize, uint32_t *buff);
	void RPhone_setHPMem64(RPhoneDev *pRPhoneDev,uint16_t offset, uint16_t BuffSize, uint32_t *buff);
	void RPhone_getHPMem(RPhoneDev *pRPhoneDev,uint16_t offset, uint16_t BuffSize, uint32_t *buff);

	void RPhone_setHPMem8(RPhoneDev *pRPhoneDev,uint16_t offset, uint16_t BuffSize, uint8_t *buff);

	void RPhone_getHPMem8(RPhoneDev *pRPhoneDev,uint16_t offset, uint16_t BuffSize, uint8_t *buff);

	uint16_t RPhone_getDbgFramen(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgDevseln(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgIrdyn(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgtrdyn(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgStopn(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgidsel(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgGntn(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgReqn(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgParn(RPhoneDev *pRPhoneDev);
	uint16_t RPhone_getDbgPerrn(RPhoneDev *pRPhoneDev);
	void RPhone_setDbgReset(RPhoneDev *pRPhoneDev,uint16_t resetVector);
	uint32_t RPhone_getIRQ(RPhoneDev *pRPhoneDev);
	uint32_t RPhone_getF56ReadStat(RPhoneDev *pRPhoneDev);
	void RPhone_settF56ResetStat(RPhoneDev *pRPhoneDev,uint8_t Reset);
	uint8_t RPhone_gettF56ResetStat(RPhoneDev *pRPhoneDev);
	uint32_t RPhone_getF56AddRegister(RPhoneDev *pRPhoneDev,uint8_t regAdd,uint8_t chipSel);
	uint32_t RPhone_setF56AddRegister(RPhoneDev *pRPhoneDev,uint8_t regAdd,uint8_t chipSel,uint8_t data);
	uint32_t RPhone_getF56ReadCapture(RPhoneDev *pRPhoneDev,uint8_t Capture);

#endif /* RAYAPHONEPCI_H_ */
