/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "HwRenderPrimitiveFactory.h"

#include <stdlib.h>

namespace filament {

using namespace utils;
using namespace backend;

bool operator<(HwRenderPrimitiveFactory::Entry const& lhs,
        HwRenderPrimitiveFactory::Entry const& rhs) noexcept {
    if (lhs.vbh == rhs.vbh) {
        if (lhs.ibh == rhs.ibh) {
            if (lhs.offset == rhs.offset) {
                if (lhs.count == rhs.count) {
                    return lhs.type < rhs.type;
                } else {
                    return lhs.count < rhs.count;
                }
            } else {
                return lhs.offset < rhs.offset;
            }
        } else {
            return lhs.ibh < rhs.ibh;
        }
    } else {
        return lhs.vbh < rhs.vbh;
    }
}

// ------------------------------------------------------------------------------------------------

HwRenderPrimitiveFactory::HwRenderPrimitiveFactory() = default;

HwRenderPrimitiveFactory::~HwRenderPrimitiveFactory() noexcept = default;

void HwRenderPrimitiveFactory::terminate(DriverApi& driver) noexcept {
    assert_invariant(mMap.empty());
    assert_invariant(mSet.empty());
}

RenderPrimitiveHandle HwRenderPrimitiveFactory::create(DriverApi& driver,
        VertexBufferHandle vbh, IndexBufferHandle ibh,
        PrimitiveType type, uint32_t offset, uint32_t minIndex, uint32_t maxIndex,
        uint32_t count) noexcept {

    Entry key = { vbh, ibh, offset, count, type, 1, {} };

    // see if we already have seen this RenderPrimitive
    auto pos = mSet.find(key);

    // the common case is that we've never seen it (i.e.: no reuse)
    if (UTILS_LIKELY(pos == mSet.end())) {
        // create the backend object
        auto handle = driver.createRenderPrimitive(vbh, ibh,
                type, offset, minIndex, maxIndex, count);
        // insert key/handle in our set with a refcount of 1
        // IMPORTANT: std::set<> doesn't invalidate iterators in insert/erase
        key.handle = handle;
        auto [ipos, _] = mSet.insert(key);
        // map the handle back to the key/payload
        mMap.insert({ handle.getId(), ipos });
        return handle;
    }
    pos->refs++;
    return pos->handle;
}

void HwRenderPrimitiveFactory::destroy(DriverApi& driver, RenderPrimitiveHandle rph) noexcept {
    // look for this handle in our map
    auto pos = mMap.find(rph.getId());

    // it must be there
    assert_invariant(pos != mMap.end());

    // check the refcount and destroy if needed
    auto ipos = pos->second;
    ipos->refs = ipos->refs - 1;
    if (ipos->refs == 0) {
        mSet.erase(ipos);
        mMap.erase(pos);
        driver.destroyRenderPrimitive(rph);
    }
}

} // namespace filament
