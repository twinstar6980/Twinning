module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.binary.binary;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.stream.stream_mode;
import twinning.kernel.utility.binary.basic_binary_view;
import twinning.kernel.utility.binary.basic_binary;
import twinning.kernel.utility.binary.basic_binary_stream_flag;
import twinning.kernel.utility.binary.basic_binary_stream_view;
import twinning.kernel.utility.binary.basic_binary_stream_adapter;
import twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.math.utility;

export namespace Twinning::Kernel {

	#pragma region alias

	template <auto t_constant> requires
		AutomaticConstraint
	using BinaryView = BasicBinaryView<Byte, t_constant>;

	using VariableBinaryView = VariableBasicBinaryView<Byte>;

	using ConstantBinaryView = ConstantBasicBinaryView<Byte>;

	using Binary = BasicBinary<Byte>;

	using BinaryStreamFlag = BasicBinaryStreamFlag;

	template <auto t_mode> requires
		AutomaticConstraint
	using BinaryStreamView = BasicBinaryStreamView<Byte, t_mode>;

	using AccessBinaryStreamView = AccessBasicBinaryStreamView<Byte>;

	using InputBinaryStreamView = InputBasicBinaryStreamView<Byte>;

	using OutputBinaryStreamView = OutputBasicBinaryStreamView<Byte>;

	#pragma endregion

	#pragma region utility

	template <typename TSourceElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TSourceElement>>
		&& (IsSameOf<t_constant, Boolean>)
	inline auto to_binary_view(
		ArrayView<TSourceElement, t_constant> const & source
	) -> BinaryView<t_constant> {
		return BinaryView<t_constant>{cast_pointer<Byte>(source.data()), source.size() * type_byte_size_of<AsPure<TSourceElement>>()};
	}

	// ----------------

	inline auto use_binary_buffer(
		Binary &     container,
		Size const & size
	) -> VariableBinaryView {
		if (container.size() < size) {
			container.allocate_full(size);
		}
		return container.head(size);
	}

	// ----------------

	template <auto t_mode, auto t_mode_clone> requires
		CategoryConstraint<>
		&& (IsSameOf<t_mode, StreamMode>)
		&& (IsSameOf<t_mode_clone, StreamMode>)
	inline auto like_binary_stream(
		BinaryStreamView<t_mode_clone> const &               stream,
		typename BinaryStreamView<t_mode>::ArrayView const & view
	) -> BinaryStreamView<t_mode> {
		return BinaryStreamView<t_mode>{view, 0_sz, stream.flag()};
	}

	// ----------------

	template <typename TValue, typename ... TOption> requires
		CategoryConstraint<IsPureInstance<TValue> && IsValid<TOption ...>>
	inline constexpr auto binary_static_size_of(
		TOption && ... option
	) -> Size {
		return BasicBinaryStreamAdapter<Byte, TValue>::static_size(as_forward<TOption>(option) ...);
	}

	template <typename TValue, typename ... TOption> requires
		CategoryConstraint<IsPureInstance<TValue> && IsValid<TOption ...>>
	inline auto binary_size_of(
		TValue const & value,
		TOption && ... option
	) -> Size {
		return BasicBinaryStreamAdapter<Byte, TValue>::size(value, as_forward<TOption>(option) ...);
	}

	#pragma endregion

}
