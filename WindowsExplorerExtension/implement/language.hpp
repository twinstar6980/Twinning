#pragma once

#include "implement/common.hpp"

namespace TwinStar::WindowsExplorerExtension::Language {

	#pragma region constant

	inline auto const g_table = std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::wstring>>{
		{L"Chinese", {
			{L"method:main", L"TwinStar ToolKit - Extension"},
			{L"method:launch", L"Launch"},
			{L"method:js", L"JS"},
			{L"method:js.execute", L"执行"},
			{L"method:json", L"JSON"},
			{L"method:json.format", L"格式化"},
			{L"method:json.format:default", L"格式化 ~ 默认格式"},
			{L"method:json.format.batch", L"[批处理] 格式化"},
			{L"method:json.format.batch:default", L"[批处理] 格式化 ~ 默认格式"},
			{L"method:data", L"Data"},
			{L"method:data.hash.md5", L"MD5 散列"},
			{L"method:data.encoding.base64.encode", L"Base64 编码"},
			{L"method:data.encoding.base64.decode", L"Base64 解码"},
			{L"method:data.encryption.xor.encrypt", L"XOR 加密"},
			{L"method:data.encryption.rijndael.encrypt", L"Rijndael 加密"},
			{L"method:data.encryption.rijndael.decrypt", L"Rijndael 解密"},
			{L"method:data.compression.deflate.compress", L"Deflate 压缩"},
			{L"method:data.compression.deflate.uncompress", L"Deflate 解压"},
			{L"method:data.compression.zlib.compress", L"ZLib 压缩"},
			{L"method:data.compression.zlib.uncompress", L"ZLib 解压"},
			{L"method:data.compression.gzip.compress", L"GZip 压缩"},
			{L"method:data.compression.gzip.uncompress", L"GZip 解压"},
			{L"method:data.compression.bzip2.compress", L"BZip2 压缩"},
			{L"method:data.compression.bzip2.uncompress", L"BZip2 解压"},
			{L"method:data.compression.lzma.compress", L"Lzma 压缩"},
			{L"method:data.compression.lzma.uncompress", L"Lzma 解压"},
			{L"method:data.differentiation.vcdiff.encode", L"VCDiff 编码"},
			{L"method:data.differentiation.vcdiff.decode", L"VCDiff 解码"},
			{L"method:image", L"Image"},
			{L"method:image.transformation.flip", L"翻转"},
			{L"method:image.transformation.scale", L"缩放"},
			{L"method:image.transformation.scale_rate", L"比例缩放"},
			{L"method:image.atlas.pack", L"图集打包"},
			{L"method:image.atlas.unpack", L"图集解包"},
			{L"method:image.atlas.pack_automatic", L"图集自动打包"},
			{L"method:wwise.media", L"Wwise Media"},
			{L"method:wwise.media.decode", L"解码"},
			{L"method:wwise.media.decode.batch", L"[批处理] 解码"},
			{L"method:wwise.sound_bank", L"Wwise Sound-Bank"},
			{L"method:wwise.sound_bank.encode", L"编码"},
			{L"method:wwise.sound_bank.decode", L"解码"},
			{L"method:wwise.sound_bank.encode.batch", L"[批处理] 编码"},
			{L"method:wwise.sound_bank.decode.batch", L"[批处理] 解码"},
			{L"method:marmalade.dzip", L"Marmalade DZip"},
			{L"method:marmalade.dzip.pack", L"打包"},
			{L"method:marmalade.dzip.unpack", L"解包"},
			{L"method:marmalade.dzip.pack_automatic", L"自动打包"},
			{L"method:popcap.zlib", L"PopCap ZLib"},
			{L"method:popcap.zlib.compress:32", L"压缩 ~ 32"},
			{L"method:popcap.zlib.uncompress:32", L"解压 ~ 32"},
			{L"method:popcap.zlib.compress:64", L"压缩 ~ 64"},
			{L"method:popcap.zlib.uncompress:64", L"解压 ~ 64"},
			{L"method:popcap.zlib.compress.batch:32", L"[批处理] 压缩 ~ 32"},
			{L"method:popcap.zlib.uncompress.batch:32", L"[批处理] 解压 ~ 32"},
			{L"method:popcap.zlib.compress.batch:64", L"[批处理] 压缩 ~ 64"},
			{L"method:popcap.zlib.uncompress.batch:64", L"[批处理] 解压 ~ 64"},
			{L"method:popcap.crypt_data", L"PopCap Crypt-Data"},
			{L"method:popcap.crypt_data.encrypt", L"加密"},
			{L"method:popcap.crypt_data.decrypt", L"解密"},
			{L"method:popcap.crypt_data.encrypt.batch", L"[批处理] 加密"},
			{L"method:popcap.crypt_data.decrypt.batch", L"[批处理] 解密"},
			{L"method:popcap.reflection_object_notation", L"PopCap Reflection-Object-Notation"},
			{L"method:popcap.reflection_object_notation.encode", L"编码"},
			{L"method:popcap.reflection_object_notation.decode", L"解码"},
			{L"method:popcap.reflection_object_notation.encrypt", L"加密"},
			{L"method:popcap.reflection_object_notation.decrypt", L"解密"},
			{L"method:popcap.reflection_object_notation.encode_then_encrypt", L"编码并加密"},
			{L"method:popcap.reflection_object_notation.decrypt_then_decode", L"解密并解码"},
			{L"method:popcap.reflection_object_notation.decode_lenient", L"宽松解码"},
			{L"method:popcap.reflection_object_notation.encode.batch", L"[批处理] 编码"},
			{L"method:popcap.reflection_object_notation.decode.batch", L"[批处理] 解码"},
			{L"method:popcap.reflection_object_notation.encrypt.batch", L"[批处理] 加密"},
			{L"method:popcap.reflection_object_notation.decrypt.batch", L"[批处理] 解密"},
			{L"method:popcap.reflection_object_notation.encode_then_encrypt.batch", L"[批处理] 编码并加密"},
			{L"method:popcap.reflection_object_notation.decrypt_then_decode.batch", L"[批处理] 解密并解码"},
			{L"method:popcap.texture", L"PopCap Texture"},
			{L"method:popcap.texture.encode", L"编码"},
			{L"method:popcap.texture.decode", L"解码"},
			{L"method:popcap.texture.encode:rgba_8888_o", L"编码 ~ rgba_8888_o"},
			{L"method:popcap.texture.decode:rgba_8888_o", L"解码 ~ rgba_8888_o"},
			{L"method:popcap.texture.encode:argb_8888", L"编码 ~ argb_8888"},
			{L"method:popcap.texture.decode:argb_8888", L"解码 ~ argb_8888"},
			{L"method:popcap.texture.encode:rgba_pvrtc4", L"编码 ~ rgba_pvrtc4"},
			{L"method:popcap.texture.decode:rgba_pvrtc4", L"解码 ~ rgba_pvrtc4"},
			{L"method:popcap.texture.encode:rgb_etc1_a_8", L"编码 ~ rgb_etc1_a_8"},
			{L"method:popcap.texture.decode:rgb_etc1_a_8", L"解码 ~ rgb_etc1_a_8"},
			{L"method:popcap.texture.encode:rgb_etc1_a_palette", L"编码 ~ rgb_etc1_a_palette"},
			{L"method:popcap.texture.decode:rgb_etc1_a_palette", L"解码 ~ rgb_etc1_a_palette"},
			{L"method:popcap.u_texture", L"PopCap U-Texture"},
			{L"method:popcap.u_texture.encode", L"编码"},
			{L"method:popcap.u_texture.decode", L"解码"},
			{L"method:popcap.u_texture.encode.batch", L"[批处理] 编码"},
			{L"method:popcap.u_texture.decode.batch", L"[批处理] 解码"},
			{L"method:popcap.sexy_texture", L"PopCap Sexy-Texture"},
			{L"method:popcap.sexy_texture.encode", L"编码"},
			{L"method:popcap.sexy_texture.decode", L"解码"},
			{L"method:popcap.sexy_texture.encode.batch", L"[批处理] 编码"},
			{L"method:popcap.sexy_texture.decode.batch", L"[批处理] 解码"},
			{L"method:popcap.animation", L"PopCap Animation"},
			{L"method:popcap.animation.encode", L"编码"},
			{L"method:popcap.animation.decode", L"解码"},
			{L"method:popcap.animation.convert.flash.from", L"Flash 转换自JSON"},
			{L"method:popcap.animation.convert.flash.to", L"Flash 转换至JSON"},
			{L"method:popcap.animation.convert.flash.resize", L"Flash 图像分辨率调整"},
			{L"method:popcap.animation.convert.flash.resize:1536", L"Flash 图像分辨率调整 ~ 1536"},
			{L"method:popcap.animation.convert.flash.resize:768", L"Flash 图像分辨率调整 ~ 768"},
			{L"method:popcap.animation.convert.flash.link_media", L"Flash 创建图像文件链接"},
			{L"method:popcap.animation.encode.batch", L"[批处理] 编码"},
			{L"method:popcap.animation.decode.batch", L"[批处理] 解码"},
			{L"method:popcap.animation.convert.flash.from.batch", L"[批处理] Flash 转换自JSON"},
			{L"method:popcap.animation.convert.flash.to.batch", L"[批处理] Flash 转换至JSON"},
			{L"method:popcap.animation.convert.flash.resize.batch", L"[批处理] Flash 图像分辨率调整"},
			{L"method:popcap.animation.convert.flash.resize.batch:1536", L"[批处理] Flash 图像分辨率调整 ~ 1536"},
			{L"method:popcap.animation.convert.flash.resize.batch:768", L"[批处理] Flash 图像分辨率调整 ~ 768"},
			{L"method:popcap.animation.convert.flash.link_media.batch", L"[批处理] Flash 创建图像文件链接"},
			{L"method:popcap.re_animation", L"PopCap Re-Animation"},
			{L"method:popcap.re_animation.encode:desktop_32", L"编码 ~ desktop_32"},
			{L"method:popcap.re_animation.decode:desktop_32", L"解码 ~ desktop_32"},
			{L"method:popcap.re_animation.encode:mobile_32", L"编码 ~ mobile_32"},
			{L"method:popcap.re_animation.decode:mobile_32", L"解码 ~ mobile_32"},
			{L"method:popcap.re_animation.encode:mobile_64", L"编码 ~ mobile_64"},
			{L"method:popcap.re_animation.decode:mobile_64", L"解码 ~ mobile_64"},
			{L"method:popcap.re_animation.encode:television_32", L"编码 ~ television_32"},
			{L"method:popcap.re_animation.decode:television_32", L"解码 ~ television_32"},
			{L"method:popcap.re_animation.encode.batch:desktop_32", L"[批处理] 编码 ~ desktop_32"},
			{L"method:popcap.re_animation.decode.batch:desktop_32", L"[批处理] 解码 ~ desktop_32"},
			{L"method:popcap.re_animation.encode.batch:mobile_32", L"[批处理] 编码 ~ mobile_32"},
			{L"method:popcap.re_animation.decode.batch:mobile_32", L"[批处理] 解码 ~ mobile_32"},
			{L"method:popcap.re_animation.encode.batch:mobile_64", L"[批处理] 编码 ~ mobile_64"},
			{L"method:popcap.re_animation.decode.batch:mobile_64", L"[批处理] 解码 ~ mobile_64"},
			{L"method:popcap.re_animation.encode.batch:television_32", L"[批处理] 编码 ~ television_32"},
			{L"method:popcap.re_animation.decode.batch:television_32", L"[批处理] 解码 ~ television_32"},
			{L"method:popcap.particle", L"PopCap Particle"},
			{L"method:popcap.particle.encode:desktop_32", L"编码 ~ desktop_32"},
			{L"method:popcap.particle.decode:desktop_32", L"解码 ~ desktop_32"},
			{L"method:popcap.particle.encode:mobile_32", L"编码 ~ mobile_32"},
			{L"method:popcap.particle.decode:mobile_32", L"解码 ~ mobile_32"},
			{L"method:popcap.particle.encode:mobile_64", L"编码 ~ mobile_64"},
			{L"method:popcap.particle.decode:mobile_64", L"解码 ~ mobile_64"},
			{L"method:popcap.particle.encode:television_32", L"编码 ~ television_32"},
			{L"method:popcap.particle.decode:television_32", L"解码 ~ television_32"},
			{L"method:popcap.particle.encode.batch:desktop_32", L"[批处理] 编码 ~ desktop_32"},
			{L"method:popcap.particle.decode.batch:desktop_32", L"[批处理] 解码 ~ desktop_32"},
			{L"method:popcap.particle.encode.batch:mobile_32", L"[批处理] 编码 ~ mobile_32"},
			{L"method:popcap.particle.decode.batch:mobile_32", L"[批处理] 解码 ~ mobile_32"},
			{L"method:popcap.particle.encode.batch:mobile_64", L"[批处理] 编码 ~ mobile_64"},
			{L"method:popcap.particle.decode.batch:mobile_64", L"[批处理] 解码 ~ mobile_64"},
			{L"method:popcap.particle.encode.batch:television_32", L"[批处理] 编码 ~ television_32"},
			{L"method:popcap.particle.decode.batch:television_32", L"[批处理] 解码 ~ television_32"},
			{L"method:popcap.trail", L"PopCap Trail"},
			{L"method:popcap.trail.encode:desktop_32", L"编码 ~ desktop_32"},
			{L"method:popcap.trail.decode:desktop_32", L"解码 ~ desktop_32"},
			{L"method:popcap.trail.encode:mobile_32", L"编码 ~ mobile_32"},
			{L"method:popcap.trail.decode:mobile_32", L"解码 ~ mobile_32"},
			{L"method:popcap.trail.encode:mobile_64", L"编码 ~ mobile_64"},
			{L"method:popcap.trail.decode:mobile_64", L"解码 ~ mobile_64"},
			{L"method:popcap.trail.encode:television_32", L"编码 ~ television_32"},
			{L"method:popcap.trail.decode:television_32", L"解码 ~ television_32"},
			{L"method:popcap.trail.encode.batch:desktop_32", L"[批处理] 编码 ~ desktop_32"},
			{L"method:popcap.trail.decode.batch:desktop_32", L"[批处理] 解码 ~ desktop_32"},
			{L"method:popcap.trail.encode.batch:mobile_32", L"[批处理] 编码 ~ mobile_32"},
			{L"method:popcap.trail.decode.batch:mobile_32", L"[批处理] 解码 ~ mobile_32"},
			{L"method:popcap.trail.encode.batch:mobile_64", L"[批处理] 编码 ~ mobile_64"},
			{L"method:popcap.trail.decode.batch:mobile_64", L"[批处理] 解码 ~ mobile_64"},
			{L"method:popcap.trail.encode.batch:television_32", L"[批处理] 编码 ~ television_32"},
			{L"method:popcap.trail.decode.batch:television_32", L"[批处理] 解码 ~ television_32"},
			{L"method:popcap.effect", L"PopCap Effect"},
			{L"method:popcap.effect.encode", L"编码"},
			{L"method:popcap.effect.decode", L"解码"},
			{L"method:popcap.effect.encode.batch", L"[批处理] 编码"},
			{L"method:popcap.effect.decode.batch", L"[批处理] 解码"},
			{L"method:popcap.character_font_widget_2", L"PopCap Character-Font-Widget-2"},
			{L"method:popcap.character_font_widget_2.encode", L"编码"},
			{L"method:popcap.character_font_widget_2.decode", L"解码"},
			{L"method:popcap.character_font_widget_2.encode.batch", L"[批处理] 编码"},
			{L"method:popcap.character_font_widget_2.decode.batch", L"[批处理] 解码"},
			{L"method:popcap.package", L"PopCap Package"},
			{L"method:popcap.package.pack", L"打包"},
			{L"method:popcap.package.unpack", L"解包"},
			{L"method:popcap.package.pack_automatic", L"自动打包"},
			{L"method:popcap.package.encrypt", L"加密"},
			{L"method:popcap.resource_stream_group", L"PopCap Resource-Stream-Group"},
			{L"method:popcap.resource_stream_group.pack", L"打包"},
			{L"method:popcap.resource_stream_group.unpack", L"解包"},
			{L"method:popcap.resource_stream_bundle", L"PopCap Resource-Stream-Bundle"},
			{L"method:popcap.resource_stream_bundle.pack", L"打包"},
			{L"method:popcap.resource_stream_bundle.unpack", L"解包"},
			{L"method:popcap.resource_stream_bundle.resource_convert", L"资源转换"},
			{L"method:popcap.resource_stream_bundle.repair", L"修复"},
			{L"method:popcap.resource_stream_bundle_patch", L"PopCap Resource-Stream-Bundle-Patch"},
			{L"method:popcap.resource_stream_bundle_patch.encode", L"编码"},
			{L"method:popcap.resource_stream_bundle_patch.decode", L"解码"},
			{L"method:pvz2.text_table", L"PvZ-2 Text-Table"},
			{L"method:pvz2.text_table.convert:text", L"转换 ~ text"},
			{L"method:pvz2.text_table.convert:json_map", L"转换 ~ json_map"},
			{L"method:pvz2.text_table.convert:json_list", L"转换 ~ json_list"},
			{L"method:pvz2.remote_android_helper", L"PvZ-2 Remote-Android-Helper"},
			{L"method:pvz2.remote_android_helper.launch:pull_main_package", L"拉取主数据包"},
			{L"method:pvz2.remote_android_helper.launch:push_main_package", L"推送主数据包"},
			{L"method:pvz2.remote_android_helper.launch:pull_local_profile", L"拉取本地配置"},
			{L"method:pvz2.remote_android_helper.launch:push_local_profile", L"推送本地配置"},
			{L"method:pvz2.remote_android_helper.launch:pull_player_profile", L"拉取玩家配置"},
			{L"method:pvz2.remote_android_helper.launch:push_player_profile", L"推送玩家配置"},
			{L"method:pvz2.remote_android_helper.launch:pull_content_delivery", L"拉取内容分发"},
			{L"method:pvz2.remote_android_helper.launch:push_content_delivery", L"推送内容分发"},
			{L"method:pvz2.remote_android_helper.launch:clear_snapshot", L"清除快照"},
		}},
		{L"English", {
			{L"method:main", L"TwinStar ToolKit - Extension"},
			{L"method:launch", L"Launch"},
			{L"method:js", L"JS"},
			{L"method:js.execute", L"Execute"},
			{L"method:json", L"JSON"},
			{L"method:json.format", L"Format"},
			{L"method:json.format:default", L"Format ~ default"},
			{L"method:json.format.batch", L"[BATCH] Format"},
			{L"method:json.format.batch:default", L"[BATCH] Format ~ default"},
			{L"method:data", L"Data"},
			{L"method:data.hash.md5", L"MD5 Hash"},
			{L"method:data.encoding.base64.encode", L"Base64 Encode"},
			{L"method:data.encoding.base64.decode", L"Base64 Decode"},
			{L"method:data.encryption.xor.encrypt", L"XOR Encrypt"},
			{L"method:data.encryption.rijndael.encrypt", L"Rijndael Encrypt"},
			{L"method:data.encryption.rijndael.decrypt", L"Rijndael Decrypt"},
			{L"method:data.compression.deflate.compress", L"Deflate Compress"},
			{L"method:data.compression.deflate.uncompress", L"Deflate Uncompress"},
			{L"method:data.compression.zlib.compress", L"ZLib Compress"},
			{L"method:data.compression.zlib.uncompress", L"ZLib Uncompress"},
			{L"method:data.compression.gzip.compress", L"GZip Compress"},
			{L"method:data.compression.gzip.uncompress", L"GZip Uncompress"},
			{L"method:data.compression.bzip2.compress", L"BZip2 Compress"},
			{L"method:data.compression.bzip2.uncompress", L"BZip2 Uncompress"},
			{L"method:data.compression.lzma.compress", L"Lzma Compress"},
			{L"method:data.compression.lzma.uncompress", L"Lzma Uncompress"},
			{L"method:data.differentiation.vcdiff.encode", L"VCDiff Encode"},
			{L"method:data.differentiation.vcdiff.decode", L"VCDiff Decode"},
			{L"method:image", L"Image"},
			{L"method:image.transformation.flip", L"Flip"},
			{L"method:image.transformation.scale", L"Scale"},
			{L"method:image.transformation.scale_rate", L"Scale by rate"},
			{L"method:image.atlas.pack", L"Atlas Pack"},
			{L"method:image.atlas.unpack", L"Atlas Unpack"},
			{L"method:image.atlas.pack_automatic", L"Atlas Pack by automatic"},
			{L"method:wwise.media", L"Wwise Media"},
			{L"method:wwise.media.decode", L"Decode"},
			{L"method:wwise.media.decode.batch", L"[BATCH] Decode"},
			{L"method:wwise.sound_bank", L"Wwise Sound-Bank"},
			{L"method:wwise.sound_bank.encode", L"Encode"},
			{L"method:wwise.sound_bank.decode", L"Decode"},
			{L"method:wwise.sound_bank.encode.batch", L"[BATCH] Encode"},
			{L"method:wwise.sound_bank.decode.batch", L"[BATCH] Decode"},
			{L"method:marmalade.dzip", L"Marmalade DZip"},
			{L"method:marmalade.dzip.pack", L"Pack"},
			{L"method:marmalade.dzip.unpack", L"Unpack"},
			{L"method:marmalade.dzip.pack_automatic", L"Pack by automatic"},
			{L"method:popcap.zlib", L"PopCap ZLib"},
			{L"method:popcap.zlib.compress:32", L"Compress ~ 32"},
			{L"method:popcap.zlib.uncompress:32", L"Uncompress ~ 32"},
			{L"method:popcap.zlib.compress:64", L"Compress ~ 64"},
			{L"method:popcap.zlib.uncompress:64", L"Uncompress ~ 64"},
			{L"method:popcap.zlib.compress.batch:32", L"[BATCH] Compress ~ 32"},
			{L"method:popcap.zlib.uncompress.batch:32", L"[BATCH] Uncompress ~ 32"},
			{L"method:popcap.zlib.compress.batch:64", L"[BATCH] Compress ~ 64"},
			{L"method:popcap.zlib.uncompress.batch:64", L"[BATCH] Uncompress ~ 64"},
			{L"method:popcap.crypt_data", L"PopCap Crypt-Data"},
			{L"method:popcap.crypt_data.encrypt", L"Encrypt"},
			{L"method:popcap.crypt_data.decrypt", L"Decrypt"},
			{L"method:popcap.crypt_data.encrypt.batch", L"[BATCH] Encrypt"},
			{L"method:popcap.crypt_data.decrypt.batch", L"[BATCH] Decrypt"},
			{L"method:popcap.reflection_object_notation", L"PopCap Reflection-Object-Notation"},
			{L"method:popcap.reflection_object_notation.encode", L"Encode"},
			{L"method:popcap.reflection_object_notation.decode", L"Decode"},
			{L"method:popcap.reflection_object_notation.encrypt", L"Encrypt"},
			{L"method:popcap.reflection_object_notation.decrypt", L"Decrypt"},
			{L"method:popcap.reflection_object_notation.encode_then_encrypt", L"Encode && Encrypt"},
			{L"method:popcap.reflection_object_notation.decrypt_then_decode", L"Decrypt && Decode"},
			{L"method:popcap.reflection_object_notation.decode_lenient", L"Decode by lenient"},
			{L"method:popcap.reflection_object_notation.encode.batch", L"[BATCH] Encode"},
			{L"method:popcap.reflection_object_notation.decode.batch", L"[BATCH] Decode"},
			{L"method:popcap.reflection_object_notation.encrypt.batch", L"[BATCH] Encrypt"},
			{L"method:popcap.reflection_object_notation.decrypt.batch", L"[BATCH] Decrypt"},
			{L"method:popcap.reflection_object_notation.encode_then_encrypt.batch", L"[BATCH] Encode && Encrypt"},
			{L"method:popcap.reflection_object_notation.decrypt_then_decode.batch", L"[BATCH] Decrypt && Decode"},
			{L"method:popcap.texture", L"PopCap Texture"},
			{L"method:popcap.texture.encode", L"Encode"},
			{L"method:popcap.texture.decode", L"Decode"},
			{L"method:popcap.texture.encode:rgba_8888_o", L"Encode ~ rgba_8888_o"},
			{L"method:popcap.texture.decode:rgba_8888_o", L"Decode ~ rgba_8888_o"},
			{L"method:popcap.texture.encode:argb_8888", L"Encode ~ argb_8888"},
			{L"method:popcap.texture.decode:argb_8888", L"Decode ~ argb_8888"},
			{L"method:popcap.texture.encode:rgba_pvrtc4", L"Encode ~ rgba_pvrtc4"},
			{L"method:popcap.texture.decode:rgba_pvrtc4", L"Decode ~ rgba_pvrtc4"},
			{L"method:popcap.texture.encode:rgb_etc1_a_8", L"Encode ~ rgb_etc1_a_8"},
			{L"method:popcap.texture.decode:rgb_etc1_a_8", L"Decode ~ rgb_etc1_a_8"},
			{L"method:popcap.texture.encode:rgb_etc1_a_palette", L"Encode ~ rgb_etc1_a_palette"},
			{L"method:popcap.texture.decode:rgb_etc1_a_palette", L"Decode ~ rgb_etc1_a_palette"},
			{L"method:popcap.u_texture", L"PopCap U-Texture"},
			{L"method:popcap.u_texture.encode", L"Encode"},
			{L"method:popcap.u_texture.decode", L"Decode"},
			{L"method:popcap.u_texture.encode.batch", L"[BATCH] Encode"},
			{L"method:popcap.u_texture.decode.batch", L"[BATCH] Decode"},
			{L"method:popcap.sexy_texture", L"PopCap Sexy-Texture"},
			{L"method:popcap.sexy_texture.encode", L"Encode"},
			{L"method:popcap.sexy_texture.decode", L"Decode"},
			{L"method:popcap.sexy_texture.encode.batch", L"[BATCH] Encode"},
			{L"method:popcap.sexy_texture.decode.batch", L"[BATCH] Decode"},
			{L"method:popcap.animation", L"PopCap Animation"},
			{L"method:popcap.animation.encode", L"Encode"},
			{L"method:popcap.animation.decode", L"Decode"},
			{L"method:popcap.animation.convert.flash.from", L"Flash Convert from JSON"},
			{L"method:popcap.animation.convert.flash.to", L"Flash Convert to JSON"},
			{L"method:popcap.animation.convert.flash.resize", L"Flash Resize image"},
			{L"method:popcap.animation.convert.flash.resize:1536", L"Flash Resize image ~ 1536"},
			{L"method:popcap.animation.convert.flash.resize:768", L"Flash Resize image ~ 768"},
			{L"method:popcap.animation.convert.flash.link_media", L"Flash Create links for image files"},
			{L"method:popcap.animation.encode.batch", L"[BATCH] Encode"},
			{L"method:popcap.animation.decode.batch", L"[BATCH] Decode"},
			{L"method:popcap.animation.convert.flash.from.batch", L"[BATCH] Flash Convert from JSON"},
			{L"method:popcap.animation.convert.flash.to.batch", L"[BATCH] Flash Convert to JSON"},
			{L"method:popcap.animation.convert.flash.resize.batch", L"[BATCH] Flash Resize image"},
			{L"method:popcap.animation.convert.flash.resize.batch:1536", L"[BATCH] Flash Resize image ~ 1536"},
			{L"method:popcap.animation.convert.flash.resize.batch:768", L"[BATCH] Flash Resize image ~ 768"},
			{L"method:popcap.animation.convert.flash.link_media.batch", L"[BATCH] Flash Create links for image files"},
			{L"method:popcap.re_animation", L"PopCap Re-Animation"},
			{L"method:popcap.re_animation.encode:desktop_32", L"Encode ~ desktop_32"},
			{L"method:popcap.re_animation.decode:desktop_32", L"Decode ~ desktop_32"},
			{L"method:popcap.re_animation.encode:mobile_32", L"Encode ~ mobile_32"},
			{L"method:popcap.re_animation.decode:mobile_32", L"Decode ~ mobile_32"},
			{L"method:popcap.re_animation.encode:mobile_64", L"Encode ~ mobile_64"},
			{L"method:popcap.re_animation.decode:mobile_64", L"Decode ~ mobile_64"},
			{L"method:popcap.re_animation.encode:television_32", L"Encode ~ television_32"},
			{L"method:popcap.re_animation.decode:television_32", L"Decode ~ television_32"},
			{L"method:popcap.re_animation.encode.batch:desktop_32", L"[BATCH] Encode ~ desktop_32"},
			{L"method:popcap.re_animation.decode.batch:desktop_32", L"[BATCH] Decode ~ desktop_32"},
			{L"method:popcap.re_animation.encode.batch:mobile_32", L"[BATCH] Encode ~ mobile_32"},
			{L"method:popcap.re_animation.decode.batch:mobile_32", L"[BATCH] Decode ~ mobile_32"},
			{L"method:popcap.re_animation.encode.batch:mobile_64", L"[BATCH] Encode ~ mobile_64"},
			{L"method:popcap.re_animation.decode.batch:mobile_64", L"[BATCH] Decode ~ mobile_64"},
			{L"method:popcap.re_animation.encode.batch:television_32", L"[BATCH] Encode ~ television_32"},
			{L"method:popcap.re_animation.decode.batch:television_32", L"[BATCH] Decode ~ television_32"},
			{L"method:popcap.particle", L"PopCap Particle"},
			{L"method:popcap.particle.encode:desktop_32", L"Encode ~ desktop_32"},
			{L"method:popcap.particle.decode:desktop_32", L"Decode ~ desktop_32"},
			{L"method:popcap.particle.encode:mobile_32", L"Encode ~ mobile_32"},
			{L"method:popcap.particle.decode:mobile_32", L"Decode ~ mobile_32"},
			{L"method:popcap.particle.encode:mobile_64", L"Encode ~ mobile_64"},
			{L"method:popcap.particle.decode:mobile_64", L"Decode ~ mobile_64"},
			{L"method:popcap.particle.encode:television_32", L"Encode ~ television_32"},
			{L"method:popcap.particle.decode:television_32", L"Decode ~ television_32"},
			{L"method:popcap.particle.encode.batch:desktop_32", L"[BATCH] Encode ~ desktop_32"},
			{L"method:popcap.particle.decode.batch:desktop_32", L"[BATCH] Decode ~ desktop_32"},
			{L"method:popcap.particle.encode.batch:mobile_32", L"[BATCH] Encode ~ mobile_32"},
			{L"method:popcap.particle.decode.batch:mobile_32", L"[BATCH] Decode ~ mobile_32"},
			{L"method:popcap.particle.encode.batch:mobile_64", L"[BATCH] Encode ~ mobile_64"},
			{L"method:popcap.particle.decode.batch:mobile_64", L"[BATCH] Decode ~ mobile_64"},
			{L"method:popcap.particle.encode.batch:television_32", L"[BATCH] Encode ~ television_32"},
			{L"method:popcap.particle.decode.batch:television_32", L"[BATCH] Decode ~ television_32"},
			{L"method:popcap.trail", L"PopCap Trail"},
			{L"method:popcap.trail.encode:desktop_32", L"Encode ~ desktop_32"},
			{L"method:popcap.trail.decode:desktop_32", L"Decode ~ desktop_32"},
			{L"method:popcap.trail.encode:mobile_32", L"Encode ~ mobile_32"},
			{L"method:popcap.trail.decode:mobile_32", L"Decode ~ mobile_32"},
			{L"method:popcap.trail.encode:mobile_64", L"Encode ~ mobile_64"},
			{L"method:popcap.trail.decode:mobile_64", L"Decode ~ mobile_64"},
			{L"method:popcap.trail.encode:television_32", L"Encode ~ television_32"},
			{L"method:popcap.trail.decode:television_32", L"Decode ~ television_32"},
			{L"method:popcap.trail.encode.batch:desktop_32", L"[BATCH] Encode ~ desktop_32"},
			{L"method:popcap.trail.decode.batch:desktop_32", L"[BATCH] Decode ~ desktop_32"},
			{L"method:popcap.trail.encode.batch:mobile_32", L"[BATCH] Encode ~ mobile_32"},
			{L"method:popcap.trail.decode.batch:mobile_32", L"[BATCH] Decode ~ mobile_32"},
			{L"method:popcap.trail.encode.batch:mobile_64", L"[BATCH] Encode ~ mobile_64"},
			{L"method:popcap.trail.decode.batch:mobile_64", L"[BATCH] Decode ~ mobile_64"},
			{L"method:popcap.trail.encode.batch:television_32", L"[BATCH] Encode ~ television_32"},
			{L"method:popcap.trail.decode.batch:television_32", L"[BATCH] Decode ~ television_32"},
			{L"method:popcap.effect", L"PopCap Effect"},
			{L"method:popcap.effect.encode", L"Encode"},
			{L"method:popcap.effect.decode", L"Decode"},
			{L"method:popcap.effect.encode.batch", L"[BATCH] Encode"},
			{L"method:popcap.effect.decode.batch", L"[BATCH] Decode"},
			{L"method:popcap.character_font_widget_2", L"PopCap Character-Font-Widget-2"},
			{L"method:popcap.character_font_widget_2.encode", L"Encode"},
			{L"method:popcap.character_font_widget_2.decode", L"Decode"},
			{L"method:popcap.character_font_widget_2.encode.batch", L"[BATCH] Encode"},
			{L"method:popcap.character_font_widget_2.decode.batch", L"[BATCH] Decode"},
			{L"method:popcap.package", L"PopCap Package"},
			{L"method:popcap.package.pack", L"Pack"},
			{L"method:popcap.package.unpack", L"Unpack"},
			{L"method:popcap.package.pack_automatic", L"Pack by automatic"},
			{L"method:popcap.package.encrypt", L"Encrypt"},
			{L"method:popcap.resource_stream_group", L"PopCap Resource-Stream-Group"},
			{L"method:popcap.resource_stream_group.pack", L"Pack"},
			{L"method:popcap.resource_stream_group.unpack", L"Unpack"},
			{L"method:popcap.resource_stream_bundle", L"PopCap Resource-Stream-Bundle"},
			{L"method:popcap.resource_stream_bundle.pack", L"Pack"},
			{L"method:popcap.resource_stream_bundle.unpack", L"Unpack"},
			{L"method:popcap.resource_stream_bundle.resource_convert", L"Resource convert"},
			{L"method:popcap.resource_stream_bundle.repair", L"Repair"},
			{L"method:popcap.resource_stream_bundle_patch", L"PopCap Resource-Stream-Bundle-Patch"},
			{L"method:popcap.resource_stream_bundle_patch.encode", L"Encode"},
			{L"method:popcap.resource_stream_bundle_patch.decode", L"Decode"},
			{L"method:pvz2.text_table", L"PvZ-2 Text-Table"},
			{L"method:pvz2.text_table.convert:text", L"Convert ~ text"},
			{L"method:pvz2.text_table.convert:json_map", L"Convert ~ json_map"},
			{L"method:pvz2.text_table.convert:json_list", L"Convert ~ json_list"},
			{L"method:pvz2.remote_android_helper", L"PvZ-2 Remote-Android-Helper"},
			{L"method:pvz2.remote_android_helper.launch:pull_main_package", L"Pull main package"},
			{L"method:pvz2.remote_android_helper.launch:push_main_package", L"Push main package"},
			{L"method:pvz2.remote_android_helper.launch:pull_local_profile", L"Pull local profile"},
			{L"method:pvz2.remote_android_helper.launch:push_local_profile", L"Push local profile"},
			{L"method:pvz2.remote_android_helper.launch:pull_player_profile", L"Pull player profile"},
			{L"method:pvz2.remote_android_helper.launch:push_player_profile", L"Push player profile"},
			{L"method:pvz2.remote_android_helper.launch:pull_content_delivery", L"Pull content delivery"},
			{L"method:pvz2.remote_android_helper.launch:push_content_delivery", L"Push content delivery"},
			{L"method:pvz2.remote_android_helper.launch:clear_snapshot", L"Clear snapshot"},
		}},
	};

	#pragma endregion

	#pragma region utility

	inline auto query (
		std::wstring const & key
	) -> std::wstring const& {
		auto language = get_register_value_string(k_register_key_parent, k_register_key_path, L"language").value_or(L"Chinese");
		return g_table.at(language).at(key);
	}

	#pragma endregion

}