#pragma once

#include "version.hpp"
#include "core/utility/utility.hpp"
#include "core/tool/popcap/reanim/version.hpp"

namespace TwinKleS::Core::Tool::PopCap::Reanim::Manifest {

	template <auto t_version>
	struct Transform;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {Version::Platform::desktop}, {}))
	struct Transform<t_version> {
		Floating x;
		Floating y;
		Floating kx;
		Floating ky;
		Floating sx;
		Floating sy;
		Floating f;
		Floating a;
		String   i;
		String   font;
		String   text;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {Version::Platform::phone}, {}))
	struct Transform<t_version> {
		Floating x;
		Floating y;
		Floating kx;
		Floating ky;
		Floating sx;
		Floating sy;
		Floating f;
		Floating a;
		Integer  i;
		String   font;
		String   text;
	};

	// ----------------

	template <auto t_version>
	struct Track;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {Version::Platform::desktop, Version::Platform::phone}, {}))
	struct Track<t_version> {
		String                     name;
		List<Transform<t_version>> t;
	};

	// ----------------

	template <auto t_version>
	struct Animation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {Version::Platform::desktop, Version::Platform::phone}, {}))
	struct Animation<t_version> {
		Floating               fps;
		List<Track<t_version>> track;
	};

	// ----------------

	using AnimationVariant = VariantOfVersion<VersionEnum, Animation>;

}

namespace TwinKleS::Core::JSON {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::Reanim::check_version(t_version, {Tool::PopCap::Reanim::Version::Platform::desktop}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::Reanim::Manifest::Transform<t_version>),
		M_wrap(x, y, kx, ky, sx, sy, f, a, i, font, text),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::Reanim::check_version(t_version, {Tool::PopCap::Reanim::Version::Platform::phone}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::Reanim::Manifest::Transform<t_version>),
		M_wrap(x, y, kx, ky, sx, sy, f, a, i, font, text),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::Reanim::check_version(t_version, {Tool::PopCap::Reanim::Version::Platform::desktop, Tool::PopCap::Reanim::Version::Platform::phone}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::Reanim::Manifest::Track<t_version>),
		M_wrap(name, t),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::Reanim::check_version(t_version, {Tool::PopCap::Reanim::Version::Platform::desktop, Tool::PopCap::Reanim::Version::Platform::phone}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::Reanim::Manifest::Animation<t_version>),
		M_wrap(fps, track),
	);

}
