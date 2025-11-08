#!/bin/bash

test_cases() {
	cases=()
	while IFS= read -r file; do
		cases+=("$file")
	done < <(find "${PATH_TEST}"/testcase/${PROJECT} $1 | sort)

	TOTAL=${#cases[@]}
	SUCCESS=0
	i=0
	while [ $i -lt ${#cases[@]} ]; do
		diff_test "${cases[$i]}"
		TEST_RESULT=$? # diff_test의 반환값 (0: OK, 1: KO, 2: OK but Leak)

		# 0(OK) 또는 2(OK but Leak)일 때 SUCCESS 증가
		if [ $TEST_RESULT -eq 0 ]; then
			SUCCESS=$((SUCCESS + 1))
		elif [ $TEST_RESULT -eq 2 ]; then
			SUCCESS=$((SUCCESS + 1))
			LEAK_PENALTY=1 # 릭 페널티 플래그 설정
		fi
		# TEST_RESULT가 1 (KO)이면 SUCCESS는 증가하지 않음

		i=$((i + 1))
	done
}

test_simple_case() {
	test_cases "-type f -name simple*"
	
	if [ $TOTAL -lt 10 ]; then
		TOTAL="0${TOTAL}"
	fi
	if [ $SUCCESS -lt 10 ]; then
		SUCCESS="0${SUCCESS}"
	fi
	printf "%-16s" "[${SUCCESS}/${TOTAL}]" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
	printf "${COLOR_TOTAL}%-16s${DEFAULT}" "[${SUCCESS}/${TOTAL}]"
}

test_random_case() {
	generate_testcases 30
	test_cases "-type f -name random*"
	remove_testcases

	if [ $TOTAL -lt 10 ]; then
		TOTAL="0${TOTAL}"
	fi
	if [ $SUCCESS -lt 10 ]; then
		SUCCESS="0${SUCCESS}"
	fi
	printf "%-16s" "[${SUCCESS}/${TOTAL}]" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
	printf "${COLOR_TOTAL}%-16s${DEFAULT}" "[${SUCCESS}/${TOTAL}]"
}

start_test() {
	print_title "STARTING TESTS"

	sleep 2
	
	text="= STARTING TESTS "
	printf "\n${text}" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
	printf "%.s=" $(seq 1 $(( 80 - ${#text} ))) >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
	
	# [수정] deepthought 파일 헤더에 LEAK 컬럼 추가
	printf "\n%-16s%-16s%-16s%-16s%-16s\n" "STUDENT" "SIMPLE" "RANDOM" "LEAK" "TOTAL" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought

	mkdir -p "${PATH_TEST}"/output/${PROJECT}

	# [수정] 콘솔 출력 헤더에 LEAK 컬럼 추가
	printf "%-16s%-16s%-16s%-16s%-16s\n" "STUDENT" "SIMPLE" "RANDOM" "LEAK" "SCORE"
	for SOURCE in "${SOURCES[@]}"; do
		STUDENT_ID=$(echo "${SOURCE}" | cut -d'_' -f1)
		STUDENT_NAME=$(echo "${SOURCE}" | cut -d'_' -f2)
		EXECUTABLE="${PATH_TEST}"/student/${PROJECT}/${STUDENT_ID}_${STUDENT_NAME}_exe
		DEEPTHOUGHT="${PATH_DEEPTHOUGHT}"/${PROJECT}/${STUDENT_ID}_${STUDENT_NAME}_deepthought

		printf "%-16s" "${STUDENT_ID}" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
		printf "${COLOR_STUDENT}%-16s${DEFAULT}" "${STUDENT_ID}"
		
		SCORE=0
		LEAK_PENALTY=0 # 학생마다 릭 페널티 플래그 초기화
		
		test_simple_case
		SCORE=$((SCORE + 8 * SUCCESS))

		test_random_case
		SCORE=$((SCORE + 2 * SUCCESS))

		# [수정] 릭 페널티 적용 및 LEAK 컬럼 콘솔/파일 출력
		if [ $LEAK_PENALTY -ne 0 ]; then
			# 콘솔 출력 (빨간색 "Leak")
			printf "${COLOR_FAIL}%-16s${DEFAULT}" "Leak"
			# deepthought 파일 출력
			printf "%-16s" "Yes" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
			
		else
			# 콘솔 출력 (파란색 "OK")
			printf "${BLUE}%-16s${DEFAULT}" "OK"
			# deepthought 파일 출력
			printf "%-16s" "No" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
		fi

		printf "%-16s\n" "${SCORE}" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
		printf "${COLOR_TOTAL}%-16s${DEFAULT}\n" "${SCORE}"
	done

	printf "\n"

	return $retvalue
}
# #!/bin/bash

# test_cases() {
# 	cases=()
# 	while IFS= read -r file; do
# 		cases+=("$file")
# 	done < <(find "${PATH_TEST}"/testcase/${PROJECT} $1 | sort)

# 	TOTAL=${#cases[@]}
# 	SUCCESS=0
# 	i=0
# 	while [ $i -lt ${#cases[@]} ]; do
# 		diff_test "${cases[$i]}"
# 		#
# 		TEST_RESULT=$?
# 		# 0(OK) 또는 2(OK but Leak)일 때 SUCCESS 증가
# 		if [ $TEST_RESULT -eq 0 ]; then
# 			SUCCESS=$((SUCCESS + 1))
# 		elif [ $TEST_RESULT -eq 2 ]; then
# 			SUCCESS=$((SUCCESS + 1))
# 			LEAK_PENALTY=1 # 릭 페널티 플래그 설정
# 		fi
# 		# TEST_RESULT가 1 (KO)이면 SUCCESS는 증가하지 않음

# 		# if [ $? -eq 0 ]; then
# 		# 	SUCCESS=$((SUCCESS + 1))
# 		# fi
# 		i=$((i + 1))
# 	done
# }

# test_simple_case() {
# 	test_cases "-type f -name simple*"
	
# 	if [ $TOTAL -lt 10 ]; then
# 		TOTAL="0${TOTAL}"
# 	fi
# 	if [ $SUCCESS -lt 10 ]; then
# 		SUCCESS="0${SUCCESS}"
# 	fi
# 	printf "%-16s" "[${SUCCESS}/${TOTAL}]" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
# 	printf "${COLOR_TOTAL}%-16s${DEFAULT}" "[${SUCCESS}/${TOTAL}]"
# }

# test_random_case() {
# 	generate_testcases 30
# 	test_cases "-type f -name random*"
# 	remove_testcases

# 	if [ $TOTAL -lt 10 ]; then
# 		TOTAL="0${TOTAL}"
# 	fi
# 	if [ $SUCCESS -lt 10 ]; then
# 		SUCCESS="0${SUCCESS}"
# 	fi
# 	printf "%-16s" "[${SUCCESS}/${TOTAL}]" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
# 	printf "${COLOR_TOTAL}%-16s${DEFAULT}" "[${SUCCESS}/${TOTAL}]"
# }

# start_test() {
# 	print_title "STARTING TESTS"

# 	sleep 2
	
# 	text="= STARTING TESTS "
# 	printf "\n${text}" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
# 	printf "%.s=" $(seq 1 $(( 80 - ${#text} ))) >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
# 	printf "\n%-16s%-16s%-16s%-16s\n" "STUDENT" "SIMPLE" "RANDOM" "TOTAL" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought

# 	mkdir -p "${PATH_TEST}"/output/${PROJECT}

# 	printf "%-16s%-16s%-16s%-16s\n" "STUDENT" "SIMPLE" "RANDOM" "SCORE"
# 	for SOURCE in "${SOURCES[@]}"; do
# 		STUDENT_ID=$(echo "${SOURCE}" | cut -d'_' -f1)
# 		STUDENT_NAME=$(echo "${SOURCE}" | cut -d'_' -f2)
# 		EXECUTABLE="${PATH_TEST}"/student/${PROJECT}/${STUDENT_ID}_${STUDENT_NAME}_exe
# 		DEEPTHOUGHT="${PATH_DEEPTHOUGHT}"/${PROJECT}/${STUDENT_ID}_${STUDENT_NAME}_deepthought

# 		printf "%-16s" "${STUDENT_ID}" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
# 		printf "${COLOR_STUDENT}%-16s${DEFAULT}" "${STUDENT_ID}"
		
# 		SCORE=0
# 		#
# 		LEAK_PENALTY=0 # 학생마다 릭 페널티 플래그 초기화
		
# 		test_simple_case
# 		SCORE=$((SCORE + 8 * SUCCESS))

# 		test_random_case
# 		SCORE=$((SCORE + 2 * SUCCESS))

# 		# 릭 페널티 적용
# 		if [ $LEAK_PENALTY -ne 0 ]; then
# 			printf "%-16s" "Yes" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
# 			SCORE=$((SCORE - 20))
# 			# 점수가 0 미만으로 내려가지 않도록 보정
# 			if [ $SCORE -lt 0 ]; then
# 				SCORE=0
# 			fi
# 		else
# 			printf "%-16s" "No" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
# 		fi

# 		printf "%-16s\n" "${SCORE}" >> "${PATH_DEEPTHOUGHT}"/${PROJECT}/deepthought
# 		printf "${COLOR_TOTAL}%-16s${DEFAULT}\n" "${SCORE}"
# 	done

# 	printf "\n"

# 	return $retvalue
# }