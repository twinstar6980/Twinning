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
		&& (IsSameOf<t_constant, Boolean>)
	class BasicImageView {

	private:

		using ConstantView = BasicImageView<TPixel, k_true>;

	public:

		using Pixel = TPixel;

		inline static constexpr auto constant = Boolean{t_constant};

		using QualifyPixel = AsConstantIf<Pixel, constant.value>;

		using QualifyPixelRow = ListView<Pixel, constant>;

	protected:

		Array<QualifyPixelRow> m_data;

	public:

		#pragma region constructor

		~BasicImageView (
		) = default;

		// ----------------

		BasicImageView (
		) :
			m_data{} {
			return;
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
		) const -> QualifyPixelRow const & {
			assert_test(y < thiz.size().height);
			return thiz.m_data[y];
		}

		auto operator [] (
			ImagePosition const & position
		) const -> QualifyPixel & {
			assert_test(position.y < thiz.size().height);
			assert_test(position.x < thiz.size().width);
			return thiz.m_data[position.y][position.x];
		}

		// ----------------

		implicit operator ConstantView & () requires
			(!constant.value) {
			return self_cast<ConstantView>(thiz);
		}

		implicit operator ConstantView const & () const requires
			(!constant.value) {
			return self_cast<ConstantView>(thiz);
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
		) const -> ConstantListView<QualifyPixelRow> const & {
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
			ConstantView const & image
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
		AutomaticConstraint
	using VariableBasicImageView = BasicImageView<Pixel, k_false>;

	template <typename Pixel> requires
		AutomaticConstraint
	using ConstantBasicImageView = BasicImageView<Pixel, k_true>;

	#pragma endregion

}
