#ifndef __XGP_IMAGE_H__
#define __XGP_IMAGE_H__

#include <iostream>

namespace xgp {

	enum ImageType {
		IMG_1D   = 0,
		IMG_2D   = 1,
		IMG_3D   = 2,
		IMG_CUBE = 3
	};

	class Image {
	public:
		Image();
		~Image();

		void loadImage(const std::string& filepath, ImageType type);
		void free();

		const int width() const;
		const int height() const;
		const int channels() const;
		const ImageType type() const;
		unsigned char* data() const;

	private:
		int _width;
		int _height;
		int _channels;
		ImageType _type;
		unsigned char* _data;
	};
}



#endif