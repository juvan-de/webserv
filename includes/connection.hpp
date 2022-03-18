/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 12:56:39 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/17 15:27:07 by ztan          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <Request.hpp>
# include <poll.h>
# include <Server.hpp>
# include <defines.hpp> // data struct, client struct

void				handle_connection(std::vector<pollfd> &fds, std::vector<Server> servers, size_t start);
Request				read_request(struct pollfd &fd);
struct sockaddr_in	get_addr();
void				add_cli_to_pollfds(t_data &data, int cli_sock);
void				check_connection(t_data &data, int i);


#endif
