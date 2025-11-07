number_of_test=$1

for i in $(seq 0 $(($number_of_test - 1))); do
	text=$(printf "%0${#number_of_test}d" $i)

	for j in {0..99}; do
		instruction=$(shuf -n 1 -e 'i' 'd')
		number=$(shuf -i 1-25 -n1)
		echo "$instruction $number" >> "test_input"
	done

	./test_exe < test_input > test_output_${text} 2>&1

	rm -f test_input

	grep "(-2)" test_output_${text}

	if [ $? -eq 1 ]; then
		grep "(2)" test_output_${text}
		if [ $? -eq 1 ]; then
			echo "Test ${text}: Passed"
		else
			echo "Test ${text}: Failed"
		fi
	else
		echo "Test ${text}: Failed"
	fi
done
