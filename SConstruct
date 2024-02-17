#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
# env.Append(CPPPATH=["src/"])

if env["platform"] == "ios":
    env.Append(CCFLAGS=[
        "-Iinclude"
    ])
    env.Append(LINKFLAGS=[
        '-ObjC',
        '-Flib/ios',
        '-framework', 'Firebase',
        '-framework', 'FirebaseAuth',
        '-framework', 'FirebaseDatabase',
        '-framework', 'FirebaseFirestore',
        '-framework', 'FirebaseFunctions',
        '-framework', 'FirebaseMessaging',
        '-framework', 'FirebaseStorage',
        '-l', 'sqlite3',
        '-l', 'z'
    ])
elif env["platform"] == "macos":
    env.Append(CCFLAGS=[
        "-Iinclude",
    ])
    env.Append(LINKFLAGS=[
        '-Flib/macos',
        '-framework', 'firebase',
        '-framework', 'firebase_analytics',
        '-framework', 'firebase_auth',
        '-framework', 'firebase_database',
        '-framework', 'firebase_firestore',
        '-framework', 'firebase_functions',
        '-framework', 'firebase_messaging',
        '-framework', 'firebase_remote_config',
        '-framework', 'firebase_storage',
        '-l', 'sqlite3',
        '-l', 'z'
    ])
elif env["platform"] == "android":
    env.Append(LIBPATH=["lib/android"])
    env.Append(LIBS=["firebasedat"])
elif env["platform"] == "windows":
    env.Append(CCFLAGS=[
        "-Iinclude",
    ])
    env.Append(LIBPATH=["lib/windows/Release"])
    env.Append(LIBS=[
        'firebase_analytics',
        'firebase_app',
        'firebase_auth',
        'firebase_database',
        'firebase_firestore',
        'firebase_functions',
        'firebase_messaging',
        'firebase_remote_config',
        'firebase_storage',
        'advapi32', 'ws2_32', 'crypt32',
        'rpcrt4', 'ole32', 'shell32',
        'iphlpapi', 'psapi', 'userenv',
        'user32', 'dbghelp', 'bcrypt'
    ])

sources = [
    "src/register_types.cpp",
    "src/conversion.cpp",
    "src/GodotFirebaseObjects.cpp",
    "src/GodotFirebase.cpp",
    "src/GodotFirebaseAnalytics.cpp",
    "src/GodotFirebaseAuth.cpp",
    "src/GodotFirebaseDatabase.cpp",
    "src/GodotFirestore.cpp",
    "src/FirestoreBatch.cpp",
    "src/FirestoreFilter.cpp",
    "src/FirestoreListener.cpp",
    "src/FirestoreQuery.cpp",
    "src/FirestoreTransaction.cpp",
    "src/GodotFirebaseFunctions.cpp",
    "src/FirebaseFunction.cpp",
    "src/GodotFirebaseMessaging.cpp",
    "src/GodotFirebaseRemoteConfig.cpp",
    "src/GodotFirebaseStorage.cpp"
]

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "lib/libgodotfirebase.{}.{}.framework/libgodotfirebase.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "lib/libgodotfirebase.{}-{}.{}.simulator.a".format(
                env["platform"], env["arch"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "lib/libgodotfirebase.{}-{}.{}.a".format(
                env["platform"], env["arch"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "lib/libgodotfirebase{}{}".format(
            env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
