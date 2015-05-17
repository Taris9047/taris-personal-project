PYIOPTIONS = -F -w
PYINST = pyinstaller
MAIN_FILE = TEdit.py
DIST_NAME = TEdit
DIST = ./dist
BUILD = ./build

freeze:
	$(PYINST) $(PYIOPTIONS) $(MAIN_FILE)

clean:
	rm -rfv $(BUILD) $(DIST) ./__pycache__

run:
	python ./TEdit.py

all:
	make run