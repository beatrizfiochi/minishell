Verificar se precisa implementar o |&
valgrind-supression -> valgrind-suppression

# Variaveis

## Variaveis na mesma linha
lidar com x=10 y=20 (na mesma linha) --> deve guardar essas variaveis
	se x=10 y=20 ls ou echo etc (outro comando), nao guarda as variaveis
lidar com x=10 && echo $x
	output esperado: 10

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

# Update the SHLVL env var

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
