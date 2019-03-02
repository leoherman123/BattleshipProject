#include<iostream>
#include<string>

using namespace std;

//creating constant global variable
const int SHIP_TYPES = 3;
const char isWATER = '0'; 
const char isHIT = 'X';
const char isSHIP = 'S';
const char isMISS = 'M';

/*
Creating a Data Structure(struct) which is a group of data 
elements grouped together under one name.
These data elements, known as members, 
can have different types and different lengths
*/
//in the case a Structure POINT contain the variable X and Y for coordinates input 
struct POINT {
	//A location on the grid defined
	//by X(horizontal) Y(vertical) coordinates
	int X;
	int Y;
};

/*
Creating a Data Sturcture for the board
consisting the difficulty and 2D array for grid
*/
struct board{
	int easy;  //variable for easy difficulty
	int medium;  //variable for medium difficulty
	int hard;  //variable for hard difficulty
	char grid[30][30]; //creating a 2D array called grid to for user 
}player[3]; //creating a array to loop between player1 and player2


/*
Creating a Data Structure for the Ships 
and creating array for the ship length and 
array to keep track of damage
*/
struct SHIP {
	//Ship name
	string name;
	//Total points on the grid
	int length;
	
	//Coordinates of those points
	POINT onGrid[6]; //0-4 max length of biggest ship
	
	//Whether or not those points are a "hit"
	bool hitFlag[6];
}ship[SHIP_TYPES]; // creates a array for the type of ships in this case 3


/*Creates a enum(enumeration type) which is a 
user-defined type that consists of a set of named 
integral constants that are known as enumerators*/
//all enum has the same variable type (user declared)
enum DIRECTION {HORIZONTAL,VERTICAL};

/*
Creating a Data Structure for the Placement of ships 
called PLACESHIPS, consisting a reference variable to the 
struct Direction and a reference variable to the struct SHIP
*/
//used to store info about the placment of ships 
struct PLACESHIPS {
	DIRECTION direction;
	SHIP shipType;
};

//prototype for fucntions
void DrawBoard(int,int,string);
void LoadShips();
void DrawBoard(int, int, string);
void ResetBoard(int);
PLACESHIPS UserInputShipPlacement(int c);
bool UserInputAttack(int&, int&, int, int );
bool GameOverCheck(int enemyPLAYER, int b);
bool gameRunning = false;

//main method 
int main()
{
	//creating a reference variable type(board) to change variable in structure board
	struct board b1;
	
	//intializing value to variable in a struct 
	b1.easy = 5;
	b1.medium = 6;
	b1.hard = 7;
	
	//creating variable for the user name and input of difficulty
	string player1;
	string enemyPlayer1;
	string userDiff;

	
	//prompt user for the difficulty
	cout << "Welcome to BattleShip! Please choose your level of difficulty (e = easy, m = medium, h = hard)" << endl;
    cin >> userDiff;
    
//------------------------------------------------------------if the selected difficutly is easy------------------------------------------------------------------------    
    //if loop if the select difficulty is easy 
    if(userDiff == "e" || userDiff == "E")
    {
    	//fucntion call for loadShip (for intital ship placement)
    	LoadShips();
    	//clearing and resetting board 
		ResetBoard(b1.easy);
		
		//prompting user for name
    	cout << "\nYou have chosen the easy difficulty" << endl;
    	cout << "Please enter the name of the 1st player" << endl;
    	cin >> player1;
    	cout << endl;
    	cout << "Please enter the name of the enemy" << endl;
    	cin >> enemyPlayer1;//"PLACE SHIPS" phase of game
	//Loop through each player... 
		for (int aplyr=1; aplyr<3; ++aplyr)
	{
		//Loop through each ship type to place
		for (int thisShip=0; thisShip<SHIP_TYPES; ++thisShip)
		{
			//Display gameboard for player
			//clear screen after selecting difficulty
			system("cls");
			//creates a player board for user and enemy(player2)
			if(aplyr == 1)
			{
				DrawBoard(aplyr,b1.easy,player1);
			}
			else if(aplyr == 2)
			{	
				DrawBoard(aplyr,b1.easy,enemyPlayer1);
			}
			//Give instructions
			cout << "\n";
			cout << "INSTRUCTIONS (Player " << aplyr << ")\n\n";
			cout << "You are about to place your ships.  Format should be:\n";
			cout << "Facing (0:Horizontal,1:Vertical), X (top-row) coords, Y (left-side) coords\n";
			cout << "Example: 0 7 2    This would place a ship beginning at X:7 Y:2 going horizontal\n";
			cout << "If you were to enter the wrong coordinate of the board it would reprompt for a second chance\n\n";
			cout << "Ship to place: " << ship[thisShip].name << " which has a length of " << ship[thisShip].length  << "\n";
			cout << "Where do you want it placed? ";
			
			//Get input from user and loop until good data is returned
			//creating a reference variable ships for function call UserInputShipPlace for ship coordinates
			PLACESHIPS ships;
			ships.shipType.onGrid[0].X = -1;
			while (ships.shipType.onGrid[0].X == -1)
			{
				ships = UserInputShipPlacement(b1.easy);
			}

			//Combine user data with "this ship" data
			ships.shipType.length = ship[thisShip].length;
			ships.shipType.name = ship[thisShip].name;

			//Add the FIRST grid point to the current player's game board
			player[aplyr].grid[ships.shipType.onGrid[0].X][ships.shipType.onGrid[0].Y] = isSHIP;

			//Determine ALL grid points based on length and direction
			for (int i=1; i<ships.shipType.length; ++i)
			{
				//if user selected direction is horizontal
				if (ships.direction == HORIZONTAL){
					ships.shipType.onGrid[i].X = ships.shipType.onGrid[i-1].X+1;
					ships.shipType.onGrid[i].Y = ships.shipType.onGrid[i-1].Y; }
				//if user selected direction is vertical	
				if (ships.direction == VERTICAL){
					ships.shipType.onGrid[i].Y = ships.shipType.onGrid[i-1].Y+1;
					ships.shipType.onGrid[i].X = ships.shipType.onGrid[i-1].X; }

				//Add the REMAINING grid points to our current players game board
				player[aplyr].grid[ships.shipType.onGrid[i].X][ships.shipType.onGrid[i].Y] = isSHIP;
			}
			//Loop back through each ship type
		}
		//Loop back through each player
	}

	//********* FINISHED WITH "PLACE SHIPS" PHASE *********************************
	//*****************************************************************************

	//Ready to play the game
	gameRunning = true;
	int thisPlayer = 1;
	do {
		//Because we are ATTACKING now, the 
		//opposite players board is the display board
			int enemyPlayer;
		if (thisPlayer == 1) enemyPlayer = 2;
		if (thisPlayer == 2) enemyPlayer = 1;
		system("cls");
		DrawBoard(enemyPlayer,b1.easy,enemyPlayer1);
		

		//Get attack coords from this player
		bool goodInput = false;
		int x,y;
		while (goodInput == false) {
			goodInput = UserInputAttack(x,y,thisPlayer, b1.easy);
		}
		
		//Check board; if a ship is there, set as HIT.. otherwise MISS
		if (player[enemyPlayer].grid[x-1][y-1] == isSHIP) player[enemyPlayer].grid[x-1][y-1] = isHIT;
		if (player[enemyPlayer].grid[x-1][y-1] == isWATER) player[enemyPlayer].grid[x-1][y-1] = isMISS;

		//Check to see if the game is over
		//If 0 is returned, nobody has won yet
		int aWin = GameOverCheck(enemyPlayer, b1.easy);
		if (aWin != 0) {
			gameRunning = false;
			break;
		}
		//Alternate between each player as we loop back around
		thisPlayer = (thisPlayer == 1) ? 2 : 1;
	} while (gameRunning);

	//clear screen after game
	system("cls");
	cout << "\n\nCONGRATULATIONS!!!  PLAYER " << thisPlayer << " HAS WON THE GAME!\n\n\n\n";
	
	//pauses system until user input
	system("pause");
	}//end of if statement
	
//--------------------------------------------------------if the selected difficulty is medium-------------------------------------------------------------------------	
	//if loop if the difficulty selected is medium 
	else if(userDiff == "m" || userDiff == "M")
    {
    	//function call to load the ship
    	LoadShips();
    	//function call to reset the board 
    	ResetBoard(b1.medium);
    	
    	//prompting the user for input of name 
    	cout << "\nYou have chosen the medium difficulty" << endl;
    	cout << "Please enter the name of the 1st player" << endl;
    	cin >> player1;
    	cout << endl;
    	cout << "Please enter the name of the enemy" << endl;
    	cin >> enemyPlayer1;//"PLACE SHIPS" phase of game
	//Loop through each player... 
		for (int aplyr=1; aplyr<3; ++aplyr)
	{
		//Loop through each ship type to place
		for (int thisShip=0; thisShip<SHIP_TYPES; ++thisShip)
		{
			//Display gameboard for player
			system("cls");
			//creating board for user and enemy(player2)
			if(aplyr == 1)
			{
				DrawBoard(aplyr,b1.medium,player1);
			}
			else if(aplyr == 2)
			{	
				DrawBoard(aplyr,b1.medium,enemyPlayer1);
			}
			//Give instructions
			cout << "\n";
			cout << "INSTRUCTIONS (Player " << aplyr << ")\n\n";
			cout << "You are about to place your ships.  Format should be:\n";
			cout << "Facing (0:Horizontal,1:Vertical), X (top-row) coords, Y (left-side) coords\n";
			cout << "Example: 0 7 2    This would place a ship beginning at X:7 Y:2 going horizontal\n";
			cout << "If you were to enter the wrong coordinate of the board it would reprompt for a second chance\n\n";
			cout << "Ship to place: " << ship[thisShip].name << " which has a length of " << ship[thisShip].length  << "\n";
			cout << "Where do you want it placed? ";
			
			//Get input from user and loop until good data is returned
			//creating a reference variable aShip to the function UserInputShipPlacement
			PLACESHIPS aShip;
			aShip.shipType.onGrid[0].X = -1;
			while (aShip.shipType.onGrid[0].X == -1)
			{
				//using aShip reference for function call
				aShip = UserInputShipPlacement(b1.medium);
			}

			//Combine user data with "this ship" data
			aShip.shipType.length = ship[thisShip].length;
			aShip.shipType.name = ship[thisShip].name;

			//Add the FIRST grid point to the current player's game board
			player[aplyr].grid[aShip.shipType.onGrid[0].X][aShip.shipType.onGrid[0].Y] = isSHIP;

			//Determine ALL grid points based on length and direction
			for (int i=1; i<aShip.shipType.length; ++i)
			{
				//if the user selected the direction as horizontal 
				if (aShip.direction == HORIZONTAL){
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X+1;
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y; }
				//if the user selected the direction as vertical	
				if (aShip.direction == VERTICAL){
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y+1;
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X; }

				//Add the REMAINING grid points to our current players game board
				player[aplyr].grid[aShip.shipType.onGrid[i].X][aShip.shipType.onGrid[i].Y] = isSHIP;
			}
			//Loop back through each ship type
		}
		//Loop back through each player
	}

	//********* FINISHED WITH "PLACE SHIPS" PHASE *********************************
	//*****************************************************************************

	//Ready to play the game
	gameRunning = true;
	int thisPlayer = 1;
	do {
		//Because we are ATTACKING now, the 
		//opposite players board is the display board
		int enemyPlayer;
		if (thisPlayer == 1) enemyPlayer = 2;
		if (thisPlayer == 2) enemyPlayer = 1;
		system("cls");
		if(thisPlayer == 2)
		DrawBoard(thisPlayer,b1.medium,player1);
		if(thisPlayer == 1 )
		DrawBoard(enemyPlayer,b1.medium,enemyPlayer1);

		//Get attack coords from this player
		bool goodInput = false;
		int x,y;
		while (goodInput == false) {
			goodInput = UserInputAttack(x,y,thisPlayer,b1.medium);
		}

		//Check board; if a ship is there, set as HIT.. otherwise MISS
		if (player[enemyPlayer].grid[x-1][y-1] == isSHIP) player[enemyPlayer].grid[x-1][y-1] = isHIT;
		if (player[enemyPlayer].grid[x-1][y-1] == isWATER) player[enemyPlayer].grid[x-1][y-1] = isMISS;

		//Check to see if the game is over
		//If 0 is returned, nobody has won yet
		int aWin = GameOverCheck(enemyPlayer,b1.medium);
		if (aWin != 0) {
			gameRunning = false;
			break;
		}
		//Alternate between each player as we loop back around
		thisPlayer = (thisPlayer == 1) ? 2 : 1;
	} while (gameRunning);

	//clear screen 
	system("cls");	
	cout << "\n\nCONGRATULATIONS!!!  PLAYER " << thisPlayer << " HAS WON THE GAME!\n\n\n\n";
	
	//pauses system until user input
	system("pause");
	}//end of if statement

//------------------------------------------------------------if the selected difficulty is hard----------------------------------------------------------------------
	//if loop if the difficulty selected is hard 
	else if(userDiff == "h" || userDiff == "H")
    {
    	LoadShips();
    	//using the function to reset the board
    	ResetBoard(b1.hard);
    	//uses the function loadship 
    	
    	//input and output the users for selected difficulty
    	cout << "\nYou have chosen the Hard difficulty" << endl;
    	cout << "Please enter the name of the 1st player" << endl;
    	cin >> player1;
    	cout << endl;
    	cout << "Please enter the name of the enemy" << endl;
    	cin >> enemyPlayer1;//"PLACE SHIPS" phase of game
    	
	//Loop through each player... 
		for (int aplyr=1; aplyr<3; ++aplyr)
	{
		//Loop through each ship type to place
		for (int thisShip=0; thisShip<SHIP_TYPES; ++thisShip)
		{
			//Display gameboard for player
			//clearing screen after prompt for difficulty
			system("cls");
			
			//creating game board for user and enemy(player 2)
			if(aplyr == 1)
			{
				DrawBoard(aplyr,b1.hard,player1);
			}
			else if(aplyr == 2)
			{	
				DrawBoard(aplyr,b1.hard,enemyPlayer1);
			}
			//Give instructions
			cout << "\n";
			cout << "INSTRUCTIONS (Player " << aplyr << ")\n\n";
			cout << "You are about to place your ships.  Format should be:\n";
			cout << "Facing (0:Horizontal,1:Vertical), X (top-row) coords, Y (left-side) coords\n";
			cout << "Example: 0 7 2    This would place a ship beginning at X:7 Y:2 going horizontal\n";
			cout << "If you were to enter the wrong coordinate of the board it would reprompt for a second chance\n\n";
			cout << "Ship to place: " << ship[thisShip].name << " which has a length of " << ship[thisShip].length  << "\n";
			cout << "Where do you want it placed? ";
			
			//Get input from user and loop until good data is returned
			//setting a reference variable for the function UserInputShipPlacement
			//returning a ship coordinate for placement on grid
			PLACESHIPS aShips;
			aShips.shipType.onGrid[0].X = -1;
			while (aShips.shipType.onGrid[0].X == -1)
			{
				//setting the aShips reference variable to the function 
				aShips = UserInputShipPlacement(b1.hard);
			}

			//Combine user data with "this ship" data
			aShips.shipType.length = ship[thisShip].length;
			aShips.shipType.name = ship[thisShip].name;

			//Add the FIRST grid point to the current player's game board
			player[aplyr].grid[aShips.shipType.onGrid[0].X][aShips.shipType.onGrid[0].Y] = isSHIP;

			//Determine ALL grid points based on length and direction
			for (int i=1; i<aShips.shipType.length; ++i)
			{
				//if the direction facing selected is horizontal
				if (aShips.direction == HORIZONTAL){
					aShips.shipType.onGrid[i].X = aShips.shipType.onGrid[i-1].X+1;
					aShips.shipType.onGrid[i].Y = aShips.shipType.onGrid[i-1].Y; }
				//if the direction facing selected is vertical 	
				if (aShips.direction == VERTICAL){
					aShips.shipType.onGrid[i].Y = aShips.shipType.onGrid[i-1].Y+1;
					aShips.shipType.onGrid[i].X = aShips.shipType.onGrid[i-1].X; }

				//Add the REMAINING grid points to our current players game board
				player[aplyr].grid[aShips.shipType.onGrid[i].X][aShips.shipType.onGrid[i].Y] = isSHIP;
			}
			//Loop back through each ship type
		}
		//Loop back through each player
	}

	//********* FINISHED WITH "PLACE SHIPS" PHASE *********************************
	//*****************************************************************************

	//Ready to play the game
	gameRunning = true;
	int thisPlayer = 1;
	do {
		//Because we are ATTACKING now, the 
		//opposite players board is the display board
		int enemyPlayer;
		if (thisPlayer == 1) enemyPlayer = 2;
		if (thisPlayer == 2) enemyPlayer = 1;
		system("cls");
		if(thisPlayer == 1)
		DrawBoard(thisPlayer,b1.hard,player1);
		if(thisPlayer == 2 )
		DrawBoard(enemyPlayer,b1.hard,enemyPlayer1);

		//Get attack coords from this player
		bool goodInput = false;
		int x,y;
		while (goodInput == false) {
			goodInput = UserInputAttack(x,y,thisPlayer,b1.hard);
		}

		//Check board; if a ship is there, set as HIT.. otherwise MISS
		if (player[enemyPlayer].grid[x-1][y-1] == isSHIP) player[enemyPlayer].grid[x-1][y-1] = isHIT;
		if (player[enemyPlayer].grid[x-1][y-1] == isWATER) player[enemyPlayer].grid[x-1][y-1] = isMISS;

		//Check to see if the game is over
		//If 0 is returned, nobody has won yet
		int aWin = GameOverCheck(enemyPlayer,b1.hard);
		if (aWin != 0) {
			gameRunning = false;
			break;
		}
		//Alternate between each player as we loop back around
		thisPlayer = (thisPlayer == 1) ? 2 : 1;
	} while (gameRunning);

	//clears the screen(cls) and prompt
	system("cls");
	cout << "\n\nCONGRATULATIONS!!!  PLAYER " << thisPlayer << " HAS WON THE GAME!\n\n\n\n";

	//Pauses the system program until user input 
	system("pause");
}
	
	return 0;	
	}

//Function to reset the board at the beginning after game is finished
void ResetBoard(int b)
{
	//Loop through each player
	for (int plyr=1; plyr<3; ++plyr)
	{
		//For each grid point, set contents to 'water'
		for (int w=0; w<b; ++w){
			for (int h=0; h<b; ++h){
				player[plyr].grid[w][h] = isWATER;
		}}
		//Loop back to next player
	}
}

// function to draw the board
void DrawBoard(int thisPlayer, int b, string n)
{
	//Draws the board for a player (thisPlayer)
	cout << "PLAYER " << n << "'s GAME BOARD\n";
	cout << "----------------------\n";

	//Loop through top row (board_width) and number columns
	cout << "   ";
	for (int w=0; w < b; ++w) {
		if (w < b)
			//Numbers only 1 character long, add two spaces after
			cout << w + 1 << "  ";
	}
	cout << "\n";

	//Loop through each grid point and display to console
	for (int h=0; h< b; ++h){
		for (int w=0; w<b; ++w){
			
			//If this is the FIRST (left) grid point, number the grid first
			if (w==0) cout << h + 1 << " ";
			//If h was 1 character long, add an extra space to keep numbers lined up
			if (w<10 && w==0) cout << " ";
			//Display contents of this grid (if game isn't running yet, we are placing ships
			//so display the ships
			if (gameRunning == false) cout << player[thisPlayer].grid[w][h] << "  ";
			//Don't show ships, BUT show damage if it's hit
			if (gameRunning == true && player[thisPlayer].grid[w][h] != isSHIP) 
			{cout << player[thisPlayer].grid[w][h] << "  ";}
			else if (gameRunning == true && player[thisPlayer].grid[w][h] == isSHIP)
			{cout << isWATER << "  ";}	
			//If we have reached the border.. line feed
			if (w == b-1) cout << "\n";
		}
	}
}


void LoadShips()
{
	//Sets the default data for the ships
	//we plan to include in the game
	//IMPORTANT!! > MUST MATCH SHIP_TYPES -Default=3 (0-3)
	ship[0].name = "Destroyer"; ship[0].length = 2;
	ship[1].name = "Submarine"; ship[1].length = 3;
	ship[2].name = "BattleShip"; ship[2].length = 4;
	
}

// Userinput the attack
bool UserInputAttack(int& x, int& y, int theplayer, int b)
{
	cout << "\nPLAYER " << theplayer << ", ENTER COORDINATES TO ATTACK: ";
	bool goodInput = false;
	cin >> x >> y;
	if (x<0 || x>b) return goodInput;
	if (y<0 || y>b) return goodInput;
	goodInput = true; 
	return goodInput;
}

//Calling the struct PLACESHIPS to enter the user coordinate in (direction, x, y)
PLACESHIPS UserInputShipPlacement(int c)
{
	int d, x, y;
	//create a reference to the struct class PLACESHIPS
	PLACESHIPS tmp;
	//Using this as a bad return
	tmp.shipType.onGrid[0].X = -1;
	//Get 3 integers from user
	cin >> d >> x >> y;
	if (d!=0 && d!=1) return tmp;
	if (x<0 || x> c) return tmp;
	if (y<0 || y> c) return tmp;
	//Good data
	tmp.direction = (DIRECTION)d;
	tmp.shipType.onGrid[0].X = x-1;
	tmp.shipType.onGrid[0].Y = y-1;
	return tmp;
}

// function for testing if the game is over 
bool GameOverCheck(int enemyPLAYER, int b)
{
	bool winner = true;
	//Loop through enemy board
	for (int w=0; w<b; ++w){
			for (int h=0; h<b; ++h){
				//If any ships remain, game is NOT over
				if (player[enemyPLAYER].grid[w][h] = isSHIP)
					{
						winner = false;
						return winner;
					}
		}}
	//If we get here, somebody won, game over!
	return winner;
}
