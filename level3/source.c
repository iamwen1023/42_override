#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main() {
    int user_input;
    
    // Assembly: push %ebp; mov %esp,%ebp; and $0xfffffff0,%esp; sub $0x20,%esp
    // Function prologue: Save base pointer, set up stack frame, align stack, allocate 32 bytes
    
    // Assembly: push %eax; xor %eax,%eax; je 0x804886b; add $0x4,%esp; pop %eax
    // Stack canary check (security feature to detect stack overflow)
    
    // Assembly: movl $0x0,(%esp); call time@plt
    // time(NULL) - Get current timestamp as seed for random number generator
    time(NULL);
    
    // Assembly: mov %eax,(%esp); call srand@plt  
    // srand(time(NULL)) - Seed the random number generator with current time
    srand(time(NULL));
    
    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password:");
    
    // Assembly: mov $0x8048a85,%eax; lea 0x1c(%esp),%edx; mov %edx,0x4(%esp); mov %eax,(%esp); call __isoc99_scanf@plt
    // scanf("%d", &user_input) - Read integer input from user
    scanf("%d", &user_input);
    
    // Assembly: mov 0x1c(%esp),%eax; movl $0x1337d00d,0x4(%esp); mov %eax,(%esp); call test
    // test(user_input, 0x1337d00d) - Call test function with user input and magic number
    test(user_input, 0x1337d00d);
    
    // Assembly: mov $0x0,%eax; leave; ret
    // return 0 - Return from main function
    return 0;
}

void test(int user_input, int expected_value) {
    int difference;
    
    // Assembly: push %ebp; mov %esp,%ebp; sub $0x28,%esp
    // Function prologue: Save base pointer, set up stack frame, allocate 40 bytes
    
    // Assembly: mov 0x8(%ebp),%eax; mov 0xc(%ebp),%edx; mov %edx,%ecx; sub %eax,%ecx; mov %ecx,%eax; mov %eax,-0xc(%ebp)
    // difference = expected_value - user_input
    difference = expected_value - user_input;
    
    // Assembly: cmpl $0x15,-0xc(%ebp); ja 0x804884a
    // if (difference > 21) goto default_case
    if (difference > 21) {
        // Assembly: call rand@plt; mov %eax,(%esp); call decrypt
        // decrypt(rand()) - Use random number as key
        decrypt(rand());
        return;
    }
    
    // Assembly: mov -0xc(%ebp),%eax; shl $0x2,%eax; add $0x80489f0,%eax; mov (%eax),%eax; jmp *%eax
    // Jump table implementation - switch statement based on difference value
    // This is a switch statement with 22 cases (0-21)
    switch (difference) {
        case 0:
            decrypt(difference);
            break;
        case 1:
            decrypt(difference);
            break;
        case 2:
            decrypt(difference);
            break;
        case 3:
            decrypt(difference);
            break;
        case 4:
            decrypt(difference);
            break;
        case 5:
            decrypt(difference);
            break;
        case 6:
            decrypt(difference);
            break;
        case 7:
            decrypt(difference);
            break;
        case 8:
            decrypt(difference);
            break;
        case 9:
            decrypt(difference);
            break;
        case 10:
            decrypt(difference);
            break;
        case 11:
            decrypt(difference);
            break;
        case 12:
            decrypt(difference);
            break;
        case 13:
            decrypt(difference);
            break;
        case 14:
            decrypt(difference);
            break;
        case 15:
            decrypt(difference);
            break;
        case 16:
            decrypt(difference);
            break;
        case 17:
            decrypt(difference);
            break;
        case 18:
            decrypt(difference);
            break;
        case 19:
            decrypt(difference);
            break;
        case 20:
            decrypt(difference);
            break;
        case 21:
            decrypt(difference);
            break;
        default:
            // Assembly: call rand@plt; mov %eax,(%esp); call decrypt
            decrypt(rand());
            break;
    }
    
    // Assembly: leave; ret
    // Return from function
}

void decrypt(int key) {
    char encrypted_string[29];  // -0x1d(%ebp) = 29 bytes
    int string_length;
    int i;
    
    // Assembly: push %ebp; mov %esp,%ebp; push %edi; push %esi; sub $0x40,%esp
    // Function prologue: Save base pointer, save registers, allocate 64 bytes
    
    // Assembly: mov %gs:0x14,%eax; mov %eax,-0xc(%ebp); xor %eax,%eax
    // Stack canary setup (security feature)
    
    // Assembly: movl $0x757c7d51,-0x1d(%ebp); movl $0x67667360,-0x19(%ebp); movl $0x7b66737e,-0x15(%ebp); movl $0x33617c7d,-0x11(%ebp); movb $0x0,-0xd(%ebp)
    // Initialize encrypted string with hardcoded values
    // This is the encrypted flag/password
    strcpy(encrypted_string, "Q}|u`sfg~sf{}|a3");  // Decoded from the hex values
    
    // Assembly: lea -0x1d(%ebp),%eax; movl $0xffffffff,-0x2c(%ebp); mov %eax,%edx; mov $0x0,%eax; mov -0x2c(%ebp),%ecx; mov %edx,%edi; repnz scas %es:(%edi),%al; mov %ecx,%eax; not %eax; sub $0x1,%eax; mov %eax,-0x24(%ebp)
    // string_length = strlen(encrypted_string)
    string_length = strlen(encrypted_string);
    
    // Assembly: movl $0x0,-0x28(%ebp); jmp 0x80486e5
    // i = 0; goto loop_condition
    i = 0;
    
    // Decryption loop
    while (i < string_length) {
        // Assembly: lea -0x1d(%ebp),%eax; add -0x28(%ebp),%eax; movzbl (%eax),%eax; mov %eax,%edx; mov 0x8(%ebp),%eax; xor %edx,%eax; mov %eax,%edx; lea -0x1d(%ebp),%eax; add -0x28(%ebp),%eax; mov %dl,(%eax)
        // encrypted_string[i] = encrypted_string[i] ^ key
        encrypted_string[i] = encrypted_string[i] ^ key;
        
        // Assembly: addl $0x1,-0x28(%ebp)
        // i++
        i++;
    }
    
    // Assembly: lea -0x1d(%ebp),%eax; mov %eax,%edx; mov $0x80489c3,%eax; mov $0x11,%ecx; mov %edx,%esi; mov %eax,%edi; repz cmpsb %es:(%edi),%ds:(%esi); seta %dl; setb %al; mov %edx,%ecx; sub %al,%cl; mov %ecx,%eax; movsbl %al,%eax; test %eax,%eax; jne 0x8048723
    // if (strcmp(encrypted_string, "Congratulations!") == 0)
    if (strcmp(encrypted_string, "Congratulations!") == 0) {
        // Assembly: movl $0x80489d4,(%esp); call system@plt
        // system("/bin/sh") - Execute shell
        system("/bin/sh");
    } else {
        // Assembly: movl $0x80489dc,(%esp); call puts@plt
        // puts("Invalid Password") - Print error message
        puts("Invalid Password");
    }
    
    // Assembly: mov -0xc(%ebp),%esi; xor %gs:0x14,%esi; je 0x8048740; call __stack_chk_fail@plt
    // Stack canary check (security feature)
    
    // Assembly: add $0x40,%esp; pop %esi; pop %edi; pop %ebp; ret
    // Function epilogue: restore registers, deallocate stack, return
}
