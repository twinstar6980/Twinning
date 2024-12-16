module;

#include "shell/common.hpp"

export module twinning.shell.bridge.proxy;
import twinning.shell.utility.exception;
import twinning.shell.bridge.data;

export namespace Twinning::Shell::Bridge {

	#pragma region type

	class MessageProxy {

	public:

		std::vector<std::string> value;

	public:

		#pragma region structor

		~MessageProxy (
		) = default;

		// ----------------

		MessageProxy (
		) :
			value{} {
		}

		MessageProxy (
			MessageProxy const & that
		) = default;

		MessageProxy (
			MessageProxy && that
		) = default;

		// ----------------

		explicit MessageProxy (
			std::vector<std::string> const & value
		) :
			value{value} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			MessageProxy const & that
		) -> MessageProxy & = default;

		auto operator = (
			MessageProxy && that
		) -> MessageProxy & = default;

		#pragma endregion

	public:

		#pragma region convert

		inline static auto parse (
			Message const & instance
		) -> MessageProxy {
			auto   proxy = MessageProxy{};
			auto & value = proxy.value;
			auto   data_position = std::size_t{0};
			auto   value_size = *reinterpret_cast<std::uint32_t *>(instance.data + data_position);
			data_position += sizeof(std::uint32_t);
			value.reserve(value_size);
			for (auto value_index = std::size_t{0}; value_index < value_size; ++value_index) {
				auto value_item_size = *reinterpret_cast<std::uint32_t *>(instance.data + data_position);
				data_position += sizeof(std::uint32_t);
				auto value_item = std::string{reinterpret_cast<char *>(instance.data + data_position), value_item_size};
				data_position += sizeof(std::uint8_t) * value_item_size;
				value.emplace_back(std::move(value_item));
				auto data_padding = data_position % sizeof(std::uint32_t);
				if (data_padding != 0) {
					data_padding = sizeof(std::uint32_t) - data_padding;
				}
				data_position += data_padding;
			}
			assert_test(data_position == instance.size);
			return proxy;
		}

		inline static auto construct (
			Message &            instance,
			MessageProxy const & proxy
		) -> void {
			auto & value = proxy.value;
			auto   data_size = std::size_t{0};
			data_size += sizeof(std::uint32_t);
			for (auto & value_item : value) {
				data_size += sizeof(std::uint32_t);
				data_size += sizeof(std::uint8_t) * value_item.size();
				auto data_padding = data_size % sizeof(std::uint32_t);
				if (data_padding != 0) {
					data_padding = sizeof(std::uint32_t) - data_padding;
				}
				data_size += data_padding;
			}
			instance.data = new std::uint8_t[data_size]{};
			instance.size = data_size;
			auto data_position = std::size_t{0};
			auto value_size = value.size();
			*reinterpret_cast<std::uint32_t *>(instance.data + data_position) = static_cast<std::uint32_t>(value_size);
			data_position += sizeof(std::uint32_t);
			for (auto value_index = std::size_t{0}; value_index < value_size; ++value_index) {
				auto & value_item = value[value_index];
				auto   value_item_size = value_item.size();
				*reinterpret_cast<std::uint32_t *>(instance.data + data_position) = static_cast<std::uint32_t>(value_item_size);
				data_position += sizeof(std::uint32_t);
				std::memcpy(instance.data + data_position, value_item.data(), value_item_size);
				data_position += sizeof(std::uint8_t) * value_item_size;
				auto data_padding = data_position % sizeof(std::uint32_t);
				if (data_padding != 0) {
					data_padding = sizeof(std::uint32_t) - data_padding;
				}
				data_position += data_padding;
			}
			assert_test(data_position == instance.size);
			return;
		}

		inline static auto destruct (
			Message & instance
		) -> void {
			delete[] instance.data;
			instance.data = nullptr;
			instance.size = 0;
			return;
		}

		#pragma endregion

	};

	class ExecutorProxy {

	public:

		std::function<void  (ExecutorProxy const &, MessageProxy const &, MessageProxy &)> value;

	public:

		#pragma region structor

		~ExecutorProxy (
		) = default;

		// ----------------

		ExecutorProxy (
		) :
			value{[] (auto &, auto &, auto &) -> auto {
				throw std::runtime_error{"incomplete"s};
				return;
			}} {
		}

		ExecutorProxy (
			ExecutorProxy const & that
		) = default;

		ExecutorProxy (
			ExecutorProxy && that
		) = default;

		// ----------------

		explicit ExecutorProxy (
			std::function<void  (ExecutorProxy const &, MessageProxy const &, MessageProxy &)> const & value
		) :
			value{value} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			ExecutorProxy const & that
		) -> ExecutorProxy & = default;

		auto operator = (
			ExecutorProxy && that
		) -> ExecutorProxy & = default;

		#pragma endregion

	public:

		#pragma region convert

		inline static auto g_guard = std::unordered_map<Executor *, std::unique_ptr<ExecutorProxy>>{};

		// ----------------

		inline static auto parse (
			Executor const & instance
		) -> ExecutorProxy {
			auto proxy = ExecutorProxy{};
			proxy.value = [self=&const_cast<Executor &>(instance)] (
				ExecutorProxy const & callback_proxy,
				MessageProxy const &  argument_proxy,
				MessageProxy &        result_proxy
			) -> void {
					auto exception_proxy = MessageProxy{};
					auto callback = std::add_pointer_t<Executor>{nullptr};
					auto argument = std::add_pointer_t<Message>{nullptr};
					auto result = std::add_pointer_t<Message>{nullptr};
					auto exception = std::add_pointer_t<Message>{nullptr};
					{
						callback = new Executor{};
						argument = new Message{};
						result = new Message{};
						exception = new Message{};
					}
					{
						ExecutorProxy::construct(*callback, callback_proxy);
						MessageProxy::construct(*argument, argument_proxy);
					}
					{
						(*self).invoke(self, callback, argument, result, exception);
						result_proxy.value = MessageProxy::parse(*result).value;
						exception_proxy.value = MessageProxy::parse(*exception).value;
						(*self).clear(self, callback, argument, result, exception);
					}
					{
						ExecutorProxy::destruct(*callback);
						MessageProxy::destruct(*argument);
					}
					{
						delete callback;
						delete argument;
						delete result;
						delete exception;
					}
					if (!exception_proxy.value.empty()) {
						throw exception_proxy.value.front();
					}
					return;
				};
			return proxy;
		}

		inline static auto construct (
			Executor &            instance,
			ExecutorProxy const & proxy
		) -> void {
			g_guard.emplace(&instance, std::make_unique<ExecutorProxy>(proxy));
			instance.invoke = [] (
				Executor * self,
				Executor * callback,
				Message *  argument,
				Message *  result,
				Message *  exception
			) -> void {
					auto & guard = *g_guard.at(self);
					#if defined M_build_release
					try
					#endif
					{
						auto callback_proxy = ExecutorProxy::parse(*callback);
						auto argument_proxy = MessageProxy::parse(*argument);
						auto result_proxy = MessageProxy{};
						guard.value(callback_proxy, argument_proxy, result_proxy);
						MessageProxy::construct(*result, result_proxy);
						MessageProxy::construct(*exception, MessageProxy{{}});
					}
					#if defined M_build_release
					catch (...) {
						MessageProxy::construct(*exception, MessageProxy{{parse_current_exception()}});
						MessageProxy::construct(*result, MessageProxy{{}});
					}
					#endif
					return;
				};
			instance.clear = [] (
				Executor * self,
				Executor * callback,
				Message *  argument,
				Message *  result,
				Message *  exception
			) -> void {
					auto & guard = *g_guard.at(self);
					if (result != nullptr) {
						MessageProxy::destruct(*result);
					}
					if (exception != nullptr) {
						MessageProxy::destruct(*exception);
					}
					return;
				};
			return;
		}

		inline static auto destruct (
			Executor & instance
		) -> void {
			assert_test(g_guard.erase(&instance) == 1);
			instance.invoke = nullptr;
			instance.clear = nullptr;
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
