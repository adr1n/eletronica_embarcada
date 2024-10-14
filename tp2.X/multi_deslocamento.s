#include <xc.inc>

global regA
global regB
global result_msb
global result_lsb
global i ;contador
valor EQU 0x08
PSECT udata_shr
regA:
    DS 1
regB:
    DS 1
result_msb:
    DS 1
result_lsb:
    DS 1
i:
    DS 1
psect resetVec,class=CODE,delta=2
resetVec:
    PAGESEL start
    goto start
    
psect code,class=CODE,delta=2
    start:
	BANKSEL ANSEL
	clrf ANSEL
	BANKSEL ANSELH
	clrf ANSELH
    
    pin_config:
	BANKSEL TRISA
	clrf TRISA
	clrf TRISC
	movlw 0xFF
	movwf TRISB
	movwf TRISD
    
    get_values:
	BANKSEL PORTB
	movf PORTB, W
	movwf regA
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
	
    mult_shift:
	movlw valor
	movwf i
	mult_loop:
	    RRF regA
	    btfss STATUS, 0
	    goto $+3
	    movf regB, W
	    addwf result_msb, F
	shift_prod:
	    BCF STATUS, 0
	    RRF result_msb
	    RRF result_lsb
	    DECFSZ i, F
	    goto mult_loop
	    
    show_result:
	BANKSEL PORTA
	movf result_lsb, W
	movwf PORTA
	clrf result_lsb
	movf result_msb, W
	movwf PORTC
	clrf result_msb
	


