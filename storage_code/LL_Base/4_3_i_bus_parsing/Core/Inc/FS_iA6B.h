/*
 * FS_iA6B.h
 *
 *  Created on: Aug 29, 2020
 *      Author: jin
 */

#ifndef INC_FS_IA6B_H_
#define INC_FS_IA6B_H_

/* Define to prevent recursive inclusion -------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef struct _FSiA6B_iBus {
	unsigned short RH;
	unsigned short RV;
	unsigned short LV;
	unsigned short LH;
	unsigned short SwA;
	unsigned short SwB;
	unsigned short SwC;
	unsigned short SwD;
	unsigned short VrA;
	unsigned short VrB;

	unsigned short FailSafe;
}FSiA6B_iBus;

extern FSiA6B_iBus iBus;

unsigned char iBus_Check_CHECKSUM(unsigned char *data, unsigned char len);
void iBus_Parsing(unsigned char *data, FSiA6B_iBus *iBus);
void FSiA6B_UART5_Initialization(void);


#ifdef __cplusplus
}
#endif
#endif /* INC_FS_IA6B_H_ */
