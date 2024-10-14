#include <xc.inc>

global regA
global regB
global result_msb
global result_lsb
PSECT udata_shr
regA:
    DS 1
regB:
    DS 1
result_msb:
    DS 1
result_lsb:
    DS 1
psect resetVec,class=CODE,delta=2
resetVec:
    PAGESEL start
    goto start
    
psect code,class=CODE,delta=2
    start:
	BANKSEL PORTA
	clrf PORTA
	BANKSEL PORTB
	clrf PORTB
	BANKSEL PORTC
	clrf PORTC
	BANKSEL PORTD
	clrf PORTD
	BANKSEL ANSEL
	clrf ANSEL
	BANKSEL ANSELH
	clrf ANSELH
    
    pin_config:
	BANKSEL TRISA
	movlw 0x00
	movwf TRISA
	BANKSEL TRISC
	movwf TRISC
	movlw 0xFF
	BANKSEL TRISB
	movwf TRISB
	BANKSEL TRISD
	movwf TRISD
    
    get_values:
	BANKSEL PORTB
	movf PORTB, W
	movwf regA
	BANKSEL PORTD
	movf PORTD, W
	movwf regB

    mult_sum:
	movf regB, W ;W = b
	addwf result_lsb, F ; result_lsb = result_lsb + W
	btfss STATUS, 0 ; se a soma der um carry, pula uma instrução
	goto $+3
	movlw 0x01 ; w = 1
	addwf result_msb, F ;result_msb = result_msb + carry	
	DECFSZ regA
	goto mult_sum
	goto show_result
    show_result:
	movf result_lsb, W
	movwf PORTA
	clrf result_lsb
	movf result_msb, W
	movwf PORTC
	clrf result_msb
	goto get_values


