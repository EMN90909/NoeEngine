// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IMAGE_LOADER_JPEGD_H
#define IMAGE_LOADER_JPEGD_H

#include "core/io/image_loader.h"

class ImageLoaderJPG : public ImageFormatLoader {
public:
	virtual Error load_image(Ref<Image> p_image, Ref<FileAccess> f, BitField<ImageFormatLoader::LoaderFlags> p_flags, float p_scale);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	ImageLoaderJPG();
};

#endif // IMAGE_LOADER_JPEGD_H
