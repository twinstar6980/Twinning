module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.binary_stream;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.container.array.static_array;
import twinning.kernel.utility.container.list;
import twinning.kernel.utility.memory.endian;
import twinning.kernel.utility.memory.utility;
import twinning.kernel.utility.binary.basic_binary_view;
import twinning.kernel.utility.binary.basic_binary;
import twinning.kernel.utility.binary.basic_binary_stream_view;
import twinning.kernel.utility.binary.basic_binary_stream_adapter;
import twinning.kernel.utility.binary.binary;
import twinning.kernel.utility.model.record;
import twinning.kernel.utility.model.four_character_code;
import twinning.kernel.utility.miscellaneous.constant_block;
import twinning.kernel.utility.miscellaneous.string_block;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_string_stream_view;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.string.utility;
import twinning.kernel.utility.range.integer_range;

export namespace Twinning::Kernel {

	#pragma region basic

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, BaseBox<TValue>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = BaseBox<TValue>;

		// ----------------

		inline static constexpr auto static_size(
		) -> Size {
			return type_byte_size_of<That>();
		}

		inline static auto size(
			That const & that
		) -> Size {
			return type_byte_size_of<That>();
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			constexpr auto size = type_byte_size_of<TValue>();
			assert_test(thix.reserve() >= size);
			auto data = Memory::to_data(that, cast_box<Memory::Endian>(thix.flag()));
			auto current = thix.current();
			for (auto & index : SizeRange{size}) {
				*current++ = data[index];
			}
			thix.forward(size);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			constexpr auto size = type_byte_size_of<TValue>();
			assert_test(thix.reserve() >= size);
			auto data = StaticArray<Byte, size>{};
			auto current = thix.current();
			for (auto & index : SizeRange{size}) {
				data[index] = *current++;
			}
			that = Memory::from_data<That>(data, cast_box<Memory::Endian>(thix.flag()));
			thix.forward(size);
			return;
		}

	};

	// ----------------

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, BooleanBox<TValue>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = BooleanBox<TValue>;

		// ----------------

		inline static constexpr auto static_size(
		) -> Size {
			return binary_static_size_of<BaseBox<TValue>>();
		}

		inline static auto size(
			That const & that
		) -> Size {
			return binary_size_of<BaseBox<TValue>>(that);
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			using RawValueAlternative = AsSwitch<IsSame<TValue, ZBoolean>, ZIntegerU8, TValue>;
			using RawBox = IntegerBox<std::make_unsigned_t<RawValueAlternative>>;
			thix.write(unsafe_cast<RawBox>(that));
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			using RawValueAlternative = AsSwitch<IsSame<TValue, ZBoolean>, ZIntegerU8, TValue>;
			using RawBox = IntegerBox<std::make_unsigned_t<RawValueAlternative>>;
			thix.read(unsafe_cast<RawBox>(that));
			assert_test((unsafe_cast<RawBox>(that) & ~make_box<RawBox>(0b1)) == make_box<RawBox>(0b0));
			return;
		}

	};

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, NumberBox<TValue>> :
		BasicBinaryStreamAdapter<Byte, BaseBox<TValue>> {

	};

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, IntegerBox<TValue>> :
		BasicBinaryStreamAdapter<Byte, NumberBox<TValue>> {

	};

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, FloaterBox<TValue>> :
		BasicBinaryStreamAdapter<Byte, NumberBox<TValue>> {

	};

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, CharacterBox<TValue>> :
		BasicBinaryStreamAdapter<Byte, IntegerBox<TValue>> {

	};

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, ByteBox<TValue>> :
		BasicBinaryStreamAdapter<Byte, IntegerBox<TValue>> {

	};

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, EnumeratedBox<TValue>> :
		BasicBinaryStreamAdapter<Byte, IntegerBox<TValue>> {

	};

	template <typename TValue> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, EnumerationBox<TValue>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = EnumerationBox<TValue>;

		// ----------------

		inline static constexpr auto static_size(
		) -> Size {
			return binary_static_size_of<typename That::Underlying>();
		}

		inline static auto size(
			That const & that
		) -> Size {
			return binary_size_of<typename That::Underlying>(that.underlying());
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(that.as_underlying());
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			thix.read(that.as_underlying());
			return;
		}

	};

	// ----------------

	template <>
	struct BasicBinaryStreamAdapter<Byte, Character> :
		BasicBinaryStreamAdapter<Byte, CharacterN> {

	};

	template <>
	struct BasicBinaryStreamAdapter<Byte, Unicode> :
		BasicBinaryStreamAdapter<Byte, Character32> {

	};

	template <>
	struct BasicBinaryStreamAdapter<Byte, Byte> :
		BasicBinaryStreamAdapter<Byte, Byte8> {

	};

	template <>
	struct BasicBinaryStreamAdapter<Byte, Enumerated> :
		BasicBinaryStreamAdapter<Byte, Enumerated8> {

	};

	template <typename TType> requires
		AutomaticConstraint
		&& (IsEnumerationBox<TType>)
		&& (IsDerivedFrom<TType, Enumeration<typename TType::Value>>)
		&& (!IsSame<TType, Enumeration<typename TType::Value>>)
	struct BasicBinaryStreamAdapter<Byte, TType> :
		BasicBinaryStreamAdapter<Byte, EnumerationBox<typename TType::Value>> {

	};

	#pragma endregion

	#pragma region container

	template <typename TElement, auto t_constant> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, ArrayView<TElement, t_constant>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = ArrayView<TElement, t_constant>;

		// ----------------

		inline static constexpr auto static_size(
			Size const & size
		) -> Size {
			return binary_static_size_of<TElement>() * size;
		}

		inline static auto size(
			That const & that
		) -> Size {
			auto result = 0_sz;
			for (auto & element : that) {
				result += binary_size_of(element);
			}
			return result;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				thix.write(element);
			}
			return;
		}

		inline static auto read(
			ThisInput &  thix,
			That const & that
		) -> Void requires
			(!t_constant.value) {
			for (auto & element : that) {
				thix.read(element);
			}
			return;
		}

	};

	template <typename TElement, auto t_size> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, StaticArray<TElement, t_size>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = StaticArray<TElement, t_size>;

		// ----------------

		inline static constexpr auto static_size(
		) -> Size {
			return binary_static_size_of<TElement>() * t_size;
		}

		inline static auto size(
			That const & that
		) -> Size {
			return binary_size_of(that.view());
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(that.view());
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			thix.read(that.view());
			return;
		}

	};

	template <typename TElement> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, List<TElement>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = List<TElement>;

		// ----------------

		inline static constexpr auto static_size(
			Size const & size
		) -> Size {
			return binary_static_size_of<TElement>() * size;
		}

		inline static auto size(
			That const & that
		) -> Size {
			auto result = 0_sz;
			for (auto & element : that) {
				result += binary_size_of(element);
			}
			return result;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				thix.write(element);
			}
			return;
		}

		inline static auto read(
			ThisInput &  thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				thix.read(element);
			}
			return;
		}

		inline static auto read(
			ThisInput &  thix,
			That &       that,
			Size const & size
		) -> Void {
			that.allocate_full(size);
			for (auto & element : that) {
				thix.read(element);
			}
			return;
		}

	};

	#pragma endregion

	#pragma region binary

	template <typename TElement, auto t_constant> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, BasicBinaryView<TElement, t_constant>> :
		BasicBinaryStreamAdapter<Byte, ArrayView<TElement, t_constant>> {

	};

	template <typename TElement> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, BasicBinary<TElement>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = BasicBinary<TElement>;

		// ----------------

		inline static constexpr auto static_size(
			Size const & size
		) -> Size {
			return binary_static_size_of<TElement>() * size;
		}

		inline static auto size(
			That const & that
		) -> Size {
			return binary_size_of(that.as_view());
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(that.as_view());
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			thix.read(that.as_view());
			return;
		}

		inline static auto read(
			ThisInput &  thix,
			That &       that,
			Size const & size
		) -> Void {
			that.allocate_full(size);
			thix.read(that.as_view());
			return;
		}

	};

	#pragma endregion

	#pragma region string

	template <typename TElement, auto t_constant> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, BasicStringView<TElement, t_constant>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = BasicStringView<TElement, t_constant>;

		// ----------------

		inline static constexpr auto static_size(
			Size const & size
		) -> Size {
			return binary_static_size_of<TElement>() * size;
		}

		inline static auto size(
			That const & that
		) -> Size {
			auto result = 0_sz;
			for (auto & element : that) {
				result += binary_size_of(element);
			}
			return result;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				thix.write(element);
			}
			return;
		}

		inline static auto read(
			ThisInput &  thix,
			That const & that
		) -> Void requires
			(!t_constant.value) {
			for (auto & element : that) {
				thix.read(element);
			}
			return;
		}

	};

	template <typename TElement> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, BasicString<TElement>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = BasicString<TElement>;

		// ----------------

		inline static constexpr auto static_size(
			Size const & size
		) -> Size {
			return binary_static_size_of<TElement>() * size;
		}

		inline static auto size(
			That const & that
		) -> Size {
			return binary_size_of(that.as_view());
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(that.as_view());
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			thix.read(that.as_view());
			return;
		}

		inline static auto read(
			ThisInput &  thix,
			That &       that,
			Size const & size
		) -> Void {
			that.allocate_full(size);
			thix.read(that.as_view());
			return;
		}

	};

	#pragma endregion

	#pragma region record

	template <typename TType> requires
		AutomaticConstraint
		&& (IsDerivedFrom<TType, Record>)
	struct BasicBinaryStreamAdapter<Byte, TType> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = TType;

		using FieldPackage = TType::Reflection::MemberVariable;

		// ----------------

		inline static constexpr auto static_size(
		) -> Size {
			auto result = 0_sz;
			Generalization::each<FieldPackage>(
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					result += binary_static_size_of<AsPure<decltype(TField::value_of(declare<That const &>()))>>();
				}
			);
			return result;
		}

		inline static auto size(
			That const & that
		) -> Size {
			auto result = 0_sz;
			Generalization::each<FieldPackage>(
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					result += binary_size_of(TField::value_of(that));
				}
			);
			return result;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			Generalization::each<FieldPackage>(
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					thix.write(TField::value_of(that));
				}
			);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			Generalization::each<FieldPackage>(
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					thix.read(TField::value_of(that));
				}
			);
			return;
		}

	};

	#pragma endregion

	#pragma region miscellaneous

	template <>
	struct BasicBinaryStreamAdapter<Byte, FourCharacterCode> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = FourCharacterCode;

		// ----------------

		inline static constexpr auto static_size(
		) -> Size {
			auto result = 0_sz;
			result += binary_static_size_of<decltype(That::first)>();
			result += binary_static_size_of<decltype(That::second)>();
			result += binary_static_size_of<decltype(That::third)>();
			result += binary_static_size_of<decltype(That::fourth)>();
			return result;
		}

		inline static auto size(
			That const & that
		) -> Size {
			auto result = 0_sz;
			result += binary_size_of(that.first);
			result += binary_size_of(that.second);
			result += binary_size_of(that.third);
			result += binary_size_of(that.fourth);
			return result;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(that.first);
			thix.write(that.second);
			thix.write(that.third);
			thix.write(that.fourth);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			thix.read(that.first);
			thix.read(that.second);
			thix.read(that.third);
			thix.read(that.fourth);
			return;
		}

	};

	// ----------------

	template <auto t_value> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, ConstantBlock<t_value>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = ConstantBlock<t_value>;

		// ----------------

		inline static constexpr auto static_size(
		) -> Size {
			return binary_static_size_of<AsUnmakeConstant<decltype(t_value)>>();
		}

		inline static auto size(
			That const & that
		) -> Size {
			return binary_size_of(t_value);
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write_constant(t_value);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			thix.read_constant(t_value);
			return;
		}

	};

	// ----------------

	template <typename TLength> requires
		AutomaticConstraint
	struct BasicBinaryStreamAdapter<Byte, StringBlock<TLength>> {

		using ThisInput = InputBinaryStreamView;

		using ThisOutput = OutputBinaryStreamView;

		using That = StringBlock<TLength>;

		// ----------------

		inline static auto size(
			That const & that
		) -> Size {
			if constexpr (IsVoid<TLength>) {
				return binary_size_of(that.value) + binary_static_size_of<Character>();
			}
			else {
				return binary_static_size_of<TLength>() + binary_size_of(that.value);
			}
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			if constexpr (IsVoid<TLength>) {
				StringUtility::write_string_until(unsafe_cast<OutputStringStreamView>(thix), that.value, StringUtility::k_null_character);
				unsafe_cast<OutputStringStreamView>(thix).write_constant(StringUtility::k_null_character);
			}
			else {
				thix.write(cast_box<TLength>(that.value.size()));
				thix.write(that.value);
			}
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			if constexpr (IsVoid<TLength>) {
				auto that_view = ConstantStringView{};
				StringUtility::read_string_until(unsafe_cast<InputStringStreamView>(thix), that_view, StringUtility::k_null_character);
				unsafe_cast<InputStringStreamView>(thix).read_constant(StringUtility::k_null_character);
				that.value = that_view;
			}
			else {
				auto length = thix.read_of<TLength>();
				thix.read(that.value, cast_box<Size>(length));
			}
			return;
		}

	};

	#pragma endregion

}
