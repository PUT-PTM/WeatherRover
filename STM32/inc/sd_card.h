#ifndef SD_CARD_H_
#define SD_CARD_H_

#include "ff.h"

#define SD_SUCCESS			1
#define SD_ERROR			2

extern FATFS fatfs;
extern FIL file;
extern DIR directory;
extern FRESULT fresult;

int SD_write_measurement(TCHAR* filename);
int SD_init();

#endif /* SD_CARD_H_ */
