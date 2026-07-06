# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SDL2_OpenTouch is a customized SDL2/SDL3 library for Android, used as the native multimedia layer for the OpenTouch game emulator framework. It lives at `opentouch/src/main/jni/SDL2_OpenTouch/` within the larger OpenTouch Android project and provides audio, graphics, input, and networking via JNI.

## Build System

Built with **Android NDK** using `Android.mk` files (not CMake for the Android build). The top-level `Android.mk` includes subdirectory makefiles that build:

- `libSDL2.so` / `libSDL2_static.a` — from `SDL2-2.0.12/`
- `libSDL2_mixer.a` — from `SDL2_mixer/` (FLAC, OGG, MP3, MOD, MIDI support)
- `libSDL2_image.a` — from `SDL2_image/` (BMP, PNG, JPEG, GIF, WebP, etc.)
- `libSDL2_net.a` — from `SDL2_net/`
- `libfluidsynth-SDL-static.a` — from `fluidsynth-lite/` (MIDI SoundFont synthesis)
- `libSDL3.so` — from `SDL-3.x/` (next-gen, alternative)

Build is triggered as part of the parent OpenTouch project's Gradle/NDK build pipeline. To build from the OpenTouch project root:

```bash
./gradlew assembleDebug    # Full debug build including native libraries
./gradlew assembleRelease  # Release build
```

There are no standalone tests or linting tools configured for this native library — correctness is verified by building and running within the parent OpenTouch app.

## OpenTouch Customizations

All custom modifications to upstream SDL2 are guarded by the `OPENTOUCH_SDL_EXTRA` preprocessor flag. Key customizations:

- **Persistent hardware keyboard** — always works without soft keyboard dependency
- **EGL surface state management** — fixes for display transitions and resume
- **Window surface_valid handling** — prevents black screen issues (e.g., Hexen 2 SW renderer)
- **Joystick update disabled** — prevents crashes in certain engines (EDuke)
- **AAudio fixes** — corrupt sound fix for Android 15

Modified files (14 total) are in `SDL2-2.0.12/src/` across video, audio, events, render, core/android, and file subsystems. Search for `OPENTOUCH_SDL_EXTRA` to find all customization points.

## Architecture

**Audio pipeline:** AAudio (primary, low-latency) → OpenSL ES (fallback) → AudioTrack (legacy). SDL2_mixer adds multi-channel mixing with codec support via bundled external libraries in `SDL2_mixer/external/`.

**Graphics pipeline:** EGL surface management → OpenGL ES 1.x/2.x rendering → Android SurfaceView.

**System dependencies:** `libGLESv1_CM`, `libGLESv2`, `libOpenSLES`, `liblog`, `libandroid`, `libdl`, plus `hidapi` and `saffal` (scoped storage) as shared library dependencies.

## Key Conventions

- C is the primary language; C++11 only for HIDAPI (`src/hidapi/android/hid.cpp`)
- ARM assembly files use `.c.arm` extension for NDK compatibility
- Audio codec libraries are vendored in `SDL2_mixer/external/`
- Compiler flags include `-Wall -Wextra` with `-Wno-unused-parameter -Wno-sign-compare`
