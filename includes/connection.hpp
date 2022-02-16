/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 12:56:39 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/02/15 13:38:32 by ztan          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <Header.hpp>

void		handle_connection(std::vector<pollfd> &fds);
Header		read_request(struct pollfd &fd);


#endif
