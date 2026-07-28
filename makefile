# Variables
SHELL = pwsh.exe
.SHELLFLAGS = -NoProfile -Command
export TARGET = main.exe
export CC = g++
export DEBUGGER = gdb
export DEBUG_TARGET = debug
export SRC = src/*.cpp
export OBJ = bin/*.o
export CFLAGS = -std=c++20 -Wall -Wextra -Icore
export LIBS = -lmingw32 -lSDL2main -lSDL2
export NULL = logs/null.log

# Compiling
$(TARGET): $(OBJ)
	@$(CC) $^ $(CFLAGS) $(LIBS) -o $@ > $(NULL)

$(OBJ): $(SRC)
	@echo "[INFO] Compiling source files into object files."
	@$(CC) -c $^ $(CFLAGS) > $(NULL) && echo "[INFO] Object files compiled successfully!"
	@move *.o bin/ && echo "[INFO] Object files moved to bin directory."

# Make Commands

.PHONY: build run debug clean

build:
	@make --no-print-directory && echo "[INFO] Project built successfully!" || echo "[ERROR] Failed to build the project!"

debug: $(SRC)
	$(CC) -g $^ $(CFLAGS) $(LIBS) -o $(DEBUG_TARGET)
	$(DEBUGGER) $(DEBUG_TARGET)

run: 
	@make build --no-print-directory
	@echo "[INFO] running ${TARGET}..."
	@./$(TARGET) && echo "[INFO] ${TARGET} exited normally." || echo "[WARNING] ${TARGET} exited with error code!"
	@make clean --no-print-directory 2> $(NULL) || echo ""

clean:
	@rm $(TARGET)
	@rm $(DEBUG_TARGET).exe
	@rm $(OBJ)

