#! /bin/bash

BASEDIR=$(dirname $(readlink -f $0))
cd -- $BASEDIR

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RESET='\033[0m'

function tester() {
	cmd=$1
	cmd_expected=$1
	op_expected=$2
	num_of_nodes_expected=$3

	echo -e "${GREEN}Running ./minishell $cmd${RESET}"
	echo "$cmd" | valgrind --leak-check=full ../delivery/minishell &> $OUT_FILE
	if [[ $? -ne 0 ]]; then
		echo -e "${RED}Test failed: $cmd${RESET}"
		exit 1
	else
		echo -e "${GREEN}execution passed: $cmd${RESET}"
	fi

	# Check leaks
	leak_def_lost=$(cat $OUT_FILE | grep "definitely lost: " | sed 's/.*definitely lost: //')
	if [[ "$leak_def_lost" != "0 bytes in 0 blocks" ]]; then
		echo -e "${RED}LEAK detected!!!${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		exit 1
	fi;
	leak_indir_lost=$(cat $OUT_FILE | grep "indirectly lost: " | sed 's/.*indirectly lost: //')
	if [[ "$leak_indir_lost" != "0 bytes in 0 blocks" ]]; then
		echo -e "${RED}LEAK detected!!!${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		exit 1
	fi;
	leak_poss_lost=$(cat $OUT_FILE | grep "possibly lost: " | sed 's/.*possibly lost: //')
	if [[ "$leak_poss_lost" != "0 bytes in 0 blocks" ]]; then
		echo -e "${RED}LEAK detected!!!${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		exit 1
	fi;
	echo -e "${GREEN}No leaks detected${RESET}"

	cmd_received=$(cat $OUT_FILE | grep "Command received: " | sed 's/Command received: //')
	if [[ $cmd_received == "$cmd" ]]; then
		echo -e "${GREEN}Command received correctly${RESET}"
	else
		echo -e "${RED}Command received: $cmd_received${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		exit 1
	fi

	valid_operators=('&&' '||' '|' '<' '>' '<<' '>>')
	if [[ "${array[@]}" =~ 'op_expected' ]]; then
		found_op=$(cat $OUT_FILE | grep "Nodes: $op_expected")
		if [[ ! -z $found_op ]]; then
			echo -e "${GREEN}Operators detected correctly${RESET}"
		else
			echo -e "${RED}Error to detect operators${RESET}"
			echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
			echo -e "${BLUE}Output is:${RESET}"
			cat $OUT_FILE
			exit 1
		fi
	fi

	nodes=$(cat $OUT_FILE | grep "Nodes" | wc -l)
	if [[ $nodes -eq $num_of_nodes_expected ]]; then
		echo -e "${GREEN}Found $nodes nodes in the output.${RESET}"
	else
		echo -e "${RED}No nodes found in the output.${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		exit 1
	fi

	echo ""
}

make -C ../delivery/ clean
CFLAGS="-DTEST" make -C ../delivery/ -j 16 all
if [[ $? -ne 0 ]]; then
	echo -e "${RED}Error to compile the project${RESET}"
	exit 1
fi

OUT_FILE="/tmp/my_minishell"
# tester cmd found_op num_of_nodes
tester   "oi hi"                  0      2
tester   "oi oi oi"               0      3
tester   "1 2 3 4 5 6 7 8 9 10"   0      10

# Testing quotes " usage
tester 'oi "hi"'             0      2
tester 'oi "hi" oi'          0      3
tester 'oi "hi oioioi" oi'   0      3
tester 'oi "hi oioioi"oi'    0      2

# Testing quotes ' usage
tester "oi 'hi'"             0      2
tester "oi 'hi' oi"          0      3
tester "oi 'hi oioioi' oi"   0      3
tester "oi 'hi oioioi'oi"    0      2

# Testing quotes ' and " usage
tester "oi 'hi' \"oi\""      0      3
tester "oi 'hi'\"oi\""       0      2
tester "oi 'hi \"asas\" oioioi' oi"   0      3

# Testing quotes ' and " usage
tester "oi 'hi' \"oi\""      0      3
tester "oi 'hi'\"oi\""       0      2
tester "oi 'hi \"asas\" oioioi' oi"   0      3

# Testing with operator &&
tester 'oi "&&" oi'         0       3
tester "oi '&&' oi"         0       3
tester "oi && oi"           "&&"    3
tester "oi & oi"            0       3
tester "oi &&& oi"          0       4
tester "oi &&&& oi"         0       4
tester "oi &&&&& oi"        0       5
tester "oi&&oi"             "&&"    3
tester "oioi&&"             0       2
tester "&&oioi"             0       2

# Testing with operator ||
tester 'oi "||" oi'         0       3
tester "oi '||' oi"         0       3
tester "oi || oi"           "||"    3
tester "oi ||| oi"          0       4
tester "oi |||| oi"         0       4
tester "oi ||||| oi"        0       5
tester "oi||oi"             "||"    3
tester "oioi||"             0       2
tester "||oioi"             0       2

# Testing with operator |
tester 'oi "|" oi'         0       3
tester "oi '|' oi"         0       3
tester "oi | oi"           "|"     3
tester "oi|oi"             "|"     3
tester "oioi|"             0       2


make -C ../delivery/ clean > /dev/null
