# Stealify

This is a small project to download songs from youtube and then play them back.

It uses [youtube-dl](https://github.com/ytdl-org/youtube-dl) to download the video and then converts it using [ffmpeg](https://ffmpeg.org/). <br>

The `Scripts/` directory contains build scripts for Windows and Linux, and the `Vendor/` directory contains Premake binaries (currently version `5.0-beta2`).

## Prerequisites

[Visual Studio 2010 (VC++ 10.0) SP1](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#visual-studio-2010-vc-100-sp1-no-longer-supported) <br>
(Optional) [ffmpeg](https://ffmpeg.org/) installed in `"C:\\FFmpeg\\ffmpeg\\bin\\ffmpeg.exe"` (or simply edit the `checkFFMPEGInstallation` method in `Backend/Backend.cpp`)

## Getting Started
1. Clone this repository
2. `Backend` is where the magic happens and `Frontend` will be used for a GUI in future versions
3. Open the `Scripts/` directory and run the appropriate `Setup` script to generate projects files. You can edit the setup scripts to change the type of project that is generated - out of the box they are set to Visual Studio 2022 for Windows and gmake2 for Linux.

Note that no macOS setup script is currently provided; you can duplicate the Linux script and adjust accordingly.

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)
