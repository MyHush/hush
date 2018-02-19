/** @file
 *****************************************************************************
 Implementation of misc math and serialization utility functions
 *****************************************************************************
 * @author     This file is part of libsnark, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdarg>
#include "common/utils.hpp"

namespace libsnark {

unsigned long long log2(unsigned long long n)
/* returns ceil(log2(n)), so 1ul<<log2(n) is the smallest power of 2,
   that is not less than n. */
{
    unsigned long long r = ((n & (n-1)) == 0 ? 0 : 1); // add 1 if n is not power of 2

    while (n > 1)
    {
        n >>= 1;
        r++;
    }

    return r;
}

unsigned long long bitreverse(unsigned long long n, const unsigned long long l)
{
    unsigned long long r = 0;
    for (unsigned long long k = 0; k < l; ++k)
    {
        r = (r << 1) | (n & 1);
        n >>= 1;
    }
    return r;
}

bit_vector int_list_to_bits(const std::initializer_list<uint64_t> &l, const size_t wordsize)
{
    bit_vector res(wordsize*l.size());
    for (uint64_t i = 0; i < l.size(); ++i)
    {
        for (uint64_t j = 0; j < wordsize; ++j)
        {
            res[i*wordsize + j] = (*(l.begin()+i) & (UINT64_C(1)<<(wordsize-1-j)));
        }
    }
    return res;
}

int64_t div_ceil(int64_t x, int64_t y)
{
    return (x + (y-1)) / y;
}

bool is_little_endian()
{
    uint64_t a = 0x12345678;
    unsigned char *c = (unsigned char*)(&a);
    return (*c = 0x78);
}

std::string FORMAT(const std::string &prefix, const char* format, ...)
{
    const static unsigned long long MAX_FMT = 256;
    char buf[MAX_FMT];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, MAX_FMT, format, args);
    va_end(args);

    return prefix + std::string(buf);
}

void serialize_bit_vector(std::ostream &out, const bit_vector &v)
{
    out << v.size() << "\n";
    for (unsigned long long i = 0; i < v.size(); ++i)
    {
        out << v[i] << "\n";
    }
}

void deserialize_bit_vector(std::istream &in, bit_vector &v)
{
    unsigned long long size;
    in >> size;
    v.resize(size);
    for (unsigned long long i = 0; i < size; ++i)
    {
        bool b;
        in >> b;
        v[i] = b;
    }
}
} // libsnark
