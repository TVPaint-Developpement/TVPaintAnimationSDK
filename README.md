# TVPaint Animation SDK

This SDK provides the necessary tools and libraries to develop plugins for TVPaint Animation software. 
It includes a C-based API that allows developers to extend TVPaint Animation's functionality through custom plugins.

## Requirements

- CMake 3.25 or higher
- Ninja (or an equivalent, such as Make)
- C compiler compatible with your platform (Windows, Linux, or macOS)

# Compilation and Installation

Follow these steps to build and install the SDK:

1. Create a `build` directory.
2. Navigate into the `build` directory.
3. Configure the project with CMake using the following command: `cmake .. -G Ninja`

    If you want to install the library in a specific location, add the `CMAKE_INSTALL_PREFIX` option: `cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/installation -G Ninja`

4. To compile the examples, include the `TVPASDK_BUILD_EXAMPLE` option: `cmake .. -DTVPASDK_BUILD_EXAMPLE=ON -G Ninja`
5. To compile the SDK (and examples if enabled), run: `ninja`
6. To install the SDK, run: `ninja install`
7. To uninstall the SDK, run: `ninja uninstall`

## Examples

The example projects are located in: `build/examples/PluginName`

## Installation Structure

After installation, the SDK will be organized as follows:
```
InstallFolder/
├─ include/
│  ├─ TVPaintAnimationSDK/
│  │  ├─ TVPaintSDK.h
│  │  ├─ pi-basics.h
│  │  ├─ ...
├─ lib/
│  ├─ cmake/
│  ├─ pkgconfig/
│  ├─ libTVPaintAnimationSDK.(a|so|dll)
```

# Plugin Creation
