#pragma once

#include "System.h"
#include "Hardware.h"
#include "Gps.h"
#include "Network.h"
#include "Sleeps.h"
#include "Alerts.h"

#define BACKUP_ENABLE_SDCARD 0	///< Enable (1) or disable (0) the backup of positions using an sd card.
								///< Note: This code has never been finished properly because of the lack of space
								///< on the ATMega
#define BACKUP_ENABLE_NETWORK 1	///< Enable (1) or disable (0) the backup of positions using the network.

#if BACKUP_ENABLE_SDCARD
	#include "BackupSd.h"
#endif

#if BACKUP_ENABLE_NETWORK
	#include "BackupNetwork.h"
#endif