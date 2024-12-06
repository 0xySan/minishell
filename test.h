/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:28:07 by etaquet           #+#    #+#             */
/*   Updated: 2024/12/06 23:25:45 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <linux/limits.h>
# include "libft/libft.h"
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>

# define BUFFER_SIZE 1024

typedef struct s_path
{
	char	*rpath;
	char	*tpath;
}	t_path;

typedef struct s_pidstruct
{
	pid_t	*pid;
}	t_pidstruct;

#endif