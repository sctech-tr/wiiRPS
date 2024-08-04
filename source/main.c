#include <ogc/video.h>
#include <ogc/machine/processor.h>
#include <wiiuse/wpad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gccore.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int get_input() {
    WPAD_ScanPads();
    return WPAD_ButtonsDown(0);
}

int play_round() {
    int player_choice = -1;
    int computer_choice = rand() % 3;

    while (player_choice == -1) {
        int buttons = get_input();
        if (buttons & WPAD_BUTTON_A) {
            player_choice = 0; // rock
        } else if (buttons & WPAD_BUTTON_1) {
            player_choice = 1; // paper
        } else if (buttons & WPAD_BUTTON_2) {
            player_choice = 2; // scissors
        }
    }

    if (player_choice == computer_choice) {
        printf("It's a tie!\n");
	} else if (player_choice == 0 && computer_choice == 2) {
		printf("You win!\n");
		printf("Your choice: rock\n");
		printf("Wii's choice: scissors\n");
    } else if (player_choice == 1 && computer_choice == 0) {
		printf("You win!\n");
		printf("Your choice: paper\n");
		printf("Wii's choice: rock\n");
	} else if (player_choice == 2 && computer_choice == 1) {
		printf("You win!\n");
		printf("Your choice: scissors\n");
		printf("Wii's choice: paper\n");
	} else if (player_choice == 2 && computer_choice == 0) {
		printf("You lose.\n");
		printf("Your choice: scissors\n");
		printf("Wii's choice: rock\n");
	} else if (player_choice == 0 && computer_choice == 1) {
		printf("You lose.\n");
		printf("Your choice: rock\n");
		printf("Wii's choice: paper\n");
	} else if (player_choice == 1 && computer_choice == 2) {
		printf("You lose.\n");
		printf("Your choice: paper\n");
		printf("Wii's choice: scissors\n");
	}
    

    return computer_choice;
}

int main(int argc, char **argv) {
    // initialize ------------
    VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(false);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	printf("\x1b[2;0H"); // send cursor to row 1, line 2
	// initialize ------------
    srand(time(NULL));
    printf("Welcome to wiiRPS!\n");
	printf("Press A to select rock.\n");
	printf("Press 1 to select paper.\n");
	printf("Press 2 to select scissors.\n");
    while (1) {
        int computer_choice = play_round();
        printf("Press A to restart or HOME to exit.\n");
        
        while (1) {
            int buttons = get_input();
            if (buttons & WPAD_BUTTON_A) {
				printf("\033[2J"); // clear the screen
				printf("\x1b[2;0H"); // send cursor to row 1, line 2
				printf("Welcome to wiiRPS!\n");
				printf("Press A to select rock.\n");
				printf("Press 1 to select paper.\n");
				printf("Press 2 to select scissors.\n");
                break; 
            } else if (buttons & WPAD_BUTTON_HOME) {
                exit(0);
            }
        }
    }
	




    return 0;
}