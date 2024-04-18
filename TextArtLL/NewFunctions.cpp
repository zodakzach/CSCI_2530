#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Definitions.h"
using namespace std;


Point::Point(DrawPoint p)
{
	row = (int)round(p.row);
	col = (int)round(p.col);
}

// https://math.stackexchange.com/questions/39390/determining-end-coordinates-of-line-with-the-specified-length-and-angle
DrawPoint findEndPoint(DrawPoint start, int len, int angle)
{
	DrawPoint end;
	end.col = start.col + len * cos(degree2radian(angle));
	end.row = start.row + len * sin(degree2radian(angle));
	return end;
}


// Use this to draw characters into the canvas, with the option of performing animation
void drawHelper(char canvas[][MAXCOLS], Point p, char ch, bool animate)
{
	// Pause time between steps (in milliseconds)
	const int TIME = 50;

	// Make sure point is within bounds
	if (p.row >= 0 && p.row < MAXROWS && p.col >= 0 && p.col < MAXCOLS)
	{
		// Draw character into the canvas
		canvas[p.row][p.col] = ch;

		// If animation is enabled, draw to screen at same time
		if (animate)
		{
			gotoxy(p.row, p.col);
			printf("%c", ch);
			Sleep(TIME);
		}
	}
}


// Fills gaps in a row caused by mismatch between match calculations and screen coordinates
// (i.e. the resolution of our 'canvas' isn't very good)
void drawLineFillRow(char canvas[][MAXCOLS], int col, int startRow, int endRow, char ch, bool animate)
{
	// determine if we're counting up or down
	if (startRow <= endRow)
		for (int r = startRow; r <= endRow; r++)
		{
			Point point(r, col);
			drawHelper(canvas, point, ch, animate);
		}
	else
		for (int r = startRow; r >= endRow; r--)
		{
			Point point(r, col);
			drawHelper(canvas, point, ch, animate);
		}
}


// Draw a single line from start point to end point
void drawLine(char canvas[][MAXCOLS], DrawPoint start, DrawPoint end, bool animate)
{
	char ch;

	Point scrStart(start);
	Point scrEnd(end);

	// vertical line
	if (scrStart.col == scrEnd.col)
	{
		ch = '|';

		drawLineFillRow(canvas, scrStart.col, scrStart.row, scrEnd.row, ch, animate);
	}
	// non-vertical line
	else
	{
		int row = -1, prevRow;

		// determine the slope of the line
		double slope = (start.row - end.row) / (start.col - end.col);

		// choose appropriate characters based on 'steepness' and direction of slope
		if (slope > 1.8)  ch = '|';
		else if (slope > 0.08)  ch = '`';
		else if (slope > -0.08)  ch = '-';
		else if (slope > -1.8) ch = '\'';
		else ch = '|';

		// determine if columns are counting up or down
		if (scrStart.col <= scrEnd.col)
		{
			// for each column from start to end, calculate row values
			for (int col = scrStart.col; col <= scrEnd.col; col++)
			{
				prevRow = row;
				row = (int)round(slope * (col - start.col) + start.row);

				// draw from previous row to current row (to fill in row gaps)
				if (prevRow > -1)
				{
					drawLineFillRow(canvas, col, prevRow, row, ch, animate);
				}
			}
		}
		else
		{
			// for each column from start to end, calculate row values
			for (int col = scrStart.col; col >= scrEnd.col; col--)
			{
				prevRow = row;
				row = (int)round(slope * (col - start.col) + start.row);

				// draw from previous row to current row (to fill in row gaps)
				if (prevRow > -1)
				{
					drawLineFillRow(canvas, col, prevRow, row, ch, animate);
				}
			}
		}
	}
}


// Draws a single box around a center point
void drawBox(char canvas[][MAXCOLS], Point center, int height, bool animate)
{
	int sizeHalf = height / 2;
	int ratio = (int)round(MAXCOLS / (double)MAXROWS * sizeHalf);

	// Calculate where the four corners of the box should be
	DrawPoint points[4];
	points[0] = DrawPoint(center.row - sizeHalf, center.col - ratio);
	points[1] = DrawPoint(center.row - sizeHalf, center.col + ratio);
	points[2] = DrawPoint(center.row + sizeHalf, center.col + ratio);
	points[3] = DrawPoint(center.row + sizeHalf, center.col - ratio);

	// Draw the four lines of the box
	for (int x = 0; x < 3; x++)
	{
		drawLine(canvas, points[x], points[x + 1], animate);
	}
	drawLine(canvas, points[3], points[0], animate);

	// Replace the corners with a better looking character
	for (int x = 0; x < 4; x++)
	{
		drawHelper(canvas, points[x], '+', animate);
	}
}


// Menu for the drawing tools
void menuTwo(Node*& current, List& undoList, List& redoList, List& clips, bool& animate)
{
	// TODO: Write the code for the function
	char choice, center, startPoint, endPoint, fill, Nested, tree;
	char menu[200] = "<F>ill / <L>ine / <B>ox / <N>ested Boxes / <T>ree / <M>ain Menu: ";
	int menuSize = sizeof(menu) + 5;
	int size, largestBox, treeHeight, branchAngle;
	int row = 0, col = 0;
	Point boxCenter(0, 0);
	Point startLine(0, 0);
	Point endLine(0, 0);
	Point fillPoint(0, 0);
	Point NestedBoxPoint(0, 0);
	Point TreeStart(0, 0);
	DrawPoint TreeDraw(0, 0);

	char Animate = 'N';
	Node* backup;
	
	int undoCount;
	int redoCount;
	int clipsCount;

	backup = newCanvas();

	// TODO: write code for the main program
	do
	{
		undoCount = undoList.count;
		redoCount = redoList.count;
		clipsCount = clips.count;

		displayCanvas(current->item);
		clearLine(MAXROWS + 1, menuSize);
		clearLine(MAXROWS + 2, menuSize);

		if (animate == false)
		{
			Animate = 'N';
		}
		else
		{
			Animate = 'Y';
		}

		gotoxy(MAXROWS + 1, 0);
		if (clips.count > 1 && redoList.count > 0)
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
		else if (clips.count > 1 && redoList.count == 0)
		{
			cout << "<A>nimate: ";
			cout << Animate;
			cout << " / <U>ndo: ";
			cout << undoCount;
			cout << " / Cl<I>p: ";
			cout << clipsCount;
			cout << " / <P>lay";
		}
		else if (clips.count < 2 && redoList.count == 0)
		{
			cout << "<A>nimate: ";
			cout << Animate;
			cout << " / <U>ndo: ";
			cout << undoCount;
			cout << " / Cl<I>p: ";
			cout << clipsCount;
		}
		else if (clips.count < 2 && redoList.count > 0)
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
		case 'O':
		case 'o':
			if (redoList.count > 0)
			{
				addNode(undoList, newCanvas(current));
				current = removeNode(redoList);
			}
			break;
		case 'P':
		case 'p':
			if (clips.count >= 2)
			{
				clearLine(MAXROWS + 1, menuSize);
				clearLine(MAXROWS + 2, menuSize);
				gotoxy(MAXROWS + 1, 0);
				cout << "Hold <ESC> to stop";
				cout << "       Clip: ";
				cout << clips.count;
				play(clips);
			}
			break;
		case 'i':
		case 'I':
			addNode(clips, newCanvas(current));
			break;
		case 'u':
		case 'U':
			if (undoList.count > 0)
			{
				restore(undoList, redoList, current);
			}
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
		case 'F':
		case 'f':
			addUndoState(undoList, redoList, current);
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Enter character to fill with from current location / <ESC> to cancel";
			fill = getPoint(fillPoint);
			if (fill != ESC) 
			{
				fillRecursive(current->item, fillPoint.row, fillPoint.col, current->item[fillPoint.row][fillPoint.col], fill, animate);
			}
			break;
		case 'B':
		case 'b':
			addUndoState(undoList, redoList, current);
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Enter size: ";
			cin >> size;
			clearLine(MAXROWS + 1, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Type any letter to choose box center, or <C> for screen center / <ESC> to cancel";
			center = getPoint(boxCenter);
			if (center != ESC && center != 'C' && center != 'c')
			{
				drawBox(current->item, boxCenter, size, animate);
			}
			if (center == 'c' || center == 'C')
			{
				boxCenter.row = MAXROWS / 2;
				boxCenter.col = MAXCOLS / 2;
				drawBox(current->item, boxCenter, size, animate);
			}
			break;
		case 'L':
		case 'l':
			addUndoState(undoList, redoList, current);
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Type any letter to choose start point / <ESC> to cancel";
			startPoint = getPoint(startLine);
			if (startPoint != ESC) 
			{
				gotoxy(startLine.row, startLine.col);
				cout << startPoint;

				clearLine(MAXROWS + 1, menuSize);
				gotoxy(MAXROWS + 1, 0);
				cout << "Type any letter to choose end point / <ESC> to cancel";
				endPoint = getPoint(endLine);
				if (endPoint != ESC) 
				{
					drawLine(current->item, startLine, endLine, animate);
				}
			}
			break;
		case 'n':
		case 'N':
			addUndoState(undoList, redoList, current);
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Enter size of largest box: ";
			cin >> largestBox;
			clearLine(MAXROWS + 1, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Type any letter to choose box center, or <C> for screen center / <ESC> to cancel";
			Nested = getPoint(NestedBoxPoint);
			if(Nested != ESC && Nested != 'C' && Nested != 'c')
			{
				drawBoxesRecursive(current->item, NestedBoxPoint, largestBox, animate);
			}
			if (Nested == 'c' || Nested == 'C')
			{
				NestedBoxPoint.row = MAXROWS / 2;
				NestedBoxPoint.col = MAXCOLS / 2;
				drawBoxesRecursive(current->item, NestedBoxPoint, largestBox, animate);
			}
			break;
		case 'T':
		case 't':
			addUndoState(undoList, redoList, current);
			clearLine(MAXROWS + 1, menuSize);
			clearLine(MAXROWS + 2, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Enter approximate tree height: ";
			cin >> treeHeight;
			clearLine(MAXROWS + 1, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Enter branch angle: ";
			cin >> branchAngle;
			clearLine(MAXROWS + 1, menuSize);
			gotoxy(MAXROWS + 1, 0);
			cout << "Type any letter to choose start point, or <C> for bottom center / <ESC> to cancel";
			tree = getPoint(TreeStart);
			if (tree != ESC && tree != 'C' && tree != 'c')
			{
				TreeDraw.row = TreeStart.row;
				TreeDraw.col = TreeStart.col;
				treeRecursive(current->item, TreeDraw, treeHeight, 270, branchAngle, animate);
			}
			if (tree == 'c' || tree == 'C')
			{
				TreeDraw.row = MAXROWS - 1;
				TreeDraw.col = MAXCOLS / 2;
				treeRecursive(current->item, TreeDraw, treeHeight, 270, branchAngle, animate);
			}
			break;
		}

	} while (choice != 'm' && choice != 'M');

	return;
}





// Get a single point from screen, with character entered at that point
char getPoint(Point& pt)
{
	char input;
	int row = 0, col = 0;

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
				if (col - 1 >= 0)
					col -= 1;
				gotoxy(row, col);
				break;
				// allows user to use the up arrow key to move up on canvas
			case UPARROW:
				if (row - 1 >= 0)
					row -= 1;
				gotoxy(row, col);
				break;
				// allows user to use the right arrow key to move right on canvas
			case RIGHTARROW:
				if (col + 1 < MAXCOLS)
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
			pt.row = row;
			pt.col = col;
			return input;
		}

		input = _getch();
	}

	return ESC;

}


// Recursively fill a section of the screen
void fillRecursive(char canvas[][MAXCOLS], int row, int col, char oldCh, char newCh, bool animate)
{
	// TODO: Write the code for the function
	if ((row < MAXROWS && row >= 0) && (col < MAXCOLS && col >= 0) && (canvas[row][col] == oldCh))
	{
		if (canvas[row][col] == oldCh)
		{
			Point pt; pt.row = row; pt.col = col;
			drawHelper(canvas, pt, newCh, animate);
			fillRecursive(canvas, row, col, oldCh, newCh, animate);
		}
		fillRecursive(canvas, row - 1, col, oldCh, newCh, animate);
		fillRecursive(canvas, row + 1, col, oldCh, newCh, animate);
		fillRecursive(canvas, row, col - 1, oldCh, newCh, animate);
		fillRecursive(canvas, row, col + 1, oldCh, newCh, animate);

	}
}


// Recursively draw a tree
void treeRecursive(char canvas[][MAXCOLS], DrawPoint start, int height, int startAngle, int branchAngle, bool animate)
{
	if (height>2)
	{
		DrawPoint end = findEndPoint(start, height / 3, startAngle);
		drawLine(canvas, start, end, animate);
		treeRecursive(canvas, end, height - 2, branchAngle + startAngle, branchAngle, animate);
		treeRecursive(canvas, end, height - 2, startAngle - branchAngle, branchAngle, animate);

	}

}


// Recursively draw nested boxes
void drawBoxesRecursive(char canvas[][MAXCOLS], Point center, int height, bool animate)
{
	// TODO: Write the code for the function
	drawBox(canvas, center, height, animate);
	if (height < 2)
	{
		return;
	}
	drawBoxesRecursive(canvas, center, height - 2, animate);
}