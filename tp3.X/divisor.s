
#include <xc.inc>
#define numerador PORTB
#define denominador PORTD
#define quociente PORTA
#define resto PORTC
CONFIG FOSC = INTRC_NOCLKOUT
global num ;Registradores de trabalho A e B
global den
global quoc ;Registrador do quociente
global rest
global i ;contador
valor_i EQU 0x08 ;numero de iteracoes na multiplicaçao com deslocamento

PSECT udata_shr
num:
    DS 1
den:
    DS 1
quoc:
    DS 1
rest:
    DS 1
i:
    DS 1

psect resetVec,class=CODE,delta=2
resetVec:
    PAGESEL start
    goto start
    
psect code,class=CODE,abs,delta=2
ORG 0x100			;Aloca o código no endereço 0x100
    start:
	BANKSEL	ANSEL		;Setando entradas e saídas digitais
	clrf	ANSEL
	BANKSEL ANSELH
	clrf	ANSELH
    
    pin_config:
	BANKSEL OPTION_REG
	BCF	OPTION_REG, 7
	BANKSEL TRISA		;Setando PORTA e PORTC como saídas
	clrf	TRISA
	clrf	TRISC
	movlw	0xFF		;Setando PORTB e PORTD como entradas
	movwf	TRISB
	movwf	TRISD
	movlw	0x01		;Setando PORTE com RE0 como entrada e RE1 como saída
	movwf	TRISE
    
    get_values:
	BANKSEL PORTB
	movf	numerador, W
	movwf	num		;Colocando os valores nos registradores de 
	movf	denominador, W
	movwf	den
	clrwdt
	
    zero_verification:
	BCF	STATUS, 2
	movf	denominador, F
	BTFSC	STATUS, 2
	goto	zero_error
	
    op_verification:
	BTFSC	PORTE, 0	;Verificando qual divisão deve ser feita 
	goto	div_shift	;RE0=0: Subtrações sucessivas, RE0=1: Subtração e deslocamento
	call	div_sub
	call	show_result
	goto	get_values
	
    div_sub:
	movf	num, W		;W = numerador
	movwf	rest		;resto <= numerador
	clrwdt
	div_sub_loop:
	    clrwdt
	    movf    den, W	
	    subwf   rest, W		;Testa para saber se rest ja chegou a 0 ou ficou negativo
	    BTFSS   STATUS, 0
	    goto    $+5
	    incf    quoc, F		;q = q + 1
	    movf    den, W		
	    subwf   rest, F		;rest-den: C=0 se den>rest, C=1 se den<=rest
	    goto    div_sub_loop
	    return
			
    div_shift:
	clrwdt
	clrf	rest
	clrf	quoc
	movlw	valor_i
	movwf	i
	
	div_shift_loop:
	    clrf    STATUS
	    RLF	    num, F	;n = n<<1
	    RLF	    rest, F	;r = r<<1
	    movf    den, W
	    subwf   rest, W	;W = rest - den
	    BTFSC   STATUS, 0	;C=0 se den>rest, C=1 se den<=rest
	    movwf   rest
	    RLF	    quoc, F	;q = q<<1
	    DECFSZ  i		;i = i-1
	    goto    div_shift_loop  
	    goto    show_result
	    
    zero_error:
	clrf    quociente   ;q = 0
	movf	numerador
	movwf   resto	    ;r = n
	BSF	PORTE, 1
	
    show_result:
	BANKSEL PORTA
	movf	quoc, W
	movwf	quociente
	movf	rest, W
	movwf	resto

    
    store:
	BCF	STATUS, 5
	BCF	STATUS, 6   ;Selecionando o banco 0 para armazenar resultados
	;
	movlw	high(table)
	movwf	PCLATH
	;
	swapf	quoc, F	    ;Trocando Nibble 1 com Nibble 0
	movlw	0x0F	    ;Criando máscara para pegar Nible 1
	andwf	quoc, W
	call	table
	movwf	0x20
	;
	swapf	quoc, F
	movlw	0x0F	    ;Criando máscara para pegar Nible 0
	andwf	quoc, W
	call	table
	movwf	0x21
	;
	swapf	rest, F	    ;Trocando Nibble 1 com Nibble 0
	movlw	0x0F	    ;Criando máscara para pegar Nible 1
	andwf	rest, W
	call	table
	movwf	0x22
	;
	swapf	rest, F	    ;Trocando Nibble 1 com Nibble 0
	movlw	0x0F	    ;Criando máscara para pegar Nible 0
	andwf	rest, W
	call	table
	movwf	0x23
	;
	clrf	quoc
	clrf	rest
	return
    
    ORG 0x400
    table:
	addwf	PCL, F
	retlw	0x30	;0
	retlw	0x31	;1
	retlw	0x32	;2
	retlw	0x33	;3
	retlw	0x34	;4
	retlw	0x35	;5
	retlw	0x36	;6
	retlw	0x37	;7
	retlw	0x38	;8
	retlw	0x39	;9
	retlw	0x41	;A
	retlw	0x42	;B
	retlw	0x43	;C
	retlw	0x44	;D
	retlw	0x45	;E
	retlw	0x46	;F
	
	






