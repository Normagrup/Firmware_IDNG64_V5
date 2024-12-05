
unsigned char sensorStoringStart (unsigned char address, unsigned char disabled, unsigned char mode, const unsigned char *interfacesCfgIp, unsigned char packetId_1, unsigned char packetId_2, unsigned char *cfg);
void sensorStoringQuerySetpoint (unsigned char address, unsigned char* ip);
unsigned char sensorStoringSetSetpoint (unsigned char address, unsigned char level);

void cmd_QUERY_ENABLE_DISABLE_cb (unsigned char command, unsigned char valid, unsigned char answer);
void cmd_QUERY_MODE_cb (unsigned char command, unsigned char valid, unsigned char answer);