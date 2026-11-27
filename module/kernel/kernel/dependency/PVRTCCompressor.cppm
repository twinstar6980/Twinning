module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/PVRTCCompressor/origin/PvrTcDecoder.h"
#include "dependency/PVRTCCompressor/origin/PvrTcEncoder.h"
#include "dependency/PVRTCCompressor/origin/RgbBitmap.h"
#include "dependency/PVRTCCompressor/origin/RgbaBitmap.h"

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
