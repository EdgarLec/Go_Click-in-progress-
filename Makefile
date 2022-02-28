SRC = *.cpp

FLAGS = -Wall -Wextra

NAME = -o click_go

all:
	g++ $(FLAGS) $(NAME) $(SRC) -I ./ -lsfml-graphics -lsfml-window -lsfml-system -O3 -g
