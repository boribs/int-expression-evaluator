test:
	@gcc src/cal.c tests/cal_tests.c tests/unity/unity.c -o out
	@./out
	@rm out

t:
	@gcc src/cal.c -o out
	@./out
	@rm out
