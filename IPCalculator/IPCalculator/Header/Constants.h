#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <bitset>

const size_t OCTET_SIZE = 8;
const size_t AMOUNT_OF_DOTS = 3;
const size_t AMOUNT_OF_OCTETS = 4;
const size_t ADDITIONAL_REQUIRED_HOSTS = 2;

const size_t OCTET_MASK_BITSHIFTS[4] = { 24, 16, 8, 0 };
const std::bitset<32> OCTET_MASKS[4] = {
		static_cast<std::uint32_t>(255 << OCTET_MASK_BITSHIFTS[0]),
		static_cast<std::uint32_t>(255 << OCTET_MASK_BITSHIFTS[1]),
		static_cast<std::uint32_t>(255 << OCTET_MASK_BITSHIFTS[2]),
		static_cast<std::uint32_t>(255 << OCTET_MASK_BITSHIFTS[3])
};


#endif // CONSTANTS_H
