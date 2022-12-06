/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isspace.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: fsnelder <fsnelder@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/05 14:22:28 by fsnelder      #+#    #+#                 */
/*   Updated: 2022/12/05 14:22:29 by fsnelder      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(int ch)
{
	return (ch == ' '
		|| ch == '\n' \
		|| ch == '\t' \
		|| ch == '\v' \
		|| ch == '\f' \
		|| ch == '\r');
}