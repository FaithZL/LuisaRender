//
// Created by Mike Smith on 2020/8/17.
//

#pragma once

#include <compute/device.h>
#include <compute/dsl.h>

using luisa::compute::Device;
using luisa::compute::Dispatcher;
using luisa::compute::Kernel;
using luisa::compute::Texture;
using luisa::compute::dsl::Function;

namespace detail {

inline void box_blur_x_or_y(int rx, int ry, Texture &input, Texture &output) noexcept {
    
    using namespace luisa;
    using namespace luisa::compute;
    using namespace luisa::compute::dsl;
    
    Arg<ReadOnlyTex2D> in{input};
    Arg<WriteOnlyTex2D> out{output};
    
    auto width = static_cast<int>(input.width());
    auto height = static_cast<int>(input.height());
    
    Auto tx = cast<int>(thread_xy().x());
    Auto ty = cast<int>(thread_xy().y());
    If(tx < width && ty < height) {
        Float4 sum{0.0f};
        for (auto dx = -rx; dx <= rx; dx++) {
            auto x = tx + dx;
            If(x >= 0 && x < width) {
                sum += make_float4(make_float3(read(in, make_uint2(x, ty))), 1.0f);
            };
        }
        for (auto dy = -ry; dy <= ry; dy++) {
            auto y = ty + dy;
            If(y >= 0 && y < height) {
                sum += make_float4(make_float3(read(in, make_uint2(tx, y))), 1.0f);
            };
        }
        write(out, thread_xy(), make_float4(make_float3(sum) / sum.a(), 1.0f));
    };
}

}// namespace detail

class BoxBlur {

private:
    int _width;
    int _height;
    std::unique_ptr<Kernel> _blur_x;
    std::unique_ptr<Kernel> _blur_y;
    std::unique_ptr<Texture> _temp;

public:
    // Note: "input" and "output" may alias
    BoxBlur(Device &device, int rx, int ry, Texture &input, Texture &output) noexcept
        : _width{static_cast<int>(input.width())},
          _height{static_cast<int>(input.height())},
          _temp{device.allocate_texture<luisa::float4>(input.width(), input.height())} {
        _blur_x = device.compile_kernel([&] { detail::box_blur_x_or_y(rx, 0, input, *_temp); });
        _blur_y = device.compile_kernel([&] { detail::box_blur_x_or_y(0, ry, *_temp, output); });
    }
    
    void operator()(Dispatcher &dispatch) noexcept {
        using namespace luisa;
        dispatch(*_blur_x, make_uint2(_width, _height));
        dispatch(*_blur_y, make_uint2(_width, _height));
    }
};
