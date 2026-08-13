CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -g
SRCS    := $(wildcard src/*.c)
OBJS    := $(SRCS:src/%.c=build/%.o)
BIN     := bin/gen

.PHONY: all clean format release

all: $(BIN)

$(BIN): $(OBJS) | bin
	$(CC) $(CFLAGS) -o $@ $^

bin:
	mkdir -p bin

build:
	mkdir -p build

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BIN)

format:
	clang-format -i src/*.c

release:
	@test -n "$(VERSION)" || { echo "ERROR: specify VERSION (e.g. make release VERSION=v0.2.1)"; exit 1; }
	git cliff --unreleased -t $(VERSION) > CHANGELOG.md
	git add CHANGELOG.md
	git commit -m "docs: $(VERSION)"
	git tag -a $(VERSION) -m "release $(VERSION)"
	git push && git push --tags
	gh release create $(VERSION) --notes-file CHANGELOG.md
