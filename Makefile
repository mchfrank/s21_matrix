OS := $(shell uname -s)
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
TEST_LIBS = -lcheck -lm -lpthread -lsubunit
OBJECTS = s21_matrix.o
SOURSES = s21_matrix.c
GCOVFLAGS = -fprofile-arcs -ftest-coverage


ifeq ($(OS), Darwin)
 FLAGS_PLATFORM = -lcheck
else
 FLAGS_PLATFORM = -lcheck -lsubunit -lrt -lm -lpthread
endif

all: clean s21_matrix.a test

s21_matrix.a: s21_matrix.o
	@ar rcs s21_matrix.a s21_matrix.o
	ranlib s21_matrix.a

s21_matrix.o: s21_matrix.c
	$(CC) $(CFLAGS) -c s21_matrix.c -o s21_matrix.o

clean:
	rm -rf ./*.o ./*.a ./a.out gcov_test *.html *.css ./GcovReport ./*.gcno ./*.gcda ./report ./*.info ./s21_matrix.so ./*.dSYM ./s21_matrix_test test

test: test.c s21_matrix.a
	$(CC) $(CFLAGS) -c test.c $(FLAGS_PLATFORM) -o test.o
	$(CC) $(CFLAGS) -g test.o s21_matrix.a $(FLAGS_PLATFORM) -o test

gcov_report: s21_matrix.a
	$(CC) $(CFLAGS) --coverage test.c s21_matrix.c s21_matrix.a $(FLAGS_PLATFORM) -o gcov_test
	chmod +x *
	./gcov_test
	lcov -t test -o test.info -c -d .
	genhtml -o report test.info
	open ./report/index.html

check: s21_matrix.a
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c
	clang-format -n *.h
	cppcheck *.h *.c

rebuild: clean all

mini_grind:test
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./test

