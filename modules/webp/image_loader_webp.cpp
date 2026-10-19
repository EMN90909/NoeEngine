// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "image_loader_webp.h"

#include "core/config/project_settings.h"
#include "core/io/marshalls.h"
#include "core/os/os.h"
#include "core/string/print_string.h"
#include "webp_common.h"

#include <stdlib.h>
#include <webp/decode.h>
#include <webp/encode.h>

static Ref<Image> _webp_mem_loader_func(const uint8_t *p_png, int p_size) {
	Ref<Image> img;
	img.instantiate();
	Error err = WebPCommon::webp_load_image_from_buffer(img.ptr(), p_png, p_size);
	ERR_FAIL_COND_V(err, Ref<Image>());
	return img;
}

Error ImageLoaderWebP::load_image(Ref<Image> p_image, Ref<FileAccess> f, BitField<ImageFormatLoader::LoaderFlags> p_flags, float p_scale) {
	Vector<uint8_t> src_image;
	uint64_t src_image_len = f->get_length();
	ERR_FAIL_COND_V(src_image_len == 0, ERR_FILE_CORRUPT);
	src_image.resize(src_image_len);

	uint8_t *w = src_image.ptrw();

	f->get_buffer(&w[0], src_image_len);

	Error err = WebPCommon::webp_load_image_from_buffer(p_image.ptr(), w, src_image_len);

	return err;
}

void ImageLoaderWebP::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("webp");
}

ImageLoaderWebP::ImageLoaderWebP() {
	Image::_webp_mem_loader_func = _webp_mem_loader_func;
	Image::webp_lossy_packer = WebPCommon::_webp_lossy_pack;
	Image::webp_lossless_packer = WebPCommon::_webp_lossless_pack;
	Image::webp_unpacker = WebPCommon::_webp_unpack;
}
