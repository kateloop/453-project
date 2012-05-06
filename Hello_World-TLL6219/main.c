/****************************************************************************
 *
 ****************************************************************************/
#include <stdlib.h>
#include "monlib.h"

#define FREQ_ADDR (*(volatile unsigned long*) 0xD3000008)
#define PLAY(FREQ) (100000000 / (FREQ * 512))

/* frequency definitions (in Hz) */
#define FC3  131
#define FC3S 139
#define FD3F 139
#define FD3  147
#define FD3S 156
#define FE3F 165
#define FE3  175
#define FF3  175
#define FF3S 185
#define FG3F 185
#define FG3  196
#define FG3S 208
#define FA3F 208
#define FA3  220
#define FA3S 233
#define FB3F 233
#define FB3  247
#define FC4  262
#define FC4S 277
#define FD4F 277
#define FD4  294
#define FD4S 311
#define FE4F 311
#define FE4  330
#define FF4  349
#define FF4S 370
#define FG4F 370
#define FG4  392
#define FG4S 415
#define FA4F 415
#define FA4  440
#define FA4S 466
#define FB4F 466
#define FB4  494
#define FC5  523
#define FC5S 554
#define FD5F 554
#define FD5  587
#define FD5S 622
#define FE5F 622
#define FE5  659
#define FF5  698
#define FF5S 740
#define FG5F 740
#define FG5  784
#define FG5S 831
#define FA5F 831
#define FA5  880
#define FA5S 932
#define FB5F 932
#define FB5  988

/* Note definitions */
#define C3  0
#define C3S 1
#define D3F 2
#define D3  3
#define D3S 4
#define E3F 5
#define E3  6
#define F3  7
#define F3S 8
#define G3F 9
#define G3  10
#define G3S 11
#define A3F 12
#define A3  13
#define A3S 14
#define B3F 15
#define B3  16
#define C4  17
#define C4S 18
#define D4F 19
#define D4  20
#define D4S 21
#define E4F 22
#define E4  23
#define F4  24
#define F4S 25
#define G4F 26
#define G4  27
#define G4S 28
#define A4F 29
#define A4  30
#define A4S 31
#define B4F 32
#define B4  33
#define C5  34
#define C5S 35
#define D5F 36
#define D5  37
#define D5S 38
#define E5F 39
#define E5  40
#define F5  41
#define F5S 42
#define G5F 43
#define G5  44
#define G5S 45
#define A5F 46
#define A5  47
#define A5S 48
#define B5F 49
#define B5  50

void playNote(int);

/****************************************************************************
 *
 ****************************************************************************/
int main(int argc, char* argv[])
{
   mon_printf("Testing Sound\n");
   int i, j; 
   int count = 0;
 

  FREQ_ADDR = 100000000 / (512 * 300);
  return 0;
  while(1);


   while(1) {
        for (i = 0; i < 51; i++) {
            playNote(i);
            for (j = 0; j < 5000000; j++) {
             // spin
             count++;
	     if (j >0 && j < 80000) 
		count = j;
            }
        }
   }
   return 0;
}


void playNote (int note) {
    switch (note) {
        case C3 :
            FREQ_ADDR = PLAY(FC3);
            break;
        case C3S :
            FREQ_ADDR = PLAY(FC3S);
            break;
        case D3F :
            FREQ_ADDR = PLAY(FD3S);
            break;
        case D3 :
            FREQ_ADDR = PLAY(FD3);
            break;
        case D3S :
            FREQ_ADDR = PLAY(FD3S);
            break;
        case E3F :
            FREQ_ADDR = PLAY(FE3F);
            break;
        case E3 :
            FREQ_ADDR = PLAY(FE3);
            break;
        case F3 :
                FREQ_ADDR = PLAY(FF3);
                break;
        case F3S :
            FREQ_ADDR = PLAY(FF3S);
            break;
        case G3F :
            FREQ_ADDR = PLAY(FG3F);
            break;
        case G3 :
            FREQ_ADDR = PLAY(FG3);
            break;
        case G3S :
            FREQ_ADDR = PLAY(FG3S);
            break;
        case A3F :
            FREQ_ADDR = PLAY(FA3F);
            break;
        case A3 :
            FREQ_ADDR = PLAY(FA3);
            break;
        case A3S :
            FREQ_ADDR = PLAY(FA3S);
            break;
        case B3F :
            FREQ_ADDR = PLAY(FB3F);
            break;
        case B3 :
            FREQ_ADDR = PLAY(FB3);
            break;
        case C4 :
            FREQ_ADDR = PLAY(FC4);
            break;
        case C4S :
            FREQ_ADDR = PLAY(FC4S);
            break;
        case D4F :
            FREQ_ADDR = PLAY(FD4S);
            break;
        case D4 :
            FREQ_ADDR = PLAY(FD4);
            break;
        case D4S :
            FREQ_ADDR = PLAY(FD4S);
            break;
        case E4F :
            FREQ_ADDR = PLAY(FE4F);
            break;
        case E4 :
            FREQ_ADDR = PLAY(FE4);
            break;
        case F4 :
                FREQ_ADDR = PLAY(FF4);
                break;
        case F4S :
            FREQ_ADDR = PLAY(FF4S);
            break;
        case G4F :
            FREQ_ADDR = PLAY(FG4F);
            break;
        case G4 :
            FREQ_ADDR = PLAY(FG4);
            break;
        case G4S :
            FREQ_ADDR = PLAY(FG4S);
            break;
        case A4F :
            FREQ_ADDR = PLAY(FA4F);
            break;
        case A4 :
            FREQ_ADDR = PLAY(FA4);
            break;
        case A4S :
            FREQ_ADDR = PLAY(FA4S);
            break;
        case B4F :
            FREQ_ADDR = PLAY(FB4F);
            break;
        case B4 :
            FREQ_ADDR = PLAY(FB4);
            break;
        case C5 :
            FREQ_ADDR = PLAY(FC5);
            break;
        case C5S :
            FREQ_ADDR = PLAY(FC5S);
            break;
        case D5F :
            FREQ_ADDR = PLAY(FD5S);
            break;
        case D5 :
            FREQ_ADDR = PLAY(FD5);
            break;
        case D5S :
            FREQ_ADDR = PLAY(FD5S);
            break;
        case E5F :
            FREQ_ADDR = PLAY(FE5F);
            break;
        case E5 :
            FREQ_ADDR = PLAY(FE5);
            break;
        case F5 :
                FREQ_ADDR = PLAY(FF5);
                break;
        case F5S :
            FREQ_ADDR = PLAY(FF5S);
            break;
        case G5F :
            FREQ_ADDR = PLAY(FG5F);
            break;
        case G5 :
            FREQ_ADDR = PLAY(FG5);
            break;
        case G5S :
            FREQ_ADDR = PLAY(FG5S);
            break;
        case A5F :
            FREQ_ADDR = PLAY(FA5F);
            break;
        case A5 :
            FREQ_ADDR = PLAY(FA5);
            break;
        case A5S :
            FREQ_ADDR = PLAY(FA5S);
            break;
        case B5F :
            FREQ_ADDR = PLAY(FB5F);
            break;
        case B5 :
            FREQ_ADDR = PLAY(FB5);
            break;
    }

}
