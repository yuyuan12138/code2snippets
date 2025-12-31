#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

struct SnippetConfig {
    std::string name;
    std::string prefix;
    std::string description;
};

struct CommandLineArgs {
    std::string inputFile;
    std::string outputFile;
    std::optional<std::string> name;
    std::optional<std::string> prefix;
    std::optional<std::string> description;
    bool append = false;
};

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [OPTIONS]\n\n"
              << "Convert code files to VS Code snippets format (.code-snippets)\n\n"
              << "Required:\n"
              << "  -i, --input <file>       Input code file path\n"
              << "  -o, --output <file>      Output snippets file path\n\n"
              << "Optional:\n"
              << "  -n, --name <name>        Custom snippet name (default: derived from filename)\n"
              << "  -p, --prefix <prefix>    Custom snippet prefix (default: derived from filename)\n"
              << "  -d, --description <desc> Custom snippet description (default: based on filename)\n"
              << "  -a, --append             Append to existing snippets file instead of overwriting\n"
              << "  -h, --help               Show this help message\n\n"
              << "Examples:\n"
              << "  " << programName << " -i utils.cpp -o my-snippets.code-snippets\n"
              << "  " << programName << " -i script.py -o python.code-snippets -n myfunc -p mf\n"
              << "  " << programName << " -i code.txt -o snippets.json -a\n";
}

std::string getBaseName(const std::string& filepath) {
    fs::path path(filepath);
    std::string stem = path.stem().string();
    if (stem.empty()) {
        stem = "snippet";
    }
    return stem;
}

std::string escapeForJson(const std::string& str) {
    std::string result;
    result.reserve(str.size() * 1.2);
    for (char c : str) {
        switch (c) {
            case '\\': result += "\\\\"; break;
            case '"':  result += "\\\""; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (c < 32) {
                    std::ostringstream oss;
                    oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                    result += oss.str();
                } else {
                    result += c;
                }
                break;
        }
    }
    return result;
}

SnippetConfig generateConfigFromFilename(const std::string& filepath) {
    std::string baseName = getBaseName(filepath);

    // Generate prefix: lowercase, replace underscores/hyphens with spaces
    std::string prefix = baseName;
    for (char& c : prefix) {
        if (c == '_' || c == '-') {
            c = ' ';
        } else {
            c = std::tolower(c);
        }
    }

    // Remove extra spaces
    std::string cleanedPrefix;
    bool prevSpace = false;
    for (char c : prefix) {
        if (c == ' ') {
            if (!prevSpace) {
                cleanedPrefix += c;
            }
            prevSpace = true;
        } else {
            cleanedPrefix += c;
            prevSpace = false;
        }
    }

    return {
        baseName,
        cleanedPrefix,
        "Code snippet from " + baseName
    };
}

std::vector<std::string> readFileContent(const std::string& filepath) {
    std::vector<std::string> lines;
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Cannot open input file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    if (file.bad()) {
        throw std::runtime_error("Error reading file: " + filepath);
    }

    return lines;
}

std::string generateSnippetJson(const SnippetConfig& config, const std::vector<std::string>& contentLines) {
    std::ostringstream json;
    std::string escapedName = escapeForJson(config.name);
    std::string escapedPrefix = escapeForJson(config.prefix);
    std::string escapedDesc = escapeForJson(config.description);

    json << "  \"" << escapedName << "\": {\n";
    json << "    \"prefix\": \"" << escapedPrefix << "\",\n";
    json << "    \"body\": [\n";

    for (size_t i = 0; i < contentLines.size(); ++i) {
        json << "      \"" << escapeForJson(contentLines[i]) << "\"";
        if (i < contentLines.size() - 1) {
            json << ",";
        }
        json << "\n";
    }

    json << "    ],\n";
    json << "    \"description\": \"" << escapedDesc << "\"\n";
    json << "  }";

    return json.str();
}

bool parseCommandLine(int argc, char* argv[], CommandLineArgs& args) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            return false;
        } else if (arg == "-i" || arg == "--input") {
            if (++i >= argc) return false;
            args.inputFile = argv[i];
        } else if (arg == "-o" || arg == "--output") {
            if (++i >= argc) return false;
            args.outputFile = argv[i];
        } else if (arg == "-n" || arg == "--name") {
            if (++i >= argc) return false;
            args.name = argv[i];
        } else if (arg == "-p" || arg == "--prefix") {
            if (++i >= argc) return false;
            args.prefix = argv[i];
        } else if (arg == "-d" || arg == "--description") {
            if (++i >= argc) return false;
            args.description = argv[i];
        } else if (arg == "-a" || arg == "--append") {
            args.append = true;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return false;
        }
    }

    if (args.inputFile.empty() || args.outputFile.empty()) {
        std::cerr << "Error: Both --input and --output are required.\n\n";
        return false;
    }

    if (!fs::exists(args.inputFile)) {
        std::cerr << "Error: Input file does not exist: " << args.inputFile << "\n";
        return false;
    }

    return true;
}

std::string readExistingSnippets(const std::string& filepath) {
    if (!fs::exists(filepath)) {
        return "";
    }

    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + filepath);
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    return content;
}

std::string mergeSnippets(const std::string& existing, const std::string& newSnippet) {
    std::string trimmedExisting = existing;
    while (!trimmedExisting.empty() && std::isspace(trimmedExisting.back())) {
        trimmedExisting.pop_back();
    }

    if (trimmedExisting.empty()) {
        return "{\n" + newSnippet + "\n}\n";
    }

    // Remove trailing } if exists
    if (trimmedExisting.back() == '}') {
        trimmedExisting.pop_back();
        while (!trimmedExisting.empty() && std::isspace(trimmedExisting.back())) {
            trimmedExisting.pop_back();
        }
    }

    // Check if existing has content
    if (trimmedExisting.empty() || trimmedExisting == "{") {
        return "{\n" + newSnippet + "\n}\n";
    }

    return trimmedExisting + ",\n" + newSnippet + "\n}\n";
}

void writeSnippets(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file) {
        throw std::runtime_error("Cannot create output file: " + filepath);
    }
    file << content;
}

int main(int argc, char* argv[]) {
    CommandLineArgs args;

    if (!parseCommandLine(argc, argv, args)) {
        printUsage(argv[0]);
        return 1;
    }

    try {
        // Generate snippet config
        SnippetConfig config = generateConfigFromFilename(args.inputFile);

        // Override with command line options if provided
        if (args.name) config.name = *args.name;
        if (args.prefix) config.prefix = *args.prefix;
        if (args.description) config.description = *args.description;

        // Read file content
        std::vector<std::string> contentLines = readFileContent(args.inputFile);

        // Generate JSON snippet
        std::string snippetJson = generateSnippetJson(config, contentLines);

        // Write output
        if (args.append) {
            std::string existing = readExistingSnippets(args.outputFile);
            std::string merged = mergeSnippets(existing, snippetJson);
            writeSnippets(args.outputFile, merged);
        } else {
            writeSnippets(args.outputFile, "{\n" + snippetJson + "\n}\n");
        }

        std::cout << "Snippet created successfully!\n";
        std::cout << "  Name: " << config.name << "\n";
        std::cout << "  Prefix: " << config.prefix << "\n";
        std::cout << "  Output: " << args.outputFile << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
