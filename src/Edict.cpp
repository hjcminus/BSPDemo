/*==============================================================================
  BSPDemo
  Edict.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

char com_token[1024];

char * COM_Parse(char * data)
{
    int len = 0;
    com_token[0] = 0;

    if (!data)
    {
        return NULL;
    }

    int c;

    // skip whitespace
skipwhite:
    while ((c = *data) <= ' ')
    {
        if (0 == c)
        {
            return NULL;
        }
        data++;
    }

    // skip // comments
    if ('/' == c && '/' == data[1])
    {
        while (*data && *data != '\n')
        {
            data++;
        }
        goto skipwhite;
    }

    // handle quoted strings specially
    if ('\"' == c)
    {
        data++;
        while (1)
        {
            c = *data++;
            if ('\"' == c || !c)
            {
                com_token[len] = 0;
                return data;
            }
            com_token[len] = c;
            len++;
        }
    }

    // parse single characters
    if ('{' == c || '}' == c || ')' == c || '(' == c || '\'' == c || ':' == c)
    {
        com_token[len] = c;
        len++;
        com_token[len] = 0;
        return data+1;
    }

    // parse a regular word
    do 
    {
        com_token[len] = c;
        data++;
        len++;
        c = *data;

        if ('{' == c || '}' == c || ')' == c || '(' == c || '\'' == c || ':' == c) 
        {
            break;
        }
    } while (c > 32);

    com_token[len] = 0;

    return data;
}

char * Com_FindNext(char * text)
{
    while (*text != ' ')
    {
        if (*text == 0)
        {
            return NULL;
        }
        text++;
    }
    return text + 1;
}

char * ED_ValueForKey(epair_s *p, char *key)
{
    while (p)
    {
        if (!strcmp(p->key, key))
        {
            return p->value;
        }
        p = p->next;
    }
    return NULL;
}

void ED_ParseOrigin(epair_s *p, dxVec3 &origin)
{
	origin.Clear();
    char * v = ED_ValueForKey(p, "origin");
    if (v)
    {
		for (int i = 0; i < 3; i++)
		{
			origin[i] = (float)atof(v);
			v = Com_FindNext(v);
			if (!v)
			{
				break;
			}
		}
    }
}

void ED_ParseRenderColor(epair_s *p, dxVec3 &rendercolor)
{
	rendercolor.Clear();
    char * v = ED_ValueForKey(p, "rendercolor");
    if (v)
    {
		for (int i = 0; i < 3; i++)
		{
			rendercolor[i] = (float)atof(v);
			v = Com_FindNext(v);
			if (!v)
			{
				break;
			}
		}
    } 
}

void ED_ParseRenderAmt(epair_s *p, float &renderamt)
{
    renderamt = 1.0f;
    char * v = ED_ValueForKey(p, "renderamt");
    if (v)
    {
        renderamt = atoi(v) / 255.0f;
    }
}

void ED_ParseRenderMode(epair_s *p, int &rendermode)
{
    rendermode = 0;
    char * v = ED_ValueForKey(p, "rendermode");
    if (v)
    {
        rendermode = atoi(v);
    }
}

void ED_ParseModel(epair_s *p, int &model)
{
    char * v = ED_ValueForKey(p, "model");
    if (!v)
    {
        Sys_Error("no model");
    }

    model = atoi(v+1); //*number
}

void ED_ParseAngles(epair_s *p, dxVec3 &angles)
{
    angles[DX_PITCH] = 0;
    angles[DX_YAW]   = 0;
    angles[DX_ROLL]  = 0;

    char * v = ED_ValueForKey(p, "angles");
    if (v)
    {
        for (int j = 0; j < 3; j++)
        {
            angles[j] = (float)atof(v);
            v = Com_FindNext(v);
            if (!v)
            {
                break;
            }
        }
    }
}

void ED_ParseRenderfx(epair_s *p, int &renderfx)
{
	renderfx = 0;
    char * v = ED_ValueForKey(p, "renderfx");
    if (v)
    {
		renderfx = atoi(v);
    }
}

void ED_ParseSkyName(epair_s *p, char *value)
{
    char * v = ED_ValueForKey(p, "skyname");
	if (v)
	{
		strcpy_s(value, DX_ID_LEN, v);
	}
}

void ED_ParseHostageEntity(dxEntity * e)
{
    ED_ParseOrigin(e->epairs, e->origin);
    ED_ParseAngles(e->epairs, e->angles);
}

void ED_ParseInfoPlayerDeathmatch(dxEntity * e)
{
    ED_ParseOrigin(e->epairs, e->origin);
    ED_ParseAngles(e->epairs, e->angles);
}

void ED_ParseInfoPlayerStart(dxEntity * e)
{
    ED_ParseOrigin(e->epairs, e->origin);
    ED_ParseAngles(e->epairs, e->angles);
}

void ED_ParseFuncBreakable(dxEntity * e)
{
    ED_ParseModel(e->epairs, e->model);
    ED_ParseRenderColor(e->epairs, e->rendercolor);
    ED_ParseRenderAmt(e->epairs, e->renderamt);
    ED_ParseRenderMode(e->epairs, e->rendermode);
}

void ED_ParseFuncWall(dxEntity * e)
{
    //"model" "*5"
    //"rendercolor" "0 0 0"
    ED_ParseModel(e->epairs, e->model);
    ED_ParseRenderColor(e->epairs, e->rendercolor);
    ED_ParseRenderAmt(e->epairs, e->renderamt);
}

void ED_ParseFuncIllusionary(dxEntity * e) //func_illusionary
{
    //"model" "*10"
    //"skin" "-1"
    //"renderamt" "255"
    //"rendermode" "4"
    //"rendercolor" "0 0 0"
    ED_ParseModel(e->epairs, e->model);
    ED_ParseRenderAmt(e->epairs, e->renderamt);
    ED_ParseRenderMode(e->epairs, e->rendermode);
    ED_ParseRenderColor(e->epairs, e->rendercolor);
}

void ED_ParseFuncLadder(dxEntity * e)
{
    //"model" "*49"
    ED_ParseModel(e->epairs, e->model);
}

void ED_ParseFuncHostageRescue(dxEntity * e)
{
    ED_ParseModel(e->epairs, e->model);
}

void ED_ParseFuncBuyZone(dxEntity * e)
{
    //"model" "*58"
    //"team" "1"
    ED_ParseModel(e->epairs, e->model);
}

void ED_ParseFuncButton(dxEntity * e)
{
    //"model" "*199"
    //"target" "door1"
    //"delay" "1"
    //"wait" "3"
    //"speed" "5"
    //"angles" "0 0 0"
    //"rendercolor" "0 0 0"
    //"spawnflags" "33"
    //"classname" "func_button"
    ED_ParseModel(e->epairs, e->model);
    ED_ParseAngles(e->epairs, e->angles);
    ED_ParseRenderColor(e->epairs, e->rendercolor);

}

void ED_ParseFuncDoor(dxEntity * e)
{
    //"model" "*200"
    //"lip" "66"
    //"wait" "20"
    //"stopsnd" "1"
    //"movesnd" "6"
    //"speed" "40"
    //"targetname" "door1"
    //"angles" "-90 0 0"
    //"rendercolor" "0 0 0"
    //"classname" "func_door"
    ED_ParseModel(e->epairs, e->model);
    ED_ParseAngles(e->epairs, e->angles);
    ED_ParseRenderColor(e->epairs, e->rendercolor);
}

void ED_ParseFuncDoorRotating(dxEntity * e)
{
    //"model" "*10"
    //"origin" "-1902 2706 -179"
    //"distance" "90"
    //"wait" "4"
    //"movesnd" "9"
    //"speed" "100"
    //"angles" "0 0 0"
    //"rendercolor" "0 0 0"
    //"classname" "func_door_rotating"
    ED_ParseModel(e->epairs, e->model);
    ED_ParseOrigin(e->epairs, e->origin);
    ED_ParseAngles(e->epairs, e->angles);
    ED_ParseRenderColor(e->epairs, e->rendercolor);
}

void ED_ParseFuncWater(dxEntity * e)
{
    //"model" "*33"
    //"skin" "-3"
    //"wait" "4"
    //"speed" "100"
    //"angles" "0 0 0"
    //"renderamt" "100"
    //"rendermode" "2"
    //"rendercolor" "0 0 0"
    //"classname" "func_water"
    ED_ParseModel(e->epairs, e->model);
    ED_ParseAngles(e->epairs, e->angles);
    ED_ParseRenderAmt(e->epairs, e->renderamt);
    ED_ParseRenderMode(e->epairs, e->rendermode);
    ED_ParseRenderColor(e->epairs, e->rendercolor);
}

void ED_ParseLight(dxEntity * e)
{
    //"origin" "-1805 2728 -40"
    //"_light" "255 255 160 100"
    //"classname" "light"
    ED_ParseOrigin(e->epairs, e->origin);
}

void ED_ParseInfoBombTarget(dxEntity * e)
{
    //"origin" "-1855 2880 -120"
    //"classname" "info_bomb_target"
    ED_ParseOrigin(e->epairs, e->origin);
}

void ED_ParseEnvGlow(dxEntity * e)
{
    //"origin" "-1809 2740 -144"
    //"scale" "1"
    //"model" "sprites/glow01.spr"
    //"rendercolor" "255 255 160"
    //"renderamt" "150"
    //"rendermode" "3"
    //"renderfx" "14"
    //"classname" "env_glow"
    ED_ParseOrigin(e->epairs, e->origin);
    ED_ParseRenderColor(e->epairs, e->rendercolor);
    ED_ParseRenderAmt(e->epairs, e->renderamt);
    ED_ParseRenderMode(e->epairs, e->rendermode);
    ED_ParseRenderfx(e->epairs, e->renderfx);
}

