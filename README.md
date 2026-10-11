# Spectra

Spectra is a C++/Qt desktop research and homebrew-organizing toolkit for PlayStation 5 development research.

> **Current status:** early prototype. Spectra does not currently jailbreak a PS5, inject code into a console, or bypass PlayStation security. No public method is assumed to work on the latest firmware.

## Initial goals

- Provide a desktop UI for browsing files and removable drives.
- Let developers inspect homebrew project files and package metadata before use.
- Keep an auditable session log of user actions and validation results.
- Offer a **simulation mode** for testing workflows without modifying a console or deploying exploit payloads.
- Track firmware research using public, responsibly disclosed sources.

## Safety and scope

The initial release is a host-side organizer and research UI, not a jailbreak injector. It will not exploit the console, bypass platform security, install unauthorized payloads, or claim compatibility with firmware that has not been tested. Any future console-side development should use documented development pathways and devices the developer is authorized to test.

## Proposed structure

```text
Spectra/
├── README.md
├── CMakeLists.txt
├── LICENSE
├── docs/
│   ├── architecture.md
│   └── development.md
├── src/
│   ├── main.cpp
│   ├── MainWindow.h
│   └── MainWindow.cpp
└── tests/
    └── README.md
```

## Technology

- C++17 or newer
- Qt 6 Widgets for the desktop interface
- CMake for builds
- Qt logging plus a visible, timestamped activity panel

## First milestone

Build a desktop shell with a file/folder chooser, a removable-drive list, package metadata inspection, a timestamped activity log, and a simulation-only workflow. The UI must clearly distinguish selecting files from installing or executing them.

## Build

A Qt 6 development environment and CMake are required. Build instructions will be added with the first compilable UI prototype.

## Contributions

Please document tested operating systems, reproducible steps, and limitations. Do not submit exploit payloads, credential theft, malware, or claims of firmware support without verifiable evidence.
