module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_group.pack;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_group.definition;
import twinning.kernel.tool.popcap.resource_stream_group.structure;
import twinning.kernel.tool.popcap.resource_stream_group.common;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;

export namespace Twinning::Kernel::Tool::Popcap::ResourceStreamGroup {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Pack :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::Definition;

		// ----------------

		inline static auto process_package(
			OutputBinaryStreamView &    data,
			Definition::Package const & definition,
			Path const &                resource_directory
		) -> Void {
			data.write_constant(Structure::k_magic_marker);
			data.write_constant(cast_box<Structure::VersionNumber>(t_version.number));
			struct {
				OutputBinaryStreamView header{};
				OutputBinaryStreamView resource_information{};
				Size                   resource_information_offset{};
			} information_data = {};
			{
				auto information_structure = Structure::Information<t_version>{};
				information_structure.resource_information.allocate_full(definition.resource.size());
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					auto & resource_information_structure = information_structure.resource_information.at(resource_index);
					assert_test(resource_definition.path.type() == Storage::PathType::Constant::detached());
					resource_information_structure.key = resource_definition.path.emit_windows();
					switch (resource_definition.additional.type().value) {
						case ResourceType::Constant::general().value: {
							resource_information_structure.value.type = Structure::ResourceTypeFlag<t_version>::general;
							resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::general()>();
							break;
						}
						case ResourceType::Constant::texture().value: {
							resource_information_structure.value.type = Structure::ResourceTypeFlag<t_version>::texture;
							resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::texture()>();
							break;
						}
						default: throw UnreachableException{};
					}
				}
				CompiledMapData::adjust_sequence(information_structure.resource_information);
				information_data.header = data.split_forward(binary_size_of(information_structure.header));
				information_data.resource_information_offset = data.position();
				information_data.resource_information = data.split_forward(CompiledMapData::compute_ripe_size(information_structure.resource_information));
				data.write_space(Math::compute_padding_size(data.position(), k_padding_unit_size), k_null_byte);
			}
			auto information_structure = Structure::Information<t_version>{};
			if constexpr (check_version(t_version, {1, 3})) {
				information_structure.header.unknown_1 = 1_iu32;
			}
			if constexpr (check_version(t_version, {3})) {
				information_structure.header.unknown_1 = 0_iu32;
			}
			information_structure.header.information_section_size = cast_box<IntegerU32>(data.position());
			information_structure.header.resource_data_section_compression = packet_compression_to_data(definition.compression);
			information_structure.resource_information.allocate_full(definition.resource.size());
			auto resource_data_section_container = Binary{};
			for (auto & current_resource_type : make_static_array<ResourceType>(ResourceType::Constant::general(), ResourceType::Constant::texture())) {
				auto resource_data_section_stream = OutputBinaryStreamView{};
				auto resource_data_section_offset = data.position();
				auto resource_data_section_size_original = 0_sz;
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					if (resource_definition.additional.type() != current_resource_type) {
						continue;
					}
					auto resource_path = resource_directory.push(resource_definition.path);
					auto resource_size = Storage::size_file(resource_path);
					resource_data_section_size_original += Math::compute_padded_size(resource_size, k_padding_unit_size);
				}
				auto compress_resource_data_section = k_false;
				switch (current_resource_type.value) {
					case ResourceType::Constant::general().value: {
						compress_resource_data_section = definition.compression.general;
						break;
					}
					case ResourceType::Constant::texture().value: {
						compress_resource_data_section = definition.compression.texture;
						break;
					}
					default: throw UnreachableException{};
				}
				if (!compress_resource_data_section) {
					resource_data_section_stream = data.split_next(resource_data_section_size_original);
				}
				else {
					resource_data_section_stream = like_binary_stream<StreamMode::Constant::output()>(data, use_binary_buffer(resource_data_section_container, resource_data_section_size_original));
				}
				for (auto & resource_index : SizeRange{definition.resource.size()}) {
					auto & resource_definition = definition.resource[resource_index];
					if (resource_definition.additional.type() != current_resource_type) {
						continue;
					}
					auto & resource_information_structure = information_structure.resource_information.at(resource_index);
					auto   resource_path = resource_directory.push(resource_definition.path);
					auto   resource_size = Storage::size_file(resource_path);
					resource_information_structure.key = resource_definition.path.emit_windows();
					resource_information_structure.value.offset = cast_box<IntegerU32>(resource_data_section_stream.position());
					resource_information_structure.value.size = cast_box<IntegerU32>(resource_size);
					Storage::read_file(resource_path, 0_sz, resource_data_section_stream.split_forward(resource_size).view());
					switch (resource_definition.additional.type().value) {
						case ResourceType::Constant::general().value: {
							auto & resource_additional_definition = resource_definition.additional.template get_of_type<ResourceType::Constant::general()>();
							resource_information_structure.value.type = Structure::ResourceTypeFlag<t_version>::general;
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::general()>();
							break;
						}
						case ResourceType::Constant::texture().value: {
							auto & resource_additional_definition = resource_definition.additional.template get_of_type<ResourceType::Constant::texture()>();
							resource_information_structure.value.type = Structure::ResourceTypeFlag<t_version>::texture;
							auto & resource_additional_information_structure = resource_information_structure.value.additional.template set_of_type<ResourceType::Constant::texture()>();
							resource_additional_information_structure.index = cast_box<IntegerU32>(resource_additional_definition.index);
							resource_additional_information_structure.size_width = cast_box<IntegerU32>(resource_additional_definition.size.width);
							resource_additional_information_structure.size_height = cast_box<IntegerU32>(resource_additional_definition.size.height);
							break;
						}
						default: throw UnreachableException{};
					}
					resource_data_section_stream.write_space(Math::compute_padding_size(resource_data_section_stream.position(), k_padding_unit_size), k_null_byte);
				}
				assert_test(resource_data_section_stream.full());
				if (!compress_resource_data_section || (current_resource_type == ResourceType::Constant::texture() && resource_data_section_size_original == 0_sz)) {
					data.forward(resource_data_section_stream.position());
				}
				else {
					Data::Compression::Deflate::Compress::process(as_left(unsafe_cast<InputBinaryStreamView>(resource_data_section_stream.split_all())), data, Data::Compression::Deflate::Container::Constant::zlib(), 9_i, 15_i, 9_i, Data::Compression::Deflate::StrategyMode::Constant::default_mode());
					data.write_space(Math::compute_padding_size(data.position(), k_padding_unit_size), k_null_byte);
				}
				auto resource_data_section_size = data.position() - resource_data_section_offset;
				switch (current_resource_type.value) {
					case ResourceType::Constant::general().value: {
						information_structure.header.general_resource_data_section_offset = cast_box<IntegerU32>(resource_data_section_offset);
						information_structure.header.general_resource_data_section_size = cast_box<IntegerU32>(resource_data_section_size);
						information_structure.header.general_resource_data_section_size_original = cast_box<IntegerU32>(resource_data_section_size_original);
						break;
					}
					case ResourceType::Constant::texture().value: {
						information_structure.header.texture_resource_data_section_offset = cast_box<IntegerU32>(resource_data_section_offset);
						information_structure.header.texture_resource_data_section_size = cast_box<IntegerU32>(resource_data_section_size);
						information_structure.header.texture_resource_data_section_size_original = cast_box<IntegerU32>(resource_data_section_size_original);
						break;
					}
					default: throw UnreachableException{};
				}
			}
			resource_data_section_container.reset();
			information_structure.header.resource_information_section_offset = cast_box<IntegerU32>(information_data.resource_information_offset);
			information_structure.header.resource_information_section_size = cast_box<IntegerU32>(information_data.resource_information.size());
			CompiledMapData::adjust_sequence(information_structure.resource_information);
			{
				information_data.header.write(information_structure.header);
				CompiledMapData::encode(information_structure.resource_information, information_data.resource_information);
			}
			return;
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView &    data_,
			Definition::Package const & definition,
			Path const &                resource_directory
		) -> Void {
			M_use_split_stream_of(data);
			return process_package(data, definition, resource_directory);
		}

	};

}
