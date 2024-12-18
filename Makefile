NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror #-fsanitize=thread #-g

HEADER_DIR = include
HEADER_FILES = $(HEADER_DIR)/philosophers.h 

SRC_DIR = src
SRC_FILES = $(SRC_DIR)/ft_error.c \
			$(SRC_DIR)/ft_free.c \
			$(SRC_DIR)/init.c \
			$(SRC_DIR)/main.c \
			$(SRC_DIR)/monitoring.c \
			$(SRC_DIR)/mutex_utils.c \
			$(SRC_DIR)/parsing.c \
			$(SRC_DIR)/sim_actions.c \
			$(SRC_DIR)/simulation.c \
			$(SRC_DIR)/utils.c \
			$(SRC_DIR)/utils2.c

OBJ_DIR = obj
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@
	@echo "$(GREEN)Compiling: $< into $@$(RESET)"

$(NAME): $(OBJ_FILES)
	@cc $(CFLAGS) -o $(NAME) $(OBJ_FILES)
	@echo "$(GREEN_LIGHT)Project successfully compiled. Target 'philo' created.$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(GOLD)Cleanup: Object files and binaries removed.$(RESET)"

fclean:	clean
	rm -f $(NAME)
	@echo "$(YELLOW)Cleanup: Build artifacts and temporary files removed.$(RESET)"

re: fclean all

exe:
	@./philo 199 650 200 200 9

rexe: re exe

.PHONY: all clean fclean re exe rexe

GREEN=\033[0;32m
GREEN_LIGHT=\033[1;32m
YELLOW=\033[0;33m
GOLD=\033[38;5;220m
RESET=\033[0m