#ifndef __IAPMESSAGE_F_H__
#define __IAPMESSAGE_F_H__

#include	"Define.h"

void clearStatusText(void);
void showIapStatus(char * statusStr);

void showIapProgess(unsigned char num);
void hideIapProgess(void);

void showIapVersion(unsigned short firmwareVersion, unsigned short bootVersion);
#endif

