// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RENDERING_SERVER_GLOBALS_H
#define RENDERING_SERVER_GLOBALS_H

#include "servers/rendering/environment/renderer_fog.h"
#include "servers/rendering/environment/renderer_gi.h"
#include "servers/rendering/renderer_canvas_cull.h"
#include "servers/rendering/renderer_canvas_render.h"
#include "servers/rendering/rendering_method.h"
#include "servers/rendering/storage/camera_attributes_storage.h"
#include "servers/rendering/storage/light_storage.h"
#include "servers/rendering/storage/material_storage.h"
#include "servers/rendering/storage/mesh_storage.h"
#include "servers/rendering/storage/particles_storage.h"
#include "servers/rendering/storage/texture_storage.h"
#include "servers/rendering/storage/utilities.h"

class RendererCanvasCull;
class RendererViewport;
class RenderingMethod;

class RenderingServerGlobals {
public:
	static bool threaded;

	static RendererUtilities *utilities;
	static RendererLightStorage *light_storage;
	static RendererMaterialStorage *material_storage;
	static RendererMeshStorage *mesh_storage;
	static RendererParticlesStorage *particles_storage;
	static RendererTextureStorage *texture_storage;
	static RendererGI *gi;
	static RendererFog *fog;
	static RendererCameraAttributes *camera_attributes;
	static RendererCanvasRender *canvas_render;
	static RendererCompositor *rasterizer;

	static RendererCanvasCull *canvas;
	static RendererViewport *viewport;
	static RenderingMethod *scene;
};

#define RSG RenderingServerGlobals

#endif // RENDERING_SERVER_GLOBALS_H
