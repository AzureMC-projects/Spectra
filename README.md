# Spectra

Spectra contains two Qt 6 desktop applications for Windows and Linux, plus a separate experimental native PS5 homebrew test target.

## Apps

### Spectra.exe — file utility

- Select local files and copy them to a destination folder or mounted volume.
- Validate readable files and destination writability.
- Track progress, log file operations, and export the activity log.
- Ask before replacing existing files.

This app performs normal filesystem operations only.

### Spectra Simulator.exe — desktop console-inspired shell

- A dark blue, console-dashboard-inspired home screen.
- A Browser page with an address field and quick links. Websites open in the computer's default browser.
- A Store page with links to the official PlayStation Store, PlayStation Plus, and PlayStation Support websites.
- A Local Library shortcut to open the user's home folder.

This is a Windows/Linux desktop UI, not a PS5 emulator. Its Browser and Store features do not access PS5 system apps.

### Spectra PS5 — native homebrew development target

The separate [ps5/](ps5/) folder now contains an initial C++ native test screen and a GitHub Actions build workflow. It uses a pinned public community PS5 native-app toolchain to build a **development title folder** and ZIP. The first screen checks rendering and lays out Home, Browser, and Store panels; Browser and Store are placeholders, not working system applications.

**Artifact status:** the workflow has been added, but its first CI run has not yet been confirmed. No PS5 artifact is claimed as built until that workflow succeeds. The output is not a Sony-signed retail PKG/FPKG, and a successful host build does not prove it launches on a particular console. Running it requires a compatible PS5 homebrew environment/loader.

## Desktop build

Requirements: CMake 3.21+, a C++17 compiler, and Qt 6 Widgets (6.2+).

    cmake -S . -B build
    cmake --build build --config Release

GitHub Actions builds the desktop applications for Windows and Linux and packages Qt runtime dependencies for Windows.

## PS5 native build

See [ps5/README.md](ps5/README.md) for prerequisites, the pinned upstream toolchain, and local build instructions. The workflow is [Build Spectra PS5 native test app](.github/workflows/build-ps5.yml); it uploads a spectra-ps5-native-dev artifact when successful.

The PS5 path is separate from the Qt desktop build. It does not include an exploit, automatically configure a console, or deploy to hardware.

## Repository layout

    Spectra/
    ├── .github/workflows/
    │   ├── build.yml
    │   └── build-ps5.yml
    ├── docs/
    │   └── ps5-homebrew-research.md
    ├── ps5/
    │   ├── assets/banner.txt
    │   ├── src/main.cpp
    │   └── README.md
    └── src/
        ├── main.cpp
        └── simulator.cpp

## License

The existing Spectra desktop project is released under the MIT License. The PS5 build uses a separate third-party toolchain under its own license; see the upstream boilerplate repository for its terms.
