#include "kernel.h"
#include "askr-utils.h"
#include "askr-char.h"

UINT32 VGA_INDEX = 0;
UINT8 fore_color = 0, back_color = 0;

UINT16 VGAEntry(unsigned char ch, UINT8 fc, UINT8 bc) {
    UINT16 ax = 0;
    UINT8 ah = 0, al = 0;
    ah = bc;
    ah <<= 4;
    ah |= fc;
    ax = ah;
    ax <<= 8;
    al = ch;
    ax |= al;
    return ax;
}

void ClearVGABuffer(UINT16 **buffer, UINT8 fc, UINT8 bc) {
    for (int i= 0 ; i < BUFFERSIZE ; i++) {
        (*buffer)[i] = VGAEntry(NULL, fc, bc);
    }
    VGA_INDEX = 0;
}

void InitVGA(UINT8 fc, UINT8 bc) {
    VGA_BUFFER = (UINT16*) VGA_ADDRESS;
    ClearVGABuffer(&VGA_BUFFER, fc, bc);
    fore_color = fc;
    back_color = bc;
}

void printnl() {
    VGA_INDEX += VGA_WIDTH - (VGA_INDEX % VGA_WIDTH);
}

void printch(char ch) {
    VGA_BUFFER[VGA_INDEX] = VGAEntry(ch, fore_color, back_color);
    VGA_INDEX++;
}

void printst(char *str) {
    UINT32 idx = 0;
    while(str[idx]){
        printch(str[idx]);
        idx++;
    }
}

void printi(int n) {
    char strn[digitcount(n)+1];
    itoa(n, strn);
    printst(strn);
}

UINT8 inb(UINT16 port) {
    UINT8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

void outb(UINT16 port, UINT8 data){
    asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}

char getInputKeycode() {
    char ch = 0;
    while((ch = inb(KEYBOARD_PORT)) != 0) {
        if (ch > 0) return ch;
    }
    return ch;
}

void waitForIo(UINT32 timer) {
    while(1) {
        asm volatile("nop");
        timer--;
        if (timer <= 0) break;
    }
}

void sleep(UINT32 timer) {
    waitForIo(timer);
}

void testInput() {
    char ch = 0;
    char keycode = 0;
    do{
        keycode = getInputKeycode();
        if(keycode == KEY_ENTER){
            printnl();
        } else if(keycode == KEY_BACKSPACE && VGA_BUFFER[VGA_INDEX-1] != VGAEntry(NULL, fore_color, back_color)) {
            VGA_INDEX--;
            printch(' ');
            VGA_INDEX--;
        } else {
            ch = getAscii(keycode);
            printch(ch);
        }
        sleep(0x02FFFFFF);
    } while(ch > 0);
}

void KERNEL_MAIN() {

    InitVGA(WHITE, BRIGHT_BLUE);
    printst("Hello world!");
    printnl();
    for(int i = 0 ; i < 100 ; i++){
        printi(i);
    }
    printst("Line wrap test");
    printnl();
    printch('a');
    testInput();

}