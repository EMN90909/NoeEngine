// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "rendering_server_globals.h"

bool RenderingServerGlobals::threaded = false;

RendererUtilities *RenderingServerGlobals::utilities = nullptr;
RendererLightStorage *RenderingServerGlobals::light_storage = nullptr;
RendererMaterialStorage *RenderingServerGlobals::material_storage = nullptr;
RendererMeshStorage *RenderingServerGlobals::mesh_storage = nullptr;
RendererParticlesStorage *RenderingServerGlobals::particles_storage = nullptr;
RendererTextureStorage *RenderingServerGlobals::texture_storage = nullptr;
RendererGI *RenderingServerGlobals::gi = nullptr;
RendererFog *RenderingServerGlobals::fog = nullptr;
RendererCameraAttributes *RenderingServerGlobals::camera_attributes = nullptr;
RendererCanvasRender *RenderingServerGlobals::canvas_render = nullptr;
RendererCompositor *RenderingServerGlobals::rasterizer = nullptr;

RendererCanvasCull *RenderingServerGlobals::canvas = nullptr;
RendererViewport *RenderingServerGlobals::viewport = nullptr;
RenderingMethod *RenderingServerGlobals::scene = nullptr;
