#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/crypt_data/version.hpp"

namespace TwinStar::Core::Tool::PopCap::CryptData {

	template <auto version> requires (check_version(version))
	struct EncryptCommon {

	protected:

		using MagicIdentifier = StaticByteArray<11_sz>;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{{0x43_b, 0x52_b, 0x59_b, 0x50_b, 0x54_b, 0x5F_b, 0x52_b, 0x45_b, 0x53_b, 0x0A_b, 0x00_b}};

	};

	template <auto version> requires (check_version(version))
	struct Encrypt :
		EncryptCommon<version> {

	protected:

		using Common = EncryptCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			OByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.write_constant(value);
			return;
		}

		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerU64>(value));
			return;
		}

		// ----------------

		static auto compute_size (
			Size const & plain_size,
			Size &       cipher_size,
			Size const & limit
		) -> Void {
			cipher_size = k_none_size;
			if (plain_size >= limit) {
				cipher_size += bs_static_size<MagicIdentifier>();
				cipher_size += bs_static_size<IntegerU64>();
			}
			cipher_size += plain_size;
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView & plain,
			OByteStreamView & cipher,
			Size const &      limit,
			String const &    key
		) -> Void {
			if (plain.reserve() >= limit) {
				cipher.write_constant(k_magic_identifier);
				exchange_unit_integer(cipher, cbw<Integer>(plain.reserve()));
				Data::Encryption::XOR::Encrypt::do_process_whole(as_lvalue(IByteStreamView{plain.forward_view(limit)}), cipher, to_byte_view(key.as_view()));
			}
			cipher.write(plain.forward_view(plain.reserve()));
			return;
		}

	public:

		static auto do_compute_size (
			Size const & plain_size,
			Size &       cipher_size,
			Size const & limit
		) -> Void {
			restruct(cipher_size);
			return compute_size(plain_size, cipher_size, limit);
		}

		static auto do_process_whole (
			IByteStreamView & plain_,
			OByteStreamView & cipher_,
			Size const &      limit,
			String const &    key
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, limit, key);
		}

	};

	template <auto version> requires (check_version(version))
	struct Decrypt :
		EncryptCommon<version> {

	protected:

		using Common = EncryptCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			IByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.read_constant(value);
			return;
		}

		static auto exchange_unit_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerU64>());
			return;
		}

		// ----------------

		static auto compute_size (
			CByteListView const & cipher,
			Size &                plain_size,
			Size const &          limit
		) -> Void {
			plain_size = k_none_size;
			if (cipher.size() >= limit + bs_static_size<MagicIdentifier>() + bs_static_size<IntegerU64>()) {
				auto cipher_stream = IByteStreamView{cipher};
				cipher_stream.read_constant(k_magic_identifier);
				auto plain_data_size = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(cipher_stream, it); })));
				assert_test(plain_data_size >= limit);
				plain_size += plain_data_size;
			} else {
				plain_size += cipher.size();
			}
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView & cipher,
			OByteStreamView & plain,
			Size const &      limit,
			String const &    key
		) -> Void {
			if (cipher.reserve() >= limit + bs_static_size<MagicIdentifier>() + bs_static_size<IntegerU64>()) {
				cipher.read_constant(k_magic_identifier);
				auto plain_data_size = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(cipher, it); })));
				assert_test(plain_data_size >= limit);
				Data::Encryption::XOR::Encrypt::do_process_whole(as_lvalue(IByteStreamView{cipher.forward_view(limit)}), plain, to_byte_view(key.as_view()));
				plain.write(cipher.forward_view(plain_data_size - limit));
			} else {
				plain.write(cipher.forward_view(cipher.reserve()));
			}
			return;
		}

	public:

		static auto do_compute_size (
			CByteListView const & cipher,
			Size &                plain_size,
			Size const &          limit
		) -> Void {
			restruct(plain_size);
			return compute_size(cipher, plain_size, limit);
		}

		static auto do_process_whole (
			IByteStreamView & cipher_,
			OByteStreamView & plain_,
			Size const &      limit,
			String const &    key
		) -> Void {
			M_use_zps_of(cipher);
			M_use_zps_of(plain);
			return process_whole(cipher, plain, limit, key);
		}

	};

}
