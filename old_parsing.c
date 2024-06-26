/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 16:45:55 by tchalaou          #+#    #+#             */
/*   Updated: 2024/06/21 12:08:41 by tchalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->id = 0;
	token->ac = 0;
	token->av = NULL;
	return (token);
}

void	free_token(t_token *token)
{
	if (token->av)
		free_array(token->av);
	free(token);
}

void	switch_redirect_in(t_token *token, int *i, char *line)
{
	if (line[*i++ + 1] == '<')
		token->id = DOUBLE_REDIRECT_IN;
	else
	{
		token->id = REDIRECT_IN;
	}
}

void	switch_redirect_out(t_token *token, int *i, char *line)
{
	if (line[*i++ + 1] == '>')
		token->id = DOUBLE_REDIRECT_OUT;
	else
		token->id = REDIRECT_OUT;
}

int	count_args(int i, char *line)
{
	(void)i;
	(void)line;
	return (1);
}

void	fill_command(t_token *token, int *i, char *line)
{
	int	j;

	token->id = COMMAND;
	token->ac = count_args(*i, line);
	token->av = malloc(sizeof(char *) * token->ac);
	if (!token->av)
		return ;
	//COMMAND
}

void	fill_token(t_token *token, int *i, char *line)
{
	if (line[*i] == '\\')
		token->id = ESCAPE;
	else if (line[*i] == ';')
		token->id = SEMICOLON;
	else if (line[*i] == '\'')
		token->id = SINGLE_QUOTE;
	else if (line[*i] == '"')
		token->id = DOUBLE_QUOTE;
	else if (line[*i] == '<')
		switch_redirect_in(token, i, line);
	else if (line[*i] == '>')
		switch_redirect_out(token, i, line);
	else if (line[*i] == '|')
		token->id = PIPE;
	else if (line[*i] == '$')
		token->id = VARIABLE;
	else
		fill_command(token, i, line);
}

int	count_tokens(char *line)
{
	(void)line;
	return (1);	
}

t_token	**parsing(char *line)
{
	t_token	**tokens;
	int		i;
	int		j;

	tokens = malloc(sizeof(t_token *) * count_tokens(line));
	if (!tokens)
		return (NULL);
	i = -1;
	j = -1;
	while (line[++i])
	{
		tokens[++j] = create_token();
		fill_token(tokens[j], &i, line);
	}
	tokens[++j] = NULL;
	return (tokens);
}

int	main(int ac, char **av)
{
	t_token	**tokens;
	int		i;
	int		j;

	if (ac != 2)
		return (1);
	tokens = parsing(av[1]);
	i = -1;
	while (tokens[++i])
	{
		printf("id: %d\n", tokens[i]->id);
		printf("ac: %d\n", tokens[i]->ac);
		j = -1;
		while (tokens[i]->av && tokens[i]->av[++j])
			printf("av[%d]: %s\n", j, tokens[i]->av[j]);
	}
	return (0);
}
