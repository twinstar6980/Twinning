module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.system_native_string_extended;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.third.system.windows;
import twinning.kernel.third.system.posix;

export namespace Twinning::Kernel::SystemNativeString {

	#pragma region windows

	#if defined M_system_windows

	inline auto wide_to_utf8 (
		ConstantBasicStringView<CharacterW> const & source
	) -> BasicString<CharacterN> {
		auto destination = BasicString<CharacterN>{};
		if (!source.empty()) {
			auto destination_size = Size{};
			destination_size = mbox<Size>(
				Third::system::windows::$WideCharToMultiByte(
					Third::system::windows::$CP_UTF8,
					Third::system::windows::$WC_ERR_INVALID_CHARS,
					cast_pointer<Third::system::windows::$WCHAR>(source.begin()).value,
					static_cast<int>(source.size().value),
					nullptr,
					0,
					nullptr,
					nullptr
				)
			);
			assert_test(destination_size != k_none_size);
			destination.allocate_full(destination_size);
			destination_size = mbox<Size>(
				Third::system::windows::$WideCharToMultiByte(
					Third::system::windows::$CP_UTF8,
					Third::system::windows::$WC_ERR_INVALID_CHARS,
					cast_pointer<Third::system::windows::$WCHAR>(source.begin()).value,
					static_cast<int>(source.size().value),
					cast_pointer<ZCharacterN>(destination.begin()).value,
					static_cast<int>(destination.size().value),
					nullptr,
					nullptr
				)
			);
			assert_test(destination_size == destination.size());
		}
		return destination;
	}

	// ----------------

	inline auto wide_from_utf8 (
		ConstantBasicStringView<CharacterN> const & source
	) -> BasicString<CharacterW> {
		auto destination = BasicString<CharacterW>{};
		if (!source.empty()) {
			auto destination_size = Size{};
			destination_size = mbox<Size>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_UTF8,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					cast_pointer<Third::system::windows::$CHAR>(source.begin()).value,
					static_cast<int>(source.size().value),
					nullptr,
					0
				)
			);
			assert_test(destination_size != k_none_size);
			destination.allocate_full(destination_size);
			destination_size = mbox<Size>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_UTF8,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					cast_pointer<Third::system::windows::$CHAR>(source.begin()).value,
					static_cast<int>(source.size().value),
					cast_pointer<Third::system::windows::$WCHAR>(destination.begin()).value,
					static_cast<int>(destination.size().value)
				)
			);
			assert_test(destination_size == destination.size());
		}
		return destination;
	}

	inline auto wide_from_ansi (
		ConstantBasicStringView<CharacterN> const & source
	) -> BasicString<CharacterW> {
		auto destination = BasicString<CharacterW>{};
		if (!source.empty()) {
			auto destination_size = Size{};
			destination_size = mbox<Size>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_ACP,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					cast_pointer<Third::system::windows::$CHAR>(source.begin()).value,
					static_cast<int>(source.size().value),
					nullptr,
					0
				)
			);
			assert_test(destination_size != k_none_size);
			destination.allocate_full(destination_size);
			destination_size = mbox<Size>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_ACP,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					cast_pointer<Third::system::windows::$CHAR>(source.begin()).value,
					static_cast<int>(source.size().value),
					cast_pointer<Third::system::windows::$WCHAR>(destination.begin()).value,
					static_cast<int>(destination.size().value)
				)
			);
			assert_test(destination_size == destination.size());
		}
		return destination;
	}

	#endif

	#pragma endregion

	#pragma region posix

	#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

	#endif

	#pragma endregion

	#pragma region universal

	inline auto utf8_from_native (
		ConstantBasicStringView<CharacterN> const & source
	) -> BasicString<CharacterN> {
		#if defined M_system_windows
		return wide_to_utf8(wide_from_ansi(source));
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		return BasicString<CharacterN>{source};
		#endif
	}

	#pragma endregion

}
