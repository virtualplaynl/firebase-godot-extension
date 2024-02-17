#!/bin/sh

scons ios_simulator=yes platform=ios target=$1 $2
scons platform=ios target=$1 $2

rm -rf ./lib/libgodotfirebase.ios.$1.xcframework
rm -rf ./lib/libgodot-cpp.ios.$1.xcframework
xcodebuild -create-xcframework -library ./lib/libgodotfirebase.ios-universal.$1.a -library ./lib/libgodotfirebase.ios-universal.$1.simulator.a -output ./lib/libgodotfirebase.ios.$1.xcframework
xcodebuild -create-xcframework -library ./godot-cpp/bin/libgodot-cpp.ios.$1.universal.a -library ./godot-cpp/bin/libgodot-cpp.ios.$1.universal.simulator.a  -output ./lib/libgodot-cpp.ios.$1.xcframework

rm -rf ./example/addons/firebase/lib/ios/libgodotfirebase.ios.$1.xcframework
rm -rf ./example/addons/firebase/lib/ios/libgodot-cpp.ios.$1.xcframework
cp -r ./lib/libgodotfirebase.ios.$1.xcframework ./example/addons/firebase/lib/ios
cp -r ./lib/libgodot-cpp.ios.$1.xcframework ./example/addons/firebase/lib/ios
