message(STATUS "Building with Apple Metal support")

enable_language(OBJC)
enable_language(OBJCXX)
file(GLOB SOURCE_FILES *.mm *.h)
luisa_render_add_backend(metal SOURCES ${SOURCE_FILES})
target_compile_options(luisa-backend-metal PRIVATE $<$<OR:$<COMPILE_LANGUAGE:OBJC>,$<COMPILE_LANGUAGE:OBJCXX>>:-fobjc-arc>)
target_link_libraries(luisa-backend-metal PRIVATE "-framework Foundation" "-framework Metal" "-framework MetalKit" "-framework MetalPerformanceShaders")
