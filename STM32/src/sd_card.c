#include "sd_card.h"
#include "dht11.h"
#include "spi_sd.h"
#include "ff.h"

FATFS fatfs;
FIL file;
FRESULT fresult;

int SD_write_measurement(TCHAR* filename){
	fresult = f_open(&file, filename, FA_OPEN_ALWAYS | FA_WRITE);

	if(fresult == FR_OK){
		char buffer[10], buffer2[10];
		char* temperature = itoa(measurement.temperature, buffer, 10);
		char* humidity = itoa(measurement.humidity, buffer2, 10);

		f_lseek(&file, f_size(&file));

		f_puts("Temperature: ", &file);
		f_puts(temperature, &file);
		f_puts("\t Humidity: ", &file);
		f_puts(humidity, &file);
		f_puts("\r\n", &file);

		f_close(&file);

		return SD_SUCCESS;
	} else{
		return SD_ERROR;
	}
}

int SD_init(){
	delay_init(168);
	SPI_SD_Init();

	if(f_mount(0, &fatfs) == FR_OK){
		return SD_SUCCESS;
	} else{
		return SD_ERROR;
	}
}
