PYIOPTIONS = -F -w
PYINST = pyinstaller
MAIN_FILE = TEdit.py
DIST = ./dist
BUILD = ./build

freeze: $(MAIN_FILE)
	$(PYINST) $(PYIOPTIONS) $(MAIN_FILE)

clean:
	rm -rfv $(BUILD) $(DIST)

run: $(DIST)
	python ./TEdit.py

all:
	make freeze
	make run