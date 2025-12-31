# Makefile for code2snippet

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lstdc++fs
TARGET = code2snippet
SRCDIR = src
SRC = $(SRCDIR)/main.cpp
OBJ = $(SRCDIR)/main.o

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

debug: $(SRC)
	$(CXX) $(CXXFLAGS) -g -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	install -d /usr/local/bin
	install -m 755 $(TARGET) /usr/local/bin/$(TARGET)
	@echo "Installation complete!"

uninstall:
	@echo "Uninstalling $(TARGET) from /usr/local/bin..."
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstallation complete!"
