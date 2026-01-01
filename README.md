# code2snippet

A C++ command-line tool that converts code files into VS Code snippets format (`.code-snippets`).

## Features

- Convert any text file to VS Code snippet format
- Auto-generate snippet name and prefix from filename
- Support for custom snippet name, prefix, and description
- Append mode to add snippets to existing files
- JSON output compatible with VS Code
- **NEW: Configuration file support for default settings**

## Requirements

- C++17 compatible compiler (g++ 8+, clang++ 7+)
- Autotools (autoconf, automake, libtool) - for building from source
- Make

## Building from Source

### Using the Autotools Build System (Recommended)

First, generate the configure script and Makefiles:

```bash
./autogen.sh
```

Then configure and build:

```bash
./configure
make
```

For a debug build:

```bash
./configure CXXFLAGS="-g -O0"
make
```

For custom installation prefix:

```bash
./configure --prefix=/usr/local
make
```

### Quick Install from Repository

```bash
./autogen.sh && ./configure && make
```

## Installation

Install system-wide:

```bash
sudo make install
```

By default, this installs to `/usr/local`. You can change this with `--prefix` during configure:

```bash
./configure --prefix=/usr
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
  -c, --config <path>      Path to custom configuration file
  --show-config            Show which config file is being used
  -h, --help               Show help message
```

## Configuration File

`code2snippet` supports configuration files to set default values for commonly used options. Configuration is searched in the following order (first match wins):

1. `.code2snippet.json` - Current directory
2. `.code2snippet/config.json` - Current directory
3. `~/.config/code2snippet/config.json` - User home directory
4. `/etc/code2snippet/config.json` - System-wide configuration

### Configuration File Format

```json
{
  "settings": {
    "defaultName": "my_snippet",
    "defaultPrefix": "myprefix",
    "defaultDescription": "My code snippet",
    "defaultOutputDir": "./snippets",
    "defaultAppend": false,
    "verbose": false
  }
}
```

Or use a flat format without the `settings` wrapper:

```json
{
  "defaultName": "my_snippet",
  "defaultAppend": true,
  "verbose": true
}
```

### Configuration Options

| Option | Type | Description |
|--------|------|-------------|
| `defaultName` | string | Default snippet name (can be overridden with `-n`) |
| `defaultPrefix` | string | Default snippet prefix (can be overridden with `-p`) |
| `defaultDescription` | string | Default snippet description (can be overridden with `-d`) |
| `defaultOutputDir` | string | Default directory for output files (relative paths are resolved) |
| `defaultAppend` | boolean | Whether to append to existing snippet files by default |
| `verbose` | boolean | Enable verbose output for debugging |

### Example Configuration Files

**Project-specific config** (`.code2snippet.json`):
```json
{
  "settings": {
    "defaultOutputDir": "./.vscode/snippets",
    "defaultAppend": true
  }
}
```

**User config** (`~/.config/code2snippet/config.json`):
```json
{
  "defaultName": "my_snippet",
  "verbose": true
}
```

An example configuration file is provided at `.code2snippet.json.example` in the repository.

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

### Using Configuration File

With `.code2snippet.json` in your directory:
```json
{
  "settings": {
    "defaultOutputDir": "./snippets",
    "defaultAppend": true
  }
}
```

Then simply run:
```bash
./code2snippet -i utils.cpp -o cpp.code-snippets
```

Show which config file is being used:
```bash
./code2snippet --show-config -i utils.cpp -o cpp.code-snippets
```

Use a custom config file:
```bash
./code2snippet -c /path/to/config.json -i utils.cpp -o cpp.code-snippets
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
│   ├── main.cpp      # Main source code
│   ├── config.h      # Configuration header
│   └── config.cpp    # Configuration parser implementation
├── configure.ac      # Autoconf configuration
├── Makefile.am       # Automake template (top-level)
├── src/Makefile.am   # Automake template (src)
├── autogen.sh        # Build system generation script
├── .gitignore        # Git ignore patterns
├── .code2snippet.json.example  # Example configuration file
└── README.md         # This file
```
