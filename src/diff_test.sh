#!/bin/bash

timeout_test() {
	retvalue=0

	if command -v setarch &> /dev/null; then
		setarch $(uname -m) --addr-no-randomize timeout "${TIMEOUT_DURATION}" "${EXECUTABLE}" < "$1" > /dev/null 2>&1
	else
		timeout "${TIMEOUT_DURATION}" "${EXECUTABLE}" < "$1" > /dev/null 2>&1
	fi

	SIG=$?
	if [ $SIG -eq 124 ]; then
		printf "Command './user_exe < $1' got killed by a Timeout\n" >> "${DEEPTHOUGHT}"
		retvalue=1
	fi

	return $retvalue
}

leak_test() {
	retvalue=0

	if command -v setarch &> /dev/null; then
		result=$(setarch $(uname -m) --addr-no-randomize leaks -atExit -- "${EXECUTABLE}" < "$1" 2>&1)
	else
		result=$(leaks -atExit -- "${EXECUTABLE}" < "$1" 2>&1)
	fi
	if echo $result | grep -qv "0 leaks for 0 total leaked bytes"; then
		printf "Command './user_exe < $1' got leaks\n" >> "${DEEPTHOUGHT}"
		retvalue=1
	fi

	return $retvalue
}

diff_test() {
	text="\n= TEST CASE - $(basename "$1") "
	printf "${text}" >> "${DEEPTHOUGHT}"
	printf "%.s=" $(seq 1 $(( 80 - ${#text} ))) >> "${DEEPTHOUGHT}"
	printf "\n" >> "${DEEPTHOUGHT}"
	
	printf "$> ./user_exe < $(basename "$1")\n" >> "${DEEPTHOUGHT}"

	timeout_test "$1"
	if [ $? -eq 1 ]; then
		return 1
	fi

	leak_test "$1"
	LEAK_STATUS=$?
	# if [ $? -eq 1 ]; then
	# 	return 1
	# fi

	"${EXECUTABLE}" < "$1" > "${PATH_TEST}"/output/${PROJECT}/user_output_$(basename "$1") 2>&1
	SIG=$?
	if [ $SIG -eq 134 ]; then
		printf "Command './user_exe < $1' got killed by an Abort\n" >> "${DEEPTHOUGHT}"
		retvalue=1
	elif [ $SIG -eq 138 ]; then
		printf "Command './user_exe < $1' got killed by a Bus error\n" >> "${DEEPTHOUGHT}"
		retvalue=1
	elif [ $SIG -eq 139 ]; then
		printf "Command './user_exe < $1' got killed by a Segmentation fault\n" >> "${DEEPTHOUGHT}"
		retvalue=1
	else
		"${PATH_TEST}"/test/test_exe < "$1" > "${PATH_TEST}"/output/${PROJECT}/test_output_$(basename "$1") 2>&1
		DIFF=$(diff -u "${PATH_TEST}"/output/${PROJECT}/user_output_$(basename "$1") "${PATH_TEST}"/output/${PROJECT}/test_output_$(basename "$1"))
		printf "$> diff -u user_output_$(basename "$1") test_output_$(basename "$1")\n" >> "${DEEPTHOUGHT}"
		if [ "$DIFF" != "" ] || [ ! -e "${PATH_TEST}"/output/${PROJECT}/user_output_$(basename "$1") ]; then
			echo "${DIFF}" | cat -e >> "${DEEPTHOUGHT}"
			printf "\nDiff KO :(\n" >> "${DEEPTHOUGHT}"
			retvalue=1
		else
			printf "\nDiff OK :D\n" >> "${DEEPTHOUGHT}"
			retvalue=0
		fi
	fi

	# if leaks, return 2
	if [ $retvalue -eq 0 ] && [ $LEAK_STATUS -eq 1 ]; then
		retvalue=2
	fi
	
	return $retvalue
}