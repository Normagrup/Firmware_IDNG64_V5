unsigned char actualLevelDestTipoGet(unsigned char dest);
unsigned char actualLevelGrupoGet (unsigned char dest);
unsigned char actualLevelAddressPerteneceGrupoGet(unsigned char address, unsigned char grupo);
unsigned char actualLevelLvlToStoreGet (unsigned char cmd, unsigned char level);
void actualLevelGruposPerteneceGet(unsigned char address, unsigned char *grupos);
void actualLevelGruposAfectadosGet(unsigned char dest, unsigned char *gruposAfectados);
void actualLevelCmdNotify (unsigned char dest, unsigned char cmd, unsigned char level);
void actualLevel_GrupoValoresGet (unsigned char grupo, unsigned char *valores);













