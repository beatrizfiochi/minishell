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
