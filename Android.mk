LOCAL_SDL2_PATH := $(call my-dir)

#include $(LOCAL_SDL2_PATH)/SDL2/Android.mk
include $(LOCAL_SDL2_PATH)/SDL2-2.0.12/Android.mk
include $(LOCAL_SDL2_PATH)/SDL2_mixer/Android.mk
include $(LOCAL_SDL2_PATH)/SDL2_net/Android.mk
include $(LOCAL_SDL2_PATH)/SDL2_image/Android.mk
include $(LOCAL_SDL2_PATH)/fluidsynth-lite/src/Android.mk

#SDL_INCLUDE_PATHS = $(LOCAL_SDL2_PATH)/SDL2/include  $(LOCAL_SDL2_PATH)/SDL2_mixer  $(LOCAL_SDL2_PATH)/SDL2_net  $(LOCAL_SDL2_PATH)/SDL2_image
SDL_INCLUDE_PATHS = $(LOCAL_SDL2_PATH)/SDL2-2.0.12/include  $(LOCAL_SDL2_PATH)/SDL2_mixer  $(LOCAL_SDL2_PATH)/SDL2_net  $(LOCAL_SDL2_PATH)/SDL2_image
