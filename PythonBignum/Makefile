PYTHON=python
CC=pyinstaller
CFLAGS=-F

all: test factorial

test: test.py
	$(CC) $(CFLAGS) $@.py

bench: factorial.py
	$(PYTHON) ./factorial.py
	evince ./factorial_comparison.eps

factorial: factorial.py
	$(CC) $(CFLAGS) $@.py

clean:
	rm -rfv ./build ./dist ./*.eps ./*.pyc ./*.txt ./__pycache__

run_test:
	$(PYTHON) ./test.py

run_factorial:
	$(PYTHON) ./factorial.py



