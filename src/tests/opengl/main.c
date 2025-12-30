#include "math/utilities.h"

#include <stdio.h>

#include "opengl/opengl_context.h"
#include "opengl/opengl_core.h"
#include "opengl/opengl_ebo.h"
#include "opengl/opengl_image.h"
#include "opengl/opengl_loader.h"
#include "opengl/opengl_pipeline.h"
#include "opengl/opengl_shader.h"
#include "opengl/opengl_ubo.h"
#include "opengl/opengl_vao.h"
#include "platform/audio.h"
#include "platform/input.h"
#include "platform/input_gamepad.h"
#include "platform/input_keyboard.h"
#include "platform/input_mouse.h"
#include "platform/platform.h"
#include "platform/utils.h"
#include "platform/window.h"

cco_window window = CCO_NIL;
cco_opengl_vbo vbo = CCO_NIL;
cco_opengl_vao vao = CCO_NIL;
cco_opengl_ebo ebo = CCO_NIL;
cco_opengl_ubo ubo = CCO_NIL;
cco_opengl_shader vs = CCO_NIL;
cco_opengl_shader ps = CCO_NIL;
cco_opengl_pipeline pip = CCO_NIL;
cco_opengl_image img = CCO_NIL;

vec3 position = {0, 0, 0};
vec3 rotation = {0, 0, 0};
vec3 scale = {1, 1, 1};


cco_model_view_projection mvp_buffer = {0};

typedef struct {
    vec3 position;
    f32 yaw;    // Horizontal rotation (radians)
    f32 pitch;  // Vertical rotation (radians)
} camera;

camera cam;
f32 fov = 90.0f;

void update_camera(f32 delta_time) {
    const f32 mouse_sensitivity = 0.002f;
    const f32 gamepad_sensitivity = 3.0f;
    
    const f32 move_speed = 5.0f;
    
    cco_mouse_delta mouse = cco_input_get_mouse_delta();
    
    f32 rotation_x = mouse.x * mouse_sensitivity;
    f32 rotation_y = mouse.y * mouse_sensitivity;
    
    cco_gamepad_stick_pos right_stick;
    if (cco_input_read_gamepad_stick_pos(0, CCO_GAMEPAD_STICK_RIGHT, &right_stick)) {
        rotation_x += right_stick.x * gamepad_sensitivity * delta_time;
        rotation_y -= right_stick.y * gamepad_sensitivity * delta_time;
    }

    cam.yaw += rotation_x;
    cam.pitch -= rotation_y;

    const f32 max_pitch = cco_deg_to_rad(89.0f);
    cam.pitch = CCO_CLAMP(cam.pitch, -max_pitch, max_pitch);

    vec3 forward;
    forward.x = cosf(cam.yaw) * cosf(cam.pitch);
    forward.y = sinf(cam.pitch);
    forward.z = sinf(cam.yaw) * cosf(cam.pitch);
    forward = cco_vec3_normalized(forward);

    vec3 world_up = cco_vec3_up();
    vec3 right = cco_vec3_normalized(cco_vec3_cross(forward, world_up));

    vec3 move_input = cco_vec3(0, 0, 0);
    if (cco_input_key_is_pressed(CCO_INPUT_KEY_W)) {
        move_input = cco_vec3_add(move_input, forward);
    }
    if (cco_input_key_is_pressed(CCO_INPUT_KEY_S)) {
        move_input = cco_vec3_sub(move_input, forward);
    }
    if (cco_input_key_is_pressed(CCO_INPUT_KEY_D)) {
        move_input = cco_vec3_add(move_input, right);
    }
    if (cco_input_key_is_pressed(CCO_INPUT_KEY_A)) {
        move_input = cco_vec3_sub(move_input, right);
    }
    if (cco_input_key_is_pressed(CCO_INPUT_KEY_SPACE)) {
        move_input.y += 1.0f;
    }
    if (cco_input_key_is_pressed(CCO_INPUT_KEY_LEFT_SHIFT)) {
        move_input.y -= 1.0f;
    }

    cco_gamepad_stick_pos left_stick;
    if (cco_input_read_gamepad_stick_pos(0, CCO_GAMEPAD_STICK_LEFT, &left_stick)) {
        vec3 stick_forward = cco_vec3_scale(forward, left_stick.y);
        move_input = cco_vec3_add(move_input, stick_forward);

        vec3 stick_right = cco_vec3_scale(right, left_stick.x);
        move_input = cco_vec3_add(move_input, stick_right);

        f32 left_trigger, right_trigger;
        if (cco_input_read_gamepad_trigger_pressure(0, CCO_GAMEPAD_TRIGGER_LEFT, &left_trigger)) {
            move_input.y -= left_trigger;
        }
        if (cco_input_read_gamepad_trigger_pressure(0, CCO_GAMEPAD_TRIGGER_RIGHT, &right_trigger)) {
            move_input.y += right_trigger;
        }
    }

    f32 move_magnitude = cco_vec3_magnitude(move_input);
    if (move_magnitude > 1.0f) {
        move_input = cco_vec3_normalized(move_input);
    }

    vec3 movement = cco_vec3_scale(move_input, move_speed * delta_time);
    cam.position = cco_vec3_add(cam.position, movement);

    vec3 target = cco_vec3_add(cam.position, forward);
    mat4 view = cco_mat4_eye(cam.position, target, world_up);
    
    cco_window_content_size window_content_size = cco_window_get_content_size(window);
    const cco_mouse_delta delta = cco_input_get_mouse_delta();
    fov = CCO_CLAMP(fov + delta.wheel * 3.5, 10.0f, 150.0f);

    mvp_buffer.projection = cco_mat4_transpose(cco_mat4_perspective(CCO_NO, CCO_NO, cco_deg_to_rad(fov),
                             (f32)window_content_size.width / (f32)window_content_size.height, 0.001f, 100.0f));
    mvp_buffer.view = cco_mat4_transpose(view);
    mvp_buffer.model = cco_mat4_transpose(cco_mat4_model(cco_mat4_translation(position), cco_mat4_rotation(rotation), cco_mat4_scale(scale)));
}

int main() {
    if (cco_platform_init(CCO_PLATFORM_INIT_AUDIO_BIT | CCO_PLATFORM_INIT_WINDOWING_BIT |
                          CCO_PLATFORM_INIT_INPUT_BIT) != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize platform!");
        return -1;
    }

    cco_result window_result = cco_create_window(0, 0, 800, 600, "cocoa", &window);
    if (window_result != CCO_SUCCESS) {
        CCO_LOG("Failed to create window!");
        return -1;
    }

    cco_input_give_window_focus(window);

    cco_opengl_context glCtx = CCO_NIL;
    cco_create_opengl_context(cco_window_get_handle(window), NULL, &glCtx);
    cco_opengl_context_make_current(glCtx);

    cco_initialize_opengl();

    cco_vertex vertices[3] = {
        {{0.0f, 0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    };

    u32 indices[3] = {0, 1, 2};

    cco_create_opengl_vbo(&vbo);
    cco_create_opengl_vao(&vao);
    cco_create_opengl_ebo(&ebo);
    cco_create_opengl_ubo(&ubo);
    cco_create_opengl_image(&img);

    cco_opengl_vbo_allocate(vbo, 3 * sizeof(cco_vertex));
    cco_opengl_ebo_allocate(ebo, 3 * sizeof(u32));
    cco_opengl_ubo_allocate(ubo, sizeof(cco_model_view_projection));

    cco_create_opengl_shader(
        &(cco_opengl_shader_desc){.shader_type = CCO_SHADER_TYPE_VERTEX, .shader_path = "assets/shaders/test.vert"},
        &vs);
    cco_create_opengl_shader(
        &(cco_opengl_shader_desc){.shader_type = CCO_SHADER_TYPE_PIXEL, .shader_path = "assets/shaders/test.frag"},
        &ps);
    cco_create_opengl_pipeline(&(cco_opengl_pipeline_desc){.vertex_shader = vs, .pixel_shader = ps}, &pip);
    cco_opengl_pipeline_use(pip);
    cco_opengl_pipeline_set_uniform_image_slot(pip, "tex", 0);

    cco_opengl_vbo_upload(
        vbo, &(cco_buffer_mapping){.data_size = 3 * sizeof(cco_vertex), .data_offset = 0, .data = &vertices});

    cco_opengl_ebo_upload(ebo, &(cco_buffer_mapping){.data_size = 3 * sizeof(u32), .data_offset = 0, .data = &indices});

    cco_opengl_ubo_upload(
        ubo,
        &(cco_buffer_mapping){.data_size = sizeof(cco_model_view_projection), .data_offset = 0, .data = &mvp_buffer});

    cco_image image_data;
    cco_load_image_from_file("assets/texture/checkers.jpg", &image_data);

    cco_opengl_image_upload(GL_TEXTURE_2D, img, &image_data);
    cco_opengl_image_set_wrapping(img, GL_REPEAT, GL_REPEAT);
    cco_opengl_image_set_filtering(img, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    cco_opengl_image_use(GL_TEXTURE_2D, img);
    cco_opengl_image_bind(0, img);

    cco_vertex_attribute vertex_attributes[3] = {
        {.location = 0, .num_components = 3, .stride = sizeof(cco_vertex), .offset = offsetof(cco_vertex, pos)},
        {.location = 1, .num_components = 2, .stride = sizeof(cco_vertex), .offset = offsetof(cco_vertex, uv)},
        {.location = 2, .num_components = 4, .stride = sizeof(cco_vertex), .offset = offsetof(cco_vertex, col)}};

    cco_opengl_vao_set_layout(vao, vbo, ebo,
                              &(cco_vertex_layout){.attributes = vertex_attributes, .attribute_count = 3});

    f32 fov = 90.0f;
    vec3 look_dir = cco_vec3(0, 0, 0);

    // MUSIC

    cco_audio_pcm music_pcm = CCO_NIL;
    if (cco_audio_pcm_from_file("assets/audio/track00.wav", &music_pcm) != CCO_SUCCESS) {
        CCO_LOG("Failed to load audio pcm!");
        return -1;
    }

    cco_audio_stream music_track = CCO_NIL;
    if (cco_audio_build_stream_from_pcm(music_pcm, &music_track) != CCO_SUCCESS) {
        CCO_LOG("Failed to build music stream from pcm!");
        return -1;
    }

    cco_audio_stream_set_looped(music_track, CCO_YES);
    cco_audio_stream_set_volume(music_track,
                                0.3f); // Just in case the audio gets messed up and doesn't blow your eardrums
    cco_audio_stream_play(music_track);

    f32 yaw = 0.0f;
    f32 pitch = 0.0f;

    while (!cco_window_will_close(window)) {
        cco_window_pump_events(window);
        cco_input_poll();

        if (cco_input_key_was_just_pressed(CCO_INPUT_KEY_ESC))
            cco_window_request_close(window);

        cco_window_content_size window_content_size = cco_window_get_content_size(window);

        update_camera(1.0f / 60.0f);

        cco_opengl_ubo_upload(ubo, &(cco_buffer_mapping){.data_size = sizeof(cco_model_view_projection),
                                                         .data_offset = 0,
                                                         .data = &mvp_buffer});

        cco_opengl_set_viewport((cco_viewport){.x = 0,
                                               .y = 0,
                                               .w = (i32)window_content_size.width,
                                               .h = (i32)window_content_size.height,
                                               .min_depth = 0,
                                               .max_depth = 1});
        cco_opengl_set_clear_color((cco_clear_color){.r = 0.12f, .g = 0.12f, .b = 0.12f, .a = 1.0f});
        cco_opengl_clear_buffers(CCO_OPENGL_COLOR_BUFFER_BIT | CCO_OPENGL_DEPTH_BUFFER_BIT);

        cco_opengl_vao_use(vao);
        cco_opengl_ubo_use(
            &(cco_opengl_ubo_binding){.type = CCO_OPENGL_UBO_BINDING_BLOCK_NAME, .pip = pip, .name = "MVP"}, ubo);
        cco_opengl_draw_elements(CCO_OPENGL_PRIMITIVE_TRIANGLES, 3, CCO_OPENGL_INDEX_TYPE_U32);

        cco_opengl_context_flush(glCtx);

        cco_sleep(5);
    }

    cco_destroy_opengl_image(img);
    cco_destroy_opengl_pipeline(pip);
    cco_destroy_opengl_shader(ps);
    cco_destroy_opengl_shader(vs);
    cco_destroy_opengl_ubo(ubo);
    cco_destroy_opengl_ebo(ebo);
    cco_destroy_opengl_vao(vao);
    cco_destroy_opengl_vbo(vbo);

    cco_free_image(image_data);

    cco_opengl_context_make_current(NULL);
    cco_destroy_opengl_context(glCtx);

    cco_audio_stream_stop(music_track);
    cco_audio_unload_stream(music_track);
    cco_audio_unload_pcm(music_pcm);

    cco_destroy_window(window);

    cco_platform_quit();
    return 0;
}