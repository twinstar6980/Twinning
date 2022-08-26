project(
	PVRTCCompressor
	VERSION 0 # none
	HOMEPAGE_URL https://github.com/brenwill/PVRTCCompressor
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} STATIC
	./AlphaBitmap.h
	./Bitmap.h
	./BitScale.h
	./BitUtility.h
	./ColorRgba.h
	./Interval.h
	./MortonTable.h
	./Point2.h
	./PvrTcDecoder.h
	./PvrTcEncoder.h
	./PvrTcPacket.h
	./RgbaBitmap.h
	./RgbBitmap.h
	./BitScale.cpp
	./MortonTable.cpp
	./PvrTcDecoder.cpp
	./PvrTcEncoder.cpp
	./PvrTcPacket.cpp
)
