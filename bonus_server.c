/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_server.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qduong <qduong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 03:50:19 by qduong            #+#    #+#             */
/*   Updated: 2022/01/05 10:25:29 by qduong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf/ft_printf.h"
#include <signal.h>

/*takes client PID received from using sigaction
(in info-> si_pid if not existing), static variables 
declared to save the value of byte transfered, PID and building
up bitwise charsigusr 2 sent after successful putchar, sigusr1 sent to signal
termination, since that means no information is being received from client*/

static void	binary_handler(int sig, siginfo_t *info, void *context)
{
	static int				i = 0;
	static pid_t			client_pid = 0;
	static unsigned char	c = 0;

	(void)context;
	if (!client_pid)
		client_pid = info->si_pid;
	c = c | (sig == SIGUSR2);
	if (++i == 8)
	{
		i = 0;
		if (!c)
		{
			kill(client_pid, SIGUSR1);
			client_pid = 0;
			return ;
		}
		ft_putchar_fd(c, 1);
		c = 0;
		kill (client_pid, SIGUSR2);
	}
	else
		c <<= 1;
}

int	main(void)
{
	struct sigaction	s_sigaction;
	int					id;

	id = getpid();
	ft_printf("Server pid: %i\n", id);
	s_sigaction.sa_sigaction = binary_handler;
	s_sigaction.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &s_sigaction, 0);
	sigaction(SIGUSR2, &s_sigaction, 0);
	while (1)
		pause();
	return (0);
}
