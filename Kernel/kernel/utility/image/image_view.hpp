#pragma once

#include "kernel/utility/image/size.hpp"
#include "kernel/utility/image/color.hpp"
#include "kernel/utility/image/pixel.hpp"

namespace Twinning::Kernel::Image {

	#pragma region type

	template <typename TPixel, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TPixel>>
		&& (IsSameV<t_constant, ZBoolean>)
	class BasicImageView {

	private:

		using CView = BasicImageView<TPixel, true>;

	public:

		using Pixel = TPixel;

		inline static constexpr auto constant = ZBoolean{t_constant};

		using QPixel = AsConstantIf<Pixel, constant>;

		using QPixelRow = ListView<Pixel, constant>;

	protected:

		Array<QPixelRow> m_data;

	public:

		#pragma region structor

		~BasicImageView (
		) = default;

		// ----------------

		BasicImageView (
		) :
			m_data{} {
		}

		BasicImageView (
			BasicImageView const & that
		) = default;

		BasicImageView (
			BasicImageView && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator = (
			BasicImageView const & that
		) -> BasicImageView & = default;

		auto operator = (
			BasicImageView && that
		) -> BasicImageView & = default;

		// ----------------

		auto operator [] (
			Size const & y
		) const -> QPixelRow const & {
			assert_test(y < thiz.size().height);
			return thiz.m_data[y];
		}

		auto operator [] (
			ImagePosition const & position
		) const -> QPixel & {
			assert_test(position.y < thiz.size().height);
			assert_test(position.x < thiz.size().width);
			return thiz.m_data[position.y][position.x];
		}

		// ----------------

		implicit operator CView & () requires
			(!constant) {
			return self_cast<CView>(thiz);
		}

		implicit operator CView const & () const requires
			(!constant) {
			return self_cast<CView>(thiz);
		}

		#pragma endregion

		#pragma region set

		auto reset_view (
		) -> Void {
			thiz.m_data.reset();
			return;
		}

		auto set_view (
			BasicImageView const & view
		) -> Void {
			thiz.m_data = view.m_data;
			return;
		}

		#pragma endregion

		#pragma region data & size

		auto data (
		) const -> CListView<QPixelRow> const & {
			return thiz.m_data;
		}

		auto size (
		) const -> ImageSize {
			return thiz.m_data.empty() ? (k_none_image_size) : (ImageSize{thiz.m_data.first().size(), thiz.m_data.size()});
		}

		#pragma endregion

		#pragma region draw

		auto fill (
			Pixel const & pixel
		) const -> Void requires
			(!constant) {
			for (auto & row : thiz.data()) {
				Range::assign(row, pixel);
			}
			return;
		}

		auto draw (
			CView const & image
		) const -> Void requires
			(!constant) {
			assert_test(thiz.size() == image.size());
			Range::convert_from(
				thiz.data(),
				image.data(),
				[] (auto & destination_row, auto & source_row) -> auto {
					Range::assign_from(destination_row, source_row);
					return;
				}
			);
			return;
		}

		#pragma endregion

		#pragma region sub

		auto sub (
			ImagePosition const & position,
			ImageSize const &     size
		) -> BasicImageView {
			assert_test(position.y + size.height <= thiz.size().height);
			assert_test(position.x + size.width <= thiz.size().width);
			auto sub_view = BasicImageView{};
			sub_view.m_data.convert(
				thiz.m_data.sub(position.y, size.height),
				[&] (auto & destination_row, auto & source_row) -> auto {
					destination_row = source_row.sub(position.x, size.width);
					return;
				}
			);
			return sub_view;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename Pixel> requires
		AutoConstraint
	using VBasicImageView = BasicImageView<Pixel, false>;

	template <typename Pixel> requires
		AutoConstraint
	using CBasicImageView = BasicImageView<Pixel, true>;

	#pragma endregion

}
