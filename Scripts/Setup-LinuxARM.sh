#!/bin/bash

pushd ..
Vendor/Binaries/Premake/LinuxARM/premake5 --cc=clang --file=Build.lua gmake2
popd
