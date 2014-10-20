/*==============================================================================
  BSPDemo
  Renderer.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

/*
========================================
  dxRenderer
========================================
*/

#define DX_FOVY 70.0

dxRenderer renderer;

dxRenderer::dxRenderer()
{
    inited = false;

    numtextures = 0;
    viewratio = 1.0;
    enable_alphatest = false;

    texture_text = 0;

    int count = 0;

    for (float y = 128.0f; y > 16.0f; y-=16.0f)
    {
        for (float x = 0.0f; x < 112.0f; x+= 8.0f)
        {
            chartexcoords[count].s1 =  x / 128.0f;
            chartexcoords[count].t1 = (y-16.0f) / 128.0f;
            chartexcoords[count].s2 = (x+8.0f) / 128.0f;
            chartexcoords[count].t2 =  y / 128.0f;

            count++;

            if (count >= 95)
            {
                return;
            }
        }
    }
}

dxRenderer::~dxRenderer()
{
    Shutdown();
}

void dxRenderer::Init()
{
    if (inited)
    {
        return;
    }

    inited = true;

    dxglClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    dxglClearDepth(1.0);
    dxglEnable(GL_DEPTH_TEST);
    dxglDepthFunc(GL_LEQUAL);
    dxglAlphaFunc(GL_GREATER, 0.8f);
    dxglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    dxglCullFace(GL_BACK);
    dxglFrontFace(GL_CW);
    dxglEnable(GL_CULL_FACE);

    dxglDrawBuffer(GL_BACK);

    LoadTextTexture();
    LoadPrograms();

    SetColorMappings(1.5f);
}

void dxRenderer::Shutdown()
{
    if (!inited)
    {
        return;
    }

    inited = false;

    SetColorMappings(1.0f);

    UnloadTextures();

    GL_UnloadTempTexture(texture_text);
    texture_text = 0;

    GL_DestroyProg(program_alpha);
    GL_DestroyProg(program_opaque);

    GL_DestroyVertexBuffer(vertbuf_sky);
    GL_DestroyVertexBuffer(texcoordbuf_sky);
    GL_DestroyProg(program_sky);
}

void dxRenderer::SetViewport(int viewwidth_, int viewheight_)
{
    viewwidth = viewwidth_;
    viewheight = viewheight_;

    if (viewheight < 1)
    {
        viewheight = 1;
    }

    viewratio = viewwidth / (double)viewheight;
}

//texture

GLuint dxRenderer::LoadMipTexture(miptex_s * mt)
{
    if (mt->offsets[0] > 0 && mt->offsets[3] > 0)
    {
        int w = mt->width  >> 3;
        int h = mt->height >> 3;

        byte * pal = (byte*)mt + mt->offsets[3] + w * h + sizeof(short);
        byte * data = (byte*)(mt+1);

        return GL_LoadTexture8(mt->name, mt->width, mt->height, pal, data, true, GL_REPEAT);
    }
    return 0;
}

const byte * dxRenderer::LoadBMPImage(const char * filename, int &image_cx, int &image_cy)
{
    #define ALIGN4(Bytes) ((Bytes + 3) & ~3)

	static byte dst[DX_MAX_TEXTURE_SIZE * DX_MAX_TEXTURE_SIZE * 4]; //static allocate

	BITMAPFILEHEADER file_header;
	BITMAPINFOHEADER info_header;

	RGBQUAD RGBquad[256]; //for 8bit bmp map

	FILE *f = NULL;

	fopen_s(&f, filename, "rb");
	if (!f)
	{
		return NULL;
	}

	fread(&file_header, sizeof(file_header), 1, f);
	fread(&info_header, sizeof(info_header), 1, f);

	image_cx = info_header.biWidth;
	image_cy = info_header.biHeight;

	if (image_cx > DX_MAX_TEXTURE_SIZE || image_cy > DX_MAX_TEXTURE_SIZE)
	{
		fclose(f);
		Sys_Error("texture is too large");
	}

	int imagelinebytes = 0;
	int bytes_per_clr = 0;

    if (info_header.biBitCount == 32)
    {
        imagelinebytes = ALIGN4(image_cx * 4);
        bytes_per_clr = 4;
    }
	else if (info_header.biBitCount == 24)
	{
		imagelinebytes = ALIGN4(image_cx * 3);
		bytes_per_clr = 3;
	} 
	else if (info_header.biBitCount == 16)
	{
		imagelinebytes = ALIGN4(image_cx * 2);
		bytes_per_clr = 2;
	}
	else if (info_header.biBitCount == 8)
	{
		imagelinebytes = ALIGN4(image_cx);
		bytes_per_clr = 1;

		byte pal[256 * 4];
		fread(pal, sizeof(pal), 1, f);

		for (int i = 0; i < 256; i++)
		{
			RGBquad[i].rgbBlue     = pal[i * 4 + 0];
			RGBquad[i].rgbGreen    = pal[i * 4 + 1];
			RGBquad[i].rgbRed      = pal[i * 4 + 2];
			RGBquad[i].rgbReserved = 0;
		}
	}

	if (imagelinebytes == 0)
	{
		//unsupported format
		fclose(f);
		Sys_Error("bad bmp file %s", filename);
	}

	byte *src = (byte*)DX_MEM_ALLOC(image_cy * imagelinebytes);

	size_t nReadCount = fread(src, sizeof(byte), image_cy * imagelinebytes, f);

	fclose(f);

	if (nReadCount != image_cy * imagelinebytes)
	{
		DX_MEM_FREE(src);
		Sys_Error("bad bmp file data %s", filename);
	}

	//read image

	for (int y = 0; y < image_cy; y++)
	{
		byte *srcline = src + y * imagelinebytes; //bmp files are stored bottom-up, never need reverse y
		byte *dstline = dst + y * image_cx * 4;

		for (int x = 0; x < image_cx; x++)
		{
			byte *srcclr = srcline + x * bytes_per_clr;
			byte *dstclr = dstline + x * 4;

            if (bytes_per_clr == 4)
            {
                dstclr[0] = srcclr[2]; //r
                dstclr[1] = srcclr[1]; //g
                dstclr[2] = srcclr[0]; //b
                dstclr[3] = srcclr[3]; //a
            }
			else if (bytes_per_clr == 3)
			{
				dstclr[0] = srcclr[2]; //r
				dstclr[1] = srcclr[1]; //g
				dstclr[2] = srcclr[0]; //b
                dstclr[3] = 255;
			}
			else if (bytes_per_clr == 2)
			{
				unsigned short clr = *(unsigned short *)srcclr;

				dstclr[0] = (clr & 0x001f);       //r
				dstclr[1] = (clr & 0x07e0) >> 5;  //g
				dstclr[2] = (clr & 0xf800) >> 11; //b
                dstclr[3] = 255;
			}
			else //1
			{
				dstclr[0] = RGBquad[*srcclr].rgbRed;
				dstclr[1] = RGBquad[*srcclr].rgbGreen;
				dstclr[2] = RGBquad[*srcclr].rgbBlue;
                dstclr[3] = 255;
			}
		}
	}

	DX_MEM_FREE(src);

	return dst;
}

GLuint dxRenderer::LoadBMPTexture(char *identifier, const char * filename)
{
	int image_cx = 0;
	int image_cy = 0;
	const byte * data = LoadBMPImage(filename, image_cx, image_cy);

	return GL_LoadTexture32(identifier, image_cx, image_cy, data, false, GL_CLAMP_TO_EDGE);
}

lightmap_s * dxRenderer::AllocLightmapBlock(int extendx, int extendy, int &light_s, int &light_t)
{
    int w = extendx;
    int h = extendy;

    for (int l = 0; l < MAX_LIGHTMAPS; l++)
    {
        lightmap_s * map = lightmaps + l;
        int best = BLOCK_HEIGHT;
        int x = 0;

        for (int i = 0; i < BLOCK_WIDTH-w; i++)
        {
            int best2 = 0;
            int j = 0;

            for (j = 0; j < w; j++)
            {
                if (map->allocated[i+j] >= best)
                {
                    break;
                }

                if (map->allocated[i+j] > best2)
                {
                    best2 = map->allocated[i+j];
                }
            }

            if (j == w)
            {
                light_s = x = i;
                light_t = best = best2;
            }
        }

        if (best + h > BLOCK_HEIGHT)
        {
            continue;
        }

        for (int i = 0; i < w; i++)
        {
            map->allocated[x+i] = best + h;
        }

        map->used = true;
        return map;
    }

    Sys_Error("AllocBlock: full");
}

void dxRenderer::LoadLightmaps()
{
    for (int i = 0; i < MAX_LIGHTMAPS; i++)
    {
        if (!lightmaps[i].used)
        {
            lightmaps[i].texnum = (GLuint)-1;
            continue;
        }

        char texname[DX_ID_LEN];
        sprintf_s(texname, DX_ID_LEN, "lightmap%d", i+1);

        lightmaps[i].texnum = GL_LoadTexture32(texname, BLOCK_WIDTH, BLOCK_HEIGHT, (byte*)lightmaps[i].buffer, true, GL_CLAMP_TO_EDGE);
    }
}

void dxRenderer::ResetLightmaps()
{
    memset(lightmaps, 0, sizeof(lightmaps));
}

void dxRenderer::UnloadTextures()
{
    GL_ClearTextures();
    GL_BindTexture(0);
}

GLuint dxRenderer::LoadTempBMPTexture(const char * filename)
{
	int image_cx = 0;
	int image_cy = 0;
	const byte * data = LoadBMPImage(filename, image_cx, image_cy);

	return GL_LoadTempTexture32(image_cx, image_cy, data, false, GL_CLAMP_TO_EDGE);
}

void dxRenderer::UnloadTempBMPTexture(GLuint texnum)
{
	GL_UnloadTempTexture(texnum);
	GL_BindTexture(0);
}

//vertex buffer
GLuint dxRenderer::CreateVertexBufferVec2(const void * data, GLsizei count)
{
    return GL_CreateVertexBuffer(data, count * sizeof(dxVec2));
}

GLuint dxRenderer::CreateVertexBufferVec3(const void * data, GLsizei count)
{
    return GL_CreateVertexBuffer(data, count * sizeof(dxVec3));
}

void dxRenderer::DestroyVertexBuffer(GLuint &buffer)
{
    GL_DestroyVertexBuffer(buffer);
}

void dxRenderer::BeginDraw()
{
    dxglViewport(0, 0, viewwidth, viewheight);
    dxglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    dxglPolygonMode(GL_FRONT_AND_BACK, debugvar.fillmode ? GL_FILL : GL_LINE);
}

void dxRenderer::EndDraw()
{
    dxglFinish();
    GL_SwapBuffers();
}

void dxRenderer::SetupTextMode()
{
    dxglMatrixMode(GL_PROJECTION);
    dxglLoadIdentity();
    dxglOrtho(-(viewwidth >> 1), viewwidth >> 1, -(viewheight >> 1), viewheight >> 1, 1, -1);

    dxglMatrixMode(GL_MODELVIEW);
    dxglLoadIdentity();

    dxglEnable(GL_BLEND);
    dxglDisable(GL_DEPTH_TEST);
    dxglDisable(GL_CULL_FACE);
    dxglDepthMask(GL_FALSE);
}

void dxRenderer::EndTextMode()
{
    dxglDepthMask(GL_TRUE);
    dxglEnable(GL_CULL_FACE);
    dxglEnable(GL_DEPTH_TEST);
    dxglDisable(GL_BLEND);
}

void dxRenderer::FrameRect(const dxVec4 &color, int left, int bottom, int right, int top)
{
    dxglColor4fv(color);

    dxglBegin(GL_LINE_LOOP);

        dxglVertex2i(left,  bottom);
        dxglVertex2i(right, bottom);
        dxglVertex2i(right, top);
        dxglVertex2i(left,  top);

    dxglEnd();
}

void dxRenderer::FillRect(const dxVec4 &color, int left, int bottom, int right, int top)
{
    dxglColor4fv(color);

    dxglBegin(GL_QUADS);

        dxglVertex2i(left,  bottom);
        dxglVertex2i(right, bottom);
        dxglVertex2i(right, top);
        dxglVertex2i(left,  top);

    dxglEnd();
}

void dxRenderer::Print(const dxVec4 &color, int x, int y, const char * str)
{
	dxglActiveTexture(GL_TEXTURE0);
    dxglEnable(GL_TEXTURE_2D);

	GL_BindTexture(texture_text);

    dxglColor4fv(color);

    dxglBegin(GL_QUADS);

    const char *pc = str;
    while (*pc)
    {
        char c = *pc;

        if (c < 32)
        {
            pc++;
            continue;
        }

        int index = c - 32;

        chartexcoord_s * ct = chartexcoords + index;

        dxglTexCoord2f(ct->s1, ct->t1);
        dxglVertex2i(x, y);

        dxglTexCoord2f(ct->s2, ct->t1);
        dxglVertex2i(x+8, y);

        dxglTexCoord2f(ct->s2, ct->t2);
        dxglVertex2i(x+8, y+16);

        dxglTexCoord2f(ct->s1, ct->t2);
        dxglVertex2i(x, y+16);

        x += 8;

        pc++;
    }

    dxglEnd();

    dxglDisable(GL_TEXTURE_2D);
}

void dxRenderer::DrawLine(const dxVec4 &color, int x0, int y0, int x1, int y1)
{
	dxglColor4fv(color);

	dxglBegin(GL_LINES);

	    dxglVertex2i(x0, y0);
	    dxglVertex2i(x1, y1);

	dxglEnd();
}

//stretch draw
void dxRenderer::DrawImage(GLuint image, int left, int bottom, int right, int top)
{
    dxglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    dxglDisable(GL_BLEND);
	dxglActiveTexture(GL_TEXTURE0);
    dxglEnable(GL_TEXTURE_2D);
    GL_BindTexture(image);

    dxglBegin(GL_QUADS);

        dxglTexCoord2f(0, 0);
        dxglVertex2i(left, bottom);

        dxglTexCoord2f(1, 0);
        dxglVertex2i(right, bottom);

        dxglTexCoord2f(1, 1);
        dxglVertex2i(right, top);

        dxglTexCoord2f(0, 1);
        dxglVertex2i(left, top);

    dxglEnd();

    dxglDisable(GL_TEXTURE_2D);
    dxglEnable(GL_BLEND);

    dxglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void dxRenderer::SetupFrame(const dxVec3 &vieworigin_, const dxVec3 &viewforward_,
    const dxVec3 &viewright_, const dxVec3 &viewup_)
{
    vieworigin = vieworigin_;

    dxglMatrixMode(GL_PROJECTION);
    dxglLoadIdentity();
    dxgluPerspective(DX_FOVY, viewratio, 1.0, 4096.0);

    dxglMatrixMode(GL_MODELVIEW);
    dxglLoadIdentity();

    dxgluLookAt(vieworigin[0], vieworigin[1], vieworigin[2],
        vieworigin[0] + viewforward_[0],
        vieworigin[1] + viewforward_[1],
        vieworigin[2] + viewforward_[2],
        viewup_[0], viewup_[1], viewup_[2]);

    //setup frustum
    dxMat3 m;
    float fovy = (float)DX_FOVY;
    float fovx = (float)DX_FOVY * (float)viewratio;

	dxMat4 matProj;
	dxMat4 matModelView;

	dxglGetFloatv(GL_PROJECTION_MATRIX, matProj);
	dxglGetFloatv(GL_MODELVIEW_MATRIX,  matModelView);

	matMVP = matProj * matModelView;

    //left
    Mat3Rotation(-(90.0f - fovx * 0.5f), viewup_, m);
    Vec3Transform(viewforward_, m, frustum[0].normal);
    frustum[0].dist = -Vec3DotProduct(vieworigin_, frustum[0].normal);

    //right
    Mat3Rotation((90.0f - fovx * 0.5f), viewup_, m);
    Vec3Transform(viewforward_, m, frustum[1].normal);
    frustum[1].dist = -Vec3DotProduct(vieworigin_, frustum[1].normal);

    //top
    Mat3Rotation(-(90.0f - fovy * 0.5f), viewright_, m);
    Vec3Transform(viewforward_, m, frustum[2].normal);
    frustum[2].dist = -Vec3DotProduct(vieworigin_, frustum[2].normal);

    //bottom
    Mat3Rotation((90.0f - fovy * 0.5f), viewright_, m);
    Vec3Transform(viewforward_, m, frustum[3].normal);
    frustum[3].dist = -Vec3DotProduct(vieworigin_, frustum[3].normal);
}

bool dxRenderer::CullBox(float *mins, float *maxs)
{
    for (int i = 0; i < 4; i++)
    {
        if (frustum[i].BoxOnPlaneSide(mins, maxs) == 2)
        {
            return true;
        }
    }

    return false;
}

void dxRenderer::BeginDrawSky()
{
	dxglDisable(GL_CULL_FACE);
	dxglDepthMask(GL_FALSE);

    dxglUseProgram(program_sky.prgnum);

	dxglUniformMatrix4fv(uniform_skyMatMVP, 1, GL_FALSE, matMVP);
    dxglUniform1i(uniform_skytex,  0);
    dxglUniform3f(uniform_skyviewpos, vieworigin[0], vieworigin[1], vieworigin[2]);

    dxglEnableVertexAttribArray(0);
    dxglBindBuffer(GL_ARRAY_BUFFER, vertbuf_sky);
    dxglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    dxglEnableVertexAttribArray(1);
    dxglBindBuffer(GL_ARRAY_BUFFER, texcoordbuf_sky);
    dxglVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
}

void dxRenderer::EndDrawSky()
{
    dxglDisableVertexAttribArray(1);
    dxglDisableVertexAttribArray(0);

    dxglUseProgram(0);

    dxglDepthMask(GL_TRUE);
    dxglEnable(GL_CULL_FACE);
}

void dxRenderer::BeginDrawOpaque(GLuint vertbuf_model, GLuint texcoord1_model, GLuint texcoord2_model, bool alphatest)
{
    enable_alphatest = alphatest;

    if (enable_alphatest)
    {
        dxglEnable(GL_ALPHA_TEST);
    }

    dxglUseProgram(program_opaque.prgnum);

	dxglUniformMatrix4fv(uniform_opaqueMatMVP, 1, GL_FALSE, matMVP);
    dxglUniform1i(uniform_surftex_opaque,  0);
    dxglUniform1i(uniform_lightmap_opaque, 1);

    dxglEnableVertexAttribArray(0);
    dxglBindBuffer(GL_ARRAY_BUFFER, vertbuf_model);
    dxglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    dxglEnableVertexAttribArray(1);
    dxglBindBuffer(GL_ARRAY_BUFFER, texcoord1_model);
    dxglVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    dxglEnableVertexAttribArray(2);
    dxglBindBuffer(GL_ARRAY_BUFFER, texcoord2_model);
    dxglVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
}

void dxRenderer::EndDrawOpaque()
{
    dxglDisableVertexAttribArray(2);
    dxglDisableVertexAttribArray(1);
    dxglDisableVertexAttribArray(0);

    dxglUseProgram(0);

    if (enable_alphatest)
    {
        dxglDisable(GL_ALPHA_TEST);
    }
}

void dxRenderer::BeginDrawOpaque2(GLuint vertbuf_model, GLuint texcoord1_model, bool alphatest)
{
	enable_alphatest = alphatest;

	if (enable_alphatest)
	{
		dxglEnable(GL_ALPHA_TEST);
	}

	dxglUseProgram(program_opaque2.prgnum);

	dxglUniformMatrix4fv(uniform_opaque2MatMVP, 1, GL_FALSE, matMVP);
	dxglUniform1i(uniform_surftex_opaque2, 0);

	dxglEnableVertexAttribArray(0);
	dxglBindBuffer(GL_ARRAY_BUFFER, vertbuf_model);
	dxglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	dxglEnableVertexAttribArray(1);
	dxglBindBuffer(GL_ARRAY_BUFFER, texcoord1_model);
	dxglVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
}

void dxRenderer::EndDrawOpaque2()
{
	dxglDisableVertexAttribArray(1);
	dxglDisableVertexAttribArray(0);

	dxglUseProgram(0);

	if (enable_alphatest)
	{
		dxglDisable(GL_ALPHA_TEST);
	}
}

void dxRenderer::BeginDrawAlpha(GLuint vertbuf_model, GLuint texcoord_model)
{
    dxglEnable(GL_BLEND);
    dxglDepthMask(GL_FALSE);
	//dxglDisable(GL_CULL_FACE);

    dxglUseProgram(program_alpha.prgnum);

	dxglUniformMatrix4fv(uniform_alphaMatMVP, 1, GL_FALSE, matMVP);
	dxglUniform1i(uniform_surftex_alpha, 0);

    dxglEnableVertexAttribArray(0);
    dxglBindBuffer(GL_ARRAY_BUFFER, vertbuf_model);
    dxglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    dxglEnableVertexAttribArray(1);
    dxglBindBuffer(GL_ARRAY_BUFFER, texcoord_model);
    dxglVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
}

void dxRenderer::EndDrawAlpha()
{
    dxglDisableVertexAttribArray(1);
    dxglDisableVertexAttribArray(0);

    dxglUseProgram(0);

	//dxglEnable(GL_CULL_FACE);
    dxglDepthMask(GL_TRUE);
    dxglDisable(GL_BLEND);
}

void dxRenderer::DrawQuad(GLuint polytex, GLint offset)
{
    dxglActiveTexture(GL_TEXTURE0);
    GL_BindTexture(polytex);
    dxglDrawArrays(GL_QUADS, offset, 4);
}

void dxRenderer::DrawPolygonOpaque(GLuint polytex, GLuint lighttex, GLint offset, GLsizei count)
{
    dxglActiveTexture(GL_TEXTURE0);
    GL_BindTexture(polytex);
    dxglActiveTexture(GL_TEXTURE1);
    GL_BindTexture(lighttex);
    dxglDrawArrays(GL_POLYGON, offset, count);
}

void dxRenderer::DrawPolygonOpaque2(GLuint polytex, GLint offset, GLsizei count)
{
	dxglActiveTexture(GL_TEXTURE0);
	GL_BindTexture(polytex);
	dxglDrawArrays(GL_POLYGON, offset, count);
}

void dxRenderer::DrawPolygonAlpha(GLuint polytex, float alpha, float texoffset, GLint offset, GLsizei count)
{
    dxglUniform1f(uniform_alpha, alpha);
    dxglUniform1f(uniform_offset, texoffset);

    dxglActiveTexture(GL_TEXTURE0);
    GL_BindTexture(polytex);
    dxglDrawArrays(GL_POLYGON, offset, count);
}

const byte CHAR_IMAGE[128 * 128] = 
{
#include "Characters.txt"
};

void dxRenderer::LoadTextTexture()
{
    byte image_reverse[128 * 128 * 4];

    for (int y = 0; y < 128; y++)
    {
        const byte * src = CHAR_IMAGE + y * 128;
        byte * dst = image_reverse + (128 - y - 1) * 128 * 4;

        for (int x = 0; x < 128; x++)
        {
            byte s = src[x];

            dst[0] = s;
            dst[1] = s;
            dst[2] = s;
            dst[3] = s;

            dst += 4;
        }
    }

    texture_text = GL_LoadTempTexture32(128, 128, image_reverse, false, GL_CLAMP_TO_EDGE);
}

void dxRenderer::LoadPrograms()
{
    //sky shader
    static const char * VS_SKY [] =
    {
        "#version 140                                                  \n"

        "uniform vec3 viewPos;                                         \n"
		"uniform mat4 matMVP;                                          \n"

        "in  vec4 vertPos;                                             \n"
        "in  vec2 texCoordIn;                                          \n"
        "out vec2 texCoord;                                            \n"

        "void main()                                                   \n"
        "{                                                             \n"
        "  vec4 pos    = vertPos + vec4(viewPos, 0.0f);                \n"
        "  gl_Position = matMVP * pos;                                 \n"
        "  texCoord    = texCoordIn;                                   \n"
        "}"
    };

    static const char * FS_SKY [] =
    {
        "#version 140                                                  \n"

        "uniform sampler2D skyTex;                                     \n" 

        "in vec2 texCoord;                                             \n"

        "void main()                                                   \n"
        "{                                                             \n"
        "    gl_FragColor = texture(skyTex, texCoord);                 \n"
        "}"
    };

    //opaque shader
	static const char * VS_OPAQUE [] =
	{
		"#version 140                                                  \n"

		"uniform mat4 matMVP;                                          \n"

		"in  vec4 vertPos;                                             \n"
		"in  vec2 texCoordIn1;                                         \n"
		"in  vec2 texCoordIn2;                                         \n"
		"out vec2 texCoord1;                                           \n"
		"out vec2 texCoord2;                                           \n"

		"void main()                                                   \n"
		"{                                                             \n"
		"    gl_Position = matMVP * vertPos;                           \n"
		"    texCoord1   = texCoordIn1;                                \n"
		"    texCoord2   = texCoordIn2;                                \n"
		"}"
	};

    static const char * FS_OPAQUE [] =
    {
        "#version 140                                                  \n"

        "uniform sampler2D surfTex;                                    \n"
        "uniform sampler2D lightMap;                                   \n"

        "in vec2 texCoord1;                                            \n"
        "in vec2 texCoord2;                                            \n"

        "void main()                                                   \n"
        "{                                                             \n"
        "    vec4 fragColor0 = texture(surfTex,  texCoord1);           \n"
        "    vec4 fragColor1 = texture(lightMap, texCoord2);           \n"
        "    gl_FragColor = fragColor0 * fragColor1;                   \n"
        "}"
    };

	//opaque shader
	static const char * VS_OPAQUE2[] =
	{
		"#version 140                                                  \n"

		"uniform mat4 matMVP;                                          \n"

		"in  vec4 vertPos;                                             \n"
		"in  vec2 texCoordIn1;                                         \n"
		"out vec2 texCoord1;                                           \n"

		"void main()                                                   \n"
		"{                                                             \n"
		"    gl_Position = matMVP * vertPos;                           \n"
		"    texCoord1   = texCoordIn1;                                \n"
		"}"
	};

	static const char * FS_OPAQUE2[] =
	{
		"#version 140                                                  \n"

		"uniform sampler2D surfTex;                                    \n"

		"in vec2 texCoord1;                                            \n"

		"void main()                                                   \n"
		"{                                                             \n"
		"    vec4 fragColor0 = texture(surfTex,  texCoord1);           \n"
		"    gl_FragColor = fragColor0;                                \n"
		"}"
	};

    //alpha shader
    static const char * VS_ALPHA [] =
    {
        "#version 140                                                  \n"

        "uniform float offset;                                         \n"
		"uniform mat4  matMVP;                                         \n"

        "in  vec4 vertPos;                                             \n"
        "in  vec2 texCoordIn;                                          \n"
        "out vec2 texCoord;                                            \n"

        "void main()                                                   \n"
        "{                                                             \n"
        "  gl_Position = matMVP * vertPos;                             \n"
        "  texCoord.x  = texCoordIn.x + offset;                        \n"
        "  texCoord.y  = texCoordIn.y + offset;                        \n"
        "}"
    };

    static const char * FS_ALPHA [] =
    {
        "#version 140                                                  \n"

        "uniform float     alpha;                                      \n"
        "uniform sampler2D surfTex;                                    \n"

        "in vec2 texCoord;                                             \n"

        "void main()                                                   \n"
        "{                                                             \n"
        "    vec4 fragColor0 = texture(surfTex,  texCoord);            \n"
        "    gl_FragColor    = vec4(fragColor0.rgb, alpha);            \n"
        "}"
    };

    //sky program
    GL_CreateProg(VS_SKY, FS_SKY, program_sky);

    GL_BindAttribLocation(program_sky, 0, "vertPos");
    GL_BindAttribLocation(program_sky, 1, "texCoordIn");

    GL_LinkProg(program_sky);

    uniform_skyviewpos = GL_GetUniformLocation(program_sky, "viewPos");
    uniform_skytex     = GL_GetUniformLocation(program_sky, "skyTex");
	uniform_skyMatMVP  = GL_GetUniformLocation(program_sky, "matMVP");

    float vert_skybox[24][3] = 
    {
        {-16,  16, -16},{ 16,  16, -16},{ 16,  16,  16},{-16,  16,  16},//right
        { 16, -16, -16},{-16, -16, -16},{-16, -16,  16},{ 16, -16,  16},//left
        { 16,  16, -16},{ 16, -16, -16},{ 16, -16,  16},{ 16,  16,  16},//front
        {-16, -16, -16},{-16,  16, -16},{-16,  16,  16},{-16, -16,  16},//back
        {-16,  16,  16},{ 16,  16,  16},{ 16, -16,  16},{-16, -16,  16},//up
        {-16, -16, -16},{ 16, -16, -16},{ 16,  16, -16},{-16,  16, -16} //down
    };

    float texcoord_skybox[24][2] =
    {
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1}
    };

    vertbuf_sky     = GL_CreateVertexBuffer(vert_skybox, sizeof(vert_skybox));
    texcoordbuf_sky = GL_CreateVertexBuffer(texcoord_skybox, sizeof(texcoord_skybox));

    //opaque program
    GL_CreateProg(VS_OPAQUE, FS_OPAQUE, program_opaque);

    GL_BindAttribLocation(program_opaque, 0, "vertPos");
    GL_BindAttribLocation(program_opaque, 1, "texCoordIn1");
    GL_BindAttribLocation(program_opaque, 2, "texCoordIn2");

    GL_LinkProg(program_opaque);

	uniform_opaqueMatMVP    = GL_GetUniformLocation(program_opaque, "matMVP");
	uniform_surftex_opaque  = GL_GetUniformLocation(program_opaque, "surfTex");
	uniform_lightmap_opaque = GL_GetUniformLocation(program_opaque, "lightMap");

	GL_CreateProg(VS_OPAQUE2, FS_OPAQUE2, program_opaque2);

	GL_BindAttribLocation(program_opaque2, 0, "vertPos");
	GL_BindAttribLocation(program_opaque2, 1, "texCoordIn1");

	GL_LinkProg(program_opaque2);

	uniform_opaque2MatMVP = GL_GetUniformLocation(program_opaque2, "matMVP");
	uniform_surftex_opaque2 = GL_GetUniformLocation(program_opaque2, "surfTex");


    //alpha program
    GL_CreateProg(VS_ALPHA,  FS_ALPHA,  program_alpha);

    GL_BindAttribLocation(program_alpha, 0, "vertPos");
    GL_BindAttribLocation(program_alpha, 1, "texCoordIn");

    GL_LinkProg(program_alpha);

	uniform_alphaMatMVP    = GL_GetUniformLocation(program_alpha, "matMVP");
    uniform_surftex_alpha  = GL_GetUniformLocation(program_alpha, "surfTex");
    uniform_alpha          = GL_GetUniformLocation(program_alpha, "alpha");
    uniform_offset         = GL_GetUniformLocation(program_alpha, "offset");

}

void dxRenderer::SetColorMappings(float gamma)
{
    if (sysvar.hMainDC == NULL)
    {
        return;
    }

    unsigned short gammaTable[256];

    float b = 1.0f; //brightness
    float g = gamma; //gamma

    for (int i = 0; i < 256; i++ ) 
    {
        int inf;
        int j = (int)(i * b);

        if (j > 255) 
        {
            j = 255;
        }

        if ( g == 1 ) 
        {
            inf = (j<<8) | j;
        } 
        else 
        {
            (int)inf = (int)(0xffff * pow(j / 255.0f, 1.0f / g) + 0.5f);
        }

        if (inf < 0) 
        {
            inf = 0;
        }

        if (inf > 0xffff) 
        {
            inf = 0xffff;
        }

        gammaTable[i] = inf;
    }

    unsigned short table[3][256];

    for (int i = 0; i < 256; i++ ) 
    {
        table[0][i] = gammaTable[i];
        table[1][i] = gammaTable[i];
        table[2][i] = gammaTable[i];
    }

    SetDeviceGammaRamp(sysvar.hMainDC, table);
}
