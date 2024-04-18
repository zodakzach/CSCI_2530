/*
* This program allows display and editing of text art (also called ASCII art).
*/

#include <iostream>
#include <fstream>
#include <cctype>
#include <windows.h>
#include <conio.h>
using namespace std;

const int BUFFERSIZE = 20;
const int FILENAMESIZE = 255;
const int MAXROWS = 22;
const int MAXCOLS = 80;

// ASCII codes for special keys; for editing
const char ESC = 27;
const char LEFTARROW = 75;
const char UPARROW = 72;
const char RIGHTARROW = 77;
const char DOWNARROW = 80;
const char SPECIAL = (char)224;

/*
* Gets a filename from the user. If file can be opened for reading,
* this function loads the file's contents into canvas.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed and
* canvas is left unchanged.
*/
void loadCanvas(char canvas[][MAXCOLS]);

/*
* Gets a filename from the user. If file can be opened for writing,
* this function writes the canvas contents into the file.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed.
*/
void saveCanvas(char canvas[][MAXCOLS]);

/*
* Initializes canvas to contain all spaces.
*/
void initCanvas(char canvas[][MAXCOLS]);

/*
* Displays canvas contents on the screen, with a border
* around the right and bottom edges.
*/
void displayCanvas(char canvas[][MAXCOLS]);

/*
* Allows user to edit the canvas by moving the cursor around and
* entering characters. Editing continues until the ESC key is pressed.
*/
void editCanvas(char canvas[][MAXCOLS]);

/*
* Copies contents of the "from" canvas into the "to" canvas.
*/
void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS]);

/*
* Replaces all instances of a character in the canvas.  
* oldCh is the character to be replaced.
* newCh character is the character to replace with.
*/
void replace(char canvas[][MAXCOLS], char oldCh, char newCh);

/*
* Shifts contents of the canvas by a specified number of rows and columns.
* rowValue is the number of rows by which to shift
*    positive numbers shift downward; negative numbers shift upward
* colValue is the number of rows by which to shift
*    positive numbers shift right; negative numbers shift left
*/
void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue);

/*
* Clears a line on the output screen, then resets the cursor back to the
* beginning of this line.
* lineNum is the line number on the output screen to clear
* numOfChars is the number of characters to clear on this line
*/
void clearLine(int lineNum, int numOfChars);

/*
* Moves the cursor in the output window to a specified row and column.
* The next output produced by the program will begin at this position.
*/
void gotoxy(short row, short col);


int main()
{
	char canvas[MAXROWS][MAXCOLS];
	char backUpCanvas[MAXROWS][MAXCOLS];
	char choice, oldC, newC;
	int cols, rows;
	char oldCH[2], newCH[2];

	// TODO: write code for the main program
	initCanvas(canvas);
	initCanvas(backUpCanvas);
	do
	{
	
		displayCanvas(canvas);
		gotoxy(23, 0);
		cout << "<E>dit / <M>ove / <R>eplace / <U>ndo / <L>oad / <S>ave / <Q>uit: ";
		cin >> choice;


		while (cin.fail())
		{
			displayCanvas(canvas);
			cin.clear();
			cin.ignore();
			gotoxy(23, 0);
			cout << "<E>dit / <M>ove / <R>eplace / <U>ndo / <L>oad / <S>ave / <Q>uit: ";
			cin >> choice;
		}


		cin.ignore();

		switch (choice)
		{
		case 'e':
		case 'E':
			copyCanvas(backUpCanvas, canvas);
			editCanvas(canvas);
			break;
		case 'm':
		case 'M':
			copyCanvas(backUpCanvas, canvas);
			displayCanvas(canvas);
			gotoxy(23, 0);
			cout << "Enter column units to move: ";
			cin >> cols;
			cin.ignore();
			gotoxy(24, 0);
			cout << "Enter row units to move: ";
			cin >> rows;
			cin.ignore();
			moveCanvas(canvas, rows, cols);
			break;
		case 'r':
		case 'R':
			copyCanvas(backUpCanvas, canvas);
			displayCanvas(canvas);
			gotoxy(23, 0);
			cout << "Enter character to replace: ";
			cin.getline(oldCH, 2);
			oldC = oldCH[0];
			gotoxy(24, 0);
			cout << "Enter character to replace with: ";
			cin.getline(newCH, 2);
			newC = newCH[0];
			replace(canvas, oldC, newC);
			break;
		case 'u':
		case 'U':
			copyCanvas(canvas, backUpCanvas);
			break;
		case 'l':
		case 'L':
			copyCanvas(backUpCanvas, canvas);
			loadCanvas(canvas);
			break;
		case 's':
		case 'S':
			saveCanvas(canvas);
			break;
		}

	} while (choice != 'q' && choice != 'Q');

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
	// TODO: write code for the function
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
	displayCanvas(canvas);
	gotoxy(23, 0);
	cout << "Press ESC to stop editing";
	gotoxy(row, col);
	input = _getch();
	while (input != ESC)
	{
		if (input == SPECIAL) 
		{
			switch (input = _getch())
			{
			case LEFTARROW:
				if(col -1 >= 0)
				col -= 1;
				gotoxy(row, col);
				break;

			case UPARROW:
				if(row -1 >= 0)
				row -= 1;
				gotoxy(row, col);
				break;

			case RIGHTARROW:
				if(col + 1 < MAXCOLS)
				col += 1;
				gotoxy(row, col);
				break;

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
			gotoxy(23, 0);
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
	// up to the left

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

	//down to the right
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
	// down to the left
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
	// up to the right
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
	// TODO: write code for the function
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
	system("cls");
	for (int i = 0; i < MAXROWS+1; i++)
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

	// TODO: write code for the function
}


void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS])
{
	// TODO: write code for the function
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			to[i][j] = from[i][j];
		}
	}
}


void saveCanvas(char canvas[][MAXCOLS])
{
	// TODO: write code for the function
	{
		displayCanvas(canvas);
		char fileName[FILENAMESIZE];
		char path[FILENAMESIZE] = "SavedFiles\\";
		const char txt[FILENAMESIZE] = ".txt";
		ofstream outFile;
		gotoxy(23, 1);
		cout << "Enter the filename (dont enter 'txt'): ";
		cin.getline(fileName, FILENAMESIZE);
		strncat_s(path, fileName, FILENAMESIZE - 15);
		strncat_s(path, txt, FILENAMESIZE);
		outFile.open(path);

		if (!outFile)
		{
			cerr << "ERROR: output file could not be created\n";
			system("pause");
			return;
		}

		for (int i = 0; i < MAXROWS; i++)
		{
			for (int x = 0; x < MAXCOLS; x++)
			{
				outFile << canvas[i][x];
			}

		}

		outFile.close();
		cout << "Output file written!\n";
		system("pause");

		return;
	}
}


void loadCanvas(char canvas[][MAXCOLS])
{
	// TODO: write code for the function
	displayCanvas(canvas);
	initCanvas(canvas);
	int row = 0, col = 0, i = 0;
	char c;
	char fileName[FILENAMESIZE];
	char path[FILENAMESIZE] = "SavedFiles\\";
	const char txt[FILENAMESIZE] = ".txt";
	ifstream inFile;
	gotoxy(23, 1);
	cout << "Enter the filename (dont enter 'txt'): ";
	cin.getline(fileName, FILENAMESIZE - 15);
	strncat_s(path, fileName, FILENAMESIZE - 15);
	strncat_s(path, txt, FILENAMESIZE);
	inFile.open(path);

	if (!inFile)
	{
		cerr << "ERROR: File cannot be read.\n";
		system("pause");
		return;
	} 
	else 
	{

	c = inFile.get();
	while (!inFile.eof() || row < MAXROWS)
	{
		if (c == '\n' && i < MAXCOLS) 
		{
			canvas[row][col] = ' ';
		}
		else if (i < MAXCOLS)
		{
			canvas[row][col] = c;
		}
		if (c == '\n' || i > 150)
		{
			col = 0;
			row++;
			i = 0;
		}
		else 
		{
			i++;
			col++;
		}
		c = inFile.get();
	}
}
}