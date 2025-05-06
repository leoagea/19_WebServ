# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lagea <lagea@student.s19.be>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/16 13:14:22 by lagea             #+#    #+#              #
#    Updated: 2025/05/06 18:03:14 by lagea            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= Webserv

SRC_DIR 	= src
OBJ_DIR 	= build

SRC 		= 	$(SRC_DIR)/main.cpp $(SRC_DIR)/Webserv.cpp \
				$(SRC_DIR)/config/configFileParser.cpp $(SRC_DIR)/config/serverBlock.cpp $(SRC_DIR)/config/locationBlock.cpp $(SRC_DIR)/config/tokenizer.cpp  $(SRC_DIR)/config/PathChecking.cpp $(SRC_DIR)/config/ErrorReporter.cpp \
				$(SRC_DIR)/server/TcpServer.cpp $(SRC_DIR)/server/DirectoryListing.cpp $(SRC_DIR)/server/Response.cpp $(SRC_DIR)/server/Cookies.cpp $(SRC_DIR)/server/ErrorPageGenerator.cpp \
				$(SRC_DIR)/CGI/CgiHandler.cpp \

OBJ			= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS 		= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.d)
CXX 		= c++
RM 			= rm -f
CXXFLAGS 	= -Wall -Wextra -Wshadow -Wuninitialized -Werror -std=c++98 #-fsanitize=address -g
INCS 		= -Isrc/**/ -I.
	
all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/config
	@mkdir -p $(OBJ_DIR)/server
	@mkdir -p $(OBJ_DIR)/log
	@mkdir -p $(OBJ_DIR)/errors
	@mkdir -p $(OBJ_DIR)/CGI
#go build -C var/www/cgi-bin/scripts/wikipedia/
	$(CXX) $(CXXFLAGS) $(INCS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: all
	./$(NAME)
	
clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re