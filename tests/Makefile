#
# Created by Anonymus Raccoon on 28/12/2019.
#

SRC = test_main.c

OBJ = $(SRC:%.c=%.o)

INCLUDE = -I ../include

CFLAGS = $(INCLUDE) -Wall -Wextra -Wshadow -g

LDFLAGS = -lm -lbz2 -lva -lva-drm -lva-x11 -lX11 -llzma -lvdpau -L ../cmake-build-debug -ltranscoder

NAME = ts

CC = gcc

all: build

build: $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
