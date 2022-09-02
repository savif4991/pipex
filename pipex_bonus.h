/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 16:27:31 by daejlee           #+#    #+#             */
/*   Updated: 2022/09/02 16:27:34 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include <unistd.h>

typedef struct fd_list
{
	int		outfile_fd;
	int		infile_fd;
	int		pfd_arr[2][2];
	int		*pfd;
	int		*next_pfd;
	char	**envp;
	char	**com;
	pid_t	*pids;
}	t_fd_list;

void		swap_pfd(int **pfd1, int **pfd2);
int			check_com_for_quote(char *com);
void		prep_fds(t_fd_list *p, int i, int argc);
void		prep(int input_fd, int output_fd, int closing_fd, t_fd_list *p);
void		execve_failed(t_fd_list *p, char *sh_func);
t_fd_list	*init_p(void);
int			free_arr(char **com);
int			err_terminate(t_fd_list *p);
char		*ft_strjoin_modified(char const *s1, char const *s2);
char		**ft_split_awk(char *s);
int			wait_for_children(t_fd_list *p, pid_t *pids);

#endif
