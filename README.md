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
  - [C Plugin Example](#c-plugin-example)
  - [C++ Plugin Example](#c-plugin-example-1)
- [Examples](#examples)
- [Tools](#tools)
- [Documentation](#documentation)
  - [Generating API Documentation](#generating-api-documentation)

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
│       ├── pi-basics.h                     # C SDK headers (.h)
│       ├── pi-main.h
│       ├── pi-filter.h
│       ├── ... (21 C headers total)
│       ├── TVPaintSDK.hpp                  # C++ SDK master include
│       ├── Plugin.hpp                      # C++ SDK headers (.hpp)
│       ├── Window.hpp
│       ├── Button.hpp
│       ├── Button-*.hpp                    # 16 specialized button types
│       └── ... (24 C++ headers total)
│
├── lib/
│   ├── cmake/
│   │   └── TVPaintAnimationSDK/
│   │       ├── TVPaintAnimationSDKConfig.cmake
│   │       ├── TVPaintAnimationSDKConfigVersion.cmake
│   │       ├── TVPaintAnimationSDKTargets.cmake
│   │       ├── TVPaintAnimationSDKUtilities.cmake    # Utility functions (create_plugin_bundle)
│   │       ├── TVPaintAnimationSDK-CPPConfig.cmake
│   │       ├── TVPaintAnimationSDK-CPPConfigVersion.cmake
│   │       └── TVPaintAnimationSDK-CPPTargets.cmake
│   ├── pkgconfig/
│   │   ├── TVPaintAnimationSDK.pc
│   │   └── TVPaintAnimationSDK-CPP.pc
│   ├── libTVPaintAnimationSDK.(a|so|dll)      # C SDK static library
│   └── libTVPaintAnimationSDK-CPP.(a|so|dll)  # C++ SDK static library
```

## Plugin Creation

TVPaint plugins are built as dynamic libraries with a `.plugin` bundle structure. The SDK provides two CMake utility functions, both available after calling `find_package(TVPaintAnimationSDK)`:

### CMake Utility Functions

#### `create_plugin_bundle( TARGET_NAME )`

Creates the platform-specific `.plugin` bundle structure after the build. The bundle name automatically includes the OS suffix:

| Platform | Output |
|----------|--------|
| macOS    | `PluginName-MacOS.plugin/Contents/MacOS/PluginName` |
| Windows  | `PluginName-Windows.plugin/Contents/Windows/PluginName.dll` |
| Linux    | `PluginName-Linux.plugin/Contents/Linux/libPluginName.so` |

Must be called before `copy_resources_to_bundle()`.

#### `copy_resources_to_bundle( TARGET_NAME  RESOURCES_DIR )`

Copies the contents of `RESOURCES_DIR` into the `Contents/Resources/` directory of the bundle. The source directory structure is preserved as-is, so organize your `Resources/` folder to match the expected bundle layout (language subdirectories such as `english/`, `french/`, etc.).

`create_plugin_bundle()` must be called first, as this function relies on the bundle name it sets.

### C Plugin Example

```cmake
cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( MyPlugin LANGUAGES C )

find_package( TVPaintAnimationSDK REQUIRED )

add_library( ${PROJECT_NAME} MODULE
    my_plugin.c
    $<$<PLATFORM_ID:Windows>:my_plugin.def>
)

create_plugin_bundle( ${PROJECT_NAME} )
copy_resources_to_bundle( ${PROJECT_NAME}
                          ${CMAKE_CURRENT_SOURCE_DIR}/Resources )

target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK )
```

**Minimal C plugin code (`my_plugin.c`):**

```c
#include "TVPaintAnimationSDK/TVPaintSDK.h"

// Plugin initialization
int PI_Open(void) {
    return 1;  // Success
}

// Plugin cleanup
void PI_Close(void) {
    // Cleanup code
}

// Show about dialog
void PI_About(void) {
    TVDisplayAbout("My Plugin\nVersion 1.0\n\nA simple plugin example");
}

// Main plugin entry point
int PI_Msg(int iMsg, int iOpt1, int iOpt2, long lOpt1, long lOpt2, void* pOpt) {
    switch(iMsg) {
        case PIMSG_OPEN:
            return PI_Open();
        case PIMSG_CLOSE:
            PI_Close();
            return 1;
        case PIMSG_ABOUT:
            PI_About();
            return 1;
        default:
            return 0;
    }
}
```

### C++ Plugin Example

```cmake
cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( MyCppPlugin LANGUAGES CXX )

find_package( TVPaintAnimationSDK-CPP REQUIRED )

add_library( ${PROJECT_NAME} MODULE
    my_cpp_plugin.cpp
    $<$<PLATFORM_ID:Windows>:my_cpp_plugin.def>
)

target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_20 )

create_plugin_bundle( ${PROJECT_NAME} )
copy_resources_to_bundle( ${PROJECT_NAME}
                          ${CMAKE_CURRENT_SOURCE_DIR}/Resources )

target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK-CPP )
```

**Minimal C++ plugin code (`my_cpp_plugin.cpp`):**

```cpp
#include "TVPaintAnimationSDK/TVPaintSDK.hpp"

using namespace nSDK;

class MyPlugin : public cTV2Plugin {
public:
    // Plugin identification
    const char* GetPluginName() const override {
        return "My C++ Plugin";
    }

    const char* GetPluginVersion() const override {
        return "1.0";
    }

    // Called when plugin is opened
    void PI2Open() override {
        // Initialize plugin
    }

    // Called when plugin is closed
    void PI2Close() override {
        // Cleanup
    }

    // Show about dialog
    void PI2About() override {
        TVDisplayAbout("My C++ Plugin\nVersion 1.0\n\nUsing C++ SDK");
    }
};

// Export the plugin instance
EXPORT_PLUGIN(MyPlugin)
```

### Building Your Plugin

```bash
mkdir build
cd build
cmake .. -G Ninja
ninja
```

The resulting plugin bundle will be created at:
- **macOS**: `build/MyPlugin.plugin/Contents/MacOS/`
- **Windows**: `build/MyPlugin.plugin/Contents/Windows/`
- **Linux**: `build/MyPlugin.plugin/Contents/Linux/`

To install your plugin, copy the entire `.plugin` directory to TVPaint's plugin folder.

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

### tvp-merge-plugin.py (`tools/`)

A Python tool to merge multiple per-platform `.plugin` bundles into a single universal multi-platform bundle. Useful when distributing a plugin that supports Windows, Linux, and macOS.

```bash
python tools/tvp-merge-plugin.py PI_Flip-Universal.plugin \
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

**Requirements:**
- Doxygen 1.9.0 or higher

**To generate the documentation:**

```bash
# From the SDK root directory
doxygen Doxyfile
```

This will generate:
- **HTML documentation** in `html/` directory
- Complete API reference for both C and C++ SDKs
- Class hierarchies, dependency graphs, and cross-referenced source code

**To view the documentation:**

```bash
# Open the generated documentation in your browser
open html/index.html        # macOS
xdg-open html/index.html    # Linux
start html/index.html       # Windows
```

The generated documentation includes:
- Detailed function and class descriptions
- Parameter documentation
- Return value specifications
- Code examples and usage notes
- Cross-references between related APIs

### Key Concepts

- **Plugin Lifecycle**: Plugins implement specific entry points (PI_Open, PI_Close, PI_Msg, etc.)
- **Filter Execution**: Sequence of Start → Work → Finish for frame processing
- **Multi-Threading**: Use `TVRunMultiThread()` to parallelize image processing
- **George Scripting**: Execute George commands via `TVSendCmd()` for automation
- **Localization**: Support multiple languages through `.loc` files

## License

See the LICENSE file for details.

## Support

For questions and support, please visit the [TVPaint forums](https://forum.tvpaint.com) or contact [TVPaint Development Support](https://tvpaint.odoo.com/contactus-1)