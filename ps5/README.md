# Spectra PS5 native test app

This folder contains Spectra's **separate native PS5 homebrew UI starter**. It is not the Qt Windows simulator.

## What the first milestone does

- Builds a native title folder using the public [PS5 Native App Boilerplate](https://github.com/blackbearreloaded/ps5-native-app-boilerplate).
- Draws a Spectra-branded home screen with three UI test panels.
- Marks Browser and Store as placeholders; this build does not open the console browser, connect to PlayStation Store, or use private system APIs.

The generated output is a development title folder and ZIP. It is **not** a Sony-signed retail PKG/FPKG. A compatible PS5 homebrew environment/loader is required to run it; the GitHub build alone does not install it on a console.

## Build locally (Linux or WSL)

Install the prerequisites listed in the upstream boilerplate README (Git, Make, Python 3, Clang 18, LLD 18, curl, wget, unzip, and related build tools). Then run:

```bash
git clone https://github.com/blackbearreloaded/ps5-native-app-boilerplate.git ../ps5-native-app-boilerplate
cd ../ps5-native-app-boilerplate
git checkout 4f531c4b517f80bcb6b1267135848168d2250047
cp ../Spectra/ps5/src/main.cpp src/main.cpp
make init TITLE_ID=PPSA94721 APP_NAME="Spectra PS5"
make
```

Adjust the path to your Spectra checkout if needed. The build uses upstream pinned dependency checks. The expected title folder is `dist/PPSA94721/`; the upstream build may also emit a ZIP in `dist/`.

## CI artifact

The workflow `.github/workflows/build-ps5.yml` runs the same host build and uploads the title folder/ZIP as a GitHub Actions artifact. A successful CI build proves host compilation and packaging only, **not** that the app launches on your specific console/firmware.

## Current limitations

- The first screen is a visual/render-loop smoke test; controller navigation and interaction are not implemented yet.
- Browser and Store panels are intentionally placeholders.
- Firmware/loader compatibility must be tested on the actual target.
- No exploit, console configuration, or deployment automation is included.
