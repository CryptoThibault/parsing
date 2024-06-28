/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:15:54 by tchalaou          #+#    #+#             */
/*   Updated: 2024/06/28 17:38:42 by tchalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <curses.h>
# include <limits.h>

# define WORD 1
# define SEMICOLON 2
# define SMALLER 3
# define BIGGER 4
# define PIPE 5
# define DOLLAR 6

typedef struct	s_var
{
	char	*key;
	char	*value;
}		t_var;

typedef struct s_token
{
	int		id;
	char		*word;
	struct s_token	*next;
}		t_token;

typedef struct s_msh
{
	int		index;
	char	**cmd;
	char	*infile;
	char	*outfile;
	int		here_doc;
	int		trunc_out;
	int		pipe_next;
	int		in_fd;
	int		out_fd;
	int		pipefd[2];
	struct s_msh	*next;
	struct s_msh	*prev;
}		t_msh;

t_token	*lexing(char *line);
void	free_token(t_token **token);

t_msh	*parsing(t_token **token);
void	free_msh(t_msh **msh);

int		array_size(char **array);
void	free_array(char **array);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	builtins(int ac, char **av);
void	cmd_echo(int ac, char **av);
void	cmd_cd(int ac, char **av);
void	cmd_pwd(int ac, char **av);
void	cmd_export(int ac, char **av);
void	cmd_unset(int ac, char **av);
void	cmd_env(int ac, char **av);
void	cmd_exit(int ac, char **av);

#endif
