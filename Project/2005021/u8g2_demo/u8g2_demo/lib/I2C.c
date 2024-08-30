/*
 * I2C.c
 *
 * Created: 3/1/2024 7:56:23 PM
 *  Author: Afzal Hossan
 */ 

#include <I2C.h>

void I2C_Init(){
	TWBR = BITRATE(TWSR=0x00);	// No Pre-scal
}

unsigned char i2c_start(unsigned char address)
{
	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	uint8_t status = TW_STATUS & TWI_STATUS_BIT;
	
	if ( (status != TW_START) && (status != TW_REP_START)) return 0;

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	status = TW_STATUS & TWI_STATUS_BIT;
	if ( (status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK) ) return 0;

	return 1;
}

unsigned char I2C_Repeated_Start(unsigned char address){
	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	uint8_t status = TW_STATUS & TWI_STATUS_BIT;
	if ( status != TW_REP_START ) return 0;

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	status = TW_STATUS & TWI_STATUS_BIT;
	if ( (status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK) ) return 0;

	return 1;
}

void I2C_Start_Wait(unsigned char address){
	while ( 1 )
	{
		// send START condition
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
		
		// wait until transmission completed
		while(!(TWCR & (1<<TWINT)));

		// check value of TWI Status Register. Mask prescaler bits.
		uint8_t status = TW_STATUS & TWI_STATUS_BIT;
		if ( (status != TW_START) && (status != TW_REP_START)) continue;
		
		// send device address
		TWDR = address;
		TWCR = (1<<TWINT) | (1<<TWEN);
		
		// wail until transmission completed
		while(!(TWCR & (1<<TWINT)));
		
		// check value of TWI Status Register. Mask prescaler bits.
		status = TW_STATUS & TWI_STATUS_BIT;
		if ( (status == TW_MT_SLA_NACK )||(status == TW_MR_DATA_NACK) )
		{
			/* device busy, send stop condition to terminate write operation */
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
			
			// wait until stop condition is executed and bus released
			while((TWCR & (1<<TWSTO)));
			
			continue;
		}
		break;
	}
}

unsigned char I2C_Write(unsigned char data){
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	uint8_t status = TW_STATUS & TWI_STATUS_BIT;
	if( status != TW_MT_DATA_ACK) return 0;
	return 1;
}

unsigned char I2C_Read_Ack(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}

unsigned char I2C_Read_Nack(){
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}

unsigned char I2C_Read(unsigned char ack){
	if(ack) return I2C_Read_Ack();
	else return I2C_Read_Nack();
}

void I2C_Stop(){
	/* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	while((TWCR & (1<<TWSTO)));
}