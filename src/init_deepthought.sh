#!/bin/bash

init_deepthought() {
	mkdir -p "${PATH_DEEPTHOUGHT}"/${PROJECT}
	rm -rf "${PATH_DEEPTHOUGHT}"/${PROJECT}/*
	
	for SOURCE in "${SOURCES[@]}"; do
		STUDENT_ID=$(echo "${SOURCE}" | cut -d'_' -f1)
		STUDENT_NAME=$(echo "${SOURCE}" | cut -d'_' -f2)
		DEEPTHOUGHT="${PATH_DEEPTHOUGHT}"/${PROJECT}/${STUDENT_ID}_${STUDENT_NAME}_deepthought

		if [ -e "${DEEPTHOUGHT}" ]; then
			rm -f "${DEEPTHOUGHT}"
		fi

		text="= Host-specific information "
		printf "${text}" >> "${DEEPTHOUGHT}"
		printf "%.s=" $(seq 1 $(( 80 - ${#text} ))) >> "${DEEPTHOUGHT}"
		printf "\n$> hostname; uname -msr\n" >> "${DEEPTHOUGHT}"
		hostname >> "${DEEPTHOUGHT}"
		uname -msr >> "${DEEPTHOUGHT}"
		printf "\n$> date\n" >> "${DEEPTHOUGHT}"
		date >> "${DEEPTHOUGHT}"
		printf "\n$> clang --version\n" >> "${DEEPTHOUGHT}"
		clang --version >> "${DEEPTHOUGHT}"
		printf "\n$> clang++ --version\n" >> "${DEEPTHOUGHT}"
		clang++ --version >> "${DEEPTHOUGHT}"
	done
}