Verificar se precisa implementar o |&
Decidir o que fazer quando, na expansao de variavel, recebemos algo como echo "$$oi"
> oi=123
>>esperamos: '$123'    ou        'oi'            ??? "tratando $$ como uma variavel vazia?"
valgrind-supression -> valgrind-suppression
lidar com:
		bfiochi-@c2r3s11:~/minishell/delivery$ x="1"1"&&""
		> "
		bfiochi-@c2r3s11:~/minishell/delivery$ x="1"1"&&\""
		bfiochi-@c2r3s11:~/minishell/delivery$ echo $x
		11&&"
lidar com x=10 y=20 (na mesma linha) --> deve guardar essas variaveis
	se x=10 y=20 ls ou echo etc (outro comando), nao guarda as variaveis
lidar com x=10 && echo $x
	output esperado: 10

