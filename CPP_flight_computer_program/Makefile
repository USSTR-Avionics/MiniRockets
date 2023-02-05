.PHONY: *

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
	@echo "make clean"
	@echo "    Clean the project dependencies."
	@echo "make upload"
	@echo "    Upload the project to the device."
	@echo "make build"
	@echo "    Build the project."

tidy:
	clang-tidy-15 --config-file=.clang-tidy src/* include/*

verify:
	clang-tidy-15 --config-file=.clang-tidy --verify-config

format:
	clang-format-15 -i -style=file ./src/* ./include/*

check:
	clang-format-15 --dry-run --Werror ./src/* ./include/*

preview:
	clang-format-15 -style=file ./src/* ./include/*

clean:
	pio run -t clean

upload:
	pio run -t upload

build:
	pio run

hook:
	git config core.hooksPath /.githooks
	chmod +x ../.githooks
