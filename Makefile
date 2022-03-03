NAME 		= 	webserv

PARSE_SRC	=	parse.cpp
UTILS_SRC	=	split.cpp \
				error.cpp
CONNECT_SRC	=	handle_connection.cpp
REQUEST_SRC	=	Header.cpp \
				Response.cpp \
				StatusCodes.cpp 
SERVER_SRC	= 	Location.cpp \
				redir.cpp \
				Server.cpp
SOCKET_SRC	=	Socket.cpp

PARSE		=	$(addprefix srcs/parse/, $(PARSE_SRC))
UTILS		=	$(addprefix srcs/utils/, $(UTILS_SRC))
CONNECT		=	$(addprefix srcs/connection/, $(CONNECT_SRC))
REQUESTS	=	$(addprefix requests/, $(REQUEST_SRC))
SERVER		= 	$(addprefix server/, $(SERVER_SRC))
SOCKET		= 	$(addprefix socket/, $(SOCKET_SRC))

SOURCES		= 	main.cpp \
				$(PARSE) \
				$(UTILS) \
				$(CONNECT) \
				$(REQUESTS) \
				$(SOCKET) \
				$(SERVER)

OBJDIR		=	./obj/
OBJECTS 	=	$(SOURCES:%.cpp=$(OBJDIR)%.o)

FLAGS 		=	 -std=c++98
COMPILE		=	clang++

INC			=	-Iincludes -Irequests -Iserver -Isocket

GREEN 		= 	\033[38;5;46m
WHITE 		= 	\033[38;5;15m
GREY 		= 	\033[38;5;8m
ORANGE 		= 	\033[38;5;202m
RED 		= 	\033[38;5;160m

all: $(NAME)

$(OBJDIR)%.o: %.cpp
	@mkdir -p $(@D)
	@echo "$(GREY)Compiling...				$(WHITE)$<"
	@$(COMPILE) $(FLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJECTS)
	@$(COMPILE) $(FLAGS) $(INC) $(OBJECTS) -o $(NAME)
	@echo "$(GREEN)Use exe - $(NAME)"

clean:
	@echo "$(RED)Deleting objectfiles"
	@/bin/rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)Deleting executable"
	@/bin/rm -f $(NAME)

fclena: fclean

re: fclean all

.PHONY: all clean fclean re