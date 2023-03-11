#pragma once

#include "implement/common.hpp"
#include "implement/language.hpp"
#include "implement/method_invoke_command.hpp"

#define M_define_exposed_method_invoke_command(_uuid, _base, _name, _id)\
	class __declspec(uuid(_uuid)) _name##_base :\
		public _base {\
	public:\
		_name##_base () :\
			_base{MethodInvokeCommandConfigTable::_id} {\
		}\
	};

#define M_define_exposed_visible_method_invoke_command(_uuid, _base, _name, _id)\
	class __declspec(uuid(_uuid)) _name##_base :\
		public VisibleCommand<_base> {\
	public:\
		_name##_base () :\
			VisibleCommand{L###_id, MethodInvokeCommandConfigTable::_id} {\
		}\
	};

namespace TwinStar::WindowsExplorerExtension {

	#pragma region config

	namespace MethodInvokeCommandConfigTable {

		inline auto const main = MethodInvokeCommandConfig{
			.id = L"main",
			.type = std::nullopt,
			.rule = std::nullopt,
			.method = std::nullopt,
			.argument = LR"({})",
		};

		// ----------------

		inline auto const launch = MethodInvokeCommandConfig{
			.id = L"launch",
			.type = std::nullopt,
			.rule = std::nullopt,
			.method = std::nullopt,
			.argument = LR"({})",
		};

		inline auto const js = GroupMethodInvokeCommandConfig{
			.id = L"js",
			.child = {
				{
					.id = L"js.execute",
					.type = false,
					.rule = std::wregex{LR"(.+(\.js)$)", std::wregex::icase},
					.method = L"js.execute",
					.argument = LR"({})",
				},
			},
			.separator = {
				1,
			},
		};

		inline auto const json = GroupMethodInvokeCommandConfig{
			.id = L"json",
			.child = {
				{
					.id = L"json.format",
					.type = false,
					.rule = std::wregex{LR"(.+(\.json)$)", std::wregex::icase},
					.method = L"json.format",
					.argument = LR"({})",
				},
				{
					.id = L"json.format:default",
					.type = false,
					.rule = std::wregex{LR"(.+(\.json)$)", std::wregex::icase},
					.method = L"json.format",
					.argument = LR"({ "disable_trailing_comma": "?default", "disable_array_wrap_line": "?default" })",
				},
				{
					.id = L"json.format.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"json.format.batch",
					.argument = LR"({})",
				},
				{
					.id = L"json.format.batch:default",
					.type = true,
					.rule = std::nullopt,
					.method = L"json.format.batch",
					.argument = LR"({ "disable_trailing_comma": "?default", "disable_array_wrap_line": "?default" })",
				},
			},
			.separator = {
				2,
				2,
			},
		};

		inline auto const data = GroupMethodInvokeCommandConfig{
			.id = L"data",
			.child = {
				{
					.id = L"data.hash.md5",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.hash.md5",
					.argument = LR"({})",
				},
				{
					.id = L"data.encoding.base64.encode",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.encoding.base64.encode",
					.argument = LR"({})",
				},
				{
					.id = L"data.encoding.base64.decode",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.encoding.base64.decode",
					.argument = LR"({})",
				},
				{
					.id = L"data.encryption.xor.encrypt",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.encryption.xor.encrypt",
					.argument = LR"({})",
				},
				{
					.id = L"data.encryption.rijndael.encrypt",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.encryption.rijndael.encrypt",
					.argument = LR"({})",
				},
				{
					.id = L"data.encryption.rijndael.decrypt",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.encryption.rijndael.decrypt",
					.argument = LR"({})",
				},
				{
					.id = L"data.compression.deflate.compress",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.compression.deflate.compress",
					.argument = LR"({})",
				},
				{
					.id = L"data.compression.deflate.uncompress",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.compression.deflate.uncompress",
					.argument = LR"({})",
				},
				{
					.id = L"data.compression.zlib.compress",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.compression.zlib.compress",
					.argument = LR"({})",
				},
				{
					.id = L"data.compression.zlib.uncompress",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.compression.zlib.uncompress",
					.argument = LR"({})",
				},
				{
					.id = L"data.compression.gzip.compress",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.compression.gzip.compress",
					.argument = LR"({})",
				},
				{
					.id = L"data.compression.gzip.uncompress",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.compression.gzip.uncompress",
					.argument = LR"({})",
				},
				{
					.id = L"data.compression.bzip2.compress",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.compression.bzip2.compress",
					.argument = LR"({})",
				},
				{
					.id = L"data.compression.bzip2.uncompress",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.compression.bzip2.uncompress",
					.argument = LR"({})",
				},
				{
					.id = L"data.differentiation.vcdiff.encode",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.differentiation.vcdiff.encode",
					.argument = LR"({})",
				},
				{
					.id = L"data.differentiation.vcdiff.decode",
					.type = false,
					.rule = std::nullopt,
					.method = L"data.differentiation.vcdiff.decode",
					.argument = LR"({})",
				},
			},
			.separator = {
				1,
				2,
				1,
				2,
				2,
				2,
				2,
				2,
				2,
			},
		};

		inline auto const image = GroupMethodInvokeCommandConfig{
			.id = L"image",
			.child = {
				{
					.id = L"image.transformation.flip",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"image.transformation.flip",
					.argument = LR"({})",
				},
				{
					.id = L"image.transformation.scale",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"image.transformation.scale",
					.argument = LR"({})",
				},
				{
					.id = L"image.transformation.scale_rate",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"image.transformation.scale_rate",
					.argument = LR"({})",
				},
				{
					.id = L"image.atlas.pack",
					.type = false,
					.rule = std::wregex{LR"(.+(\.atlas)(\.json)$)", std::wregex::icase},
					.method = L"image.atlas.pack",
					.argument = LR"({})",
				},
				{
					.id = L"image.atlas.unpack",
					.type = false,
					.rule = std::wregex{LR"(.+(\.atlas)(\.json)$)", std::wregex::icase},
					.method = L"image.atlas.unpack",
					.argument = LR"({})",
				},
				{
					.id = L"image.atlas.pack_automatic",
					.type = true,
					.rule = std::wregex{LR"(.+(\.sprite)$)", std::wregex::icase},
					.method = L"image.atlas.pack_automatic",
					.argument = LR"({})",
				},
			},
			.separator = {
				1,
				2,
				2,
				1,
			},
		};

		inline auto const wwise_encoded_media = GroupMethodInvokeCommandConfig{
			.id = L"wwise.encoded_media",
			.child = {
				{
					.id = L"wwise.encoded_media.decode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.wem)$)", std::wregex::icase},
					.method = L"wwise.encoded_media.decode",
					.argument = LR"({})",
				},
				{
					.id = L"wwise.encoded_media.decode.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"wwise.encoded_media.decode.batch",
					.argument = LR"({})",
				},
			},
			.separator = {
				1,
				1,
			},
		};

		inline auto const wwise_sound_bank = GroupMethodInvokeCommandConfig{
			.id = L"wwise.sound_bank",
			.child = {
				{
					.id = L"wwise.sound_bank.encode",
					.type = true,
					.rule = std::wregex{LR"(.+(\.bnk)(\.bundle)$)", std::wregex::icase},
					.method = L"wwise.sound_bank.encode",
					.argument = LR"({})",
				},
				{
					.id = L"wwise.sound_bank.decode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.bnk)$)", std::wregex::icase},
					.method = L"wwise.sound_bank.decode",
					.argument = LR"({})",
				},
				{
					.id = L"wwise.sound_bank.encode.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"wwise.sound_bank.encode.batch",
					.argument = LR"({})",
				},
				{
					.id = L"wwise.sound_bank.decode.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"wwise.sound_bank.decode.batch",
					.argument = LR"({})",
				},
			},
			.separator = {
				2,
				2,
			},
		};

		inline auto const marmalade_dzip = GroupMethodInvokeCommandConfig{
			.id = L"marmalade.dzip",
			.child = {
				{
					.id = L"marmalade.dzip.pack",
					.type = true,
					.rule = std::wregex{LR"(.+(\.dz)(\.bundle)$)", std::wregex::icase},
					.method = L"marmalade.dzip.pack",
					.argument = LR"({})",
				},
				{
					.id = L"marmalade.dzip.unpack",
					.type = false,
					.rule = std::wregex{LR"(.+(\.dz)$)", std::wregex::icase},
					.method = L"marmalade.dzip.unpack",
					.argument = LR"({})",
				},
				{
					.id = L"marmalade.dzip.pack_automatic",
					.type = true,
					.rule = std::nullopt,
					.method = L"marmalade.dzip.pack_automatic",
					.argument = LR"({})",
				},
			},
			.separator = {
				2,
				1,
			},
		};

		inline auto const popcap_zlib = GroupMethodInvokeCommandConfig{
			.id = L"popcap.zlib",
			.child = {
				{
					.id = L"popcap.zlib.compress:32",
					.type = false,
					.rule = std::nullopt,
					.method = L"popcap.zlib.compress",
					.argument = LR"({ "version_variant_64": false })",
				},
				{
					.id = L"popcap.zlib.uncompress:32",
					.type = false,
					.rule = std::nullopt,
					.method = L"popcap.zlib.uncompress",
					.argument = LR"({ "version_variant_64": false })",
				},
				{
					.id = L"popcap.zlib.compress:64",
					.type = false,
					.rule = std::nullopt,
					.method = L"popcap.zlib.compress",
					.argument = LR"({ "version_variant_64": true })",
				},
				{
					.id = L"popcap.zlib.uncompress:64",
					.type = false,
					.rule = std::nullopt,
					.method = L"popcap.zlib.uncompress",
					.argument = LR"({ "version_variant_64": true })",
				},
				{
					.id = L"popcap.zlib.compress.batch:32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.zlib.compress.batch",
					.argument = LR"({ "version_variant_64": false })",
				},
				{
					.id = L"popcap.zlib.uncompress.batch:32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.zlib.uncompress.batch",
					.argument = LR"({ "version_variant_64": false })",
				},
				{
					.id = L"popcap.zlib.compress.batch:64",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.zlib.compress.batch",
					.argument = LR"({ "version_variant_64": true })",
				},
				{
					.id = L"popcap.zlib.uncompress.batch:64",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.zlib.uncompress.batch",
					.argument = LR"({ "version_variant_64": true })",
				},
			},
			.separator = {
				2,
				2,
				2,
				2,
			},
		};

		inline auto const popcap_reanim = GroupMethodInvokeCommandConfig{
			.id = L"popcap.reanim",
			.child = {
				{
					.id = L"popcap.reanim.encode:desktop_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.reanim)(\.json)$)", std::wregex::icase},
					.method = L"popcap.reanim.encode",
					.argument = LR"({ "version_platform": "desktop", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.decode:desktop_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.reanim)(\.compiled)$)", std::wregex::icase},
					.method = L"popcap.reanim.decode",
					.argument = LR"({ "version_platform": "desktop", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.encode:mobile_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.reanim)(\.json)$)", std::wregex::icase},
					.method = L"popcap.reanim.encode",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.decode:mobile_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.reanim)(\.compiled)$)", std::wregex::icase},
					.method = L"popcap.reanim.decode",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.encode:mobile_64",
					.type = false,
					.rule = std::wregex{LR"(.+(\.reanim)(\.json)$)", std::wregex::icase},
					.method = L"popcap.reanim.encode",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": true })",
				},
				{
					.id = L"popcap.reanim.decode:mobile_64",
					.type = false,
					.rule = std::wregex{LR"(.+(\.reanim)(\.compiled)$)", std::wregex::icase},
					.method = L"popcap.reanim.decode",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": true })",
				},
				{
					.id = L"popcap.reanim.encode:television_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.reanim)(\.json)$)", std::wregex::icase},
					.method = L"popcap.reanim.encode",
					.argument = LR"({ "version_platform": "television", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.decode:television_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.reanim)(\.compiled)$)", std::wregex::icase},
					.method = L"popcap.reanim.decode",
					.argument = LR"({ "version_platform": "television", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.encode.batch:desktop_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.reanim.encode.batch",
					.argument = LR"({ "version_platform": "desktop", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.decode.batch:desktop_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.reanim.decode.batch",
					.argument = LR"({ "version_platform": "desktop", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.encode.batch:mobile_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.reanim.encode.batch",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.decode.batch:mobile_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.reanim.decode.batch",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.encode.batch:mobile_64",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.reanim.encode.batch",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": true })",
				},
				{
					.id = L"popcap.reanim.decode.batch:mobile_64",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.reanim.decode.batch",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": true })",
				},
				{
					.id = L"popcap.reanim.encode.batch:television_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.reanim.encode.batch",
					.argument = LR"({ "version_platform": "television", "version_variant_64": false })",
				},
				{
					.id = L"popcap.reanim.decode.batch:television_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.reanim.decode.batch",
					.argument = LR"({ "version_platform": "television", "version_variant_64": false })",
				},
			},
			.separator = {
				2,
				2,
				2,
				2,
				2,
				2,
				2,
				2,
			},
		};

		inline auto const popcap_particle = GroupMethodInvokeCommandConfig{
			.id = L"popcap.particle",
			.child = {
				{
					.id = L"popcap.particle.encode:desktop_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.particle)(\.json)$)", std::wregex::icase},
					.method = L"popcap.particle.encode",
					.argument = LR"({ "version_platform": "desktop", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.decode:desktop_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.xml)(\.compiled)$)", std::wregex::icase},
					.method = L"popcap.particle.decode",
					.argument = LR"({ "version_platform": "desktop", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.encode:mobile_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.particle)(\.json)$)", std::wregex::icase},
					.method = L"popcap.particle.encode",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.decode:mobile_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.xml)(\.compiled)$)", std::wregex::icase},
					.method = L"popcap.particle.decode",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.encode:mobile_64",
					.type = false,
					.rule = std::wregex{LR"(.+(\.particle)(\.json)$)", std::wregex::icase},
					.method = L"popcap.particle.encode",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": true })",
				},
				{
					.id = L"popcap.particle.decode:mobile_64",
					.type = false,
					.rule = std::wregex{LR"(.+(\.xml)(\.compiled)$)", std::wregex::icase},
					.method = L"popcap.particle.decode",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": true })",
				},
				{
					.id = L"popcap.particle.encode:television_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.particle)(\.json)$)", std::wregex::icase},
					.method = L"popcap.particle.encode",
					.argument = LR"({ "version_platform": "television", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.decode:television_32",
					.type = false,
					.rule = std::wregex{LR"(.+(\.xml)(\.compiled)$)", std::wregex::icase},
					.method = L"popcap.particle.decode",
					.argument = LR"({ "version_platform": "television", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.encode.batch:desktop_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.particle.encode.batch",
					.argument = LR"({ "version_platform": "desktop", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.decode.batch:desktop_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.particle.decode.batch",
					.argument = LR"({ "version_platform": "desktop", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.encode.batch:mobile_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.particle.encode.batch",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.decode.batch:mobile_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.particle.decode.batch",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.encode.batch:mobile_64",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.particle.encode.batch",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": true })",
				},
				{
					.id = L"popcap.particle.decode.batch:mobile_64",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.particle.decode.batch",
					.argument = LR"({ "version_platform": "mobile", "version_variant_64": true })",
				},
				{
					.id = L"popcap.particle.encode.batch:television_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.particle.encode.batch",
					.argument = LR"({ "version_platform": "television", "version_variant_64": false })",
				},
				{
					.id = L"popcap.particle.decode.batch:television_32",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.particle.decode.batch",
					.argument = LR"({ "version_platform": "television", "version_variant_64": false })",
				},
			},
			.separator = {
				2,
				2,
				2,
				2,
				2,
				2,
				2,
				2,
			},
		};

		inline auto const popcap_rton = GroupMethodInvokeCommandConfig{
			.id = L"popcap.rton",
			.child = {
				{
					.id = L"popcap.rton.encode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.json)$)", std::wregex::icase},
					.method = L"popcap.rton.encode",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.decode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rton)$)", std::wregex::icase},
					.method = L"popcap.rton.decode",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.encrypt",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rton)$)", std::wregex::icase},
					.method = L"popcap.rton.encrypt",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.decrypt",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rton)$)", std::wregex::icase},
					.method = L"popcap.rton.decrypt",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.encode_then_encrypt",
					.type = false,
					.rule = std::wregex{LR"(.+(\.json)$)", std::wregex::icase},
					.method = L"popcap.rton.encode_then_encrypt",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.decrypt_then_decode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rton)$)", std::wregex::icase},
					.method = L"popcap.rton.decrypt_then_decode",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.encode.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.rton.encode.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.decode.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.rton.decode.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.encrypt.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.rton.encrypt.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.decrypt.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.rton.decrypt.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.encode_then_encrypt.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.rton.encode_then_encrypt.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rton.decrypt_then_decode.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.rton.decrypt_then_decode.batch",
					.argument = LR"({})",
				},
			},
			.separator = {
				2,
				2,
				2,
				2,
				2,
				2,
			},
		};

		inline auto const popcap_ptx = GroupMethodInvokeCommandConfig{
			.id = L"popcap.ptx",
			.child = {
				{
					.id = L"popcap.ptx.encode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"popcap.ptx.encode",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.ptx.decode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.ptx)$)", std::wregex::icase},
					.method = L"popcap.ptx.decode",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.ptx.encode:abgr_8888",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"popcap.ptx.encode",
					.argument = LR"({ "format": "abgr_8888" })",
				},
				{
					.id = L"popcap.ptx.decode:abgr_8888",
					.type = false,
					.rule = std::wregex{LR"(.+(\.ptx)$)", std::wregex::icase},
					.method = L"popcap.ptx.decode",
					.argument = LR"({ "format": "abgr_8888" })",
				},
				{
					.id = L"popcap.ptx.encode:argb_8888",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"popcap.ptx.encode",
					.argument = LR"({ "format": "argb_8888" })",
				},
				{
					.id = L"popcap.ptx.decode:argb_8888",
					.type = false,
					.rule = std::wregex{LR"(.+(\.ptx)$)", std::wregex::icase},
					.method = L"popcap.ptx.decode",
					.argument = LR"({ "format": "argb_8888" })",
				},
				{
					.id = L"popcap.ptx.encode:rgba_pvrtc4",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"popcap.ptx.encode",
					.argument = LR"({ "format": "rgba_pvrtc4" })",
				},
				{
					.id = L"popcap.ptx.decode:rgba_pvrtc4",
					.type = false,
					.rule = std::wregex{LR"(.+(\.ptx)$)", std::wregex::icase},
					.method = L"popcap.ptx.decode",
					.argument = LR"({ "format": "rgba_pvrtc4" })",
				},
				{
					.id = L"popcap.ptx.encode:rgb_etc1_a_8",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"popcap.ptx.encode",
					.argument = LR"({ "format": "rgb_etc1_a_8" })",
				},
				{
					.id = L"popcap.ptx.decode:rgb_etc1_a_8",
					.type = false,
					.rule = std::wregex{LR"(.+(\.ptx)$)", std::wregex::icase},
					.method = L"popcap.ptx.decode",
					.argument = LR"({ "format": "rgb_etc1_a_8" })",
				},
				{
					.id = L"popcap.ptx.encode:rgb_etc1_a_palette",
					.type = false,
					.rule = std::wregex{LR"(.+(\.png)$)", std::wregex::icase},
					.method = L"popcap.ptx.encode",
					.argument = LR"({ "format": "rgb_etc1_a_palette" })",
				},
				{
					.id = L"popcap.ptx.decode:rgb_etc1_a_palette",
					.type = false,
					.rule = std::wregex{LR"(.+(\.ptx)$)", std::wregex::icase},
					.method = L"popcap.ptx.decode",
					.argument = LR"({ "format": "rgb_etc1_a_palette" })",
				},
			},
			.separator = {
				2,
				2,
				2,
				2,
				2,
				2,
			},
		};

		inline auto const popcap_pam = GroupMethodInvokeCommandConfig{
			.id = L"popcap.pam",
			.child = {
				{
					.id = L"popcap.pam.encode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.pam)(\.json)$)", std::wregex::icase},
					.method = L"popcap.pam.encode",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.decode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.pam)$)", std::wregex::icase},
					.method = L"popcap.pam.decode",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.convert.flash.from",
					.type = false,
					.rule = std::wregex{LR"(.+(\.pam)(\.json)$)", std::wregex::icase},
					.method = L"popcap.pam.convert.flash.from",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.convert.flash.to",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pam)(\.xfl)$)", std::wregex::icase},
					.method = L"popcap.pam.convert.flash.to",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.convert.flash.resize",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pam)(\.xfl)$)", std::wregex::icase},
					.method = L"popcap.pam.convert.flash.resize",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.convert.flash.resize:1536",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pam)(\.xfl)$)", std::wregex::icase},
					.method = L"popcap.pam.convert.flash.resize",
					.argument = LR"({ "resolution": 1536 })",
				},
				{
					.id = L"popcap.pam.convert.flash.resize:768",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pam)(\.xfl)$)", std::wregex::icase},
					.method = L"popcap.pam.convert.flash.resize",
					.argument = LR"({ "resolution": 768 })",
				},
				{
					.id = L"popcap.pam.convert.flash.link_media",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pam)(\.xfl)$)", std::wregex::icase},
					.method = L"popcap.pam.convert.flash.link_media",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.encode.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pam.encode.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.decode.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pam.decode.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.convert.flash.from.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pam.convert.flash.from.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.convert.flash.to.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pam.convert.flash.to.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.convert.flash.resize.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pam.convert.flash.resize.batch",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pam.convert.flash.resize.batch:1536",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pam.convert.flash.resize.batch",
					.argument = LR"({ "resolution": 1536 })",
				},
				{
					.id = L"popcap.pam.convert.flash.resize.batch:768",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pam.convert.flash.resize.batch",
					.argument = LR"({ "resolution": 768 })",
				},
				{
					.id = L"popcap.pam.convert.flash.link_media.batch",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pam.convert.flash.link_media.batch",
					.argument = LR"({})",
				},
			},
			.separator = {
				2,
				2,
				3,
				1,
				2,
				2,
				3,
				1,
			},
		};

		inline auto const popcap_pak = GroupMethodInvokeCommandConfig{
			.id = L"popcap.pak",
			.child = {
				{
					.id = L"popcap.pak.pack",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pak)(\.bundle)$)", std::wregex::icase},
					.method = L"popcap.pak.pack",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pak.unpack",
					.type = false,
					.rule = std::wregex{LR"(.+(\.pak)$)", std::wregex::icase},
					.method = L"popcap.pak.unpack",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pak.pack_automatic",
					.type = true,
					.rule = std::nullopt,
					.method = L"popcap.pak.pack_automatic",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.pak.encrypt",
					.type = false,
					.rule = std::wregex{LR"(.+(\.pak)$)", std::wregex::icase},
					.method = L"popcap.pak.encrypt",
					.argument = LR"({})",
				},
			},
			.separator = {
				2,
				1,
				1,
			},
		};

		inline auto const popcap_rsgp = GroupMethodInvokeCommandConfig{
			.id = L"popcap.rsgp",
			.child = {
				{
					.id = L"popcap.rsgp.pack",
					.type = true,
					.rule = std::wregex{LR"(.+(\.rsgp)(\.bundle)$)", std::wregex::icase},
					.method = L"popcap.rsgp.pack",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rsgp.unpack",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rsgp)$)", std::wregex::icase},
					.method = L"popcap.rsgp.unpack",
					.argument = LR"({})",
				},
			},
			.separator = {
				2,
			},
		};

		inline auto const popcap_rsb = GroupMethodInvokeCommandConfig{
			.id = L"popcap.rsb",
			.child = {
				{
					.id = L"popcap.rsb.pack",
					.type = true,
					.rule = std::wregex{LR"(.+(\.rsb)(\.bundle)$)", std::wregex::icase},
					.method = L"popcap.rsb.pack",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rsb.unpack",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rsb)$)", std::wregex::icase},
					.method = L"popcap.rsb.unpack",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rsb.resource_convert",
					.type = true,
					.rule = std::wregex{LR"(.+(\.rsb)(\.bundle)$)", std::wregex::icase},
					.method = L"popcap.rsb.resource_convert",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rsb.repair",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rsb)$)", std::wregex::icase},
					.method = L"popcap.rsb.repair",
					.argument = LR"({})",
				},
			},
			.separator = {
				2,
				1,
				1,
			},
		};

		inline auto const popcap_rsb_patch = GroupMethodInvokeCommandConfig{
			.id = L"popcap.rsb_patch",
			.child = {
				{
					.id = L"popcap.rsb_patch.encode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rsb)$)", std::wregex::icase},
					.method = L"popcap.rsb_patch.encode",
					.argument = LR"({})",
				},
				{
					.id = L"popcap.rsb_patch.decode",
					.type = false,
					.rule = std::wregex{LR"(.+(\.rsb)$)", std::wregex::icase},
					.method = L"popcap.rsb_patch.decode",
					.argument = LR"({})",
				},
			},
			.separator = {
				2,
			},
		};

		inline auto const pvz2_lawn_string_text = GroupMethodInvokeCommandConfig{
			.id = L"pvz2.lawn_string_text",
			.child = {
				{
					.id = L"pvz2.lawn_string_text.convert:text",
					.type = false,
					.rule = std::wregex{LR"(.*(LawnStrings).*(\.(txt|json))$)", std::wregex::icase},
					.method = L"pvz2.lawn_string_text.convert",
					.argument = LR"({ "destination_version": "text" })",
				},
				{
					.id = L"pvz2.lawn_string_text.convert:json_map",
					.type = false,
					.rule = std::wregex{LR"(.*(LawnStrings).*(\.(txt|json))$)", std::wregex::icase},
					.method = L"pvz2.lawn_string_text.convert",
					.argument = LR"({ "destination_version": "json_map" })",
				},
				{
					.id = L"pvz2.lawn_string_text.convert:json_list",
					.type = false,
					.rule = std::wregex{LR"(.*(LawnStrings).*(\.(txt|json))$)", std::wregex::icase},
					.method = L"pvz2.lawn_string_text.convert",
					.argument = LR"({ "destination_version": "json_list" })",
				},
			},
			.separator = {
				3,
			},
		};

		inline auto const pvz2_remote_android_helper = GroupMethodInvokeCommandConfig{
			.id = L"pvz2.remote_android_helper",
			.child = {
				{
					.id = L"pvz2.remote_android_helper.launch:pull_main_package",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "pull_main_package" })",
				},
				{
					.id = L"pvz2.remote_android_helper.launch:push_main_package",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "push_main_package" })",
				},
				{
					.id = L"pvz2.remote_android_helper.launch:pull_local_profile",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "pull_local_profile" })",
				},
				{
					.id = L"pvz2.remote_android_helper.launch:push_local_profile",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "push_local_profile" })",
				},
				{
					.id = L"pvz2.remote_android_helper.launch:pull_player_profile",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "pull_player_profile" })",
				},
				{
					.id = L"pvz2.remote_android_helper.launch:push_player_profile",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "push_player_profile" })",
				},
				{
					.id = L"pvz2.remote_android_helper.launch:pull_content_delivery",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "pull_content_delivery" })",
				},
				{
					.id = L"pvz2.remote_android_helper.launch:push_content_delivery",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "push_content_delivery" })",
				},
				{
					.id = L"pvz2.remote_android_helper.launch:clear_snapshot",
					.type = true,
					.rule = std::wregex{LR"(.+(\.pvz2_remote_android_helper_project)$)", std::wregex::icase},
					.method = L"pvz2.remote_android_helper.launch",
					.argument = LR"({ "action": "clear_snapshot" })",
				},
			},
			.separator = {
				2,
				2,
				2,
				2,
				1,
			},
		};

	}

	#pragma endregion

	#pragma region type

	M_define_exposed_method_invoke_command("9992EC48-22A5-86FA-EA42-72DA1A53F23D", MethodInvokeCommand, Main, main);

	// ----------------

	M_define_exposed_method_invoke_command("A918A8A8-1436-43C5-99C1-9420D0D56D40", MethodInvokeCommand, Launch, launch);

	M_define_exposed_visible_method_invoke_command("314F8A42-AC6F-4192-8F9A-D2DF1C9F0546", GroupMethodInvokeCommand, JS, js);

	M_define_exposed_visible_method_invoke_command("D60FB9DD-7677-4742-A2BC-C9049E268CBA", GroupMethodInvokeCommand, JSON, json);

	M_define_exposed_visible_method_invoke_command("1DAED04B-CD51-45DA-91E4-F15DC831C7D5", GroupMethodInvokeCommand, Data, data);

	M_define_exposed_visible_method_invoke_command("2BEEB70E-8211-4DB4-B6BF-8D6FDAB791BF", GroupMethodInvokeCommand, Image, image);

	M_define_exposed_visible_method_invoke_command("C9A257AC-8048-4257-85AD-CE68CACD7922", GroupMethodInvokeCommand, WwiseEncodedMedia, wwise_encoded_media);

	M_define_exposed_visible_method_invoke_command("8B32AE59-795E-4F2D-B3B8-A4558CF9488F", GroupMethodInvokeCommand, WwiseSoundBank, wwise_sound_bank);

	M_define_exposed_visible_method_invoke_command("164D100A-5228-429D-A926-EFF742D00E9D", GroupMethodInvokeCommand, MarmaladeDZip, marmalade_dzip);

	M_define_exposed_visible_method_invoke_command("5F334CE4-29A4-4B0F-A696-10EB3D773666", GroupMethodInvokeCommand, PopCapZLib, popcap_zlib);

	M_define_exposed_visible_method_invoke_command("E2D6187D-7E0E-4089-82F2-768A6EE7D4D1", GroupMethodInvokeCommand, PopCapREANIM, popcap_reanim);

	M_define_exposed_visible_method_invoke_command("0DD08DF0-3B5D-5273-99AA-1A3766355ED1", GroupMethodInvokeCommand, PopCapPARTICLE, popcap_particle);

	M_define_exposed_visible_method_invoke_command("A7121460-0260-4E4A-829F-D21A7E649B50", GroupMethodInvokeCommand, PopCapRTON, popcap_rton);

	M_define_exposed_visible_method_invoke_command("654DB9CD-DA07-4942-96A5-57F948C6A44C", GroupMethodInvokeCommand, PopCapPTX, popcap_ptx);

	M_define_exposed_visible_method_invoke_command("D3EBD69C-CB8B-452D-BFC7-7C06519BDA68", GroupMethodInvokeCommand, PopCapPAM, popcap_pam);

	M_define_exposed_visible_method_invoke_command("64DBC2C3-402F-42AA-B8BF-B43B3280F813", GroupMethodInvokeCommand, PopCapPAK, popcap_pak);

	M_define_exposed_visible_method_invoke_command("95E74A50-81DC-48BA-A24B-E0AD30C75139", GroupMethodInvokeCommand, PopCapRSGP, popcap_rsgp);

	M_define_exposed_visible_method_invoke_command("BCF8438E-67D1-4B7B-8649-78C46FAB4CFE", GroupMethodInvokeCommand, PopCapRSB, popcap_rsb);

	M_define_exposed_visible_method_invoke_command("C83A33D4-B96D-4002-9D76-4DB88AF589C0", GroupMethodInvokeCommand, PopCapRSBPatch, popcap_rsb_patch);

	M_define_exposed_visible_method_invoke_command("3F961602-866A-4305-8031-42597990C6AB", GroupMethodInvokeCommand, PvZ2LawnStringText, pvz2_lawn_string_text);

	M_define_exposed_visible_method_invoke_command("CD04E4D3-2224-14FC-C9DD-66D746408D62", GroupMethodInvokeCommand, PvZ2RemoteAndroidHelper, pvz2_remote_android_helper);

	#pragma endregion

}
