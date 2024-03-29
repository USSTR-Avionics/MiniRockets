.PHONY: *

PORT=/dev/ttyACM0
BAUD=9600

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "make help"
	@echo "    Show this help message."
	@echo "make tidy"
	@echo "    Run clang-tidy on all source files."
	@echo "make verify"
	@echo "    Verify the configuration file for clang-tidy."
	@echo "make format"
	@echo "    Format all source files in-place."
	@echo "make check"
	@echo "    Check that all source files are formatted correctly."
	@echo "make preview"
	@echo "    Preview the changes that would be made by make format."
	@echo "make compiledb"
	@echo "    Generate a compile_commands.json file for clangd."
	@echo "make clean"
	@echo "    Clean the project dependencies."
	@echo "make upload"
	@echo "    Upload the project to the device."
	@echo "make monitor"
	@echo "    Monitor the device serial port."
	@echo "make build"
	@echo "    Build the project."

tidy:
	clang-tidy-15 --config-file=.clang-tidy src/*.cpp include/*.h

verify:
	clang-tidy-15 --config-file=.clang-tidy --verify-config

format:
	clang-format-15 -i -style=file ./src/* ./include/*

check:
	clang-format-15 --dry-run --Werror ./src/* ./include/*

preview:
	clang-format-15 -style=file ./src/* ./include/*

compiledb:
	pio run -t compiledb

clean:
	pio run -t clean

upload:
	pio run -t upload

monitor:
	pio device monitor -p $(PORT) -b $(BAUD)

build:
	pio run

