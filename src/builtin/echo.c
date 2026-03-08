/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 19:46:56 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/08 16:03:58 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	return (TRUE); // this mean we skip it all
static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (FALSE);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE); // this mean we skip it all
}

// this is fun for printing
// echo
// echo "hello world"    is the same as
// echo hello world
// echo -n this mean dont print \n
// i++; // skip the -n flag itself
//	if (!n_flag) //! true == false

int	echo(char **args)
{
	int	n_flag;
	int	i;

	i = 1;
	n_flag = FALSE;
	while (is_n_flag(args[i]))
	{
		n_flag = TRUE;
		i++;
	}
	while (args[i] != NULL)
	{
		ft_printf("%s", args[i]);
		if (args[i + 1] != NULL)
			ft_printf(" ");
		i++;
	}
	if (!n_flag)
		ft_printf("\n");
	return (TRUE);
}
