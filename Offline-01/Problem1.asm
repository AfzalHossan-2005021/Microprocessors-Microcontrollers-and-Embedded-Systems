.MODEL SMALL

.STACK 100H

.DATA
    CR EQU 0DH
    LF EQU 0AH
    
    PROMPT          DB  'Input a single printable ASCII character: $'  
    NUMBER_MSG      DB  CR, LF, 'Number$' 
    UPPERCASE_MSG   DB  CR, LF, 'Uppercase letter$'
    LOWERCASE_MSG   DB  CR, LF, 'Lowercase letter$'
    OTHERS_MSG      DB  CR, LF, 'Not an alphanumeric value$'

.CODE

MAIN PROC
;initialize DS
    MOV AX, @DATA
    MOV DS, AX
    
;print user prompt
    LEA DX, PROMPT
    MOV AH, 9
    INT 21H

;input a upper case character    
    MOV AH, 1
    INT 21H
  
NUMBER_CHECK:
    CMP AL, '0'
    JB  OTHERS 
    CMP AL, '9' 
    JA  UPPERCASE_CHECK
    LEA DX, NUMBER_MSG
    JMP DISPLAY
    
UPPERCASE_CHECK:    
    CMP AL, 'A'
    JB OTHERS
    CMP AL, 'Z'
    JA  LOWERCASE_CHECK
    LEA DX, UPPERCASE_MSG
    JMP DISPLAY
     
LOWERCASE_CHECK:
    CMP AL, 'a'
    JB OTHERS
    CMP AL, 'z'
    JA  OTHERS
    LEA DX, LOWERCASE_MSG
    JMP DISPLAY
    
OTHERS:
    LEA DX, OTHERS_MSG
    JMP DISPLAY
     
DISPLAY:
    MOV AH, 9
    INT 21H 

EXIT:
    MOV AH, 4CH
    INT 21H
  
MAIN ENDP

    END MAIN