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
			List<String> const & value
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
			assert_test(instance.data != nullptr && instance.size != 0_szz);
			auto proxy = MessageProxy{};
			auto data_position = 0_sz;
			auto next_integer = [&]() -> Size {
				auto data_size = type_byte_size_of<ZSize>();
				assert_test(data_position <= data_position + data_size && data_position + data_size <= make_box<Size>(instance.size));
				auto value = Size{};
				Memory::copy(make_pointer(&value), make_pointer(instance.data) + data_position, data_size);
				data_position += data_size;
				return value;
			};
			auto next_string = [&](Size const & size) -> String {
				auto data_size = type_byte_size_of<ZCharacter8>() * size;
				assert_test(data_position <= data_position + data_size && data_position + data_size <= make_box<Size>(instance.size));
				auto value = String{};
				value.allocate_full(size);
				Memory::copy(value.data(), make_pointer(instance.data) + data_position, data_size);
				data_position += data_size;
				return value;
			};
			auto & value = proxy.value;
			auto   value_size = next_integer();
			value.allocate(value_size);
			for (auto & value_index : SizeRange{value_size}) {
				auto value_item_size = next_integer();
				auto value_item = next_string(value_item_size);
				value.append(as_moveable(value_item));
			}
			assert_test(data_position == make_box<Size>(instance.size));
			return proxy;
		}

		inline static auto construct(
			Message &            instance,
			MessageProxy const & proxy
		) -> Void {
			assert_test(instance.data == nullptr && instance.size == 0_szz);
			auto data_position = 0_sz;
			auto next_integer = [&](Size const & value) -> Void {
				auto data_size = type_byte_size_of<ZSize>();
				assert_test(data_position <= data_position + data_size && data_position + data_size <= make_box<Size>(instance.size));
				Memory::copy(make_pointer(instance.data) + data_position, make_pointer(&value), data_size);
				data_position += data_size;
				return;
			};
			auto next_string = [&](String const & value) -> Void {
				auto data_size = type_byte_size_of<ZCharacter8>() * value.size();
				assert_test(data_position <= data_position + data_size && data_position + data_size <= make_box<Size>(instance.size));
				Memory::copy(make_pointer(instance.data) + data_position, value.data(), data_size);
				data_position += data_size;
				return;
			};
			auto & value = proxy.value;
			auto   data_size = 0_sz;
			data_size += type_byte_size_of<ZSize>();
			for (auto & value_item : value) {
				data_size += type_byte_size_of<ZSize>();
				data_size += type_byte_size_of<ZCharacter8>() * value_item.size();
			}
			instance.data = new ZByte[data_size.value]{};
			instance.size = data_size.value;
			auto value_size = value.size();
			next_integer(value_size);
			for (auto & value_index : SizeRange{value_size}) {
				auto & value_item = value[value_index];
				auto   value_item_size = value_item.size();
				next_integer(value_item_size);
				next_string(value_item);
			}
			assert_test(data_position == make_box<Size>(instance.size));
			return;
		}

		inline static auto destruct(
			Message & instance
		) -> Void {
			assert_test(instance.data != nullptr && instance.size != 0_szz);
			delete[] instance.data;
			instance.data = nullptr;
			instance.size = 0_szz;
			return;
		}

		#pragma endregion

	};

	class ExecutorProxy {

	public:

		Function<Void, ExecutorProxy const &, MessageProxy const &, MessageProxy &> value;

	public:

		#pragma region constructor

		~ExecutorProxy(
		) = default;

		// ----------------

		ExecutorProxy(
		) :
			value{[](auto &, auto &, auto &) -> auto {
				throw UnimplementedException{};
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
			Function<Void, ExecutorProxy const &, MessageProxy const &, MessageProxy &> const & value
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

		inline static auto g_guard = std::unordered_map<ZPointer<Executor>, ExecutorProxy>{};

		// ----------------

		inline static auto parse(
			Executor const & instance
		) -> ExecutorProxy {
			assert_test(instance.invoke != nullptr && instance.clear != nullptr);
			auto proxy = ExecutorProxy{};
			proxy.value = Function<Void, ExecutorProxy const &, MessageProxy const &, MessageProxy &>{[self = &as_variable(instance)](
				ExecutorProxy const & callback_proxy,
				MessageProxy const &  argument_proxy,
				MessageProxy &        result_proxy
			) -> Void {
					auto exception_proxy = MessageProxy{};
					auto callback = ZPointer<Executor>{nullptr};
					auto argument = ZPointer<Message>{nullptr};
					auto result = ZPointer<Message>{nullptr};
					auto exception = ZPointer<Message>{nullptr};
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
						throw exception_proxy.value.first();
					}
					return;
				}};
			return proxy;
		}

		inline static auto construct(
			Executor &            instance,
			ExecutorProxy const & proxy
		) -> Void {
			assert_test(instance.invoke == nullptr && instance.clear == nullptr);
			auto _ = std::lock_guard{g_mutex};
			assert_test(g_guard.try_emplace(&instance, proxy).second);
			instance.invoke = [](
				ZPointer<Executor> self,
				ZPointer<Executor> callback,
				ZPointer<Message>  argument,
				ZPointer<Message>  result,
				ZPointer<Message>  exception
			) -> Void {
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
						guard.value.call(callback_proxy, argument_proxy, result_proxy);
						MessageProxy::construct(*result, result_proxy);
						MessageProxy::construct(*exception, MessageProxy{});
					}
					#if defined M_build_release
					catch (...) {
						MessageProxy::construct(*exception, MessageProxy{make_list<String>(from_std_string(parse_current_exception().what()))});
						MessageProxy::construct(*result, MessageProxy{});
					}
					#endif
					return;
				};
			instance.clear = [](
				ZPointer<Executor> self,
				ZPointer<Executor> callback,
				ZPointer<Message>  argument,
				ZPointer<Message>  result,
				ZPointer<Message>  exception
			) -> Void {
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
		) -> Void {
			assert_test(instance.invoke != nullptr && instance.clear != nullptr);
			auto _ = std::lock_guard{g_mutex};
			assert_test(g_guard.erase(&instance) == 1_szz);
			instance.invoke = nullptr;
			instance.clear = nullptr;
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
