/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:02:41 by tchalaou          #+#    #+#             */
/*   Updated: 2024/06/28 17:47:32 by tchalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_msh	*create_msh(int index)
{
	t_msh	*msh;

	msh = malloc(sizeof(t_msh));
	if (!msh)
		return (NULL);
	msh->index = index;
	msh->cmd = NULL;
	msh->infile = NULL;
	msh->outfile = NULL;
	msh->here_doc = 0;
	msh->trunc_out = 0;
	msh->pipe_next = 0;
	msh->in_fd = -1;
	msh->out_fd = -1;
	msh->next = NULL;
	msh->prev = NULL;
	return (msh);
}

void	fill_command(t_msh *msh, t_token **token)
{
	int	i;
	
	i = 0;
	msh->cmd[i] = ft_strdup((*token)->word);
	while ((*token)->next && (*token)->next->id == WORD)
	{
		token = &(*token)->next;
		msh->cmd[++i] = ft_strdup((*token)->word);
	}
}

void	fill_smaller(t_msh *msh, t_token **token)
{
	if ((*token)->next->id == SMALLER)
	{
		token = &(*token)->next;
		msh->here_doc = 1;
	}
	if ((*token)->next->id == WORD)
	{
		token = &(*token)->next;
		msh->infile = ft_strdup((*token)->next->word);
	}
}

void	fill_bigger(t_msh *msh, t_token **token)
{
	if ((*token)->next->id == SMALLER)
	{
		token = &(*token)->next;
		msh->trunc_out = 1;
	}
	if ((*token)->next->id == WORD)
	{
		token = &(*token)->next;
		msh->outfile = ft_strdup((*token)->next->word);
	}
}

void	fill_msh(t_msh *msh, t_token **token)
{
	if ((*token)->id == WORD)
	{
		fill_command(msh, token);
		fill_msh(msh, token);
	}
	else if ((*token)->id == SEMICOLON)
		return ;
	else if ((*token)->id == SMALLER)
	{
		fill_smaller(msh, token);
		fill_msh(msh, token);
	}
	else if ((*token)->id == BIGGER)
	{
		fill_bigger(msh, token);
		fill_msh(msh, token);
	}
	else if ((*token)->id == PIPE)
	{
		msh->pipe_next = 1;
		return ;
	}
}

t_msh	*mshget_last(t_msh *msh)
{
	t_msh	*current;

	if (!msh)
		return (0);
	current = msh;
	while (current->next)
		current = current->next;
	return (current);
}

void	mshadd_back(t_msh **msh, t_msh *new)
{
	t_msh	*last;

	if (!new)
		return ;
	if (!*msh)
		*msh = new;
	else
	{
		last = mshget_last(*msh);
		last->next = new;
		new->prev = last;
	}
}

t_msh	*parsing(t_token **token)
{
	t_msh	*msh;
	t_msh	*new;
	int		i;

	msh = NULL;
	i = -1;
	while (*token)
	{
		new = create_msh(++i);
		fill_msh(new, token);
		mshadd_back(&msh, new);
		token = &(*token)->next;
	}
	return (msh);
}

void	free_msh(t_msh **msh)
{
	t_msh	*current;

	while (*msh)
	{
		current = *msh;
		*msh = current->next;
		if (current->cmd)
			free_array(current->cmd);
		if (current->infile)
			free(current->infile);
		if (current->outfile)
			free(current->outfile);
		free(current);
	}
}

int	main(void)
{
	t_token	*token;
	t_token *st_token;
	t_msh	*msh;
	t_msh	*st_msh;


	token = lexing("< infile ls | cat -e > outfile; echo $PATH");
	st_token = token;
	msh = parsing(&token);
	free_token(&st_token);
	st_msh = msh;
	while (msh)
	{
		printf("index: %d\n", msh->index);
		if (msh->cmd)
		{
			int	i = -1;
			while (msh->cmd[++i])
				printf("%s-", msh->cmd[i]);
		}
		msh = msh->next;
	}
	free_msh(&st_msh);
	return (0);
}
