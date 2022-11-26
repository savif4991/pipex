/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 19:54:15 by daejlee           #+#    #+#             */
/*   Updated: 2022/08/13 19:54:16 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>

typedef struct s_pipex
{
	int		outfile_fd;
	int		infile_fd;
	int		pfd_arr[2][2];
	int		*pfd;
	int		*next_pfd;
	char	**envp;
	char	**com;
	pid_t	*pids;
}	t_pipex;

void		swap_pfd(int **pfd1, int **pfd2);
int			check_com_for_quote(char *com);
void		prep_fds(t_pipex *p, int i, int argc);
void		execve_failed(t_pipex *p, char *sh_func);
t_pipex		*init_p(void);
int			free_arr(char **com);
int			err_terminate(t_pipex *p);
char		*ft_strjoin_modified(char const *s1, char const *s2);
char		**ft_split_awk(char *s);
int			wait_for_children(t_pipex *p, pid_t *pids);
int			exec_fork(t_pipex *p, int argc, char *argv[]);

#endif
