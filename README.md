# Spectra

Spectra currently builds two separate Qt 6 desktop applications for Windows and Linux.

## Apps

### Spectra.exe — file utility

- Select local files and copy them to a destination folder or mounted volume.
- Validate readable files and destination writability.
- Track progress, log file operations, and export the activity log.
- Ask before replacing existing files.

This app performs normal filesystem operations only.

### Spectra Simulator.exe — console-inspired shell

- A dark blue, console-dashboard-inspired home screen.
- A Browser page with an address field and quick links. Websites open in the computer's default browser.
- A Store page with links to the official PlayStation Store, PlayStation Plus, and PlayStation Support websites.
- A Local Library shortcut to open the user's home folder.

**Important platform note:** the simulator is a Windows desktop UI inspired by a console home screen. It is not a PS5 emulator, does not run PS5 games or PS5 system software, and does not reproduce the PS5's built-in browser. A real PS5-native build requires access to Sony's authorized PlayStation development SDK and hardware; this repository does not currently contain a PS5 target.

The Store page is a set of official web links, not a native PlayStation Store client. The browser opens pages in the default Windows browser rather than embedding a browser engine.

## Build

Requirements: CMake 3.21+, a C++17 compiler, and Qt 6 Widgets (6.2+).

    cmake -S . -B build
    cmake --build build --config Release

The build produces Spectra and Spectra Simulator executables. GitHub Actions builds both applications for Windows and Linux and packages Qt runtime dependencies for Windows.

## Repository layout

    Spectra/
    ├── CMakeLists.txt
    ├── LICENSE
    ├── README.md
    ├── docs/
    │   └── development.md
    └── src/
        ├── main.cpp
        └── simulator.cpp

## PS5 platform research

See [docs/ps5-homebrew-research.md](docs/ps5-homebrew-research.md) for a high-level summary of public homebrew architecture and a realistic platform plan. The PS5 target remains future work; no native PS5 build is included.

## License

Spectra is released under the MIT License. See LICENSE.