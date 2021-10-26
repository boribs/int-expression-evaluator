test:
	@gcc src/cal.c tests/cal_tests.c tests/unity/unity.c -o out
	@./out

build:
	@gcc src/cal.c src/main.c -o out

run:
	@./out

br: build run

clear:
	@rm out
