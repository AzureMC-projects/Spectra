# PS5 Homebrew Research and Platform Plan

This note records high-level public research to guide future platform work. It is not an exploit guide, and Spectra does not implement a jailbreak, exploit chain, payload sender, or security bypass.

## What existing projects show

- **Firmware-specific homebrew:** Public PS5 HEN projects describe compatibility by firmware version and depend on console-specific entry points and payload formats. Compatibility is not universal, and instability is possible.
- **Native PS5 builds are a separate target:** The community PS5SDK describes itself as work in progress and is aimed at selected native payload/homebrew scenarios. It is not a drop-in compiler that makes a Qt Widgets Windows app run on a PS5.
- **A Windows port is not an emulator:** A Windows build can reproduce visual concepts and open public websites, but it cannot execute PS5 system software or native PS5 games merely by copying their files.
- **The Store and browser are platform services:** Linking to public PlayStation web pages is different from implementing Sony's native browser, sign-in, commerce, entitlements, or system integration.

## Proposed Spectra split

### Windows port

- Keep `Spectra.exe` as a host-side file utility.
- Keep `Spectra Simulator.exe` as a console-inspired desktop shell.
- Support normal web URLs through the user's default browser unless a dedicated, maintained browser engine is added deliberately.
- Detect file extensions and metadata without automatically executing unknown files.

### Future PS5-oriented target

- Treat this as a separate application target, not as another name for the Windows executable.
- First identify the actual target format, intended purpose, and lawful/authorized development environment.
- Use an authorized PlayStation development path for native application/system integration. If pursuing community homebrew, investigate the relevant public SDK's current scope and limitations before designing a target.
- Keep firmware and runtime compatibility explicit and experimental where applicable. Do not imply that a successful Windows build proves PS5 compatibility.

## Not implemented

This repository does not currently contain a PS5-native executable, PS5 system UI, native PS5 browser, native PlayStation Store client, emulator, exploit chain, or payload injector. The current Store screen opens public official websites, and the simulator UI runs on desktop Qt.

## Public references

- [PS5SDK — project README](https://github.com/PS5Dev/PS5SDK)
- [PS5 HEN — project README](https://github.com/cragson/ps5-hen)
- [PS5 Developer Wiki — Homebrew Enabler overview](https://www.psdevwiki.com/ps5/Homebrew_Enabler)
- [Sony Interactive Entertainment — developer program overview](https://sonyinteractive.com/en/news/blog/showing-your-game-to-playstation/)

These projects and compatibility details can change. Re-check upstream documentation before making implementation decisions.