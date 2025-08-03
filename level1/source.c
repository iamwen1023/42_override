#include <stdio.h>
#include <string.h>

// Global variables (addresses from assembly)
char *banner_msg = "********* ADMIN LOGIN PROMPT *********\n";
char *username_prompt = "Enter Username:";
char *password_prompt = "verifying username....\n\nEnter Password: ";
char *error_msg = "nope, incorrect username.\n";
char *success_msg = "nope, incorrect password.\n";

// Hardcoded strings (addresses from assembly)
char *expected_username = "dat_wil";  // 0x80486a8, 7 bytes
char *expected_password = "admin"; // 0x80486b0, 5 bytes (only first 5 bytes compared)

// Global buffers
char username_buffer[256];  // 0x804a040
FILE *stdin_ptr;           // 0x804a020

int verify_user_name() {
    puts("verif_user: ");
    
    // String comparison using repz cmpsb equivalent
    // Returns: 0 if equal, 1 if user > expected, -1 if user < expected
    return strncmp(username_buffer, expected_username, 7);
}

int verify_user_pass(char *password) {
    // String comparison using repz cmpsb equivalent
    // Only compares first 5 bytes of password
    // Returns: 0 if equal, 1 if user > expected, -1 if user < expected
    return strncmp(password, expected_password, 5);
}

int main() {
    char password_buffer[16];  // Local buffer at %esp + 0x1c
    int result = 0;           // Return value at %esp + 0x5c
    
    // Initialize password buffer with zeros (rep stos equivalent)
    memset(password_buffer, 0, 16);
    
    // Display banner
    puts(banner_msg);
    
    // Prompt for username
    printf("%s", username_prompt);
    
    // Read username
    fgets(username_buffer, 256, stdin);
    
    // Verify username
    result = verify_user_name();
    if (result != 0) {
        puts(error_msg);
        return 1;
    }
    
    // Prompt for password
    puts(password_prompt);
    
    // Read password
    fgets(password_buffer, 100, stdin);
    
    // Verify password
    result = verify_user_pass(password_buffer);
    if (result != 0) {
        puts(success_msg);  // Note: this prints "nope, incorrect password" on failure
        return 1;
    }
    
    // Success - this would normally spawn a shell
    // But in this case, it falls through to return 0
    return 0;
}
