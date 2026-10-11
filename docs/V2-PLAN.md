# Spectra v2 development plan

## 1. Automatic file discovery
- Watch/scan `Documents/Spectra Imports` and show discovered files automatically.
- Provide Rescan and Open imports folder actions.
- Never copy files without an explicit confirmation.

## 2. Inject workflow
- Rename the primary Copy files action to Inject.
- Keep the behavior transparent: Inject means copying files to a user-selected local library/destination, not jailbreaking or injecting code into a real console.
- Show progress, skipped conflicts, errors, and an activity log.

## 3. PS5-inspired simulator
- Create a polished console-style home screen with focus states and app tiles.
- Add Settings, Media Gallery, Library, Store, and Browser sections.
- Add a selectable library of files copied into the local Spectra Library.

## 4. Built-in browser
- Use Qt WebEngine if available in the supported Qt build environment.
- Include address bar, back/forward, reload, home, and quick links.
- Keep browser-based pages inside the simulator rather than opening the external browser.

## 5. Media and settings
- Media Gallery should browse supported local image/video formats.
- Settings should expose practical simulator preferences such as theme and start page.
- Unsupported file types should be clearly identified; arbitrary files must not run silently.

## Acceptance checks
- No manual source-file picker is required for normal import discovery.
- Inject has a confirmation step and clear destination.
- Browser and other web-backed sections stay inside the simulator.
- Windows packaging includes any required Qt WebEngine resources and plugins.
