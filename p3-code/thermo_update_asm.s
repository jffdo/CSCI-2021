### Begin with functions/executable code in the assmebly file via '.text' directive
.text
.global  set_temp_from_ports
        
## ENTRY POINT FOR REQUIRED FUNCTION
set_temp_from_ports:      # Arguments:  %rdi, pointer to temp struct
  ## assembly instructions here
  cmpw  $0,  THERMO_SENSOR_PORT(%rip)
  jl    .OUT_OF_BOUNDS    # Check if Sensor Port is < 0,
  cmpw  $28800,  THERMO_SENSOR_PORT(%rip)
  jg    .OUT_OF_BOUNDS    # Check if Sensor Port is > 28800
  movl  $1, %eax          # Put 1, into eax
  sal   $2,  %eax         # Leftshift 1 by 2
  testb THERMO_STATUS_PORT(%rip), %al  # Bitwise and of Status Port and (1<<2)
  jnz   .OUT_OF_BOUNDS    # Jump if NonZero Flag, (bitwise and of status and (1<<2) is 1) 
  
  # IN_BOUNDS
  movb  $1, 2(%rdi)       # 1 into 2(%rdi), temp->temp_mode (Assume temp is Celcius)
  movw  THERMO_SENSOR_PORT(%rip), %ax  # Copy Sensor Port to %eax
  sar   $5,  %eax         # Rightshift Sensor Port Value by 5
  movw  %ax, 0(%rdi)      # Put %ax value into 0(%rdi), temp->tenths_degrees
  movl  $1, %eax          # Put 1 into eax
  sal   $4,  %eax         # Leftshift 1 by 4
  testw THERMO_SENSOR_PORT(%rip), %ax # Bitwise and of Sensor Port and (1<<4)
  jz    .CALC_CELCIUS          # Jump if Zero Flag, (bitwise and of status and (1<<4) is 0) 
  
  # REMAINDER (If 4th bit includes 16, round up)
  incw   0(%rdi)          # Increment 0(%rdi), temp->tenths_degrees by 1
  
  .CALC_CELCIUS:
  subw   $450, 0(%rdi)    # Subtract 450 from 0(%rdi), temp->tenths_degrees
  movl  $1, %eax          # Put 1 into eax
  sal   $5,  %eax         # Leftshift 1 by 5
  testb THERMO_STATUS_PORT(%rip), %al # Bitwise and of Status Port and (1<<5)
  jz    .RETURN           # Jump if Zero Flag, (bitwise and of status and (1<<5) is 0) 

  # CALC_FARENHEIT (If 5th bit is 1, then thermo is F)
  movb  $2, 2(%rdi)       # 2 into 2(%rdi), temp->temp_mode
  movw  0(%rdi) , %ax     # Copy 0(%rdi), temp->tenths_degrees into %ax
  imul  $9, %ax           # Multipy by 9           
  cwtl                    # Prep for Division
  cltq
  cqto
  movl  $5, %esi          # Put 5 in %esi
  idiv  %esi              # Divide by 5
  addl  $320, %eax        # Add 320
  movw  %ax, 0(%rdi)      # Copy %ax (result of division), into 0(%rdi),temp->tenths_degrees

  .RETURN:
  movl  $0, %eax          # 0 into %eax to return
  ret                     # eventually return from the function
  
  .OUT_OF_BOUNDS:
  movw  $0, 0(%rdi)       # 0 into 0(%rdi), temp->tenths_degrees
  movb  $3, 2(%rdi)       # 3 into 2(%rdi), temp->temp_mode
  movl  $1, %eax          # 1 into %eax to return
  ret

### Change to definine semi-global variables used with the next function 
### via the '.data' directive
.data

digit:                    # 'digit' array. Each are spaced 4 bytes from the next
  .int 0b1111011          # digit[0]
  .int 0b1001000          # digit[1]
  .int 0b0111101          # digit[2]
  .int 0b1101101          # digit[3]
  .int 0b1001110          # digit[4]
  .int 0b1100111          # digit[5]
  .int 0b1110111          # digit[6]
  .int 0b1001001          # digit[7]
  .int 0b1111111          # digit[8]
  .int 0b1101111          # digit[9]    

### Change back to defining functions/execurable instructions
.text
.global  set_display_from_temp

## ENTRY POINT FOR REQUIRED FUNCTION
set_display_from_temp:    # Arguments:  %rdi, packed temp struct; %rsi, integer pointer  
  ## assembly instructions here
  movl  $0, (%rsi)        # Set (%rsi), display pointer to 0

  movl  %edi, %eax        # Copy %edi, to %eax  
  sarl  $16,  %eax        # Left shift %eax by 16 to access temp.temp_mode
  cmpb  $1, %al          
  je    .CELCIUS          # Check if temp.temp_mode == 1
  cmpb  $2, %al
  je    .FARENHEIT        # Check if temp.temp_mode == 2

  .ERROR_DISPLAY:
  movq  $0b011011110111111011111, %rax  # Put "ERR" bitmask the into %rax
  sal   $7, %rax          # Left shift "ERR" bitmask by 7
  orl   %eax, (%rsi)      # Or the bitmask to (%rsi), display pointer
  movl  $1, %eax          # 1 into %eax to return
  ret
  
  .CELCIUS:
  movw  %di,  %ax         # Copy first 2 bytes of %di (temp.tenths_degrees) to %ax
  cmpw  $-450,  %ax
  jl    .ERROR_DISPLAY    # Check if 2 bytes of %di (temp.tenths_degrees) < -450
  cmpw  $450, %ax
  jg    .ERROR_DISPLAY    # Check if 2 bytes of %di (temp.tenths_degrees) > 450
  movl  $1, %eax          # Put 1 into %eax
  sal   $28,  %rax        # Leftshift 1 by 28
  orl    %eax, (%rsi)     # Or the bitmask to (%rsi), display pointer
  jmp   .DISPLAY          # Jump to display to avoid Farenheit calculations

  .FARENHEIT:             
  movw  %di,  %ax         # Copy first 2 bytes of %di (temp.tenths_degrees) to %ax
  cmpw  $-490,  %ax
  jl    .ERROR_DISPLAY    # Check if 2 bytes of %di (temp.tenths_degrees) < -490
  cmpw  $1130, %ax
  jg    .ERROR_DISPLAY    # Check if 2 bytes of %di (temp.tenths_degrees) > 1130
  movl  $1, %eax          # Put 1 into %eax
  sal   $29,  %rax        # Leftshift 1 by 29
  orl   %eax, (%rsi)      # Or the bitmask to (%rsi), display pointer

  .DISPLAY:
  movw  %di,  %cx         # Copy first 2 bytes of %di (temp.tenths_degrees) to %cx
  movzwq %cx,  %rax       # Copy %cx to $rax, while zeroing out register   
  cwtl                    
  cltq
  cqto                    # Convert %cx to signed number in register      | This section of code checks if temp is negative 
  sar   $31,  %rax        # Right shift $rax by 31                        | in order to apply the absolute value
  not   %rax              # Invert %rax                                   | ~(postive number >> 31)+1 = 0
  incq  %rax              # Increment %rax by 1                           | ~(negative number >> 31)+1 = 1
  testq %rax, %rax       
  jz   .CONVERT_DIGIT     # Check if %rax is 0, which is a positive number
  
  # ABSOLUTE_VALUE
  not   %cx               # Invert %cx bits                               | 2's Complement Conversion
  incw  %cx               # Increment 1 to %cx                            |
  
  .CONVERT_DIGIT:
  movq  $0, %r8           # Put 0 into %r8, which will be a packed struct containg temp_hundreds, temp_tens, temp_ones, and temp_tenths 
  movzwq %cx,  %rax       # Copy %cx to $rax, while zeroing out register
  cqto                          
  movl  $1000, %r9d       # Put 1000 in %r9d
  idiv  %r9d              # Divide by 1000
  movl  $10, %r9d         # Put 10 in %r9d
  cqto                      
  idiv  %r9d              # Divide by 10
  sal   $12, %edx         # Leftshift by 12. Bits 12-15 are temp_hundreds.
  orq  %rdx, %r8          # Or bit mask into packed struct %r9d

  movzwq %cx,  %rax       # Copy %cx to $ax, while zeroing out register
  cqto                        
  movl  $100, %r9d        # Put 100 in %r9d
  idiv  %r9d              # Divide by 100
  movl  $10, %r9d         # Put 10 in %r9d
  cqto                          
  idiv  %r9d              # Divide by 10
  sal   $8, %edx          # Leftshift by 8. Bits 8-11 are temp_tens.
  orq  %rdx, %r8          # Or bit mask into packed struct %r8

  movzwq %cx,  %rax       # Copy %cx to $ax, while zeroing out register
  cqto                         
  movl  $10, %r9d         # Put 10 in %r9d
  idiv  %r9d              # Divide by 10
  movl  $10, %r9d         # Put 10 in %r9d
  cqto                        
  idiv  %r9d              # Divide by 10
  sal   $4, %edx          # Leftshift by 4. Bits 4-7 are temp_ones.
  orq  %rdx, %r8          # Or bit mask into packed struct %r8

  movzwq %cx,  %rax       # Copy %cx to $ax, while zeroing out register
  cqto                         
  movl  $10, %r9d         # Put 10 in %r9d
  cqto                        
  idiv  %r9d              # Divide by 10
  orq  %rdx, %r8          # Or bit mask into packed struct %r8, for 0-3 bits to be temp_tenths

  movl  $0b0000100, %eax  # Replace %eax with "-" bitmask
  sal   $21,  %rax        # Left shift %rax by 28
  movl  %eax, %edx        # Put %eax bitmask into %edx, negative_sign
         
  leaq  digit(%rip), %rcx # Put digit array pointer into %rcx
  movq  %r8, %r9          # Copy packed struct to %r9
  sar   $12, %r9          # Shift by 12, to get temp_hundreds
  andq  $0xF, %r9         # And mask by 0xF to get the first 4 bytes
  testl %r9d, %r9d 
  jz    .SHIFT_NEGATIVE   # Check if hundreds are 0
  
  # SET_HUNDREDS
  movl  (%rcx,  %r9, 4), %eax  # Put digit[temp_hundrends] into %eax
  sal   $21, %rax         # Left shift %rax by 21
  orl   %eax, (%rsi)      # Or the bitmask to (%rsi), display pointer

  .SHIFT_NEGATIVE:
  movq  %r8, %r9          # Copy packed struct to %r9
  sar   $12, %r9          # Shift by 12, to get temp_hundreds
  andq  $0xF, %r9         # And mask by 0xF to get the first 4 bytes
  testl %r9d, %r9d        
  jnz   .SET_TENS         # Check if hundreds are 0
  movq  %r8, %r9          # Copy packed struct to %r9
  sar   $8, %r9           # Shift by 8, to get temp_tens
  andq  $0xF, %r9         # And mask by 0xF to get the first 4 bytes
  testl %r9d, %r9d 
  jnz   .SET_TENS         # Check if tens are 0
  sar   $7, %edx          # Right shift %edx, negative_sign to the right
  jmp   .SET_ONES         # Jump to SET_ONES to avoid SET_TENS calculations

  .SET_TENS:
  movq  %r8, %r9          # Copy packed struct to %r9
  sar   $8, %r9           # Shift by 8, to get temp_tens
  andq  $0xF, %r9         # And mask by 0xF to get the first 4 bytes
  movl  (%rcx,  %r9, 4), %eax  # Put digit[temp_tens] into %eax
  sal   $14, %rax         # Shift %rax by 14
  orl   %eax, (%rsi)      # Or the bitmask to (%rsi), display pointer
  
  .SET_ONES:
  movq  %r8, %r9          # Copy packed struct to %r9
  sar   $4, %r9           # Shift by 4, to get temp_ones
  andq  $0xF, %r9         # And mask by 0xF to get the first 4 bytes
  movl  (%rcx,  %r9, 4), %eax  # Put digit[temp_ones] into %eax
  sal   $7, %rax          # Shift %rax by 7
  orl   %eax, (%rsi)      # Or the bitmask to (%rsi), display pointer

  # SET_TENTHS
  movq  %r8, %r9          # Copy packed struct to %r9
  andq  $0xF, %r9         # And mask by 0xF to get the first 4 bytes, temp_tenths
  movl  (%rcx,  %r9, 4), %eax  # Put digit[temp_tenths] into %eax
  orl   %eax, (%rsi)      # Or the bitmask to (%rsi), display pointer

  movzwq %di,  %rax       # Copy %di to $ax, while zeroing out register     
  cmpw  $0, %ax           # Check if non-negative (temp.tenths_degree)
  jge   .RETURN           # Jumps to previous function label with the same name & purpose (returning 0)

  # SET_NEGATIVE
  orl   %edx, (%rsi)      # Or the bitmask to (%rsi), display pointer
  jmp   .RETURN           # Jumps to previous function label with the same name & purpose (returning 0)

.text
.global thermo_update
        
## ENTRY POINT FOR REQUIRED FUNCTION
thermo_update:
	## assembly instructions here
  subq  $4, %rsp          # Adjust stack pointer by 4 bytes to make space for local variables
  movl  $0, (%rsp)        # Put 0 for struct arguments, (%rsp)
  movq  %rsp, %rdi        # Put %rsp pointer as argument 1 for set_temps_from_ports
  call set_temp_from_ports
  movq  (%rsp), %rdi      # Put defrerenced %rsp as argument 1 for set_display_from_temp  
  leaq  THERMO_DISPLAY_PORT(%rip), %rsi # Put defrerenced %rsp as argument 2 for set_display_from_temp
  pushq %rax              # Push return value of set_temp_from_ports to top of stack
  call  set_display_from_temp
  popq  %rdx              # Pop stack and put return value of set_temp_from_port to %rdx
  addl  %edx, %eax        # Add return values of functions
  addq  $4, %rsp          # Restore %rsp
  testl %eax, %eax        
  jz    .RETURN           # Jump if return values of both functions are 0 (returning 0)
  movl  $1, %eax          # 1 into %eax to return
  ret
  