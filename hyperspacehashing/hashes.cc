// Copyright (c) 2011, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// POSIX
#include <endian.h>

// STL
#include <algorithm>

// Google CityHash
#include <city.h>

// HyperspaceHashing
#include "hashes_internal.h"
#include "hyperspacehashing/hashes.h"

uint64_t
hyperspacehashing :: cityhash(const e::buffer& buf)
{
    return CityHash64(static_cast<const char*>(buf.get()), buf.size());
}

uint64_t
hyperspacehashing :: lendian(const e::buffer& buf)
{
    uint64_t ret = 0;
    memmove(&ret, buf.get(), std::min(buf.size(), sizeof(ret)));
    return le64toh(ret);
}

void
hyperspacehashing :: convert(const std::vector<hash_t>& in, std::vector<hash_func>* out)
{
    out->resize(in.size());

    for (size_t i = 0; i < in.size(); ++i)
    {
        switch (in[i])
        {
            case EQUALITY:
                (*out)[i] = cityhash;
                break;
            case RANGE:
                (*out)[i] = lendian;
                break;
            default:
                assert(false);
        }
    }
}

bool
hyperspacehashing :: is_range_func(hash_func hf)
{
    if (hf == cityhash)
    {
        return false;
    }
    else if (hf == lendian)
    {
        return true;
    }
    else
    {
        assert(false);
    }
}
