#!/bin/bash

pushd ..
vendor/bin/premake/Linux/premake5 --cc=clang --file=Build.lua gmake2
popd
