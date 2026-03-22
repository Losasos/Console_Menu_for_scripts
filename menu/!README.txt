[file.menu]

menu file is a structure of any menu (like an html) which defines:
description text, points order, paths to other menus or scripts and some other parameters
so the code itself consists only of fuctions witch read menu files,
and based on that draw menus in console

every line in menu file represents a line in menu drawn in console
scheme - 0.type|1.description|2.path|3.extra|4.path arg|5.args

0.type
	't' - title (description to menu)
	'h' - header (marker to order menu items into groups)
	'm' - enter a path to another menu in point #2
              if EXTRA is a script, it will be executed before entering the menu
	'n' - calls a script
	'i' - cals a script with user input as script argument 
	      input will be the very first argument on calling a script

	'a' - menu point alphabetic (scale automatic dependent on amount of files to be found)
      	      the path to the variable file to be replaced in point #2 (path)
	      diractory to the files should be entered in point #3 (extra) 

	'f' - path to a variable file (the curren state of a variable will be shown in menu)
	'd' - displays variable file content without a script (description works as usual)

	'x' - plain text (a posibility to enter some text no color change orenything else)
	'#' - commentary whis line will be complitly ignored
	    - empy lines will be complytly ignored

1.description
	- description to menu item is entered here it is the main text to be shown in a menue

2.path
	- a path entered here if requared

3.extra
	- is used for several options
	- if type is input 'i' then extra is used as a an input description for user (a colon and a white space are entered automaticly ": ")
	- if type is alphabetic 'a' then a directory name to search for availble files is ment to be entered here
        - is used with 'm' command

4.path argument
	- it is an argument which is to be found in a config files so a oath should be entered here
	- the curren state of a variable will be shown in menu

5.arguments
	- enter arguments one by one separaten by ' ' white space
	- separator between #4 and #5 is optional if no standard argument is entered