module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.render_effect.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.render_effect.version;
import twinning.kernel.tool.popcap.render_effect.definition;
import twinning.kernel.tool.popcap.render_effect.common;
import twinning.kernel.tool.common.byte_stream;

export namespace Twinning::Kernel::Tool::PopCap::RenderEffect {

	template <auto version> requires (check_version(version, {}, {}))
	struct Encode :
		Common<version>,
		CommonByteStreamExchangeForOutput {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		// ----------------

		inline static auto exchange_block (
			OByteStreamView &                   data,
			typename Definition::Block1 const & value
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
			OByteStreamView &                   data,
			typename Definition::Block2 const & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			if constexpr (check_version(version, {}, {1, 2})) {
				exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			}
			return;
		}

		inline static auto exchange_block (
			OByteStreamView &                      data,
			typename Definition::Block3 const &    value,
			VOptionalView<OByteStreamView> const & string_chunk_data
		) -> Void {
			auto string_offset = k_begin_index;
			auto string_size = value.string.size();
			if (string_chunk_data.has()) {
				string_offset = string_chunk_data.get().position();
				exchange_string_block<Void>(string_chunk_data.get(), value.string);
			}
			exchange_size_fixed<IntegerU32>(data, string_size);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			exchange_size_fixed<IntegerU32>(data, string_offset);
			return;
		}

		inline static auto exchange_block (
			OByteStreamView &                   data,
			typename Definition::Block4 const & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_4);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_5);
			return;
		}

		inline static auto exchange_block (
			OByteStreamView &                   data,
			typename Definition::Block5 const & value
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
			OByteStreamView &                   data,
			typename Definition::Block6 const & value
		) -> Void {
			exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_3);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_4);
			exchange_integer_fixed<IntegerU32>(data, value.unknown_5);
			return;
		}

		inline static auto exchange_block (
			OByteStreamView &                   data,
			typename Definition::Block7 const & value
		) -> Void {
			if constexpr (check_version(version, {}, {3})) {
				exchange_integer_fixed<IntegerU32>(data, value.unknown_1);
				exchange_integer_fixed<IntegerU32>(data, value.unknown_2);
			}
			return;
		}

		inline static auto exchange_block (
			OByteStreamView &                   data,
			typename Definition::Block8 const & value
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
			OByteStreamView &    section_information_data,
			OByteStreamView &    section_data,
			List<Block> const &  section_value,
			Size const &         section_index,
			Size const &         block_size_constant,
			ExtraArgument && ... extra_argument
		) -> Void {
			section_information_data.set_position(bs_static_size<List<IntegerU32>>(3_sz) * section_index);
			exchange_size_fixed<IntegerU32>(section_information_data, section_value.size());
			exchange_size_fixed<IntegerU32>(section_information_data, section_data.position());
			exchange_size_fixed<IntegerU32>(section_information_data, block_size_constant);
			for (auto & value : section_value) {
				exchange_block(section_data, value, as_forward<ExtraArgument>(extra_argument) ...);
			}
			return;
		}

		inline static auto exchange_effect (
			OByteStreamView &                   data,
			typename Definition::Effect const & value
		) -> Void {
			auto section_count = k_none_size;
			if constexpr (check_version(version, {}, {1, 3})) {
				section_count = 7_sz;
			}
			if constexpr (check_version(version, {}, {3})) {
				section_count = 8_sz;
			}
			auto section_information_data = OByteStreamView{data.forward_view(bs_static_size<List<IntegerU32>>(3_sz) * section_count)};
			auto string_chunk_offset_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU32>())};
			auto section_list_data = OByteStreamView{data.view(), data.position()};
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
			auto section_information_offset_3 = section_information_data.position();
			auto section_list_offset_3 = section_list_data.position();
			if constexpr (check_version(version, {}, {1})) {
				exchange_section(section_information_data, section_list_data, value.block_3, 3_ix, bs_static_size<List<IntegerU32>>(3_sz), VOptionalView<OByteStreamView>{});
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
			data.set_position(section_list_data.position());
			exchange_size_fixed<IntegerU32>(string_chunk_offset_data, data.position());
			auto string_chunk_data = OByteStreamView{data.reserve_view()};
			if constexpr (check_version(version, {}, {1})) {
				section_information_data.set_position(section_information_offset_3);
				section_list_data.set_position(section_list_offset_3);
				exchange_section(section_information_data, section_list_data, value.block_3, 3_ix, bs_static_size<List<IntegerU32>>(3_sz), VOptionalView<OByteStreamView>{string_chunk_data});
			}
			data.forward(string_chunk_data.position());
			return;
		}

		// ----------------

		inline static auto process_whole (
			OByteStreamView &                   data,
			typename Definition::Effect const & definition
		) -> Void {
			data.write_constant(k_magic_identifier);
			data.write_constant(cbox<VersionNumber>(version.number));
			exchange_effect(data, definition);
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &                   data_,
			typename Definition::Effect const & definition
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, definition);
		}

	};

}
