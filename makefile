# Variables
export TARGET = main
export CC = g++
export DEBUGGER = gdb
export DEBUG_TARGET = debug
export SRC = src/*.cpp
export OBJ = bin/*.o
export CFLAGS = -std=c++20 -Wall -Wextra -Icore
export LIBS = -lSDL2
export NULL = /dev/null

# Compiling
$(TARGET): $(OBJ)
	@$(CC) $^ $(CFLAGS) $(LIBS) -o $@ > $(NULL)

$(OBJ): $(SRC)
	@echo "[INFO] Compiling source files into object files."
	@$(CC) -c $^ $(CFLAGS) > $(NULL) && echo "[INFO] Object files compiled successfully!"
	@mv *.o bin/ && echo "[INFO] Moved object files to bin."

# Make Commands

.PHONY: build run debug clean

build:
	@make --no-print-directory && echo "[INFO] Project built successfully!" || echo "[ERROR] Failed to build the project!"

debug: $(SRC)
	$(CC) -g $^ $(CFLAGS) -o $(DEBUG_TARGET)
	$(DEBUGGER) $(DEBUG_TARGET)

run: 
	@make build --no-print-directory
	@echo "[INFO] running ${TARGET}..."
	@./$(TARGET) && echo "[INFO] ${TARGET} exited normally." || echo "[WARNING] ${TARGET} exited with error code!"
	@make clean > $(NULL) 2>&1 || echo ""

clean:
	@rm $(TARGET) $(DEBUG_TARGET) $(OBJ) > $(NULL) 2>&1 || echo ""

