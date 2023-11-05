#pragma once

#include "gctk_common.h"
#include "gctk_math.h"
#include "gctk_debug.h"
#include "gctk_filesys.h"
#include "gctk_texture.h"
#include "gctk_shader.h"
#include "gctk_sprite.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GctkCallbackFunction)(void);
typedef void (*GctkFrameCallbackFunction)(double delta);
typedef void (*GctkErrorCallbackFunction)(ErrorCode error, const char* message);

typedef struct {
	GctkCallbackFunction pOnLoaded;
	GctkFrameCallbackFunction pOnUpdate;
	GctkFrameCallbackFunction pOnRender;
	GctkCallbackFunction pOnClosed;
	GctkErrorCallbackFunction pOnError;
} GctkStandardCallbacks;

/**
 * @brief Initialize the game engine and it's dependencies.
 * @param name Game's title/name.
 * @param author Game creator's name.
 * @param argc Command line argument count.
 * @param argv Command line argument values.
 * @return Return the error code after initialization.
 */
GCTK_API ErrorCode GctkInit(const char* name, const char* author, int argc, char** argv);
/**
 * @brief Run a single update.
 * @return Return 'true' if the game should continue running, 'false' if it should close.
 */
GCTK_API bool GctkUpdate();
/**
 * @brief Stop the game and free dependencies.
 * @return
 */
GCTK_API ErrorCode GctkQuit();
/**
 * @brief Setup game engine callbacks. Pass NULL to callbacks you don't want to use.
 * @param on_init Callback to run after a successful initialization.
 * @param on_update Callback to run on every update frame.
 * @param on_render Callback to run on every render frame.
 * @param on_close Callback to run when the game is closed.
 * @param on_error Callback to run on error.
 */
GCTK_API void GctkSetupCallbacks(GctkCallbackFunction on_init,
								 GctkFrameCallbackFunction on_update,
								 GctkFrameCallbackFunction on_render,
								 GctkCallbackFunction on_close,
								 GctkErrorCallbackFunction on_error
);
/**
 * @brief Set background clear color.
 */
GCTK_API void GctkSetBackgroundColor(Color color);
/**
 * @brief Set background clear colour.
 */
#define GctkSetBackgroundColour GctkSetBackgroundColor
/**
 * @brief Get background current clear color.
 */
GCTK_API Color GctkGetBackgroundColor();
/**
 * @brief Get background current clear colour.
 */
#define GctkGetBackgroundColour GctkGetBackgroundColor

/**
 * @brief Set frequency of update frames. By default it's set to '1 / 60'.
 */
GCTK_API void GctkSetUpdateFrequency(double freq);
/**
 * Set source for main.lua
 * @param src Lua source code.
 */
GCTK_API void GctkSetLuaMainSource(const char* src);

/**
 * @brief Get the game's name.
 */
GCTK_API const char* GctkGameName();
/**
 * @brief Get the game's creator.
 */
GCTK_API const char* GctkGameAuthor();
/**
 * @brief Get the game's base directory.
 */
GCTK_API const char* GctkBaseDir();
/**
 * @brief Get the current time since setup.
 */
GCTK_API double GctkTime();
/**
 * @brief Signal the game to be closed.
 */
GCTK_API void GctkClose();

#ifdef __cplusplus
}
#endif