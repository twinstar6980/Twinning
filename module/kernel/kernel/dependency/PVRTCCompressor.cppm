module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/PVRTCCompressor/PvrTcDecoder.h"
#include "dependency/PVRTCCompressor/PvrTcEncoder.h"
#include "dependency/PVRTCCompressor/RgbBitmap.h"
#include "dependency/PVRTCCompressor/RgbaBitmap.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.PVRTCCompressor;

export namespace Twinning::Kernel::Dependency::PVRTCCompressor {

	using ::Javelin::Point2;

	using ::Javelin::ColorRgb;

	using ::Javelin::ColorRgba;

	using ::Javelin::RgbBitmap;

	using ::Javelin::RgbaBitmap;

	using ::Javelin::PvrTcEncoder;

	using ::Javelin::PvrTcDecoder;

}
