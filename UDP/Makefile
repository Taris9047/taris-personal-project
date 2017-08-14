
PROJ_DIR := $(PWD)
DATA_TOSS_DIR=$(PROJ_DIR)/data_toss
DATA_PROCESS_DIR=$(PROJ_DIR)/data_process

SUBPROJS=toss process

all: $(SUBPROJS)

toss: $(DATA_TOSS_DIR)/data_toss.c $(DATA_TOSS_DIR)/data_toss.h
	cd $(DATA_TOSS_DIR) ; make release ;
	cp -vfr $(DATA_TOSS_DIR)/data_toss $(PROJ_DIR)/toss

process: $(DATA_PROCESS_DIR)/data_process.c $(DATA_PROCESS_DIR)/data_process.h
	cd $(DATA_PROCESS_DIR) ; make release ;
	cp -vfr $(DATA_PROCESS_DIR)/data_process $(PROJ_DIR)/process

clean:
	rm -rfv $(SUBPROJS)
	cd $(DATA_TOSS_DIR) ; make clean
	cd $(DATA_PROCESS_DIR) ; make clean
