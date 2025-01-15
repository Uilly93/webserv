NAME 		=	Webserv
CXX 		=	clang++
CPPFLAGS 	=	-Wall -Wextra -Werror -MMD -g3 -O2 -std=c++98
SRC_PATH	=	src/
OBJ_PATH	=	.obj/
SRC_NAME	=	main.cpp \
				Server.cpp
OBJ_NAME	=	$(SRC_NAME:.cpp=.o) 
OBJ 		=	$(addprefix $(OBJ_PATH), $(OBJ_NAME))
SRC 		=	$(addprefix $(SRC_PATH), $(SRC_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJ)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) -o $@ -c $<

-include $(OBJ:%.o=%.d)

clean:
	rm -rf .obj

fclean: clean
	rm -rf $(NAME)

re:
	@$(MAKE) -s fclean all

.PHONY: all clean fclean re
