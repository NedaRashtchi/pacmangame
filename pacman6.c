// Pacman Game in C language 
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
//#include <time.h>
//#include <windows.h>

// All the elements to be used 
// Declared here 
#define WIDTH 40 
#define HEIGHT 20 
#define PACMAN 'C' 
#define WALL '#' 
#define FOOD '.' 
#define EMPTY ' ' 
#define DEMON 'X' 
#define BONUS '$'
#define ENEMY 'E'

typedef struct{
	char type;
}cel;

// Global Variables are 
// Declared here 
cel board[HEIGHT][WIDTH];
int res = 0; 
int score = 0; 
int pacman_x, pacman_y; 
int enemy_x, enemy_y;
int food = 0;
int bonus = 0; 
int curr = 0; 
int powermove = 0;



void savegame() {
    FILE *file = fopen("myfile.bin", "wb");
    if (file == NULL){
        perror("Failed to open file ");
		fclose(file);
        return;
    }
    fwrite(board, sizeof(cel), HEIGHT*WIDTH, file);
    fwrite(&score , sizeof(int), 1, file);
    fwrite(&pacman_x , sizeof(int), 1, file);
    fwrite(&pacman_y , sizeof(int), 1, file);
    fwrite(&enemy_x , sizeof(int), 1, file);
    fwrite(&enemy_y , sizeof(int), 1, file);
    fwrite(&score , sizeof(int), 1, file);
    fwrite(&food , sizeof(int), 1, file);
    fwrite(&bonus , sizeof(int), 1, file);
    fwrite(&curr , sizeof(int), 1, file);
    fwrite(&powermove , sizeof(int), 1, file);
    fclose(file);
    printf("\nData saved successfully.\n");
}

void loadgame() {
    FILE *file = fopen("myfile.bin", "rb");
    if (file == NULL) {
        perror("Failed to open file ");
		fclose(file);
        return;
    }
    fread(board, sizeof(cel),HEIGHT*WIDTH, file);
    fread(&score , sizeof(int), 1, file);
    fread(&pacman_x , sizeof(int), 1, file);
    fread(&pacman_y , sizeof(int), 1, file);
    fread(&enemy_x , sizeof(int), 1, file);
    fread(&enemy_y , sizeof(int), 1, file);
    fread(&score , sizeof(int), 1, file);
    fread(&food , sizeof(int), 1, file);
    fread(&bonus , sizeof(int), 1, file);
    fread(&curr , sizeof(int), 1, file);
    fread(&powermove , sizeof(int), 1, file);
    fclose(file);
    printf("\nData loaded successfully.\n");
}

void initialize() 
{ 
	// Putting Walls as boundary in the Game 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			if (i == 0 || j == WIDTH - 1 || j == 0 || i == HEIGHT - 1) { 
				board[i][j].type = WALL; 
			} 
			else{
				board[i][j].type = EMPTY;
			}
				 
		} 
	} 

	// Putting Walls inside the Game 
	int count = 50; 
	while (count != 0) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1)); 
		//int i = rand() % HEIGHT ; 
		//int j = rand() % WIDTH ; 

		if (board[i][j].type != WALL && board[i][j].type != PACMAN) { 
			board[i][j].type = WALL; 
			count--; 
		} 
	} 

	int val = 5; 
	while (val--) { 
		int row = (rand() % (HEIGHT + 1)); 
		//int row = rand() % HEIGHT;
		for (int j = 3; j < WIDTH - 3; j++) { 
			if (board[row][j].type != WALL && board[row][j].type != PACMAN) { 
				board[row][j].type = WALL; 
			} 
		} 
	} 

	// Putting Demons in the Game 
	count = 10; 
	while (count != 0) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1));
		//int i = rand() % HEIGHT;
        //int j = rand() % WIDTH;
 

		if (board[i][j].type != WALL && board[i][j].type != PACMAN) { 
			board[i][j].type = DEMON; 
			count--; 
		} 
	} 

	// Cursor at Center 
	pacman_x = WIDTH / 2; 
	pacman_y = HEIGHT / 2; 
	board[pacman_y][pacman_x].type = PACMAN; 

    //placing the enemy
   
	while (1) { 
		//int i = (rand() % (HEIGHT + 1)); 
		//int j = (rand() % (WIDTH + 1)); 
		int i = rand() % HEIGHT;
        int j = rand() % WIDTH;

		if (board[i][j].type != WALL && board[i][j].type != PACMAN && board[i][j].type != DEMON) { 
			board[i][j].type = ENEMY; 
            enemy_y = i ;
            enemy_x = j ;
			break; 
		} 
	}

	// Putting bonus 
	count = 10; 
	while (count != 0) { 
		//int i = (rand() % (HEIGHT + 1)); 
		//int j = (rand() % (WIDTH + 1));
		int i = rand() % HEIGHT;
        int j = rand() % WIDTH; 

		if (board[i][j].type != WALL && board[i][j].type != PACMAN && board[i][j].type != DEMON && board[i][j].type != ENEMY) { 
			board[i][j].type = BONUS; 
			count--; 
            bonus++;
		} 
	} 

	// Points Placed 
	food = 0 ;
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			if (i % 2 == 1 && j % 2 == 1 //food khone haye fard 
				&& board[i][j].type != WALL 
				&& board[i][j].type != DEMON 
				&& board[i][j].type != PACMAN
                && board[i][j].type != BONUS
                && board[i][j].type != ENEMY) { 

				board[i][j].type = FOOD; 
				food++; 
			} 
		} 
	} food -= 85;
	
} 

void draw() 
{ 
	// Clear screen 
	system("cls"); 

	// Drawing All the elements in the screen 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			printf("%c", board[i][j].type); 
		} 
		printf("\n"); 
	} 
	printf("Score: %d\n", score); 
} 

// Function enables to move the Cursor 
void move(int move_x, int move_y) 
{ 
	int x = pacman_x + move_x; 
	int y = pacman_y + move_y; 

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && board[y][x].type != WALL) { 
		if (board[y][x].type == FOOD) { 
			score++; 
			food--; 
			curr++; 
			if (food == 0) { 
				res = 2; 
				return; 
			} 
		} 
		else if (board[y][x].type == DEMON || board[y][x].type == ENEMY) { 
			res = 1; 
		}
        else if(board[y][x].type == BONUS){
            powermove += 10;
        } 

		board[pacman_y][pacman_x].type = EMPTY; 
		pacman_x = x; 
		pacman_y = y; 
		board[pacman_y][pacman_x].type = PACMAN; 
	} 
} 
void enemymove(){
	// 1 for up , 2 for left, 3 for down , 4 for right
	while(1){
        //srand(time(NULL));
		int move = rand() % 4 + 1 ;
		if(move == 1 && (enemy_y+1) < HEIGHT){ // shart kharej nashodan az divar
			if(board[enemy_y + 1][enemy_x].type != WALL && board[enemy_y + 1][enemy_x].type != FOOD
			&& board[enemy_y + 1][enemy_x].type != DEMON && board[enemy_y + 1][enemy_x].type != BONUS)
			{ board[enemy_y][enemy_x].type = EMPTY ;
			  enemy_y++;
			  board[enemy_y][enemy_x].type = ENEMY ;
			  break;
			}
		}
		else if(move == 2 && (enemy_x-1) >= 0){
			if(board[enemy_y][enemy_x - 1].type != WALL && board[enemy_y][enemy_x - 1].type != FOOD
			&& board[enemy_y][enemy_x - 1].type != DEMON && board[enemy_y][enemy_x - 1].type != BONUS)
			{ board[enemy_y][enemy_x].type = EMPTY ;
			  enemy_x--;
			  board[enemy_y][enemy_x].type = ENEMY ;
			  break;
			}
		}
		else if(move == 3 && (enemy_y-1) >= 0){
			if(board[enemy_y - 1][enemy_x].type != WALL && board[enemy_y - 1][enemy_x].type != FOOD
			&& board[enemy_y - 1][enemy_x].type != DEMON && board[enemy_y - 1][enemy_x].type != BONUS)
			{ board[enemy_y][enemy_x].type = EMPTY ;
			  enemy_y--;
			  board[enemy_y][enemy_x].type = ENEMY ;
			  break;
			}
		}
		else if(move == 4 && (enemy_x+1) < WIDTH){
			if(board[enemy_y][enemy_x + 1].type != WALL && board[enemy_y][enemy_x + 1].type != FOOD
			&& board[enemy_y][enemy_x + 1].type != DEMON && board[enemy_y][enemy_x + 1].type != BONUS)
			{ board[enemy_y][enemy_x].type = EMPTY ;
			  enemy_x++;
			  board[enemy_y][enemy_x].type = ENEMY ;
			  break;
			}
		}
	}

}

void play(){ // 1 for up , 2 for left, 3 for down , 4 for right
	while(res != 2){
		//srand(time(NULL));
		enemymove();
		int move = rand() % 4 + 1 ;
		
		if(move==1 && pacman_y + 1 < HEIGHT ){
			//usleep(500);
			if(board[pacman_y+1][pacman_x].type == FOOD || board[pacman_y+1][pacman_x].type == EMPTY ){
				if(board[pacman_y+1][pacman_x].type == FOOD){
					score++; 
					food--; 
					curr++; 
					if(food == 0) { 
					res = 2;  
					}
					usleep(100);
				}
				board[pacman_y][pacman_x].type = EMPTY;
				pacman_y++;
				board[pacman_y][pacman_x].type = PACMAN;
				draw();
			}
		}
		else if(move==2 && (pacman_x-1) >= 0){
			//usleep(500);
			if(board[pacman_y][pacman_x-1].type == FOOD || board[pacman_y][pacman_x-1].type == EMPTY ){
				if(board[pacman_y][pacman_x-1].type == FOOD){
					score++; 
					food--; 
					curr++; 
					if(food == 0) { 
					res = 2;  
					}
					usleep(100);
				}
				board[pacman_y][pacman_x].type = EMPTY;
				pacman_x--;
				board[pacman_y][pacman_x].type = PACMAN;
				draw();
			}
		}
		else if(move==3 && (pacman_y-1) >= 0 ){
			//usleep(500);
			if(board[pacman_y-1][pacman_x].type == FOOD || board[pacman_y-1][pacman_x].type == EMPTY ){
				if(board[pacman_y-1][pacman_x].type == FOOD){
					score++; 
					food--; 
					curr++; 
					if(food == 0) { 
					res = 2; 
					//return; 
					}
					usleep(100);
				}
				board[pacman_y][pacman_x].type = EMPTY;
				pacman_y--;
				board[pacman_y][pacman_x].type = PACMAN;
				draw();
			}
		}
		else if(move==4 && (pacman_x+1) < WIDTH ){
			//usleep(500);
			if(board[pacman_y][pacman_x+1].type == FOOD || board[pacman_y][pacman_x+1].type == EMPTY ){
				if(board[pacman_y][pacman_x+1].type == FOOD){
					score++; 
					food--; 
					curr++; 
					if(food == 0) { 
					res = 2; 
					//return; 
					}
					usleep(100);
				}
				board[pacman_y][pacman_x].type = EMPTY;
				pacman_x++;
				board[pacman_y][pacman_x].type = PACMAN;
				draw();
			}
		}
		
	}
}


// Main Function 
int main() 
{ 
	printf("Do you want to ?\n1.start a new game.\n2.Continue the last game.\n ");
    int choice;
    scanf("%d" , &choice);
    if(choice != 1 && choice != 2){
        printf("Invalid selection.");
        return 1;
    }else if(choice == 1){
        initialize(); 
    }else if(choice == 2){
        loadgame("testing.bin");
    }
	
	char ch; 
	//food -= 35; 
	int totalFood = food;
    int totalbonus = bonus;
    curr = 0 ;
	res = 0;
	// Instructions to Play 
	printf(" Use buttons for w(up), a(left) , d(right) and "
		"s(down)\nAlso, Press 'q' for quit\nand, press 'p' if you want the computer to play.\n"); 

	printf("Enter Y to continue: \n"); 

	ch = getch(); 
	if (ch != 'Y' && ch != 'y') { 
		printf("Exit Game! "); 
		return 1; 
	} 

	while (1) { 
		draw(); 
        enemymove();

		printf("Total Food count: %d\n", totalFood); 
		printf("Total Food eaten: %d\n", curr); 
        printf("Total Bonus count: %d\n" , totalbonus);
        if(powermove > 0) printf("Power moves: %d\n" , powermove);
		if (res == 1) { 
			// Clear screen 
			system("cls"); 
			printf("Game Over! Dead by Demon or Enemy\n Your Score: "
				"%d\n", 
				score); 
			return 1; 
		} 

		if (res == 2) { 
			// Clear screen 
			system("cls"); 
			printf("You Win! \n Your Score: %d\n", score); 
			return 1; 
		} 

		// Taking the Input from the user 
		ch = getch(); 

		// Moving According to the 
		// input character
        if(powermove > 0){ 
		switch (ch) { 
		case 'w': 
			move(0, -2);
            powermove--; 
			break; 
		case 's': 
			move(0, 2);
            powermove--;  
			break; 
		case 'a': 
			move(-2, 0); 
            powermove--; 
			break; 
		case 'd': 
			move(2, 0); 
            powermove--; 
			break; 
		case 'q': 
			printf("Do you want to save the game? (y/n)");
			if(getch() == 'y'){
				savegame("filename.bin");
				return 0;
			}
			printf("Game Over! Your Score: %d\n", score); 
			return 0;
		case 'p':
			play();
			break;	 
		} 
	    } 

        else if(powermove <= 0){ 
		switch (ch) { 
		case 'w': 
			move(0, -1); 
			break; 
		case 's': 
			move(0, 1); 
			break; 
		case 'a': 
			move(-1, 0); 
			break; 
		case 'd': 
			move(1, 0); 
			break; 
		case 'q': 
			printf("Do you want to save the game? (y/n)");
			if(getch() == 'y'){
				savegame("filename.bin");
				return 0;
			}
			printf("Game Over! Your Score: %d\n", score); 
			return 0;
		case 'p':
			play();
			break;	
		} 
	    } 
        
	    
    }

	return 0; 
}