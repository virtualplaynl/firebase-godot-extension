#!/bin/sh

scons platform=macos target=$1 $2

rm -rf ./example/lib/macos/libgodotfirebase.macos.$1.framework
cp -r ./lib/libgodotfirebase.macos.$1.framework ./example/addons/firebase/lib/macos
