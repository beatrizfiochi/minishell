Verificar se precisa implementar o |&
valgrind-supression -> valgrind-suppression
lidar com x=10 y=20 (na mesma linha) --> deve guardar essas variaveis
	se x=10 y=20 ls ou echo etc (outro comando), nao guarda as variaveis
lidar com x=10 && echo $x
	output esperado: 10

# issue on wait of last process
When running the command
ls && pwd && ls -l
the `ls -l` is not waiting for the finished of aplication pwd
If the tree is 
        &&
      /   \
     /     \
   &&      ls -l
 /   \
ls  pwd

The && on top (the second && to run) is checking the operator of the left (another &&) if is a command to wait for the PID
I guess the best is to create a function that gets the last pid to run that will search on the left and possible on left->right

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

