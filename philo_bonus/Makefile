NAME = philo_bonus

SRC_DIR = src/
OBJ_DIR = obj/
INC = includes/
CC = cc
CFLAGS = -Wall -Werror -Wextra
THREADFLAG = -pthread

SRC = $(SRC_DIR)observer.c $(SRC_DIR)philo_routine.c $(SRC_DIR)philos.c \
	$(SRC_DIR)init_struct.c $(SRC_DIR)utils.c $(SRC_DIR)philo_bonus.c \

OBJ = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC))

all : $(NAME)

$(NAME) : $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(THREADFLAG) -I $(INC) $(OBJ) -o $(NAME)

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(THREADFLAG) -g3 -I $(INC) -c $< -o $@

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re