# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lagea <lagea@student.s19.be>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/16 13:14:22 by lagea             #+#    #+#              #
#    Updated: 2024/12/30 17:21:36 by lagea            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= Webserv

SRC_DIR 	= src
OBJ_DIR 	= build

SRC 		= 	$(SRC_DIR)/main.cpp \
				$(SRC_DIR)/config/configFileParser.cpp $(SRC_DIR)/config/serverBlock.cpp $(SRC_DIR)/config/locationBlock.cpp $(SRC_DIR)/config/tokenizer.cpp  $(SRC_DIR)/config/PathChecking.cpp $(SRC_DIR)/config/ErrorReporter.cpp \
				$(SRC_DIR)/server/TcpServer.cpp $(SRC_DIR)/server/Request.cpp \
				$(SRC_DIR)/log/Log.cpp $(SRC_DIR)/log/LogReporter.cpp \
				
OBJ			= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS 		= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.d)
CC 			= c++
RM 			= rm -f
CPPFLAGS 	= -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g
INCS 		= -Isrc/**/ -I.
	
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/config
	@mkdir -p $(OBJ_DIR)/server
	@mkdir -p $(OBJ_DIR)/log
	$(CC) $(CPPFLAGS) $(INCS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: all
	./$(NAME)
	
clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) -r $(OBJ_DIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re