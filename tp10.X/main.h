/* 
 * File:   main.h
 * Author: Guillermo
 *
 * Created on June 20, 2023, 6:17 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#define COUNT_MAX 10// Quantidade de medições
#define EE_WRITE 0x02    
#define EE_READ  0b00000011  // Leitura de byte(s)

#define flip_matrix true    // Se for True inverte horizontalmente a matriz
    
// Define valores de uma matriz de 4x8 para repersentra números entre 0-9
// A parte inferior do número é o bit 0.
// Tabela de conversão (lookup) na EEPROM. São 10 símbolos (0-9) de 4x8 LEDs. 
    
const uint8_t matrix_conf[] = {
    0x09,0x00,  // Decode mode = 0
    0x0A,0x00,  // Intensity 1/32
    0x0B,0x07,  // Scan Limit
    0x0C,0x01,  // Shutdown mode = 1
    0x0F,0x01,  // Display-Test = 1
    0x0F,0x00,  // Display-Test = 0
};  ///< Configuração da matriz de  LEDs

typedef enum{
    spiCS1,             ///< Matriz de LEDs (MAX7219))
    spiCS2              ///< Memória EEPROM (25LC512))
}spiCS_t;               ///< CS do dispositivo a ser acessado

typedef union{          ///< União
    uint16_t v16;       ///< Valor uint16
    struct{             ///< Estrutura com os bytes que forman o uint16
        uint8_t b0; ///< byte b0 
        uint8_t b1; ///< byte b1  
    };
}uint16_u; 

uint8_t T; 
uint16_t random;
uint8_t x;
uint8_t y;
uint8_t incx;
float a;

char bufferRx;  ///< Buffer de Rx
uint8_t countRx = 0;        ///< Contador de bytes recebidos (ponteiro))

uint16_u address_write = 0;           ///< Endereço inicial na EEPROM
uint16_u address_read = 0;
bool show;                  ///< Exibe COUNT_MAX números lidos da EEPROM a partir do endereço address

uint8_t eeNumber[10];      ///< Números lidos da EEPROM

/**
 * Configura matrizes de LEDs
 */
void initMatrix();


void matrixClear();
void positionCalc();
void positionUpdate(uint8_t cordX, uint8_t cordY);
void txSpi(spiCS_t cs, uint8_t *data, size_t dataSize);
void executaTudo();
/**
 * Envia e recebe dados por SPI
 * @param cs Dispositivo a ser acessado (ver spiCS_t)
 * @param data Arranjo de dados a serem transmitidos e recebidos
 * @param dataSize Quantidade de dados a serem transmitidos e recebidos
 */
void txSpi(spiCS_t cs, uint8_t *data, size_t dataSize);


/**
 * Analisa quadro de comunicação
 */
void analisaRx();
void Ext_Write(uint8_t addressLow, uint8_t addressHigh, uint8_t x, uint8_t y);
void Ext_Read(uint8_t addressLow, uint8_t addressHigh);



#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */
