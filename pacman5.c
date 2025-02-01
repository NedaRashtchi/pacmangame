// Pacman Game in C language 
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h>
//#include <time.h>

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


// Global Variables are 
// Declared here 
int res = 0; 
int score = 0; 
int pacman_x, pacman_y; 
int enemy_x, enemy_y;
char board[HEIGHT][WIDTH]; 
int food = 0;
int bonus = 0; 
int curr = 0; 
int powermove = 0;

void savegame(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL){
        perror("Failed to open file ");
        return;
    }
    fwrite(board, sizeof(int), HEIGHT*WIDTH, file);
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

void loadgame(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file ");
        return;
    }
    fread(board, sizeof(int),HEIGHT*WIDTH, file);
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
			if (i == 0 || j == WIDTH - 1 || j == 0 
				|| i == HEIGHT - 1) { 
				board[i][j] = WALL; 
			} 
			else
				board[i][j] = EMPTY; 
		} 
	} 

	// Putting Walls inside the Game 
	int count = 50; 
	while (count != 0) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1)); 

		if (board[i][j] != WALL && board[i][j] != PACMAN) { 
			board[i][j] = WALL; 
			count--; 
		} 
	} 

	int val = 5; 
	while (val--) { 
		int row = (rand() % (HEIGHT + 1)); 
		for (int j = 3; j < WIDTH - 3; j++) { 
			if (board[row][j] != WALL 
				&& board[row][j] != PACMAN) { 
				board[row][j] = WALL; 
			} 
		} 
	} 

	// Putting Demons in the Game 
	count = 10; 
	while (count != 0) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1)); 

		if (board[i][j] != WALL && board[i][j] != PACMAN) { 
			board[i][j] = DEMON; 
			count--; 
		} 
	} 

	// Cursor at Center 
	pacman_x = WIDTH / 2; 
	pacman_y = HEIGHT / 2; 
	board[pacman_y][pacman_x] = PACMAN; 

    //placing the enemy
   
	while (1) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1)); 

		if (board[i][j] != WALL && board[i][j] != PACMAN && board[i][j] != DEMON) { 
			board[i][j] = ENEMY; 
            enemy_y = i ;
            enemy_x = j ;
			break; 
		} 
	}

	// Putting bonus 
	count = 10; 
	while (count != 0) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1)); 

		if (board[i][j] != WALL && board[i][j] != PACMAN && board[i][j] != DEMON && board[i][j] != ENEMY) { 
			board[i][j] = BONUS; 
			count--; 
            bonus++;
		} 
	} 

	// Points Placed 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			if (i % 2 == 0 && j % 2 == 0 
				&& board[i][j] != WALL 
				&& board[i][j] != DEMON 
				&& board[i][j] != PACMAN
                && board[i][j] != BONUS
                && board[i][j] != ENEMY) { 

				board[i][j] = FOOD; 
				food++; 
			} 
		} 
	} food-=35;
} 

void draw() 
{ 
	// Clear screen 
	system("cls"); 

	// Drawing All the elements in the screen 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			printf("%c", board[i][j]); 
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

	if (board[y][x] != WALL) { 
		if (board[y][x] == FOOD) { 
			score++; 
			food--; 
			curr++; 
			if (food == 0) { 
				res = 2; 
				return; 
			} 
		} 
		else if (board[y][x] == DEMON || board[y][x] == ENEMY) { 
			res = 1; 
		}
        else if(board[y][x] == BONUS){
            powermove += 10;
        } 

		board[pacman_y][pacman_x] = EMPTY; 
		pacman_x = x; 
		pacman_y = y; 
		board[pacman_y][pacman_x] = PACMAN; 
	} 
} 
void enemymove(){
	// 1 for up , 2 for left, 3 for down , 4 for right
	while(1){
        //srand(time(NULL));
		int move = rand() % 4 + 1 ;
		if(move == 1){
			if(board[enemy_y + 1][enemy_x] != WALL && board[enemy_y + 1][enemy_x] != FOOD
			&& board[enemy_y + 1][enemy_x] != DEMON && board[enemy_y + 1][enemy_x] != BONUS)
			{ board[enemy_y][enemy_x] = EMPTY ;
			  enemy_y++;
			  board[enemy_y][enemy_x] = ENEMY ;
			  break;
			}
		}
		else if(move == 2){
			if(board[enemy_y][enemy_x - 1] != WALL && board[enemy_y][enemy_x - 1] != FOOD
			&& board[enemy_y][enemy_x - 1] != DEMON && board[enemy_y][enemy_x - 1] != BONUS)
			{ board[enemy_y][enemy_x] = EMPTY ;
			  enemy_x--;
			  board[enemy_y][enemy_x] = ENEMY ;
			  break;
			}
		}
		else if(move == 3){
			if(board[enemy_y - 1][enemy_x] != WALL && board[enemy_y - 1][enemy_x] != FOOD
			&& board[enemy_y - 1][enemy_x] != DEMON && board[enemy_y - 1][enemy_x] != BONUS)
			{ board[enemy_y][enemy_x] = EMPTY ;
			  enemy_y--;
			  board[enemy_y][enemy_x] = ENEMY ;
			  break;
			}
		}
		else if(move == 4){
			if(board[enemy_y][enemy_x + 1] != WALL && board[enemy_y][enemy_x + 1] != FOOD
			&& board[enemy_y][enemy_x + 1] != DEMON && board[enemy_y][enemy_x + 1] != BONUS)
			{ board[enemy_y][enemy_x] = EMPTY ;
			  enemy_x++;
			  board[enemy_y][enemy_x] = ENEMY ;
			  break;
			}
		}
	}

}

void play(){ // 1 for up , 2 for left, 3 for down , 4 for right
	while(res != 2){
		//srand(time(NULL));
		int move = rand() % 4 + 1 ;

		if(move==1){
			if(board[pacman_y+1][pacman_x] == FOOD || board[pacman_y+1][pacman_x] == EMPTY ){
				if(board[pacman_y+1][pacman_x] == FOOD){
					score++; 
					food--; 
					curr++; 
					if(food == 0) { 
					res = 2;  
					}
				}
				board[pacman_y][pacman_x] == EMPTY;
				pacman_y++;
				board[pacman_y][pacman_x] == PACMAN;
			}
		}
		else if(move==2){
			if(board[pacman_y][pacman_x-1] == FOOD || board[pacman_y][pacman_x-1] == EMPTY ){
				if(board[pacman_y][pacman_x-1] == FOOD){
					score++; 
					food--; 
					curr++; 
					if(food == 0) { 
					res = 2;  
					}
				}
				board[pacman_y][pacman_x] == EMPTY;
				pacman_x--;
				board[pacman_y][pacman_x] == PACMAN;
			}
		}
		else if(move==3){
			if(board[pacman_y-1][pacman_x] == FOOD || board[pacman_y-1][pacman_x] == EMPTY ){
				if(board[pacman_y-1][pacman_x] == FOOD){
					score++; 
					food--; 
					curr++; 
					if(food == 0) { 
					res = 2; 
					//return; 
					}
				}
				board[pacman_y][pacman_x] == EMPTY;
				pacman_y--;
				board[pacman_y][pacman_x] == PACMAN;
			}
		}
		else if(move==4){
			if(board[pacman_y][pacman_x+1] == FOOD || board[pacman_y][pacman_x+1] == EMPTY ){
				if(board[pacman_y][pacman_x+1] == FOOD){
					score++; 
					food--; 
					curr++; 
					if(food == 0) { 
					res = 2; 
					//return; 
					}
				}
				board[pacman_y][pacman_x] == EMPTY;
				pacman_x++;
				board[pacman_y][pacman_x] == PACMAN;
			}
		}

	}
}


// Main Function 
int main() 
{ 
    printf("DO you want to ?\n1.start a new game.\n2.Continue the last game.\n ");
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
        printf("Power moves: %d\n" , powermove);
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
            printf("\nDo you want to save the game? (y / n)\n");
            if(getch() == 'y'){
                savegame("testing.bin");
                return 0;
            }else{
			    printf("Game Over! Your Score: %d\n", score); 
			    return 0; }
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
			printf("\nDo you want to save the game? (y / n)\n");
            if(getch() == 'y'){
                savegame("testing.bin");
                return 0;
            }else{
			    printf("Game Over! Your Score: %d\n", score); 
			    return 0; }
		case 'p':
			play();
			break;	
		} 
	    } 
    }
	return 0; 
}