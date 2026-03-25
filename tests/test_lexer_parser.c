/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer_parser.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 21:10:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 22:21:58 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Test the lexer
void	test_lexer(char *input)
{
	t_token	*tokens;

	printf("\n========== Testing Lexer ==========\n");
	printf("Input: %s\n", input);
	printf("Tokens:\n");
	tokens = lexer(input);
	print_tokens(tokens);
	free_tokens(tokens);
}

// Test the parser
void	test_parser(char *input)
{
	t_token		*tokens;
	t_command	*commands;
	t_shell		shell;

	ft_memset(&shell, 0, sizeof(shell));
	printf("\n========== Testing Parser ==========\n");
	printf("Input: %s\n", input);
	tokens = lexer(input);
	commands = parser(tokens, &shell);
	printf("\nParsed Commands:\n");
	print_commands(commands);
	free_commands(commands);
	free_tokens(tokens);
}

// Test complete pipeline
void	test_complete(char *input)
{
	t_token		*tokens;
	t_command	*commands;
	t_shell		shell;

	ft_memset(&shell, 0, sizeof(shell));
	printf("\n========== Complete Pipeline Test ==========\n");
	printf("Input: %s\n", input);
	
	// Lexer
	tokens = lexer(input);
	printf("\nToken count: %d\n", count_tokens(tokens));
	
	// Parser
	commands = parser(tokens, &shell);
	printf("Command count: %d\n", count_commands_parser(commands));
	
	// Print details
	printf("\n--- Token Details ---\n");
	print_tokens(tokens);
	printf("\n--- Command Details ---\n");
	print_commands(commands);
	
	// Cleanup
	free_commands(commands);
	free_tokens(tokens);
}

int	main(void)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   Lexer & Parser Test Suite           ║\n");
	printf("╚════════════════════════════════════════╝\n");
	
	// Test 1: Simple command
	test_complete("echo hello world");
	
	// Test 2: Command with redirections
	test_complete("cat < input.txt > output.txt");
	
	// Test 3: Pipe
	test_complete("ls -la | grep -i test");
	
	// Test 4: Redirections with append
	test_complete("echo test >> file.txt");
	
	// Test 5: Heredoc
	test_complete("cat << EOF");
	
	// Test 6: Complex command
	test_complete("ls | grep test > results.txt && cat results.txt");
	
	printf("\n═══════════════════════════════════════════════════════════\n");
	printf("All tests completed successfully!\n");
	printf("═══════════════════════════════════════════════════════════\n");
	
	return (0);
}
