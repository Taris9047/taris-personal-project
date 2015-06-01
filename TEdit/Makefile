PYIOPTIONS = -F -w
PYTHON = python3 -u 
PYINST = pyinstaller
MAIN_FILE = TEdit.pyw
DIST_NAME = TEdit
DIST = ./dist
BUILD = ./build

all:
	make run

freeze: $(MAIN_FILE) MainWindow.py Find.py
	make clean
	$(PYINST) $(PYIOPTIONS) $(MAIN_FILE)

py2app: $(MAIN_FILE) MainWindow.py Find.py setup.py
	make clean
	python setup.py py2app -A

clean:
	rm -rfv $(BUILD) $(DIST) ./__pycache__/ *.conf *.pyc ./.config/
	rm -rfv ~/.config/$(DIST_NAME).conf

run:
	$(PYTHON) ./$(MAIN_FILE)

