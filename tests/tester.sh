#! /bin/bash

# echo $SHLVL
# exit 1

BASEDIR=$(dirname $(readlink -f $0))
cd -- $BASEDIR

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
RESET='\033[0m'

function check_valgrind_logs() {
	# Check leaks
	# echo ---------------------------
	# cat $OUT_FILE_VALGRIND
	# echo ---------------------------
	leak_still_reachable=$(cat $OUT_FILE_VALGRIND | grep -a "still reachable: " | grep -a -v "0 bytes in 0 blocks")
	if [[ ! -z "$leak_still_reachable" ]]; then
		echo -e "${RED}LEAK detected!!! (still reachable)${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		echo -e "${BLUE}Valgrind output is:${RESET}"
		cat $OUT_FILE_VALGRIND
		exit 1
	fi;
	leak_def_lost=$(cat $OUT_FILE_VALGRIND | grep -a "definitely lost: " | grep -a -v "0 bytes in 0 blocks")
	if [[ ! -z "$leak_def_lost" ]]; then
		echo -e "${RED}LEAK detected!!! (definitely lost)${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		echo -e "${BLUE}Valgrind output is:${RESET}"
		cat $OUT_FILE_VALGRIND
		exit 1
	fi;
	leak_indir_lost=$(cat $OUT_FILE_VALGRIND | grep -a "indirectly lost: " | grep -a -v "0 bytes in 0 blocks")
	if [[ ! -z "$leak_indir_lost" ]]; then
		echo -e "${RED}LEAK detected!!! (indirectly lost)${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		echo -e "${BLUE}Valgrind output is:${RESET}"
		cat $OUT_FILE_VALGRIND
		exit 1
	fi;
	leak_poss_lost=$(cat $OUT_FILE_VALGRIND | grep -a "possibly lost: " | grep -a -v "0 bytes in 0 blocks")
	if [[ ! -z "$leak_poss_lost" ]]; then
		echo -e "${RED}LEAK detected!!! (possibly lost)${RESET}"
		echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		echo -e "${BLUE}Valgrind output is:${RESET}"
		cat $OUT_FILE_VALGRIND
		exit 1
	fi;
	# echo -e "${GREEN}No leaks detected${RESET}"
}

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
	rm "$OUT_FILE" &> /dev/null || echo -n ""
	echo "$cmd" | valgrind --trace-children=yes --child-silent-after-fork=no --leak-check=full --show-leak-kinds=all --suppressions=$(pwd)/../delivery/valgrind-supression --log-file="$OUT_FILE_VALGRIND" ../delivery/minishell &> $OUT_FILE
	if [ ! -f "$OUT_FILE" ]; then
		echo -e "${RED}Test failed: $cmd${RESET}"
		exit 1
	fi

	# Check leaks
	check_valgrind_logs

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
		found_op=$(cat $OUT_FILE | grep -E "Nodes: $op_expected\$")
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
			found=$(cat $OUT_FILE | grep -E -B 1 "Btree: Leaf content: $item \$")
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
			found=$(cat $OUT_FILE | grep -E -A 1 "Btree: Operator node> $item\$")
			echo "$found" | grep -E "OP_OR|OP_AND|OP_PIPE" > /dev/null
			if [[ $? -ne 0 ]]; then
				echo -e "${RED}Error to find the operator $item in the output${RESET}"
				cat $OUT_FILE
				exit 1
			fi
		done
	else
		found=$(cat $OUT_FILE | grep "Btree: Operator node> ")
		if [ ! -z "$found" ]; then
			echo -e "${RED}Found an opereator when not expecting one${RESET}"
			exit 1
		fi
	fi

	echo ""
}

function tester_grep() {
	cmd=$1
	text=$2

	echo -e "${GREEN}Running ./minishell $cmd${RESET}"
	rm "$OUT_FILE" &> /dev/null || echo -n ""
	echo "$cmd" | valgrind --trace-children=yes --child-silent-after-fork=no --leak-check=full --show-leak-kinds=all --suppressions=$(pwd)/../delivery/valgrind-supression --log-file="$OUT_FILE_VALGRIND" ../delivery/minishell &> $OUT_FILE
	if [ ! -f "$OUT_FILE" ]; then
		echo -e "${RED}Test failed: $cmd${RESET}"
		exit 1
	fi
	check_valgrind_logs

	text_found=$(cat $OUT_FILE | grep "^$text\$")
	if [[ ! -z $text_found ]]; then
		echo -e "${GREEN}Success${RESET}"
	else
		echo -e "${RED}CMD send is: \"$cmd\"${RESET}"
		echo -e "${BLUE}Output is:${RESET}"
		cat $OUT_FILE
		exit 1
	fi
}

function test_signal() {
	echo -n "Testing $2 on command: $1 ... "
	echo "$1" | (../delivery/./minishell &> /dev/null) &
	parent_pid=$!
	# Get the child PID
	#TODO: Using a fixed sleep can make tests brittle and slow; Another option: polling or looping on pgrep until the child PID appears.
	#
	# This sleep is necessary to ensure that the minishell spawn the child before we try to send a signal to it
	sleep 1
	child_pid=$(pgrep -n -P $parent_pid)
	# Ensure child_pid is not empty
	if [[ -z "$child_pid" ]]; then
		echo -e "${RED}Error: No child process found for parent PID $parent_pid${RESET}"
		exit 1
	fi

	# Kill and get the status code of the parent process
	kill -"$2" $child_pid
	if [ $? -ne 0 ]; then
		echo -e "${RED}Error to send signal $2 to PID $child_pid${RESET}"
		exit 1
	fi
	wait $parent_pid 2>/dev/null
	status=$?

	# The code below get the expected status code for that signal
	signal_number=$(kill -l $2)
	exit_code_expected=$((128 + signal_number))
	if [ $status -eq $exit_code_expected ]; then
		echo -e "${GREEN}Success (status code is $status)${RESET}"
	else
		echo -e "${RED}Error (exit code $status)${RESET}"
		exit 1
	fi
}

function tester_with_real() {
	cmd=$1
	should_compare=true
	if [ $# -eq 2 ]; then
		should_compare=$2
	fi

	# Run minishell
	echo "$cmd" | valgrind --track-origins=yes --trace-children=yes --child-silent-after-fork=no --leak-check=full --show-leak-kinds=all --suppressions=$(pwd)/../delivery/valgrind-supression --log-file="$OUT_FILE_VALGRIND" ../delivery/minishell &> $OUT_FILE
	EXIT_STATUS=$?
	if [ ! -f "$OUT_FILE" ]; then
		echo -e "${RED}Test failed: $cmd${RESET}"
		exit 1
	fi
	check_valgrind_logs

	#TODO: Since we are using readline on non-interactive mode, we need to remove the first line of the output. readline is always printing the received command.
	tail -n +2 ${OUT_FILE} > ${OUT_FILE}_2

	# Run bash
	# TODO: Using valgrind on bash just to avoid issues when coparing the output of the applications
	# Valgring --trace-children option changes the path of the executables, like ls to /bin/ls. So a msg like
	# `ls: cannot access 'aaa': No such file or directory` will become `/usr/bin/ls: cannot access 'aaa': No such file or directory`
	# which will cause issues with diff application. To simplify, we apply valgring on bash too
	echo "$cmd" | valgrind --trace-children=yes -q --log-file=/dev/null bash &> $OUT_REAL_FILE
	REAL_EXIT_STATUS=$?

	# Ajust some bash message to make it easier to compare
	sed -i 's/bash: line 1: //' ${OUT_REAL_FILE}

	if [[ "$EXIT_STATUS" != "$REAL_EXIT_STATUS" ]]; then
		echo -e "${RED}Error different error status presented: $cmd${RESET}"
		echo "minishell exit status $EXIT_STATUS"
		echo "bash      exit status $REAL_EXIT_STATUS"
		exit 1
	fi

	if [ $should_compare == true ]; then
		diff "${OUT_FILE}_2" "$OUT_REAL_FILE"
		if [ $? -ne 0 ]; then
			echo -e "${RED}Error on comparing the output: $cmd${RESET}"
			echo -e "${BLUE}CMD send is: \"$cmd\"${RESET}"
			echo -e "${BLUE}Output of minishell is:${RESET}"
			cat ${OUT_FILE}_2
			echo -e "${BLUE}Output from bash is:${RESET}"
			cat $OUT_REAL_FILE
			exit 1
		fi
	fi

	echo -e "${GREEN}Test comparing with real bash passed: $cmd${RESET}"

	rm $OUT_FILE       &> /dev/null
	rm ${OUT_FILE}_2   &> /dev/null
	rm $OUT_REAL_FILE  &> /dev/null
}

function tester_with_real_should_be_different() {
	cmd="$1"
	output=$(tester_with_real "$cmd")
	echo "$output" | grep "Error on comparing the output:" > /dev/null
	if [ $? -ne 0 ]; then
		echo -e "${RED}Error: The output is equal!!!!!!${RESET}"
		exit 1
	fi
	echo -ne "${GREEN}Test comparing with builtin limitation with real bash passed: "
	echo -nE "$cmd"
	echo -e "${RESET}"
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
cmds=("oi hi")
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
tester_grep             'ls && || pwd'    'syntax error near unexpected token "||"'
tester_grep             'ls && ||pwd'     'syntax error near unexpected token "||"'
tester_grep             'ls&& || pwd'     'syntax error near unexpected token "||"'
tester_grep             'ls&&|| pwd'      'syntax error near unexpected token "||"'
tester_grep             'ls &&||pwd'      'syntax error near unexpected token "||"'
tester_grep             'ls&& ||pwd'      'syntax error near unexpected token "||"'
tester_grep             'ls &&|| pwd'     'syntax error near unexpected token "||"'
tester_grep             'ls &&||& pwd'    'syntax error near unexpected token "||"'
tester_grep             'ls & pwd'        "syntax error near unexpected token.*"
tester_grep             'ls&pwd'          'syntax error near unexpected token.*'
tester_grep             'ls &&& pwd'      'syntax error near unexpected token.*'
tester_grep             'ls&&&pwd'        'syntax error near unexpected token.*'
tester_grep             'ls&&&&pwd'       'syntax error near unexpected token.*'
tester_grep             'ls &&&& pwd'     'syntax error near unexpected token.*'
tester_grep             'ls &&&&& pwd'    'syntax error near unexpected token.*'
tester_grep             'ls ||| pwd'      'syntax error near unexpected token.*'
tester_grep             'ls |||| pwd'     'syntax error near unexpected token.*'
tester_grep             'ls ||||| pwd'    'syntax error near unexpected token.*'
tester_grep             'ls <<< pwd'      'syntax error near unexpected token.*'
tester_grep             'ls <<<< pwd'     'syntax error near unexpected token.*'
tester_grep             'ls <<<<< pwd'    'syntax error near unexpected token.*'
tester_grep             'ls >>>>> pwd'    'syntax error near unexpected token.*'
echo ""

tester_grep             '1=10'       "1=10: No such file or directory"
echo ""

tester_grep             'x=10'                "var_name = x, var_value = 10"
tester_grep             'y=20'                "var_name = y, var_value = 20"
tester_grep             'w==20'               "var_name = w, var_value = =20"
tester_grep             "a=\"123\"456'789'\"\". && echo \$a"      "var_name = a, var_value = 123456789."
tester_grep             "a=\"123\"456'789'\"\". && echo \$a"             "123456789."
tester_grep             'echo $'              '$'
tester_grep             'echo $$'             '$$'
tester_grep             'echo $$oi'           '$hi'
tester_grep             "echo '123"           "syntax error near unexpected token.*"
tester_grep             "echo '123'"          "123"
tester_grep             "echo '123\""         "syntax error near unexpected token.*"
tester_grep             "echo '123\"'"        "123\""
tester_grep             "echo '123\"'456"     "123\"456"
tester_grep             "echo '123\"''456'"   "123\"456"
tester_grep             "echo '123\"'\"456\"" "123\"456"
tester_grep             'echo $SHLVL'         "var_name = SHLVL, var_value = 3"
echo ""

echo "################ Testing signals ################"
test_signal "sleep 10"   "SIGINT"  #testing Ctrl+C
test_signal "sleep 10"   "SIGQUIT" #testing Ctrl+\
echo ""

echo "################ Comparing with real bash ################"
make -C ../delivery/ clean &> /dev/null
make -C ../delivery/ -j 16 all &> /dev/null
if [[ $? -ne 0 ]]; then
	echo -e "${RED}Error to compile the project${RESET}"
	exit 1
fi

echo -e "${MAGENTA}Testing var assignments${RESET}"
tester_with_real 'x=10 echo $x'
tester_with_real 'x=10 && echo $x'
tester_with_real 'x=10 y=1 w=3 echo "$x $y $w"'
tester_with_real 'x=10 y=1 w=3 && echo "$x $y $w"'
tester_with_real "x=10 y=1 w=3 bash -c 'echo \"\$x \$y \$w\"'"
tester_with_real "x=10 y=1 w=3 && bash -c 'echo \"\$x \$y \$w\"'"

echo -e "${MAGENTA}Testing redirect output${RESET}"
touch /tmp/test
touch /tmp/test2
tester_with_real 'rm /tmp/test && echo ola! > /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && pwd > /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && ls -la > /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && rm /tmp/test2 && ls -la > /tmp/test  > /tmp/test2 && echo "test" && cat -e /tmp/test && echo "test2" && cat -e /tmp/test2'
tester_with_real 'rm /tmp/test && echo 1 > /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && echo 1 > /tmp/test && echo 2 > /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && echo 1 > /tmp/test && echo 2 >> /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && echo 1 > /tmp/test && echo 2 >> /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && echo 1 >> /tmp/test && echo 2 >> /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && echo 1 >> /tmp/test && echo 2 > /tmp/test && cat -e /tmp/test'
tester_with_real 'rm /tmp/test && ls > /tmp/test | echo 1'
tester_with_real 'rm /tmp/test && ls | grep mini > /tmp/test'
tester_with_real 'rm /tmp/test && ls | grep mini > /tmp/test | echo 1'
tester_with_real 'rm /tmp/test && ls | grep mini > /tmp/test | ls | grep mini'
tester_with_real 'rm /tmp/test && ls | grep mini > /tmp/test && echo 1'
echo -e "${MAGENTA}Testing redirect input${RESET}"
touch /tmp/test
touch /tmp/test2
tester_with_real 'echo ola! > /tmp/test && cat < /tmp/test'
tester_with_real 'rm /tmp/test && echo ola! > /tmp/test && cat -e < /tmp/test'
tester_with_real 'rm /tmp/test && echo ola! > /tmp/test && < /tmp/test cat -e'
rm /tmp/test2
tester_with_real 'rm /tmp/test && echo ola! > /tmp/test && < /tmp/test < /tmp/test2 cat -e'
tester_with_real 'rm /tmp/test && echo ola! > /tmp/test && cat -e < /tmp/test < /tmp/test2'

echo -e "${MAGENTA}Testing parenthesis${RESET}"
tester_with_real "ls"
tester_with_real "(ls)"
tester_with_real "(ls && ls)"
tester_with_real "ls || (ls && ls)"
tester_with_real "ls aaaa || (ls && ls)"
tester_with_real "ls || (ls aaaaa && ls)"
tester_with_real "ls || (ls && ls aaaa)"
tester_with_real "echo 1 && (pwd && cd)"
tester_with_real "echo 1 || (pwd && cd)"
tester_with_real "pwd || (ls | grep mini | cat -e)"
tester_with_real "pwd && (ls | grep mini | cat -e)"
tester_with_real "pwd | (ls)"
tester_with_real "pwd | (ls | grep mini | cat -e)"
# reversed
tester_with_real "(ls && ls) || ls"
tester_with_real "(ls && ls) || ls aaah"
tester_with_real "(ls aaaaa && ls) || ls"
tester_with_real "(ls && ls aaaa) || ls"
tester_with_real "(pwd && cd) && echo 1"
tester_with_real "(pwd && cd) || echo 1"
tester_with_real "(ls | grep mini | cat -e) || pwd"
tester_with_real "(ls | grep mini | cat -e) && pwd"
tester_with_real "(ls) | pwd"
tester_with_real "(ls | grep mini | cat -e) | pwd"
tester_grep      '(ls | pwd'             "Error: Missing closing parenthesis!"
tester_grep      '(ls && pwd'            "Error: Missing closing parenthesis!"
tester_grep      '(ls || pwd'            "Error: Missing closing parenthesis!"
tester_grep      'ls | pwd)'             "Error: syntax error near unexpected token \")\""
tester_grep      'ls && pwd)'            "Error: syntax error near unexpected token \")\""
tester_grep      'ls || pwd)'            "Error: syntax error near unexpected token \")\""
echo ""

echo -e "${MAGENTA}Testing builtin commands${RESET}"
echo -e "${MAGENTA}Testing echo${RESET}"
tester_with_real "echo opa"
tester_with_real "echo -"
tester_with_real "echo - look"
tester_with_real "echo -n look"
tester_with_real "echo look -n"
tester_with_real "echo -nnnn look"
tester_with_real "echo -n -nnn look"
tester_with_real "echo -nnn -nnn look"
tester_with_real "echo -nnxn -nnn look"
tester_with_real "echo -nnnn -nxn look"
tester_with_real_should_be_different "echo -e \"\nlook -n\""
tester_with_real "/usr/bin/echo hello world"
tester_with_real 'echo      456 $y 789 $x     456   '
tester_with_real 'echo "$USER"'
tester_with_real "echo \"'\$USER'\""
tester_with_real "echo \"''\"\$USER            \"oi            aaaa'\""
tester_with_real "echo \"''\"\$USER\"oi            aaaa'\""
tester_with_real "echo \"''\"\$USER\"            oi            aaaa'\""
tester_with_real "echo \"oi\" 'oi' \"'\$USER'\""
echo ""
echo -e "${MAGENTA}Testing cd${RESET}"
tester_with_real "cd"
tester_with_real "cd .."
tester_with_real "cd nonexistent"       false
tester_grep      'cd && pwd'            "$HOME"
tester_grep      "cd $HOME && pwd"      "$HOME"
tester_grep      'cd /tmp && pwd'       "/tmp"
tester_grep      "cd ../ && pwd"        "$(realpath ..)"
tester_grep      "cd ../.. && pwd"      "$(realpath ../..)"
tester_grep      "cd . && pwd"          "$(realpath .)"
tester_grep      "cd /var/log && cd .. && pwd" "/var"
tester_grep      "cd -l"                "minishell: cd: -l: No such file or directory"
echo ""
echo -e "${MAGENTA}Testing env${RESET}"
tester_grep        "x=1 env"               "x=1"
tester_grep        "x=1 y=2 env"               "x=1"
tester_grep        "x=1 y=2 env"               "y=2"
echo ""
echo -e "${MAGENTA}Testing export${RESET}"
tester_with_real 'y=1 && export y && export | grep "declare -x y="'
tester_with_real 'y=1 && export y d && export | grep "declare -x y="'
tester_with_real 'y=1 && export y d && export | grep "declare -x d"'
tester_with_real 'y=1 && export y d w=2 && export | grep "declare -x y="'
tester_with_real 'y=1 && export y d w=2 && export | grep "declare -x d"'
tester_with_real 'y=1 && export y d w=2 && export | grep "declare -x w="'
tester_with_real "y=1 && y=2 export y && export | grep 'declare -x y=\"2\"'"
echo ""
echo -e "${MAGENTA}Testing export and env${RESET}"
tester_with_real 'y=1 && export y && env | grep "y=1"'
tester_with_real 'export y && export | grep "declare -x y"'
tester_with_real 'export y && env | grep "y="'
tester_with_real '(export y && env | grep "y=") && y=2 && env | grep "y=2"'
echo ""
echo -e "${MAGENTA}Testing pwd${RESET}"
tester_with_real 'pwd'
tester_with_real 'cd .. && pwd'
tester_with_real 'cd .. && pwd && cd .. && pwd'
tester_grep      'cd /tmp && pwd'                  "/tmp"
echo ""
echo -e "${MAGENTA}Testing exit${RESET}"
tester_with_real 'exit'
tester_with_real 'y=1 exit'
tester_with_real 'x=1 && exit'
tester_with_real 'ls && exit'
tester_with_real 'pwd && exit'
tester_with_real 'ls && pwd && exit'
tester_with_real 'exit && pwd && ls'
tester_with_real 'exit 12'
tester_with_real 'exit +12'
tester_with_real 'exit -12'
tester_with_real 'exit ++12'
tester_with_real 'exit --12'
tester_with_real 'exit +-12'
tester_with_real 'exit -+12'
tester_with_real 'y=1 exit 13'
tester_with_real 'x=1 && exit 14'
tester_with_real 'ls && exit 15'
tester_with_real 'pwd && exit 16'
tester_with_real 'ls && pwd && exit 17'
tester_with_real 'exit 18 && pwd && ls'
tester_with_real 'exit 9223372036854775807'
tester_with_real 'exit 9223372036854775808'
tester_with_real 'exit 123h'
tester_with_real 'exit asdj'
# Too many arguments
tester_with_real 'exit 1 2'
tester_with_real 'exit 1 221'
tester_with_real 'exit 1 221 1243'
tester_with_real 'exit 1asd 221 1243'
echo ""
echo -e "${MAGENTA}Testing unset${RESET}"
tester_with_real "unset"
tester_with_real "unset y"
tester_with_real 'x=2 && echo $x && unset x && echo $x'
tester_with_real 'x=2 && unset x | ls && echo $x'
echo ""

# Test a normal command
echo -e "${MAGENTA}Testing a normal command${RESET}"
tester_with_real "ls"
tester_with_real "echo opa"
tester_with_real "ls && echo \$?"
tester_with_real "ls llll && echo \$?"
tester_with_real "ls && sleep 1 && pwd"
tester_with_real "pwd && ls && sleep 3 && pwd && sleep 1 && ls"
echo ""

# test AND
echo -e "${MAGENTA}Testing AND operator${RESET}"
tester_with_real "ls && pwd"
tester_with_real "ls aaa && pwd"
# TODO: Enable the test bellow one we are able to implement the printf for stderr
# tester_with_real "lssss && pwd"
echo ""

# test OR
echo -e "${MAGENTA}Testing OR operator${RESET}"
tester_with_real "ls || pwd"
tester_with_real "ls aaa || pwd"
# TODO: Enable the test bellow one we are able to implement the printf for stderr
# tester_with_real "lssss || pwd"
echo ""

# Test PIPE
echo -e "${MAGENTA}Testing PIPE operator${RESET}"
tester_with_real "ls | cat -e"
tester_with_real "ls aaa | cat -e"
# TODO: Enable the test bellow one we are able to implement the printf for stderr
# and chnage the msg from ' lssss: No such file or directory' to 'lssss: command not found'
# tester_with_real "lssss | cat -e"
tester_with_real "ls ../delivery | cat -e"
tester_with_real "ls ../delivery | grep mini | cat -e"
tester_with_real "ls ../delivery | grep mini | cat -e | grep '\.h'"
tester_with_real "ls ../delivery | grep mini | cat -e | grep '\.h' | cat -e"
echo ""

# Mixed AND and OR
echo -e "${MAGENTA}Testing AND and OR operators mixed${RESET}"
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
echo ""

# Mixed AND, OR and PIPE
echo -e "${MAGENTA}Testing AND, OR and PIPE operators mixed${RESET}"
tester_with_real "echo oi && ls ../delivery | grep mini | cat -e"
tester_with_real "ls || echo oi && ls ../delivery | grep mini | cat -e"
tester_with_real "echo 'Hello World' && ls -l | grep 'o'"
echo ""

make -C ../delivery/ clean > /dev/null
