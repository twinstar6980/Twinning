module;

#include "kernel/common.hpp"

// TODO : refactor

export module twinning.kernel.tool.popcap.resource_stream_bundle.shared;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundle::Shared {

	#pragma region padding unit

	inline constexpr auto k_padding_unit_size = Size{0x1000_sz};

	#pragma endregion

	#pragma region resource type

	M_enumeration(
		M_wrap(ResourceType),
		M_wrap(
			general,
			texture,
		),
	);

	#pragma endregion

	#pragma region packet compression

	M_record_of_map(
		M_wrap(PacketCompression),
		M_wrap(
			(Boolean) general,
			(Boolean) texture,
		),
	);

	// ----------------

	inline constexpr auto k_packet_compression_flag_count = Size{2_sz};

	namespace PacketCompressionFlag {
		inline constexpr auto texture = Size{1_ix};
		inline constexpr auto general = Size{2_ix};
	}

	inline auto packet_compression_to_data (
		PacketCompression const & value
	) -> IntegerU32 {
		auto data_bit = BitSet<k_packet_compression_flag_count>{};
		data_bit.set(PacketCompressionFlag::general, value.general);
		data_bit.set(PacketCompressionFlag::texture, value.texture);
		return cbox<IntegerU32>(data_bit.to_integer());
	}

	inline auto packet_compression_from_data (
		IntegerU32 const & data
	) -> PacketCompression {
		auto value = PacketCompression{};
		auto data_bit = BitSet<k_packet_compression_flag_count>{};
		data_bit.from_integer(cbox<IntegerU8>(data));
		value.general = data_bit.get(PacketCompressionFlag::general);
		value.texture = data_bit.get(PacketCompressionFlag::texture);
		return value;
	}

	#pragma endregion

	#pragma region string block fixed 128

	using StringBlockFixed128 = StaticArray<Character, 128_sz>;

	// ----------------

	inline auto string_block_fixed_128_from_string (
		CStringView const & block
	) -> StringBlockFixed128 {
		assert_test(block.size() < 128_sz);
		return StringBlockFixed128{block};
	}

	#pragma endregion

	#pragma region compiled map data

	namespace CompiledMapData {

		inline constexpr auto k_block_size = Size{bs_static_size<IntegerU32>()};

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
		inline auto adjust_sequence (
			Map<String, Value> & map
		) -> Void {
			Range::each(
				map,
				[] (auto & element) -> auto {
					element.key.as_upper_case();
					return;
				}
			);
			Range::sort(
				map,
				[] (auto & thix, auto & that) -> auto {
					return thix.key > that.key;
				}
			);
			return;
		}

		// ----------------

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
		inline auto compute_ripe_size (
			Map<String, Value> const & map
		) -> Size {
			struct WorkOption {
				Size inherit_length{};
			};
			auto work_option = Array<Optional<WorkOption>>{map.size()};
			if (!map.empty()) {
				work_option[1_ix].set(
					WorkOption{
						.inherit_length = k_none_size,
					}
				);
			}
			auto block_count = k_none_size;
			for (auto & index : SizeRange{map.size()}) {
				auto & element = map.at(index);
				{
					auto a_string_has_child = Array<Boolean>{element.key.size() + 1_sz};
					for (auto & index_1 : SizeRange{index + 2_ix, map.size()}) {
						if (!work_option[index_1].has()) {
							auto common_size = Range::common_size(map.at(index).key, map.at(index_1).key);
							if (!a_string_has_child[common_size] && common_size >= work_option[index].get().inherit_length) {
								a_string_has_child[common_size] = k_true;
								work_option[index_1].set(
									WorkOption{
										.inherit_length = common_size,
									}
								);
							}
						}
					}
				}
				auto character_index = k_begin_index;
				if (work_option[index].has()) {
					character_index += work_option[index].get().inherit_length;
					work_option[index].reset();
				}
				block_count += element.key.size() - character_index + 1_sz;
				auto additional_size = bs_size(element.value);
				assert_test(is_padded_size(additional_size, bs_static_size<IntegerU32>()));
				block_count += additional_size / bs_static_size<IntegerU32>();
			}
			return block_count * k_block_size;
		}

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
		inline auto encode (
			Map<String, Value> const & map,
			OByteStreamView &          data
		) -> Void {
			auto stream = IOByteStreamView{data.reserve_view()};
			struct WorkOption {
				Size inherit_length{};
				Size parent_offset{};
			};
			auto work_option = Array<Optional<WorkOption>>{map.size()};
			if (!map.empty()) {
				work_option.first().set(
					WorkOption{
						.inherit_length = k_none_size,
						.parent_offset = k_begin_index,
					}
				);
				for (auto & index : SizeRange{map.size()}) {
					auto & element = map.at(index);
					auto   current_string_has_child = Array<Boolean>{element.key.size() + 1_sz};
					for (auto & index_1 : SizeRange{index + k_next_index, map.size()}) {
						if (!work_option[index_1].has()) {
							auto common_size = Range::common_size(element.key, map.at(index_1).key);
							if (!current_string_has_child[common_size] && common_size >= work_option[index].get().inherit_length) {
								current_string_has_child[common_size] = k_true;
								work_option[index_1].set(
									WorkOption{
										.inherit_length = common_size,
										.parent_offset = stream.position() / k_block_size + (common_size - work_option[index].get().inherit_length),
									}
								);
							}
						}
					}
					auto character_index = k_begin_index;
					if (work_option[index].has()) {
						character_index += work_option[index].get().inherit_length;
						auto current_position = stream.position();
						stream.set_position(work_option[index].get().parent_offset * k_block_size);
						auto composite_value = stream.read_of<IntegerU32>();
						composite_value |= cbox<IntegerU32>(current_position / k_block_size) << k_type_bit_count<IntegerU8>;
						stream.backward(bs_static_size<IntegerU32>());
						stream.write(composite_value);
						stream.set_position(current_position);
						work_option[index].reset();
					}
					while (character_index < element.key.size()) {
						stream.write(cbox<IntegerU32>(element.key[character_index]));
						++character_index;
					}
					stream.write(cbox<IntegerU32>(CharacterType::k_null));
					stream.write(element.value);
				}
			}
			data.forward(stream.position());
			return;
		}

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
		inline auto decode (
			Map<String, Value> & map,
			IByteStreamView &    data
		) -> Void {
			auto stream = IByteStreamView{data.reserve_view()};
			map.allocate(stream.reserve() / k_block_size);
			auto parent_string_list = Array<Optional<CStringView>>{stream.reserve() / k_block_size};
			while (!stream.full()) {
				map.append();
				auto & element = map.last();
				auto   string_length_in_next_stream = k_none_size;
				{
					auto string_begin_position = stream.position();
					while (cbox<Character>(clip_bit(stream.read_of<IntegerU32>(), 1_ix * k_type_bit_count<IntegerU8>, 1_sz * k_type_bit_count<IntegerU8>)) != CharacterType::k_null) {
						++string_length_in_next_stream;
					}
					stream.set_position(string_begin_position);
				}
				if (auto & parent_string = parent_string_list[stream.position() / k_block_size]; parent_string.has()) {
					element.key.allocate(parent_string.get().size() + string_length_in_next_stream);
					element.key.append_list(parent_string.get());
					parent_string.reset();
				}
				else {
					element.key.allocate(string_length_in_next_stream);
				}
				while (k_true) {
					auto composite_value = stream.read_of<IntegerU32>();
					auto child_string_offset = clip_bit(composite_value, 2_ix * k_type_bit_count<IntegerU8>, 3_sz * k_type_bit_count<IntegerU8>);
					auto current_character = cbox<Character>(clip_bit(composite_value, 1_ix * k_type_bit_count<IntegerU8>, 1_sz * k_type_bit_count<IntegerU8>));
					if (child_string_offset != 0x000000_iu32) {
						parent_string_list[cbox<Size>(child_string_offset)].set(element.key);
					}
					if (current_character == CharacterType::k_null) {
						break;
					}
					element.key.append(current_character);
				}
				stream.read(element.value);
			}
			map.shrink_to_fit();
			data.forward(stream.position());
			return;
		}

	}

	#pragma endregion

}

// NOTE : USING

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamGroup {
	using namespace ResourceStreamBundle::Shared;
}
export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundle {
	using namespace ResourceStreamBundle::Shared;
}
export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundlePatch {
	using namespace ResourceStreamBundle::Shared;
}
