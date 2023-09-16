#include <wups.h>
#include <whb/log.h>
#include <whb/log_module.h>
#include <whb/log_cafe.h>
#include <whb/log_udp.h>
#include <kernel/kernel.h>
#include <string.h>

#include <coreinit/title.h>
#include <coreinit/memorymap.h>

#include <patcher/rplinfo.h>
#include <patcher/patcher.h>

WUPS_PLUGIN_NAME("GiveMiiSonic");
WUPS_PLUGIN_DESCRIPTION("Patch to auto-boot into the debug menu for Mario & Sonic at the Rio 2016 Olympic Games");
WUPS_PLUGIN_VERSION("v2.0");
WUPS_PLUGIN_AUTHOR("GiveMiiYoutube originally be GaryOderNichts, modified by Whovian9369");
WUPS_PLUGIN_LICENSE("MIT");

#define USA_TITLE_ID 0x00050000101E5300llu
#define EUR_TITLE_ID 0x00050000101E5400llu
#define JPN_TITLE_ID 0x0005000010190300llu
// The Japanese title (disc, at least) uses different RPX as noted below, so we need specific patches just for the Japanese version.

ON_APPLICATION_START()
{
    // Set OSGetTitleID() to a variable so the function only actually needs to be run once for the below comparison
    uint64_t titleid = OSGetTitleID();
    // If this is not Mario & Sonic at the Rio 2016 Olympic Games, then there's no need to do anything
    // Japanese RPX differs from US and EUR versions, so removed from comparison as a different patch offset is needed.
   // Add this to below for actual JPN Title use:  `&& titleid != JPN_TITLE_ID)`
    if (titleid != USA_TITLE_ID && titleid != EUR_TITLE_ID) {
        return;
    }

    auto debugOffset = 0x55C4A4;
    // Change this whenever we get the new patch or two
    //
    // if (titleid == JPN_TITLE_ID) {
    //     debugOffset = 0x55C4A4; // CHANGE ME
    // }

    // Init logging
    if (!WHBLogModuleInit()) {
        WHBLogCafeInit();
        WHBLogUdpInit();
    }

    WHBLogPrintf("GiveMiiSonic: applying patches...");

    // Patch the dynload functions so GetRPLInfo works
    if (!PatchDynLoadFunctions()) {
        WHBLogPrintf("GiveMiiSonic: Failed to patch dynload functions");
        return;
    }

    // Get the RPLInfo
    auto rpl_info = TryGetRPLInfo();
    if (!rpl_info) {
        WHBLogPrintf("GiveMiiSonic: Failed to get RPL info");
        return;
    }

    // Find the rpx
    rplinfo rpls = *rpl_info;
    auto unison_rpx = FindRPL(rpls, "unison.rpx");
    if (!unison_rpx) {
        WHBLogPrintf("GiveMiiSonic: Failed to find unison.rpx");
        return;
    }

    // Patch the title :)
    // Set the instruction as a variable to so it is a touch easier for us to use.
    uint32_t debugInstruction = 0x38600005;
    OSDynLoad_NotifyData rpx_data = *unison_rpx;
    KernelCopyData(OSEffectiveToPhysical(rpx_data.textAddr + debugOffset), OSEffectiveToPhysical((uint32_t)(&debugInstruction)), sizeof(uint32_t));
    WHBLogPrintf("GiveMiiSonic: So we attempted KernelCopyData but don't exactly know what happened.");
    return;
}
