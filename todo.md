Verificar se precisa implementar o |&
valgrind-supression -> valgrind-suppression

# Variaveis

## Expansao de variaveis
exemplo e como se comporta no bash:
var="echo var      ll"
$var -> var      ll
var="echo                var"
echo $var -> echo var
var="echo     ola | cat -e"
$var -> ola | cat -e
test="cho ola"
e$test -> ola
e"$test" -> echo ola: command not found

```
bfiochi-@c2r6s11:~/minishell/delivery$ var2="beatriz"
bfiochi-@c2r6s11:~/minishell/delivery$ var1="daniel e $var2"
bfiochi-@c2r6s11:~/minishell/delivery$ var="echo 'ola $var1   ola'&&ls"
bfiochi-@c2r6s11:~/minishell/delivery$ echo "$var"
echo 'ola daniel e beatriz   ola'&&ls
bfiochi-@c2r6s11:~/minishell/delivery$ $var
'ola daniel e beatriz ola'&&ls
bfiochi-@c2r6s11:~/minishell/delivery$ ^C
bfiochi-@c2r6s11:~/minishell/delivery$ echo $var
echo 'ola daniel e beatriz ola'&&ls
bfiochi-@c2r6s11:~/minishell/delivery$
```

## Uso de parentesis, segundo o man bash não serão persistidos
```
(list) list is executed in a subshell (see COMMAND EXECUTION ENVIRONMENT below for a description of a subshell environment).  Variable assignments and builtin commands that affect the shell's environment do not remain in effect after the command completes.  The return
              status is the exit status of list.
```
O seguinte exemplo mostra esse comportamento:
```
$ a=1 && echo outside $a && (echo inside $a && a=2 && echo inside $a) && echo outside $a
outside 1
inside 1
inside 2
outside 1
```

Outro exemplo:
```
$ y=1 && (x=11 && y=2 && echo "x=$x; y=$y") && echo "x=$x; y=$y"
x=11; y=2
x=; y=1
```

Ideia de solução. Ao entrar num parentesis, copiar a lista de váriáveis para uma outra. E usar essa outra

# HEREDOC
## dont expand before checking the eof
cat << $somecar
needs o compare before expanding

## run all the heredocs before runing the tree
<< a && ls && << b
needs to run all the heredocs before running the tree

## Ctr-C exits all the code

## Ctr-D exits only the current heredoc
- Runs as it was found the eof
```
$ << a cat | cat -e
> aaaaaaaaaaaaaaaaa
>
bash: warning: here-document at line 31 delimited by end-of-file (wanted `a')
$
aaaaaaaaaaaaaaaaa$
```

# Redirect out

Com a estratégia implementada (marcar o possivel ultimo commando que será executado como redirect), teremos alguns problemas ao usar parentesis
Por exemplo: `(ls || pwd) > /tmp/file`

Ideia de solução: Rodar os parentesis em um fork a parte. O before iria fazer o fork. O filho iria rodar e todo output do filho iria para o arquivo.

# echo

verificar: echo 456 $x 789 -> output: 456  789
no bash o output seria: 456 789

# delete of current folder

``` minishell
My shell > cd /tmp/
My shell > pwd
/tmp
My shell > mkdir deleteme
My shell > cd deleteme/
My shell > pwd
/tmp/deleteme
My shell > env | grep PWD
PWD=/home/bfiochi-/minishell/delivery
OLDPWD=/home/bfiochi-/minishell/delivery
My shell > rm -rf ../deleteme
My shell > pwd
bash: pwd: getcwd failed: No such file or directory
```

```bash
bfiochi-@c2r6s11:~$ cd /tmp
bfiochi-@c2r6s11:/tmp$ mkdir test
mkdir: cannot create directory ‘test’: File exists
bfiochi-@c2r6s11:/tmp$ cd test
bash: cd: test: Not a directory
bfiochi-@c2r6s11:/tmp$ mkdir deleteme
bfiochi-@c2r6s11:/tmp$ cd deleteme/
bfiochi-@c2r6s11:/tmp/deleteme$ env | grep PWD
PWD=/tmp/deleteme
OLDPWD=/tmp
bfiochi-@c2r6s11:/tmp/deleteme$ pwd
/tmp/deleteme
bfiochi-@c2r6s11:/tmp/deleteme$ rm -rf ../deleteme
bfiochi-@c2r6s11:/tmp/deleteme$ env | grep PWD
PWD=/tmp/deleteme
OLDPWD=/tmp
bfiochi-@c2r6s11:/tmp/deleteme$ pwd
/tmp/deleteme
```

```
My shell > cd /tmp
My shell > mkdir test1
My shell > cd test1/
My shell > mkdir test1
My shell > cd test1/
My shell > pwd
/tmp/test1/test1
My shell > rm -rf ../../test1
My shell > cd ..
fish: Job 1, './minishell' terminated by signal SIGSEGV (Address boundary error)
```

Sugest~ao data 'e usar a vari'avel PWD quando o getcwd falhar!

# Limpar

- Verificar a Removoção do arquivo clean_token_quotes.c
