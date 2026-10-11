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

## Prototype behavior

The current desktop shell can:

- Select local files for inspection.
- Select a folder or mounted USB volume as a recorded destination.
- Check whether selected files exist and are readable.
- Show timestamped activity messages in the UI.

The current prototype deliberately does **not** copy files, execute payloads, communicate with a PS5, modify console storage, or exploit/bypass firmware security. Destination selection is informational only.

## Next engineering tasks

1. Add unit tests for file metadata validation and log formatting.
2. Add removable-volume discovery with clear platform-specific permissions and labels.
3. Add a package manifest format and display hashes before any optional export workflow is designed.
4. Add persistent, user-controlled logs with rotation and a clear-data option.
5. Test the interface on Windows, macOS, and Linux; document which operating systems are supported.
