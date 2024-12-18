module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.image_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.container.array.array;
import twinning.kernel.utility.image.size;
import twinning.kernel.utility.image.color;
import twinning.kernel.utility.image.pixel;
import twinning.kernel.utility.range.algorithm;

export namespace Twinning::Kernel::Image {

	#pragma region type

	template <typename TPixel, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TPixel>>
		&& (IsSameV<t_constant, Boolean>)
	class BasicImageView {

	private:

		using CView = BasicImageView<TPixel, k_true>;

	public:

		using Pixel = TPixel;

		inline static constexpr auto constant = Boolean{t_constant};

		using QPixel = AsConstantIf<Pixel, constant.value>;

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
			(!constant.value) {
			return self_cast<CView>(thiz);
		}

		implicit operator CView const & () const requires
			(!constant.value) {
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
			(!constant.value) {
			for (auto & row : thiz.data()) {
				Range::assign(row, pixel);
			}
			return;
		}

		auto draw (
			CView const & image
		) const -> Void requires
			(!constant.value) {
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
	using VBasicImageView = BasicImageView<Pixel, k_false>;

	template <typename Pixel> requires
		AutoConstraint
	using CBasicImageView = BasicImageView<Pixel, k_true>;

	#pragma endregion

}
