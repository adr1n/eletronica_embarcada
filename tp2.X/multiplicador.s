#include <xc.inc>
CONFIG FOSC = INTRC_NOCLKOUT
global regA ;Registradores de trabalho A e B
global regB
global storeA ;Registradores de armazenamento A e B
global storeB
global result_msb ;Registradores de resultado
global result_lsb
global i ;contador
global bank ;contador de endereços
valor_i EQU 0x08 ;numero de iteracoes na multiplicaçao com deslocamento

PSECT udata_shr
regA:
    DS 1
regB:
    DS 1
storeA:
    DS 1
storeB:
    DS 1
result_msb:
    DS 1
result_lsb:
    DS 1
i:
    DS 1
bank:
    DS 1
psect resetVec,class=CODE,delta=2
resetVec:
    PAGESEL start
    goto start
    
psect code,class=CODE,delta=2
    start:
	BANKSEL	ANSEL		;Setando entradas e saídas digitais
	clrf	ANSEL
	BANKSEL ANSELH
	clrf	ANSELH
	movlw	0x20	;Setando o valor inicial do contador de endereços
	movwf	bank
    
    pin_config:
	BANKSEL OPTION_REG
	BCF	OPTION_REG, 7
	BANKSEL TRISA		;Setando trisA e trisC como saídas
	clrf	TRISA
	clrf	TRISC
	movlw	0xFF		;Setando trisB e trisD como entradas
	movwf	TRISB
	movwf	TRISD
	
    
    get_values:
	BANKSEL PORTB
	movf	PORTB, W
	movwf	regA		;Colocando os valores nos registradores de 
	movwf	storeA		;trabalho e armazenamento citados anteriormente
	movf	PORTD, W
	movwf	regB
	movwf	storeB
	clrwdt
	
   bit_verification:
	BTFSC	PORTE, 0	;Verificando qual multiplicaçao deve ser feita 
	goto	mult_shift	;RE0=0: Somas sucessivas, RE0=1: Soma e deslocamento
    mult_sum:
	movf	regB, W		;W = b
	addwf	result_lsb, F	;result_lsb = result_lsb + W
	btfss	STATUS, 0	;se a soma der um carry, pula uma instrução
	goto	$+2
	INCF	result_msb, F	;result_msb = result_msb + carry	
	DECFSZ	regA
	goto	mult_sum
	goto	store_result
	
    mult_shift:
	clrwdt
	movlw	valor_i
	movwf	i		;Setando o numero 8 no contador
	mult_loop:
	    RRF	    regA	;Deslocando A para saber se soma ou passa 
	    btfss   STATUS, 0	;Se carry for 1: Soma e desloca
	    goto    $+3		;Se carry=0, apenas desloca
	    movf    regB, W
	    addwf   result_msb, F
	shift_prod:
	    BCF	    STATUS, 0	;Zerando a flag carry para nao interferir no rotate
	    RRF	    result_msb  ;Desloca-se à direita para obter o resultado em 16 bits
	    RRF	    result_lsb
	    DECFSZ  i, F	;Se i chegar a 0, a iteração acaba
	    goto    mult_loop
    
    store_result:
	clrwdt
	overflow_test:		    ;Essa label testa se bank já chegou a 6F, para resetá-lo.
	    movlw   0x6F	   
	    xorwf   bank, W	    
	    BTFSS   STATUS, 2	    ;se bank == 0x6F, flag Z é ativada
	    goto    iteration_address
	    movlw   0x20	    ;Reseta valor de bank para 0x20
	    movwf   bank  
	iteration_address:
	    clrf STATUS		;Como irei usar endereçamento indireto, melhor limpar o STATUS
	    movf bank, W 
	    movwf FSR		;FSR <= bank
	    BCF	STATUS, 7	;Escreverei nos bancos 0 e 1 agora
	    BCF FSR, 7		;Selecionando o banco 0
		movf storeA, W
		movwf INDF
	    BSF FSR, 7	    	;Selecionando o banco 1
		movf storeB, W
		movwf INDF
	    BSF STATUS, 7	;Escreverei nos bancos 2 e 3 agora
	    BCF FSR, 7		;Selecionando o banco 2
		movf result_lsb, W
		movwf INDF
	    BSF FSR, 7		;Selecionando o banco 3
		movf result_msb, W
		movwf INDF
	    INCF bank, F	;bank <= bank + 1
    
    show_result:
	BANKSEL PORTA
	movf result_lsb, W
	movwf PORTA
	clrf result_lsb
	movf result_msb, W
	movwf PORTC
	clrf result_msb
	goto get_values
	
	
    
   
   




