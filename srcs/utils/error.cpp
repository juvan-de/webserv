/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 14:37:43 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/02/15 14:44:16 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <utils.hpp>

void	error_check(int err, std::string msg)
{
	if (err < 0)
	{
		std::cout << "Error: " << msg << std::endl;
		exit(EXIT_FAILURE);	
	}
}