/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:18:34 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 03:31:45 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

# define RESET "\033[0m"

# define SHELL_NAME "LAZYSHELL"

# define GRAY "\033[90m"
# define RED "\033[31m"    // die or finish eating
# define GREEN "\033[32m"  // eating
# define YELLOW "\033[33m" //
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"

# define BOLD "\033[1m"

/* Readline non-printing sequence guards for colored prompts. */
# define RL_NP_START "\001"
# define RL_NP_END "\002"
# define PROMPT_SHELL_STYLE "\001\033[1m\033[36m\002"
# define PROMPT_DOLLAR_STYLE "\001\033[1m\033[32m\002"
# define PROMPT_RESET "\001\033[0m\002"
# define PROMPT_FULL "\001\033[1m\033[36m\002LAZYSHELL" \
	"\001\033[1m\033[32m\002$ \001\033[0m\002"
# define LINE1 "██╗      █████╗ ███████╗██╗   ██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n"
# define LINE2 "██║     ██╔══██╗╚══███╔╝╚██╗ ██╔╝██╔════╝██║  ██║██╔════╝██║     ██║     \n"
# define LINE3 "██║     ███████║  ███╔╝  ╚████╔╝ ███████╗███████║█████╗  ██║     ██║     \n"
# define LINE4 "██║     ██╔══██║ ███╔╝    ╚██╔╝  ╚════██║██╔══██║██╔══╝  ██║     ██║     \n"
# define LINE5 "███████╗██║  ██║███████╗   ██║   ███████║██║  ██║███████╗███████╗███████╗\n"
# define LINE6 "╚══════╝╚═╝  ╚═╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n"
#endif
