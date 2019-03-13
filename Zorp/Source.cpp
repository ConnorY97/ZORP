#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>


const char* ESC = "\x1b";
const char* CSI = "\x1b[";
const char* TITLE = "\x1b[5;20H";
const char* INDENT = "\x1b[5C";
const char* YELLOW = "\x1b[93m";
const char* MAGENTA = "\x1b[95m";
const char* RED = "\x1b[91m";
const char* BLUE = "\x1b[94m";
const char* WHITE = "\x1b[97m";
const char* GREEN = "\x1b[92m";
const char* RESET_COLOR = "\x1b[0m";
const char* EXTRA_OUTPUT_POS = "\x1b[25;6h";

using namespace std;

const int EMPTY = 0;
const int ENEMY = 1;
const int TREASURE = 2;
const int FOOD = 3;
const int ENTERANCE = 4;
const int EXIT = 5;
const int MAX_RANDOM_TYPE = FOOD + 1;

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 6;

const int INDENT_X = 5; //How many spaces to use to indent all text. This cal also be used as a tab
const int ROOM_DESC_Y = 8; //The line to use for our room descriptions
const int MOVEMENT_DESC_Y = 9; 
const int MAP_Y = 13; //The first line where the map is drawn
const int PLAYER_INPUT_X = 30; //The character column where the player will type their input
const int PLAYER_INPUT_Y = 23; //The line where the player will type their input

//Input commands
const int WEST = 4; //Used for directional input from player 
const int EAST = 6;
const int NORTH = 8;
const int SOUTH = 2;

const int LOOK = 9;
const int FIGHT = 10; 

	bool enableVirtualTerminal()
	{
		//Set out mode to handle virtual terminal sequences
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode))
		{
			return false; 
		}

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwMode))
		{
			return false;
		}
		return true; 
	}

	void initialize(int map[MAZE_HEIGHT][MAZE_WIDTH])
	{
		srand(time(nullptr));

		//Fill the arrays with random room types
		for (int y = 0; y < MAZE_HEIGHT; y++)
		{
			for (int x = 0; x < MAZE_WIDTH; x++)
			{
				int type = rand() % (MAX_RANDOM_TYPE * 2);
				if (type < MAX_RANDOM_TYPE)
					map[y][x] = type;
				else
					map[y][x] = EMPTY;
			}
		}

		//Set the enterance and exit of the maze
		map[0][0] = ENTERANCE;
		map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT;
	}

	void drawWelcomeMessage()
	{
		cout << TITLE << MAGENTA << "Welcome to Zorp!" << RESET_COLOR << endl;
		cout << INDENT << "Zorp is a game of adventure, danger, and low cunning." << endl;
		cout << INDENT << "It is definitely not related to any other text-based adventure game." << endl;
	}

	void drawRoom(int map[MAZE_HEIGHT][MAZE_WIDTH], int x, int y)
	{
		//Find the console output position
		int outX = INDENT_X + (6 * x) + 1;
		int outY = MAP_Y + y; 

		//Jump to the correct location
		cout << CSI << outY << ";" << outX << "H";
		//Draw the rooms
		switch (map[y][x])
		{
		case EMPTY:
			cout << "[ " << GREEN << "\xb0" << RESET_COLOR << " ] ";
			break;
		case ENEMY:
			cout << "[ " << RED << "\x94" << RESET_COLOR << " ] ";
			break;
		case TREASURE:
			cout << "[ " << YELLOW << "$" << RESET_COLOR << " ] ";
			break;
		case FOOD:
			cout << "[ " << WHITE << "\x3D" << RESET_COLOR << " ] ";
			break;
		case ENTERANCE:
			cout << "[ " << WHITE << "\x9d" << RESET_COLOR << " ] ";
			break;
		case EXIT:
			cout << "[ " << WHITE << "\xFE" << RESET_COLOR << " ] ";
			break;
		}
	}

	void drawMap(int map[MAZE_HEIGHT][MAZE_WIDTH])
	{
		//Reset draw colors
		cout << RESET_COLOR;
		for (int y = 0; y < MAZE_HEIGHT; y++)
		{
			cout << INDENT;
			for (int x = 0; x < MAZE_WIDTH; x++)
			{
				drawRoom(map, x, y);
			}
			cout << endl; 
		}
	}

	void drawRoomDescription(int roomType) 
	{
		//Reset draw colors
		cout << RESET_COLOR;
		//Jump to the correct location
		cout << CSI << ROOM_DESC_Y << ";" << 0 << "H";
		//Delete 4 lines and intert 4 empty lines
		cout << CSI << "4M" << CSI << "4L" << endl; 

		//Write description of current room
		switch (roomType)
		{
		case EMPTY:
			cout << INDENT << "You are in an empty meadow. There is nothing of note here." << endl;
			break;
		case ENEMY:
			cout << INDENT << "BEWARE. An enemy is approaching." << endl;
			break;
		case TREASURE:
			cout << INDENT << "Your journey has been rewarded. You have found some treasure" << endl;
			break;
		case FOOD:
			cout << INDENT << "At last! You collect some food to sustain you on your journey." << endl;
			break;
		case ENTERANCE:
			cout << INDENT << "The entrance you used to enter this maze is blocked. There is no going back." << endl;
			break;
		case EXIT:
			cout << INDENT << "Despite all odds, you made it to the exit. Congratulations." << endl;
			break;
		}
	}

	void drawPlayer(int x, int y)
	{
		x = INDENT_X + (6 * x) + 3;
		y = MAP_Y + y; 

		//Draw the player's position on the map 
		//Move the cursor to map pos and delete character at current position
		cout << CSI << y << ";" << x << "H";
		cout << MAGENTA << "\x81" << RESET_COLOR;
	}

	void drawValidDirctions(int x, int y)
	{
		//Reset draw colours
		cout << RESET_COLOR;
		//Jump to the correct location
		cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
		cout << INDENT << "You can see paths leading to the " <<
			((x > 0) ? "west, " : "") <<
			((x < MAZE_WIDTH - 1) ? "east, " : "") <<
			((y > 0) ? "north, " : "") <<
			((y < MAZE_HEIGHT - 1) ? "south, " : "") << endl;
	}

	/*int getCommand()
	{
		//Jump to correct location
		cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
		cout << INDENT << "Where to now?";

		int direction;
		//Move the cursor to position for the player to input 
		cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

		//Ckear the input buffer, ready for player input
		cin.clear();
		cin.ignore(std::cin.rdbuf()->in_avail());

		cin >> direction;
		cout << RESET_COLOR;

		if (cin.fail())
			return 0;
		return direction; 
	}*/

	int getCommand()
	{
		//For now, we can;t read commands longer than 50 characters
	}

void main()
{
	int rooms[MAZE_HEIGHT][MAZE_WIDTH];
	
	bool gameOver = false;
	int playerX = 0;
	int playerY = 0;

	if (enableVirtualTerminal() == false)
	{
		cout << "The virtual termincal processing mode could not be activated" << endl << "Press 'Enter' to exit" << endl;
		cin.get();
		return; 
	}

	initialize(rooms);

	drawWelcomeMessage();

	//output the map
	drawMap(rooms);

	//Game loop
	while (!gameOver)
	{
		drawRoomDescription(rooms[playerY][playerX]);

		drawPlayer(playerX, playerY);

		if (rooms[playerY][playerX] == EXIT)
		{
			gameOver = true;
			continue;
		}

		//List the directions the player can take
		drawValidDirctions(playerX, playerY);
		int command = getCommand(); 

		//Before updating the player position, redraw the old room
		//character over the old position
		drawRoom(rooms, playerX, playerY);

		//Update the player's position using the input movement data
		switch (command)
		{
		case EAST:
			if (playerX < MAZE_WIDTH - 1)
				playerX++;
			break;
		case WEST:
			if (playerX > 0)
				playerX--;
			break;
		case NORTH:
			if (playerY > 0)
				playerY--;
			break;
		case SOUTH:
			if (playerY < MAZE_HEIGHT - 1)
				playerY++;
			break;
		case LOOK:
			drawPlayer(playerX, playerY);
			cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You look around, but see nothing worth mentioning" << endl; 
			cout << INDENT << "Press 'Enter' to continue."; 
			cin.clear();
			cin.ignore(std::cin.rdbuf()->in_avail());
			cin.get();
			break;
		case FIGHT:
			drawPlayer(playerX, playerY);
			cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You could try to fight but you don't have a weapon!" << endl;
			cout << INDENT << "Press 'Enter' to continue.";
			cin.clear();
			cin.ignore(std::cin.rdbuf()->in_avail());
			cin.get();
		default:
			//The direction was not valid, 
			//do nothing, go back to the top of the loop and ask again 
			drawPlayer(playerX, playerY);
			cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You try, but you just cant do it!" << endl; 
			cout << INDENT << "Press 'Enter' to continue.";
			cin.clear();
			cin.ignore(std::cin.rdbuf()->in_avail());
			cin.get();
			break; 
		}
		//End of game loop
	}

	//Jump to the correct location
	cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	cout << std::endl << INDENT << "Press 'Enter' to exit the program.";
	cin.clear();
	cin.ignore(std::cin.rdbuf()->in_avail());
	cin.get();  
}