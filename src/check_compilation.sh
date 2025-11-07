#!/bin/bash

check_compilation() {
	print_title "CHECKING COMPILATION"

	sleep 2

	text="= Checking Compilation "
	printf "${text}" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
	printf "%.s=" $(seq 1 $(( 80 - ${#text} ))) >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
	printf "\n" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
	printf "%-16s%-16s\n" "STUDENT_ID" "RESULT" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought

	COMPILED=()
	for SOURCE in "${SOURCES[@]}"; do
		STUDENT_ID=$(echo "${SOURCE}" | cut -d'_' -f1)
		STUDENT_NAME=$(echo "${SOURCE}" | cut -d'_' -f2)
		DEEPTHOUGHT="${PATH_DEEPTHOUGHT}"/${PROJECT}/${STUDENT_ID}_${STUDENT_NAME}_deepthought
		EXECUTABLE="${PATH_TEST}"/student/${PROJECT}/${STUDENT_ID}_${STUDENT_NAME}_exe
		if [ "${SOURCE##*.}" == "c" ]; then
			COMPILER="clang"
		else
			COMPILER="clang++"
		fi
		
		if [ -e "${EXECUTABLE}" ]; then
			rm -f "${EXECUTABLE}"
		fi

		text="= Compiling "
		printf "\n${text}" >> "${DEEPTHOUGHT}"
		printf "%.s=" $(seq 1 $(( 80 - ${#text} ))) >> "${DEEPTHOUGHT}"

		printf "\n$> "${COMPILER}" -Wall -Wextra -Werror -g3 "${SOURCE}" -o "${STUDENT_ID}"_exe\n" >> "${DEEPTHOUGHT}"
		${COMPILER} -Wall -Wextra -Werror -g3 "${PATH_TEST}"/student/${PROJECT}/"${SOURCE}" -o "${EXECUTABLE}" 2>> "${DEEPTHOUGHT}"
		FLAG=$?
		printf "\n" >> "${DEEPTHOUGHT}"

		printf "\n$> "${COMPILER}" -g3 "${SOURCE}" -o "${STUDENT_ID}"_exe\n" >> "${DEEPTHOUGHT}"
		${COMPILER} -g3 "${PATH_TEST}"/student/${PROJECT}/"${SOURCE}" -o ${EXECUTABLE} 2>> "${DEEPTHOUGHT}"
		retvalue=$?
		printf "\n" >> "${DEEPTHOUGHT}"
		
		if [ $retvalue -ne 0 ] && [ "${COMPILER}" == "clang++" ]; then
			${COMPILER} -g3 -std=c++17 "${PATH_TEST}"/student/${PROJECT}/"${SOURCE}" -o ${EXECUTABLE}
			retvalue=$?
			if [ $retvalue -ne 0 ]; then
				${COMPILER} -g3 -std=c++20 "${PATH_TEST}"/student/${PROJECT}/"${SOURCE}" -o ${EXECUTABLE}
				retvalue=$?
			fi
		fi
		
		printf "%-16s" "${STUDENT_ID}" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
		if [ $retvalue -eq 0 ]; then
			COMPILED+=("${SOURCE}")
			printf "%-16s\n" "success" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
		else
			printf "%-16s\n" "failure" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
		fi
		
		printf "%-16s%-16s%-16s%-16s%-16s\n" "STUDENT_ID" "COMPILER" "RESULT" "-Wall -Wextra -Werror"
        printf "${COLOR_OK}%-16s%-16s${DEFAULT}" "${STUDENT_ID}" "${COMPILER}"
        if [ $retvalue -eq 0 ]; then
			printf "${COLOR_OK}%-16s${DEFAULT}" "success"
        else
			printf "${COLOR_FAIL}%-16s${DEFAULT}" "failure"
        fi
        if [ $FLAG -eq 0 ]; then
			printf "${COLOR_OK}%-16s${DEFAULT}" "success"
        else
			printf "${COLOR_FAIL}%-16s${DEFAULT}" "failure"
        fi

        printf "\n\n"

		sleep 0.5
	done

	SOURCES=("${COMPILED[@]}")
	
	printf "\n" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought

	make -s -C "${PATH_TEST}"/test ${PROJECT}

	return $retvalue
}