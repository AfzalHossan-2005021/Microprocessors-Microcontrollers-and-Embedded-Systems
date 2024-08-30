.MODEL SMALL

.STACK 0FFFFH

.DATA          
    CR  EQU 0DH
    LF  EQU 0AH
    INPUT_PROMPT    DW  'Input two numbers(space separated) : $' 
    OUTPUT_PROMPT   DW  'Total chocolates : $'
    N   DW  ?
    K   DW  ?
    TOTAL   DW  ?
    
.CODE

MAIN PROC
    ;initialize DATA segment
    MOV AX, @DATA
    MOV DS, AX 
    
    ;show input prompt
    LEA DX, INPUT_PROMPT
    MOV AH, 9
    INT 21H
    
    ;input two numbers
    CALL INPUT_NUMBER
    MOV N,  CX
    CALL INPUT_NUMBER
    MOV K, CX
    
    ;count total 
    MOV CX, N
    MOV BX, K
    
    MOV TOTAL, CX
    
WHILE_1:
    CMP CX, BX
    JB  PRINT_1 
    XOR DX, DX
    MOV AX, CX 
    DIV BX
    ADD TOTAL, AX
    XOR CX, CX
    ADD CX, AX
    ADD CX, DX
    JMP WHILE_1
         
PRINT_1:    ;print output
    MOV DL, LF
    MOV AH, 2
    INT 21H 
        
    MOV DL, CR
    MOV AH, 2
    INT 21H 
        
    LEA DX, OUTPUT_PROMPT
    MOV AH, 9
    INT 21H 
        
        ;print in decimal format
    PUSH TOTAL
    CALL    PRINT_NUMBER
        
    MOV 	AH, 4CH
	INT	21H 
	
MAIN ENDP


INPUT_NUMBER    PROC    ;return value will be in CX
    MOV BX, 0
    PUSH BX  
    MOV BX, 10 
                 
WHILE_2:
    MOV AH, 1
    INT 21H
    CMP AL, '0'
    JB RETURN_1
    CMP AL, '9'
    JA RETURN_1
    SUB AL, '0'
    MOV CX, 0
    MOV CL, AL 
    POP AX
    MUL BX
    ADD AX, CX
    PUSH AX
    JMP WHILE_2
    
RETURN_1: 
    POP CX
    RET 
    
INPUT_NUMBER ENDP 


PRINT_NUMBER PROC
    PUSH    BP
    MOV BP, SP
    MOV AX, WORD PTR[BP+4] 
    XOR DX, DX
    MOV BX, 10
    DIV BX
    CMP AX, 0
    JNZ  RECUR 
    CMP DX, 0
    JZ  RETURN_2 
    
RECUR:
    PUSH DX
    PUSH AX 
    CALL PRINT_NUMBER
    
    ;print character
    POP DX
    ADD DL, '0'
    MOV AH, 2
    INT 21H 
    
RETURN_2: 
    POP BP
    RET 2 
    
PRINT_NUMBER ENDP
    
END MAIN