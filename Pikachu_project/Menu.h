﻿//#pragma once
#ifndef MENU
#define MENU
#include <iostream>
#include <string>
#include <unordered_map>
#include "Controller.h"
#include "Game.h"
#include <random>
#include <fstream>

#define _EASY 4
#define _MEDIUM 6

using namespace std;

struct Menu
{	
	static int current_option;								//Lựa chọn hiện tại
	const static string options[8];							//Tên các lựa chọn
	static bool check_continue_game;
	static void mainScreen();								//Màn hình chính
	static void mainMenu();									//Menu
	static void playMenu();									//Menu play
	static void continueGame();
	static void continueScreen();
	static void leaderBoard();								//Màn hình load file leaderboard
	static void helpScreen();								//Màn hình hướng dẫn
	static void exitScreen();								//Màn hình thoát
	static void playEasy();									//Chế độ easy
	static void playMedium();								//Chế độ medium
	static void playHard();								//Chế độ medium
	static void selectSizeBoard(int &);						//select the size of board 
	static void printLogo();								//In ra logo
	static void goBack();									//Trở về menu
	static void printOptionsBoard();						//In ra các bảng lựa chọn
	static void printRectangle(int, int, int, int);			//Vẽ hình chữ nhật
	static void changeOption(bool);					//Thay đổi lựa chọn
};

struct Player {
	string playerName;
	string playerID;
	string className;
	string mode;
	int score = 0;
	int interval;
};

#endif