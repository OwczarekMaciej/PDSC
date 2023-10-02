#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>

#define PEGS 7
#define DISKS 10
#define DEVIDE_SCREEN gfx_screenWidth() / PEGS
#define FLOOR_HEIGHT 20
#define PEG_HEIGHT gfx_screenHeight() / 3.5
#define PEG_WIDTH 25 / PEGS
#define DISK_WIDTH DEVIDE_SCREEN/ (DISKS*4.0)
#define DISK_HEIGHT (PEG_HEIGHT - 20) / DISKS
#define CONGR_TEXT_WIDTH 100
#define TRY_AGAIN_TEXT_WIDTH 35

int disks_on_pegs[PEGS][DISKS];

void clearScreen(){
	gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);
}

void drawPegsAndFloor()
{
	for (int peg = 0; peg < PEGS; peg++)
		{
			gfx_filledRect(peg * DEVIDE_SCREEN + DEVIDE_SCREEN/2 - PEG_WIDTH, gfx_screenHeight() - PEG_HEIGHT - FLOOR_HEIGHT,
						   peg * DEVIDE_SCREEN + DEVIDE_SCREEN/2 + PEG_WIDTH, gfx_screenHeight(), RED);
		}

	gfx_filledRect(0, gfx_screenHeight() - FLOOR_HEIGHT, gfx_screenWidth(), gfx_screenHeight(), YELLOW);
}

int getkeyToNumber(SDL_Keycode keycode)
{
    if(keycode > SDLK_0 && keycode <= SDLK_9) 
	{
        return keycode - SDLK_0;
    } else if (keycode == SDLK_0) 
	{
        return 10;
    } else {
        return -1;
    }
}

void initialTableFilling(int disks_on_pegs[PEGS][DISKS])
{
	for (int disk = 1; disk <= DISKS; disk++)
	{
		disks_on_pegs[0][disk-1] = disk;
	}
	for (int peg = 1; peg < PEGS; peg++) 
	{
        for (int disk = 0; disk < DISKS; disk++) 
		{
            disks_on_pegs[peg][disk] = 0;
        }
    }
}

void drawDisks(int disks_on_pegs[PEGS][DISKS])
{
	for (int peg = 0; peg < PEGS; peg++)
	{
		for (int disk = DISKS; disk > 0; disk--)
		{
			if (disks_on_pegs[peg][disk - 1])
			{
				gfx_filledRect(peg * DEVIDE_SCREEN + DEVIDE_SCREEN/2 - PEG_WIDTH - (DISK_WIDTH *(disks_on_pegs[peg][disk - 1])), 
						       gfx_screenHeight() - FLOOR_HEIGHT - (DISKS - disk + 1)*DISK_HEIGHT,
						       peg * DEVIDE_SCREEN + DEVIDE_SCREEN/2 + PEG_WIDTH + (DISK_WIDTH *(disks_on_pegs[peg][disk - 1])), 
						       gfx_screenHeight() - FLOOR_HEIGHT - (DISKS - disk)*DISK_HEIGHT, CYAN);
			}
		}
	}
}

void diskMovement(int disk_move_height, int disk_move_width, int temporary_var, int keycode1, int disk1)
{
	gfx_filledRect((keycode1 - 1) * DEVIDE_SCREEN + DEVIDE_SCREEN/2 - PEG_WIDTH - (DISK_WIDTH *(temporary_var)) + disk_move_width, 
					gfx_screenHeight() - FLOOR_HEIGHT - (DISKS - disk1 + 1)*DISK_HEIGHT + disk_move_height,
					(keycode1 - 1) * DEVIDE_SCREEN + DEVIDE_SCREEN/2 + PEG_WIDTH + (DISK_WIDTH *(temporary_var)) + disk_move_width, 
					gfx_screenHeight() - FLOOR_HEIGHT - (DISKS - disk1)*DISK_HEIGHT + disk_move_height, CYAN);
	SDL_Delay(2);
}

void endingScreen(int disks_on_pegs[PEGS][DISKS])
{
	clearScreen();
	drawPegsAndFloor();
	drawDisks(disks_on_pegs);
	gfx_textout(gfx_screenWidth()/2 - CONGR_TEXT_WIDTH, gfx_screenHeight()/2, "CONGRATULATIONS! YOU WON", RED);
	gfx_updateScreen();
	SDL_Delay(1000);				
}

void tryAgain(int disks_on_pegs[PEGS][DISKS])
{
	clearScreen();
	drawPegsAndFloor();
	drawDisks(disks_on_pegs);
	gfx_textout(gfx_screenWidth()/2 - TRY_AGAIN_TEXT_WIDTH, gfx_screenHeight()/2, "TRY AGAIN", RED);
	gfx_updateScreen();
	SDL_Delay(2000);
	initialTableFilling(disks_on_pegs);	
}

void displayingAnimation(int disks_on_pegs[PEGS][DISKS], int disk_move_height, int disk_move_width, int temporary_var, int keycode1, int disk1){
	clearScreen();
	drawPegsAndFloor();
	drawDisks(disks_on_pegs);
	diskMovement(disk_move_height, disk_move_width, temporary_var, keycode1, disk1);
	gfx_updateScreen();
	gfx_pollkey();
}

void game_logic(SDL_KeyCode keycode1, SDL_KeyCode keycode2){

	while(1){

		clearScreen();
		drawPegsAndFloor();
		drawDisks(disks_on_pegs);
		
		gfx_updateScreen();

		keycode1 = gfx_getkey();
		if(keycode1 == SDLK_BACKSPACE)
		{
			tryAgain(disks_on_pegs);
		}
		keycode1 = getkeyToNumber(keycode1);

		if(keycode1 > 0 && keycode1 <= PEGS)
		{
			keycode2 = gfx_getkey();
			if(keycode2 == SDLK_BACKSPACE)
			{
			tryAgain(disks_on_pegs);
			}
			keycode2 = getkeyToNumber(keycode2);

			if(keycode2 > 0 && keycode2 <= PEGS)
			{
				for (int disk1 = 0; disk1 < DISKS; disk1++)
				{
					if(disks_on_pegs[keycode1 - 1][disk1] != 0){
						for (int disk2 = 0; disk2 < DISKS; disk2++)
						{
							if(disks_on_pegs[keycode2 - 1][disk2] == 0 && (disk2 == DISKS - 1 ||
								(disks_on_pegs[keycode2 - 1][disk2 + 1] > disks_on_pegs[keycode1 - 1][disk1])))
								{
									int disk_move_height = 0;
									int disk_move_width = 0;
									int anitamted_disk_value = 0;
									anitamted_disk_value = disks_on_pegs[keycode1 - 1][disk1];
									disks_on_pegs[keycode1 - 1][disk1] = 0;
									
									while ((gfx_screenHeight() - FLOOR_HEIGHT - (DISKS - disk1 + 1)*DISK_HEIGHT + disk_move_height) >= gfx_screenHeight()/2)
									{
										displayingAnimation(disks_on_pegs, disk_move_height, disk_move_width, anitamted_disk_value, keycode1, disk1);
										disk_move_height --;
									}
									while (((keycode1 - 1) * DEVIDE_SCREEN + DEVIDE_SCREEN/2 + disk_move_width) <= (keycode2 - 1) * DEVIDE_SCREEN + DEVIDE_SCREEN/2)
									{
										displayingAnimation(disks_on_pegs, disk_move_height, disk_move_width, anitamted_disk_value, keycode1, disk1);
										disk_move_width ++;
									}
									while (((keycode1 - 1) * DEVIDE_SCREEN + DEVIDE_SCREEN/2 + disk_move_width) >= (keycode2 - 1) * DEVIDE_SCREEN + DEVIDE_SCREEN/2)
									{
										displayingAnimation(disks_on_pegs, disk_move_height, disk_move_width, anitamted_disk_value, keycode1, disk1);
										disk_move_width --;
									}
									while ((gfx_screenHeight() - FLOOR_HEIGHT - (DISKS - disk1 + 1)*DISK_HEIGHT + disk_move_height) <= 
										   (gfx_screenHeight() - FLOOR_HEIGHT - (DISKS - disk2)*DISK_HEIGHT))
									{
										displayingAnimation(disks_on_pegs, disk_move_height, disk_move_width, anitamted_disk_value, keycode1, disk1);
										disk_move_height ++;
									}
								disks_on_pegs[keycode2 - 1][disk2] = anitamted_disk_value;
								
								break;
							}
						}		
					}
				}
			}
		}
		if(disks_on_pegs[PEGS - 1][0] == 1)
		{
			endingScreen(disks_on_pegs);
			break;
		}
		gfx_pollkey();
	}
}

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	SDL_KeyCode keycode1 = -1;
	SDL_KeyCode keycode2 = -1;

	initialTableFilling(disks_on_pegs);
	
	game_logic(keycode1, keycode2);

	return 0;
}
