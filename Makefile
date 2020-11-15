OBJ = helper.o movement.o
LD = -lSDL2 -lSDL2main -lSDL2_ttf
FLAGS = -Wall
NAME = Movement

$(NAME) : $(OBJ)
	gcc $(LD) $(FLAGS) -o $(NAME) $(OBJ)

movement.o: helper.h
helper.o: helper.h

.PHONY : clean dir test
clean:
	rm $(OBJ) $(NAME)
