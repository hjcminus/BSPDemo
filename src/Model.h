/*==============================================================================
  BSPDemo
  Model.h
================================================================================
*/

#ifndef __MODEL_H__
#define __MODEL_H__

#define BSPVERSION         30

#define LUMP_ENTITIES       0
#define LUMP_PLANES         1
#define LUMP_TEXTURES       2
#define LUMP_VERTEXES       3
#define LUMP_VISIBILITY     4
#define LUMP_NODES          5
#define LUMP_TEXINFO        6
#define LUMP_FACES          7
#define LUMP_LIGHTING       8
#define LUMP_CLIPNODES      9
#define LUMP_LEAFS         10
#define LUMP_MARKSURFACES  11
#define LUMP_EDGES         12
#define LUMP_SURFEDGES     13
#define LUMP_MODELS        14

#define	HEADER_LUMPS	   15

#define	SURF_PLANEBACK		2
//#define	SURF_DRAWSKY	4
#define SURF_DRAWSPRITE		8
#define SURF_DRAWTURB		0x10
#define SURF_DRAWTILED		0x20
#define SURF_DRAWBACKGROUND	0x40
#define SURF_UNDERWATER		0x80

#define SURF_NORMAL	        0
#define SURF_DRAWSKY        1
#define SURF_NODRAW         8

#define	CONTENTS_EMPTY	   -1
#define	CONTENTS_SOLID	   -2
#define	CONTENTS_WATER	   -3
#define	CONTENTS_SLIME	   -4
#define	CONTENTS_LAVA	   -5
#define	CONTENTS_SKY	   -6
#define	CONTENTS_ORIGIN	   -7  // removed at csg time
#define	CONTENTS_CLIP	   -8  // changed to contents_solid

#define MAX_EPAIRS        4096
#define MAX_MAP_LEAFS     8192

/*
========================================
  epair_s
========================================
*/
struct epair_s
{
    epair_s	* next;
    char	* key;
    char	* value;
};

enum dxEntClass
{
    UNKNOWN,
    WORLDSPAWN,             //worldspawn
    FUNC_BOMB_TARGET,       //func_bomb_target
    FUNC_BUYZONE,           //func_buyzone
    FUNC_HOSTAGE_RESCURE,   //func_hostage_rescue
    FUNC_ILLUSIONARY,       //func_illusionary
    FUNC_BREAKABLE,         //func_breakable
    LIGHT_SPOT,             //light_spot
    TRIGGER_MULTIPLE,       //trigger_multiple
    FUNC_WATER,             //func_water
    FUNC_WALL,              //func_wall
    LIGHT_ENVIRONMENT,      //light_environment
    FUNC_LADDER,            //func_ladder
    HOSTAGE_ENTITY,         //hostage_entity
    INFO_PLAYER_DEATHMATCH, //info_player_deathmatch
    INFO_PLAYER_START,      //info_player_start
    TRIGGER_CAMERA,         //trigger_camera
    INFO_TARGET,            //info_target
    AMBIENT_GENERIC,        //ambient_generic
    INFODECAL,              //infodecal
    FUNC_BUTTON,            //func_button
    FUNC_DOOR,              //func_door
    FUNC_DOOR_ROTATING,     //func_door_rotating
    LIGHT,                  //light
    INFO_BOMB_TARGET,       //info_bomb_target
    ENV_GLOW,               //env_glow    
};

/*
========================================
  dxEntity
========================================
*/
#define MAX_MAP_ENTITIES 1024

struct dxEntity
{
    dxVec3	  origin;
    dxVec3    angles;
    int		  firstbrush;
    int		  numbrushes;
    epair_s * epairs;
    int       visframe;
    int       model;
    int       classtype;

    //"skin" "-1"
    int       rendermode;
    float     renderamt;
    dxVec3    rendercolor;
    int       renderfx;

    //"team" "1"
};

/*
========================================
  dxModel
========================================
*/
class dxModel
{
public:

    dxModel();
    ~dxModel();

    void               Load(const char * filename);
    void               Free();

    void               DrawLoading();

    void               Draw(const dxVec3 &vieworigin_, const dxVec3 &viewforward_,
                           const dxVec3 &viewright_, const dxVec3 &viewup_);

	bool               IsLoaded() const;

private:

    struct lump_s
    {
        int		       fileofs; 
        int            filelen;
    };

    struct dheader_s
    {
        int            version;	
        lump_s         lumps[HEADER_LUMPS];
    };

    struct medge_s
    {
        unsigned short v[2];
    };

    struct texture_s
    {
        char		   name[16];
        unsigned	   width;
        unsigned       height;
        int			   gl_texturenum;
    };

    struct mtexinfo_s
    {
        float		   vecs[2][4];
        texture_s *    texture;
        int            flags;
    };

    struct mplane_s
    {
        float	       normal[3];
        float	       dist;
        byte	       type;			// for texture axis selection and fast side tests
        byte	       signbits;		// signx + signy<<1 + signz<<1
        byte	       pad[2];
    };

    struct dclipnode_s
    {
        int			   planenum;
        short		   children[2]; // negative numbers are contents
    };

    #define	MAX_MAP_HULLS 4

    struct mmodel_s
    {
        float		   mins[3];
        float          maxs[3];
        float		   origin[3];		// for sounds or lights
        float		   radius;
        int			   headnode[MAX_MAP_HULLS];
        int			   visleafs;		// not including the solid leaf 0
        int			   firstface;
        int            numfaces;
    };

    #define	MAXLIGHTMAPS  4

    struct msurface_s
    {
        int            visframe; // should be drawn when node is crossed

        mplane_s *     plane;
        int		       flags;

        int			   firstedge;	// look up in model->surfedges[], negative numbers
        int			   numedges;	// are backwards edges

        short		   texturemins[2];
        short		   extents[2];

        lightmap_s *   lightmap;
        int			   light_s;
        int            light_t;	// gl lightmap coordinates

        //glpoly_s *  polys;				// multiple if warped
        msurface_s *   texturechain;

        mtexinfo_s *   texinfo;

        // lighting info
        int			   dlightframe;
        int			   dlightbits;

        int			   lightmaptexturenum;
        byte		   styles[MAXLIGHTMAPS];
        //int            cached_light[MAXLIGHTMAPS];	// values currently used in lightmap
        //bool           cached_dlight;				// true if dynamic light in cache
        byte *         samples;		// [numstyles*surfsize]

        int		       numverts;
        int            vet_offset;
        int            tex_offset;

        float          alpha;
        float          texoffset;
    };

    struct mnode_s
    {
        // common with leaf
        int			   contents;     // 0, to differentiate from leafs
        int			   visframe;     // node needs to be traversed if current

        float		   minmaxs[6];   // for bounding box culling

        mnode_s *      parent;

        // node specific
        mplane_s *     plane;
        mnode_s *      children[2];

        unsigned short firstsurface;
        unsigned short numsurfaces;
    };

    #define	NUM_AMBIENTS 4 // automatic ambient sounds

    struct mleaf_s
    {
        // common with node
        int			   contents;		// wil be a negative contents number
        int			   visframe;		// node needs to be traversed if current

        float		   minmaxs[6];		// for bounding box culling

        mnode_s *      parent;

        // leaf specific
        byte *         compressed_vis;
        //efrag_t  *efrags;

        msurface_s **  firstmarksurface;
        int			   nummarksurfaces;
        //int			 key;			// BSP sequence number for leaf's contents
        byte		   ambient_sound_level[NUM_AMBIENTS];
    };

    struct hull_s
    {
        dclipnode_s	*  clipnodes;
        mplane_s *     planes;
        int			   firstclipnode;
        int			   lastclipnode;
        float          clip_mins[3];
        float          clip_maxs[3];
    };

    char *             mod_base;

    char               skyname[DX_ID_LEN];

    int                numentities;
    dxEntity           entities[MAX_MAP_ENTITIES];

    int                numepairs;
    epair_s            epairs[MAX_EPAIRS];

    int			       numvertices;
    dxVec3 *           vertices;

    int			       numedges;
    medge_s *	       edges;

    int			       numsurfedges;
    int *		       surfedges;

    byte *             lightdata;

    int			       numtexinfo;
    mtexinfo_s *       texinfo;

    int                numtextures;
    texture_s **       textures;

    int			       numplanes;
    mplane_s *         planes;

    #define MAX_WAD_FILE 64

    int                numwadfiles;
    char               wadfiles[MAX_WAD_FILE][DX_ID_LEN];

    int			       numsurfaces;
    msurface_s *       surfaces;

    int			       nummarksurfaces;
    msurface_s **      marksurfaces;

    byte *             visdata;

    int			       numnodes;
    mnode_s *          nodes;

    int			       numleafs;	// number of visible leafs, not counting 0
    int                visleafs;
    mleaf_s *          leafs;

    int			       numclipnodes;
    dclipnode_s *      clipnodes;

    hull_s		       hulls[MAX_MAP_HULLS];

    int			       numsubmodels;
    mmodel_s *         submodels;

    GLuint             skytexture[6];

    int                framecount;
    int                visframecount;
    mleaf_s	*          viewleaf;
    mleaf_s	*          oldviewleaf;

    byte               novis[MAX_MAP_LEAFS/8];

    GLuint             vertbuf_model, texcoord1_model, texcoord2_model;

    msurface_s *       r_opaque;
    msurface_s *       r_opaque_rear;
    msurface_s *       r_alpha;
    msurface_s *       r_alpha_rear;

    bool               loaded;
    int                loading_percent;

    void               InitWadFileList();
    void               SetParent(mnode_s *node, mnode_s *parent);
    
    void               LoadEntities(lump_s *l);
    void               LoadVertexes(lump_s *l);
    void               LoadEdges(lump_s *l);
    void               LoadSurfedges(lump_s *l);
    void               LoadTextures(lump_s *l);
    void               LoadLighting(lump_s *l);
    void               LoadPlanes(lump_s *l);
    void               LoadTexinfo(lump_s *l);
    void               LoadFaces(lump_s *l);
    void               LoadMarksurfaces(lump_s *l);
    void               LoadVisibility(lump_s *l);
    void               LoadLeafs(lump_s *l);
    void               LoadNodes(lump_s *l);
    void               LoadClipnodes(lump_s *l);
    void               LoadSubmodels(lump_s *l);
    void               LoadSky();

    void               InitViewer();

    void               CalcSurfaceExtents(msurface_s *s);
    void               CreateSurfaceLightmap(msurface_s *surf);

    //draw
    byte *             LeafPVS(mleaf_s *leaf);
    byte *             DecompressVis(byte *in);
	mleaf_s *          PointInLeaf(const float *p);
    void               MarkLeaves();

    void               DrawSkyBox();
    void               DrawWorld();
    void               RecursiveWorldNode(mnode_s *node);
    void               DrawEntitiesOnList();

    void               DrawOpaqueSurface(bool alphatest);
    void               DrawAlphaSurface();

    void               UpdateLoadingPercent(int percent);

};

extern dxModel worldModel;

#endif /* !__MODEL_H__ */
