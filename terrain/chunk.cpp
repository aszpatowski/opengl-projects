//
// Created by aleksander on 01.02.2022.
//

#pragma once
#ifndef CHUNK_H
#define CHUNK_H


#include <cstdlib>
#include <cstdio>

#include <cmath>


#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"
#include "zadanie6.hpp"

#define PI 3.14159265


class Chunk
{
private:
    static constexpr unsigned int SRTM_SIZE = 1201;

    unsigned int ShaderHandle_;

    unsigned int BufferLocation_;

    unsigned int vao;
    unsigned int positions;
    unsigned int indices;

    unsigned int tesselationOuter_;
    unsigned int tesselationInner_;

    unsigned int uProjectionMatrix_;
    unsigned int uModelViewMatrix_;
    unsigned int uLightPosition_;
    unsigned int uTesselationInnerLevel_;

    unsigned int uTesselationOuterLevel_;
    unsigned int uCameraPosition_;
    unsigned int uMVP_;

    unsigned int Pretesselation_;
    unsigned int uChunkPosition_;
    unsigned int uTextureSampler_;
    unsigned int uCartesian_;


    unsigned int patchCount;
    unsigned int pretesselationLevel;
    unsigned int uBackground_;

    unsigned int TextureId[8];

    bool background = false;

    glm::vec3 beginOffset;

    unsigned short int *mapHandle;
    unsigned int lod[8];

    void succ_uniform_ids()
    {

        uProjectionMatrix_ = glGetUniformLocation(ShaderHandle_, "Projection");
        uModelViewMatrix_ = glGetUniformLocation(ShaderHandle_, "Modelview");
        uLightPosition_ = glGetUniformLocation(ShaderHandle_, "LightPosition");
        uTesselationInnerLevel_ = glGetUniformLocation(ShaderHandle_, "TessLevelInner");
        uTesselationOuterLevel_ = glGetUniformLocation(ShaderHandle_, "TessLevelOuter");
        uCameraPosition_ = glGetUniformLocation(ShaderHandle_, "CameraPosition");
        uMVP_ = glGetUniformLocation(ShaderHandle_, "MVP");
        Pretesselation_ = glGetUniformLocation(ShaderHandle_, "preteselation");
        uChunkPosition_ = glGetUniformLocation(ShaderHandle_, "ChunkPosition");
        uTextureSampler_ = glGetUniformLocation(ShaderHandle_, "myTextureSampler");
        uBackground_ = glGetUniformLocation(ShaderHandle_, "background");
        uCartesian_ = glGetUniformLocation(ShaderHandle_, "cartesian");
    }

    void set_buffers(GLuint preteselation_level, unsigned short int *mapdata)
    {
        mapHandle = mapdata;
        if (mapdata != nullptr)
        {
            for (unsigned int id_lod = 0; id_lod < 8; ++id_lod)
            {
                lod[id_lod] = pow(2, id_lod);
                unsigned int N = (1200 + lod[id_lod] - 1) / lod[id_lod];
                unsigned int index = 0;
                {
                    float *important_pointer = (float *) malloc(1201 * 1201 * sizeof(float));
                    for (unsigned int i = 0; i < SRTM_SIZE - 1; i = i + lod[id_lod])
                    {
                        for (unsigned int j = 0; j < SRTM_SIZE - 1; j = j + lod[id_lod])
                        {
                            unsigned int index2 = i * (SRTM_SIZE - 1) + j;
                            important_pointer[index] = (float) (mapdata[index2]) / (255.0 * 255.0);
                            index++;
                        }
                    }
                    glGenTextures(1, &TextureId[id_lod]);
                    glBindTexture(GL_TEXTURE_2D, TextureId[id_lod]);

                    glTexImage2D(
                            GL_TEXTURE_2D,
                            0,
                            GL_R16F,
                            N,
                            N,
                            0,
                            GL_RED,
                            GL_FLOAT,
                            important_pointer
                    );


                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

                    free(important_pointer);
                }

            }
            std::cout << "Texture: " << TextureId << " ";
        }
        // pretesselate
        unsigned int tesselationFaces = preteselation_level * preteselation_level;
        unsigned int tesselationVerts = (preteselation_level + 1) * (preteselation_level);
        patchCount = tesselationFaces;
        pretesselationLevel = preteselation_level;

        int *faces;
        float *verts;
        faces = (int *) malloc(tesselationFaces * 4 * sizeof(int));
        verts = (float *) malloc(tesselationVerts * 4 * sizeof(float));

        double msx = 1.0;
        double msy = 1.0;

        if (mapdata == NULL)
        {
            msx = 100.0;
            msy = 100.0;
        }

        for (int i = 0; i < (preteselation_level + 1); ++i)
            for (int j = 0; j < (preteselation_level + 1); ++j)
            {
                verts[(preteselation_level + 1) * i + j + 0] =
                     (2.0 * msx) * (double(i) / (double) (preteselation_level + 1)) - msx;
                verts[(preteselation_level + 1) * i + j + 1] =
                     (2.0 * msy) * (double(j) / (double) (preteselation_level + 1)) - msy;
                verts[(preteselation_level + 1) * i + j + 2] = 0.0;
            }

        for (int i = 0; i < preteselation_level; ++i)
            for (int j = 0; j < preteselation_level; ++j)
            {
                faces[preteselation_level * i + j + 0] = preteselation_level * (i + 0) + (j + 0);
                faces[preteselation_level * i + j + 1] = preteselation_level * (i + 1) + (j + 0);
                faces[preteselation_level * i + j + 2] = preteselation_level * (i + 1) + (j + 1);
                faces[preteselation_level * i + j + 3] = preteselation_level * (i + 0) + (j + 1);
            }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &positions);
        glBindBuffer(GL_ARRAY_BUFFER, positions);
        glBufferData(GL_ARRAY_BUFFER, tesselationVerts * 4 * sizeof(float), verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(BufferLocation_);
        glVertexAttribPointer(BufferLocation_, 3, GL_FLOAT, GL_FALSE, sizeof(float), 0);

        glGenBuffers(1, &indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tesselationFaces * 4 * sizeof(int), faces, GL_STATIC_DRAW);


        if (mapHandle) free(mapHandle);
    }


public:

    void init(GLuint ShaderHandle, GLuint BufferLocation, GLuint TesselationInner, GLuint TesselationOuter,
              GLuint pretesselation_level, unsigned short int *mapData)
    {
        BufferLocation_ = BufferLocation;
        ShaderHandle_ = ShaderHandle;
        tesselationOuter_ = TesselationOuter;
        tesselationInner_ = TesselationInner;
        succ_uniform_ids();
        background = (mapData == NULL);

        set_buffers(pretesselation_level, mapData);
    }

    void setBeginOffset(glm::vec3 offset)
    {
        beginOffset = offset;
    }

    void
    draw(glm::vec3 cameraPos, glm::mat4 projectionMatrix, glm::mat4 modelViewMatrix, glm::mat4 MVP, unsigned int Lod,
         bool isLinear)
    {
        glUseProgram(ShaderHandle_);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, positions);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

        glUniformMatrix4fv(uMVP_, 1, GL_FALSE, &MVP[0][0]);
        glUniform1f(uTesselationInnerLevel_, tesselationInner_);
        glUniform1f(uTesselationOuterLevel_, tesselationOuter_);
        glUniform1i(Pretesselation_, pretesselationLevel);

        glUniform3f(uLightPosition_, 0.25, 0.25, 1.0);

        glUniform3f(uCameraPosition_, cameraPos[0], cameraPos[1], cameraPos[2]);

        glUniform3f(uChunkPosition_, beginOffset[0], beginOffset[1], beginOffset[2]);

        glUniformMatrix4fv(uProjectionMatrix_, 1, 0, &projectionMatrix[0][0]);
        glUniformMatrix4fv(uModelViewMatrix_, 1, 0, &modelViewMatrix[0][0]);

        if (viewMode == SPHERE)
            glUniform1i(uCartesian_, 0);
        else if (viewMode == TOPDOWN)
            glUniform1i(uCartesian_, 1);


        if (!background)
        {
            glBindTexture(GL_TEXTURE_2D, TextureId[Lod]);
            if (isLinear)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            } else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(uTextureSampler_, 0);
            glUniform1i(uBackground_, 0);
        } else
        {

            glUniform1i(uBackground_, 1);
        }


        /* wireframe */
        if (mapHandle == NULL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        /* how much vertrex on one patch */
        glPatchParameteri(GL_PATCH_VERTICES, 4);

        glDrawElements(GL_PATCHES, patchCount * 4, GL_UNSIGNED_INT, (void *) 0);
    }
};

#endif //CHUNK_HC