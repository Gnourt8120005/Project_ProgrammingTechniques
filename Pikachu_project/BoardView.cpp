﻿#include "BoardView.h"

BoardView::BoardView(int psize, int pX, int pY)
{
	size = psize;
	left = pX;
	top = pY;
	pokemons = new int *[size];
	for (int i = 0; i < size; i++)
		pokemons[i] = new int[size];
	pBoard = new Point *[size];
	for (int i = 0; i < size; i++)
		pBoard[i] = new Point[size];
	background = new string[size * 10];
	// Location of first pikachu (10,3)
	// Each pikachu has 8space in horizal
	//					4space in vertical
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			pBoard[i][j].setX(8 * j + left + 5); // x-value of boardgame
			pBoard[i][j].setY(4 * i + top + 2);	 // y-value of boardgame
			pBoard[i][j].setCheck(0);
		}
}

BoardView::~BoardView()
{
	for (int i = 0; i < size; i++)
		delete[] pBoard[i];
	delete[] pBoard,
		pBoard = nullptr;
	delete[] background;
	background = nullptr;
}

void BoardView::createBackground()
{
	ifstream bg;
	if (size == 4)
		bg.open("images\\easy.txt");
	else
		bg.open("images\\medium.txt");
	int i = 0;
	while (!bg.eof())
	{
		getline(bg, background[i]);
		i++;
	}
	bg.close();
}

int BoardView::getSize()
{
	return size;
}

int BoardView::getLeft()
{
	return left;
}

int BoardView::getTop()
{
	return top;
}

// get location X at pboard[i][j]
int BoardView::getXAt(int i, int j)
{
	return pBoard[i][j].getX();
}
// get location Y at pboard[i][j]
int BoardView::getYAt(int i, int j)
{
	return pBoard[i][j].getY();
}
// get the index of row at the (x,y)
int BoardView::getRAt(int x, int y)
{
	return (y - 2 - top) / 4;
}
// get the index of col at the (x,y)
int BoardView::getCAt(int x, int y)
{
	return (x - 5 - left) / 8;
}

// get pokemon at the location (x,y) on the screen
char BoardView::getPokemons(int x, int y)
{
	return pBoard[getRAt(x, y)][getCAt(x, y)].getPokemons();
}

//
int BoardView::getCheckAtXY(int pX, int pY)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (pBoard[i][j].getX() == pX && pBoard[i][j].getY() == pY)
				return pBoard[i][j].getCheck();
		}
	}
	throw "Problem with cursor position";
}

// check the state of cell at (x,y) i
int BoardView::getCheck(int x, int y)
{
	int r = getRAt(x, y);
	int c = getCAt(x, y);
	return pBoard[r][c].getCheck();
}
/////////////////////////////////////////////////////////////////////
// print the border of board
void BoardView::showBoard()
{
	//Controller::clearConsole();
	Controller::setConsoleColor(BLACK, WHITE);
	// Draw top line
	Controller::gotoXY(left + 1, top);
	putchar(201); //left top corner
	for (int i = 1; i < size * 8; i++)
	{
		Sleep(5);
		putchar(205);
	}
	putchar(187); // right top corner

	// Draw right line
	for (int i = 1; i < size * 4; i++)
	{
		Sleep(10);
		Controller::gotoXY(size * 8 + left + 1, i + top);
		putchar(186);
	}
	Controller::gotoXY(size * 8 + left + 1, size * 4 + top);
	putchar(188); // right bottom corner

	// Draw bottom line
	//Controller::gotoXY(left + 1, top);
	for (int i = 1; i < size * 8; i++)
	{
		Controller::gotoXY(size * 8 + left - i + 1, size * 4 + top);
		Sleep(5);
		putchar(205);
	}
	Controller::gotoXY(left + 1, size * 4 + top);
	putchar(200); // left bottom corner

	// Draw left line
	for (int i = 1; i < size * 4; i++)
	{
		Sleep(10);
		Controller::gotoXY(left + 1, size * 4 + top - i);
		putchar(186);
	}

	
}
// print the pokemon on the board
void BoardView::renderBoard()
{
	Controller::setConsoleColor(BLACK, WHITE);
	//Draw vertical lines
	for (int i = 1; i < size * 4; i++)
	{
		for (int j = 8; j < size * 8; j += 8)
		{
			if (i % 4 != 0)
			{
				Controller::gotoXY(j + left + 1, i + top);
				putchar(179);
			}
		}
		//Sleep(10);
	}

	// Draw horizontal lines
	for (int i = 1; i < size * 8; i++)
	{
		for (int j = 4; j < size * 4; j += 4)
		{
			Controller::gotoXY(i + left + 1, j + top);
			if (i % 8 == 0)
				putchar(32);
			else
				putchar(196);
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			if (pBoard[i][j].getCheck() != _DELETE)
			{
				// if this cell is not deleted then print the pokemon
				int x = pBoard[i][j].getX();
				int y = pBoard[i][j].getY();
				for (int i = y - 1; i <= y + 1; i++)
					for (int j = x - 3; j <= x + 3; j++)
					{
						Controller::gotoXY(j, i);
						putchar(32);
					}
				Controller::gotoXY(x, y);
				Controller::setConsoleColor(BLACK, WHITE);
				if (blind == TRUE)
					putchar(32);
				else 
					putchar(pBoard[i][j].getPokemons());
			}
			else
				BoardView::deleteBlock(BoardView::getXAt(i, j), BoardView::getYAt(i, j));
	}
}
// create the data(pokemon) for each cell
void BoardView::buildBoardData()
{
	srand((unsigned int)time(NULL));

	bool *checkDuplicate = new bool[size * size];
	int *pos = new int[size * size];

	// Random pokemons
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j += 2)
		{
			pokemons[i][j] = pokemons[i][j + 1] = rand() % 26 + 'A';
		}
	}

	// Random pokemons position
	for (int i = 0; i < size * size; i++)
		checkDuplicate[i] = 0;
	for (int i = 0; i < size * size; i++)
	{
		int tmp = 0;
		do
		{
			tmp = rand() % (size * size);
		} while (checkDuplicate[tmp]);
		checkDuplicate[tmp] = 1;
		pos[i] = tmp;
	}

	// Construct pokemons matrix
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int r = pos[size * i + j] / size;
			int c = pos[size * i + j] % size;
			pBoard[i][j].setPokemons(pokemons[r][c]);
		}
	}

	delete[] pos;
	delete[] checkDuplicate;
}

void BoardView::slideBlock(pair<int, int> block) {
	int row = getRAt(block.first, block.second);
	int col = getCAt(block.first, block.second);
	if (col == 0 || pBoard[row][col - 1].getCheck() == _DELETE) {
		return;
	}
	else {
		int i = col;
		while (i > 0 && pBoard[row][i - 1].getCheck() != _DELETE) {
			char temp = pBoard[row][i - 1].getPokemons();
			pBoard[row][i].setPokemons(temp);
			pBoard[row][i].setCheck(_NORMAL); 
			i--;
		}
		pBoard[row][i].setCheck(_DELETE);
	}
}
/////////////////////////////////////////////////////////////////////
// highlight the selected block (where the cusor be)
void BoardView::selectedBlock(int x, int y, int color)
{
	// if the cell at (x,y) is not deleted
	if (getCheck(x, y) != _DELETE)
	{
		Controller::setConsoleColor(color, BLACK);
		for (int i = y - 1; i <= y + 1; i++)
			for (int j = x - 3; j <= x + 3; j++)
			{
				Controller::gotoXY(j, i);
				putchar(32);
			}
		Controller::gotoXY(x, y);
		putchar(getPokemons(x, y));
		Controller::gotoXY(x, y);
	}
	// if the cell at (x,y) is deleted
	else
	{
		Controller::setConsoleColor(color, YELLOW);
		for (int i = y - 1; i <= y + 1; i++)
			for (int j = x - 3; j <= x + 3; j++)
			{
				Controller::gotoXY(j, i);
				putchar(background[i - top][j - left]);
			}
	}
}
// unhighlight the block
void BoardView::unselectedBlock(int x, int y)
{
	// int r = getRAt(x, y);
	// int c = getCAt(x, y);
	// if(getCheck(x, y) != _DELETE)
	//	pBoard[r][c].setCheck(_NORMAL);

	// if the cell at (x,y) is not deleted
	if (getCheck(x, y) != _DELETE)
	{
		int r = getRAt(x, y);
		int c = getCAt(x, y);
		pBoard[r][c].setCheck(_NORMAL);
		Controller::setConsoleColor(BLACK, WHITE);
		for (int i = y - 1; i <= y + 1; i++)
			for (int j = x - 3; j <= x + 3; j++)
			{
				Controller::gotoXY(j, i);
				putchar(32);
			}
		Controller::gotoXY(x, y);
		if (blind == TRUE)
			putchar(32);
		else 
			putchar(getPokemons(x, y));
		Controller::gotoXY(x, y);
	}
	// if the cell at (x,y) is deleted
	else
	{
		Controller::setConsoleColor(BLACK, YELLOW);
		for (int i = y - 1; i <= y + 1; i++)
			for (int j = x - 3; j <= x + 3; j++)
			{
				Controller::gotoXY(j, i);
				putchar(background[i - top][j - left]);
			}
	}
}
// highlight the locked block
void BoardView::lockBlock(int x, int y)
{
	int r = getRAt(x, y);
	int c = getCAt(x, y);
	pBoard[r][c].setCheck(_LOCK);
	Controller::setConsoleColor(RED, BLACK);
	for (int i = y - 1; i <= y + 1; i++)
		for (int j = x - 3; j <= x + 3; j++)
		{
			Controller::gotoXY(j, i);
			putchar(32);
		}
	Controller::gotoXY(x, y);
	putchar(getPokemons(x, y));
	Controller::gotoXY(x, y);
}
// set the state of this block to _DELETE
// and insert the background
void BoardView::deleteBlock(int x, int y)
{
	int r = getRAt(x, y);
	int c = getCAt(x, y);
	pBoard[r][c].setCheck(_DELETE);
	// print the image of background
	Controller::setConsoleColor(BLACK, YELLOW);
	for (int i = y - 1; i <= y + 1; i++)
		for (int j = x - 3; j <= x + 3; j++) {
			Controller::gotoXY(j, i);
			putchar(background[i - top][j - left]);
		}
	// Delete top border
	Controller::gotoXY(x, y);
	if (y - 4 >= getYAt(0, 0) && getCheck(x, y - 4) == _DELETE)
		for (int i = x - 3; i <= x + 3; i++) {
			Controller::gotoXY(i, y - 2);
			// putchar(32);
			putchar(background[y - 2 - top][i - left]);
		}
	// Delete bottom border
	if (y + 4 <= getYAt(size - 1, size - 1) && getCheck(x, y + 4) == _DELETE)
		for (int i = x - 3; i <= x + 3; i++)
		{
			Controller::gotoXY(i, y + 2);
			// putchar(32);
			putchar(background[y + 2 - top][i - left]);
		}
	// Delete left border
	if (x - 8 >= getXAt(0, 0) && getCheck(x - 8, y) == _DELETE)
		for (int i = y - 1; i <= y + 1; i++)
		{
			Controller::gotoXY(x - 4, i);
			// putchar(32);
			putchar(background[i - top][x - 4 - left]);
		}

	// Delete right border
	if (x + 8 <= getXAt(size - 1, size - 1) && getCheck(x + 8, y) == _DELETE)
		for (int i = y - 1; i <= y + 1; i++)
		{
			Controller::gotoXY(x + 4, i);
			// putchar(32);
			putchar(background[i - top][x + 4 - left]);
		}
}

////////////////////////////////////////////////////////////////////
void BoardView::drawLineI(pair<int, int> firstBlock, pair<int, int> secondBlock)
{
	Controller::setConsoleColor(RED, BRIGHT_WHITE);
	if (firstBlock.first == secondBlock.first)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(30);
		for (int i = firstBlock.second + 2; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(31);
		return;
	}
	//========================================================================//
	if (firstBlock.second == secondBlock.second)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(17);
		for (int i = firstBlock.first + 2; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(16);
		return;
	}
}

void BoardView::deleteLineI(pair<int, int> firstBlock, pair<int, int> secondBlock)
{
	Controller::setConsoleColor(BLACK, BRIGHT_WHITE);
	if (firstBlock.first == secondBlock.first)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(32);
		for (int i = firstBlock.second + 2; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(32);
		return;
	}
	//========================================================================//
	if (firstBlock.second == secondBlock.second)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(32);
		for (int i = firstBlock.first + 2; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(32);
		return;
	}
}

void BoardView::drawLineL(pair<int, int> firstBlock, pair<int, int> secondBlock, pair<int, int> Lcorner)
{
	Controller::setConsoleColor(RED, BRIGHT_WHITE);
	// down-left corner
	if (Lcorner.first < secondBlock.first && Lcorner.second > firstBlock.second)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(30);
		for (int i = firstBlock.second + 2; i <= Lcorner.second - 1; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		for (int i = Lcorner.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(16);
		return;
	}
	//========================================================================//
	// up-left corner
	if (Lcorner.first < secondBlock.first && Lcorner.second < firstBlock.second)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second - 1);
		putchar(31);
		for (int i = firstBlock.second - 2; i >= Lcorner.second + 1; i--)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		for (int i = Lcorner.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(16);
		return;
	}
	//========================================================================//
	// up-right corner
	if (Lcorner.second < secondBlock.second && Lcorner.first > firstBlock.first)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(17);
		for (int i = firstBlock.first + 2; i <= Lcorner.first; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		for (int i = Lcorner.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(31);
		return;
	}
	//========================================================================//
	// down-right corner
	if (Lcorner.second > secondBlock.second && Lcorner.first > firstBlock.first)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(17);
		for (int i = firstBlock.first + 2; i <= Lcorner.first; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		for (int i = Lcorner.second - 1; i >= secondBlock.second + 2; i--)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second + 1);
		putchar(30);
		return;
	}
}

void BoardView::deleteLineL(pair<int, int> firstBlock, pair<int, int> secondBlock, pair<int, int> Lcorner)
{
	Controller::setConsoleColor(BLACK, BRIGHT_WHITE);
	// down-left corner
	if (Lcorner.first < secondBlock.first && Lcorner.second > firstBlock.second)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(32);
		for (int i = firstBlock.second + 2; i <= Lcorner.second - 1; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		for (int i = Lcorner.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(32);
		return;
	}
	//========================================================================//
	// up-left corner
	if (Lcorner.first < secondBlock.first && Lcorner.second < firstBlock.second)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second - 1);
		putchar(32);
		for (int i = firstBlock.second - 2; i >= Lcorner.second + 1; i--)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		for (int i = Lcorner.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(32);
		return;
	}
	//========================================================================//
	// up-right corner
	if (Lcorner.second < secondBlock.second && Lcorner.first > firstBlock.first)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(32);
		for (int i = firstBlock.first + 2; i <= Lcorner.first; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		for (int i = Lcorner.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(32);
		return;
	}
	//========================================================================//
	// down-right corner
	if (Lcorner.second > secondBlock.second && Lcorner.first > firstBlock.first)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(32);
		for (int i = firstBlock.first + 2; i <= Lcorner.first; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		for (int i = Lcorner.second - 1; i >= secondBlock.second + 2; i--)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second + 1);
		putchar(32);
		return;
	}
}

void BoardView::drawLineZ(pair<int, int> firstBlock, pair<int, int> secondBlock, pair<int, int> Zcorner1, pair<int, int> Zcorner2)
{
	Controller::setConsoleColor(RED, BRIGHT_WHITE);
	// down-left corner
	if (Zcorner1.first < secondBlock.first && Zcorner1.second > firstBlock.second)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(30);
		for (int i = firstBlock.second + 2; i <= Zcorner1.second - 1; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		for (int i = Zcorner1.first; i <= Zcorner2.first; i++)
		{
			Controller::gotoXY(i, Zcorner1.second);
			putchar(45);
		}
		for (int i = Zcorner2.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(31);
		return;
	}
	//========================================================================//
	// up-left corner
	if (Zcorner1.first > secondBlock.first && Zcorner1.second > firstBlock.second)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(30);
		for (int i = firstBlock.second + 2; i <= Zcorner1.second - 1; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		for (int i = Zcorner1.first; i >= Zcorner2.first; i--)
		{
			Controller::gotoXY(i, Zcorner1.second);
			putchar(45);
		}
		for (int i = Zcorner2.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(31);
		return;
	}
	//========================================================================//
	// up-right corner
	if (Zcorner1.second < secondBlock.second && Zcorner1.first > firstBlock.first)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(17);
		for (int i = firstBlock.first + 2; i <= Zcorner1.first; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		for (int i = Zcorner1.second + 1; i <= Zcorner2.second - 1; i++)
		{
			Controller::gotoXY(Zcorner1.first, i);
			putchar(179);
		}
		for (int i = Zcorner2.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(16);
		return;
	}
	//========================================================================//
	// down-right corner
	if (Zcorner1.second > secondBlock.second && Zcorner1.first > firstBlock.first)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(17);
		for (int i = firstBlock.first + 2; i <= Zcorner1.first; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		for (int i = Zcorner1.second - 1; i >= Zcorner2.second + 1; i--)
		{
			Controller::gotoXY(Zcorner1.first, i);
			putchar(179);
		}
		for (int i = Zcorner2.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(16);
		return;
	}
}

void BoardView::deleteLineZ(pair<int, int> firstBlock, pair<int, int> secondBlock, pair<int, int> Zcorner1, pair<int, int> Zcorner2)
{
	Controller::setConsoleColor(BLACK, BRIGHT_WHITE);
	// down-left corner
	if (Zcorner1.first < secondBlock.first && Zcorner1.second > firstBlock.second)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(32);
		for (int i = firstBlock.second + 2; i <= Zcorner1.second - 1; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		for (int i = Zcorner1.first; i <= Zcorner2.first; i++)
		{
			Controller::gotoXY(i, Zcorner1.second);
			putchar(32);
		}
		for (int i = Zcorner2.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(32);
		return;
	}
	//========================================================================//
	// up-left corner
	if (Zcorner1.first > secondBlock.first && Zcorner1.second > firstBlock.second)
	{
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(32);
		for (int i = firstBlock.second + 2; i <= Zcorner1.second - 1; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		for (int i = Zcorner1.first; i >= Zcorner2.first; i--)
		{
			Controller::gotoXY(i, Zcorner1.second);
			putchar(32);
		}
		for (int i = Zcorner2.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(32);
		return;
	}
	//========================================================================//
	// up-right corner
	if (Zcorner1.second < secondBlock.second && Zcorner1.first > firstBlock.first)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(32);
		for (int i = firstBlock.first + 2; i <= Zcorner1.first; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		for (int i = Zcorner1.second + 1; i <= Zcorner2.second - 1; i++)
		{
			Controller::gotoXY(Zcorner1.first, i);
			putchar(32);
		}
		for (int i = Zcorner2.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(32);
		return;
	}
	//========================================================================//
	// down-right corner
	if (Zcorner1.second > secondBlock.second && Zcorner1.first > firstBlock.first)
	{
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(32);
		for (int i = firstBlock.first + 2; i <= Zcorner1.first; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		for (int i = Zcorner1.second - 1; i >= Zcorner2.second + 1; i--)
		{
			Controller::gotoXY(Zcorner1.first, i);
			putchar(32);
		}
		for (int i = Zcorner2.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(32);
		return;
	}
}

void BoardView::drawLineU(pair<int, int> firstBlock, pair<int, int> secondBlock, pair<int, int> Ucorner1, pair<int, int> Ucorner2)
{
	Controller::setConsoleColor(RED, BRIGHT_WHITE);
	//========================================================================//
	// U matching to the left and firstblock above secondblock
	if (Ucorner1.first < firstBlock.first && Ucorner1.second < secondBlock.second)
	{
		for (int i = Ucorner1.first; i <= firstBlock.first - 2; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		Controller::gotoXY(firstBlock.first - 1, firstBlock.second);
		putchar(16);
		for (int i = Ucorner1.second + 1; i <= Ucorner2.second - 1; i++)
		{
			Controller::gotoXY(Ucorner1.first, i);
			putchar(179);
		}
		for (int i = Ucorner2.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(16);
		return;
	}
	// U matching to the left and firstblock below secondblock
	if (Ucorner1.first < firstBlock.first && Ucorner1.second > secondBlock.second)
	{
		for (int i = Ucorner1.first; i <= firstBlock.first - 2; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		Controller::gotoXY(firstBlock.first - 1, firstBlock.second);
		putchar(16);
		for (int i = Ucorner1.second - 1; i >= Ucorner2.second + 1; i--)
		{
			Controller::gotoXY(Ucorner1.first, i);
			putchar(179);
		}
		for (int i = Ucorner2.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(16);
		return;
	}
	//========================================================================//
	// U matching to the right and firstblock above secondblock
	if (Ucorner1.first > firstBlock.first && Ucorner1.second < secondBlock.second)
	{
		for (int i = Ucorner1.first; i >= firstBlock.first + 2; i--)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(17);
		for (int i = Ucorner1.second + 1; i <= Ucorner2.second - 1; i++)
		{
			Controller::gotoXY(Ucorner1.first, i);
			putchar(179);
		}
		for (int i = Ucorner2.first; i >= secondBlock.first + 2; i--)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first + 1, secondBlock.second);
		putchar(17);
		return;
	}
	// U matching to the right and firstblock below secondblock
	if (Ucorner1.first > firstBlock.first && Ucorner1.second > secondBlock.second)
	{
		for (int i = Ucorner1.first; i >= firstBlock.first + 2; i--)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(45);
		}
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(17);
		for (int i = Ucorner1.second - 1; i >= Ucorner2.second + 1; i--)
		{
			Controller::gotoXY(Ucorner1.first, i);
			putchar(179);
		}
		for (int i = Ucorner2.first; i >= secondBlock.first + 2; i--)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(45);
		}
		Controller::gotoXY(secondBlock.first + 1, secondBlock.second);
		putchar(17);
		return;
	}
	//========================================================================//
	// U matching upward and firstblock on the left of secondblock
	if (Ucorner1.second < firstBlock.second && Ucorner1.first < secondBlock.first)
	{
		for (int i = Ucorner1.second + 1; i <= firstBlock.second - 2; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(firstBlock.first, firstBlock.second - 1);
		putchar(31);
		for (int i = Ucorner1.first; i <= Ucorner2.first; i++)
		{
			Controller::gotoXY(i, Ucorner1.second);
			putchar(45);
		}
		for (int i = Ucorner2.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(31);
		return;
	}
	// U matching upward and firstblock on the right of secondblock
	if (Ucorner1.second < firstBlock.second && Ucorner1.first > secondBlock.first)
	{
		for (int i = Ucorner1.second + 1; i <= firstBlock.second - 2; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(firstBlock.first, firstBlock.second - 1);
		putchar(31);
		for (int i = Ucorner1.first; i >= Ucorner2.first; i--)
		{
			Controller::gotoXY(i, Ucorner1.second);
			putchar(45);
		}
		for (int i = Ucorner2.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(31);
		return;
	}
	//========================================================================//
	// U matching downward and firstblock on the left of secondblock
	if (Ucorner1.second > firstBlock.second && Ucorner1.first < secondBlock.first)
	{
		for (int i = Ucorner1.second - 1; i >= firstBlock.second + 2; i--)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(30);
		for (int i = Ucorner1.first; i <= Ucorner2.first; i++)
		{
			Controller::gotoXY(i, Ucorner1.second);
			putchar(45);
		}
		for (int i = Ucorner2.second - 1; i >= secondBlock.second + 2; i--)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second + 1);
		putchar(30);
		return;
	}
	// U matching downward and firstblock on the right of secondblock
	if (Ucorner1.second > firstBlock.second && Ucorner1.first > secondBlock.first)
	{
		for (int i = Ucorner1.second - 1; i >= firstBlock.second + 2; i--)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(30);
		for (int i = Ucorner1.first; i >= Ucorner2.first; i--)
		{
			Controller::gotoXY(i, Ucorner1.second);
			putchar(45);
		}
		for (int i = Ucorner2.second - 1; i >= secondBlock.second + 2; i--)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(179);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second + 1);
		putchar(30);
		return;
	}
}

void BoardView::deleteLineU(pair<int, int> firstBlock, pair<int, int> secondBlock, pair<int, int> Ucorner1, pair<int, int> Ucorner2)
{
	Controller::setConsoleColor(BLACK, BRIGHT_WHITE);
	//========================================================================//
	// U matching to the left and firstblock above secondblock
	if (Ucorner1.first < firstBlock.first && Ucorner1.second < secondBlock.second)
	{
		for (int i = Ucorner1.first; i <= firstBlock.first - 2; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		Controller::gotoXY(firstBlock.first - 1, firstBlock.second);
		putchar(32);
		for (int i = Ucorner1.second + 1; i <= Ucorner2.second - 1; i++)
		{
			Controller::gotoXY(Ucorner1.first, i);
			putchar(32);
		}
		for (int i = Ucorner2.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(32);
		for (int i = 1; i < size * 4; i++)
		{
			Controller::gotoXY(left + 1, i + top);
			putchar(186);
		}
		return;
	}
	// U matching to the left and firstblock below secondblock
	if (Ucorner1.first < firstBlock.first && Ucorner1.second > secondBlock.second)
	{
		for (int i = Ucorner1.first; i <= firstBlock.first - 2; i++)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		Controller::gotoXY(firstBlock.first - 1, firstBlock.second);
		putchar(32);
		for (int i = Ucorner1.second - 1; i >= Ucorner2.second + 1; i--)
		{
			Controller::gotoXY(Ucorner1.first, i);
			putchar(32);
		}
		for (int i = Ucorner2.first; i <= secondBlock.first - 2; i++)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first - 1, secondBlock.second);
		putchar(32);
		for (int i = 1; i < size * 4; i++)
		{
			Controller::gotoXY(left + 1, i + top);
			putchar(186);
		}
		return;
	}
	//========================================================================//
	// U matching to the right and firstblock above secondblock
	if (Ucorner1.first > firstBlock.first && Ucorner1.second < secondBlock.second)
	{
		for (int i = Ucorner1.first; i >= firstBlock.first + 2; i--)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(32);
		for (int i = Ucorner1.second + 1; i <= Ucorner2.second - 1; i++)
		{
			Controller::gotoXY(Ucorner1.first, i);
			putchar(32);
		}
		for (int i = Ucorner2.first; i >= secondBlock.first + 2; i--)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first + 1, secondBlock.second);
		putchar(32);
		// redraw right margin
		for (int i = 1; i < size * 4; i++)
		{
			Controller::gotoXY(size * 8 + left + 1, i + top);
			putchar(186);
		}
		return;
	}
	// U matching to the right and firstblock below secondblock
	if (Ucorner1.first > firstBlock.first && Ucorner1.second > secondBlock.second)
	{
		for (int i = Ucorner1.first; i >= firstBlock.first + 2; i--)
		{
			Controller::gotoXY(i, firstBlock.second);
			putchar(32);
		}
		Controller::gotoXY(firstBlock.first + 1, firstBlock.second);
		putchar(32);
		for (int i = Ucorner1.second - 1; i >= Ucorner2.second + 1; i--)
		{
			Controller::gotoXY(Ucorner1.first, i);
			putchar(32);
		}
		for (int i = Ucorner2.first; i >= secondBlock.first + 2; i--)
		{
			Controller::gotoXY(i, secondBlock.second);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first + 1, secondBlock.second);
		putchar(32);
		// redraw right margin
		for (int i = 1; i < size * 4; i++)
		{
			Controller::gotoXY(size * 8 + left + 1, i + top);
			putchar(186);
		}
		return;
	}
	//========================================================================//
	// U matching upward and firstblock on the left of secondblock
	if (Ucorner1.second < firstBlock.second && Ucorner1.first < secondBlock.first)
	{
		for (int i = Ucorner1.second + 1; i <= firstBlock.second - 2; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(firstBlock.first, firstBlock.second - 1);
		putchar(32);
		for (int i = Ucorner1.first; i <= Ucorner2.first; i++)
		{
			Controller::gotoXY(i, Ucorner1.second);
			putchar(32);
		}
		for (int i = Ucorner2.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(32);
		// redraw margin
		Controller::gotoXY(left + 2, top);
		for (int i = 1; i < size * 8; i++)
			putchar(205);
		
		return;
	}
	// U matching upward and firstblock on the right of secondblock
	if (Ucorner1.second < firstBlock.second && Ucorner1.first > secondBlock.first)
	{
		for (int i = Ucorner1.second + 1; i <= firstBlock.second - 2; i++)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(firstBlock.first, firstBlock.second - 1);
		putchar(32);
		for (int i = Ucorner1.first; i >= Ucorner2.first; i--)
		{
			Controller::gotoXY(i, Ucorner1.second);
			putchar(32);
		}
		for (int i = Ucorner2.second + 1; i <= secondBlock.second - 2; i++)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second - 1);
		putchar(32);
		// redraw margin
		Controller::gotoXY(left + 2, top);
		for (int i = 1; i < size * 8; i++)
			putchar(205);
		return;
	}
	//========================================================================//
	// U matching downward and firstblock on the left of secondblock
	if (Ucorner1.second > firstBlock.second && Ucorner1.first < secondBlock.first)
	{
		for (int i = Ucorner1.second - 1; i >= firstBlock.second + 2; i--)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(32);
		for (int i = Ucorner1.first; i <= Ucorner2.first; i++)
		{
			Controller::gotoXY(i, Ucorner1.second);
			putchar(32);
		}
		for (int i = Ucorner2.second - 1; i >= secondBlock.second + 2; i--)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second + 1);
		putchar(32);
		Controller::gotoXY(left + 2, top + size*4);
		for (int i = 1; i < size * 8; i++)
			putchar(205);
		return;
	}
	// U matching downward and firstblock on the right of secondblock
	if (Ucorner1.second > firstBlock.second && Ucorner1.first > secondBlock.first)
	{
		for (int i = Ucorner1.second - 1; i >= firstBlock.second + 2; i--)
		{
			Controller::gotoXY(firstBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(firstBlock.first, firstBlock.second + 1);
		putchar(32);
		for (int i = Ucorner1.first; i >= Ucorner2.first; i--)
		{
			Controller::gotoXY(i, Ucorner1.second);
			putchar(32);
		}
		for (int i = Ucorner2.second - 1; i >= secondBlock.second + 2; i--)
		{
			Controller::gotoXY(secondBlock.first, i);
			putchar(32);
		}
		Controller::gotoXY(secondBlock.first, secondBlock.second + 1);
		putchar(32);
		Controller::gotoXY(left + 2, top + size*4);
		for (int i = 1; i < size * 8; i++)
			putchar(205);
		return;
	}
}
