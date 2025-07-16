#!/bin/bash
set -e
CMAKE_VERSION="3.31.0"
CMAKE_SH="cmake-${CMAKE_VERSION}-linux-x86_64.sh"
DOWNLOAD_URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${CMAKE_SH}"

# if cmake already installed and version matches, exit
if command -v cmake >/dev/null 2>&1; then
    INSTALLED_VERSION="$(cmake --version | head -n1 | awk '{print $3}')"
    if [ "$INSTALLED_VERSION" = "$CMAKE_VERSION" ]; then
        echo "CMake ${CMAKE_VERSION} already installed."
        exit 0
    fi
fi

# Download installer to temporary location
TMP_SCRIPT="/tmp/${CMAKE_SH}"
if [ ! -f "$TMP_SCRIPT" ]; then
    echo "Downloading CMake ${CMAKE_VERSION}..."
    wget "$DOWNLOAD_URL" -O "$TMP_SCRIPT"
fi

chmod +x "$TMP_SCRIPT"

# Run installer
sudo "$TMP_SCRIPT" --skip-license --prefix=/usr/local

# Refresh hash in case shell cached old cmake
hash -r

echo "CMake ${CMAKE_VERSION} installation complete."
