// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_DEFINES_H
#define GLTF_DEFINES_H

// This file should only be included by other headers.

// Godot classes used by GLTF headers.
class BoneAttachment3D;
class CSGShape3D;
class GridMap;
class ImporterMeshInstance3D;
class Light3D;
class MeshInstance3D;
class MultiMeshInstance3D;
class Skeleton3D;
class Skin;

// GLTF classes.
struct GLTFAccessor;
class GLTFAnimation;
class GLTFBufferView;
class GLTFCamera;
class GLTFDocument;
class GLTFDocumentExtension;
class GLTFLight;
class GLTFMesh;
class GLTFNode;
class GLTFSkeleton;
class GLTFSkin;
class GLTFSpecGloss;
class GLTFState;
class GLTFTexture;
class GLTFTextureSampler;

// GLTF index aliases.
using GLTFAccessorIndex = int;
using GLTFAnimationIndex = int;
using GLTFBufferIndex = int;
using GLTFBufferViewIndex = int;
using GLTFCameraIndex = int;
using GLTFImageIndex = int;
using GLTFLightIndex = int;
using GLTFMaterialIndex = int;
using GLTFMeshIndex = int;
using GLTFNodeIndex = int;
using GLTFSkeletonIndex = int;
using GLTFSkinIndex = int;
using GLTFTextureIndex = int;
using GLTFTextureSamplerIndex = int;

enum GLTFType {
	TYPE_SCALAR,
	TYPE_VEC2,
	TYPE_VEC3,
	TYPE_VEC4,
	TYPE_MAT2,
	TYPE_MAT3,
	TYPE_MAT4,
};

#endif // GLTF_DEFINES_H
