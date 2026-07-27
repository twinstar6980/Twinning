module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.exception.exception;
import twinning.kernel.utility.miscellaneous.low_level.compiler;
import twinning.kernel.utility.miscellaneous.low_level.system_native_string;

export namespace Twinning::Kernel {

	#pragma region type

	class UnknownException :
		public Exception {

	public:

		explicit UnknownException(
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"UnknownException", {}, location} {
			return;
		}

	};

	class UnnamedException :
		public Exception {

	public:

		explicit UnnamedException(
			std::string const &          message,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"UnnamedException", {}, location} {
			thiz.m_description.emplace_back(std::format("message: {}", message));
			return;
		}

	};

	// ----------------

	class UnreachableException :
		public Exception {

	public:

		explicit UnreachableException(
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"UnreachableException", {}, location} {
			return;
		}

	};

	class UnimplementedException :
		public Exception {

	public:

		explicit UnimplementedException(
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"UnimplementedException", {}, location} {
			return;
		}

	};

	class UnsupportedException :
		public Exception {

	public:

		explicit UnsupportedException(
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"UnsupportedException", {}, location} {
			return;
		}

	};

	// ----------------

	class AssertionException :
		public Exception {

	public:

		explicit AssertionException(
			std::string const &          expression,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"AssertionException", {}, location} {
			thiz.m_description.emplace_back(std::format("expression: {}", expression));
			return;
		}

	};

	// ----------------

	class ConversionException :
		public Exception {

	public:

		explicit ConversionException(
			std::type_info const &       source,
			std::type_info const &       destination,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"ConversionException", {}, location} {
			thiz.m_description.emplace_back(std::format("source: {}", LowLevel::Compiler::parse_mangled_name(std::string_view{source.name()})));
			thiz.m_description.emplace_back(std::format("destination: {}", LowLevel::Compiler::parse_mangled_name(std::string_view{destination.name()})));
			return;
		}

	};

	// ----------------

	class InvocationException :
		public Exception {

	public:

		explicit InvocationException(
			std::string const &          target,
			std::string const &          message,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"InvocationException", {}, location} {
			thiz.m_description.emplace_back(std::format("target: {}", target));
			thiz.m_description.emplace_back(std::format("message: {}", message));
			return;
		}

	};

	// ----------------

	class SyntaxException :
		public Exception {

	public:

		explicit SyntaxException(
			std::size_t const &          position,
			std::string const &          message,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"SyntaxException", {}, location} {
			thiz.m_description.emplace_back(std::format("position: {:X}h", position));
			thiz.m_description.emplace_back(std::format("message: {}", message));
			return;
		}

	};

	// ----------------

	class StandardException :
		public Exception {

	public:

		explicit StandardException(
			std::exception const &       exception,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"StandardException", {}, location} {
			thiz.m_description.emplace_back(std::format("type: {}", LowLevel::Compiler::parse_mangled_name(std::string_view{typeid(exception).name()})));
			thiz.m_description.emplace_back(std::format("message: {}", exception.what()));
			return;
		}

	};

	class StandardSystemException :
		public Exception {

	public:

		explicit StandardSystemException(
			std::system_error const &    exception,
			std::source_location const & location = std::source_location::current()
		) :
			Exception{"StandardSystemException", {}, location} {
			thiz.m_description.emplace_back(std::format("type: {}", LowLevel::Compiler::parse_mangled_name(std::string_view{typeid(exception).name()})));
			thiz.m_description.emplace_back(std::format("message: {}", LowLevel::SystemNativeString::utf8_from_native(exception.code().message())));
			return;
		}

	};

	class StandardFileSystemException :
		public Exception {

	public:

		explicit StandardFileSystemException(
			std::filesystem::filesystem_error const & exception,
			std::source_location const &              location = std::source_location::current()
		) :
			Exception{"StandardFileSystemException", {}, location} {
			auto path_1 = exception.path1().generic_u8string();
			auto path_2 = exception.path2().generic_u8string();
			thiz.m_description.emplace_back(std::format("type: {}", LowLevel::Compiler::parse_mangled_name(std::string_view{typeid(exception).name()})));
			thiz.m_description.emplace_back(std::format("message: {}", LowLevel::SystemNativeString::utf8_from_native(exception.code().message())));
			thiz.m_description.emplace_back(std::format("path_1: {}", reinterpret_cast<std::string &>(path_1)));
			thiz.m_description.emplace_back(std::format("path_2: {}", reinterpret_cast<std::string &>(path_2)));
			return;
		}

	};

	#pragma endregion

	#pragma region function

	inline auto parse_current_exception(
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
