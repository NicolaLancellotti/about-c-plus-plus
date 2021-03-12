.PHONY: configure
configure:
	cmake --preset debug

.PHONY: build
build:
	cmake --build ./build/debug

.PHONY: format
format:
	@find . -name "*.cpp" -or -name "*.h" | xargs clang-format -i

.PHONY: lint
lint:
	@find . -name "*.cpp" -or -name "*.h" | xargs clang-tidy
