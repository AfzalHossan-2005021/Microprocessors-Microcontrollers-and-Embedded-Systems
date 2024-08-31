/*
 * SPI_Slave_H_file.h
 *
 * Created: 3/2/2024 10:25:28 AM
 *  Author: Afzal Hossan
 */ 


#ifndef SPI_SLAVE_H_FILE_H_
#define SPI_SLAVE_H_FILE_H_


#define SS DDB4
#define MOSI DDB5
#define MISO DDB6
#define SCK DDB7

void SPI_Init()					/* SPI Initialize function */
{
	DDRB |= (1<<MISO);			/* Make MISO pin as output pin */
	SPCR = (1<<SPE);			/* Enable SPI in slave mode */
}

char SPI_Exchange(char data_to_be_sent)		/* SPI write data function */
{
	SPDR = data_to_be_sent;			/* Write data to SPI data register */
	while(!(SPSR & (1<<SPIF)));		/* Wait till transmission complete */
	return SPDR;					/* Return the received data*/
}


#endif /* SPI_SLAVE_H_FILE_H_ */