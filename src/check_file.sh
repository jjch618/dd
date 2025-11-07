#!/bin/bash

check_file() {
	print_title "SEARCHING SOURCE FILES"

	sleep 2

	OLD_IFS=$IFS
	IFS=$'\n'

	SOURCES=($(find "${PATH_TEST}"/student/${PROJECT} -type f -name '*.c' -o -name '*.cpp' | awk -F/ '{print $NF}' | sort))
	printf "${COLOR_TITLE}Found source files:\n${DEFAULT}"
	for source in "${SOURCES[@]}"; do
		printf "${COLOR_STUDENT}${source}${DEFAULT}\n"
	done
	printf "\n\n"

	IFS=$OLD_IFS
}