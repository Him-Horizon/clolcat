#!/bin/bash
# install.sh — Simple, zero-fuss installer for clolcat
# Author: Shubham Ramdeo (ramdeoshubham.com)

set -e

INSTALL_DIR="$HOME/bin"
mkdir -p "$INSTALL_DIR"

# Compile clolcat
echo "Compiling clolcat..."
gcc -o clolcat clolcat.c -lm
mv clolcat "$INSTALL_DIR"
chmod +x "$INSTALL_DIR/clolcat"

# Add ~/bin to PATH in shell config if not already
SHELL_RC="$HOME/.bashrc"
[ -n "$ZSH_VERSION" ] && SHELL_RC="$HOME/.zshrc"

if ! grep -q "$INSTALL_DIR" <<< "$PATH"; then
    echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >> "$SHELL_RC"
    echo "Added $INSTALL_DIR to PATH in $SHELL_RC"
    echo "It will be effective in new terminals automatically."
fi

# Temporarily update PATH for this script so clolcat works immediately
export PATH="$INSTALL_DIR:$PATH"

# Immediate rainbow output
echo "Installation complete! You can now run clolcat from anywhere." | clolcat

