
#ifndef LEVEL_INC__
#define LEVEL_INC__

#define	CHUNK_LEN	3
#define	LOCAL_LEVEL	"currentlevel.txt"

bool		lvl_LoadRaw(int episode_id, int level_id, bool load_local);
char	*	lvl_LoadLevelLocally(int episode_id, int level_id);
void		lvl_CopyLevelLocally(char * levelRaw);
char	*	lvl_getLevelRaw(int episode_id, int level_id);
void		lvl_clearTile(int index);
int			lvl_getHexaChunkValue(char *tab, int size);
int			lvl_getHexaValue(int hexa);
void		lvl_UpdateLocalLevelDatas();


//bool		lvl_LoadGedit(char *level_name);
//bool		lvl_LoadMaped(char *level_name);
int			g_GetNBunlockedLevel(int episodeID);
void		g_SetLevelCleared(int episodeID, int levelID, bool LevelCleared, bool DiamantGrabed);
void		g_SetTilePropFileName(int episode_id);
void		level_Affiche(int x_offset, int y_offset);
void		g_FreeMap();
void		g_AllocMap();
void		g_CadrePlayer();
void		g_FindPlayerPos();


#endif // LEVEL_INC__

