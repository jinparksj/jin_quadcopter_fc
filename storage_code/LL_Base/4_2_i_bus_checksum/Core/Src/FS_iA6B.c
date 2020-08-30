/*
 * FS_iA6B.c
 *
 *  Created on: Aug 29, 2020
 *      Author: jin
 */

#include "FS_iA6B.h"

unsigned char iBus_Check_CHECKSUM(unsigned char *data, unsigned char len) {

	unsigned short checksum = 0xffff;

	for (int i = 0; i < len - 2; i++) {
		checksum = checksum - data[i];
	}

	return ((checksum & 0x00ff) == data[30]) && ((checksum >> 8) == data[31]);
}

