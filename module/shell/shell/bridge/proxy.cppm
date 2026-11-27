module;

#include "shell/common.hpp"

export module twinning.shell.bridge.proxy;
import twinning.shell.utility.exception;
import twinning.shell.utility.finalizer;
import twinning.shell.bridge.data;

export namespace Twinning::Shell::Bridge {

	#pragma region type

	class MessageProxy {

	public:

		std::vector<std::string> value;

	public:

		#pragma region constructor

		~MessageProxy(
		) = default;

		// ----------------

		MessageProxy(
		) :
			value{} {
			return;
		}

		MessageProxy(
			MessageProxy const & that
		) = default;

		MessageProxy(
			MessageProxy && that
		) = default;

		// ----------------

		explicit MessageProxy(
			std::vector<std::string> const & value
		) :
			value{value} {
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			MessageProxy const & that
		) -> MessageProxy & = default;

		auto operator =(
			MessageProxy && that
		) -> MessageProxy & = default;

		#pragma endregion

	public:

		#pragma region convert

		inline static auto parse(
			Message const & instance
		) -> MessageProxy {
			assert_test(instance.data != nullptr && instance.size != 0zu);
			auto proxy = MessageProxy{};
			auto data_position = 0zu;
			auto next_integer = [&]() -> std::size_t {
				auto data_size = sizeof(std::size_t);
				assert_test(data_position <= data_position + data_size && data_position + data_size <= static_cast<std::size_t>(instance.size));
				auto value = std::size_t{};
				std::memmove(&value, instance.data + data_position, data_size);
				data_position += data_size;
				return value;
			};
			auto next_string = [&](std::size_t const & size) -> std::string {
				auto data_size = sizeof(std::uint8_t) * size;
				assert_test(data_position <= data_position + data_size && data_position + data_size <= static_cast<std::size_t>(instance.size));
				auto value = std::string{};
				value.resize(size);
				std::memmove(value.data(), instance.data + data_position, data_size);
				data_position += data_size;
				return value;
			};
			auto & value = proxy.value;
			auto   value_size = next_integer();
			value.reserve(value_size);
			for (auto value_index = 0zu; value_index < value_size; ++value_index) {
				auto value_item_size = next_integer();
				auto value_item = next_string(value_item_size);
				value.emplace_back(std::move(value_item));
			}
			assert_test(data_position == instance.size);
			return proxy;
		}

		inline static auto construct(
			Message &            instance,
			MessageProxy const & proxy
		) -> void {
			assert_test(instance.data == nullptr && instance.size == 0zu);
			auto data_position = 0zu;
			auto next_integer = [&](std::size_t const & value) -> void {
				auto data_size = sizeof(std::size_t);
				assert_test(data_position <= data_position + data_size && data_position + data_size <= instance.size);
				std::memmove(instance.data + data_position, &value, data_size);
				data_position += data_size;
				return;
			};
			auto next_string = [&](std::string const & value) -> void {
				auto data_size = sizeof(std::uint8_t) * value.size();
				assert_test(data_position <= data_position + data_size && data_position + data_size <= instance.size);
				std::memmove(instance.data + data_position, value.data(), data_size);
				data_position += data_size;
				return;
			};
			auto & value = proxy.value;
			auto   data_size = 0zu;
			data_size += sizeof(std::size_t);
			for (auto & value_item : value) {
				data_size += sizeof(std::size_t);
				data_size += sizeof(std::uint8_t) * value_item.size();
			}
			instance.data = new std::uint8_t[data_size]{};
			instance.size = data_size;
			auto value_size = value.size();
			next_integer(value_size);
			for (auto value_index = 0zu; value_index < value_size; ++value_index) {
				auto & value_item = value[value_index];
				auto   value_item_size = value_item.size();
				next_integer(value_item_size);
				next_string(value_item);
			}
			assert_test(data_position == instance.size);
			return;
		}

		inline static auto destruct(
			Message & instance
		) -> void {
			assert_test(instance.data != nullptr && instance.size != 0zu);
			delete[] instance.data;
			instance.data = nullptr;
			instance.size = 0;
			return;
		}

		#pragma endregion

	};

	class ExecutorProxy {

	public:

		std::function<void (ExecutorProxy const & callback, MessageProxy const & argument, MessageProxy & result)> value;

	public:

		#pragma region constructor

		~ExecutorProxy(
		) = default;

		// ----------------

		ExecutorProxy(
		) :
			value{[](auto &, auto &, auto &) -> auto {
				throw std::runtime_error{std::format("UnimplementedException"sv)};
			}} {
			return;
		}

		ExecutorProxy(
			ExecutorProxy const & that
		) = default;

		ExecutorProxy(
			ExecutorProxy && that
		) = default;

		// ----------------

		explicit ExecutorProxy(
			std::function<void (ExecutorProxy const & callback, MessageProxy const & argument, MessageProxy & result)> const & value
		) :
			value{value} {
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			ExecutorProxy const & that
		) -> ExecutorProxy & = default;

		auto operator =(
			ExecutorProxy && that
		) -> ExecutorProxy & = default;

		#pragma endregion

	public:

		#pragma region convert

		inline static auto g_mutex = std::mutex{};

		inline static auto g_guard = std::unordered_map<std::add_pointer_t<Executor>, ExecutorProxy>{};

		// ----------------

		inline static auto parse(
			Executor const & instance
		) -> ExecutorProxy {
			assert_test(instance.invoke != nullptr && instance.clear != nullptr);
			auto proxy = ExecutorProxy{};
			proxy.value = [self = &const_cast<Executor &>(instance)](
				ExecutorProxy const & callback_proxy,
				MessageProxy const &  argument_proxy,
				MessageProxy &        result_proxy
			) -> void {
					auto exception_proxy = MessageProxy{};
					auto callback = std::add_pointer_t<Executor>{nullptr};
					auto argument = std::add_pointer_t<Message>{nullptr};
					auto result = std::add_pointer_t<Message>{nullptr};
					auto exception = std::add_pointer_t<Message>{nullptr};
					callback = new Executor{};
					argument = new Message{};
					result = new Message{};
					exception = new Message{};
					auto _ = make_finalizer(
						[&] {
							delete callback;
							delete argument;
							delete result;
							delete exception;
						}
					);
					ExecutorProxy::construct(*callback, callback_proxy);
					auto _ = make_finalizer(
						[&] {
							ExecutorProxy::destruct(*callback);
						}
					);
					MessageProxy::construct(*argument, argument_proxy);
					auto _ = make_finalizer(
						[&] {
							MessageProxy::destruct(*argument);
						}
					);
					(*self).invoke(self, callback, argument, result, exception);
					auto _ = make_finalizer(
						[&] {
							(*self).clear(self, callback, argument, result, exception);
						}
					);
					result_proxy = MessageProxy::parse(*result);
					exception_proxy = MessageProxy::parse(*exception);
					if (!exception_proxy.value.empty()) {
						throw std::runtime_error{exception_proxy.value.front()};
					}
					return;
				};
			return proxy;
		}

		inline static auto construct(
			Executor &            instance,
			ExecutorProxy const & proxy
		) -> void {
			assert_test(instance.invoke == nullptr && instance.clear == nullptr);
			auto _ = std::lock_guard{g_mutex};
			assert_test(g_guard.try_emplace(&instance, proxy).second);
			instance.invoke = [](
				std::add_pointer_t<Executor> self,
				std::add_pointer_t<Executor> callback,
				std::add_pointer_t<Message>  argument,
				std::add_pointer_t<Message>  result,
				std::add_pointer_t<Message>  exception
			) -> void {
					auto & guard = [&]() -> auto & {
						auto _ = std::lock_guard{g_mutex};
						return g_guard.at(self);
					}();
					#if defined M_build_release
					try
					#endif
					{
						auto callback_proxy = ExecutorProxy::parse(*callback);
						auto argument_proxy = MessageProxy::parse(*argument);
						auto result_proxy = MessageProxy{};
						guard.value(callback_proxy, argument_proxy, result_proxy);
						MessageProxy::construct(*result, result_proxy);
						MessageProxy::construct(*exception, MessageProxy{});
					}
					#if defined M_build_release
					catch (...) {
						MessageProxy::construct(*exception, MessageProxy{{parse_current_exception()}});
						MessageProxy::construct(*result, MessageProxy{});
					}
					#endif
					return;
				};
			instance.clear = [](
				std::add_pointer_t<Executor> self,
				std::add_pointer_t<Executor> callback,
				std::add_pointer_t<Message>  argument,
				std::add_pointer_t<Message>  result,
				std::add_pointer_t<Message>  exception
			) -> void {
					auto & guard = [&]() -> auto & {
						auto _ = std::lock_guard{g_mutex};
						return g_guard.at(self);
					}();
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

		inline static auto destruct(
			Executor & instance
		) -> void {
			assert_test(instance.invoke != nullptr && instance.clear != nullptr);
			auto _ = std::lock_guard{g_mutex};
			assert_test(g_guard.erase(&instance) == 1zu);
			instance.invoke = nullptr;
			instance.clear = nullptr;
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
