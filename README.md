# AD-Tornado

A Linux-only application that combines OpenFrameworks video playback with Chromium Embedded Framework (CEF) browser rendering in a single window.

## Features

- Fullscreen display with split layout
- Video playback in the bottom half
- Interactive HTML content in the top half
- Smooth animations and transitions
- Hardware-accelerated video rendering

## Requirements

- Linux x64 (tested on Ubuntu 20.04+)
- GCC 6 or later
- CMake 3.10 or later
- OpenGL 3.2+
- Git

## Setup

1. Clone the repository:

```bash
git clone https://github.com/yourusername/ad-tornado.git
cd ad-tornado
```

2. Download and setup dependencies:

```bash
chmod +x deps.sh
./deps.sh
```

3. Build OpenFrameworks:
   Follow the instructions in `deps/openFrameworks/INSTALL.md` to build OpenFrameworks for your system.

4. Build CEF:

```bash
cd deps/cef
cmake -B build .
cmake --build build
cd ../..
```

5. Build the application:

```bash
chmod +x build.sh run.sh
./build.sh
```

## Usage

Run the application with required HTML and video paths:

```bash
./run.sh
```

Or manually:

```bash
./build/bin/ad-tornado --html path/to/index.html --video path/to/video.mp4
```

### Command Line Arguments

- `--html <path>` : Path to HTML file (required)
- `--video <path>` : Path to video file (required)
- `--help` : Show usage information

## Development

The application structure:

- `src/` - Source files
- `include/` - Header files
- `assets/` - Default HTML and video files
- `deps/` - Dependencies (OpenFrameworks and CEF)

## License

This project is licensed under the MIT License - see the LICENSE file for details.
