# CEF + OpenFrameworks Video Player

An application that displays a web page with JS animation in the top half of the window and video in the bottom half.

## Requirements

- CMake 3.16 or higher
- C++17 compatible compiler
- CEF (Chromium Embedded Framework)
- OpenFrameworks
- GStreamer codecs:
  - gstreamer1.0-libav
  - gstreamer1.0-plugins-good
  - gstreamer1.0-plugins-bad

On Ubuntu/Debian systems, install the codecs with:

```bash
sudo apt-get install gstreamer1.0-libav gstreamer1.0-plugins-good gstreamer1.0-plugins-bad
```

## Project Structure

```
.
├── assets/             # Resources (HTML, video)
├── include/            # Header files
├── src/               # Source files
├── scripts/           # Helper scripts
└── CMakeLists.txt    # CMake configuration
```

## Building

1. Make sure CEF and OpenFrameworks are installed in the parent directory:

   ```
   parent_dir/
   ├── cef/
   ├── of/
   └── this_project/
   ```

2. Create a build directory and navigate to it:

   ```bash
   mkdir build
   cd build
   ```

3. Configure the project with CMake:

   ```bash
   cmake ..
   ```

4. Build:
   ```bash
   make
   ```

## Running

After successful build, run the application:

```bash
./bin/cef_of_player
```

## Controls

- F - toggle fullscreen mode
- ESC - exit application

## Known Issues

- First launch may take some time for CEF initialization
- In fullscreen mode, there might be a slight delay when updating the web page

## License

MIT
