/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:00:26 by djunho            #+#    #+#             */
/*   Updated: 2025/04/21 19:28:07 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <stdlib.h>
#include "utils.h"

#define RUN_TEST(x) \
	if (test##x() == false)						\
	{											\
		printf(RED"Test %d failed\n"RESET, x);	\
		return (1);								\
	}

#define DECL_TEST(x)	\
	extern bool	test##x(void);

#define TEST(x)	\
	test##x

int	main(int argc, char* argv[])
{
	DECL_TEST(0);
	DECL_TEST(1);
	DECL_TEST(2);
	DECL_TEST(3);
	DECL_TEST(4);

	bool (*func[])(void) = {
		TEST(0),
		TEST(1),
		TEST(2),
		TEST(3),
		TEST(4),
	};

	if (argc == 2)
	{
		int x = atoi(argv[1]);
		func[x]();
	} else {
		for (size_t i = 0; i < sizeof(func)/sizeof(func[0]); i++)
			func[i]();
	}

	printf(GREEN"All tests run successfully ✅\n"RESET);
	return (0);
}
