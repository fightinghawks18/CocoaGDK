#include <stdio.h>

#include "opengl/opengl_core.h"
#include "opengl/opengl_pipeline.h"
#include "opengl/opengl_context.h"
#include "opengl/opengl_ebo.h"
#include "opengl/opengl_shader.h"
#include "opengl/opengl_ubo.h"
#include "opengl/opengl_vao.h"
#include "platform/input.h"
#include "platform/utils.h"
#include "platform/window.h"

int main() {
    if (cco_windowing_init() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize windowing!");
        return -1;
    }

    if (cco_input_init() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize input!");
        return -1;
    }
    cco_input_enable();

    cco_window window = CCO_NIL;
    cco_result window_result = cco_create_window(0, 0, 800, 600, "cocoa", &window);
    if (window_result != CCO_SUCCESS) {
        CCO_LOG("Failed to create window!");
        return -1;
    }

    cco_input_give_window_focus(window);

    cco_opengl_context glCtx = CCO_NIL;
    cco_create_opengl_context(cco_window_get_handle(window), NULL, &glCtx);
    cco_make_current_opengl_context(glCtx);

    cco_initialize_opengl();

    cco_vertex vertices[3] = {
        {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    };

    u32 indices[3] = {0, 1, 2};

    vec3 position = cco_vec3(0, 0, 0);
    vec3 rotation = cco_vec3(0, 0, 0);
    vec3 scale = cco_vec3(1, 1, 1);

    vec3 camera_position = cco_vec3(0, 0, 1.0f);

    mat4 model_matrix = cco_mat4_model(
        cco_mat4_translation(position), cco_mat4_rotation(rotation), cco_mat4_scale(scale));
    mat4 view_matrix = cco_mat4_eye(camera_position, cco_vec3(0, 0, 0), cco_vec3_up());
    mat4 projection_matrix =
        cco_mat4_perspective(CCO_NO, CCO_NO, cco_deg_to_rad(80.0f), 800.0f / 600.0f, 0.001f, 100.0f);

    cco_model_view_projection mvp_buffer = {.model = cco_mat4_transpose(model_matrix),
                                        .view = cco_mat4_transpose(view_matrix),
                                        .projection = cco_mat4_transpose(projection_matrix)};

    cco_opengl_vbo vbo = CCO_NIL;
    cco_opengl_vao vao = CCO_NIL;
    cco_opengl_ebo ebo = CCO_NIL;
    cco_opengl_ubo ubo = CCO_NIL;
    cco_opengl_shader vs = CCO_NIL;
    cco_opengl_shader ps = CCO_NIL;
    cco_opengl_pipeline pip = CCO_NIL;

    cco_create_opengl_vbo(&vbo);
    cco_create_opengl_vao(&vao);
    cco_create_opengl_ebo(&ebo);
    cco_create_opengl_ubo(&ubo);

    cco_create_opengl_shader(
        &(cco_opengl_shader_desc){.shader_type = CCO_SHADER_TYPE_VERTEX, .shader_path = "shaders/test.vert"}, &vs);
    cco_create_opengl_shader(
        &(cco_opengl_shader_desc){.shader_type = CCO_SHADER_TYPE_PIXEL, .shader_path = "shaders/test.frag"}, &ps);
    cco_create_opengl_pipeline(&(cco_opengl_pipeline_desc){.vertex_shader = vs, .pixel_shader = ps}, &pip);

    cco_map_to_opengl_vbo(vbo, &(cco_buffer_mapping){.data_size = 3 * sizeof(cco_vertex), .data_offset = 0, .data = vertices});

    cco_map_to_opengl_ebo(ebo, &(cco_buffer_mapping){.data_size = 3 * sizeof(u32), .data_offset = 0, .data = indices});

    cco_map_to_opengl_ubo(
        ubo, &(cco_buffer_mapping){.data_size = sizeof(cco_model_view_projection), .data_offset = 0, .data = &mvp_buffer});

    cco_vertex_attribute vertex_attributes[2] = {
        {.location = 0, .num_components = 3, .stride = sizeof(cco_vertex), .offset = offsetof(cco_vertex, pos)},
        {.location = 1, .num_components = 4, .stride = sizeof(cco_vertex), .offset = offsetof(cco_vertex, col)}};

    cco_set_opengl_vao_layout(vao, vbo, ebo, &(cco_vertex_layout){.attributes = vertex_attributes, .attribute_count = 2});

    while (!cco_window_will_close(window)) {
        cco_window_pump_events(window);
        cco_input_poll();

        if (cco_input_key_was_just_pressed(CCO_INPUT_KEY_W)) {
            CCO_LOG("INPUT");
        }

        cco_make_current_opengl_context(glCtx);

        cco_window_content_size window_content_size = cco_window_get_content_size(window);

        projection_matrix =
            cco_mat4_perspective(CCO_NO, CCO_NO, cco_deg_to_rad(80.0f),
                                          (f32)window_content_size.width / (f32)window_content_size.height, 0.001f, 100.0f);
        mvp_buffer.projection = cco_mat4_transpose(projection_matrix);

        cco_map_to_opengl_ubo(ubo, &(cco_buffer_mapping){.data_size = sizeof(cco_model_view_projection),
                                                   .data_offset = offsetof(cco_model_view_projection, projection),
                                                   .data = &mvp_buffer.projection});

        cco_set_opengl_viewport((cco_viewport){.x = 0,
                                           .y = 0,
                                           .w = (i32)window_content_size.width,
                                           .h = (i32)window_content_size.height,
                                           .min_depth = 0,
                                           .max_depth = 1});
        cco_set_opengl_clear_color((cco_clear_color){.r = 0.12f, .g = 0.12f, .b = 0.12f, .a = 1.0f});
        cco_clear_opengl_buffers(CCO_OPENGL_COLOR_BUFFER_BIT | CCO_OPENGL_DEPTH_BUFFER_BIT);

        cco_use_opengl_pipeline(pip);
        cco_use_opengl_vao(vao);
        cco_use_opengl_ubo(&(cco_opengl_ubo_binding){.type = CCO_OPENGL_UBO_BINDING_BLOCK_NAME, .pip = pip, .name = "MVP"},
                        ubo);
        cco_draw_opengl_elements(CCO_OPENGL_PRIMITIVE_TRIANGLES, 3, CCO_OPENGL_INDEX_TYPE_U32);

        cco_flush_opengl_context_buffer(glCtx);

        cco_sleep(5);
    }

    cco_destroy_window(window);
    cco_windowing_quit();
    return 0;
}