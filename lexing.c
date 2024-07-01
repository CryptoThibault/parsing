/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:51:23 by tchalaou          #+#    #+#             */
/*   Updated: 2024/06/28 15:20:54 by tchalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(int id, char *word)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->id = id;
	token->word = word;
	token->next = NULL;
	return (token);
}

int	is_whitespace(char c)
{
	return (c != ' ' && c != '\t' && c != '\v' && c != '\n');
}

int	word_len(char *line, int start)
{
	int	len;

	len = 0;
	while (line[start] && !ft_strchr(" \t\n;<>|$", line[start]))
	{
		len++;
		start++;
	}
	return (len);
}

void	fill_word(char **word, char *line, int *i)
{
	int	j;

	*word = malloc(word_len(line, *i) + 1);
	if (!*word)
		return ;
	j = 0;
	while (line[*i] && !ft_strchr(" \t\n;<>|$", line[*i]))
	{
		(*word)[j++] = line[*i];
		(*i)++;
	}
	(*word)[j] = 0;
	(*i)--;
}

int	fill_token(char **word, char *line, int *i)
{
	while (!is_whitespace(line[*i]))
		(*i)++;
	if (!line[*i])
		return (0);
	if (line[*i] == '<')
		return (SMALLER);
	else if (line[*i] == '>')
		return (BIGGER);
	else if (line[*i] == '|')
		return (PIPE);
	else if (line[*i] == '$')
		return (DOLLAR);
	else
	{
		fill_word(word, line, i);
		return (WORD);
	}
}

void	tokenadd_back(t_token **token, t_token *new)
{
	if (!new)
		return ;
	if (!*token)
		*token = new;
	else
	{
		while (*token)
			token = &(*token)->next;
		*token = new;
	}
}

t_token	*lexing(char *line)
{
	t_token	*token;
	int		i;
	int		id;
	char	*word;

	token = NULL;
	i = -1;
	while (line[++i])
	{
		word = NULL;
		id = fill_token(&word, line, &i);
		if (!id)
			break ;
		tokenadd_back(&token, create_token(id, word));
	}
	return (token);
}

void	free_token(t_token **token)
{
	t_token	*current;

	while (*token)
	{
		current = *token;
		*token = current->next;
		if (current->word)
			free(current->word);
		free(current);
	}
}
/*
int	main()
{
	t_token	*token;
	t_token *start;

	token = lexing(" aa; <bb> | $cc ");
	start = token;
	while (token)
	{
		printf("id: %d\n", token->id);
		if (token->word)
			printf("word: %s\n", token->word);
		token = token->next;
	}
	free_token(&start);
	return (0);
}*/
