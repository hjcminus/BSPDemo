/*==============================================================================
  BSPDemo
  Edict.h
================================================================================
*/

#ifndef __EDICT_H__
#define __EDICT_H__

extern char com_token[1024];

char * COM_Parse(char * data);
char * Com_FindNext(char * text);
char * ED_ValueForKey(epair_s *p, char *key);

void   ED_ParseSkyName(epair_s *p, char *value);
void   ED_ParseHostageEntity(dxEntity * e);
void   ED_ParseInfoPlayerDeathmatch(dxEntity * e);
void   ED_ParseInfoPlayerStart(dxEntity * e);
void   ED_ParseFuncBreakable(dxEntity * e);
void   ED_ParseFuncWall(dxEntity * e);
void   ED_ParseFuncIllusionary(dxEntity * e);
void   ED_ParseFuncLadder(dxEntity * e);
void   ED_ParseFuncHostageRescue(dxEntity * e);
void   ED_ParseFuncBuyZone(dxEntity * e);
void   ED_ParseFuncButton(dxEntity * e);
void   ED_ParseFuncDoor(dxEntity * e);
void   ED_ParseFuncDoorRotating(dxEntity * e);
void   ED_ParseFuncWater(dxEntity * e);
void   ED_ParseLight(dxEntity * e);
void   ED_ParseInfoBombTarget(dxEntity * e);
void   ED_ParseEnvGlow(dxEntity * e);

#endif /* !__EDICT_H__ */
