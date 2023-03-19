#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/effect/version.hpp"
#include "core/tool/popcap/effect/manifest.hpp"

namespace TwinStar::Core::Tool::PopCap::Effect {

	template <auto version> requires (check_version(version, {}))
	struct EncodeCommon {

	protected:

		using Manifest = Manifest<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0x70636678_iu32};

		// ----------------

		using VersionNumber = IntegerU32;

	};

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		// ----------------

		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerU32>(value));
			return;
		}

		static auto exchange_unit_string (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			data.write(self_cast<StringBlockZeroed>(value));
			return;
		}

		// ----------------

		static auto process_block (
			OByteStreamView &                 block_data,
			typename Manifest::Block1 const & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			exchange_unit_integer(block_data, block_manifest.unknown_6);
			return;
		}

		static auto process_block (
			OByteStreamView &                 block_data,
			typename Manifest::Block2 const & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			return;
		}

		static auto process_block (
			OByteStreamView &                      block_data,
			typename Manifest::Block3 const &      block_manifest,
			VOptionalView<OByteStreamView> const & string_chunk_data
		) -> Void {
			auto string_offset = cbw<Integer>(k_begin_index);
			auto string_size = cbw<Integer>(block_manifest.string.size());
			if (string_chunk_data.has()) {
				string_offset = cbw<Integer>(string_chunk_data.get().position());
				exchange_unit_string(string_chunk_data.get(), block_manifest.string);
			}
			exchange_unit_integer(block_data, string_size);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, string_offset);
			return;
		}

		static auto process_block (
			OByteStreamView &                 block_data,
			typename Manifest::Block4 const & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			return;
		}

		static auto process_block (
			OByteStreamView &                 block_data,
			typename Manifest::Block5 const & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			exchange_unit_integer(block_data, block_manifest.unknown_6);
			exchange_unit_integer(block_data, block_manifest.unknown_7);
			return;
		}

		static auto process_block (
			OByteStreamView &                 block_data,
			typename Manifest::Block6 const & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			return;
		}

		static auto process_block (
			OByteStreamView &                 block_data,
			typename Manifest::Block7 const & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			return;
		}

		static auto process_block (
			OByteStreamView &                 block_data,
			typename Manifest::Block8 const & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			return;
		}

		// ----------------

		template <typename Block, typename ... ExtraArgument> requires
			CategoryConstraint<IsPureInstance<Block> && IsValid<ExtraArgument ...>>
		static auto process_section (
			OByteStreamView &    section_information_data,
			OByteStreamView &    section_data,
			List<Block> const &  section_manifest,
			Size const &         section_index,
			Size const &         block_size_constant,
			ExtraArgument && ... extra_argument
		) -> Void {
			section_information_data.set_position(bs_static_size<List<IntegerU32>>(3_sz) * section_index);
			exchange_unit_integer(section_information_data, cbw<Integer>(section_manifest.size()));
			exchange_unit_integer(section_information_data, cbw<Integer>(section_data.position()));
			exchange_unit_integer(section_information_data, cbw<Integer>(block_size_constant));
			for (auto & block_manifest : section_manifest) {
				process_block(section_data, block_manifest, as_forward<ExtraArgument>(extra_argument) ...);
			}
			return;
		}

		static auto process_effect (
			OByteStreamView &                 effect_data,
			typename Manifest::Effect const & effect_manifest
		) -> Void {
			effect_data.write_constant(k_magic_identifier);
			effect_data.write_constant(cbw<VersionNumber>(version.number));
			auto section_information_data = OByteStreamView{effect_data.forward_view(bs_static_size<List<IntegerU32>>(3_sz) * 8_sz)};
			auto string_chunk_offset_data = OByteStreamView{effect_data.forward_view(bs_static_size<IntegerU32>())};
			auto section_list_data = OByteStreamView{effect_data.view(), effect_data.position()};
			process_section(section_information_data, section_list_data, effect_manifest.block_1, 1_ix, bs_static_size<List<IntegerU32>>(6_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_5, 5_ix, bs_static_size<List<IntegerU32>>(7_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_6, 6_ix, bs_static_size<List<IntegerU32>>(5_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_2, 2_ix, bs_static_size<List<IntegerU32>>(2_sz));
			auto section_information_offset_3 = section_information_data.position();
			auto section_list_offset_3 = section_list_data.position();
			process_section(section_information_data, section_list_data, effect_manifest.block_3, 3_ix, bs_static_size<List<IntegerU32>>(3_sz), VOptionalView<OByteStreamView>{});
			process_section(section_information_data, section_list_data, effect_manifest.block_4, 4_ix, bs_static_size<List<IntegerU32>>(5_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_7, 7_ix, bs_static_size<List<IntegerU32>>(2_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_8, 8_ix, bs_static_size<List<IntegerU32>>(5_sz));
			effect_data.set_position(section_list_data.position());
			exchange_unit_integer(string_chunk_offset_data, cbw<Integer>(effect_data.position()));
			auto string_chunk_data = OByteStreamView{effect_data.reserve_view()};
			section_information_data.set_position(section_information_offset_3);
			section_list_data.set_position(section_list_offset_3);
			process_section(section_information_data, section_list_data, effect_manifest.block_3, 3_ix, bs_static_size<List<IntegerU32>>(3_sz), VOptionalView<OByteStreamView>{string_chunk_data});
			effect_data.forward(string_chunk_data.position());
			return;
		}

	public:

		static auto do_process_effect (
			OByteStreamView &                 effect_data_,
			typename Manifest::Effect const & effect_manifest
		) -> Void {
			M_use_zps_of(effect_data);
			return process_effect(effect_data, effect_manifest);
		}

	};

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		// ----------------

		static auto exchange_unit_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerU32>());
			return;
		}

		static auto exchange_unit_string (
			IByteStreamView & data,
			String &          value
		) -> Void {
			data.read(self_cast<StringBlockZeroed>(value));
			return;
		}

		// ----------------

		static auto process_block (
			IByteStreamView &           block_data,
			typename Manifest::Block1 & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			exchange_unit_integer(block_data, block_manifest.unknown_6);
			return;
		}

		static auto process_block (
			IByteStreamView &           block_data,
			typename Manifest::Block2 & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			return;
		}

		static auto process_block (
			IByteStreamView &           block_data,
			typename Manifest::Block3 & block_manifest,
			IByteStreamView &           string_chunk_data,
			Size &                      string_chunk_size
		) -> Void {
			auto string_offset = Integer{};
			auto string_size = Integer{};
			exchange_unit_integer(block_data, string_size);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, string_offset);
			string_chunk_data.set_position(cbw<Size>(string_offset));
			exchange_unit_string(string_chunk_data, block_manifest.string);
			assert_test(block_manifest.string.size() == cbw<Size>(string_size));
			string_chunk_size = maximum(string_chunk_size, string_chunk_data.position());
			return;
		}

		static auto process_block (
			IByteStreamView &           block_data,
			typename Manifest::Block4 & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			return;
		}

		static auto process_block (
			IByteStreamView &           block_data,
			typename Manifest::Block5 & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			exchange_unit_integer(block_data, block_manifest.unknown_6);
			exchange_unit_integer(block_data, block_manifest.unknown_7);
			return;
		}

		static auto process_block (
			IByteStreamView &           block_data,
			typename Manifest::Block6 & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			return;
		}

		static auto process_block (
			IByteStreamView &           block_data,
			typename Manifest::Block7 & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			return;
		}

		static auto process_block (
			IByteStreamView &           block_data,
			typename Manifest::Block8 & block_manifest
		) -> Void {
			exchange_unit_integer(block_data, block_manifest.unknown_1);
			exchange_unit_integer(block_data, block_manifest.unknown_2);
			exchange_unit_integer(block_data, block_manifest.unknown_3);
			exchange_unit_integer(block_data, block_manifest.unknown_4);
			exchange_unit_integer(block_data, block_manifest.unknown_5);
			return;
		}

		// ----------------

		template <typename Block, typename ... ExtraArgument> requires
			CategoryConstraint<IsPureInstance<Block> && IsValid<ExtraArgument ...>>
		static auto process_section (
			IByteStreamView &    section_information_data,
			IByteStreamView &    section_data,
			List<Block> &        section_manifest,
			Size const &         section_index,
			Size const &         block_size_constant,
			ExtraArgument && ... extra_argument
		) -> Void {
			auto block_count = Integer{};
			auto offset = Integer{};
			auto block_size = Integer{};
			section_information_data.set_position(bs_static_size<List<IntegerU32>>(3_sz) * section_index);
			exchange_unit_integer(section_information_data, block_count);
			exchange_unit_integer(section_information_data, offset);
			exchange_unit_integer(section_information_data, block_size);
			assert_test(cbw<Size>(block_size) == block_size_constant);
			section_data.set_position(cbw<Size>(offset));
			section_manifest.allocate_full(cbw<Size>(block_count));
			for (auto & block_manifest : section_manifest) {
				process_block(section_data, block_manifest, as_forward<ExtraArgument>(extra_argument) ...);
			}
			return;
		}

		static auto process_effect (
			IByteStreamView &           effect_data,
			typename Manifest::Effect & effect_manifest
		) -> Void {
			effect_data.read_constant(k_magic_identifier);
			effect_data.read_constant(cbw<VersionNumber>(version.number));
			auto section_information_data = IByteStreamView{effect_data.forward_view(bs_static_size<List<IntegerU32>>(3_sz) * 8_sz)};
			auto string_chunk_offset = Integer{};
			exchange_unit_integer(effect_data, string_chunk_offset);
			effect_data.set_position(cbw<Size>(string_chunk_offset));
			auto section_list_data = IByteStreamView{effect_data.stream_view()};
			auto string_chunk_data = IByteStreamView{effect_data.reserve_view()};
			auto string_chunk_size = Size{};
			process_section(section_information_data, section_list_data, effect_manifest.block_1, 1_ix, bs_static_size<List<IntegerU32>>(6_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_5, 5_ix, bs_static_size<List<IntegerU32>>(7_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_6, 6_ix, bs_static_size<List<IntegerU32>>(5_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_2, 2_ix, bs_static_size<List<IntegerU32>>(2_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_3, 3_ix, bs_static_size<List<IntegerU32>>(3_sz), string_chunk_data, string_chunk_size);
			process_section(section_information_data, section_list_data, effect_manifest.block_4, 4_ix, bs_static_size<List<IntegerU32>>(5_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_7, 7_ix, bs_static_size<List<IntegerU32>>(2_sz));
			process_section(section_information_data, section_list_data, effect_manifest.block_8, 8_ix, bs_static_size<List<IntegerU32>>(5_sz));
			effect_data.forward(string_chunk_size);
			return;
		}

	public:

		static auto do_process_effect (
			IByteStreamView &           effect_data_,
			typename Manifest::Effect & effect_manifest
		) -> Void {
			M_use_zps_of(effect_data);
			restruct(effect_manifest);
			return process_effect(effect_data, effect_manifest);
		}

	};

}
