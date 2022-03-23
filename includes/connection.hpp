/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 12:56:39 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/21 17:10:00 by ztan          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <Request.hpp>
# include <poll.h>
# include <Server.hpp>
# include <defines.hpp>


void				handle_connection(std::vector<Server> &servers, std::vector<t_client> &clients);
Request				read_request(struct pollfd &fd);

#endif
