//
// Created by Mike Smith on 2019/10/21.
//

#include <random.h>
#include <frame_data.h>
#include <ray_data.h>
#include <intersection_data.h>
#include <material_data.h>
#include <light_data.h>
#include <onb.h>
#include <sampling.h>
#include <color_spaces.h>

using namespace metal;

kernel void sample_lights(
    device const uint *ray_index_buffer,
    device const Ray *ray_buffer,
    device uint *ray_seed_buffer,
    device const IntersectionData *intersection_buffer,
    device const LightData *light_buffer,
    device const Vec3f *p_buffer,
    device Ray *shadow_ray_buffer,
    device LightSample *light_sample_buffer,
    constant uint &light_count,
    device const uint &ray_count,
    uint tid [[thread_index_in_threadgroup]],
    uint2 tgsize [[threads_per_threadgroup]],
    uint2 tgid [[threadgroup_position_in_grid]],
    uint2 gsize [[threadgroups_per_grid]]) {
    
    auto thread_index = (tgsize.x * tgsize.y) * (tgid.y * gsize.x + tgid.x) + tid;
    
    if (thread_index < ray_count) {
        
        auto ray_index = ray_index_buffer[thread_index];
        auto its = intersection_buffer[ray_index];
        
        if (ray_buffer[ray_index].max_distance <= 0.0f || its.distance <= 0.0f) {  // no intersection
            shadow_ray_buffer[ray_index].max_distance = -1.0f;  // terminate the ray
        } else {  // has an intersection
            auto uvw = Vec3f(its.barycentric, 1.0f - its.barycentric.x - its.barycentric.y);
            auto i0 = its.triangle_index * 3u;
            auto P = uvw.x * p_buffer[i0] + uvw.y * p_buffer[i0 + 1] + uvw.z * p_buffer[i0 + 2];
            auto seed = ray_seed_buffer[ray_index];
            auto light = light_buffer[min(static_cast<uint>(halton(seed) * light_count), light_count - 1u)];
            ray_seed_buffer[ray_index] = seed;
            auto L = light.position - P;
            auto dist = length(L);
            auto inv_dist = 1.0f / dist;
            auto wi = normalize(L);
            
            Ray shadow_ray{};
            shadow_ray.direction = wi;
            shadow_ray.origin = P + 1e-4f * wi;
            shadow_ray.min_distance = 0.0f;
            shadow_ray.max_distance = dist - 1e-4f;
            shadow_ray_buffer[thread_index] = shadow_ray;
            
            LightSample light_sample{};
            light_sample.radiance = XYZ2ACEScg(RGB2XYZ(light.emission)) * inv_dist * inv_dist * static_cast<float>(light_count);
            light_sample.pdf = 1.0f;
            light_sample.direction = wi;
            light_sample.distance = dist;
            light_sample_buffer[thread_index] = light_sample;
        }
    }
}

kernel void trace_radiance(
    device const uint *ray_index_buffer,
    device Ray *ray_buffer,
    device Vec3f *ray_radiance_buffer,
    device Vec3f *ray_throughput_buffer,
    device uint *ray_seed_buffer,
    device uint *ray_depth_buffer,
    device const LightSample *light_sample_buffer,
    device const IntersectionData *its_buffer,
    device const ShadowIntersectionData *shadow_its_buffer,
    device const Vec3f *p_buffer,
    device const Vec3f *n_buffer,
    device const uint *material_id_buffer,
    device const MaterialData *material_buffer,
    device const uint &ray_count,
    uint tid [[thread_index_in_threadgroup]],
    uint2 tgsize [[threads_per_threadgroup]],
    uint2 tgid [[threadgroup_position_in_grid]],
    uint2 gsize [[threadgroups_per_grid]]) {
    
    auto thread_index = (tgsize.x * tgsize.y) * (tgid.y * gsize.x + tgid.x) + tid;
    
    if (thread_index < ray_count) {
        
        auto ray_index = ray_index_buffer[thread_index];
        auto ray = ray_buffer[ray_index];
        if (ray.max_distance <= 0.0f || its_buffer[ray_index].distance <= 0.0f) {  // no intersection
            ray_buffer[ray_index].max_distance = -1.0f;  // terminate the ray
        } else {
            auto its = its_buffer[ray_index];
            auto material = material_buffer[material_id_buffer[its.triangle_index]];
            material.albedo = XYZ2ACEScg(RGB2XYZ(material.albedo));
            auto i0 = its.triangle_index * 3;
            auto uvw = Vec3f(its.barycentric, 1.0f - its.barycentric.x - its.barycentric.y);
            auto P = uvw.x * p_buffer[i0] + uvw.y * p_buffer[i0 + 1] + uvw.z * p_buffer[i0 + 2];
            auto N = normalize(uvw.x * n_buffer[i0] + uvw.y * n_buffer[i0 + 1] + uvw.z * n_buffer[i0 + 2]);
            auto V = -ray.direction;
            
            auto NdotV = dot(N, V);
            if (NdotV < 0.0f) {
                N = -N;
                NdotV = -NdotV;
            }
            
            // direct lighting
            auto shadow_its = shadow_its_buffer[thread_index];
            auto ray_throughput = ray_throughput_buffer[ray_index];
            if (!material.is_mirror && shadow_its.distance < 0.0f) {  // not occluded
                auto light_sample = light_sample_buffer[thread_index];
                auto NdotL = max(dot(N, light_sample.direction), 0.0f);
                ray_radiance_buffer[ray_index] += ray_throughput * material.albedo * NdotL * light_sample.radiance / light_sample.pdf;
            }
            
            // sampling brdf
            ray.max_distance = INFINITY;
            if (material.is_mirror) {
                ray.direction = normalize(2.0f * NdotV * N - V);
            } else {
                auto seed = ray_seed_buffer[ray_index];
                ray.direction = normalize(Onb{N}.inverse_transform(Vec3f(cosine_sample_hemisphere(halton(seed), halton(seed)))));
                ray_throughput *= material.albedo;  // simplified for lambertian materials
                auto ray_depth = (++ray_depth_buffer[ray_index]);
                if (ray_depth > 3) {  // RR
                    auto q = max(0.05f, 1.0f - ACEScg2Luminance(ray_throughput));
                    if (halton(seed) < q) {
                        ray.max_distance = -1.0f;
                    } else {
                        ray_throughput /= 1.0f - q;
                    }
                }
                ray_seed_buffer[ray_index] = seed;
            }
            ray_throughput_buffer[ray_index] = ray_throughput;
            ray.origin = P + 1e-4f * ray.direction;
            ray.min_distance = 0.0f;
            ray_buffer[ray_index] = ray;
        }
    }
}

kernel void sort_rays(
    device const uint *ray_index_buffer,
    device const Ray *ray_buffer,
    device const Vec3f *ray_radiance_buffer,
    device const Vec2f *ray_pixel_buffer,
    device uint *output_ray_index_buffer,
    device const uint &ray_count,
    device atomic_uint &output_ray_count,
    constant FrameData &frame_data,
    device GatherRayData *gather_ray_buffer,
    uint tid [[thread_index_in_threadgroup]],
    uint2 tgsize [[threads_per_threadgroup]],
    uint2 tgid [[threadgroup_position_in_grid]],
    uint2 gsize [[threadgroups_per_grid]]) {
    
    auto thread_index = (tgsize.x * tgsize.y) * (tgid.y * gsize.x + tgid.x) + tid;
    
    if (thread_index < ray_count) {
        auto ray_index = ray_index_buffer[thread_index];
        if (ray_buffer[ray_index].max_distance > 0.0f) {  // add active rays to next bounce
            auto vote = static_cast<simd_vote::vote_t>(simd_active_threads_mask());
            auto output_index = 0u;
            if (simd_is_first()) {
                auto count = popcount(static_cast<uint>(vote)) + popcount(static_cast<uint>(vote >> 32u));
                output_index = atomic_fetch_add_explicit(&output_ray_count, count, memory_order_relaxed);
            }
            output_index = simd_broadcast_first(output_index) + simd_prefix_exclusive_sum(1u);
            output_ray_index_buffer[output_index] = ray_index;
        } else {
            auto ray_pixel = ray_pixel_buffer[ray_index];
            auto screen = uint2(ray_pixel);
            auto gather_index = screen.y * frame_data.size.x + screen.x;
            gather_ray_buffer[gather_index] = {ray_radiance_buffer[ray_index], ray_pixel};
        }
    }
}

kernel void gather_rays(
    device const uint *ray_index_buffer,
    device const Vec3f *ray_radiance_buffer,
    device const Vec2f *ray_pixel_buffer,
    device const uint &ray_count,
    device GatherRayData *gather_ray_buffer,
    constant FrameData &frame_data,
    uint tid [[thread_index_in_threadgroup]],
    uint2 tgsize [[threads_per_threadgroup]],
    uint2 tgid [[threadgroup_position_in_grid]],
    uint2 gsize [[threadgroups_per_grid]]) {
    
    auto thread_index = (tgsize.x * tgsize.y) * (tgid.y * gsize.x + tgid.x) + tid;
    
    if (thread_index < ray_count) {
        auto ray_index = ray_index_buffer[thread_index];
        auto pixel = ray_pixel_buffer[ray_index];
        auto screen = uint2(pixel);
        auto gather_index = screen.y * frame_data.size.x + screen.x;
        gather_ray_buffer[gather_index] = {ray_radiance_buffer[ray_index], pixel};
    }
}
