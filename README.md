# Spectra

Spectra is a C++17 / Qt 6 desktop toolkit for organizing PS5 homebrew research files and recording a transparent, reproducible review workflow.

> **Prototype / simulation only:** Spectra does not exploit or jailbreak a PS5, inject code into a console, bypass platform security, or install payloads. The UI can inspect selected files, record a chosen folder or mounted drive, validate readable files, and export an activity log. It does not copy files to a destination or communicate with a console.

## Features

- Select one or more local files for review.
- Choose a destination folder, or select a currently mounted volume from the drive list.
- Validate that selected paths refer to readable regular files.
- Record timestamped actions in the on-screen activity log.
- Export the session log to a text file.
- Run a simulation that reports what was selected without modifying the destination or a console.
- Review public firmware/security research without claiming unverified compatibility.

## Build

Requirements: CMake 3.21+, a C++17 compiler, and Qt 6 Widgets (6.2+).

```sh
cmake -S . -B build
cmake --build build --config Release
```

Run the resulting `Spectra` executable from the build directory appropriate to your platform.

## Scope and responsible use

This is a host-side file organizer and research UI, not a jailbreak injector. Selecting a USB drive or storing files on it does not itself jailbreak a PS5. No console exploit, payload execution, security bypass, or unauthorized installation is implemented. Any future console-side work should use documented development pathways and hardware the developer is authorized to test.

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
