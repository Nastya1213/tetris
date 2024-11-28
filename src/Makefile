CC=gcc
CC_FLAGS=-Wall -Werror -Wextra
BUILD_DIR = build

.PHONY: all clean install

all: clean install

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

install: $(BUILD_DIR)/tetris_lib.a
	$(CC) $(FLAGS) -o $(BUILD_DIR)/tetris main.c gui/cli/interface.c help_func.c $(BUILD_DIR)/tetris_lib.a -lncurses

uninstall:
	-rm -f $(BUILD_DIR)/tetris

$(BUILD_DIR)/tetris.o: | $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -c brick_game/tetris/tetris.c -o $(BUILD_DIR)/tetris.o

$(BUILD_DIR)/tetris_lib.a: $(BUILD_DIR)/tetris.o
	ar rcs $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)/*.o
	ranlib $(BUILD_DIR)/tetris_lib.a

clang:
	clang-format --style=Google -n brick_game/tetris/*.c brick_game/tetris/*.h main.c gui/cli/*.c gui/cli/*.h tests/*.c

clean:
	rm -rf $(BUILD_DIR) *.gz test *.gcno *.gcda *.dSYM report a.out *.info

dvi:
	open info/tetris.md

test: clean $(BUILD_DIR)/tetris_lib.a
	$(CC) -g --coverage $(FLAGS) tests/tests.c -o $(BUILD_DIR)/test  $(BUILD_DIR)/tetris_lib.a \
		-I/opt/homebrew/Cellar/check/0.15.2/include \
		-L/opt/homebrew/Cellar/check/0.15.2/lib \
		-lcheck  # -lpthread -lrt -lm -lsubunit
	$(BUILD_DIR)/test

gcov_report: $(BUILD_DIR)/tetris_lib.a
	rm -f *.g*
	$(CC) $(FLAGS) brick_game/tetris/tetris.c tests/*.c $(BUILD_DIR)/tetris_lib.a -I/opt/homebrew/Cellar/check/0.15.2/include \
		-L/opt/homebrew/Cellar/check/0.15.2/lib \
		-lcheck --coverage -lncurses
	./a.out
	lcov -t ./a.out -o rep.info -c -d .
	genhtml -o report rep.info
	open ./report/index.html
	rm -rf *.gcda *.gcno

dist:
	@echo "Создание архива с исходниками..."
	tar -czf tetrisgame-dist.tar.gz $(SRC_DIR) Makefile
	@echo "Архив tetrisgame-dist.tar.gz успешно создан."