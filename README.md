| Platform | Build Status                                                                                                                                                                                                                                        |
|----------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| macOS    | [![macOS build status](https://github.com/TVPaint-Developpement/TVPaintAnimationSDK/actions/workflows/macos-build.yml/badge.svg?branch=main)](https://github.com/TVPaint-Developpement/TVPaintAnimationSDK/actions/workflows/macos-build.yml)       |
| Linux    | [![Linux build status](https://github.com/TVPaint-Developpement/TVPaintAnimationSDK/actions/workflows/linux-build.yml/badge.svg?branch=main)](https://github.com/TVPaint-Developpement/TVPaintAnimationSDK/actions/workflows/linux-build.yml)       |
| Windows  | [![Windows build status](https://github.com/TVPaint-Developpement/TVPaintAnimationSDK/actions/workflows/windows-build.yml/badge.svg?branch=main)](https://github.com/TVPaint-Developpement/TVPaintAnimationSDK/actions/workflows/windows-build.yml) |

# TVPaint Animation SDK

The TVPaint Animation SDK provides the tools and libraries needed to develop plugins for TVPaint Animation software. It consists of two complementary SDK layers:

- **C SDK** (`sdk/`): Core C-based API with comprehensive functionality for plugin development
- **C++ SDK** (`sdk-cpp/`): Modern C++20 object-oriented wrapper providing high-level classes for easier plugin development

Both SDKs are built as static libraries and can be used independently or together.

## Table of Contents

- [Requirements](#requirements)
- [Quick Start](#quick-start)
- [Build Options](#build-options)
- [Installation Structure](#installation-structure)
- [Plugin Creation](#plugin-creation)
- [Examples](#examples)
- [Tools](#tools)
- [Documentation](#documentation)
- [Deprecated API](#deprecated-api)
- [License](#license)

## Requirements

- **CMake** 3.25 or higher
- **Ninja** (or an equivalent, such as Make)
- **C compiler** compatible with your platform (Windows, Linux, or macOS)
- **C++20 compiler** (only required if building the C++ SDK or C++ examples)

## Quick Start

```bash
# 1. Create and enter build directory
mkdir build
cd build

# 2. Configure the project
cmake .. -G Ninja

# 3. Build the SDK
ninja

# 4. Install the SDK (may require sudo on Linux/macOS)
ninja install

# 5. (Optional) Uninstall the SDK
ninja uninstall
```

## Build Options

### SDK Configuration

- **`TVPASDK_BUILD_C_SDK`** (default: `ON`)
  Build the C SDK static library

- **`TVPASDK_BUILD_CPP_SDK`** (default: `ON`)
  Build the C++ wrapper SDK static library (requires C SDK)

- **`TVPASDK_BUILD_EXAMPLE`** (default: `OFF`)
  Build example plugins (PI_Flip, PI_WaveForm, PI_Fade, PI_Demo)

### Custom Installation Path

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/installation -G Ninja
```

### Example Configurations

```bash
# Build only the C SDK
cmake .. -DTVPASDK_BUILD_CPP_SDK=OFF -G Ninja

# Build both SDKs with examples
cmake .. -DTVPASDK_BUILD_EXAMPLE=ON -G Ninja

# Build and install to custom location
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/a/custom/location/tvpaint-sdk -G Ninja
ninja install
```

## Installation Structure

After running `ninja install`, the SDK is organized as follows:

```
<InstallPrefix>/
├── include/
│   └── TVPaintAnimationSDK/
│       ├── TVPaintSDK.h                    # C SDK master include
│       ├── pi-basics.h                     # C SDK headers (.h, 20 total)
│       ├── ...
│       ├── TVPaintSDK.hpp                  # C++ SDK master include
│       ├── Plugin.hpp                      # C++ SDK headers (.hpp, 24 total)
│       ├── Button-*.hpp                    # 16 specialized button types
│       └── ...
│
└── lib/
    ├── cmake/
    │   ├── TVPaintAnimationSDK/
    │   │   ├── TVPaintAnimationSDKConfig.cmake
    │   │   ├── TVPaintAnimationSDKConfigVersion.cmake
    │   │   ├── TVPaintAnimationSDKTargets.cmake
    │   │   └── TVPaintAnimationSDKUtilities.cmake    # Utility functions
    │   └── TVPaintAnimationSDK-CPP/
    │       ├── TVPaintAnimationSDK-CPPConfig.cmake
    │       ├── TVPaintAnimationSDK-CPPConfigVersion.cmake
    │       └── TVPaintAnimationSDK-CPPTargets.cmake
    ├── pkgconfig/
    │   ├── TVPaintAnimationSDK.pc
    │   └── TVPaintAnimationSDK-CPP.pc
    ├── libTVPaintAnimationSDK.(a|so|dll)      # C SDK static library
    └── libTVPaintAnimationSDK-CPP.(a|so|dll)  # C++ SDK static library
```

## Plugin Creation

TVPaint plugins are built as dynamic libraries with a `.plugin` bundle structure. The SDK provides two CMake utility functions, both available after calling `find_package(TVPaintAnimationSDK)`:

### CMake Utility Functions

#### `create_plugin_bundle( TARGET_NAME )`

Creates the platform-specific `.plugin` bundle structure after the build. The bundle name automatically includes the OS suffix:

| Platform | Output                                                      |
|----------|-------------------------------------------------------------|
| macOS    | `PluginName-MacOS.plugin/Contents/MacOS/PluginName`         |
| Windows  | `PluginName-Windows.plugin/Contents/Windows/PluginName.dll` |
| Linux    | `PluginName-Linux.plugin/Contents/Linux/libPluginName.so`   |

On Windows (MSVC), this function automatically links the CRT statically (`/MT`/`/MTd`) so the plugin loads on machines without the Visual C++ Redistributable installed. This matches the CRT used by the SDK static libraries themselves.

Must be called before `copy_resources_to_bundle()`.

#### `copy_resources_to_bundle( TARGET_NAME  RESOURCES_DIR )`

Copies the contents of `RESOURCES_DIR` into the `Contents/Resources/` directory of the bundle. The source directory structure is preserved as-is, so organize your `Resources/` folder to match the expected bundle layout (language subdirectories such as `english/`, `french/`, etc.).

`create_plugin_bundle()` must be called first, as this function relies on the bundle name it sets.

### Bundle Structure

The resulting plugin bundle has the following layout:

```
MyPlugin-<OS>.plugin/
└── Contents/
    ├── MacOS/          # or Windows/ or Linux/
    │   └── MyPlugin    # binary (.dll on Windows, .so on Linux)
    └── Resources/      # optional, copied via copy_resources_to_bundle()
        ├── english/
        │   ├── strings.txt   # localization strings
        │   └── *.png         # optional images
        ├── french/
        └── ...               # chinese, japanese
```

**Resource file constraints:**
- Language directories must be among: `english`, `french`, `chinese`, `japanese`
- Each language directory may only contain `.png` files and a single `strings.txt` file

To install your plugin, copy the entire `.plugin` directory to TVPaint's plugin folder.

### C Plugin Example

This section walks through creating a minimal C plugin. See `examples/PI_Flip/` for a complete reference.

#### Project Structure

```
MyPlugin/
├── CMakeLists.txt
├── myplugin.c
├── myplugin.def          # Windows only: export symbols
└── Resources/
    ├── english/
    │   └── strings.txt
    └── french/
        └── strings.txt
```

#### CMakeLists.txt

```cmake
cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( MyPlugin LANGUAGES C )

find_package( TVPaintAnimationSDK REQUIRED )

add_library( ${PROJECT_NAME} MODULE
    myplugin.c
    $<$<PLATFORM_ID:Windows>:myplugin.def>
)

create_plugin_bundle( ${PROJECT_NAME} )
copy_resources_to_bundle( ${PROJECT_NAME}
                          ${CMAKE_CURRENT_SOURCE_DIR}/Resources )

target_include_directories( ${PROJECT_NAME} PRIVATE . )
target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK )
```

`create_plugin_bundle()` and `copy_resources_to_bundle()` are CMake utility functions provided by the SDK (available after `find_package`). See [CMake Utility Functions](#cmake-utility-functions) for details.

#### myplugin.def (Windows only)

On Windows, entry points must be explicitly exported via a `.def` file:

```
LIBRARY      MyPlugin
DESCRIPTION  'TVPaint plugin MyPlugin.DLL'

EXPORTS
  PI_Msg
  PI_Open
  PI_About
  PI_Parameters
  PI_Start
  PI_Work
  PI_Finish
  PI_Close
```

#### Building

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/sdk -G Ninja
ninja
```

Replace `/path/to/sdk` with the SDK install prefix (the directory passed to `CMAKE_INSTALL_PREFIX` when installing the SDK, e.g. `C:/TVPaintSDK` or `/usr/local`).

The resulting bundle will be in `build/MyPlugin-<OS>.plugin/`. Copy it to TVPaint's plugin folder to install it.

### C++ Plugin Example

This section walks through creating a minimal C++ plugin. See `examples/PI_Demo/` for a complete reference.

#### Project Structure

```
MyPlugin/
├── CMakeLists.txt
├── myplugin.cpp       # entry points + plugin class implementation
├── myplugin.h         # plugin and window class declarations
└── myplugin.def       # Windows only: export symbols
```

#### CMakeLists.txt

```cmake
cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( MyPlugin )

find_package( TVPaintAnimationSDK-CPP REQUIRED )

add_library( ${PROJECT_NAME} MODULE
    myplugin.cpp
    $<$<PLATFORM_ID:Windows>:myplugin.def>
)

target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_20 )

create_plugin_bundle( ${PROJECT_NAME} )

target_include_directories( ${PROJECT_NAME} PRIVATE . )
target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK-CPP )
```

Note: the C++ SDK uses `find_package( TVPaintAnimationSDK-CPP )` and the target `TVPaintAnimationSDK::TVPaintAnimationSDK-CPP`, as opposed to their C counterparts. `copy_resources_to_bundle()` is optional — omit it if your plugin has no resources.

#### myplugin.def (Windows only)

```
LIBRARY      MyPlugin
DESCRIPTION  'TVPaint plugin MyPlugin.DLL'

EXPORTS
  PI_Open
  PI_Msg
  PI_About
  PI_Parameters
  PI_Start
  PI_Work
  PI_Finish
  PI_Close
```

#### Building

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/sdk -G Ninja
ninja
```

Replace `/path/to/sdk` with the SDK install prefix (the directory passed to `CMAKE_INSTALL_PREFIX` when installing the SDK, e.g. `C:/TVPaintSDK` or `/usr/local`).

The resulting bundle will be in `build/MyPlugin-<OS>.plugin/`. Copy it to TVPaint's plugin folder to install it.

## Examples

The SDK includes example plugins demonstrating different features. Enable them with:

```bash
cmake .. -DTVPASDK_BUILD_EXAMPLE=ON -G Ninja
ninja
```

Built plugins will be in `build/examples/PluginName/`.

### PI_Flip (`examples/PI_Flip/`)

A simple C-based filter plugin that flips images horizontally or vertically.
- Demonstrates basic filter structure
- Shows PI_* callback functions
- Good starting point for C plugin development

### PI_WaveForm (`examples/PI_WaveForm/`)

An advanced C-based waveform monitor and vectorscope.
- Complex image analysis
- Custom GUI requesters
- Localization with `.loc` files
- Anti-aliased drawing utilities

### PI_Fade (`examples/PI_Fade/`)

A C++ filter plugin that fades the image to black using the C++ SDK.
- Demonstrates the C++ SDK with a real filter use case
- Shows key handling (`NeedKeys`, `CreateKeys`)
- Implements live preview via `MetaEventPreview`
- Uses `cTV2Window` with checkboxes and action buttons
- Good reference for C++ filter plugins with preview support

### PI_Demo (`examples/PI_Demo/`)

Comprehensive C++ example using the modern SDK wrapper.
- Multi-tab interface
- All button types (sliders, popups, text inputs, etc.)
- Preview windows
- Project API usage
- George scripting integration
- Meta-event handling
- **Best reference for modern C++ plugin development**

### SDK_Utilities (`examples/SDK_Utilities/`)

Reusable C utility helpers shared across C example plugins.
- `colorpicker`: Color selection and block generation
- `colorrange`: Color range handling

These are not standalone plugins but source files intended to be compiled into other plugins.

## Tools

### tvp_merge_plugin.py (`tools/`)

A Python 3.10+ tool to verify and merge TVPaint `.plugin` bundles. It supports two subcommands:
- **`verify`**: validate the structure of a single plugin bundle
- **`merge`**: merge multiple per-platform bundles into a single universal multi-platform bundle

```bash
# Validate a single bundle
python tools/tvp_merge_plugin.py verify PI_Flip-MacOS.plugin

# Merge per-platform bundles into a universal bundle
python tools/tvp_merge_plugin.py merge PI_Flip-Universal.plugin \
    PI_Flip-MacOS.plugin \
    PI_Flip-Windows.plugin \
    PI_Flip-Linux.plugin
```

See [`tools/README.md`](tools/README.md) for full documentation.

## Documentation

For detailed information about plugin development:

- **C SDK Headers**: Extensive inline documentation in `sdk/include/TVPaintAnimationSDK/`
- **C++ SDK Headers**: Class documentation in `sdk-cpp/include/TVPaintAnimationSDK/`
- **Example Plugins**: Real-world implementations in `examples/`

### Generating API Documentation

The SDK includes a Doxygen configuration file for generating comprehensive HTML documentation from the source code.

**Requirements:** Doxygen 1.9.0 or higher

```bash
# From the SDK root directory
doxygen Doxyfile

# Open the generated documentation
open docs/html/index.html        # macOS
xdg-open docs/html/index.html    # Linux
start docs/html/index.html       # Windows
```

## Deprecated API

The SDK preserves backward compatibility by keeping deprecated symbols alongside their modern replacements. Deprecated symbols emit compiler warnings (`[[deprecated]]`) in C++17+ and C23.

See [`DEPRECATED.md`](DEPRECATED.md) for the full list of deprecated symbols and their replacements, organized by header file.

## License

This project is licensed under the [MIT License](LICENSE).

Copyright (c) 2026 TVPaint Development. You are free to use, modify, distribute, and include this SDK in your own projects (including commercial ones), provided you retain the copyright notice and license text.

## Support

For questions and support, please visit the [TVPaint forums](https://forum.tvpaint.com) or contact [TVPaint Development Support](https://tvpaint.odoo.com/contactus-1).