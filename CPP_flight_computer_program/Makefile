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
	@echo "make preview"
	@echo "    Preview the changes that would be made by make format."

tidy:
	clang-tidy-15 --config-file=.clang-tidy src/* include/*

verify:
	clang-tidy-15 --config-file=.clang-tidy --verify-config

format:
	clang-format-15 -i -style=file ./src/* ./include/*

preview:
	clang-format15 -style=file ./src/* ./include/*
