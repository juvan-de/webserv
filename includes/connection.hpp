/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 12:56:39 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/04/21 16:24:56 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <Request.hpp>
# include <poll.h>
# include <Server.hpp>
# include <defines.hpp>

void				handle_connection(t_data &data);
Request				read_request(struct pollfd &fd);

// connection utils
std::string	getFileName(const Location& loc);
Server	*find_server(std::map<std::pair<int, std::string>, Server*>& table, Request Request);

// handle response
void				handle_response(t_client& client, t_data& data);

// errors

#endif
