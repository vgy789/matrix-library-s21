CC = gcc -std=c11

SRCDIR=src
BUILDDIR=build
BINDIR=bin

SRCS=$(SRCDIR)/s21_matrix.c $(SRCDIR)/matrix_utils.c $(SRCDIR)/matrix_operations.c
OBJS=$(BUILDDIR)/s21_matrix.o $(BUILDDIR)/matrix_utils.o $(BUILDDIR)/matrix_operations.o
TARGET=$(BINDIR)/s21_matrix.a
	
CFLAGS = -Wall -Werror -Wextra -pedantic
LDFLAGS = -lm
LDCHECKLIB = `pkg-config --cflags --libs check`
OPTFLAGS = -O2 -flto -march=native

VALGRIND_REPORT = ../valgrind_output.txt
TEST_RUNNER = run_tests.out
GCOV_REPORT_DIR = ../html_gcov_report

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $< -o $@ $(LDFLAGS)

clean:
	find . -name "*.out" -or -name "*.o" -or -name "*.gch" -or -name "*.gcno" -or -name "*.gcda" | xargs rm -f
	rm -f report.info $(TEST_RUNNER) $(VALGRIND_REPORT) $(TARGET)
	rm -rf $(GCOV_REPORT_DIR)/

rebuild: clean $(TARGET)

test: clean $(TARGET)
	checkmk clean_mode=1 tests/in > tests/tests.c && make fmt
	$(CC) $(CFLAGS) --coverage -o $(TEST_RUNNER) tests/tests.c $(TARGET) $(LDFLAGS) $(LDCHECKLIB)
	./$(TEST_RUNNER)

gcov_report: test
	lcov -c -t "$(TEST_RUNNER)" --directory ./ -o report.info --exclude *tests.c
	genhtml -o $(GCOV_REPORT_DIR) report.info


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
	valgrind --tool=memcheck --leak-check=yes --log-file=$(VALGRIND_REPORT) ./$(TEST_RUNNER)

# For BSD Unix
leaks:
	sed -i '' 's/CK_FORK/CK_NOFORK/g' tests/tests.c
	make test; \
	sed -i '' 's/CK_NOFORK/CK_FORK/g' tests/tests.c
	leaks --atExit -- ./$(TEST_RUNNER)

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .
