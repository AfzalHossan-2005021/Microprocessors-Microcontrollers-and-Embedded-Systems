.MODEL SMALL

.STACK 100H

.DATA
    CR EQU 0DH
    LF EQU 0AH
    
    PROMPT      DB  'Input three lowercase letters: $'  
    EQUAL_MSG   DB  'All letters are equal$'
    INVALID_MSG DB  'Invalid input$'

.CODE

MAIN PROC
;initialize DS
    MOV AX, @DATA
    MOV DS, AX
    
;print user prompt
    LEA DX, PROMPT
    MOV AH, 9
    INT 21H

;input first character    
    MOV AH, 1
    INT 21H
    MOV CL, AL  
;print space
    MOV DX, ' '
    MOV AH, 2
    INT 21H
    
;input second character    
    MOV AH, 1
    INT 21H 
    MOV BL, AL 
    
;print space
    MOV DX, ' '
    MOV AH, 2
    INT 21H
    
;input third character    
    MOV AH, 1
    INT 21H 
    MOV BH, AL
    
;print newline character
    MOV DX, LF
    MOV AH, 2
    INT 21H
    MOV DX, CR
    INT 21H
    
;CL -> first characterr
;BL -> second characterr
;BH -> third characterr
    
;check validity of first character
    CMP CL, 'a'       
    JB  DISPLAY_INVALID_MSG
    CMP CL, 'z'
    JA  DISPLAY_INVALID_MSG  
    
;check validity of second character
    CMP BL, 'a'       
    JB  DISPLAY_INVALID_MSG
    CMP BL, 'z'
    JA  DISPLAY_INVALID_MSG
    
;check validity of third character
    CMP BH, 'a'       
    JB  DISPLAY_INVALID_MSG
    CMP BH, 'z'
    JA  DISPLAY_INVALID_MSG
    
;comparison between first two character
    CMP CL, BL
    JA  FIRST_GREATER
    JE  BOTH_EQUAL
    JB  SECOND_GREATER
    
;comparison between previous smaller and third character
FIRST_GREATER:
    ;comparison betwee second and third character 
    CMP BL, BH
    MOV DL, BL
    JAE DISPLAY_RESULT
    MOV DL, BH           
    ;comparison between first and third character
    CMP CL, BH
    MOV DL, BH
    JA  DISPLAY_RESULT
    MOV DL, BL
    JE  DISPLAY_RESULT
    MOV DL, CL
    JMP DISPLAY_RESULT
    
BOTH_EQUAL:
    CMP BL, BH
    MOV DL, BH
    JA  DISPLAY_RESULT
    LEA DX, EQUAL_MSG
    JE  DISPLAY_EQUAL_MSG
    MOV DL, BL
    JMP DISPLAY_RESULT
    
SECOND_GREATER:
    ;comparison between first and third character
    CMP CL, BH
    MOV DL, CL
    JAE DISPLAY_RESULT
    ;comparison between second and third character
    CMP BL, BH
    MOV DL, BH
    JA  DISPLAY_RESULT
    MOV DL, CL
    JE  DISPLAY_RESULT
    MOV DL, BL
    JMP DISPLAY_RESULT
     
DISPLAY_RESULT:
    MOV AH, 2
    INT 21H
    JMP EXIT

DISPLAY_EQUAL_MSG:
    MOV AH, 9
    INT 21H
    JMP EXIT
    
DISPLAY_INVALID_MSG:
    LEA DX, INVALID_MSG
    MOV AH, 9
    INT 21H
    
EXIT:
    MOV AH, 4CH
    INT 21H
  
MAIN ENDP

    END MAIN