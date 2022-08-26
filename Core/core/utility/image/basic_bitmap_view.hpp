#pragma once

#include "core/utility/image/size.hpp"
#include "core/utility/image/channel.hpp"
#include "core/utility/image/pixel.hpp"

namespace TwinKleS::Core::Image {

	#pragma region type

	template <typename TPixel, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TPixel>>
		&& (IsSameV<t_constant, ZBoolean>)
	class BasicBitmapView {

	private: //

		using CView = BasicBitmapView<TPixel, true>;

	public: //

		using Pixel = TPixel;

		inline static constexpr auto constant = ZBoolean{t_constant};

		using QPixel = AsConstantIf<Pixel, constant>;

		using QPixelRow = ListView<Pixel, constant>;

	protected: //

		Array<QPixelRow> m_data{};

	public: //

		#pragma region structor

		~BasicBitmapView (
		) = default;

		// ----------------

		BasicBitmapView (
		) = default;

		BasicBitmapView (
			BasicBitmapView const & that
		) = default;

		BasicBitmapView (
			BasicBitmapView && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator = (
			BasicBitmapView const & that
		) -> BasicBitmapView& = default;

		auto operator = (
			BasicBitmapView && that
		) -> BasicBitmapView& = default;

		// ----------------

		auto operator [] (
			Size const & y
		) const -> QPixelRow const& {
			assert_condition(y < thiz.size().height);
			return thiz.m_data[y];
		}

		auto operator [] (
			ImagePosition const & position
		) const -> QPixel& {
			assert_condition(position.y < thiz.size().height);
			assert_condition(position.x < thiz.size().width);
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
			BasicBitmapView const & view
		) -> Void {
			thiz.m_data = view.m_data;
			return;
		}

		#pragma endregion

		#pragma region data & size

		auto data (
		) const -> CListView<QPixelRow> const& {
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
			assert_condition(thiz.size() == image.size());
			Range::convert_from(
				thiz.data(),
				image.data(),
				[] (auto & dest_row, auto & source_row) -> auto {
					Range::assign_from(dest_row, source_row);
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
		) -> BasicBitmapView {
			assert_condition(position.y + size.height <= thiz.size().height);
			assert_condition(position.x + size.width <= thiz.size().width);
			auto sub_view = BasicBitmapView{};
			sub_view.m_data.convert(
				thiz.m_data.sub(position.y, size.height),
				[&] (auto & dest_row, auto & source_row) -> auto {
					dest_row = source_row.sub(position.x, size.width);
					return;
				}
			);
			return sub_view;
		}

		#pragma endregion

	};

	#pragma endregion

}
