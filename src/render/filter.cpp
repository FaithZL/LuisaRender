//
// Created by Mike Smith on 2020/9/14.
//

#include <compute/dsl_syntax.h>
#include <render/sampling.h>

#include "filter.h"

namespace luisa::render {

using namespace luisa::compute;
using namespace luisa::compute::dsl;

std::pair<Expr<float2>, Expr<float>> SeparableFilter::importance_sample_pixel_position(Expr<uint2> p, Expr<float2> u) {
    
    constexpr auto inv_table_size = 1.0f / lookup_table_size;
    
    if (!_table_generated) {
        auto abs_sum = 0.0f;
        for (auto i = 0u; i < lookup_table_size; i++) {
            auto offset = (static_cast<float>(i) * inv_table_size * 2.0f - 1.0f) * radius();
            auto w = _weight_1d(offset);
            _weight_table[i] = w;
            _cdf_table[i] = (abs_sum += std::abs(w));
        }
        auto inv_sum = 1.0f / abs_sum;
        for (float &cdf : _cdf_table) { cdf *= inv_sum; }
        auto absolute_volume = 0.0f;
        auto signed_volume = 0.0f;
        for (float x : _weight_table) {
            for (float y : _weight_table) {
                signed_volume += x * y;
                absolute_volume += std::abs(x * y);
            }
        }
        _scale = absolute_volume / signed_volume;
        _table_generated = true;
    }
    
    auto weight = immutable(_weight_table);
    auto cdf = immutable(_cdf_table);
    
    auto sample_1d = [&](Expr<float> u) {
        
        Var lb = sample_discrete(cdf, 0u, lookup_table_size - 1u, u);
        Var cdf_lower = cdf[lb];
        Var cdf_upper = select(lb == lookup_table_size - 1u, 1.0f, cdf[lb + 1u]);
        Var offset = dsl::clamp((cast<float>(lb) + (u - cdf_lower) / (cdf_upper - cdf_lower)) * inv_table_size, 0.0f, 1.0f);
        
        constexpr auto weight_table_size_float = static_cast<float>(lookup_table_size);
        Var index_w = offset * weight_table_size_float;
        Var index_w_lower = floor(index_w);
        Var index_w_upper = ceil(index_w);
        Var w = lerp(
            cdf[cast<uint>(index_w_lower)],
            select(index_w_upper >= weight_table_size_float, 0.0f, weight[cast<uint>(index_w_upper)]),
            index_w - index_w_lower);
        
        return std::make_pair(offset * 2.0f - 1.0f, select(w >= 0.0f, 1.0f, -1.0f));
    };
    
    auto[dx, wx] = sample_1d(u.x);
    auto[dy, wy] = sample_1d(u.y);
    auto pixel_position = make_float2(p) + 0.5f + make_float2(dx, dy) * radius();
    auto pixel_weight = wx * wy * _scale;
    return std::make_pair(pixel_position, pixel_weight);
}

}
