/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:02:41 by tchalaou          #+#    #+#             */
/*   Updated: 2024/06/25 17:15:05 by tchalaou         ###   ########.fr       */
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
	msh->pipe_next = 0;
	msh->in_fd = -1;
	msh->out_fd = -1;
	msh->next = NULL;
	msh->prev = NULL;
	return (msh);
}

void	fill_msh(t_msh *msh, t_token **token)
{
		
}

t_msh	*mshget_last(t_msh *lst)
{
	t_msh	*current;

	if (!lst)
		return (0);
	current = lst;
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

t_msh	*parsing(t_token *token)
{
	t_msh	*msh;
	t_msh	*new;
	int	i;

	msh = NULL;
	i = -1;
	while (token)
	{
		new = create_msh(++i);
		//fill_msh(new, &token);
		mshadd_back(&msh, new);
		token = token->next;
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

int	main()
{
	t_token	*token;
	t_msh	*msh;
	t_msh	*start;

	token = lexing(" aa; <bb> | $cc ");
	msh = parsing(token);
	free_token(&token);
	msh = start;
	while (msh)
	{
		printf("index: %d\n", msh->index);
		msh = msh->next;
	}
//	free_msh(&start);
	return (0);
}
