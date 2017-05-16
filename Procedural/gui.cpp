/* This code is modified from the examples in the Nuklear project 1.32.0 authored from 2015-2016 by Micha Mettke. The smart parts are his, the dumb ones are mine */

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_PRIVATE
#include "lib/include/nuklear.h"
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

static struct nk_context*   gui_init();
static void                 gui_begin();
static void                 gui_end();

#include <string.h>

struct nk_sdl_device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture null;
    GLuint vbo, vao, ebo;
    GLuint prog;
    GLuint vert_shdr;
    GLuint frag_shdr;
    GLint attrib_pos;
    GLint attrib_uv;
    GLint attrib_col;
    GLint uniform_tex;
    GLint uniform_proj;
    GLuint font_tex;
};

struct nk_sdl_vertex {
    float position[2];
    float uv[2];
    nk_byte col[4];
};

static struct nk_sdl {
    SDL_Window *win;
    struct nk_sdl_device ogl;
    struct nk_context ctx;
    struct nk_font_atlas atlas;
} sdl;

static void
gui__clipboard_paste(nk_handle usr, struct nk_text_edit *edit)
{
    const char *text = SDL_GetClipboardText();
    if (text) nk_textedit_paste(edit, text, nk_strlen(text));
    (void)usr;
}

static void
gui__clipboard_copy(nk_handle usr, const char *text, int len)
{
    char *str = 0;
    (void)usr;
    if (!len) return;
    str = (char*)malloc((size_t)len+1);
    if (!str) return;
    memcpy(str, text, (size_t)len);
    str[len] = '\0';
    SDL_SetClipboardText(str);
    free(str);
}

static struct nk_context* gui_init() {
    /* SDL setup */
    {
        int win_width = 800, win_height = 600;
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
        SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        sdl.win = SDL_CreateWindow("Demo",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            win_width, win_height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
        SDL_GL_CreateContext(sdl.win);
        SDL_GetWindowSize(sdl.win, &win_width, &win_height);

        glViewport(0, 0, win_width, win_height);
    }

    /* Init NK context */
    nk_init_default(&sdl.ctx, 0);

    /* Set clipboard */
    sdl.ctx.clip.copy = gui__clipboard_copy;
    sdl.ctx.clip.paste = gui__clipboard_paste;
    sdl.ctx.clip.userdata = nk_handle_ptr(0);

    /* Init openGL for nk */
    {
        GLint status;
        static const GLchar *vertex_shader =
        #ifdef __APPLE__
          #define NK_SHADER_VERSION "#version 150\n"
        #else
          #define NK_SHADER_VERSION "#version 300 es\n"
        #endif
        NK_SHADER_VERSION
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 TexCoord;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main() {\n"
        "   Frag_UV = TexCoord;\n"
        "   Frag_Color = Color;\n"
        "   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
        "}\n";
        static const GLchar *fragment_shader =
        NK_SHADER_VERSION
        "precision mediump float;\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main(){\n"
        "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

        struct nk_sdl_device *dev = &sdl.ogl;
        nk_buffer_init_default(&dev->cmds);
        dev->prog = glCreateProgram();
        dev->vert_shdr = glCreateShader(GL_VERTEX_SHADER);
        dev->frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(dev->vert_shdr, 1, &vertex_shader, 0);
        glShaderSource(dev->frag_shdr, 1, &fragment_shader, 0);
        glCompileShader(dev->vert_shdr);
        glCompileShader(dev->frag_shdr);
        glGetShaderiv(dev->vert_shdr, GL_COMPILE_STATUS, &status);
        assert(status == GL_TRUE);
        glGetShaderiv(dev->frag_shdr, GL_COMPILE_STATUS, &status);
        assert(status == GL_TRUE);
        glAttachShader(dev->prog, dev->vert_shdr);
        glAttachShader(dev->prog, dev->frag_shdr);
        glLinkProgram(dev->prog);
        glGetProgramiv(dev->prog, GL_LINK_STATUS, &status);
        assert(status == GL_TRUE);

        dev->uniform_tex = glGetUniformLocation(dev->prog, "Texture");
        dev->uniform_proj = glGetUniformLocation(dev->prog, "ProjMtx");
        dev->attrib_pos = glGetAttribLocation(dev->prog, "Position");
        dev->attrib_uv = glGetAttribLocation(dev->prog, "TexCoord");
        dev->attrib_col = glGetAttribLocation(dev->prog, "Color");

        {
        /* buffer setup */
            GLsizei vs = sizeof(struct nk_sdl_vertex);
            size_t vp = offsetof(struct nk_sdl_vertex, position);
            size_t vt = offsetof(struct nk_sdl_vertex, uv);
            size_t vc = offsetof(struct nk_sdl_vertex, col);

            glGenBuffers(1, &dev->vbo);
            glGenBuffers(1, &dev->ebo);
            glGenVertexArrays(1, &dev->vao);

            glBindVertexArray(dev->vao);
            glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

            glEnableVertexAttribArray((GLuint)dev->attrib_pos);
            glEnableVertexAttribArray((GLuint)dev->attrib_uv);
            glEnableVertexAttribArray((GLuint)dev->attrib_col);

            glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
            glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
            glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    /* Change font */
    {
        struct nk_font *roboto;
        const void *image; int w, h;
        nk_font_atlas_init_default(&sdl.atlas);
        nk_font_atlas_begin(&sdl.atlas);

        /* Add roboto and bake atlas */
        roboto = nk_font_atlas_add_from_file(&sdl.atlas, "Roboto-Regular.ttf", 16, 0);
        image = nk_font_atlas_bake(&sdl.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
        /* load atlas into opengl */
        glGenTextures(1, &sdl.ogl.font_tex);
        glBindTexture(GL_TEXTURE_2D, sdl.ogl.font_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)w, (GLsizei)h, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, image);

        nk_font_atlas_end(&sdl.atlas, nk_handle_id((int)sdl.ogl.font_tex), &sdl.ogl.null);
        /* nk_style_set_font(&sdl.ctx, &sdl.atlas.default_font->handle); */
        nk_style_set_font(&sdl.ctx, &roboto->handle);
    }

    return &sdl.ctx;
};

static void gui_begin() {
    SDL_Event evt;
    nk_input_begin(&sdl.ctx);
    /* Get input */
    while (SDL_PollEvent(&evt)) {
        struct nk_context *ctx = &sdl.ctx;

        switch (evt.type) {
            case SDL_QUIT: exit(0);
            case SDL_WINDOWEVENT:
                if (evt.window.event == SDL_WINDOWEVENT_CLOSE) exit(0);
                break;
            case SDL_KEYUP:
            case SDL_KEYDOWN: {
                /* key events */
                int down = evt.type == SDL_KEYDOWN;
                const Uint8* state = SDL_GetKeyboardState(0);
                SDL_Keycode sym = evt.key.keysym.sym;
                if (sym == SDLK_RSHIFT || sym == SDLK_LSHIFT)
                    nk_input_key(ctx, NK_KEY_SHIFT, down);
                else if (sym == SDLK_DELETE)
                    nk_input_key(ctx, NK_KEY_DEL, down);
                else if (sym == SDLK_RETURN)
                    nk_input_key(ctx, NK_KEY_ENTER, down);
                else if (sym == SDLK_TAB)
                    nk_input_key(ctx, NK_KEY_TAB, down);
                else if (sym == SDLK_BACKSPACE)
                    nk_input_key(ctx, NK_KEY_BACKSPACE, down);
                else if (sym == SDLK_HOME) {
                    nk_input_key(ctx, NK_KEY_TEXT_START, down);
                    nk_input_key(ctx, NK_KEY_SCROLL_START, down);
                } else if (sym == SDLK_END) {
                    nk_input_key(ctx, NK_KEY_TEXT_END, down);
                    nk_input_key(ctx, NK_KEY_SCROLL_END, down);
                } else if (sym == SDLK_PAGEDOWN) {
                    nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
                } else if (sym == SDLK_PAGEUP) {
                    nk_input_key(ctx, NK_KEY_SCROLL_UP, down);
                } else if (sym == SDLK_z)
                nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && state[SDL_SCANCODE_LCTRL]);
                else if (sym == SDLK_r)
                    nk_input_key(ctx, NK_KEY_TEXT_REDO, down && state[SDL_SCANCODE_LCTRL]);
                else if (sym == SDLK_c)
                    nk_input_key(ctx, NK_KEY_COPY, down && state[SDL_SCANCODE_LCTRL]);
                else if (sym == SDLK_v)
                    nk_input_key(ctx, NK_KEY_PASTE, down && state[SDL_SCANCODE_LCTRL]);
                else if (sym == SDLK_x)
                    nk_input_key(ctx, NK_KEY_CUT, down && state[SDL_SCANCODE_LCTRL]);
                else if (sym == SDLK_b)
                    nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && state[SDL_SCANCODE_LCTRL]);
                else if (sym == SDLK_e)
                    nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && state[SDL_SCANCODE_LCTRL]);
                else if (sym == SDLK_UP)
                    nk_input_key(ctx, NK_KEY_UP, down);
                else if (sym == SDLK_DOWN)
                    nk_input_key(ctx, NK_KEY_DOWN, down);
                else if (sym == SDLK_LEFT) {
                    if (state[SDL_SCANCODE_LCTRL])
                        nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
                    else nk_input_key(ctx, NK_KEY_LEFT, down);
                } else if (sym == SDLK_RIGHT) {
                    if (state[SDL_SCANCODE_LCTRL])
                        nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
                    else nk_input_key(ctx, NK_KEY_RIGHT, down);
                }
            } break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                /* mouse button */
                int down = evt.type == SDL_MOUSEBUTTONDOWN;
                const int x = evt.button.x, y = evt.button.y;
                if (evt.button.button == SDL_BUTTON_LEFT) {
                    if (evt.button.clicks > 1)
                        nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
                    nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
                } else if (evt.button.button == SDL_BUTTON_MIDDLE)
                nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
                else if (evt.button.button == SDL_BUTTON_RIGHT)
                    nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
            } break;
            case SDL_MOUSEMOTION:
                /* mouse motion */
                if (ctx->input.mouse.grabbed) {
                    int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
                    nk_input_motion(ctx, x + evt.motion.xrel, y + evt.motion.yrel);
                } else nk_input_motion(ctx, evt.motion.x, evt.motion.y);
                break;
            case SDL_TEXTINPUT: {
                /* text input */
                nk_glyph glyph;
                memcpy(glyph, evt.text.text, NK_UTF_SIZE);
                nk_input_glyph(ctx, glyph);
            } break;
            case SDL_MOUSEWHEEL:
                /* mouse wheel */
                nk_input_scroll(ctx,nk_vec2((float)evt.wheel.x,(float)evt.wheel.y));
        }
    }
    nk_input_end(&sdl.ctx);
}

static void gui_end() {
    int win_width, win_height;
    struct nk_color background = nk_rgb(28,48,62);
    /* Draw */
    float bg[4];
    nk_color_fv(bg, background);
    SDL_GetWindowSize(sdl.win, &win_width, &win_height);
    glViewport(0, 0, win_width, win_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(bg[0], bg[1], bg[2], bg[3]);
    /* IMPORTANT: modifies some global OpenGL state
     * with blending, scissor, face culling, depth test and viewport and
     * defaults everything back into a default state.
     * Make sure to either a.) save and restore or b.) reset your own state after
     * rendering the UI. */
    {
        struct nk_sdl_device *dev = &sdl.ogl;
        int width, height;
        int display_width, display_height;
        struct nk_vec2 scale;
        GLfloat ortho[4][4] = {
            {2.0f, 0.0f, 0.0f, 0.0f},
            {0.0f,-2.0f, 0.0f, 0.0f},
            {0.0f, 0.0f,-1.0f, 0.0f},
            {-1.0f,1.0f, 0.0f, 1.0f},
        };
        SDL_GetWindowSize(sdl.win, &width, &height);
        SDL_GL_GetDrawableSize(sdl.win, &display_width, &display_height);
        ortho[0][0] /= (GLfloat)width;
        ortho[1][1] /= (GLfloat)height;

        scale.x = (float)display_width/(float)width;
        scale.y = (float)display_height/(float)height;

        /* setup global state */
        glViewport(0,0,display_width,display_height);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glActiveTexture(GL_TEXTURE0);

        /* setup program */
        glUseProgram(dev->prog);
        glUniform1i(dev->uniform_tex, 0);
        glUniformMatrix4fv(dev->uniform_proj, 1, GL_FALSE, &ortho[0][0]);
        {
            /* convert from command queue into draw list and draw to screen */
            const struct nk_draw_command *cmd;
            void *vertices, *elements;
            const nk_draw_index *offset = NULL;
            const int max_vertex_buffer = 512*1024;
            const int max_element_buffer = 128*1024;

            /* allocate vertex and element buffer */
            glBindVertexArray(dev->vao);
            glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

            glBufferData(GL_ARRAY_BUFFER, max_vertex_buffer, NULL, GL_STREAM_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_element_buffer, NULL, GL_STREAM_DRAW);

            /* load vertices/elements directly into vertex/element buffer */
            vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
            {
                /* fill convert configuration */
                struct nk_convert_config config;
                static const struct nk_draw_vertex_layout_element vertex_layout[] = {
                    {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_sdl_vertex, position)},
                    {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_sdl_vertex, uv)},
                    {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct nk_sdl_vertex, col)},
                    {NK_VERTEX_LAYOUT_END}
                };
                NK_MEMSET(&config, 0, sizeof(config));
                config.vertex_layout = vertex_layout;
                config.vertex_size = sizeof(struct nk_sdl_vertex);
                config.vertex_alignment = NK_ALIGNOF(struct nk_sdl_vertex);
                config.null = dev->null;
                config.circle_segment_count = 22;
                config.curve_segment_count = 22;
                config.arc_segment_count = 22;
                config.global_alpha = 1.0f;
                config.shape_AA = NK_ANTI_ALIASING_ON;
                config.line_AA = NK_ANTI_ALIASING_ON;

                /* setup buffers to load vertices and elements */
                {struct nk_buffer vbuf, ebuf;
                nk_buffer_init_fixed(&vbuf, vertices, (nk_size)max_vertex_buffer);
                nk_buffer_init_fixed(&ebuf, elements, (nk_size)max_element_buffer);
                nk_convert(&sdl.ctx, &dev->cmds, &vbuf, &ebuf, &config);}
            }
            glUnmapBuffer(GL_ARRAY_BUFFER);
            glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

            /* iterate over and execute each draw command */
            nk_draw_foreach(cmd, &sdl.ctx, &dev->cmds) {
                if (!cmd->elem_count) continue;
                glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
                glScissor((GLint)(cmd->clip_rect.x * scale.x),
                    (GLint)((height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * scale.y),
                    (GLint)(cmd->clip_rect.w * scale.x),
                    (GLint)(cmd->clip_rect.h * scale.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
                offset += cmd->elem_count;
            }
            nk_clear(&sdl.ctx);
        }

        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDisable(GL_BLEND);
        glDisable(GL_SCISSOR_TEST);
    }
    SDL_GL_SwapWindow(sdl.win);
}

static void
gui_shutdown(void)
{
    nk_font_atlas_clear(&sdl.atlas);
    nk_free(&sdl.ctx);
    glDetachShader(sdl.ogl.prog, sdl.ogl.vert_shdr);
    glDetachShader(sdl.ogl.prog, sdl.ogl.frag_shdr);
    glDeleteShader(sdl.ogl.vert_shdr);
    glDeleteShader(sdl.ogl.frag_shdr);
    glDeleteProgram(sdl.ogl.prog);
    glDeleteTextures(1, &sdl.ogl.font_tex);
    glDeleteBuffers(1, &sdl.ogl.vbo);
    glDeleteBuffers(1, &sdl.ogl.ebo);
    nk_buffer_free(&sdl.ogl.cmds);
    memset(&sdl, 0, sizeof(sdl));
}

