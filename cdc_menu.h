#include <cstdlib>

using namespace std;

void Draw_menu();
void Draw_line_advanced(int line_number);
void Get_longest_line();
string Get_whitespace_string(int line_length);
void Read_menu();
string Read_variable(string path);
void Write_file(string path, string input);
void User_input();
void Draw_alphabetic_menu(int menu_item_current);
string User_input_line(int menu_item_current);
void Call_script(int menu_item);
void Read_directory(string path);
int Find_last_index(string text, char value);