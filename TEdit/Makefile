PYIOPTIONS = -F -w
PYINST = pyinstaller
MAIN_FILE = TEdit.py
DIST_NAME = TEdit
DIST = ./dist
BUILD = ./build

all:
	make run

freeze: TEdit.py MainWindow.py Find.py
	make clean
	$(PYINST) $(PYIOPTIONS) $(MAIN_FILE)

py2app: TEdit.py MainWindow.py Find.py setup.py
	make clean
	python setup.py py2app -A

clean:
	rm -rfv $(BUILD) $(DIST) ./__pycache__ *.conf *.pyc

run:
	python ./TEdit.py

