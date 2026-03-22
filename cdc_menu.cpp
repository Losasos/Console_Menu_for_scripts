#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include ".\cdc_menu.h"

//version 3.3
#define MAIN_DESCRIPTION "CDC Universal Menu v3.4"
#define CLOSURE_LINE "x|--------------------|||"

// size defines
#define MAX_LINE 40
#define MIN_LINE_LENGTH 10 // (Esc "Return"/"Exit  ")
#define MAX_COLOMN 6
#define MAX_MENU_DEPTH 20
#define MAX_ALPH_LETTERS 24
#define NOT_USEFUL_LINES 7

// Paths
#define MAIN_MENU_PATH "main.menu"
#define MENU_DIR "menu\\"
#define VAR_DIR "conf\\"
#define DATA_DIR "data\\"
#define SCRIPT_DIR "tools\\"
#define STARTING_PATH ".\\"

// Line Scheme Items
#define S_TYPE 0
#define S_DESCR 1
#define S_PATH 2
#define S_EXTRA 3
#define S_PARG 4
#define S_ARG 5

// Menu Item Typs
#define M_TITLE 't'
#define M_HEADER 'h'
#define M_NORMAL 'n'
#define M_INPUT 'i'
#define M_ALPH 'a'
#define M_TEXT 'x'
#define M_MENU 'm'
#define M_FILE 'f'
#define M_DISPLAY 'd'

// special symbols
#define SEPARATOR '|'
#define COMMENT_SIGN '#'

// Extra formating length for some Menu Items
#define EXTRA_LENGTH_NORMAL 2
#define EXTRA_LENGTH_HEADER 4
#define EXTRA_LENGTH_FILES 3

// color escape sequences formating
#define COLOR_STANDARD "\033[0m"
#define COLOR_TITE "\033[97;44m"
#define COLOR_HEADER "\033[30;103m"
#define COLOR_TEXT "\033[40;107m"
#define COLOR_NUMBER "\033[91;107m"
#define COLOR_VAR "\033[94;107m"

using namespace std;

// variables are declared here
char exit_menu = 0;
string menu_paths[MAX_MENU_DEPTH];
int menu_current = 0;

string file_data[MAX_LINE]; // structure file ordered as a 1d array
int file_data_length;       // length of  file_data_matrix or file_data arrays
string file_data_matrix[MAX_LINE][MAX_COLOMN];   // structure file ordered as a 2d array
int longest_line;           // conteins length of a longest line used to print lines of the same length
int line_lengths[MAX_LINE]; // conteins line lengths used to print lines of the same length
char odd_line;              // depending on it is 0 or 1 wthe menuitem line color will differ betwing white and light grey
int menu_item_number;       // is the last number in menu_items 
int menu_items[MAX_LINE];   // contains number of a line in a strufcture file ordered to te right order number from 1 to 9
char menu_item_char[MAX_LINE];

string available_file_names[MAX_ALPH_LETTERS];
int available_file_number = 0;

// main menu drawing function
void Draw_menu()
{
	Get_longest_line();
	odd_line = 0;
	menu_item_number = 0;
	cout << "\n";
	
	for(int i = 0; i < file_data_length; i++)
	{
		switch(file_data_matrix[i][S_TYPE][0])
		{
			case M_TITLE:
				printf("%s  %s%s %s\r\n", COLOR_TITE, file_data_matrix[i][S_DESCR].c_str(), Get_whitespace_string(line_lengths[i]).c_str(), COLOR_STANDARD);
				break;
			case M_HEADER:
				printf("%s %s * %s * %s%s %s\r\n", COLOR_TEXT, COLOR_HEADER, file_data_matrix[i][S_DESCR].c_str(), COLOR_TEXT, Get_whitespace_string(line_lengths[i]+1).c_str(), COLOR_STANDARD);
				break;
			case M_NORMAL:
				Draw_line_advanced(i);
				break;
			case M_INPUT:
				Draw_line_advanced(i);
				break;
			case M_ALPH:
				Draw_line_advanced(i);
				break;
			case M_FILE:
				Draw_line_advanced(i);
				break;
			case M_MENU:
				Draw_line_advanced(i);
				break;
			case M_TEXT:
				printf("%s %s%s  %s\r\n", COLOR_TEXT, file_data_matrix[i][S_DESCR].c_str(), Get_whitespace_string(line_lengths[i]).c_str(), COLOR_STANDARD);
				break;
			case M_DISPLAY:
				string tmp = "[\033[94m" + file_data_matrix[i][S_PARG] + "\033[30m] ";
				if(file_data_matrix[i][S_DESCR].length() != 0)
				{
					tmp = " " + tmp + "\b";
				}
				printf("\033[30;107m    %s%s%s %s\r\n", file_data_matrix[i][S_DESCR].c_str(), tmp.c_str(), Get_whitespace_string(line_lengths[i]).c_str(), COLOR_STANDARD);
				break;
		}
	}
	string tmp = "Return";
	if(menu_paths[menu_current] == MAIN_MENU_PATH)
	{
		tmp = "Exit  ";
	}
	printf("%s  Esc \033[30m%s%s %s\r\n", COLOR_NUMBER, tmp.c_str(), Get_whitespace_string(MIN_LINE_LENGTH).c_str(), COLOR_STANDARD);
}

// prints advanced menu line types
void Draw_line_advanced(int line_number)
{
	string bg_color;
	menu_items[menu_item_number] = line_number;
	menu_item_char[menu_item_number] = 48 + menu_item_number + 1;
	if(menu_item_number >= 9)
	{
		menu_item_char[menu_item_number] += 7; // 7 is number of charecters between '9' and 'A' in ASCII
	}
	// decides background color depending on the previous used color
	if(odd_line == 0)
	{
		bg_color = "107"; //white
		odd_line = 1;
	}
	else
	{
		bg_color = "47"; //light gray
		odd_line = 0;
	}
	string var_file_data = "";
	if(file_data[line_number] != "")
	{
		var_file_data = " [\033[94m" + file_data_matrix[line_number][S_PARG] + "\033[30m]";
	}
	printf("\033[91;%sm  %c\033[30m %s%s%s %s\r\n", bg_color.c_str(), menu_item_char[menu_item_number], file_data_matrix[line_number][S_DESCR].c_str(), var_file_data.c_str(), Get_whitespace_string(line_lengths[line_number]).c_str(), COLOR_STANDARD);
	
	menu_item_number++;
}

// finds the longest line in a menu for neat appearance
void Get_longest_line()
{	
	longest_line = MIN_LINE_LENGTH;
	int current_line;
	for(int i = 0; i < file_data_length; i++)
	{
		current_line = 0;
		// description length is added
		current_line += file_data_matrix[i][S_DESCR].length();
		
		// clears array for later use
		file_data[i] = "";
		
		// path length is added (if needen)
		if(file_data_matrix[i][S_TYPE][0] == M_FILE || file_data_matrix[i][S_TYPE][0] == M_DISPLAY)
		{
			// saves obtained data to array to impruve code execution speed on later steps
			file_data_matrix[i][S_PARG] = Read_variable(file_data_matrix[i][S_PATH]);
			current_line += file_data_matrix[i][S_PARG].length();
			current_line += EXTRA_LENGTH_FILES;
			file_data[i] = "true";
		}
		// argument length is added (if needen)
		else if(file_data_matrix[i][S_PARG] != "")
		{
			//file_data[i] = file_data_matrix[i][S_PARG].substr(1,file_data_matrix[i][S_PARG].length()-1);
			file_data_matrix[i][S_PARG] = Read_variable(file_data_matrix[i][S_PARG]);
			current_line += file_data_matrix[i][S_PARG].length();
			current_line += EXTRA_LENGTH_FILES;
			file_data[i] = "true";
		}
		
		if(file_data_matrix[i][S_TYPE][0] == M_HEADER)
		{
			current_line += EXTRA_LENGTH_HEADER; // HEADER lextra length
		}
		
		// menu-item length is added (if needen)
		if(file_data_matrix[i][S_TYPE][0] == M_NORMAL || file_data_matrix[i][S_TYPE][0] == M_INPUT || file_data_matrix[i][S_TYPE][0] == M_ALPH || file_data_matrix[i][S_TYPE][0] == M_FILE || file_data_matrix[i][S_TYPE][0] == M_MENU || file_data_matrix[i][S_TYPE][0] == M_DISPLAY)
		{
			current_line += EXTRA_LENGTH_NORMAL;
		}
		
		if(longest_line < current_line)
		{
			longest_line = current_line;
		}
		line_lengths[i] = current_line;		
	}
}

// returns a whitespace string of a fitting length
// used to make menu neat appearance
string Get_whitespace_string(int line_length)
{
	string whitespace_string = "";
	for(int i = line_length; i < longest_line; i++)
	{
		whitespace_string += " ";
	}
	return whitespace_string;
}

// reads menu file and sorts its data in 2d array for later use
void Read_menu()
{
	string line;
    file_data_length = 0;
	int colomn;
	// reads data in 1d array line by line
    ifstream ReadFile(string(STARTING_PATH) + string(MENU_DIR) + menu_paths[menu_current]);
    while(getline(ReadFile, line)) 
    {
		// skips several types of line in a menu file
		if(line[0] == COMMENT_SIGN || line == "")
		{
			continue;
		}
		file_data[file_data_length] = line;
		file_data_length++;
    }
    ReadFile.close();
	
	//adds a closure line
	file_data[file_data_length] = CLOSURE_LINE;
	file_data_length++;
    
	// rewrites data from 1d array into 2d array
    for(int i = 0; i < file_data_length; i++)
    {
		line = "";
		colomn = 0;
		for(int j = 0; j < file_data[i].length(); j++)
		{
			// devides parts where separator is to be found
			if(file_data[i][j] == SEPARATOR)
			{
				file_data_matrix[i][colomn] = line;
				colomn++;
				line = "";
				continue;
			}
			line += file_data[i][j];
		}
		file_data_matrix[i][colomn] = line;
    }
}

// used to read variables
string Read_variable(string path)
{
	string line;
	ifstream ReadFile(string(STARTING_PATH) + string(VAR_DIR) + path);
	getline(ReadFile, line);
	ReadFile.close();
	return line;
}

void Write_file(string path, string input)
{
	path = string(STARTING_PATH) + string(VAR_DIR) + path;
	ofstream File(path);
	File << input.c_str();
	File.close();
}



void User_input()
{
	char choice_made = 0;
	char input_char;
	int input_int;
	int menu_item_current;
	string temp_cmd;

	cout << "Select Item: ";
	// checks user input
	while(choice_made == 0)
	{
		input_char = getch();
		if(input_char == 27 && menu_current == 0) // 27 Escape Charecter and it is main menu
		{
			exit_menu = 1;
			choice_made = 1;
			return;
		}
		else if(input_char == 27) // 27 Escape Charecter
		{
			menu_current--;
			printf("\033[91mEsc%s\r\n", COLOR_STANDARD);
			cout << "\n";
			choice_made = 1;
			return;
		}
		else if(input_char >= 49 && input_char <= 48 + menu_item_number && menu_item_number > 0 && input_char < 58) // 48 is '0' char
		{
			// extra -1 to use as array index more easiely as the smallest input nummber is 1 not 0
			input_int = input_char - 48 - 1;
			choice_made = 1;
			break;
		}
		else if(input_char >= 65 && input_char <= 90 && input_char <= 64 + menu_item_number - 9)
		{
			// extra -1 to use as array index more easiely as the smallest input nummber is 1 not 0
			// extra -7 is the differance between uppercase 'A' and '9' char so letters can continue nummbers
			input_int = input_char - 48 - 7 - 1;
			choice_made = 1;
			break;
		}
		else if(input_char >= 97 && input_char <= 122 && input_char <= 96 + menu_item_number - 9)
		{
			// extra -1 to use as array index more easiely as the smallest input nummber is 1 not 0
			// extra -39 is the differance between lowercase 'a' and '9' char so letters can continue nummbers
			input_int = input_char - 48 - 39 - 1;
			input_char -= 32;
			choice_made = 1;
			break;
		}
	}
	
	// prints out user choice
	printf("\033[91m%c%s\r\n", input_char, COLOR_STANDARD);
	cout << "\n";
	
	menu_item_current = menu_items[input_int];
	// checks which type of menu item is chosen
	switch(file_data_matrix[menu_item_current][S_TYPE][0])
	{
		case M_NORMAL:
			Call_script(menu_item_current);
			return;
		case M_INPUT:
			file_data_matrix[menu_item_current][S_PARG] = User_input_line(menu_item_current) + " " + file_data_matrix[menu_item_current][S_PARG];
			Call_script(menu_item_current);
			return;
		case M_ALPH:
			Draw_alphabetic_menu(menu_item_current);
			return;
		case M_MENU:
			menu_current++;
			menu_paths[menu_current] = file_data_matrix[menu_item_current][S_PATH];
			if (file_data_matrix[menu_item_current][S_EXTRA] != "")
			{
				temp_cmd = string(STARTING_PATH) + string(SCRIPT_DIR) + file_data_matrix[menu_item_current][S_EXTRA];
				system(temp_cmd.c_str());
			}
			return;
		case M_FILE:
			Write_file(file_data_matrix[menu_item_current][S_PATH], User_input_line(menu_item_current));
			return;
	}
}

void Read_directory(string path)
{
	available_file_number = 0;
	string file_line;
	string gotodir = ".\\"+ string(DATA_DIR) + path;
	string command = "dir /B /A-D " + gotodir + "> .\\conf\\dir.tmp";
	system(command.c_str());
	ifstream File(".\\conf\\dir.tmp");

	while(getline(File, file_line))
	{
		available_file_names[available_file_number] = file_line;
		available_file_number++;
	}
}

// returns index of last find charecter in a string
// returns -1 if nothing found
int Find_last_index(string text, char value)
{
	for(int i=text.length()-1; i >= 0; i--)
	{
		if(text[i]==value)
		{
			return i;
		}
	}
	return -1;
}

// draws automatic scaleble menu
void Draw_alphabetic_menu(int menu_item_current)
{
	char choice_made = 0;
	char input_char;
	odd_line = 0;
	string bg_color;
	
	// reads filenames from directory
	Read_directory(file_data_matrix[menu_item_current][S_EXTRA]);
	
	// finds new longets line
	longest_line = line_lengths[menu_item_current];
	for(int i=0; i<available_file_number; i++)
	{
		if(available_file_names[i].length() + EXTRA_LENGTH_NORMAL > longest_line)
		{
			longest_line = available_file_names[i].length() + EXTRA_LENGTH_NORMAL;
		}
	}
	
	// draws menu
	printf("%s  %s%s %s\r\n", COLOR_TITE, file_data_matrix[menu_item_current][S_DESCR].c_str(), Get_whitespace_string(line_lengths[menu_item_current] - EXTRA_LENGTH_NORMAL).c_str(), COLOR_STANDARD);
	for(int i=0; i<available_file_number; i++)
	{
		if(odd_line == 0)
		{
			bg_color = "\033[107m"; //white
			odd_line = 1;
		}
		else
		{
			bg_color = "\033[47m"; //light gray
			odd_line = 0;
		}
		printf("%s%s  %c\033[30m%s %s%s %s\r\n", COLOR_NUMBER, bg_color.c_str(), 97+i, bg_color.c_str(), available_file_names[i].c_str(), Get_whitespace_string(available_file_names[i].length() + EXTRA_LENGTH_NORMAL).c_str(), COLOR_STANDARD); // 97 is 'a'(lovercase) in ASCII
	}
	printf("%s %s%s %s\r\n", COLOR_TEXT, "------------", Get_whitespace_string(MIN_LINE_LENGTH + 1).c_str(), COLOR_STANDARD);
	printf("%s  Esc \033[30mReturn%s %s\r\n", COLOR_NUMBER, Get_whitespace_string(MIN_LINE_LENGTH).c_str(), COLOR_STANDARD);
	
	cout << "Select Item: ";
	
	// checks user input
	while(choice_made == 0)
	{
		input_char = getch();
		if(input_char == 27) // 27 Escape char
		{
			choice_made = 1;
			printf("\033[91mEsc%s\r\n", COLOR_STANDARD);
			return;
		}
		// -1 is requared becouse we need to add 0 if there is a single file available
		else if(input_char >= 97 && input_char <= 97 + available_file_number -1 && available_file_number > 0)
		{
			choice_made = 1;
			break;
		}
		// -1 is requared becouse we need to add 0 if there is a single file available
		else if(input_char >= 65 && input_char <= 65 + available_file_number -1 && available_file_number > 0)
		{
			input_char += 32;
			choice_made = 1;
			break;
		}
	}
	printf("\033[91m%c%s\r\n", input_char, COLOR_STANDARD);
	cout << "\n";
	// changes file value according to user choice
	Write_file(file_data_matrix[menu_item_current][S_PATH], available_file_names[input_char-97]);
}

// input function for menu items with user input
string User_input_line(int menu_item_current)
{
	string input_string;
	cout << file_data_matrix[menu_item_current][S_EXTRA] << ": \033[94m";
	getline(cin, input_string);
	cout << COLOR_STANDARD <<"\n";
	return input_string;
}

// finds out number of avaluable arguments and calls the script with all the posible args
void Call_script(int menu_item)
{
	string script_name = string(STARTING_PATH) + string(SCRIPT_DIR) + file_data_matrix[menu_item][S_PATH];
	// arguments
	script_name += " " + file_data_matrix[menu_item][S_PARG] + " " + file_data_matrix[menu_item][S_ARG];
	// calls script
	system(script_name.c_str());
}

// main loop
int main(int argc, char *argv[])
{
	system(".\\tools\\init.cmd");
	printf("\033[90m=======================\r\n");
	system("chcp 65001");
	printf("%s\r\n", MAIN_DESCRIPTION);
	printf("=======================\033[40m\r\n");
	menu_paths[menu_current] = MAIN_MENU_PATH;
    while(exit_menu == 0)
    {
		Read_menu();
		Draw_menu();
		User_input();
    }
    return EXIT_SUCCESS;
}