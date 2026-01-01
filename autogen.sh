#!/bin/sh
# autogen.sh - Generate the build system for code2snippet

set -e

echo "Generating build system for code2snippet..."

# Create m4 directory if it doesn't exist
if [ ! -d "m4" ]; then
    mkdir m4
    echo "Created m4 directory"
fi

# Run autoreconf
autoreconf --install --verbose --force

echo ""
echo "================================================================"
echo "Build system generated successfully!"
echo ""
echo "To build the project, run:"
echo "  ./configure"
echo "  make"
echo ""
echo "For installation, run:"
echo "  sudo make install"
echo "================================================================"
