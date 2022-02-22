/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 12:56:39 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/02/22 12:27:14 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <Header.hpp>
# include <poll.h>
# include <Server.hpp>


void				handle_connection(std::vector<pollfd> &fds, size_t start);
Header				read_request(struct pollfd &fd);
struct sockaddr_in	get_addr();


#endif
