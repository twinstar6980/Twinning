module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.exception.exception;

export namespace Twinning::Kernel {

	#pragma region type

	class Exception {

	protected:

		std::string m_title;

		std::vector<std::string> m_description;

		std::source_location m_location;

	public:

		#pragma region structor

		~Exception (
		) = default;

		// ----------------

		Exception (
		) :
			m_title{},
			m_description{},
			m_location{} {
			return;
		}

		Exception (
			Exception const & that
		) = default;

		Exception (
			Exception && that
		) = default;

		// ----------------

		explicit Exception (
			std::string const &              title,
			std::vector<std::string> const & description,
			std::source_location const &     location = std::source_location::current()
		) :
			m_title{title},
			m_description{description},
			m_location{location} {
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Exception const & that
		) -> Exception & = default;

		auto operator = (
			Exception && that
		) -> Exception & = default;

		#pragma endregion

		#pragma region what

		auto what (
		) const -> std::string {
			auto message = std::string{};
			{
				message += std::format("# {}", thiz.m_title);
			}
			for (auto & description : thiz.m_description) {
				message += '\n';
				message += std::format("$ {}", description);
			}
			{
				auto file_path = std::string{thiz.m_location.file_name() + (std::string_view{std::source_location::current().file_name()}.size() - std::string_view{"kernel/utility/exception/exception.cppm"}.size())};
				std::replace(file_path.begin(), file_path.end(), '\\', '/');
				message += '\n';
				message += std::format("@ {}:{}:{} {}", file_path, thiz.m_location.line(), thiz.m_location.column(), thiz.m_location.function_name());
			}
			return message;
		}

		#pragma endregion

	};

	#pragma endregion

}
