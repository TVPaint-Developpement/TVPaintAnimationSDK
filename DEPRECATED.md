# Deprecated API Reference

This document lists all symbols marked `TVDEPRECATED` in the C SDK headers, organized by header file.

The `TVDEPRECATED(message)` macro is defined in `pi-basics.h`. It expands to `[[deprecated(message)]]` in C++17+ and C23, causing compiler warnings when deprecated symbols are
used. In older C standards the macro is a no-op, so the symbols remain silently available.

> **Note:** All deprecated symbols are kept for backward compatibility. They will not be removed without prior notice.

---

## Summary

| Header                                     | Deprecated symbols                                                                                                                                                            | Replacement                                        |
|--------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------|
| [`pi-basics.h`](#pi-basicsh)               | `TVCHAR`, `INT32`, `ULONG32`                                                                                                                                                  | Built-in / prefixed types                          |
| [`pi-pixel.h`](#pi-pixelh)                 | `TVMakeRGBAPixel()`                                                                                                                                                           | `TVMakePIPixel32()`                                |
| [`pi-filter.h`](#pi-filterh)               | `PIEVENT_DEPRECATED`, `PIFilter`, `PIFILTER_FLAG_DEPRECATED`                                                                                                                  | `ePIEvents`, `PIPlugin`, `ePIHudHandle`            |
| [`pi-requester.h`](#pi-requesterh)         | `bPIRequesterFlags_NoDepth`, `bPIRequesterFlags_KeyGrab`, `PIRF_DEPRECATED`, `FILTERREQ_DEPRECATED`, `PIRBF_BUTTON_DEPRECATED`, `BUTTON_TEXT_DEPRECATED`, `PIDRAW_DEPRECATED` | See section                                        |
| [`pi-block.h`](#pi-blockh)                 | `PIWRAP_MODE_DEPRECATED`, `CB_READ_DEPRECATED`, `CB_WRITE_DEPRECATED`                                                                                                         | `ePIBlendingMode`, `ePIReadLayer`, `ePIWriteLayer` |
| [`pi-popup.h`](#pi-popuph)                 | `PIPOPMODE_Flags_DEPRECATED`                                                                                                                                                  | `ePIPopMode`                                       |
| [`pi-preview.h`](#pi-previewh)             | `PIPREVIEW_DEPRECATED`                                                                                                                                                        | `ePIPreviewFlags`                                  |
| [`pi-display.h`](#pi-displayh)             | `PIDISPLAY_MODE_DEPRECATED`                                                                                                                                                   | `ePIDisplayDrawingMode`                            |
| [`pi-hud.h`](#pi-hudh)                     | `HUD_HANDLE_DEPRECATED`, `HUD_TEXT_DEPRECATED`                                                                                                                                | `ePIHudHandle`, `ePIHudAlign`                      |
| [`pi-imagesequence.h`](#pi-imagesequenceh) | `ISEQ_FLAG_DEPRECATED`                                                                                                                                                        | `ePIImageSequenceFlags`                            |
| [`pi-keys.h`](#pi-keysh)                   | `KEY_TYPE`, typedefs, macros, `KEY_OFFSET_ENUM_DEPRECATED`, `KEY_ID_ENUM_DEPRECATED`, `KEY_ID_*_N` macros                                                                     | `ePIKeyType`, `PIKey*`, `kKeyID_*`, etc.           |
| [`pi-misc.h`](#pi-misch)                   | `PITICKS_DEPRECATED`                                                                                                                                                          | `ePITicks`                                         |
| [`pi-profile.h`](#pi-profileh)             | `PIPROF_DEPRECATED`                                                                                                                                                           | `ePIProfileShape`                                  |
| [`pi-videodevice.h`](#pi-videodeviceh)     | `VideoInfo`, `VideoBlock`, `VideoDevID`, `VideoDevice`                                                                                                                        | `PIVideo*` prefixed types                          |

---

## `pi-basics.h`

Three legacy type aliases for primitive types.

| Deprecated | Replacement       |
|------------|-------------------|
| `TVCHAR`   | `char` (built-in) |
| `INT32`    | `PIInt32`         |
| `ULONG32`  | `PIUInt32`        |

---

## `pi-pixel.h`

One deprecated inline helper function.

| Deprecated                            | Replacement                   |
|---------------------------------------|-------------------------------|
| `TVMakeRGBAPixel(plugin, r, g, b, a)` | `TVMakePIPixel32(r, g, b, a)` |

---

## `pi-filter.h`

### `PIEVENT_DEPRECATED` → `ePIEvents`

Old event codes used in `PI_Msg()`. Replace with the corresponding `kPIEvents_*` constant from `ePIEvents`.

| Deprecated                   | Replacement                       |
|------------------------------|-----------------------------------|
| `PICBREQ_OPEN`               | `kPIEvents_WindowOpen`            |
| `PICBREQ_CLOSE`              | `kPIEvents_WindowClose`           |
| `PICBREQ_DRAG`               | `kPIEvents_WindowDragged`         |
| `PICBREQ_RESIZE`             | `kPIEvents_WindowResizing`        |
| `PICBREQ_RESIZED`            | `kPIEvents_WindowResized`         |
| `PICBREQ_HIDE`               | `kPIEvents_WindowHidden`          |
| `PICBREQ_SHOW`               | `kPIEvents_WindowShown`           |
| `PICBREQ_COLLAPSE`           | `kPIEvents_WindowCollapsed`       |
| `PICBREQ_UNCOLLAPSE`         | `kPIEvents_WindowUncollapsed`     |
| `PICBREQ_KEY_DOWN`           | `kPIEvents_WindowKeyDown`         |
| `PICBREQ_FKEY_DOWN`          | `kPIEvents_WindowFunctionKeyDown` |
| `PICBREQ_PROJECT_CHANGE`     | `kPIEvents_ProjectChanged`        |
| `PICBREQ_PROJECT_INACTIVATE` | `kPIEvents_ProjectInactivated`    |
| `PICBREQ_PROJECT_ACTIVATE`   | `kPIEvents_ProjectActivated`      |
| `PICBREQ_FILE_RESET`         | `kPIEvents_KeysetReset`           |
| `PICBREQ_FILE_LOAD`          | `kPIEvents_KeysetLoad`            |
| `PICBREQ_FILE_SAVE`          | `kPIEvents_KeysetSave`            |
| `PICBREQ_FILE_SAVE_AS`       | `kPIEvents_KeysetSaveAs`          |
| `PICBREQ_COORD`              | `kPIEvents_Coordinates`           |
| `PICBREQ_TICKS`              | `kPIEvents_Ticks`                 |
| `PICB_HUD_REDRAW`            | `kPIEvents_RedrawHud`             |
| `PICB_PICK_COLOR`            | `kPIEvents_ColorPicked`           |
| `PICBREQ_ROOM_CHANGE`        | `kPIEvents_RoomChanged`           |
| `PICBREQ_BUTTON_DOWN`        | `kPIEvents_ButtonDown`            |
| `PICBREQ_BUTTON_UP`          | `kPIEvents_ButtonUp`              |
| `PICBREQ_BUTTON_TEXT`        | `kPIEvents_ButtonText`            |
| `PICBREQ_MINISLIDER`         | `kPIEvents_ButtonMiniSlider`      |
| `PICBREQ_SLIDER_MOVE`        | `kPIEvents_ButtonSliderDragged`   |
| `PICBREQ_SLIDER_RELEASE`     | `kPIEvents_ButtonSliderReleased`  |
| `PICBREQ_TABS_CHANGE`        | `kPIEvents_ButtonTabChanged`      |
| `PICBREQ_MOVE`               | `kPIEvents_MouseMove`             |
| `PICMETA_CLOSE`              | `kPIEvents_MetaClosed`            |
| `PICMETA_POINT`              | `kPIEvents_MetaPoint`             |
| `PICMETA_DRAW`               | `kPIEvents_MetaFreehand`          |
| `PICMETA_LINE`               | `kPIEvents_MetaLine`              |
| `PICMETA_RECT`               | `kPIEvents_MetaRectangle`         |
| `PICMETA_CIRCLE`             | `kPIEvents_MetaCircle`            |
| `PICMETA_ELLIPSE`            | `kPIEvents_MetaEllipse`           |
| `PICMETA_PREVIEW`            | `kPIEvents_MetaPreview`           |
| `PICMETA_KEYBOARD`           | `kPIEvents_MetaKeyboard`          |
| `PICBREQ_VIDEO_CHANGE_PREV`  | **No replacement**                |
| `PICBREQ_VIDEO_CHANGE`       | **No replacement**                |

### `PIFilter` → `PIPlugin`

```c
// Before
PIFilter* filter = ...;

// After
PIPlugin* plugin = ...;
```

### `PIFILTER_FLAG_DEPRECATED` → `ePIPluginFlags`

| Deprecated             | Replacement             |
|------------------------|-------------------------|
| `PIFILTER_FLAG_HIDDEN` | `kPIPluginFlags_Hidden` |

---

## `pi-requester.h`

### Deprecated flags within `ePIRequesterFlags`

Two flags in the active `ePIRequesterFlags` enum are themselves deprecated because they no longer have any effect in TVPaint 12's new GUI:

| Deprecated flag             | Reason                                                |
|-----------------------------|-------------------------------------------------------|
| `bPIRequesterFlags_NoDepth` | Doesn't make any sense with the new GUI of TVPaint 12 |
| `bPIRequesterFlags_KeyGrab` | Doesn't make any sense with the new GUI of TVPaint 12 |

### `PIRF_DEPRECATED` → `ePIRequesterFlags`

| Deprecated              | Replacement                     |
|-------------------------|---------------------------------|
| `PIRF_STANDARD_REQ`     | `bPIRequesterFlags_Standard`    |
| `PIRF_LOCK_REQ`         | `bPIRequesterFlags_Modal`       |
| `PIRF_NO_DRAG_REQ`      | `bPIRequesterFlags_NoDrag`      |
| `PIRF_NO_DEPTH_REQ`     | `bPIRequesterFlags_NoDepth`     |
| `PIRF_NO_CLOSE_REQ`     | `bPIRequesterFlags_NoClose`     |
| `PIRF_NO_SYSBUTTON_REQ` | `bPIRequesterFlags_NoSysbutton` |
| `PIRF_CENTER_REQ`       | `bPIRequesterFlags_Center`      |
| `PIRF_KEYGRAB_REQ`      | `bPIRequesterFlags_KeyGrab`     |
| `PIRF_RESIZE_REQ`       | `bPIRequesterFlags_Resize`      |
| `PIRF_HIDDEN_REQ`       | `bPIRequesterFlags_Hidden`      |
| `PIRF_COLLAPSABLE_REQ`  | `bPIRequesterFlags_Collapsable` |
| `PIRF_COLLAPSED_REQ`    | `bPIRequesterFlags_Collapsed`   |
| `PIRF_BINABLE_REQ`      | `bPIRequesterFlags_Binable`     |

### `FILTERREQ_DEPRECATED` → `ePIFilterFlags`

| Deprecated              | Replacement                 |
|-------------------------|-----------------------------|
| `FILTERREQ_NO_PATH`     | `bPIFilterFlags_NoPath`     |
| `FILTERREQ_NO_FILE`     | `bPIFilterFlags_NoFile`     |
| `FILTERREQ_NO_TBAR`     | `bPIFilterFlags_NoTopBar`   |
| `FILTERREQ_EXPORT`      | `bPIFilterFlags_Export`     |
| `FILTERREQ_MOTION`      | `bPIFilterFlags_Motion`     |
| `FILTERREQ_CUSTOM_IO`   | `bPIFilterFlags_CustomIO`   |
| `FILTERREQ_CREATE_ONLY` | `bPIFilterFlags_CreateOnly` |

### `PIRBF_BUTTON_DEPRECATED` → `ePIButtonFlags`

| Deprecated                 | Replacement                  |
|----------------------------|------------------------------|
| `PIRBF_BUTTON_NORMAL`      | `bPIButtonFlags_Normal`      |
| `PIRBF_BUTTON_IMMEDIATE`   | `bPIButtonFlags_Immediate`   |
| `PIRBF_BUTTON_INVERT`      | `bPIButtonFlags_Invert`      |
| `PIRBF_BUTTON_HIDE`        | `bPIButtonFlags_Hide`        |
| `PIRBF_BUTTON_QUIET`       | `bPIButtonFlags_Quiet`       |
| `PIRBF_BUTTON_RIGHT`       | `bPIButtonFlags_Right`       |
| `PIRBF_BUTTON_REPEAT`      | `bPIButtonFlags_Repeat`      |
| `PIRBF_BUTTON_SELECT`      | `bPIButtonFlags_Select`      |
| `PIRBF_BUTTON_POPUP`       | `bPIButtonFlags_Popup`       |
| `PIRBF_BUTTON_FLAT`        | `bPIButtonFlags_Flat`        |
| `PIRBF_BUTTON_TEXT`        | `bPIButtonFlags_Text`        |
| `PIRBF_BUTTON_TEXT_INT`    | `bPIButtonFlags_TextInteger` |
| `PIRBF_BUTTON_TEXT_ACTIVE` | `bPIButtonFlags_TextActive`  |
| `PIRBF_BUTTON_ACTION`      | `bPIButtonFlags_Action`      |
| `PIRBF_BUTTON_DRAG`        | `bPIButtonFlags_Drag`        |
| `PIRBF_BUTTON_DIALOG`      | `bPIButtonFlags_Dialog`      |
| `PIRBF_BUTTON_STAT`        | `bPIButtonFlags_Stat`        |
| `PIRBF_BUTTON_TEXT_LOCK`   | `bPIButtonFlags_TextLock`    |
| `PIRBF_BUTTON_CHECK`       | `bPIButtonFlags_Check`       |
| `PIRBF_BUTTON_MINISLIDER`  | `bPIButtonFlags_Minislider`  |
| `PIRBF_BUTTON_HSEPARATOR`  | `bPIButtonFlags_HSeparator`  |
| `PIRBF_BUTTON_VSEPARATOR`  | `bPIButtonFlags_VSeparator`  |
| `PIRBF_BUTTON_FRAME`       | `bPIButtonFlags_Frame`       |

### `BUTTON_TEXT_DEPRECATED` → `ePIButtonTextAlign`

| Deprecated            | Replacement                    |
|-----------------------|--------------------------------|
| `BUTTON_TEXT_ILEFT`   | `bPIButtonTextAlign_InLeft`    |
| `BUTTON_TEXT_IRIGHT`  | `bPIButtonTextAlign_InRight`   |
| `BUTTON_TEXT_OLEFT`   | `bPIButtonTextAlign_OutLeft`   |
| `BUTTON_TEXT_ORIGHT`  | `bPIButtonTextAlign_OutRight`  |
| `BUTTON_TEXT_ITOP`    | `bPIButtonTextAlign_InTop`     |
| `BUTTON_TEXT_IBOTTOM` | `bPIButtonTextAlign_InBottom`  |
| `BUTTON_TEXT_OTOP`    | `bPIButtonTextAlign_OutTop`    |
| `BUTTON_TEXT_OBOTTOM` | `bPIButtonTextAlign_OutBottom` |

### `PIDRAW_DEPRECATED` → `ePIMetaFlags`

| Deprecated                | Replacement               |
|---------------------------|---------------------------|
| `PIDRAW_POINT`            | `kPIMeta_Dot`             |
| `PIDRAW_DRAW`             | `kPIMeta_Draw`            |
| `PIDRAW_LINE`             | `kPIMeta_Line`            |
| `PIDRAW_RECT`             | `kPIMeta_Rectangle`       |
| `PIDRAW_CIRCLE`           | `kPIMeta_Circle`          |
| `PIDRAW_ELLIPSE`          | `kPIMeta_Ellipse`         |
| `PIDRAW_SPLINE`           | `kPIMeta_Spline`          |
| `PIDRAW_PREVIEW`          | `kPIMeta_Preview`         |
| `PIDRAW_FONC_MASK`        | `bPIMeta_Function_Mask`   |
| `PIDRAW_PREVIEW_BRUSH`    | `bPIMeta_PreviewBrush`    |
| `PIDRAW_NO_INTERPOLATION` | `bPIMeta_NoInterpolation` |
| `PIDRAW_REFRESH_PREVIEW`  | `bPIMeta_RefreshPreview`  |
| `PIDRAW_KEYBOARD`         | `bPIMeta_Keyboard`        |

---

## `pi-block.h`

### `PIWRAP_MODE_DEPRECATED` → `ePIBlendingMode`

| Deprecated   | Replacement              |
|--------------|--------------------------|
| _See header_ | `ePIBlendingMode` values |

### `CB_READ_DEPRECATED` → `ePIReadLayer`

| Deprecated   | Replacement           |
|--------------|-----------------------|
| _See header_ | `ePIReadLayer` values |

### `CB_WRITE_DEPRECATED` → `ePIWriteLayer`

| Deprecated   | Replacement            |
|--------------|------------------------|
| _See header_ | `ePIWriteLayer` values |

---

## `pi-popup.h`

### `PIPOPMODE_Flags_DEPRECATED` → `ePIPopMode`

| Deprecated            | Replacement            |
|-----------------------|------------------------|
| `PIPOPMODE_SEPARATOR` | `bPIPopMode_Separator` |
| `PIPOPMODE_USE_APEN`  | `bPIPopMode_UseAPen`   |
| `PIPOPMODE_QUIET`     | `bPIPopMode_Quiet`     |
| `PIPOPMODE_CHECK`     | `bPIPopMode_Check`     |
| `PIPOPMODE_GHOST`     | `bPIPopMode_Ghost`     |
| `PIPOPMODE_MULTI`     | `bPIPopMode_Multi`     |

---

## `pi-preview.h`

### `PIPREVIEW_DEPRECATED` → `ePIPreviewFlags`

| Deprecated                              | Replacement                 |
|-----------------------------------------|-----------------------------|
| `PREVLINE_RESTORE`                      | `bPIPreviewFlags_Restore`   |
| `PREVLINE_INVERT`                       | `bPIPreviewFlags_Invert`    |
| `PREVLINE_NO_FIRST`                     | `bPIPreviewFlags_NoFirst`   |
| `PREVLINE_CHECK`                        | `bPIPreviewFlags_Check`     |
| `PREVLINE_CROSS`                        | `bPIPreviewFlags_Cross`     |
| `PREVLINE_SELECTION`                    | `bPIPreviewFlags_Selection` |
| _See header for remaining_ `PREVTEXT_*` | `bPIPreviewFlags_Text*`     |

---

## `pi-display.h`

### `PIDISPLAY_MODE_DEPRECATED` → `ePIDisplayDrawingMode`

| Deprecated            | Replacement                  |
|-----------------------|------------------------------|
| `PIDISPLAY_MODE_COPY` | `kPIDisplayDrawingMode_Copy` |
| `PIDISPLAY_MODE_EOR`  | `kPIDisplayDrawingMode_Xor`  |

---

## `pi-hud.h`

### `HUD_HANDLE_DEPRECATED` → `ePIHudHandle`

| Deprecated   | Replacement           |
|--------------|-----------------------|
| _See header_ | `ePIHudHandle` values |

### `HUD_TEXT_DEPRECATED` → `ePIHudAlign`

| Deprecated   | Replacement          |
|--------------|----------------------|
| _See header_ | `ePIHudAlign` values |

---

## `pi-imagesequence.h`

### `ISEQ_FLAG_DEPRECATED` → `ePIImageSequenceFlags`

| Deprecated          | Replacement                    |
|---------------------|--------------------------------|
| `ISEQ_FLAG_OPEN`    | `kPIImageSequenceFlags_Open`   |
| `ISEQ_FLAG_NO_SCAN` | `kPIImageSequenceFlags_NoScan` |

---

## `pi-keys.h`

### `KEY_TYPE` → `ePIKeyType`

| Deprecated        | Replacement         |
|-------------------|---------------------|
| `KEY_TYPE_INT`    | `kPIKeyType_Int`    |
| `KEY_TYPE_DOUBLE` | `kPIKeyType_Double` |
| `KEY_TYPE_COLOR`  | `kPIKeyType_Color`  |
| `KEY_TYPE_UCOLOR` | `kPIKeyType_UColor` |
| `KEY_TYPE_BOOL`   | `kPIKeyType_Bool`   |
| `KEY_TYPE_XYZ`    | `kPIKeyType_XYZ`    |
| `KEY_TYPE_HPB`    | `kPIKeyType_HPB`    |

### Type aliases

| Deprecated | Replacement  |
|------------|--------------|
| `KeyXYZ`   | `PIKeyXYZ`   |
| `KeyHPB`   | `PIKeyHPB`   |
| `KeyColor` | `PIKeyColor` |

### Macros

| Deprecated     | Replacement      |
|----------------|------------------|
| `KEY_ID_START` | `kKeyID_Start`   |
| `KEY_ID_RES`   | `kKeyID_MaxSize` |

### `KEY_OFFSET_ENUM_DEPRECATED` → `eKeyIDOffset`

| Deprecated            | Replacement                |
|-----------------------|----------------------------|
| `KEY_OFFSET_RADIUS`   | `kKeyIDOffset_Radius`      |
| `KEY_OFFSET_H`        | `kKeyIDOffset_H`           |
| `KEY_OFFSET_P`        | `kKeyIDOffset_P`           |
| `KEY_OFFSET_B`        | `kKeyIDOffset_B`           |
| `KEY_OFFSET_COLOR`    | `kKeyIDOffset_Color`       |
| `KEY_OFFSET_WIDTH`    | `kKeyIDOffset_Width`       |
| `KEY_OFFSET_HEIGHT`   | `kKeyIDOffset_Height`      |
| `KEY_OFFSET_PRESSURE` | `kKeyIDOffset_Pressure`    |
| `KEY_OFFSET_ALTITUDE` | `kKeyIDOffset_Altitude`    |
| `KEY_OFFSET_AZIMUTH`  | `kKeyIDOffset_Azimuth`     |
| `KEY_OFFSET_FWHEEL`   | `kKeyIDOffset_FingerWheel` |
| `KEY_OFFSET_SPEED`    | `kKeyIDOffset_Speed`       |
| `KEY_OFFSET_DOWN`     | `kKeyIDOffset_Down`        |
| `KEY_OFFSET_LINEAR`   | **No replacement**         |
| `KEY_OFFSET_NOISE`    | `kKeyIDOffset_Noise`       |
| `KEY_OFFSET_MBLUR`    | `kKeyIDOffset_MotionBlur`  |
| `KEY_OFFSET_OPACITY`  | `kKeyIDOffset_Opacity`     |
| `KEY_OFFSET_XYZ`      | `kKeyIDOffset_XYZ`         |
| `KEY_OFFSET_HPB`      | `kKeyIDOffset_HPB`         |
| `KEY_OFFSET_QUAT`     | `kKeyIDOffset_Quaternion`  |

### `KEY_ID_ENUM_DEPRECATED` → `eKeyID`

| Deprecated        | Replacement          |
|-------------------|----------------------|
| `KEY_ID_RADIUS`   | `kKeyID_Radius`      |
| `KEY_ID_H`        | `kKeyID_H`           |
| `KEY_ID_P`        | `kKeyID_P`           |
| `KEY_ID_B`        | `kKeyID_B`           |
| `KEY_ID_COLOR`    | `kKeyID_Color`       |
| `KEY_ID_WIDTH`    | `kKeyID_Width`       |
| `KEY_ID_HEIGHT`   | `kKeyID_Height`      |
| `KEY_ID_PRESSURE` | `kKeyID_Pressure`    |
| `KEY_ID_ALTITUDE` | `kKeyID_Altitude`    |
| `KEY_ID_AZIMUTH`  | `kKeyID_Azimuth`     |
| `KEY_ID_FWHEEL`   | `kKeyID_FingerWheel` |
| `KEY_ID_SPEED`    | `kKeyID_Speed`       |
| `KEY_ID_DOWN`     | `kKeyID_Down`        |
| `KEY_ID_LINEAR`   | **No replacement**   |
| `KEY_ID_NOISE`    | `kKeyID_Noise`       |
| `KEY_ID_MBLUR`    | `kKeyID_MotionBlur`  |
| `KEY_ID_OPACITY`  | `kKeyID_Opacity`     |
| `KEY_ID_XYZ`      | `kKeyID_XYZ`         |
| `KEY_ID_HPB`      | `kKeyID_HPB`         |
| `KEY_ID_QUAT`     | `kKeyID_Quaternion`  |

### `KEY_ID_*_N` function-like macros

| Deprecated             | Replacement               |
|------------------------|---------------------------|
| `KEY_ID_RADIUS_N(n)`   | `kKeyID_Radius_N(n)`      |
| `KEY_ID_H_N(n)`        | `kKeyID_H_N(n)`           |
| `KEY_ID_P_N(n)`        | `kKeyID_P_N(n)`           |
| `KEY_ID_B_N(n)`        | `kKeyID_B_N(n)`           |
| `KEY_ID_COLOR_N(n)`    | `kKeyID_Color_N(n)`       |
| `KEY_ID_WIDTH_N(n)`    | `kKeyID_Width_N(n)`       |
| `KEY_ID_HEIGHT_N(n)`   | `kKeyID_Height_N(n)`      |
| `KEY_ID_PRESSURE_N(n)` | `kKeyID_Pressure_N(n)`    |
| `KEY_ID_ALTITUDE_N(n)` | `kKeyID_Altitude_N(n)`    |
| `KEY_ID_AZIMUTH_N(n)`  | `kKeyID_Azimuth_N(n)`     |
| `KEY_ID_FWHEEL_N(n)`   | `kKeyID_FingerWheel_N(n)` |
| `KEY_ID_SPEED_N(n)`    | `kKeyID_Speed_N(n)`       |
| `KEY_ID_DOWN_N(n)`     | `kKeyID_Down_N(n)`        |
| `KEY_ID_LINEAR_N(n)`   | **No replacement**        |
| `KEY_ID_NOISE_N(n)`    | `kKeyID_Noise_N(n)`       |
| `KEY_ID_MBLUR_N(n)`    | `kKeyID_MotionBlur_N(n)`  |
| `KEY_ID_OPACITY_N(n)`  | `kKeyID_Opacity_N(n)`     |
| `KEY_ID_XYZ_N(n)`      | `kKeyID_XYZ_N(n)`         |
| `KEY_ID_HPB_N(n)`      | `kKeyID_HPB_N(n)`         |
| `KEY_ID_QUAT_N(n)`     | `kKeyID_Quaternion_N(n)`  |

---

## `pi-misc.h`

### `PITICKS_DEPRECATED` → `ePITicks`

| Deprecated             | Replacement       |
|------------------------|-------------------|
| `PITICKS_FLAG_OFF`     | `kPITicks_Off`    |
| `PITICKS_FLAG_ON`      | `kPITicks_On`     |
| `PITICKS_FLAG_NO_WAIT` | `kPITicks_NoWait` |

---

## `pi-profile.h`

### `PIPROF_DEPRECATED` → `ePIProfileShape`

| Deprecated  | Replacement          |
|-------------|----------------------|
| `PIPROF_00` | `kPIProfileShape_00` |
| `PIPROF_01` | `kPIProfileShape_01` |
| `PIPROF_10` | `kPIProfileShape_10` |
| `PIPROF_11` | `kPIProfileShape_11` |

---

## `pi-videodevice.h`

Four type aliases renamed with the `PI` prefix for consistency with the rest of the SDK.

| Deprecated    | Replacement       |
|---------------|-------------------|
| `VideoInfo`   | `PIVideoInfo`     |
| `VideoBlock`  | `PIVideoBlock`    |
| `VideoDevID`  | `PIVideoDeviceID` |
| `VideoDevice` | `PIVideoDevice`   |