#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Assembly: sub $0x120,%rsp - Allocate 288 bytes (0x120) on stack
    char buffer1[100];  // Username buffer (fgets reads 100 bytes)?
    char buffer2[100];  // Password buffer (fgets reads 100 bytes)?
    char buffer3[41];   // File content buffer (fread reads 41 bytes)?
    FILE *file;         // -0x8(%rbp) - File pointer
    int bytesRead;      // -0xc(%rbp) - Number of bytes read
    
    // Assembly: mov %edi,-0x114(%rbp) - Store argc
    // Assembly: mov %rsi,-0x120(%rbp) - Store argv
    // (These are automatically handled by C)
    
    // Initialize buffer1 with zeros
    // Assembly: lea -0x70(%rbp),%rdx; rep stos %rax,%es:(%rdi)
    memset(buffer1, 0, 12);
    
    // Initialize buffer3 with zeros  
    // Assembly: lea -0xa0(%rbp),%rdx; rep stos %rax,%es:(%rdi)
    memset(buffer3, 0, 12);
    
    // Initialize buffer2 with zeros
    // Assembly: lea -0x110(%rbp),%rdx; rep stos %rax,%es:(%rdi)
    memset(buffer2, 0, 5);
    
    // Initialize variables
    // Assembly: movq $0x0,-0x8(%rbp) - file = NULL
    // Assembly: movl $0x0,-0xc(%rbp) - bytesRead = 0
    file = NULL;
    bytesRead = 0;
    
    // Open file
    // Assembly: mov $0x400bb0,%edx; mov $0x400bb2,%eax; callq fopen@plt
    // 0x400bb0 = "r" (read mode), 0x400bb2 = filename
    file = fopen("/home/users/level03/.pass", "r");  // Replace "filename" with actual filename
    
    // Check if file opened successfully
    // Assembly: cmpq $0x0,-0x8(%rbp); jne 0x4008e6
    if (file == NULL) {
        // Assembly: fwrite error message to stderr
        fprintf(stderr, "ERROR: failed to open password file\n");
        exit(1);
    }
    
    // Read 41 bytes (0x29) from file into buffer3
    // Assembly: lea -0xa0(%rbp),%rax; fread(buffer3, 1, 41, file)
    bytesRead = fread(buffer3, 1, 41, file);
    
    // Remove newline from buffer3
    // Assembly: strcspn(buffer3, "\n"); movb $0x0,...
    buffer3[strcspn(buffer3, "\n")] = '\0';
    
    // Check if we read exactly 41 bytes
    // Assembly: cmpl $0x29,-0xc(%rbp); je 0x40097d
    if (bytesRead != 41) {
        // Assembly: fwrite error messages to stderr
        fprintf(stderr, "ERROR: failed to read password file\n");
        fprintf(stderr, "ERROR: failed to read password file\n");
        exit(1);
    }
    
    // Close the file
    // Assembly: fclose(file)
    fclose(file);
    
    // Print banner messages
    // Assembly: puts("message1"); puts("message2"); etc.
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("******************************************");
    puts("| You must login to access this system. |");
    puts("\\****************************************/");
    
    // Print prompt and read username
    // Assembly: printf("prompt"); fgets(buffer1, 100, stdin)
    printf("--[ Username: ");
    fgets(buffer1, 100, stdin);
    buffer1[strcspn(buffer1, "\n")] = '\0';
    
    // Print prompt and read password
    // Assembly: printf("prompt"); fgets(buffer2, 100, stdin)
    printf("--[ Password: ");
    fgets(buffer2, 100, stdin);
    buffer2[strcspn(buffer2, "\n")] = '\0';
    
    puts("******************");
    
    // Compare password with expected value
    // Assembly: strncmp(buffer2, buffer3, 41)
    if (strncmp(buffer2, buffer3, 41) == 0) {
        printf("Greetings, %s!\n", buffer1);
        system("/bin/sh");
        return(0);
        
    } else {
        // FAILURE PATH
        // Assembly: printf("Failure: %s\n", buffer1)
        printf("Failure: %s\n", buffer1);
        puts(" does not have access!");
        exit(1);
    }
}
