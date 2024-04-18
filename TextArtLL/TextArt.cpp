/*
* This program allows display and editing of text art (also called ASCII art).
*/

#include <iostream>
#include <fstream>
#include <cctype>
#include <windows.h>
#include <conio.h>
#include "Definitions.h"
using namespace std;


int main()
{
	char choice, oldC, newC, choice2;
	int cols, rows;
	char oldCH[2], newCH[2];
	char menu[200] = "<E>dit / <M>ove / <R>eplace / <D>raw / <C>lear / <L>oad / <S>ave / <Q>uit: ";
	int menuSize = sizeof(menu) + 5;
	bool animate = false;
	char Animate = 'N';
	Node* current;
	char path[FILENAMESIZE] = "SavedFiles\\";
	const char txt[FILENAMESIZE] = ".txt";
	char fileName[FILENAMESIZE];
	char FileName[FILENAMESIZE];

	List undoL;
	List redoL;
	List clips;
	int undoCount;
	int redoCount;
	int clipsCount;


current = newCanvas();


// TODO: write code for the main program
initCanvas(current->item);
do
{
	undoCount = undoL.count;
	redoCount = redoL.count;
	clipsCount = clips.count;

	displayCanvas(current->item);
	clearLine(MAXROWS + 1, menuSize);
	clearLine(MAXROWS + 2, menuSize);
	clearLine(MAXROWS + 4, menuSize);
	clearLine(MAXROWS + 5, menuSize);
	clearLine(MAXROWS + 6, menuSize);

	gotoxy(MAXROWS + 1, 0);
	if (clips.count > 1 && redoL.count > 0)
	{
		cout << "<A>nimate: ";
		cout << Animate;
		cout << " / <U>ndo: ";
		cout << undoCount;
		cout << " / Red<O>: ";
		cout << redoCount;
		cout << " / Cl<I>p: ";
		cout << clipsCount;
		cout << " / <P>lay";
	}
	else if (clips.count > 1 && redoL.count == 0)
	{
		cout << "<A>nimate: ";
		cout << Animate;
		cout << " / <U>ndo: ";
		cout << undoCount;
		cout << " / Cl<I>p: ";
		cout << clipsCount;
		cout << " / <P>lay";
	}
	else if (clips.count < 2 && redoL.count == 0)
	{
		cout << "<A>nimate: ";
		cout << Animate;
		cout << " / <U>ndo: ";
		cout << undoCount;
		cout << " / Cl<I>p: ";
		cout << clipsCount;
	}
	else if (clips.count < 2 && redoL.count > 0)
	{
		cout << "<A>nimate: ";
		cout << Animate;
		cout << " / <U>ndo: ";
		cout << undoCount;
		cout << " / Red<O>: ";
		cout << redoCount;
		cout << " / Cl<I>p: ";
		cout << clipsCount;
	}


	if (animate == false)
	{
		Animate = 'N';
	}
	else
	{
		Animate = 'Y';
	}

	gotoxy(MAXROWS + 2, 0);
	for (int i = 0; i < menuSize - 5; i++)
	{
		cout << menu[i];
	}

	cin >> choice;

	// Displays canvas and asks user to choose an option
	while (cin.fail())
	{
		displayCanvas(current->item);
		cin.clear();
		cin.ignore();
		gotoxy(MAXROWS + 1, 0);
		for (int i = 0; i < menuSize - 5; i++)
		{
			cout << menu[i];
		}
		cin >> choice;
	}


	cin.ignore();

	switch (choice)
	{
		//Option for user to edit canvas
		case 'O':
		case 'o':
			if (redoL.count > 0)
			{
				addNode(undoL, newCanvas(current));
				current = removeNode(redoL);
			}
			break;
		case 'P':
		case 'p':
			if (clips.count >= 2)
			{
				clearLine(MAXROWS + 1, menuSize);
				clearLine(MAXROWS + 2, menuSize);
				gotoxy(MAXROWS + 1, 0);
				cout << "Hold <ESC> to stop       Clip: ";
				play(clips);
			}
			break;
		case 'i':
		case 'I':
			addNode(clips, newCanvas(current));
			break;
		case 'a':
		case 'A':
			if (animate == false)
			{
				animate = true;
			}
			else
			{
				animate = false;
			}
			break;
		case 'e':
		case 'E':
			addUndoState(undoL, redoL, current);
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			editCanvas(current->item);
			break;
			//Option for user to move canvas
		case 'm':
		case 'M':
			addUndoState(undoL, redoL, current);
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Enter column units to move: ";
			cin >> cols;
			cin.ignore();
			gotoxy(MAXROWS + 2, 0);
			cout << "Enter row units to move: ";
			cin >> rows;
			cin.ignore();
			moveCanvas(current->item, rows, cols);
			break;
			//Option for user to replace canvas
		case 'r':
		case 'R':
			addUndoState(undoL, redoL, current);
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Enter character to replace: ";
			cin.getline(oldCH, 2);
			oldC = oldCH[0];
			gotoxy(MAXROWS + 2, 0);
			cout << "Enter character to replace with: ";
			cin.getline(newCH, 2);
			newC = newCH[0];
			replace(current->item, oldC, newC);
			break;
			//Option for user to undo canvas
		case 'u':
		case 'U':
			if (undoL.count > 0)
			{
				restore(undoL, redoL, current);
			}
			break;
			//Option for user to load canvas
		case 'l':
		case 'L':
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "<C>anvas or <A>nimation  ?";
			cin >> choice2;
			switch (choice2) 
			{
			case 'a':
			case 'A':
				clearLine(MAXROWS + 1, menuSize);
				gotoxy(MAXROWS + 1, 0);
				cout << "Enter the fileName (don't enter 'txt'): ";
				cin >> fileName;
				snprintf(FileName, FILENAMESIZE - 15, "%s%s", path, fileName);
				loadClips(clips, FileName);
				break;
			case 'C':
			case 'c':
				clearLine(MAXROWS + 1, menuSize);
				gotoxy(MAXROWS + 1, 0);
				cout << "Enter the fileName (don't enter 'txt'): ";
				cin >> fileName;
				snprintf(FileName, FILENAMESIZE - 15, "%s%s%s", path, fileName, txt);
				loadCanvas(current->item, FileName);
				break;
			}
			break;
			//Option for user to save canvas
		case 's':
		case 'S':
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "<C>anvas or <A>nimation  ?";
			cin >> choice2;
			switch (choice2) 
			{
			case 'a':
			case 'A':
				clearLine(MAXROWS + 1, menuSize);
				gotoxy(MAXROWS + 1, 0);
				cout << "Enter the fileName (don't enter 'txt'): ";
				cin >> fileName;
				snprintf(FileName, FILENAMESIZE - 15, "%s%s", path, fileName);
				saveClips(clips, FileName);
				break;
			case 'C':
			case 'c':
				clearLine(MAXROWS + 1, menuSize);
				gotoxy(MAXROWS + 1, 0);
				cout << "Enter the fileName (don't enter 'txt'): ";
				cin >> fileName;
				snprintf(FileName, FILENAMESIZE - 15, "%s%s%s", path, fileName, txt);
				saveCanvas(current->item, FileName);
				break;
			}
			
			break;
		case 'c':
		case 'C':
			addUndoState(undoL, redoL, current);
			initCanvas(current->item);
			break;
		case 'd':
		case 'D':
			clearLine(MAXROWS + 2, menuSize);
			menuTwo(current,undoL,redoL,clips,animate);
			break;
		}
	} while (choice != 'q' && choice != 'Q');

	delete current;
	deleteList(undoL);
	deleteList(redoL);
	deleteList(clips);

	return 0;
}


void gotoxy(short row, short col)
{
	COORD pos = { col, row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void clearLine(int lineNum, int numOfChars)
{
	// Move cursor to the beginning of the specified line on the console
	gotoxy(lineNum, 0);

	// Write a specified number of spaces to overwrite characters
	for (int x = 0; x < numOfChars; x++)
		cout << " ";

	// Move cursor back to the beginning of the line
	gotoxy(lineNum, 0);
}


void replace(char canvas[][MAXCOLS], char oldCh, char newCh)
{
	// Checks for old canvas and replaces it with the new canvas
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			if (canvas[i][j] == oldCh)
			{
				canvas[i][j] = newCh;
			}
		}
	}
}


void editCanvas(char canvas[][MAXCOLS])
{

	// TODO: write code for the function
	char input;
	int row = 0, col = 0;

	// Move cursor to row,col and then get
	// a single character from the keyboard
	gotoxy(MAXROWS + 1, 0);
	cout << "Press ESC to stop editing";
	gotoxy(row, col);
	input = _getch();
	while (input != ESC)
	{
		if (input == SPECIAL) 
		{
			switch (input = _getch())
			{
				// allows user to use the left arrow key to move left on canvas
			case LEFTARROW:
				if(col -1 >= 0)
				col -= 1;
				gotoxy(row, col);
				break;
				// allows user to use the up arrow key to move up on canvas
			case UPARROW:
				if(row -1 >= 0)
				row -= 1;
				gotoxy(row, col);
				break;
				// allows user to use the right arrow key to move right on canvas
			case RIGHTARROW:
				if(col + 1 < MAXCOLS)
				col += 1;
				gotoxy(row, col);
				break;
				// allows user to use the down arrow key to move down on canvas
			case DOWNARROW:
				if (row + 1 < MAXROWS)
				row += 1;
				gotoxy(row, col);
				break;
			}
		}
		else if (input > 31 && input < 127)
		{
			canvas[row][col] = input;
			displayCanvas(canvas);
			gotoxy(MAXROWS + 1, 0);
			cout << "Press ESC to stop editing";
			gotoxy(row, col);
		}

		input = _getch();
	}
}


void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue)
{
	// TODO: write code for the function
	int row = rowValue, col = colValue;
	char tmpCanvas[MAXROWS][MAXCOLS];
	copyCanvas(tmpCanvas, canvas);

	//interchange - 
	// Moves the canvas up to the left
	if (rowValue <= 0 && colValue <= 0)
	{
		row *= -1;
		col *= -1;

		for (int i = 0; i < MAXROWS; i++)
		{
			for (int x = 0; x < MAXCOLS; x++)
			{

				if ((i + (rowValue * -1) >= 0 && i + (rowValue * -1) < MAXROWS) && (x + (colValue * -1) >= 0 && x + (colValue * -1) < MAXCOLS))
				{
					canvas[i][x] = tmpCanvas[i + (rowValue * -1)][x + (colValue * -1)];
				}
				else
				{
					tmpCanvas[i][x] = ' ';
					canvas[i][x] = tmpCanvas[i][x];
				}
			}
		}
	}

	// Moves the canvas down to the right
	else if (rowValue >= 0 && colValue >= 0)
	{

		for (int i = MAXROWS - 1; i >= 0; i--)
		{
			for (int x = 0; x < MAXCOLS; x++)
			{

				if ((i - rowValue >= 0 && i - rowValue < MAXROWS) && (x - colValue >= 0 && x - colValue < MAXCOLS))
				{
					canvas[i][x] = tmpCanvas[i - rowValue][x - colValue];
				}
				else
				{
					tmpCanvas[i][x] = ' ';
					canvas[i][x] = tmpCanvas[i][x];
				}
			}
		}
	}
	//
	// Moves the canvas down to the left
	else if (rowValue >= 0 && colValue <= 0)
	{
		col *= -1;
		for (int i = MAXROWS - 1; i >= 0; i--)
		{
			for (int x = 0; x < MAXCOLS; x++)
			{

				if ((i - rowValue >= 0 && i - rowValue < MAXROWS) && (x - colValue >= 0 && x - colValue < MAXCOLS))
				{
					canvas[i][x] = tmpCanvas[i - rowValue][x - colValue];
				}
				else
				{
					tmpCanvas[i][x] = ' ';
					canvas[i][x] = tmpCanvas[i][x];
				}
			}
		}
	}
	//
	// Moves the canvas up to the right
	else if (rowValue <= 0 && colValue >= 0)
	{
		row *= -1;
		for (int i = 0; i < MAXROWS; i++)
		{
			for (int x = MAXCOLS - 1; x >= 0; x--)
			{

				if ((i - rowValue >= 0 && i - rowValue < MAXROWS) && (x - colValue >= 0 && x - colValue < MAXCOLS))
				{
					canvas[i][x] = tmpCanvas[i - rowValue][x - colValue];
				}
				else
				{
					tmpCanvas[i][x] = ' ';
					canvas[i][x] = tmpCanvas[i][x];
				}
			}
		}
	}
	else
		for (int i = 0; i < MAXROWS; i++)
		{
			for (int x = MAXCOLS - 1; x >= 0; x--)
			{
				{
					tmpCanvas[i][x] = ' ';
					canvas[i][x] = tmpCanvas[i][x];
				}
			}
		}
	


}


void initCanvas(char canvas[][MAXCOLS])
{
	//Checks to see if i and j are within the array
	// and fills array with space characters if they are
	for (int i = 0; i < MAXCOLS; i++)
	{
		for (int j = 0; j < MAXROWS; j++)
		{
			canvas[j][i] = ' ';
		}
	}

}


void displayCanvas(char canvas[][MAXCOLS])
{
	// Clear the screen
	for (int row = 0; row < MAXROWS ; row++)
	{
		clearLine(row, MAXCOLS);
	}
	gotoxy(0, 0);
	for (int i = 0; i < MAXROWS + 1; i++)
	{
		for (int j = 0; j < MAXCOLS + 1; j++)
		{
			if (i == MAXROWS)
			{
				cout << "-";
			}
			else if (j == MAXCOLS && i < MAXROWS)
			{
				cout << "|";
			}
			else
			{
				cout << canvas[i][j];
			}
		}
		cout << endl;
	}
}


void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS])
{
	// Copies contents from the entire array
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			to[i][j] = from[i][j];
		}
	}
}


bool saveCanvas(char canvas[][MAXCOLS], char filename[])
{
	{
		//Asks user to enter the name of the file that they want to save
	
		ofstream outFile;
		
		outFile.open(filename);
		//Tells the user that there was an error and the file could not be saved 
		if (!outFile)
		{
			cerr << "ERROR: output file could not be created\n";
			system("pause");
			return false;
		}

		for (int i = 0; i < MAXROWS; i++)
		{
			for (int x = 0; x < MAXCOLS; x++)
			{
				outFile << canvas[i][x];
			}
			outFile << endl;
		}
		//Confirms that the file was successfully saved
		outFile.close();
		cout << "Output file written!\n";
		system("pause");

		return true;
	}
}


bool loadCanvas(char canvas[][MAXCOLS], char filename[])
{
	//Asks user to enter the name of the file that they want to load
	int row = 0, col;
	char tempArray[MAXCOLS + 1];

	ifstream inFile;

	inFile.open(filename);
	// Tells user that there was an error and the file could not be opened 
	if (!inFile)
	{
		//cerr << "ERROR: File cannot be read.\n";
		//system("pause");
		return false;
	} 
	else 
	{
		//Successfully loads the new file 
	for (int i = 0; i < MAXCOLS; i++)
	{
		tempArray[i] = ' ';
	}

	inFile.getline(tempArray, MAXCOLS + 1);

	while (!inFile.eof() || row < MAXROWS)
	{

		for (col = 0; col < MAXCOLS; col++)
		{
			if(tempArray[col] != '\0')
			canvas[row][col] = tempArray[col];
		}
		row++;

		for (int i = 0; i < MAXCOLS; i++)
		{
			tempArray[i] = ' ';
		}

		if (inFile.fail()) 
		{
			inFile.clear();
			inFile.ignore(1000, '\n');
		}
		inFile.getline(tempArray, MAXCOLS + 1);
	}
	inFile.close();
}
	return true;
}