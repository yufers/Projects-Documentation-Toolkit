#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm> // для std::min

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int INITIAL_WIDTH = 800;
const int INITIAL_HEIGHT = 600;
const int MIN_RESOLUTION = 50;
const int MAX_RESOLUTION = 2000;

float global_angle = 0.0f;
float global_alpha = 0.8f;
const float rotation_speed = 1.0f;

bool magnifier_active = false;
float magnifier_x = 0.0f;
float magnifier_y = 0.0f;
const float magnifier_radius = 150.0f; // в пикселях
const float magnifier_zoom = 2.0f;

int logicalWidth = 500;
int logicalHeight = 500;

struct Vertex {
    glm::vec2 position;
    int shaderType;
};

std::vector<Vertex> CreateLetterN() {
    return {
        {{0.0f, 0.0f},0}, {{0.0f,2.0f},0}, {{0.3f,2.0f},0}, {{0.3f,0.0f},0},
        {{0.7f,0.0f},1}, {{0.7f,2.0f},1}, {{1.0f,2.0f},1}, {{1.0f,0.0f},1},
        {{0.3f,1.0f},2}, {{0.7f,1.0f},2}, {{0.7f,1.2f},2}, {{0.3f,1.2f},2}
    };
}

std::vector<Vertex> CreateLetterT() {
    return {
        {{0.0f,1.8f},0}, {{1.0f,1.8f},0}, {{1.0f,2.0f},0}, {{0.0f,2.0f},0},
        {{0.4f,0.0f},1}, {{0.6f,0.0f},1}, {{0.6f,1.8f},1}, {{0.4f,1.8f},1}
    };
}

std::vector<GLuint> CreateIndicesN() { return {0,1,2,0,2,3,4,5,6,4,6,7,8,9,10,8,10,11}; }
std::vector<GLuint> CreateIndicesT() { return {0,1,2,0,2,3,4,5,6,4,6,7}; }

GLuint CompileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader,1,&source,nullptr);
    glCompileShader(shader);
    GLint success; glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(shader,512,nullptr,infoLog);
        std::cerr<<"Shader compile error:\n"<<infoLog<<std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint CreateProgram(const char* vertSrc,const char* fragSrc){
    GLuint vertex = CompileShader(vertSrc,GL_VERTEX_SHADER);
    GLuint fragment = CompileShader(fragSrc,GL_FRAGMENT_SHADER);
    if(!vertex||!fragment) return 0;
    GLuint program = glCreateProgram();
    glAttachShader(program,vertex);
    glAttachShader(program,fragment);
    glLinkProgram(program);
    GLint success; glGetProgramiv(program,GL_LINK_STATUS,&success);
    if(!success){
        char infoLog[512];
        glGetProgramInfoLog(program,512,nullptr,infoLog);
        std::cerr<<"Program link error:\n"<<infoLog<<std::endl;
        glDeleteProgram(program);
        return 0;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

const char* vertexShaderSource = R"(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in int aShaderType;
out vec2 FragPos;
flat out int ShaderType;
uniform mat4 MVP;
void main(){ FragPos=aPos; ShaderType=aShaderType; gl_Position=MVP*vec4(aPos,0.0,1.0); }
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 FragPos;
flat in int ShaderType;
uniform float time;
uniform float alpha;
uniform vec3 letterColor;
vec3 waveEffect(vec2 pos){ float w=0.5+0.5*sin(pos.y*10.0+time*2.0); return vec3(w,0.3,1.0-w);}
vec3 shinyEffect(vec2 pos){ float shine=0.5+0.5*sin(pos.y*6.0+time*3.0); return vec3(1.0,0.8,0.2)*shine;}
vec3 calmBarEffect(vec2 pos){ float wave=0.3+0.2*sin(pos.x*15.0+pos.y*8.0+time*1.5); return vec3(0.2,0.4+wave,0.8);}
void main(){ vec3 color; if(ShaderType==0) color=waveEffect(FragPos); else if(ShaderType==1) color=shinyEffect(FragPos); else if(ShaderType==2) color=calmBarEffect(FragPos); else color=letterColor/255.0; FragColor=vec4(color,alpha); }
)";

// Исправленный шейдер лупы
const char* magnifierVertexShader = R"(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aTex;
out vec2 TexCoord;
void main(){ TexCoord=aTex; gl_Position=vec4(aPos,0.0,1.0);}
)";

const char* magnifierFragmentShader = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D sceneTex;
uniform vec2 center;
uniform float radius;
uniform float zoom;
uniform vec2 screenAspect; // (1.0, h/w)

void main(){
    vec2 pos = TexCoord * screenAspect;
    vec2 c = center * screenAspect;
    float dist = length(pos - c);

    vec4 color = texture(sceneTex, TexCoord);

    if (dist < radius) {
        vec2 uv_sq = c + (pos - c) / zoom;
        vec2 uv = uv_sq / screenAspect;
        color = texture(sceneTex, uv);
    }

    FragColor = color;
}
)";

int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){ std::cerr<<"SDL init error"<<std::endl; return 1;}
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window=SDL_CreateWindow("OpenGL Лупа",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,INITIAL_WIDTH,INITIAL_HEIGHT,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    SDL_GLContext context=SDL_GL_CreateContext(window);
    glewInit();

    GLuint letterProgram = CreateProgram(vertexShaderSource, fragmentShaderSource);
    GLuint magnifierProgram = CreateProgram(magnifierVertexShader, magnifierFragmentShader);

    std::vector<Vertex> verticesN=CreateLetterN();
    std::vector<GLuint> indicesN=CreateIndicesN();
    std::vector<Vertex> verticesT=CreateLetterT();
    std::vector<GLuint> indicesT=CreateIndicesT();

    GLuint VAO_N,VBO_N,EBO_N;
    glGenVertexArrays(1,&VAO_N); glGenBuffers(1,&VBO_N); glGenBuffers(1,&EBO_N);
    glBindVertexArray(VAO_N);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_N); glBufferData(GL_ARRAY_BUFFER,verticesN.size()*sizeof(Vertex),verticesN.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_N); glBufferData(GL_ELEMENT_ARRAY_BUFFER,indicesN.size()*sizeof(GLuint),indicesN.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0); glEnableVertexAttribArray(0);
    glVertexAttribIPointer(1,1,GL_INT,sizeof(Vertex),(void*)(2*sizeof(float))); glEnableVertexAttribArray(1);

    GLuint VAO_T,VBO_T,EBO_T;
    glGenVertexArrays(1,&VAO_T); glGenBuffers(1,&VBO_T); glGenBuffers(1,&EBO_T);
    glBindVertexArray(VAO_T);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_T); glBufferData(GL_ARRAY_BUFFER,verticesT.size()*sizeof(Vertex),verticesT.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_T); glBufferData(GL_ELEMENT_ARRAY_BUFFER,indicesT.size()*sizeof(GLuint),indicesT.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0); glEnableVertexAttribArray(0);
    glVertexAttribIPointer(1,1,GL_INT,sizeof(Vertex),(void*)(2*sizeof(float))); glEnableVertexAttribArray(1);

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    GLuint framebuffer, textureColorbuffer;
    glGenFramebuffers(1,&framebuffer); glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    glGenTextures(1,&textureColorbuffer); glBindTexture(GL_TEXTURE_2D,textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,logicalWidth,logicalHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textureColorbuffer,0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    float quadVertices[] = {
        -1,-1,0,0,
         1,-1,1,0,
         1, 1,1,1,
        -1, 1,0,1
    };
    unsigned int quadIndices[]={0,1,2,0,2,3};
    GLuint VAO_quad,VBO_quad,EBO_quad;
    glGenVertexArrays(1,&VAO_quad); glGenBuffers(1,&VBO_quad); glGenBuffers(1,&EBO_quad);
    glBindVertexArray(VAO_quad);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_quad); glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_quad); glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(quadIndices),quadIndices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float))); glEnableVertexAttribArray(1);

    auto updateMVP=[&](float tx,float ty,float angle,float scale){
        int winW,winH; SDL_GetWindowSize(window,&winW,&winH);
        float aspect=(float)winW/(float)winH;
        float orthoSize=1.5f;
        glm::mat4 model=glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,0.0f));
        model=glm::rotate(model,glm::radians(angle),glm::vec3(0,0,1));
        model=glm::scale(model,glm::vec3(scale,scale,1.0f));
        glm::mat4 view=glm::mat4(1.0f);
        glm::mat4 proj=glm::ortho(-orthoSize*aspect,orthoSize*aspect,-orthoSize,orthoSize,-1.0f,1.0f);
        return proj*view*model;
    };

    bool running=true; SDL_Event event; Uint32 startTime=SDL_GetTicks();
    while(running){
        Uint32 frameStart=SDL_GetTicks();
        float time=(frameStart-startTime)/1000.0f;

        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT) running=false;
            else if(event.type==SDL_WINDOWEVENT && event.window.event==SDL_WINDOWEVENT_RESIZED){
                int winW,winH; SDL_GetWindowSize(window,&winW,&winH); glViewport(0,0,winW,winH);
            } else if(event.type==SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_F1){ logicalWidth=std::max(MIN_RESOLUTION,logicalWidth-50); logicalHeight=logicalWidth; glBindTexture(GL_TEXTURE_2D,textureColorbuffer); glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,logicalWidth,logicalHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);}
                else if(event.key.keysym.sym==SDLK_F2){ logicalWidth=std::min(MAX_RESOLUTION,logicalWidth+50); logicalHeight=logicalWidth; glBindTexture(GL_TEXTURE_2D,textureColorbuffer); glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,logicalWidth,logicalHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);}
                else if(event.key.keysym.sym==SDLK_PLUS||event.key.keysym.sym==SDLK_KP_PLUS) global_alpha=std::min(global_alpha+0.1f,1.0f);
                else if(event.key.keysym.sym==SDLK_MINUS||event.key.keysym.sym==SDLK_KP_MINUS) global_alpha=std::max(global_alpha-0.1f,0.1f);
            } else if(event.type==SDL_MOUSEBUTTONDOWN){ if(event.button.button==SDL_BUTTON_LEFT){ magnifier_active=true; magnifier_x=event.button.x; magnifier_y=event.button.y;} }
            else if(event.type==SDL_MOUSEBUTTONUP){ if(event.button.button==SDL_BUTTON_LEFT) magnifier_active=false; }
            else if(event.type==SDL_MOUSEMOTION){ if(magnifier_active){ magnifier_x=event.motion.x; magnifier_y=event.motion.y; } }
        }

        global_angle+=rotation_speed; if(global_angle>360.0f) global_angle-=360.0f;

        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glViewport(0,0,logicalWidth,logicalHeight);
        glClearColor(0,0,0,1); glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(letterProgram);
        glUniform1f(glGetUniformLocation(letterProgram,"time"),time);
        glUniform1f(glGetUniformLocation(letterProgram,"alpha"),global_alpha);
        glUniform3f(glGetUniformLocation(letterProgram,"letterColor"),200,200,33);

        float scaleN=0.3f+0.2f*sin(time*2.0f);
        glm::mat4 mvpN=updateMVP(-0.5f,0.0f,time*90.0f,scaleN);
        glUniformMatrix4fv(glGetUniformLocation(letterProgram,"MVP"),1,GL_FALSE,glm::value_ptr(mvpN));
        glBindVertexArray(VAO_N); glDrawElements(GL_TRIANGLES,indicesN.size(),GL_UNSIGNED_INT,0);

        float scaleT=0.3f+0.2f*cos(time*2.0f);
        glm::mat4 mvpT=updateMVP(0.5f,0.0f,-time*90.0f,scaleT);
        glUniformMatrix4fv(glGetUniformLocation(letterProgram,"MVP"),1,GL_FALSE,glm::value_ptr(mvpT));
        glBindVertexArray(VAO_T); glDrawElements(GL_TRIANGLES,indicesT.size(),GL_UNSIGNED_INT,0);

        int winW,winH; SDL_GetWindowSize(window,&winW,&winH);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,winW,winH);
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка сцены без лупы
        glUseProgram(magnifierProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureColorbuffer);
        glUniform1i(glGetUniformLocation(magnifierProgram,"sceneTex"),0);
        glUniform2f(glGetUniformLocation(magnifierProgram,"center"),0.5f,0.5f);
        glUniform1f(glGetUniformLocation(magnifierProgram,"radius"),0.0f);
        glUniform1f(glGetUniformLocation(magnifierProgram,"zoom"),1.0f);
        glUniform2f(glGetUniformLocation(magnifierProgram,"screenAspect"), 1.0f, (float)winH / (float)winW);
        glBindVertexArray(VAO_quad); glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // Отрисовка лупы (если активна)
        if(magnifier_active){
            glUseProgram(magnifierProgram);
            glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,textureColorbuffer);
            glUniform1i(glGetUniformLocation(magnifierProgram,"sceneTex"),0);

            float normX = magnifier_x / (float)winW;
            float normY = 1.0f - magnifier_y / (float)winH; // Y перевёрнут в OpenGL
            glUniform2f(glGetUniformLocation(magnifierProgram,"center"), normX, normY);

            float normalizedRadius = magnifier_radius / (float)std::min(winW, winH);
            glUniform1f(glGetUniformLocation(magnifierProgram,"radius"), normalizedRadius);

            glUniform1f(glGetUniformLocation(magnifierProgram,"zoom"), magnifier_zoom);
            glUniform2f(glGetUniformLocation(magnifierProgram,"screenAspect"), 1.0f, (float)winH / (float)winW);

            glBindVertexArray(VAO_quad); glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        }

        SDL_GL_SwapWindow(window);
        Uint32 frameTime=SDL_GetTicks()-frameStart;
        if(frameTime<16) SDL_Delay(16-frameTime);
    }

    glDeleteVertexArrays(1,&VAO_N); glDeleteBuffers(1,&VBO_N); glDeleteBuffers(1,&EBO_N);
    glDeleteVertexArrays(1,&VAO_T); glDeleteBuffers(1,&VBO_T); glDeleteBuffers(1,&EBO_T);
    glDeleteVertexArrays(1,&VAO_quad); glDeleteBuffers(1,&VBO_quad); glDeleteBuffers(1,&EBO_quad);
    glDeleteProgram(letterProgram); glDeleteProgram(magnifierProgram);
    glDeleteFramebuffers(1,&framebuffer); glDeleteTextures(1,&textureColorbuffer);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}