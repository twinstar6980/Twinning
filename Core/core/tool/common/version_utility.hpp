#pragma once

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Tool {

	#pragma region variant

	template <typename It>
	using NoWrapper = It;

	template <typename VersionEnum, template <auto> typename Value, template <typename> typename Wrapper = NoWrapper>
	using VariantOfVersion = decltype([] {
		return [&] <auto ...version> (
			ValuePackage<version...>
		) -> Variant<Wrapper<Value<version>>...> {
			}(VersionEnum{});
	}());

	#pragma endregion

	#pragma region json

	template <typename VersionEnum, typename Version, typename Variant>
	inline auto json_from_variant_of_version (
		Version const & version,
		JSON::Value &   json,
		Variant const & variant
	) -> Void {
		Generalization::execute<VersionEnum>(
			version,
			[&] <auto index, auto version> (ValuePackage<index, version>) {
				json.from(variant, mbw<Size>(index));
			}
		);
		return;
	}

	template <typename VersionEnum, typename Version, typename Variant>
	inline auto json_to_variant_of_version (
		Version const &     version,
		JSON::Value const & json,
		Variant &           variant
	) -> Void {
		Generalization::execute<VersionEnum>(
			version,
			[&] <auto index, auto version> (ValuePackage<index, version>) {
				json.to(variant, mbw<Size>(index));
			}
		);
		return;
	}

	#pragma endregion

}
