# Firebase Native GDExtension for Godot engine. 

This is a work in progress, currently testing:
- Authentication: all functions
- Firestore: all functions except Transactions
- Functions: all functions
- Cloud Messaging: with custom export template (swizzling doesn't work yet)
- Realtime Database: only connecting and disconnecting (e.g. for presence system)
- Testing on iOS, not yet on Android
- Also runs well in Godot Editor and desktop when feeding the google-services-desktop.json or google-services.json into the initialization

## Native vs. addon
This native version of Firebase libraries, although a bit faster, is a lot less simple to install than the [GDScript version](https://github.com/GodotNuts/GodotFirebase), and it adds size to your built game.

So when would you need to use the native version instead of the GDScript implementation?
- To be able to use Cloud Messaging;
- To be able to use Listen on Firestore (that is not available in the REST API and the alternative, gRPC.NET, works but does not support iOS/Android/Web);
- If the GDScript version is too slow for you.

## Installation

These steps assume you have Firebase set up with a project.

- Choose:
  - If you're comfortable with Git submodules: Set up this repository (the the `main` branch) as a Git submodule in the `addons` directory in your Godot project root. (E.g. if your project root is your git root `git submodule add https://github.com/virtualplaynl/firebase-godot-extension.git addons/firebase`)
  - Otherwise: Download the latest release and add the `firebase` directory to `addons` in your Godot project root.

- Download the google-services.json (Android) and GoogleServices-Info.plist (iOS) from your Firebase project.

- If targeting desktop and for testing in the Editor (not fully supported by Firebase): Follow the instructions under the heading "Create a desktop version of the Firebase configuration file" to generate google-services-desktop.json.

- Optional, if this repo would not be up-to-date with the latest Firebase libs: Copy the libraries you need from the Firebase C++ SDK into the 'lib' directory of the extension:
  - For iOS: into lib/ios
  - For macOS: into lib/macos/libgodotfirebase.macos.debug.framework
  - For others: (WIP)

- Optional (untested): Edit firebase.gdextension: remove from the [dependencies] section all the libraries you don't need

## Usage

> [!WARNING]
> Incomplete documentation! This is a work in progress, don't use for production (unless you've worked more on this and tested, in that case please create a PR)

Check out Example.gd (GDScript) or GodotFirebaseExample.cs (C#) for now.

iOS, after exporting Xcode project:
- Add `https://github.com/firebase/firebase-ios-sdk` as a Package Dependency, removing all dependencies except FirebaseAnalytics, FirebaseAuth, FirebaseCrashlytics, FirebaseDatabase, FirebaseFirestore, FirebaseFunctions, FirebaseMessaging, FirebaseRemoteConfig and FirebaseStorage. Only enable the base packages, not the Swift or other variations.
- Add GoogleServices-Info.plist to project

## Building

- Checkout the [source](../../tree/source) branch, use instructions there.