# Development guide

## Prerequisites

- CMake 3.21 or newer
- A C++17 compiler
- Qt 6 Widgets development libraries (Qt 6.2 or newer)

## Configure and build

From the repository root:

```sh
cmake -S . -B build
cmake --build build --config Release
```

Run the generated `Spectra` executable from the build directory (the exact path depends on your operating system and generator).

## Current behavior

The desktop app can:

- Select local files and remove selected entries.
- Choose an existing destination folder or mounted volume.
- Validate source files and destination writability.
- Copy selected files to the chosen destination, showing progress and timestamped log entries.
- Prompt before replacing an existing file, with options to replace, skip, or cancel.
- Export the activity log as a text file.

Copying is a normal filesystem operation on the host computer or a mounted volume. Spectra does not connect to a PS5, inject code into a process or console, execute payloads, or bypass console security.

## Engineering notes

- File names are preserved when copying into the destination.
- Existing destination files are never replaced without explicit confirmation.
- Copy errors and skipped files are recorded in the activity log.
- Test on Windows, macOS, and Linux; document platform-specific behavior as it is verified.
