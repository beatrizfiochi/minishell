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

The && on top (the sencond && to run) is checing the operator of the left (another &&) if is a command to wait for the PID
I guess the best is to create a function that gets the last pid to run that will search on the left and possible on left->right

# Issue on pipe with builtins
The pipe is only configured on childrens. How to handle it? Maybe move the builtins that prints (only echo?) to run on a child
