module;

#include "kernel/common.hpp"

export module twinning.kernel.interface.proxy;
import twinning.kernel.utility;
import twinning.kernel.interface.data;

export namespace Twinning::Kernel::Interface {

	#pragma region type

	class MessageProxy {

	public:

		List<String> value;

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
			List<String> const & value
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
			value.allocate(mbox<Size>(value_size));
			for (auto value_index = std::size_t{0}; value_index < value_size; ++value_index) {
				auto value_item_size = *reinterpret_cast<std::uint32_t *>(instance.data + data_position);
				data_position += sizeof(std::uint32_t);
				auto value_item = make_string(reinterpret_cast<char *>(instance.data + data_position), value_item_size);
				data_position += sizeof(std::uint8_t) * value_item_size;
				value.append(as_moveable(value_item));
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
		) -> Void {
			auto & value = proxy.value;
			auto   data_size = std::size_t{0};
			data_size += sizeof(std::uint32_t);
			for (auto & value_item : value) {
				data_size += sizeof(std::uint32_t);
				data_size += sizeof(std::uint8_t) * value_item.size().value;
				auto data_padding = data_size % sizeof(std::uint32_t);
				if (data_padding != 0) {
					data_padding = sizeof(std::uint32_t) - data_padding;
				}
				data_size += data_padding;
			}
			instance.data = new std::uint8_t[data_size]{};
			instance.size = data_size;
			auto data_position = std::size_t{0};
			auto value_size = value.size().value;
			*reinterpret_cast<std::uint32_t *>(instance.data + data_position) = static_cast<std::uint32_t>(value_size);
			data_position += sizeof(std::uint32_t);
			for (auto value_index = std::size_t{0}; value_index < value_size; ++value_index) {
				auto & value_item = value[mbox<Size>(value_index)];
				auto   value_item_size = value_item.size().value;
				*reinterpret_cast<std::uint32_t *>(instance.data + data_position) = static_cast<std::uint32_t>(value_item_size);
				data_position += sizeof(std::uint32_t);
				std::memcpy(instance.data + data_position, value_item.begin().value, value_item_size);
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
		) -> Void {
			delete[] instance.data;
			instance.data = nullptr;
			instance.size = 0;
			return;
		}

		#pragma endregion

	};

	class ExecutorProxy {

	public:

		std::function<Void  (ExecutorProxy const &, MessageProxy const &, MessageProxy &)> value;

	public:

		#pragma region structor

		~ExecutorProxy (
		) = default;

		// ----------------

		ExecutorProxy (
		) :
			value{[] (auto &, auto &, auto &) -> auto {
				throw IncompleteException{};
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
			std::function<Void  (ExecutorProxy const &, MessageProxy const &, MessageProxy &)> const & value
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
			proxy.value = [self=&as_variable(instance)] (
				ExecutorProxy const & callback_proxy,
				MessageProxy const &  argument_proxy,
				MessageProxy &        result_proxy
			) -> Void {
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
						result_proxy = MessageProxy::parse(*result);
						exception_proxy = MessageProxy::parse(*exception);
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
						throw exception_proxy.value.first();
					}
					return;
				};
			return proxy;
		}

		inline static auto construct (
			Executor &            instance,
			ExecutorProxy const & proxy
		) -> Void {
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
						MessageProxy::construct(*exception, MessageProxy{make_list<String>()});
					}
					#if defined M_build_release
					catch (...) {
						MessageProxy::construct(*exception, MessageProxy{make_list<String>(make_string(parse_current_exception().what()))});
						MessageProxy::construct(*result, MessageProxy{make_list<String>()});
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
		) -> Void {
			assert_test(g_guard.erase(&instance) == 1);
			instance.invoke = nullptr;
			instance.clear = nullptr;
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
