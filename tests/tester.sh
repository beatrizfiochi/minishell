#! /bin/bash

BASEDIR=$(dirname $(readlink -f $0))
cd -- $BASEDIR

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
RESET='\033[0m'

function tester() {
	cmd=$1
	cmd_expected=$1
	op_expected=$2
	num_of_nodes_expected=$3
	if [ $# -eq 4 ]; then
		declare -n node_cmd="$4" # items_ref is now a nameref to the array whose name was passed as $4
	else
		node_cmd=""
	fi
	if [ $# -eq 5 ]; then
		declare -n node_op="$5" # items_ref is now a nameref to the array whose name was passed as $4
	else
		node_op=""
	fi

	echo -e "${GREEN}Running ./minishell $cmd${RESET}"
	rm "$OUT_FILE" || echo -n ""
	echo "$cmd" | valgrind --trace-children=yes --child-silent-after-fork=no --leak-check=full --show-leak-kinds=all --suppressions=../delivery/valgrind-supression --log-file="$OUT_FILE_VALGRIND" ../delivery/minishell &> $OUT_FILE
	if [ ! -f "$OUT_FILE" ]; then
		echo -e "${RED}Test failed: $cmd${RESET}"
		exit 1
	fi

	# Check leaks
	leak_still_reachable=$(cat $OUT_FILE_VALGRIND | grep "still reachable: " | grep -v "0 bytes in 0 blocks")
	if [[ ! -z "$leak_still_reachable" ]]; then
		echo -e "${RED}LEAK detected!!! (still reachable)${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		echo -e "${BLUE}Valgrind output is:${RESET}"
		cat $OUT_FILE_VALGRIND
		exit 1
	fi;
	leak_def_lost=$(cat $OUT_FILE_VALGRIND | grep "definitely lost: " | grep -v "0 bytes in 0 blocks")
	if [[ ! -z "$leak_def_lost" ]]; then
		echo -e "${RED}LEAK detected!!! (definitely lost)${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		echo -e "${BLUE}Valgrind output is:${RESET}"
		cat $OUT_FILE_VALGRIND
		exit 1
	fi;
	leak_indir_lost=$(cat $OUT_FILE_VALGRIND | grep "indirectly lost: " | grep -v "0 bytes in 0 blocks")
	if [[ ! -z "$leak_indir_lost" ]]; then
		echo -e "${RED}LEAK detected!!! (indirectly lost)${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		echo -e "${BLUE}Valgrind output is:${RESET}"
		cat $OUT_FILE_VALGRIND
		exit 1
	fi;
	leak_poss_lost=$(cat $OUT_FILE_VALGRIND | grep "possibly lost: " | grep -v "0 bytes in 0 blocks")
	if [[ ! -z "$leak_poss_lost" ]]; then
		echo -e "${RED}LEAK detected!!! (possibly lost)${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		echo -e "${BLUE}Valgrind output is:${RESET}"
		cat $OUT_FILE_VALGRIND
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
	if [[ "${valid_operators[@]}" =~ $op_expected ]]; then
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

	if [ ! -z "$node_cmd" ] && [ "${#node_cmd[@]}" -gt 0 ]; then
		for item in "${node_cmd[@]}"; do
			# expected print
			# Btree: Leaf content: oi
			# Btree: Operator leaf> OP_CMD
			found=$(cat $OUT_FILE | grep -A 1 "Btree: Leaf content: $item")
			echo "$found" | grep "OP_CMD" > /dev/null
			if [[ $? -ne 0 ]]; then
				echo -e "${RED}Error to find the command $item in the output${RESET}"
				cat $OUT_FILE
				exit 1
			fi
		done
	fi

	if [ ! -z "$node_op" ] && [ "${#node_op[@]}" -gt 0 ]; then
		for item in "${node_op[@]}"; do
			# expected print
			# Btree: Node content: ||
			# Btree: Operator node> OP_OR
			found=$(cat $OUT_FILE | grep -A 1 "Btree: Operator node> $item")
			echo "$found" | grep -E "OP_OR|OP_AND|OP_PIPE" > /dev/null
			if [[ $? -ne 0 ]]; then
				echo -e "${RED}Error to find the operator $item in the output${RESET}"
				cat $OUT_FILE
				exit 1
			fi
		done
	fi

	echo ""
}

function tester_grep() {
	cmd=$1
	text=$2

	echo -e "${GREEN}Running ./minishell $cmd${RESET}"
	rm "$OUT_FILE" || echo -n ""
	echo "$cmd" | valgrind --trace-children=yes --child-silent-after-fork=no --leak-check=full --show-leak-kinds=all --suppressions=../delivery/valgrind-supression --log-file="$OUT_FILE_VALGRIND" ../delivery/minishell &> $OUT_FILE
	if [ ! -f "$OUT_FILE" ]; then
		echo -e "${RED}Test failed: $cmd${RESET}"
		exit 1
	fi

	text_found=$(cat $OUT_FILE | grep "$text")
	if [[ $text_found == "$text" ]]; then
		echo -e "${GREEN}Success${RESET}"
	else
		echo -e "${RED}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		exit 1
	fi
}

function tester_with_real() {
	cmd=$1

	# Compared with the other tester function this one removed the "--trace-children=yes" argument of the valgrind command.
	# It was somehow changing the path of teh command causing some prints like
	# /usr/bin/ls: cannot access 'aaa': No such file or directory
	# Instead of
	# ls: cannot access 'aaa': No such file or directory
	# Which was causing issues to compare with real bash
	echo "$cmd" | valgrind --child-silent-after-fork=yes --leak-check=full --log-file="$OUT_FILE_VALGRIND" ../delivery/minishell &> $OUT_FILE
	EXIT_STATUS=$?
	if [ ! -f "$OUT_FILE" ]; then
		echo -e "${RED}Test failed: $cmd${RESET}"
		exit 1
	fi

	#TODO: Since we are using readline on non-interactive mode, we need to remove the first line of the output. readline is always printing the received command.
	tail -n +2 ${OUT_FILE} > ${OUT_FILE}_2

	echo "$cmd" | bash &> $OUT_REAL_FILE
	REAL_EXIT_STATUS=$?

	diff "${OUT_FILE}_2" "$OUT_REAL_FILE"
	if [ $? -ne 0 ]; then
		echo -e "${RED}Error: $cmd${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output of minishell is:${RESET}"
		cat ${OUT_FILE}_2
		echo -e "${BLUE}Output from bash is:${RESET}"
		cat $OUT_REAL_FILE
		exit 1
	fi


	if [[ "$EXIT_STATUS" != "$REAL_EXIT_STATUS" ]]; then
		echo -e "${RED}Error different error status presented: $cmd${RESET}"
		echo "minishell exit status $EXIT_STATUS"
		echo "bash      exit status $REAL_EXIT_STATUS"
		exit 1
	fi

	echo -e "${GREEN}Test comparing with real bash passed: $cmd${RESET}"

	rm $OUT_FILE
	rm ${OUT_FILE}_2
	rm $OUT_REAL_FILE

	echo ""
}

OUT_FILE="/tmp/my_minishell"
OUT_REAL_FILE="/tmp/from_bash"
OUT_FILE_VALGRIND="/tmp/my_minishell_valgrind"

echo "################ Basic tests (parser and binary tree) ################"
make -C ../delivery/ clean &> /dev/null
make -C ../delivery/ -j 16 debug &> /dev/null
if [[ $? -ne 0 ]]; then
	echo -e "${RED}Error to compile the project${RESET}"
	exit 1
fi

# tester  cmd              found_op num_of_nodes   list_of_cmds   list_of_ops
cmds=("oi" "oi hi")
tester   "oi hi"                  0      2           "cmd"
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
cmds=("oi" "oi")
ops=("OP_AND")
tester "oi && oi"           "&&"    3      "cmds"     "ops"
tester "oi & oi"            0       0
tester "oi &&& oi"          0       0
tester "oi &&&& oi"         0       0
tester "oi &&&&& oi"        0       0
cmds=("oi" "oi")
ops=("OP_AND")
tester "oi&&oi"             "&&"    3      "cmds"     "ops"
tester "oioi&&"             0       2
tester "&&oioi"             0       2

# Testing with operator ||
tester 'oi "||" oi'         0       3
tester "oi '||' oi"         0       3
cmds=("oi" "oi")
ops=("OP_OR")
tester "oi || oi"           "||"    3      "cmds"     "ops"
tester "oi ||| oi"          0       0
tester "oi |||| oi"         0       0
tester "oi ||||| oi"        0       0
cmds=("oi" "oi")
ops=("OP_OR")
tester "oi||oi"             "||"    3      "cmds"     "ops"
tester "oioi||"             0       2
tester "||oioi"             0       2

# Testing with operator |
tester 'oi "|" oi'         0       3
tester "oi '|' oi"         0       3
cmds=("oi" "oi")
ops=("OP_PIPE")
tester "oi | oi"           "|"     3      "cmds"     "ops"
cmds=("oi" "oi")
ops=("OP_PIPE")
tester "oi|oi"             "|"     3      "cmds"     "ops"
tester "oioi|"             0       2

# Testing syntax error         cmd              text
echo "################ Testing syntax error ################"
tester_grep             'ls && || pwd'       "syntax error near unexpected token"
tester_grep             'ls && ||pwd'       "syntax error near unexpected token"
tester_grep             'ls&& || pwd'       "syntax error near unexpected token"
tester_grep             'ls&&|| pwd'       "syntax error near unexpected token"
tester_grep             'ls &&||pwd'       "syntax error near unexpected token"
tester_grep             'ls&& ||pwd'       "syntax error near unexpected token"
tester_grep             'ls &&|| pwd'       "syntax error near unexpected token"
tester_grep             'ls &&||& pwd'       "syntax error near unexpected token"
tester_grep             'ls & pwd'       "syntax error near unexpected token"
tester_grep             'ls&pwd'       "syntax error near unexpected token"
tester_grep             'ls &&& pwd'       "syntax error near unexpected token"
tester_grep             'ls&&&pwd'       "syntax error near unexpected token"
tester_grep             'ls&&&&pwd'       "syntax error near unexpected token"
tester_grep             'ls &&&& pwd'       "syntax error near unexpected token"
tester_grep             'ls &&&&& pwd'       "syntax error near unexpected token"
tester_grep             'ls ||| pwd'       "syntax error near unexpected token"
tester_grep             'ls |||| pwd'       "syntax error near unexpected token"
tester_grep             'ls ||||| pwd'       "syntax error near unexpected token"
tester_grep             'ls <<< pwd'       "syntax error near unexpected token"
tester_grep             'ls <<<< pwd'       "syntax error near unexpected token"
tester_grep             'ls <<<<< pwd'       "syntax error near unexpected token"
tester_grep             'ls >>> pwd'       "syntax error near unexpected token"
tester_grep             'ls >>>> pwd'       "syntax error near unexpected token"
tester_grep             'ls >>>>> pwd'       "syntax error near unexpected token"
echo ""

tester_grep             '1=10'       "1=10: No such file or directory"
echo ""

tester_grep             'x=10'       "var_name = x, var_value = 10"
tester_grep             'y=20'       "var_name = y, var_value = 20"
echo ""

echo "################ Comparing with real bash ################"
make -C ../delivery/ clean &> /dev/null
make -C ../delivery/ -j 16 all &> /dev/null
if [[ $? -ne 0 ]]; then
	echo -e "${RED}Error to compile the project${RESET}"
	exit 1
fi

# Test a normal command
tester_with_real "ls"
tester_with_real "echo opa"

# test AND
tester_with_real "ls && pwd"
tester_with_real "ls aaa && pwd"

# test OR
tester_with_real "ls || pwd"
tester_with_real "ls aaa || pwd"

# Test PIPE
tester_with_real "ls | cat -e"
tester_with_real "ls aaa | cat -e"
tester_with_real "ls ../delivery | cat -e"
tester_with_real "ls ../delivery | grep mini | cat -e"
tester_with_real "ls ../delivery | grep mini | cat -e | grep '\.h'"
tester_with_real "ls ../delivery | grep mini | cat -e | grep '\.h' | cat -e"

# Mixed AND and OR
tester_with_real "ls || echo oi || ls ../delivery"
tester_with_real "ls || echo oi && ls ../delivery"
tester_with_real "ls && echo oi && ls ../delivery"
tester_with_real "ls || echo oi || ls ../delivery || grep mini"
tester_with_real "ls || echo oi || ls ../delivery && grep mini"
tester_with_real "ls || echo oi && ls ../delivery && grep mini"
tester_with_real "ls && echo oi && ls ../delivery && grep mini"
tester_with_real "ls && echo oi && ls ../delivery || grep mini"
tester_with_real "ls && echo oi || ls ../delivery || grep mini"
tester_with_real "ls && echo oi || ls ../delivery && grep mini"
tester_with_real "ls || echo oi && ls ../delivery || grep mini"

# Mixed AND, OR and PIPE
#tester_with_real "echo oi && ls ../delivery | grep mini | cat -e"
#tester_with_real "ls || echo oi && ls ../delivery | grep mini | cat -e"

make -C ../delivery/ clean > /dev/null
