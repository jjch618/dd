#!/bin/bash

###################################
#     Mandatory configuration     #
###################################

#Edit the next line with the path of your project.
PATH_STUDENT="student"

#If you use a relative path, don't put double quotes.
#For example :
#PATH_STUDENT=~/student			==> Right
#PATH_STUDENT="~/student"		==> Wrong

#If you encounter some problems with a relative path, use an absolute path.
#You can put double quotes with the absolute path.
#For example :
#PATH_STUDENT=/home/user/student	==> Right
#PATH_STUDENT="/home/user/student"	==> Right

###################################
#     Optionnal configuration     #
###################################

#By default, the deepthought file will be generated in the grademe directory.
#Edit the next line with an absolute path if you want to change it.
PATH_DEEPTHOUGHT="${PATH_TEST}/result"

#You can chose color and text effect.
#Colors available :
#	- BLACK
#	- CYAN
#	- GREEN
#	- YELLOW
#	- RED
#	- BLUE
#	- PURPLE
#	- WHITE

#Effects available :
#	- BOLD
#	- UNDERLINE

#For example :
#COLOR_EXAMPLE="${UNDERLINE}${BOLD}${PURPLE}

COLOR_OK="${GREEN}"
COLOR_FAIL="${RED}"
COLOR_WARNING="${YELLOW}"
COLOR_TITLE="${BOLD}${BLUE}"
COLOR_TESTCASE="${CYAN}"
COLOR_STUDENT="${CYAN}"
COLOR_PART="${UNDERLINE}${PURPLE}"
COLOR_TOTAL="${BOLD}${YELLOW}"
COLOR_DEEPTHOUGHT_PATH="${BOLD}${PURPLE}"

#Set Timeout duration in seconds.
TIMEOUT_DURATION=5
