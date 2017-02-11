#PYTHON=/usr/local/bin/python
PYTHON=python

all:
#	$(PYTHON) ./setup.py build
	pyinstaller --onefile ./Benchmark.py
	pyinstaller --onefile ./monty.py
	pyinstaller --onefile ./Torture.py

bench:
	$(PYTHON) ./Benchmark.py

torture:
	$(PYTHON) ./Torture.py

monty:
	$(PYTHON) ./monty.py

clean:
	rm -rfv *.pyc
	rm -rfv *.txt
	rm -rfv *.log
	rm -rfv ./build
	rm -rfv ./dist

