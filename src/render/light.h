//
// Created by Mike Smith on 2020/2/5.
//

#pragma once

#include <compute/data_types.h>

namespace luisa::light {

struct Selection {
    uint data_index;
    uint interaction_index;
};

}

#ifndef LUISA_DEVICE_COMPATIBLE

#include "ray.h"
#include "core/plugin.h"
#include "core/parser.h"
#include <compute/device.h>
#include "interaction.h"

namespace luisa {

class LightSampleBufferSet {

private:
    std::unique_ptr<Buffer<float4>> _radiance_and_pdf_w_buffer;
    std::unique_ptr<Buffer<Ray>> _shadow_ray_buffer;

public:
    LightSampleBufferSet(Device *device, size_t capacity)
        : _radiance_and_pdf_w_buffer{device->allocate_buffer<float4>(capacity, BufferStorage::DEVICE_PRIVATE)},
          _shadow_ray_buffer{device->allocate_buffer<Ray>(capacity, BufferStorage::DEVICE_PRIVATE)} {}
    
    [[nodiscard]] auto radiance_and_pdf_w_buffer() { return _radiance_and_pdf_w_buffer->view(); }
    [[nodiscard]] auto shadow_ray_buffer() { return _shadow_ray_buffer->view(); }
};

class Light : public Plugin {

public:
    static constexpr auto MAX_LIGHT_TAG_COUNT = 16u;
    
    using SampleLightsDispatch = std::function<void(
        KernelDispatcher &dispatch, Kernel &kernel, uint dispatch_extent, BufferView<float> sample_buffer,
        TypelessBuffer &light_data_buffer, BufferView<light::Selection> queue, BufferView<uint> queue_size,
        BufferView<float> cdf_buffer,
        InteractionBufferSet &interactions, Geometry *geometry, LightSampleBufferSet &light_samples)>;
    
    using EvaluateLightsDispatch = std::function<void(
        KernelDispatcher &dispatch, Kernel &kernel, uint dispatch_extent,
        TypelessBuffer &light_data_buffer, BufferView<light::Selection> queue, BufferView<uint> queue_size,
        InteractionBufferSet &interactions)>;

protected:
    static uint _assign_tag() {
        static auto next_tag = 0u;
        LUISA_EXCEPTION_IF(next_tag == MAX_LIGHT_TAG_COUNT, "Too many light tags assigned, limit: ", MAX_LIGHT_TAG_COUNT);
        LUISA_INFO("Assigning light tag: ", next_tag);
        return next_tag++;
    }

public:
    Light(Device *device, const ParameterSet &parameter_set[[maybe_unused]]) : Plugin{device} {}
    [[nodiscard]] virtual uint tag() const noexcept = 0;
    [[nodiscard]] virtual std::unique_ptr<Kernel> create_generate_samples_kernel() = 0;
    [[nodiscard]] virtual SampleLightsDispatch create_generate_samples_dispatch() = 0;
    [[nodiscard]] virtual std::unique_ptr<Kernel> create_evaluate_emissions_kernel() { return nullptr; }
    [[nodiscard]] virtual EvaluateLightsDispatch create_evaluate_emissions_dispatch() { return {}; }
    [[nodiscard]] virtual size_t data_stride() const noexcept = 0;
    [[nodiscard]] virtual Shape *shape() const noexcept { return nullptr; }
    [[nodiscard]] virtual uint sampling_dimensions() const noexcept = 0;
    [[nodiscard]] virtual bool is_sky() const noexcept { return false; }
    virtual void encode_data(TypelessBuffer &buffer, size_t data_index, uint2 cdf_range, uint instance_id, uint triangle_offset, uint vertex_offset, float shape_area) = 0;
};
    
}

#endif