CC = gcc
SRCMODULES = s21_matrix.c matrix_utils.c matrix_operations.c
OBJMODULES = $(SRCMODULES:.c=.o)
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic
LDFLAGS = -lm
OPTFLAGS = -O2 -flto -march=native

TEST_EXEC = run_tests.out
REPORT_DIR = ./report

all: s21_matrix.a

rebuild: clean s21_matrix.a

test: clean s21_matrix.a
	checkmk clean_mode=1 tests/in > tests/tests.c && make fmt
	$(CC) $(CFLAGS) --coverage -o $(TEST_EXEC) tests/tests.c $(SRCMODULES) `pkg-config --cflags --libs check`
	./$(TEST_EXEC)

gcov_report: test
	lcov -c -t "$(TEST_EXEC)" --directory ./ -o report.info --exclude *tests.c
	genhtml -o $(REPORT_DIR) report.info

%.o: %.c %.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(OPTFLAGS) -c $< -o $@ 

s21_matrix.a: $(OBJMODULES)
	ar rcs $@ $^

clean:
	find . -name "*.out" -or -name "*.o" -or -name "*.gch" -or -name "*.gcno" -or -name "*.gcda" | xargs rm -f
	rm -f report.info $(TEST_EXEC) s21_matrix.a
	rm -rf $(REPORT_DIR)/


fmt_check fmt: CODE_STYLE = clang-format --style="{CommentPragmas: Insert, BasedOnStyle: Google}"
fmt_check fmt: FMT_FILES = find ./ -name '*.c' -print0 -or -name '*.h' -print0

fmt_check:
	$(FMT_FILES) | xargs -0 $(CODE_STYLE) -n 

fmt:
	$(FMT_FILES) | xargs -0 $(CODE_STYLE) -i


valgrind leaks: OPTFLAGS =
valgrind leaks: CFLAGS = -g

# For Linux
valgrind:
	sed -i 's/CK_FORK/CK_NOFORK/g' tests/tests.c
	make test; \
	sed -i 's/CK_NOFORK/CK_FORK/g' tests/tests.c
	valgrind --tool=memcheck --leak-check=yes ./$(TEST_EXEC)

# For BSD Unix
leaks:
	sed -i '' 's/CK_FORK/CK_NOFORK/g' tests/tests.c
	make test; \
	sed -i '' 's/CK_NOFORK/CK_FORK/g' tests/tests.c
	leaks --atExit -- ./$(TEST_EXEC)

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .
