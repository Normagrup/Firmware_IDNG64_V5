#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#define INC_SENSORES_PRT
    #include "sensores.h"
#undef INC_SENSORES_PRT

#define INC_SENSORES_GRUPOS_PRT
    #include "sensoresGrupos/sensoresGrupos.h"
#undef INC_SENSORES_GRUPOS_PRT


sensoresConfiguracion  sensoresConfigSensoresConfiguracionGet (unsigned char sensorAddress)
{
    union dataToRead_u
    {
        sensoresConfiguracion dataToReadStruct;
        unsigned char dataToReadByte[SENSORES_CONFIGURACION_S_SIZE];
    }dataToReadUnion;
    if (sensorAddress!=0xFF)
    {
    }
    return dataToReadUnion.dataToReadStruct;
}
sensoresGruposConfiguracion sensoresConfigSensoresGruposConfiguracionGet (unsigned char numeroGrupo)
{
    union dataToRead_u
    {
        sensoresGruposConfiguracion dataToReadStruct;
        unsigned char dataToReadByte[SENSORES_GRUPOS_CONFIGURACION_S_SIZE];
    }dataToReadUnion;
    if (numeroGrupo!=0xFF)
    {
    }
    return dataToReadUnion.dataToReadStruct;
}
interfacesConfiguracion sensoresConfigInterfacesConfiguracionGet (unsigned char interfazAddress)
{
    union dataToRead_u
    {
        interfacesConfiguracion dataToReadStruct;
        unsigned char dataToReadByte[INTERFACES_CONFIGURACION_S_SIZE];
    }dataToReadUnion;
    if (interfazAddress!=0xFF)
    {
    }
    return dataToReadUnion.dataToReadStruct;
}
void sensoresConfigSensoresConfiguracionSet (unsigned char sensorAddress, sensoresConfiguracion configuracion)
{
    union dataToStore_u
    {
        sensoresConfiguracion dataToStoreStruct;
        unsigned char dataToStoreByte[SENSORES_CONFIGURACION_S_SIZE];
    }dataToStoreUnion;
    dataToStoreUnion.dataToStoreStruct=configuracion;
    if ((dataToStoreUnion.dataToStoreByte[0]!=0xFF) && (sensorAddress!=0xFF))
    {
    }
}
void sensoresConfigSensoresGruposConfiguracionSet (unsigned char numeroGrupo, sensoresGruposConfiguracion configuracion)
{
    union dataToStore_u
    {
        sensoresGruposConfiguracion dataToStoreStruct;
        unsigned char dataToStoreByte[SENSORES_GRUPOS_CONFIGURACION_S_SIZE];
    }dataToStoreUnion;
    dataToStoreUnion.dataToStoreStruct=configuracion;
    if ((dataToStoreUnion.dataToStoreByte[0]!=0xFF) && (numeroGrupo!=0xFF))
    {
    }
}
void sensoresConfigInterfacesConfiguracionSet (unsigned char interfazAddress, interfacesConfiguracion configuracion)
{
    union dataToStore_u
    {
        interfacesConfiguracion dataToStoreStruct;
        unsigned char dataToStoreByte[INTERFACES_CONFIGURACION_S_SIZE];
    }dataToStoreUnion;
    dataToStoreUnion.dataToStoreStruct=configuracion;
    if ((dataToStoreUnion.dataToStoreByte[0]!=0xFF) && (interfazAddress!=0xFF))
    {
    }
}





