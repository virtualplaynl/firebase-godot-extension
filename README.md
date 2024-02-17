# Firebase Native GDExtension for Godot engine
# Source branch

This is a work in progress, currently testing:
- Authentication: all functions
- Firestore: all functions except Transactions
- Functions: all functions
- Cloud Messaging: with custom export template (swizzling doesn't work yet)
- Realtime Database: only connecting and disconnecting (e.g. for presence system)
- Testing on iOS, not yet on Android
- Also runs well in Godot Editor and desktop when feeding the google-services-desktop.json or google-services.json into the initialization

## Usage of the addon: see `main` branch

## Building

- Put SDK libraries in lib/[platform]
  - [platform] = 'android'/'ios'/'linux'/'macos'/'windows'/'web'
  - For windows, use 'Release' and 'Debug' folders from the MT variation (or MD if you want to use DLLs, this is untested)
- Put SDK includes in include/
- Use scons build scripts included per platform, or manually run scons if you don't want to overwrite the example lib.