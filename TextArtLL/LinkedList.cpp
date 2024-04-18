#include <iostream>
#include <Windows.h>
#include "Definitions.h"
using namespace std;


Node* newCanvas()
{
	// TODO: Write the code for the function
	Node* head = NULL;
	head = new Node;
	head->next = NULL;
	return head;
}


Node* newCanvas(Node* oldNode)
{
	// TODO: Write the code for the function
	Node* newCanvas = NULL;
	newCanvas = new Node;
	copyCanvas(newCanvas->item, oldNode->item);
	newCanvas->next = NULL;
	return newCanvas;
}


void play(List& clips)
{
	// loops as long as the ESCAPE key is not currently being pressed
	while (!(GetKeyState(VK_ESCAPE) & 0x8000))
	{
			playRecursive(clips.head, clips.count);
	}
}


void playRecursive(Node* head, int count)
{
	// TODO: Write the code for the function
	int countN = count;
	if (count == 0)
	{
		return;
	}
	displayCanvas(head->item);
	gotoxy(MAXROWS + 1, 31);
	cout << countN;
	countN--;
	// Pause for 100 milliseconds to slow down animation
	Sleep(100);
	playRecursive(head->next, countN);
}


void addUndoState(List& undoList, List& redoList, Node* &current)
{
	// TODO: Write the code for the function
	addNode(undoList, newCanvas(current));
	deleteList(redoList);
}


void restore(List& undoList, List& redoList, Node* &current)
{
	// TODO: Write the code for the function
	addNode(redoList, current);
	Node* backup = removeNode(undoList);
	if (backup != NULL)
	{
		current = backup;
	}
	else
	{
		initCanvas(current->item);
	}
}


void addNode(List& list, Node* nodeToAdd)
{
	// TODO: Write the code for the function
	nodeToAdd->next = list.head;
	list.head = nodeToAdd;
	list.count++;
}


Node* removeNode(List& list)
{
	// TODO: Write the code for the function
	if (list.head != NULL) 
	{
		Node* temp = list.head;
		list.head = list.head->next;
		list.count--;
		return temp;
	}
	else
	{
		return NULL;
	}
}


void deleteList(List& list)
{
	// TODO: Write the code for the function
	while (list.head != NULL) 
	{
		Node* temp = list.head->next;
		delete list.head;
		list.head = temp;
		list.count--;
	}
}


bool loadClips(List& clips, char filename[])
{
	// TODO: Write the code for the function
	const char txt[FILENAMESIZE] = ".txt";
	char FileName[FILENAMESIZE];
	const char dash = '-';
	int count = 1;


	deleteList(clips);
	do
	{
		snprintf(FileName, FILENAMESIZE - 15, "%s%c%d%s", filename, dash, count, txt);
		addNode(clips, newCanvas());
		count++;
	} while (loadCanvas(clips.head->item, FileName) != false);
	removeNode(clips);

	return true;
}


bool saveClips(List& clips, char filename[])
{
	const char txt[FILENAMESIZE] = ".txt";
	char FileName[FILENAMESIZE];
	const char dash = '-';
	int count = 1;
	for (Node* current = clips.head; current != NULL; current = current->next)
	{
		// assemble the filename
		snprintf(FileName, FILENAMESIZE - 15, "%s%c%d%s", filename, dash, count, txt);
		saveCanvas(current->item, FileName);
		count++;
		// try to save the current canvas to the file
		if (saveCanvas(current->item, FileName) == false)
		{
			return false;
		}
		// return false if the save didn't work
	}
	return true;
}