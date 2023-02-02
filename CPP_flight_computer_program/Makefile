.PHONY: all


tidy:
	clang-tidy-15  --config-file=.clang-tidy src/* include/*

verify:
	clang-tidy-15  --config-file=.clang-tidy --verify-config

format:
	clang-format-15 -i -style=file ./src/*  ./include/*

preview:
	clang-format-15 -style=file ./src/*  ./include/*
