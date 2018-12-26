/**
 * Configure how the system waits for the network.
 */

#pragma onces

#define NETWORK_NO_NETWORK_QUALITY_THRESHOLD	8		///< Minimum signal quality to consider the cellphone reception as reliable.
#define NETWORK_NO_NETWORK_TRIES				5		///< Maximum tries before considering an unreliable cellphone reception as no reception at all.
#define NETWORK_INTERMEDIATE_TIMEOUT_MS			10000L	///< Intermediate time to sleep between each cellphone reception assessment.
#define NETWORK_TOTAL_TIMEOUT_MS				80000L	///< Timeout after which to stop trying to register to the network.
