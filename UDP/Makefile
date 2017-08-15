
PROJ_DIR := $(PWD)
DATA_TOSS_DIR=$(PROJ_DIR)/data_toss
DATA_PROCESS_DIR=$(PROJ_DIR)/data_process

SUBPROJS=toss process

all: $(SUBPROJS)

toss: $(DATA_TOSS_DIR)/data_toss.c $(DATA_TOSS_DIR)/data_toss.h
	cd $(DATA_TOSS_DIR) ; make mpi ; \
	ln -sfv $(DATA_TOSS_DIR)/data_toss $(PROJ_DIR)/toss ; \
	ln -sfv $(DATA_TOSS_DIR)/run_mpi.sh $(PROJ_DIR)/run_tosser

process: $(DATA_PROCESS_DIR)/data_process.c $(DATA_PROCESS_DIR)/data_process.h
	cd $(DATA_PROCESS_DIR) ; make release ;
	ln -sfv $(DATA_PROCESS_DIR)/data_process $(PROJ_DIR)/process

clean:
	rm -rfv $(SUBPROJS)
	rm -rfv ./run_tosser
	cd $(DATA_TOSS_DIR) ; make clean
	cd $(DATA_PROCESS_DIR) ; make clean
