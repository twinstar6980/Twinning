module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.render_effect.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.render_effect.version;
import twinning.kernel.tool.popcap.render_effect.definition;
import twinning.kernel.tool.popcap.render_effect.common;
import twinning.kernel.tool.common.byte_stream;

export namespace Twinning::Kernel::Tool::PopCap::RenderEffect {

	template <auto version> requires (check_version(version, {}, {}))
	struct Decode :
		Common<version>,
		CommonByteStreamExchangeForInput {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		// ----------------

		inline static auto exchange_block (
			IByteStreamView &             data,
			typename Definition::Block1 & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_4);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_5);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_6);
			return;
		}

		inline static auto exchange_block (
			IByteStreamView &             data,
			typename Definition::Block2 & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			if constexpr (check_version(version, {}, {1, 2})) {
				exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			}
			return;
		}

		inline static auto exchange_block (
			IByteStreamView &             data,
			typename Definition::Block3 & value,
			IByteStreamView &             string_chunk_data,
			Size &                        string_chunk_size
		) -> Void {
			auto string_offset = Size{};
			auto string_size = Size{};
			exchange_size_fixed<IntegerU32>(data, string_size);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			exchange_size_fixed<IntegerU32>(data, string_offset);
			string_chunk_data.set_position(string_offset);
			exchange_string_block<Void>(string_chunk_data, value.string);
			assert_test(value.string.size() == string_size);
			string_chunk_size = maximum(string_chunk_size, string_chunk_data.position());
			return;
		}

		inline static auto exchange_block (
			IByteStreamView &             data,
			typename Definition::Block4 & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_4);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_5);
			return;
		}

		inline static auto exchange_block (
			IByteStreamView &             data,
			typename Definition::Block5 & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_4);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_5);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_6);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_7);
			return;
		}

		inline static auto exchange_block (
			IByteStreamView &             data,
			typename Definition::Block6 & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_4);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_5);
			return;
		}

		inline static auto exchange_block (
			IByteStreamView &             data,
			typename Definition::Block7 & value
		) -> Void {
			if constexpr (check_version(version, {}, {3})) {
				exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
				exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			}
			return;
		}

		inline static auto exchange_block (
			IByteStreamView &             data,
			typename Definition::Block8 & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			if constexpr (check_version(version, {}, {3})) {
				exchange_integer_fixed<IntegerU32>(data, value.unknown_4);
				exchange_integer_fixed<IntegerU32>(data, value.unknown_5);
			}
			exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			return;
		}

		// ----------------

		template <typename Block, typename ... ExtraArgument> requires
			CategoryConstraint<IsPureInstance<Block> && IsValid<ExtraArgument ...>>
		inline static auto exchange_section (
			IByteStreamView &    section_information_data,
			IByteStreamView &    section_data,
			List<Block> &        section_value,
			Size const &         section_index,
			Size const &         block_size_constant,
			ExtraArgument && ... extra_argument
		) -> Void {
			auto block_count = Size{};
			auto offset = Size{};
			auto block_size = Size{};
			section_information_data.set_position(bs_static_size<List<IntegerU32>>(3_sz) * section_index);
			exchange_size_fixed<IntegerU32>(section_information_data, block_count);
			exchange_size_fixed<IntegerU32>(section_information_data, offset);
			exchange_size_fixed<IntegerU32>(section_information_data, block_size);
			assert_test(block_size == block_size_constant);
			section_data.set_position(offset);
			section_value.allocate_full(block_count);
			for (auto & value : section_value) {
				exchange_block(section_data, value, as_forward<ExtraArgument>(extra_argument) ...);
			}
			return;
		}

		inline static auto exchange_effect (
			IByteStreamView &             data,
			typename Definition::Effect & value
		) -> Void {
			auto section_count = k_none_size;
			if constexpr (check_version(version, {}, {1, 3})) {
				section_count = 7_sz;
			}
			if constexpr (check_version(version, {}, {3})) {
				section_count = 8_sz;
			}
			auto section_information_data = IByteStreamView{data.forward_view(bs_static_size<List<IntegerU32>>(3_sz) * section_count)};
			auto string_chunk_offset = Size{};
			exchange_size_fixed<IntegerU32>(data, string_chunk_offset);
			data.set_position(string_chunk_offset);
			auto section_list_data = IByteStreamView{data.stream_view()};
			auto string_chunk_data = IByteStreamView{data.reserve_view()};
			auto string_chunk_size = Size{};
			if constexpr (check_version(version, {}, {1})) {
				exchange_section(section_information_data, section_list_data, value.block_1, 1_ix, bs_static_size<List<IntegerU32>>(6_sz));
			}
			if constexpr (check_version(version, {}, {1})) {
				exchange_section(section_information_data, section_list_data, value.block_5, 5_ix, bs_static_size<List<IntegerU32>>(7_sz));
			}
			if constexpr (check_version(version, {}, {1})) {
				exchange_section(section_information_data, section_list_data, value.block_6, 6_ix, bs_static_size<List<IntegerU32>>(5_sz));
			}
			if constexpr (check_version(version, {}, {1, 2})) {
				exchange_section(section_information_data, section_list_data, value.block_2, 2_ix, bs_static_size<List<IntegerU32>>(3_sz));
			}
			if constexpr (check_version(version, {}, {2})) {
				exchange_section(section_information_data, section_list_data, value.block_2, 2_ix, bs_static_size<List<IntegerU32>>(2_sz));
			}
			if constexpr (check_version(version, {}, {1})) {
				exchange_section(section_information_data, section_list_data, value.block_3, 3_ix, bs_static_size<List<IntegerU32>>(3_sz), string_chunk_data, string_chunk_size);
			}
			if constexpr (check_version(version, {}, {1})) {
				exchange_section(section_information_data, section_list_data, value.block_4, 4_ix, bs_static_size<List<IntegerU32>>(5_sz));
			}
			if constexpr (check_version(version, {}, {3})) {
				exchange_section(section_information_data, section_list_data, value.block_7, 7_ix, bs_static_size<List<IntegerU32>>(2_sz));
			}
			if constexpr (check_version(version, {}, {1, 3})) {
				exchange_section(section_information_data, section_list_data, value.block_8, 7_ix, bs_static_size<List<IntegerU32>>(3_sz));
			}
			if constexpr (check_version(version, {}, {3})) {
				exchange_section(section_information_data, section_list_data, value.block_8, 8_ix, bs_static_size<List<IntegerU32>>(5_sz));
			}
			data.forward(string_chunk_size);
			return;
		}

		// ----------------

		inline static auto process_whole (
			IByteStreamView &             data,
			typename Definition::Effect & definition
		) -> Void {
			data.read_constant(k_magic_identifier);
			data.read_constant(cbox<VersionNumber>(version.number));
			exchange_effect(data, definition);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &             data_,
			typename Definition::Effect & definition
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			return process_whole(data, definition);
		}

	};

}
