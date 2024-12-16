module;

#include <typeinfo>
#include "kernel/common.hpp"

export module twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.exception.exception;
import twinning.kernel.third.fmt;
import twinning.kernel.third.system.windows;

export namespace Twinning::Kernel {

	#pragma region detail

	namespace Detail {

		inline auto string_encoding_native_to_utf8 (
			std::string_view const & source
		) -> std::string {
			#if defined M_system_windows
			auto temporary = std::vector<wchar_t>{};
			temporary.resize(source.size());
			auto temporary_size = static_cast<std::size_t>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_ACP,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					source.data(),
					static_cast<int>(source.size()),
					temporary.data(),
					static_cast<int>(temporary.size())
				)
			);
			auto utf16_converter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{};
			auto destination = utf16_converter.to_bytes(
				temporary.data(),
				temporary.data() + temporary_size
			);
			// NOTE : SKIP
			// assert_test(utf16_converter.converted() == temporary_size);
			return destination;
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			return std::string{source};
			#endif
		}
	}

	#pragma endregion

	#pragma region type

	class UnknownException :
		public Exception {

	public:

		explicit UnknownException (
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Unknown", {}, location} {
		}

	};

	class UnnamedException :
		public Exception {

	public:

		explicit UnnamedException (
			std::string const &          message,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Unnamed", {}, location} {
			thiz.m_description.emplace_back(Third::fmt::format("message : {}", message));
		}

	};

	// ----------------

	class ImpossibleException :
		public Exception {

	public:

		explicit ImpossibleException (
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Impossible", {}, location} {
		}

	};

	class IncompleteException :
		public Exception {

	public:

		explicit IncompleteException (
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Incomplete", {}, location} {
		}

	};

	// ----------------

	class AssertionException :
		public Exception {

	public:

		explicit AssertionException (
			std::string const &          expression,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Assertion", {}, location} {
			thiz.m_description.emplace_back(Third::fmt::format("expression : {}", expression));
		}

	};

	// ----------------

	class InvocationException :
		public Exception {

	public:

		explicit InvocationException (
			std::string const &          target,
			std::string const &          message,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Invocation", {}, location} {
			thiz.m_description.emplace_back(Third::fmt::format("target  : {}", target));
			thiz.m_description.emplace_back(Third::fmt::format("message : {}", message));
		}

	};

	// ----------------

	class SyntaxException :
		public Exception {

	public:

		explicit SyntaxException (
			std::size_t const &          position,
			std::string const &          message,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Syntax", {}, location} {
			thiz.m_description.emplace_back(Third::fmt::format("position : {:X}h", position));
			thiz.m_description.emplace_back(Third::fmt::format("message  : {}", message));
		}

	};

	// ----------------

	class StandardException :
		public Exception {

	public:

		explicit StandardException (
			std::exception const &       exception,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Standard", {}, location} {
			thiz.m_description.emplace_back(Third::fmt::format("type    : {}", typeid(exception).name()));
			thiz.m_description.emplace_back(Third::fmt::format("message : {}", exception.what()));
		}

	};

	class StandardSystemException :
		public Exception {

	public:

		explicit StandardSystemException (
			std::system_error const &    exception,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Standard.System", {}, location} {
			thiz.m_description.emplace_back(Third::fmt::format("type    : {}", typeid(exception).name()));
			thiz.m_description.emplace_back(Third::fmt::format("message : {}", Detail::string_encoding_native_to_utf8(exception.code().message())));
		}

	};

	class StandardFileSystemException :
		public Exception {

	public:

		explicit StandardFileSystemException (
			std::filesystem::filesystem_error const & exception,
			std::source_location const &              location = std::source_location::current()
		) :
			Exception{"Standard.FileSystem", {}, location} {
			auto path_1 = exception.path1().generic_u8string();
			auto path_2 = exception.path2().generic_u8string();
			thiz.m_description.emplace_back(Third::fmt::format("type    : {}", typeid(exception).name()));
			thiz.m_description.emplace_back(Third::fmt::format("message : {}", Detail::string_encoding_native_to_utf8(exception.code().message())));
			thiz.m_description.emplace_back(Third::fmt::format("path_1  : {}", reinterpret_cast<std::string &>(path_1)));
			thiz.m_description.emplace_back(Third::fmt::format("path_2  : {}", reinterpret_cast<std::string &>(path_2)));
		}

	};

	#pragma endregion

	#pragma region function

	inline auto parse_current_exception (
	) -> Exception {
		auto destination = Exception{};
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (Exception & source) {
			destination = source;
		}
		catch (std::filesystem::filesystem_error & source) {
			destination = StandardFileSystemException{source};
		}
		catch (std::system_error & source) {
			destination = StandardSystemException{source};
		}
		catch (std::exception & source) {
			destination = StandardException{source};
		}
		catch (...) {
			destination = UnknownException{};
		}
		return destination;
	}

	#pragma endregion

}
