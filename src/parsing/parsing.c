#include "minishell.h"

t_token *lexer(char *line)
{
    t_token *head = NULL;
    t_token *current = NULL;
    int i = 0;

    while (line[i])
    {
        // skip spaces
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (!line[i])
            break;

        // check for operators
        if (line[i] == '|')
        {
            // add pipe token
            t_token *new = (t_token *)malloc(sizeof(t_token));
            if (!new)
                return NULL;
            new->type = TOKEN_PIPE;
            new->value = ft_strdup("|");
            new->quote_type = 0;
            new->next = NULL;
            if (!head)
                head = new;
            else
                current->next = new;
            current = new;
            i++;
        }
        else if (line[i] == '<')
        {
            if (line[i + 1] == '<')
            {
                // heredoc
                t_token *new = (t_token *)malloc(sizeof(t_token));
                if (!new)
                    return NULL;
                new->type = TOKEN_REDIR_HEREDOC;
                new->value = ft_strdup("<<");
                new->quote_type = 0;
                new->next = NULL;
                if (!head)
                    head = new;
                else
                    current->next = new;
                current = new;
                i += 2;
            }
            else
            {
                // input
                t_token *new = (t_token *)malloc(sizeof(t_token));
                if (!new)
                    return NULL;
                new->type = TOKEN_REDIR_IN;
                new->value = ft_strdup("<");
                new->quote_type = 0;
                new->next = NULL;
                if (!head)
                    head = new;
                else
                    current->next = new;
                current = new;
                i++;
            }
        }
        else if (line[i] == '>')
        {
            if (line[i + 1] == '>')
            {
                // append
                t_token *new = (t_token *)malloc(sizeof(t_token));
                if (!new)
                    return NULL;
                new->type = TOKEN_REDIR_APPEND;
                new->value = ft_strdup(">>");
                new->quote_type = 0;
                new->next = NULL;
                if (!head)
                    head = new;
                else
                    current->next = new;
                current = new;
                i += 2;
            }
            else
            {
                // output
                t_token *new = (t_token *)malloc(sizeof(t_token));
                if (!new)
                    return NULL;
                new->type = TOKEN_REDIR_OUT;
                new->value = ft_strdup(">");
                new->quote_type = 0;
                new->next = NULL;
                if (!head)
                    head = new;
                else
                    current->next = new;
                current = new;
                i++;
            }
        }
        else
        {
            // word or quoted
            int start = i;
            int quote = 0;
            if (line[i] == '\'')
            {
                quote = 1;
                i++;
                start = i;
                while (line[i] && line[i] != '\'')
                    i++;
                if (line[i] == '\'')
                    i++;
            }
            else if (line[i] == '"')
            {
                quote = 2;
                i++;
                start = i;
                while (line[i] && line[i] != '"')
                    i++;
                if (line[i] == '"')
                    i++;
            }
            else
            {
                // unquoted word
                while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '|' && line[i] != '<' && line[i] != '>')
                    i++;
            }
            // create token
            char *val = ft_substr(line, start, i - start);
            if (!val)
                return NULL;
            t_token *new = (t_token *)malloc(sizeof(t_token));
            if (!new)
            {
                free(val);
                return NULL;
            }
            new->type = TOKEN_WORD;
            new->value = val;
            new->quote_type = quote;
            new->next = NULL;
            if (!head)
                head = new;
            else
                current->next = new;
            current = new;
        }
    }
    // add EOF
    t_token *eof = (t_token *)malloc(sizeof(t_token));
    if (!eof)
        return NULL;
    eof->type = TOKEN_EOF;
    eof->value = NULL;
    eof->quote_type = 0;
    eof->next = NULL;
    if (!head)
        head = eof;
    else
        current->next = eof;
    return head;
}

parse_and_execute(line , &shell)
{
    shell->last_exit_status = 0;
    if (line == NULL)
        return;
    if (line[0] == '\0')
        return;
    // here we will parse the line and execute it

    // we will use the lexer to parse the line and get the tokens

    // then we will use the parser to parse the tokens and get the commands
    // then we will use the executor to execute the commands
    // but for now we will just print the line
    // printf("line: %s\n", line);
    t_token *tokens = lexer(line);
    if (tokens == NULL)
        return;
    t_command *commands = parser(tokens);
    if (commands == NULL)
        return;
    executor(commands , &shell);
    // after execute we will free the tokens and commands
    free_tokens(tokens);
    free_commands(commands);
}
