/*==============================================================================
  BSPDemo
  Model.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

/*
========================================
  dxModel
========================================
*/
dxModel worldModel;

dxModel::dxModel()
{
    memset(novis, 0xff, sizeof(novis));

    mod_base = NULL;

    numentities = 0;
    numepairs = 0;

    numvertices = 0;
    vertices = NULL;
    
    numedges = 0;
    edges = NULL;

    numsurfedges = 0;
    surfedges = NULL;

    lightdata = NULL;

    numtexinfo = 0;
    texinfo = NULL;

    numtextures = 0;
    textures = NULL;

    numplanes = 0;
    planes = NULL;

    numwadfiles = 0;

    numsurfaces = 0;
    surfaces = NULL;

    nummarksurfaces = 0;
    marksurfaces = NULL;

    visdata = NULL;

    numnodes = 0;
    nodes = NULL;

    numleafs = 0;
    visleafs = 0;
    leafs = NULL;

    numclipnodes = 0;
    clipnodes = NULL;

    numsubmodels = 0;
    submodels = NULL;

    memset(skytexture, 0, sizeof(skytexture));

    framecount = 0;
    visframecount = 0;
    viewleaf = oldviewleaf = NULL;

    loaded = false;
    loading_percent = 0;
}

dxModel::~dxModel()
{
    Free();
}

bool dxModel::IsLoaded() const
{
	return loaded;
}

void dxModel::Load(const char * filename)
{
    Free();

	FS_Change(FS_LOADING);

    loading_percent = 0;

    StrPool_Clear();

    DX_SAFE_FREE(mod_base);

    char fullfilename[MAX_PATH];
    sprintf_s(fullfilename, MAX_PATH, "%s\\maps\\%s.bsp", sysvar.basedir, filename);

    FILE * f = NULL;
    fopen_s(&f, fullfilename, "rb");

    if (!f)
    {
        Sys_Error("could not open file %s", fullfilename);
    }

    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    fseek(f, 0, SEEK_SET);

    mod_base = (char*)DX_MEM_ALLOC(filesize);
    if (!mod_base)
    {
        fclose(f);
        Sys_Error("could not alloc memory");
    }

    fread(mod_base, 1, filesize, f);
    fclose(f);

    dheader_s * header = (dheader_s*)mod_base;

    int v = Swap_LittleLong(header->version);
    if (v != BSPVERSION)
    {
        Sys_Error("bad bsp version");
    }

    for (int i = 0; i < HEADER_LUMPS; i++)
    {
        header->lumps[i].fileofs = Swap_LittleLong(header->lumps[i].fileofs);
        header->lumps[i].filelen = Swap_LittleLong(header->lumps[i].filelen);
    }

    UpdateLoadingPercent(6);

    //load entities first, we need wad file list
    LoadEntities(&header->lumps[LUMP_ENTITIES]);         UpdateLoadingPercent(12);
    LoadVertexes(&header->lumps[LUMP_VERTEXES]);         UpdateLoadingPercent(18);
    LoadEdges(&header->lumps[LUMP_EDGES]);               UpdateLoadingPercent(24);
    LoadSurfedges(&header->lumps[LUMP_SURFEDGES]);       UpdateLoadingPercent(30);
    LoadTextures(&header->lumps[LUMP_TEXTURES]);         UpdateLoadingPercent(36);
    LoadLighting(&header->lumps[LUMP_LIGHTING]);         UpdateLoadingPercent(42);
    LoadPlanes(&header->lumps[LUMP_PLANES]);             UpdateLoadingPercent(48);
    LoadTexinfo(&header->lumps[LUMP_TEXINFO]);           UpdateLoadingPercent(54);
    LoadFaces(&header->lumps[LUMP_FACES]);               UpdateLoadingPercent(60);
    LoadMarksurfaces(&header->lumps[LUMP_MARKSURFACES]); UpdateLoadingPercent(66);
    LoadVisibility(&header->lumps[LUMP_VISIBILITY]);     UpdateLoadingPercent(72);
    LoadLeafs(&header->lumps[LUMP_LEAFS]);               UpdateLoadingPercent(78);
    LoadNodes(&header->lumps[LUMP_NODES]);               UpdateLoadingPercent(84);
    LoadClipnodes(&header->lumps[LUMP_CLIPNODES]);       UpdateLoadingPercent(90);
    LoadSubmodels(&header->lumps[LUMP_MODELS]);          UpdateLoadingPercent(100);
    LoadSky();

    DX_SAFE_FREE(mod_base);

    visleafs = numleafs;
    if (numsubmodels > 0)
    {
        visleafs = submodels[0].visleafs;
    }

    InitViewer();

    loaded = true;

	FS_Change(FS_DEMO);
}

void dxModel::Free()
{
    if (!loaded)
    {
        return;
    }

    renderer.UnloadTextures();
    renderer.ResetLightmaps();

    renderer.DestroyVertexBuffer(texcoord2_model);
    renderer.DestroyVertexBuffer(texcoord1_model);
    renderer.DestroyVertexBuffer(vertbuf_model);

    numentities = 0;
    numepairs = 0;

    numvertices = 0;
    DX_SAFE_FREE(vertices);

    numedges = 0;
    DX_SAFE_FREE(edges);

    numsurfedges = 0;
    DX_SAFE_FREE(surfedges);

    DX_SAFE_FREE(lightdata);

    numtexinfo = 0;
    DX_SAFE_FREE(texinfo);

    for (int i = 0; i < numtextures; i++)
    {
        DX_MEM_FREE(textures[i]);
    }

    numtextures = 0;
    DX_SAFE_FREE(textures);

    numplanes = 0;
    DX_SAFE_FREE(planes);

    numwadfiles = 0;

    numsurfaces = 0;
    DX_SAFE_FREE(surfaces);

    nummarksurfaces = 0;
    DX_SAFE_FREE(marksurfaces);

    DX_SAFE_FREE(visdata);

    numnodes = 0;
    DX_SAFE_FREE(nodes);

    numleafs = 0;
    visleafs = 0;
    DX_SAFE_FREE(leafs);

    numclipnodes = 0;
    DX_SAFE_FREE(clipnodes);

    numsubmodels = 0;
    DX_SAFE_FREE(submodels);

    DX_SAFE_FREE(mod_base);

    memset(skytexture, 0, sizeof(skytexture));
    skyname[0] = 0;

    framecount = 0;
    visframecount = 0;
    viewleaf = oldviewleaf = NULL;

    loaded = false;
}

void dxModel::UpdateLoadingPercent(int percent)
{
    loading_percent = percent;
    Sys_UpdateScreen();
}

void dxModel::DrawLoading()
{
    const dxVec4 COLOR_PROGRESS = {0,0.5f,0,0.5f};
    const dxVec4 COLOR_FRAME    = {0,0.5f,0,1.0f};

    renderer.SetupTextMode();

    int rt = (sysvar.viewwidth >> 1) - 40;
    int lf = -rt;

    int progress = loading_percent * (rt - lf) / 100 + lf;

    renderer.FillRect(COLOR_PROGRESS, lf, -10, progress, 10);
    renderer.FrameRect(COLOR_FRAME, lf, -10, rt, 10);

    renderer.EndTextMode();
}

void dxModel::Draw(const dxVec3 &vieworigin_, const dxVec3 &viewforward_,
    const dxVec3 &viewright_, const dxVec3 &viewup_)
{
    if (!loaded)
    {
        return;
    }

    framecount++;

	viewleaf = PointInLeaf(vieworigin_);
    MarkLeaves();

    renderer.SetupFrame(vieworigin_, viewforward_, viewright_, viewup_);

    DrawSkyBox();
    DrawWorld();
    DrawEntitiesOnList();
}

void dxModel::InitWadFileList()
{
    numwadfiles = 0;

    if (numentities < 1)
    {
        return;
    }

    //first entity contain wad file list
    char * value = ED_ValueForKey(entities->epairs, "wad");
    if (!value)
    {
        return;
    }

	const char * p = value;
	const char * end = value + strlen(value);
	char * pp, *pf;

	char temp[4096];
	char *next_token = NULL;

	int n = 0;

	while (p < end)
	{
		strcpy_s(temp, sizeof(temp), p);
		if (strchr(temp, ';') == NULL)
		{
			strcat_s(temp, ";");
		}

		strtok_s(temp, ";", &next_token);

		//filt path
		pp = pf = temp;
		while (*pp != 0)
		{
			if (*pp == '/' || *pp == '\\')
			{
				pf = ++pp;
			}

			pp++;
		}

		strcpy_s(wadfiles[n], DX_ID_LEN, pf);

		p += strlen(temp) + 1;
		n++;
	}

	numwadfiles = n;
}

void dxModel::SetParent(mnode_s *node, mnode_s *parent)
{
    node->parent = parent;
    if (node->contents < 0)
    {
        return;
    }
    SetParent(node->children[0], node);
    SetParent(node->children[1], node);
}

void dxModel::LoadEntities(lump_s *l)
{
    numentities = 0;
    numepairs   = 0;

    char * entities_str = mod_base + l->fileofs;

    while (1)
    {
        if (!(entities_str = COM_Parse(entities_str)))
        {
            break;
        }

        if (strcmp(com_token, "{"))
        {
            break;
        }

        if (numentities >= MAX_MAP_ENTITIES)
        {
            Sys_Error("too many entities");
        }

        dxEntity * e = entities + numentities++;

        e->origin.Clear();
        e->angles.Clear();
        e->firstbrush = 0;
        e->numbrushes = 0;
        e->visframe = -1;
        e->model = -1;
        e->classtype = UNKNOWN;
        e->rendermode = 0;
        e->rendercolor.Clear();
        e->renderamt = 1.0f;
        e->renderfx = 0;
        e->epairs = NULL;

        bool break_while = false;

        while (1)
        {
            if (!(entities_str = COM_Parse(entities_str)))
            {
                break_while = true;
                break;
            }

            if (!strcmp(com_token, "}"))
            {
                break;
            }

            //parse epair
            if (MAX_EPAIRS == numepairs)
            {
                Sys_Error("too many epairs");
            }

            //alloc epair
            epair_s  * p = epairs + numepairs++;

            //set epair key
            p->key = StrPool_Copy(com_token);

            if (!(entities_str = COM_Parse(entities_str)))
            {
                Sys_Error("no pair value");
            }

            //set epair value
            p->value = StrPool_Copy(com_token);

            //link to pair list
            p->next = e->epairs;
            e->epairs = p;
        }

        if (break_while)
        {
            break;
        }
    }

    //init entity
    for (int i = 0; i < numentities; i++)
    {
        dxEntity * e = entities + i;

        char * value = ED_ValueForKey(e->epairs, "classname");

        if (value)
        {
            if (!strcmp(value, "worldspawn"))
            {
                e->classtype = WORLDSPAWN;
                ED_ParseSkyName(e->epairs, skyname);
            }
            else if (!strcmp(value, "func_bomb_target"))
            {
                e->classtype = FUNC_BOMB_TARGET;
				//to finish
            }
            else if (!strcmp(value, "func_buyzone"))
            {
                e->classtype = FUNC_BUYZONE;
                ED_ParseFuncBuyZone(e);
            }
            else if (!strcmp(value, "func_hostage_rescue"))
            {
                e->classtype = FUNC_HOSTAGE_RESCURE;
                ED_ParseFuncHostageRescue(e);
            }
            else if (!strcmp(value, "func_illusionary"))
            {
                e->classtype = FUNC_ILLUSIONARY;
                ED_ParseFuncIllusionary(e);
            }
            else if (!strcmp(value, "func_breakable"))
            {
                e->classtype = FUNC_BREAKABLE;
                ED_ParseFuncBreakable(e);
            }
            else if (!strcmp(value, "light_spot"))
            {
                e->classtype = LIGHT_SPOT;
				//to finish
            }
            else if (!strcmp(value, "trigger_multiple"))
            {
                e->classtype = TRIGGER_MULTIPLE;
				//to finish
            }
            else if (!strcmp(value, "func_water"))
            {
                e->classtype = FUNC_WATER;
				ED_ParseFuncWater(e);
            }
            else if (!strcmp(value, "func_wall"))
            {
                e->classtype = FUNC_WALL;
				ED_ParseFuncWall(e);
            }
            else if (!strcmp(value, "light_environment"))
            {
                e->classtype = LIGHT_ENVIRONMENT;
				//to finish
            }
            else if (!strcmp(value, "func_ladder"))
            {
                e->classtype = FUNC_LADDER;
                ED_ParseFuncLadder(e);
            }
            else if (!strcmp(value, "hostage_entity"))
            {
                e->classtype = HOSTAGE_ENTITY;
                ED_ParseHostageEntity(e);
            }
            else if (!strcmp(value, "info_player_deathmatch"))
            {
                e->classtype = INFO_PLAYER_DEATHMATCH;
                ED_ParseInfoPlayerDeathmatch(e);
            }
            else if (!strcmp(value, "info_player_start"))
            {
                e->classtype = INFO_PLAYER_START;
                ED_ParseInfoPlayerStart(e);
            }
            else if (!strcmp(value, "trigger_camera"))
            {
                e->classtype = TRIGGER_CAMERA;
				//to finish
            }
            else if (!strcmp(value, "info_target"))
            {
                e->classtype = INFO_TARGET;
				//to finish
            }
            else if (!strcmp(value, "ambient_generic"))
            {
                e->classtype = AMBIENT_GENERIC;
            }
            else if (!strcmp(value, "infodecal"))
            {
                e->classtype = INFODECAL;
				//to finish
            }
            else if (!strcmp(value, "func_button"))
            {
                e->classtype = FUNC_BUTTON;
                ED_ParseFuncButton(e);
            }
            else if (!strcmp(value, "func_door"))
            {
                e->classtype = FUNC_DOOR;
                ED_ParseFuncDoor(e);
            }
            else if (!strcmp(value, "func_door_rotating"))
            {
                e->classtype = FUNC_DOOR_ROTATING;
                ED_ParseFuncDoorRotating(e);
            }
            else if (!strcmp(value, "func_water"))
            {
                e->classtype = FUNC_WATER;
                ED_ParseFuncWater(e);
            }
            else if (!strcmp(value, "light"))
            {
                e->classtype = LIGHT;
                ED_ParseLight(e);
            }
            else if (!strcmp(value, "info_bomb_target"))
            {
                e->classtype = INFO_BOMB_TARGET;
                ED_ParseInfoBombTarget(e);
            }
            else if (!strcmp(value, "env_glow"))
            {
                e->classtype = ENV_GLOW;
                ED_ParseEnvGlow(e);
            }  
        }
    }

}

void dxModel::LoadVertexes(lump_s *l)
{
    struct dvertex_s
    {
        float point[3];
    };

    dvertex_s * in = (dvertex_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad vertex count");
    }

    int count = l->filelen / sizeof(*in);

    vertices = (dxVec3 *)DX_MEM_ALLOC(sizeof(dxVec3) * count);
    numvertices = count;

    dxVec3 * out = vertices;

    for (int i = 0; i < count; i++, in++, out++)
    {
        out->x = Swap_LittleFloat(in->point[0]);
        out->y = Swap_LittleFloat(in->point[1]);
        out->z = Swap_LittleFloat(in->point[2]);
    }
}

void dxModel::LoadEdges(lump_s *l)
{
    struct dedge_s
    {
        unsigned short v[2]; // vertex numbers
    };

    dedge_s * in = (dedge_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad edge count");
    }

    int count = l->filelen / sizeof(*in);

    edges = (medge_s *)DX_MEM_ALLOC(sizeof(medge_s) * count);
    numedges = count;

    medge_s * out = edges;	

    for (int i = 0; i < count; i++, in++, out++)
    {
        out->v[0] = (unsigned short)Swap_LittleShort(in->v[0]);
        out->v[1] = (unsigned short)Swap_LittleShort(in->v[1]);
    }
}

void dxModel::LoadSurfedges(lump_s *l)
{
    int * in = (int *)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad surface edge count");
    }

    int count = l->filelen / sizeof(*in);

    surfedges = (int*)DX_MEM_ALLOC(sizeof(int) * count); 
    numsurfedges = count;

    int * out = surfedges;

    for (int i = 0;i < count; i++)
    {
        out[i] = Swap_LittleLong(in[i]);
    }
}

void dxModel::LoadTextures(lump_s *l)
{
    struct dmiptexlump_s
    {
        int nummiptex;
        int dataofs[4]; // [nummiptex]
    };

    if (!l->filelen)
    {
        return;
    }

    dmiptexlump_s * m = (dmiptexlump_s*)(mod_base + l->fileofs);
    m->nummiptex = Swap_LittleLong(m->nummiptex);

    numtextures = m->nummiptex;
    textures = (texture_s**)DX_MEM_ALLOC(numtextures * sizeof(texture_s*));

    for (int i = 0; i < m->nummiptex; i++)
    {
        textures[i] = NULL;

        m->dataofs[i] = Swap_LittleLong(m->dataofs[i]);
        if (-1 == m->dataofs[i])
        {
            continue;
        }

        miptex_s * mt = (miptex_s*)((byte*)m + m->dataofs[i]);
        mt->width  = Swap_LittleLong(mt->width);
        mt->height = Swap_LittleLong(mt->height);

        for (int j = 0; j < MIPLEVELS; j++)
        {
            mt->offsets[j] = Swap_LittleLong(mt->offsets[j]);
        }

        if ((mt->width & 15) || (mt->height & 15))
        {
            Sys_Error("texture is not 16 aligned");
        }

        texture_s * tx = (texture_s*)DX_MEM_ALLOC(sizeof(texture_s));
        textures[i] = tx;
        memcpy(tx->name, mt->name, sizeof(tx->name));

        tx->width  = mt->width;
        tx->height = mt->height;
        tx->gl_texturenum = 0;

        if (strncmp(mt->name, "sky", 3)) //not sky
        {
            tx->gl_texturenum = renderer.LoadMipTexture(mt);
        }
    }

    //load wad textures
    InitWadFileList();

    for (int i = 0; i < numwadfiles; i++)
    {
        char filename[MAX_PATH];
        sprintf_s(filename, "%s\\%s", sysvar.basedir, wadfiles[i]);

        dxWad wad;

        if (wad.Load(filename))
        {
            for (int j = 0; j < numtextures; j++)
            {
                texture_s * tx = textures[j];
                if (!tx->gl_texturenum)
                {
                    miptex_s * mt = wad.GetLumpByName(tx->name);
                    if (mt)
                    {
                        tx->gl_texturenum = renderer.LoadMipTexture(mt);
                    }
                }
            }

            wad.Free();
        }
    }
}

void dxModel::LoadLighting(lump_s *l)
{
    if (!l->filelen)
    {
        return;
    }

    lightdata = (byte*)DX_MEM_ALLOC(l->filelen); 
    memcpy(lightdata, mod_base + l->fileofs, l->filelen);
}

void dxModel::LoadPlanes(lump_s *l)
{
    struct dplane_s
    {
        float	normal[3];
        float	dist;
        int		type; // PLANE_X - PLANE_ANYZ ?remove? trivial to regenerate
    };

    dplane_s * in = (dplane_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad plane count");
    }

    int count = l->filelen / sizeof(*in);

    planes = (mplane_s*)DX_MEM_ALLOC(sizeof(mplane_s) * count); 
    numplanes = count;

    mplane_s * out = planes;

    for (int i = 0; i < count; i++, in++, out++)
    {
        int bits = 0;
        for (int j = 0; j < 3; j++)
        {
            out->normal[j] = Swap_LittleFloat(in->normal[j]);
            if (out->normal[j] < 0)
            {
                bits |= 1<<j;
            }
        }

        out->dist = Swap_LittleFloat(in->dist);
        out->type = Swap_LittleLong(in->type);
        out->signbits = bits;
    }
}

void dxModel::LoadTexinfo(lump_s *l)
{
    struct texinfo_s
    {
        float vecs[2][4]; //[s/t][xyz offset]
        int   miptex;
        int   flags;
    };

    texinfo_s * in = (texinfo_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad texinfo count");
    }

    int count = l->filelen / sizeof(*in);

    texinfo = (mtexinfo_s*)DX_MEM_ALLOC(sizeof(mtexinfo_s) * count);
    numtexinfo = count;

    mtexinfo_s * out = texinfo;

    for (int i = 0; i < count; i++, in++, out++)
    {
        for (int j = 0; j < 8; j++)
        {
            out->vecs[0][j] = Swap_LittleFloat(in->vecs[0][j]);
        }

        int miptex = Swap_LittleLong(in->miptex);
        out->flags = Swap_LittleLong(in->flags);

        if (!textures)
        {
            out->texture = NULL;	// checkerboard texture
            out->flags = 0;
        }
        else
        {
            if (miptex >= numtextures)
            {
                Sys_Error("miptex >= numtextures");
            }

            out->texture = textures[miptex];

            if (!out->texture)
            {
                out->flags = 0;
            }
        }
    }
}

void dxModel::LoadFaces(lump_s *l)
{
    typedef dxItemBuffer<dxVec2> dxVec2Buffer;
    typedef dxItemBuffer<dxVec3> dxVec3Buffer;

    dxVec3Buffer vertices_buffer(512 * 1024);
    dxVec2Buffer txcoord1_buffer(512 * 1024);
    dxVec2Buffer txcoord2_buffer(512 * 1024);

    struct dface_s
    {
        short		planenum;
        short		side;

        int			firstedge;		// we must support > 64k edges
        short		numedges;	
        short		texinfo;

        // lighting info
        byte		styles[MAXLIGHTMAPS];
        int			lightofs;		// start of [numstyles*surfsize] samples
    };

    //numsufvet = 0;

    dface_s * in = (dface_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad face count");
    }

    int count = l->filelen / sizeof(*in);

    surfaces = (msurface_s*)DX_MEM_ALLOC(sizeof(msurface_s) * count);
    numsurfaces = count;

    msurface_s * out = surfaces;

    dxVec3 temp_vertices[32];
    dxVec2 temp_txcoord1[32];
    dxVec2 temp_txcoord2[32];

    for (int i = 0; i < count; i++, in++, out++)
    {
        out->firstedge = Swap_LittleLong(in->firstedge);
        out->numedges  = Swap_LittleLong(in->numedges);
        out->flags = SURF_NORMAL;

        out->lightmap  = NULL;
        out->light_s   = 0;
        out->light_t   = 0;

        short planenum = Swap_LittleShort(in->planenum);
        short side = Swap_LittleShort(in->side);
        if (side)
        {
            out->flags |= SURF_PLANEBACK;
        }

        out->plane = planes + planenum;

        short s = Swap_LittleShort(in->texinfo);
        if (s >= numtexinfo)
        {
            Sys_Error("bad texinfo index");
        }
        out->texinfo = texinfo + s;

        out->numverts = in->numedges;
        out->vet_offset = vertices_buffer.GetCount();

        int tex_w = 1;
        int tex_h = 1;

        float * v1 = out->texinfo->vecs[0];
        float * v2 = out->texinfo->vecs[1];

        if (out->texinfo->texture)
        {
            tex_w = out->texinfo->texture->width;
            tex_h = out->texinfo->texture->height;
        }

        if (out->numverts > 32)
        {
            Sys_Error("too many surface vertices");
        }

        for (int j = 0; j < out->numverts; j++)
        {
            int e = surfedges[in->firstedge + j];

            if (e >= 0)
            {
                temp_vertices[j] = vertices[edges[e].v[0]];
            }
            else
            {
                temp_vertices[j] = vertices[edges[-e].v[1]];
            }

            if (out->texinfo->texture)
            {
                float * vts = temp_vertices[j];

                float s = v1[0] * vts[0] + v1[1] * vts[1] + v1[2] * vts[2] + v1[3];
                float t = v2[0] * vts[0] + v2[1] * vts[1] + v2[2] * vts[2] + v2[3];

                temp_txcoord1[j].x = s / tex_w;
                temp_txcoord1[j].y = t / tex_h;
            }
            else
            {
                temp_txcoord1[j].x = 0;
                temp_txcoord1[j].y = 0;
            }

            temp_txcoord2[j].x = 0;
            temp_txcoord2[j].y = 0;
        }

        CalcSurfaceExtents(out);

        //lighting info
        for (int j = 0; j < MAXLIGHTMAPS; j++)
        {
            out->styles[j] = in->styles[j];
        }

        int lightofs = Swap_LittleLong(in->lightofs);

        if (-1 == lightofs)
        {
            out->samples = NULL;
        }
        else
        {
            out->samples = lightdata + lightofs;
        }

        // set the drawing flags flag
        if (!strncmp(out->texinfo->texture->name, "sky", 3))
        {
            out->flags |= (SURF_DRAWSKY | SURF_DRAWTILED);

            goto SET_VERT_BUF;
        }

        if (!strncmp(out->texinfo->texture->name, "black", 5))
        {
            out->flags = SURF_NODRAW;

            goto SET_VERT_BUF;
        }

        if (!strncmp(out->texinfo->texture->name, "hint", 4))
        {
            out->flags = SURF_NODRAW;

            goto SET_VERT_BUF;
        }

        if (!strncmp(out->texinfo->texture->name,"*",1))		// turbulent
        {
            out->flags |= (SURF_DRAWTURB | SURF_DRAWTILED);
            for (int k = 0; k < 2; k++)
            {
                out->extents[k] = 16384;
                out->texturemins[k] = -8192;
            }
            //GL_SubdivideSurface (out); // cut up polygon for warps
            
            goto SET_VERT_BUF;
        }

        //load lightmap
        CreateSurfaceLightmap(out);

        //set lightmap texture coordinate

        for (int j = 0; j < out->numverts; j++)
        {
            float s = Vec3DotProduct(temp_vertices[j], out->texinfo->vecs[0]) + out->texinfo->vecs[0][3];
            s -= out->texturemins[0];
            s += out->light_s * 16;
            s += 8;
            s /= BLOCK_WIDTH * 16;

            float t = Vec3DotProduct(temp_vertices[j], out->texinfo->vecs[1]) + out->texinfo->vecs[1][3];
            t -= out->texturemins[1];
            t += out->light_t * 16;
            t += 8;
            t /= BLOCK_HEIGHT * 16;

            temp_txcoord2[j].x = s;
            temp_txcoord2[j].y = t;
        }

SET_VERT_BUF:

        for (int j = 0; j < out->numverts; j++)
        {
            vertices_buffer.Add(temp_vertices[j]);
            txcoord1_buffer.Add(temp_txcoord1[j]);
            txcoord2_buffer.Add(temp_txcoord2[j]);
        }

    }

    renderer.LoadLightmaps();

    vertbuf_model   = renderer.CreateVertexBufferVec3(vertices_buffer.GetBuffer(), vertices_buffer.GetCount());
    texcoord1_model = renderer.CreateVertexBufferVec2(txcoord1_buffer.GetBuffer(), txcoord1_buffer.GetCount());
    texcoord2_model = renderer.CreateVertexBufferVec2(txcoord2_buffer.GetBuffer(), txcoord2_buffer.GetCount());

}

void dxModel::LoadMarksurfaces(lump_s *l)
{
    short * in = (short*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad marksurface count");
    }

    int count = l->filelen / sizeof(*in);

    marksurfaces = (msurface_s**)DX_MEM_ALLOC(sizeof(msurface_s*) * count);
    nummarksurfaces = count;

    msurface_s ** out = marksurfaces;

    for (int i = 0; i < count; i++)
    {
        int j = Swap_LittleShort(in[i]);
        if (j >= numsurfaces)
        {
            Sys_Error("bad surface number");
        }
        out[i] = surfaces + j;
    }
}

void dxModel::LoadVisibility(lump_s *l)
{
    if (!l->filelen)
    {
        return;
    }

    visdata = (byte*)DX_MEM_ALLOC(l->filelen); 
    memcpy(visdata, mod_base + l->fileofs, l->filelen);
}

void dxModel::LoadLeafs(lump_s *l)
{
#define	NUM_AMBIENTS 4 // automatic ambient sounds

    struct dleaf_s
    {
        int			   contents;
        int			   visofs;    // -1 = no visibility info

        short		   mins[3];   // for frustum culling
        short		   maxs[3];

        unsigned short firstmarksurface;
        unsigned short nummarksurfaces;

        byte		   ambient_level[NUM_AMBIENTS];
    };

    dleaf_s * in = (dleaf_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad leaf count");
    }

    int count = l->filelen / sizeof(*in);

    if (count > MAX_MAP_LEAFS)
    {
        Sys_Error("too many leafs");
    }

    leafs = (mleaf_s*)DX_MEM_ALLOC(sizeof(mleaf_s) * count);
    numleafs = count;

    mleaf_s * out = leafs;

    for (int i = 0; i < count; i++, in++, out++)
    {
        for (int j = 0; j < 3; j++)
        {
            out->minmaxs[j]   = Swap_LittleShort(in->mins[j]);
            out->minmaxs[3+j] = Swap_LittleShort(in->maxs[j]);
        }

        out->visframe = -1;
        out->contents = Swap_LittleShort(in->contents);
		out->parent = NULL;
        out->firstmarksurface = marksurfaces + Swap_LittleShort(in->firstmarksurface);
        out->nummarksurfaces  = Swap_LittleShort(in->nummarksurfaces);

        int visofs = Swap_LittleLong(in->visofs);
        if (visofs == -1)
        {
            out->compressed_vis = NULL;
        }
        else
        {
            out->compressed_vis = visdata + visofs;
        }
        //out->efrags = NULL;

        for (int j = 0; j < 4; j++)
        {
            out->ambient_sound_level[j] = in->ambient_level[j];
        }

        // gl underwater warp
        if (out->contents != CONTENTS_EMPTY)
        {
            for (int j = 0; j < out->nummarksurfaces; j++)
            {
                out->firstmarksurface[j]->flags |= SURF_UNDERWATER;
            }
        }
    }	
}

void dxModel::LoadNodes(lump_s *l)
{
    struct dnode_s
    {
        int			    planenum;
        short		    children[2]; // negative numbers are -(leafs+1), not nodes
        short		    mins[3];     // for sphere culling
        short		    maxs[3];
        unsigned short	firstface;
        unsigned short	numfaces;    // counting both sides
    };

    dnode_s * in = (dnode_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad node count");
    }

    int count = l->filelen / sizeof(*in);

    nodes = (mnode_s*)DX_MEM_ALLOC(sizeof(mnode_s) * count);
    numnodes = count;

    mnode_s * out = nodes;

    for (int i = 0; i < count; i++, in++, out++)
    {
        for (int j = 0; j < 3; j++)
        {
            out->minmaxs[j]   = Swap_LittleShort(in->mins[j]);
            out->minmaxs[3+j] = Swap_LittleShort(in->maxs[j]);
        }

        int planenum = Swap_LittleLong(in->planenum);

        out->visframe = -1;
        out->contents = 0;
		out->parent = NULL;
        out->firstsurface = Swap_LittleShort(in->firstface);
        out->numsurfaces = Swap_LittleShort(in->numfaces);

        out->plane = planes + planenum;

        for (int j = 0; j < 2; j++)
        {
            short child = Swap_LittleShort(in->children[j]);
            if (child >= 0)
            {
                out->children[j] = nodes + child;
            }
            else
            {
                out->children[j] = (mnode_s*)(leafs + (-1 - child));
            }
        }

        
    }

    SetParent(nodes, NULL); // sets nodes and leafs
}

void dxModel::LoadClipnodes(lump_s *l)
{
    dclipnode_s * in = (dclipnode_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad clip node count");
    }

    int count = l->filelen / sizeof(*in);

    clipnodes = (dclipnode_s*)DX_MEM_ALLOC(sizeof(dclipnode_s) * count);
    numclipnodes = count;

    dclipnode_s * out = clipnodes;

    hull_s * hull;

    hull = &hulls[1];
    hull->clipnodes = out;
    hull->firstclipnode = 0;
    hull->lastclipnode = count-1;
    hull->planes = planes;
    hull->clip_mins[0] = -16;
    hull->clip_mins[1] = -16;
    hull->clip_mins[2] = -24;
    hull->clip_maxs[0] =  16;
    hull->clip_maxs[1] =  16;
    hull->clip_maxs[2] =  32;

    hull = &hulls[2];
    hull->clipnodes = out;
    hull->firstclipnode = 0;
    hull->lastclipnode = count-1;
    hull->planes = planes;
    hull->clip_mins[0] = -32;
    hull->clip_mins[1] = -32;
    hull->clip_mins[2] = -24;
    hull->clip_maxs[0] =  32;
    hull->clip_maxs[1] =  32;
    hull->clip_maxs[2] =  64;

    for (int i = 0; i < count; i++, out++, in++)
    {
        out->planenum    = Swap_LittleLong(in->planenum);
        out->children[0] = Swap_LittleShort(in->children[0]);
        out->children[1] = Swap_LittleShort(in->children[1]);
    }
}

void dxModel::LoadSubmodels(lump_s *l)
{
    struct dmodel_s
    {
        float	mins[3];
        float   maxs[3];
        float	origin[3];
        int		headnode[MAX_MAP_HULLS];
        int		visleafs; // not including the solid leaf 0
        int		firstface;
        int     numfaces;
    };

    dmodel_s * in = (dmodel_s*)(mod_base + l->fileofs);
    if (l->filelen % sizeof(*in))
    {
        Sys_Error("bad submodel count");
    }

    int count = l->filelen / sizeof(*in);

    submodels = (mmodel_s*)DX_MEM_ALLOC(sizeof(mmodel_s) * count);
    numsubmodels = count;

    mmodel_s * out = submodels;

    for (int i = 0; i < count ; i++, in++, out++)
    {
        for (int j = 0; j < 3; j++)
        {	// spread the mins / maxs by a pixel
            out->mins[j]   = Swap_LittleFloat(in->mins[j]) - 1;
            out->maxs[j]   = Swap_LittleFloat(in->maxs[j]) + 1;
            out->origin[j] = Swap_LittleFloat(in->origin[j]);
        }

        for (int j = 0; j < 3; j++)
        {
            out->mins[j] += out->origin[j];
            out->maxs[j] += out->origin[j];
        }

        for (int j = 0; j < MAX_MAP_HULLS; j++)
        {
            out->headnode[j] = Swap_LittleLong (in->headnode[j]);
        }

        out->visleafs  = Swap_LittleLong(in->visleafs);
        out->firstface = Swap_LittleLong(in->firstface);
        out->numfaces  = Swap_LittleLong(in->numfaces);
    }
}

void dxModel::LoadSky()
{
    char * suf[6] = {"rt", "lf", "ft", "bk", "up", "dn"};

    char filename[MAX_PATH];
    char identifier[DX_ID_LEN];

    if (!skyname[0])
    {
        return;
    }

    for (int i = 0; i < 6; i++)
    {
        sprintf_s(filename, MAX_PATH, "%s\\gfx\\env\\%s%s.bmp", sysvar.basedir, skyname, suf[i]);
        sprintf_s(identifier, DX_ID_LEN, "skytexture_%s", suf[i]);
        skytexture[i] = renderer.LoadBMPTexture(identifier, filename);
    }
}

void dxModel::InitViewer()
{
    viewer.vieworigin.Clear();
    viewer.viewangles.Clear();

    for (int i = 0; i < numentities; i++)
    {
        dxEntity * e = entities + i;

        if (e->classtype == INFO_PLAYER_DEATHMATCH)
		//if (e->classtype == INFO_PLAYER_START)
        {
            viewer.vieworigin.Set(e->origin);
            viewer.viewangles.Set(e->angles);

            break;
        }
    }

    AngleToVec3s(viewer.viewangles, viewer.viewforward, viewer.viewright, viewer.viewup);
}

void dxModel::CalcSurfaceExtents(msurface_s *s)
{
    float mins[2], maxs[2];

    mins[0] = mins[1] =  999999.0f;
    maxs[0] = maxs[1] = -999999.0f;

    mtexinfo_s * tex = s->texinfo;

    for (int i = 0; i < s->numedges; i++)
    {
        int e = surfedges[s->firstedge+i];
        dxVec3 * v = NULL;
        if (e >= 0)
        {
            v = &vertices[edges[e].v[0]];
        }
        else
        {
            v = &vertices[edges[-e].v[1]];
        }

        for (int j = 0; j < 2; j++)
        {
            float val = v->x * tex->vecs[j][0]
            + v->y * tex->vecs[j][1]
            + v->z * tex->vecs[j][2]
            + tex->vecs[j][3];

            if (val < mins[j])
            {
                mins[j] = val;
            }

            if (val > maxs[j])
            {
                maxs[j] = val;
            }
        }
    }

    int bmins[2], bmaxs[2];

    for (int i = 0; i < 2; i++)
    {	
        bmins[i] = (int)floor(mins[i]/16.0f);
        bmaxs[i] = (int)ceil(maxs[i]/16.0f);

        s->texturemins[i] = bmins[i] * 16;
        s->extents[i] = (bmaxs[i] - bmins[i]) * 16;
    }
}

void dxModel::CreateSurfaceLightmap(msurface_s *surf)
{
    if (surf->flags & (SURF_DRAWSKY | SURF_DRAWTURB))
    {
        return;
    }

    if (!surf->samples)
    {
        return;
    }

    int w = (surf->extents[0] >> 4) + 1;
    int h = (surf->extents[1] >> 4) + 1;

    surf->lightmap = renderer.AllocLightmapBlock(w, h, surf->light_s, surf->light_t);

    lightmap_s * map = surf->lightmap;

    int s = surf->light_s;
    int t = surf->light_t;

    byte * p = surf->samples;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            map->buffer[t+i][s+j][0] = p[0];
            map->buffer[t+i][s+j][1] = p[1];
            map->buffer[t+i][s+j][2] = p[2];
            map->buffer[t+i][s+j][3] = 255;
            p += 3;
        }
    }
}

byte * dxModel::LeafPVS(mleaf_s *leaf)
{
    if (leaf == leafs)
    {
        return novis;
    }
    return DecompressVis(leaf->compressed_vis);
}

byte * dxModel::DecompressVis(byte *in)
{
    static byte	decompressed[MAX_MAP_LEAFS/8];
    int		c;
    byte	*out;
    int		row;

    row = (numleafs+7)>>3;	
    out = decompressed;

    if (!in)
    {	// no vis info, so make all visible
        while (row)
        {
            *out++ = 0xff;
            row--;
        }
        return decompressed;		
    }

    do
    {
        if (*in)
        {
            *out++ = *in++;
            continue;
        }

        c = in[1];
        in += 2;
        while (c)
        {
            *out++ = 0;
            c--;
        }
    } while (out - decompressed < row);

    return decompressed;
}

dxModel::mleaf_s * dxModel::PointInLeaf(const float *p)
{
	mnode_s * node = nodes;
	while (1)
	{
		if (node->contents < 0)
		{
			return (mleaf_s*)node;
		}

		mplane_s * plane = node->plane;

		float d = Vec3DotProduct(p, plane->normal) - plane->dist;

		if (d > 0)
		{
			node = node->children[0];
		}
		else
		{
			node = node->children[1];
		}
	}

	return NULL;	// never reached
}

void dxModel::MarkLeaves()
{
    if (oldviewleaf == viewleaf)
    {
        return;
    }

    visframecount++;
    oldviewleaf = viewleaf;

    byte * vis = LeafPVS(viewleaf);

    for (int i = 0; i < visleafs; i++)
    {
        if (vis[i>>3] & (1<<(i&7)))
        {
            mnode_s	*node = (mnode_s*)&leafs[i+1];

            do 
            {
                if (node->visframe == visframecount)
                {
                    break;
                }

                node->visframe = visframecount;
                node = node->parent;

            } while (node);
        }
    }
}

void dxModel::DrawWorld()
{
    r_opaque = NULL;
    r_opaque_rear = NULL;
    r_alpha  = NULL;
    r_alpha_rear = NULL;

    RecursiveWorldNode(nodes);
	DrawOpaqueSurface(false);
}

void dxModel::RecursiveWorldNode(mnode_s *node)
{
    if (node->contents == CONTENTS_SOLID)
    {
        return; // solid
    }

    if (node->visframe != visframecount)
    {
        return;
    }

    if (renderer.CullBox(node->minmaxs, node->minmaxs+3))
    {
        return;
    }

    if (node->contents < 0)
    {
        mleaf_s * l = (mleaf_s*)node;

        if (l->visframe != visframecount)
        {
            return;
        }

        msurface_s **mark = l->firstmarksurface;
        int c = l->nummarksurfaces;

        while (c--)
        {
            (*mark)->visframe = framecount;
            mark++;
        }

        return;
    }

    mplane_s * p = node->plane;
    float dot;

    switch (p->type)
    {
    case PLANE_X:
        dot = viewer.vieworigin[0] - p->dist;
        break;
    case PLANE_Y:
        dot = viewer.vieworigin[1] - p->dist;
        break;
    case PLANE_Z:
        dot = viewer.vieworigin[2] - p->dist;
        break;
    default:
        dot = Vec3DotProduct(viewer.vieworigin, p->normal) - p->dist;
        break;
    }

    int side = dot < 0 ? 1 : 0;

    RecursiveWorldNode(node->children[side]);

    msurface_s * s = surfaces + node->firstsurface;
    int c = node->numsurfaces;

    while (c--)
    {
        if (s->visframe != framecount || s->flags == SURF_DRAWSKY)
        {
            s++;
            continue;
        }

        s->texturechain = NULL;

        if (r_opaque)
        {
            r_opaque_rear->texturechain = s;
            r_opaque_rear = s;
        }
        else
        {
            r_opaque = s;
            r_opaque_rear = s;
        }

        s++;
    }

    RecursiveWorldNode(node->children[1-side]);

}

void dxModel::DrawEntitiesOnList()
{
    r_opaque = NULL;
    r_opaque_rear = NULL;

    r_alpha = NULL;
    r_alpha_rear = NULL;

    for (int i = 0; i < numentities; i++)
    {
        dxEntity * e = entities + i;

        if (e->model > 0 && e->classtype > FUNC_HOSTAGE_RESCURE)
        {
            mmodel_s * model = submodels + e->model;

            if (renderer.CullBox(model->mins, model->maxs))
            {
                continue;
            }

            msurface_s * s = surfaces + model->firstface;

            for (int j = 0; j < model->numfaces; j++)
            {
                s->texturechain = NULL;

                if (e->renderamt < 1.0f)
                {
					s->alpha = e->renderamt;
                    s->texoffset = 0;

                    if (e->classtype == FUNC_WATER)
                    {
						if (s->flags & SURF_UNDERWATER || s->plane->normal[2] == 0.0f)
						{
							s++;
							continue;
						}
						s->texoffset = frame.totaltime * 0.02f;
                    }

					if (r_alpha)
					{
						r_alpha_rear->texturechain = s;
						r_alpha_rear = s;
					}
					else
					{
						r_alpha = s;
						r_alpha_rear = s;
					}

                }
                else
                {
					if (r_opaque)
					{
						r_opaque_rear->texturechain = s;
						r_opaque_rear = s;
					}
					else
					{
						r_opaque = s;
						r_opaque_rear = s;
					}
                }

                s++;
            }
        }
    }

    DrawOpaqueSurface(true);
    DrawAlphaSurface();
}

void dxModel::DrawSkyBox()
{
    renderer.BeginDrawSky();

    for (int i = 0; i < 6; i++)
    {
        if (skytexture[i])
        {
            renderer.DrawQuad(skytexture[i], i*4);
        }
    }

    renderer.EndDrawSky();
}

void dxModel::DrawOpaqueSurface(bool alphatest)
{
    renderer.BeginDrawOpaque(vertbuf_model, texcoord1_model, texcoord2_model, alphatest);

    msurface_s * s = r_opaque;
    for (; s; s = s->texturechain)
    {
        if (!s->lightmap)
        {
            continue;
        }

        renderer.DrawPolygonOpaque(s->texinfo->texture->gl_texturenum, s->lightmap->texnum,
            s->vet_offset, s->numverts);
    }

    renderer.EndDrawOpaque();
}

void dxModel::DrawAlphaSurface()
{
    renderer.BeginDrawAlpha(vertbuf_model, texcoord1_model);

    msurface_s * s = r_alpha;
    for (; s; s = s->texturechain)
    {
        renderer.DrawPolygonAlpha(s->texinfo->texture->gl_texturenum, s->alpha, s->texoffset,
            s->vet_offset, s->numverts);
    }

    renderer.EndDrawAlpha();
}
