CC = gcc
NAME = Bomberman
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -std=gnu99
SDLFLAGS = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lfreetype -lpthread
SRC =	./src/main.c \
		./src/network/client.c \
		./src/network/server.c \
		./src/instances/bomb.c \
		./src/instances/cell.c \
		./src/instances/map.c \
		./src/instances/object.c \
		./src/instances/player.c \
		./src/gui/gui.c \
		./src/gui/menu_gui.c \
		./src/gui/menu_user_write.c


OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) $(SDLFLAGS) -o $(NAME)

clean:
		$(RM) $(OBJ)

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PRONY: all $(NAME) clean fclean re
