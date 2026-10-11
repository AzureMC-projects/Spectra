# Spectra

Spectra is a C++17 / Qt 6 desktop app for copying user-selected files into a destination folder or mounted volume.

## Features

- Select one or more local files and remove entries from the selection.
- Choose an existing destination folder or select a mounted volume.
- Validate selected files and destination writability.
- Copy files into the destination with progress, per-file logging, and a completion summary.
- Ask before replacing an existing destination file; users can skip it or cancel the copy.
- Export the timestamped activity log as a text file.

Spectra performs ordinary host filesystem copies only. It does not connect to a PS5, jailbreak a console, inject code into a process or console, execute payloads, or bypass platform security. Choosing a mounted drive simply chooses a filesystem destination.

## Build

Requirements: CMake 3.21+, a C++17 compiler, and Qt 6 Widgets (6.2+).

```sh
cmake -S . -B build
cmake --build build --config Release
```

Run the resulting `Spectra` executable from the build directory appropriate to your platform.

## Repository layout

```text
Spectra/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── docs/
│   └── development.md
└── src/
    └── main.cpp
```

## License

Spectra is released under the MIT License. See [LICENSE](LICENSE).
