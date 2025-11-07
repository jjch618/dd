#!/bin/bash

clean_up() {
	if [ -e "${PATH_TEST}/test/test_exe" ]; then
		make -C "${PATH_TEST}"/test fclean
	fi

	# Deepthought result must not be cleaned
	# find "${PATH_TEST}"/result -type f -name "*deepthought*" -exec rm -f {} \;

	find "${PATH_TEST}"/student -name "*exe*" -exec rm -rf {} \;

	rm -rf "${PATH_TEST}/output"
}

end_test() {
	clean_up

	sleep 3

	printf "A deepthought file has been created in ${UNDERLINE}${COLOR_DEEPTHOUGHT_PATH}\"${PATH_DEEPTHOUGHT}\"\n${DEFAULT}"
}
