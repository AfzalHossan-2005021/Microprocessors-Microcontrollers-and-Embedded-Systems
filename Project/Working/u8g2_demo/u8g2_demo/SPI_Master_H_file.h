/*
 * SPI_Master_H_file.h
 *
 * Created: 3/2/2024 9:05:47 AM
 *  Author: Afzal Hossan
 */ 


#ifndef SPI_MASTER_H_FILE_H_
#define SPI_MASTER_H_FILE_H_


void SPI_Init()					/* SPI Initialize function */
{
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);	/* Make MOSI, SCK, SS as Output pin */
	DDRB &= ~(1<<MISO);			/* Make MISO pin as input pin */
	PORTB |= (1<<SS);			/* Make high on SS pin */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);	/* Enable SPI in master mode with Fosc/16 */
	SPSR &= ~(1<<SPI2X);			/* Disable speed doubler */
}

void SPI_Write(char data)		/* SPI write data function */
{
	char flush_buffer;
	SPDR = data;			/* Write data to SPI data register */
	while(!(SPSR & (1<<SPIF)));	/* Wait till transmission complete */
	flush_buffer = SPDR;		/* Flush received data */
	/* Note: SPIF flag is cleared by first reading SPSR (with SPIF set) and then accessing SPDR hence flush buffer used here to access SPDR after SPSR read */
}

char SPI_Read()				/* SPI read data function */
{
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));	/* Wait till reception complete */
	return(SPDR);			/* Return received data */
}


#endif /* SPI_MASTER_H_FILE_H_ */