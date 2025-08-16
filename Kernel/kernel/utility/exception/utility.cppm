module;

#include <typeinfo>
#include "kernel/common.hpp"

export module twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.exception.exception;
import twinning.kernel.utility.miscellaneous.system_native_string;

export namespace Twinning::Kernel {

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
			thiz.m_description.emplace_back(std::format("message: {}", message));
		}

	};

	// ----------------

	class UnreachableException :
		public Exception {

	public:

		explicit UnreachableException (
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Unreachable", {}, location} {
		}

	};

	class UnimplementedException :
		public Exception {

	public:

		explicit UnimplementedException (
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Unimplemented", {}, location} {
		}

	};

	class UnsupportedException :
		public Exception {

	public:

		explicit UnsupportedException (
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"Unsupported", {}, location} {
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
			thiz.m_description.emplace_back(std::format("expression: {}", expression));
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
			thiz.m_description.emplace_back(std::format("target: {}", target));
			thiz.m_description.emplace_back(std::format("message: {}", message));
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
			thiz.m_description.emplace_back(std::format("position: {:X}h", position));
			thiz.m_description.emplace_back(std::format("message: {}", message));
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
			thiz.m_description.emplace_back(std::format("type: {}", typeid(exception).name()));
			thiz.m_description.emplace_back(std::format("message: {}", exception.what()));
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
			thiz.m_description.emplace_back(std::format("type: {}", typeid(exception).name()));
			thiz.m_description.emplace_back(std::format("message: {}", SystemNativeString::utf8_from_native(exception.code().message())));
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
			thiz.m_description.emplace_back(std::format("type: {}", typeid(exception).name()));
			thiz.m_description.emplace_back(std::format("message: {}", SystemNativeString::utf8_from_native(exception.code().message())));
			thiz.m_description.emplace_back(std::format("path_1: {}", reinterpret_cast<std::string &>(path_1)));
			thiz.m_description.emplace_back(std::format("path_2: {}", reinterpret_cast<std::string &>(path_2)));
		}

	};

	#pragma endregion

	#pragma region function

	inline auto parse_current_exception (
		std::source_location const & location = std::source_location::current()
	) -> Exception {
		auto result = Exception{};
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (Exception & e) {
			result = e;
		}
		catch (std::filesystem::filesystem_error & e) {
			result = StandardFileSystemException{e, location};
		}
		catch (std::system_error & e) {
			result = StandardSystemException{e, location};
		}
		catch (std::exception & e) {
			result = StandardException{e, location};
		}
		catch (...) {
			result = UnknownException{location};
		}
		return result;
	}

	#pragma endregion

}
