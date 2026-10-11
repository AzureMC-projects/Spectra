# Spectra Game Store package starter

This folder defines the first local package format for Spectra Game Store.

- `store-manifest.json` identifies the package and its demo catalogue.
- Run **Spectra Game Store Importer.exe**, select this folder as the source package, and select your Spectra version folder as the destination.
- The importer copies files into `<version folder>/Spectra Game Store/`. Existing installations are not overwritten.
- This is a local simulation-store scaffold. The current catalogue uses fictional demo entries and metadata; it does not include commercial game files or execute console packages.

The Windows build produces a separate `Spectra Game Store Importer.exe` target. PS5 support is intentionally out of scope for this first store milestone.
