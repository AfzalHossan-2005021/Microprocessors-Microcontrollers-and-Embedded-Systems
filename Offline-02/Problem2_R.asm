.MODEL SMALL

.STACK 0FFFFH

.DATA          
    CR  EQU 0DH
    LF  EQU 0AH
    INPUT_PROMPT    DW  'Input a number : $' 
    OUTPUT_PROMPT   DW  'Digit sum of the number : $'
    N   DW  ?
    DGT_SUM   DW  ?
    
.CODE

MAIN PROC
    ;initialize DATA segment
    MOV AX, @DATA
    MOV DS, AX 
    
    ;show input prompt
    LEA DX, INPUT_PROMPT
    MOV AH, 9
    INT 21H
    
    ;input number
    CALL INPUT_NUMBER
    MOV N,  CX
    
    ;count digit sum 
    PUSH N
    CALL DIGIT_SUM
    MOV DGT_SUM,    CX
         
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
    PUSH DGT_SUM
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


DIGIT_SUM PROC
    PUSH    BP
    MOV BP, SP
    MOV AX, WORD PTR[BP+4]
    XOR DX, DX
    MOV BX, 10
    DIV BX
    CMP AX, 0
    JNZ RECUR_1
    MOV CX, DX
    JMP RETURN_2
    
RECUR_1:
    PUSH DX
    PUSH AX
    CALL DIGIT_SUM
    
    POP DX
    ADD CX, DX
    
RETURN_2:
    POP BP
    RET 2
    
DIGIT_SUM ENDP


PRINT_NUMBER PROC
    PUSH    BP
    MOV BP, SP
    MOV AX, WORD PTR[BP+4] 
    XOR DX, DX
    MOV BX, 10
    DIV BX
    CMP AX, 0
    JNZ  RECUR_2 
    CMP DX, 0
    JZ  RETURN_3 
    
RECUR_2:
    PUSH DX
    PUSH AX 
    CALL PRINT_NUMBER
    
    ;print character
    POP DX
    ADD DL, '0'
    MOV AH, 2
    INT 21H 
    
RETURN_3: 
    POP BP
    RET 2 
    
PRINT_NUMBER ENDP
    
END MAIN