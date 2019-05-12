#pragma once

#ifdef COEL_PLATFORM_WINDOWS
#define COEL_WINDOWS_ONLY(x) x
#endif
#ifdef COEL_PLATFORM_LINUX
#define COEL_LINUX_ONLY(x) x
#endif
#ifdef COEL_PLATFORM_MACOS
#define COEL_MACOS_ONLY(x) x
#endif

#ifndef COEL_WINDOWS_ONLY
#define COEL_WINDOWS_ONLY(x)
#endif
#ifndef COEL_LINUX_ONLY
#define COEL_LINUX_ONLY(x)
#endif
#ifndef COEL_MACOS_ONLY
#define COEL_MACOS_ONLY(x)
#endif

#ifdef COEL_DEBUG
#define COEL_DEBUG_ONLY(x) x
#endif
#ifdef COEL_RELEASE
#define COEL_RELEASE_ONLY(x) x
#endif

#ifndef COEL_DEBUG_ONLY
#define COEL_DEBUG_ONLY(x)
#endif
#ifndef COEL_RELEASE_ONLY
#define COEL_RELEASE_ONLY(x)
#endif

#ifdef COEL_RENDER_API_VULKAN
#define COEL_RENDER_API Coel::RenderAPI::Vulkan
#endif
#ifdef COEL_RENDER_API_DIRECTX
#define COEL_RENDER_API Coel::RenderAPI::DirectX
#endif
#ifdef COEL_RENDER_API_METAL
#define COEL_RENDER_API Coel::RenderAPI::Metal
#endif

#ifndef COEL_RENDER_API
#define COEL_RENDER_API Coel::RenderAPI::OpenGL
#define COEL_RENDER_API_OPENGL
#endif