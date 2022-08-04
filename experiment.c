#include "./libft/ft_printf.h" //헤더가 없으면 gnl리턴이 안되더라..
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
// cc -g -o pipex pipex.c -lft -L./libft
// ./pipex infile
//$> ./pipex infile "grep new" "wc -w" outfile
//< infile grep new | wc -w > outfile
void	free_arr(void **com)
{
	unsigned int	i;

	i = 0;
	while (com[i])
		free(com[i++]);
	free(com);
}

int		err()
{
	perror("ERROR!");
	return (1);
}

void	swap_pfd(int **pfd1, int **pfd2)
{
	int	*temp;

	temp = *pfd1;
	*pfd1 = *pfd2;
	*pfd2 = temp;
}

void	close_fd(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

char	*get_sh_func(char **com)
{
	char	*sh_func;

	sh_func = (char *)malloc(ft_strlen("/bin/") + ft_strlen(com[0]) + 1);
	if (!sh_func)
		return (NULL);
	ft_strlcpy(sh_func, "/bin/", 6);
	ft_strlcpy(sh_func + 5, com[0], ft_strlen(com[0]) + 1);
	return (sh_func);
}

char	*get_redircom(char *argv[], unsigned int i)
{
	char	*redircom;

	redircom = (char *)malloc(ft_strlen(argv[i]) + ft_strlen("cat ") + 1);
	if (!redircom)
		return (NULL);
	ft_strlcpy(redircom, "cat ", 5);
	ft_strlcpy(redircom + 4, argv[i], ft_strlen(argv[i]) + 1);
	return (redircom);
}

char	**get_com(unsigned int i, char* argv[], int argc)
{
	char	**com;
	if (i == 1 || i == argc - 1)
		com = ft_split(get_redircom(argv, i), ' ');
	else
		com = ft_split(argv[i], ' ');
	return (com);
}

void	*exec_com(char **com, unsigned int i, int input_fd, int output_fd, int closing_fd)
{
	pid_t	pid;
	pid_t	child_pid;
	int		status;
	char	*sh_func;
	char	buffer[100];

	if (!com)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (NULL);
	else if (!pid)  //child
	{
		close(closing_fd);
		close(2);
		if (input_fd)
		{
			dup2(input_fd, 0);
			close(input_fd);
		}
		if (output_fd != 1)
		{
			dup2(output_fd, 1);
			close(output_fd);
		}
		sh_func = get_sh_func(com);
		if (!sh_func)
			exit(1);
		execve((const char *)sh_func, (char * const*)com, NULL);
	}
	else // parents
	{
		close(closing_fd);
		child_pid = wait(&status);
		printf("child status : %d, log : %x\n", child_pid, status);
	}
	free_arr((void **)com);
}

int	main(int argc, char* argv[])
{
	int				outfile_fd;
	int				infile_fd;
	int				pfd_arr[2][2];
	int				*pfd;
	int				*next_pfd;
	char			**com;
	unsigned int	i;
	char			buffer[100];

	infile_fd = open(argv[1], O_RDONLY);
	outfile_fd = open(argv[argc - 1], O_WRONLY);
	if (argc < 5 || outfile_fd == -1 || infile_fd == -1)
		return(err());
	pfd = pfd_arr[0];
	next_pfd = pfd_arr[1];
	if (pipe(pfd) == -1)
		return(err());
	i = 2;
	com = get_com(i++, argv, argc);
	exec_com(com, i, infile_fd, pfd[1], -1);
	close(infile_fd);
	while (i < argc - 1)
	{
		if (pipe(next_pfd) == -1)
			return(err());
		com = get_com(i++, argv, argc);
		if (i == argc - 1)
			dup2(outfile_fd, next_pfd[1]);
		exec_com(com, i, pfd[0], next_pfd[1], pfd[1]);
		close_fd(pfd);
		swap_pfd(&pfd, &next_pfd);
	}
	close(outfile_fd);
	close_fd(next_pfd);
	return (0);
}