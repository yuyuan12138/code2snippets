# code2snippet

A C++ command-line tool that converts code files into VS Code snippets format (`.code-snippets`).

## Features

- Convert any text file to VS Code snippet format
- Auto-generate snippet name and prefix from filename
- Support for custom snippet name, prefix, and description
- Append mode to add snippets to existing files
- JSON output compatible with VS Code

## Requirements

- C++17 compatible compiler (g++, clang++)
- Make (optional, for building)

## Building

```bash
make
```

For debug build:
```bash
make debug
```

## Installation

Install system-wide:
```bash
sudo make install
```

Uninstall:
```bash
sudo make uninstall
```

## Usage

```
code2snippet [OPTIONS]

Convert code files to VS Code snippets format (.code-snippets)

Required:
  -i, --input <file>       Input code file path
  -o, --output <file>      Output snippets file path

Optional:
  -n, --name <name>        Custom snippet name (default: derived from filename)
  -p, --prefix <prefix>    Custom snippet prefix (default: derived from filename)
  -d, --description <desc> Custom snippet description (default: based on filename)
  -a, --append             Append to existing snippets file instead of overwriting
  -h, --help               Show help message
```

## Examples

### Basic Usage

Convert a C++ file to snippets:
```bash
./code2snippet -i utils.cpp -o my-snippets.code-snippets
```

### Custom Name and Prefix

```bash
./code2snippet -i script.py -o python.code-snippets -n myfunc -p mf
```

### Append to Existing Snippets

```bash
./code2snippet -i code.txt -o snippets.json -a
```

## Output Format

The tool generates VS Code compatible JSON format:

```json
{
  "SnippetName": {
    "prefix": "prefix",
    "body": [
      "line 1",
      "line 2"
    ],
    "description": "Description"
  }
}
```

## How Snippet Names Are Generated

By default, the tool derives snippet information from the input filename:

- **Name**: The filename stem (e.g., `utils.cpp` -> `utils`)
- **Prefix**: Lowercase name with underscores/hyphens replaced by spaces
- **Description**: "Code snippet from [name]"

These can be overridden using command-line options.

## Project Structure

```
code2snnipets/
├── src/
│   └── main.cpp      # Main source code
├── Makefile          # Build configuration
└── README.md         # This file
```
