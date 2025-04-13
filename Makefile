# Makefile scris de un student anul I - Task 1: teste 1-5, Task 2: teste 6-10
CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = task1.c
OUT = a.out

INPUT_DIR = ./InputData
OUTPUT_DIR = ./out
REF_DIR = ./RefData

.PHONY: all run clean check1 check2

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: all
	@echo "Rulare exemplu: Task 1 pe data1"
	./$(OUT) $(INPUT_DIR)/data1.in $(OUTPUT_DIR)/data1.out

check1: all
	@echo "=== Verific Task 1 (T = 1) ==="
	@mkdir -p $(OUTPUT_DIR)
	@for i in $(shell seq 1 5); do \
		infile="$(INPUT_DIR)/data$$i.in"; \
		outfile="$(OUTPUT_DIR)/data$$i.out"; \
		reffile="$(REF_DIR)/data$$i.ref"; \
		if [ -f $$infile ]; then \
			echo "Test $$i:"; \
			./$(OUT) $$infile $$outfile; \
			if [ -f $$reffile ]; then \
				if diff -q -Z $$outfile $$reffile > /dev/null; then \
					echo "✔ Test $$i OK"; \
				else \
					echo "✘ Test $$i DIFFERENT"; \
				fi; \
			else \
				echo "⚠ Ref file missing for test $$i"; \
			fi; \
		fi; \
	done
	@echo "=== Verificare Task 1 completă ==="

check2: all
	@echo "=== Verific Task 2 (T = 2) ==="
	@mkdir -p $(OUTPUT_DIR)
	@for i in $(shell seq 6 10); do \
		infile="$(INPUT_DIR)/data$$i.in"; \
		outfile="$(OUTPUT_DIR)/data$$i.out"; \
		reffile="$(REF_DIR)/data$$i.ref"; \
		if [ -f $$infile ]; then \
			echo "Test $$i:"; \
			./$(OUT) $$infile $$outfile; \
			if [ -f $$reffile ]; then \
				if diff -q -Z $$outfile $$reffile > /dev/null; then \
					echo "✔ Test $$i OK"; \
				else \
					echo "✘ Test $$i DIFFERENT"; \
				fi; \
			else \
				echo "⚠ Ref file missing for test $$i"; \
			fi; \
		fi; \
	done
	@echo "=== Verificare Task 2 completă ==="

clean:
	rm -f $(OUT) $(OUTPUT_DIR)/*.out
