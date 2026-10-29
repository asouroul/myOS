#include <kernel/tty.h>
#include <kernel/vga.h>

void kernel_main(void) {
     /* Initialize terminal interface */
     terminal_initialize();
 
     /* Newline support is left as an exercise. */
     terminal_writestring("Hello, kernel World!\n");
     terminal_writestring("New Line!\n");
     terminal_writestring("And another one! :O\n");
     terminal_write_status(" OK ", VGA_COLOR_CYAN);
     terminal_writestring("\n");
 
     /* Wipe the screen completely */
     terminal_writestring("\033[2J");
 
     /* Test the bright and standard colors */
     terminal_writestring("\033[96m=== MyOS Boot Sequence ===\033[0m\n\n");
     terminal_writestring("\033[32m[ OK ]\033[0m Base System Loaded\n");
     terminal_writestring("\033[33m[WARN]\033[0m Keyboard not detected\n");
     terminal_writestring("\033[91m[FAIL]\033[0m Network disconnected\n\n");
}   

