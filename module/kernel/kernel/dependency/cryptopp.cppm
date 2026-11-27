module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/cryptopp/origin/cryptlib.h"
#include "dependency/cryptopp/origin/iterhash.h"
#include "dependency/cryptopp/origin/argnames.h"
#include "dependency/cryptopp/origin/filters.h"
#include "dependency/cryptopp/origin/crc.h"
#include "dependency/cryptopp/origin/adler32.h"
#include "dependency/cryptopp/origin/md2.h"
#include "dependency/cryptopp/origin/md4.h"
#include "dependency/cryptopp/origin/md5.h"
#include "dependency/cryptopp/origin/sha.h"
#include "dependency/cryptopp/origin/sha3.h"
#include "dependency/cryptopp/origin/basecode.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.cryptopp;

export namespace Twinning::Kernel::Dependency::cryptopp {

	using ::CryptoPP::byte;

	using ::CryptoPP::AlgorithmParameters;

	using ::CryptoPP::ConstByteArrayParameter;

	namespace Name {

		using ::CryptoPP::Name::Pad;

		using ::CryptoPP::Name::PaddingByte;

		using ::CryptoPP::Name::Log2Base;

		using ::CryptoPP::Name::EncodingLookupArray;

		using ::CryptoPP::Name::DecodingLookupArray;

	}

	using ::CryptoPP::BufferedTransformation;

	using ::CryptoPP::Redirector;

	using ::CryptoPP::ArraySource;

	using ::CryptoPP::ArraySink;

	using ::CryptoPP::HashFilter;

	using ::CryptoPP::HashTransformation;

	using ::CryptoPP::CRC32;

	using ::CryptoPP::CRC32C;

	using ::CryptoPP::Adler32;

	using ::CryptoPP::Weak::MD2;

	using ::CryptoPP::Weak::MD4;

	using ::CryptoPP::Weak::MD5;

	using ::CryptoPP::SHA1;

	using ::CryptoPP::SHA224;

	using ::CryptoPP::SHA256;

	using ::CryptoPP::SHA384;

	using ::CryptoPP::SHA512;

	using ::CryptoPP::SHA3_224;

	using ::CryptoPP::SHA3_256;

	using ::CryptoPP::SHA3_384;

	using ::CryptoPP::SHA3_512;

	using ::CryptoPP::BaseN_Encoder;

	using ::CryptoPP::BaseN_Decoder;

}
