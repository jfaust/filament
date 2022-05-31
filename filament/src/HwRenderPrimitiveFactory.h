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

#ifndef TNT_FILAMENT_HWRENDERPRIMITIVEFACTORY_H
#define TNT_FILAMENT_HWRENDERPRIMITIVEFACTORY_H

#include <backend/DriverEnums.h>
#include <backend/Handle.h>

#include <private/backend/DriverApi.h>

#include <stdint.h>

namespace filament {

class FEngine;

class HwRenderPrimitiveFactory {
public:

    HwRenderPrimitiveFactory();
    ~HwRenderPrimitiveFactory() noexcept;

    HwRenderPrimitiveFactory(HwRenderPrimitiveFactory const& rhs) = delete;
    HwRenderPrimitiveFactory(HwRenderPrimitiveFactory&& rhs) noexcept = delete;
    HwRenderPrimitiveFactory& operator=(HwRenderPrimitiveFactory const& rhs) = delete;
    HwRenderPrimitiveFactory& operator=(HwRenderPrimitiveFactory&& rhs) noexcept = delete;

    void terminate(backend::DriverApi& driver) noexcept;

    backend::RenderPrimitiveHandle create(backend::DriverApi& driver,
            backend::VertexBufferHandle vbh,
            backend::IndexBufferHandle ibh,
            backend::PrimitiveType type,
            uint32_t offset,
            uint32_t minIndex,
            uint32_t maxIndex,
            uint32_t count) noexcept;

    void destroy(backend::DriverApi& driver,
            backend::RenderPrimitiveHandle rph) noexcept;

private:

};

} // namespace filament

#endif // TNT_FILAMENT_HWRENDERPRIMITIVEFACTORY_H
