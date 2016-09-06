


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/


#include "stdlib.h"
#include "NasMmcPlmnSelectionStrategy.h"
#include "NasMmcFsmPlmnSelection.h"
#include "NasMmlLib.h"
#include "NasComm.h"
#include "NasMmcCtx.h"
#include "NasMmcSndOm.h"
#include "NasMmcComFunc.h"
#include "NasMmcProcNvim.h"
#include "NasMmcFsmBgPlmnSearchTbl.h"

#include "MmaAppLocal.h"
#include "TafMmaNetworkNameTbl.h"
#include "NasMmcSndLmm.h"

#include "NasMmcFsmMainTbl.h"
#include "NasUsimmApi.h"

#include "NasMmlCtx.h"
#include "NasMmcFsmBgPlmnSearch.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMC_PLMN_SELECTION_STRATEGY_C

/* ��������������Ӧ�������б���ʼ��������������״̬����ʼ��ʱʹ�� */
NAS_MMC_PLMN_SELECTION_LIST_INIT_TBL_STRU         g_astNasMmcPlmnSearchInitTbl[] =
{
    {NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON,                   NAS_MMC_InitPlmnSelectionList_SwitchOn},
    {NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST,                   NAS_MMC_InitPlmnSelectionList_AreaLost},
    {NAS_MMC_PLMN_SEARCH_SCENE_SYSCFG_SET,                  NAS_MMC_InitPlmnSelectionList_SyscfgSet},
    {NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED,     NAS_MMC_InitPlmnSelectionList_AvailTimerExpire},
    {NAS_MMC_PLMN_SEARCH_SCENE_USER_LIST,                   NAS_MMC_InitPlmnSelectionList_UserList},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_PLMN_SEARCH,         NAS_MMC_InitPlmnSelectionList_RegRejPlmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_CURRENT_PLMN_SEARCH, NAS_MMC_InitPlmnSelectionList_RegRejCurPlmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,       NAS_MMC_InitPlmnSelectionList_UserSpecPlmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_MANUAL_CHANGE_AUTO_MODE,     NAS_MMC_InitPlmnSelectionList_ManualChangeAutoMode},
    {NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,       NAS_MMC_InitPlmnSelectionList_HighPrioPlmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE,                 NAS_MMC_InitPlmnSelectionList_DisableLte},
    {NAS_MMC_PLMN_SEARCH_SCENE_INTER_SYS_OOS,               NAS_MMC_InitPlmnSelectionList_InterSysOos},
#if (FEATURE_ON == FEATURE_LTE)
    {NAS_MMC_PLMN_SEARCH_SCENE_CSFB_SERVIEC_REJ,       NAS_MMC_InitPlmnSelectionListCsfbServiceRej},
#endif
    {NAS_MMC_PLMN_SEARCH_SCENE_USER_AUTO_RESEL,             NAS_MMC_InitPlmnSelectionList_UserAutoResel},

    {NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_NCELL_SEARCH, NAS_MMC_InitPlmnSelectionList_AvailTimerExpireNcellSearch},

    {NAS_MMC_PLMN_SEARCH_SCENE_RF_ENABLE,                   NAS_MMC_InitPlmnSelectionListRfEnable},

    {NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_RAT_HPLMN_SEARCH,  NAS_MMC_InitPlmnSelectionList_HighPrioRatHplmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_NCELL,                   NAS_MMC_InitPlmnSelectionList_RegNCell},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_HPLMN,                   NAS_MMC_InitPlmnSelectionList_RegHPlmn},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_PREF_PLMN,               NAS_MMC_InitPlmnSelectionList_RegPrefPlmn},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_ANY_PLMN,                NAS_MMC_InitPlmnSelectionList_AvailTimerExpire},


    {NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST_ROAMING_CFG,       NAS_MMC_InitPlmnSelectionList_Roaming_AreaLost},

    {NAS_MMC_PLMN_SEARCH_SCENE_SRV_TRIG_PLMN_SEARCH,        NAS_MMC_InitPlmnSelectionListRfEnable},

#if (FEATURE_ON == FEATURE_LTE)
    {NAS_MMC_PLMN_SEARCH_SCENE_ENABLE_LTE_PLMN_SEARCH,      NAS_MMC_InitPlmnSelectionList_EnableLte},
    {NAS_MMC_PLMN_SEARCH_SCENE_CSFB_ABORT,                  NAS_MMC_InitPlmnSelectionList_CsfbAbort},
#endif

#if (FEATURE_ON == FEATURE_LTE)
    {NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_LTE_PLMN_SEARCH,      NAS_MMC_InitPlmnSelectionList_HighPrioLtePlmnSearch},
#endif

#if (FEATURE_ON == FEATURE_CSG)
    {NAS_MMC_PLMN_SEARCH_SCENE_USER_CSG_SPEC_SEARCH, NAS_MMC_InitPlmnSelectionList_UserCsgSpecSearch},

    {NAS_MMC_PLMN_SEARCH_SCENE_ASF_CSG_SEARCH,       NAS_MMC_InitPlmnSelectionList_ASFCsgSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_PSF_CSG_SEARCH,       NAS_MMC_InitPlmnSelectionList_PSFCsgSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_CSG_ID_UNAVAILABLE_SEARCH, NAS_MMC_InitPlmnSelectionList_CsgIdUnavailableSearch},
#endif

    {NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_HISTORY_PLMN_SEARCH,     NAS_MMC_InitPlmnSelectionList_AvailTimerExpire},

    {NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_PREFBAND_PLMN_SEARCH,    NAS_MMC_InitPlmnSelectionList_AvailTimerExpire},

    {NAS_MMC_PLMN_SEARCH_SCENE_CSFB_END,   NAS_MMC_InitPlmnSelectionList_CsfbEnd},

    {NAS_MMC_PLMN_SEARCH_SCENE_DPLMN_SET,  NAS_MMC_InitPlmnSelectionList_DplmnSet},

    {NAS_MMC_PLMN_SEARCH_SCENE_MO_CALL_ABORT_FR, NAS_MMC_InitPlmnSelectionList_MoCallAbortFR},

    {NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH, NAS_MMC_InitPlmnSelectionList_BgHistory},
    {NAS_MMC_PLMN_SEARCH_SCENE_PREFBAND_LIST_PLMN_SEARCH, NAS_MMC_InitPlmnSelectionList_PrefBandList},
    {NAS_MMC_PLMN_SEARCH_SCENE_FULLBAND_LIST_PLMN_SEARCH, NAS_MMC_InitPlmnSelectionList_FullBandList},
};

/* ��Ҫ�Ƚ���history�������������� */
VOS_UINT32 g_aulNasMmcNeedHistorySearchSceneTbl[] =
{
    NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED,
    NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_CSFB_SERVIEC_REJ,
    NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE,
    NAS_MMC_PLMN_SEARCH_SCENE_SRV_TRIG_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_HISTORY_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_PREFBAND_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_PREFBAND_LIST_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_FULLBAND_LIST_PLMN_SEARCH,

    NAS_MMC_PLMN_SEARCH_SCENE_MO_CALL_ABORT_FR,
    NAS_MMC_PLMN_SEARCH_SCENE_CSFB_END,
};


/* ��Ҫ�Ƚ���pref band�������������� */
VOS_UINT32 g_aulNasMmcNeedPrefBandSearchSceneTbl[] =
{
    NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_PREFBAND_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_PREFBAND_LIST_PLMN_SEARCH,
};


#if (FEATURE_ON == FEATURE_CSG)
/* ���������б�ʱ��Ҫʹ��csg duplicate rplmn��eplmn��Ϣ���������� */
VOS_UINT32 g_aulNasMmcNeedUseDuplicateRPlmnAndEplmnSearchSceneTbl[] =
{
    NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON,
    NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED,
    NAS_MMC_PLMN_SEARCH_SCENE_CSFB_SERVIEC_REJ,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_HISTORY_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_PREFBAND_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_PREFBAND_LIST_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_FULLBAND_LIST_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_USER_CSG_SPEC_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_CSG_ID_UNAVAILABLE_SEARCH,
};

#endif

VOS_UINT32 g_aulNasMmcNeedSortRoamPlmnSelectionListSceneTbl[] =
{
    NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON,
    NAS_MMC_PLMN_SEARCH_SCENE_SYSCFG_SET,
    NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_PREFBAND_LIST_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_FULLBAND_LIST_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_PLMN_SEARCH,
};

VOS_UINT32 g_aulNasMmcNeedSetHistorySearchedFlagSceneTbl[] =
{
    NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED,
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_PREFBAND_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_PREFBAND_LIST_PLMN_SEARCH,
    NAS_MMC_PLMN_SEARCH_SCENE_FULLBAND_LIST_PLMN_SEARCH,
};

/*lint -save -e958 */

VOS_UINT16 NAS_MMC_GetNeedHistorySearchSceneTblSize(VOS_VOID)
{
    VOS_UINT16                          usTblSize;

    usTblSize = sizeof(g_aulNasMmcNeedHistorySearchSceneTbl) / sizeof(g_aulNasMmcNeedHistorySearchSceneTbl[0]);

    return usTblSize;
}


VOS_UINT32 *NAS_MMC_GetNeedHistorySearchSceneTblAddr(VOS_VOID)
{
    return g_aulNasMmcNeedHistorySearchSceneTbl;
}


VOS_UINT32 NAS_MMC_IsSpecSearchSceneInNeedHistorySearchSceneTbl(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enSearchScene
)
{
    VOS_UINT16                          usTblSize;
    VOS_UINT32                          ulIndex;

    usTblSize = NAS_MMC_GetNeedHistorySearchSceneTblSize();

    for (ulIndex = 0; ulIndex < usTblSize; ulIndex++)
    {
        if (enSearchScene == g_aulNasMmcNeedHistorySearchSceneTbl[ulIndex])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT16 NAS_MMC_GetNeedSortRoamPlmnSelectionListSceneTblSize(VOS_VOID)
{
    VOS_UINT16                          usTblSize;

    usTblSize = sizeof(g_aulNasMmcNeedSortRoamPlmnSelectionListSceneTbl) / sizeof(g_aulNasMmcNeedSortRoamPlmnSelectionListSceneTbl[0]);

    return usTblSize;
}


VOS_UINT32 NAS_MMC_IsPlmnSearchSceneInNeedSortRoamPlmnSelectionListSceneTbl(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene
)
{
    VOS_UINT16                          usTblSize;
    VOS_UINT32                          ulIndex;

    usTblSize = NAS_MMC_GetNeedSortRoamPlmnSelectionListSceneTblSize();

    for (ulIndex = 0; ulIndex < usTblSize; ulIndex++)
    {
        if (enPlmnSearchScene == g_aulNasMmcNeedSortRoamPlmnSelectionListSceneTbl[ulIndex])
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_UINT16 NAS_MMC_GetNeedSetHistorySearchedFlagSceneTblSize(VOS_VOID)
{
    VOS_UINT16                          usTblSize;

    usTblSize = sizeof(g_aulNasMmcNeedSetHistorySearchedFlagSceneTbl) / sizeof(g_aulNasMmcNeedSetHistorySearchedFlagSceneTbl[0]);

    return usTblSize;
}


VOS_UINT32 NAS_MMC_IsPlmnSearchSceneInNeedSetHistorySearchedFlagSceneTbl(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene
)
{
    VOS_UINT16                          usTblSize;
    VOS_UINT32                          ulIndex;

    usTblSize = NAS_MMC_GetNeedSetHistorySearchedFlagSceneTblSize();

    for (ulIndex = 0; ulIndex < usTblSize; ulIndex++)
    {
        if (enPlmnSearchScene == g_aulNasMmcNeedSetHistorySearchedFlagSceneTbl[ulIndex])
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)


VOS_UINT32 NAS_MMC_GetMsccTriggerPlmnSrchTypeInClMode_PlmnSelection(
    NAS_MMC_PLMN_SEARCH_TYPE_ENUM_UINT32                   *penSrchType
)
{
    NAS_MMC_ENTRY_MSG_STRU                                 *pstEntryMsg           = VOS_NULL_PTR;
    MSCC_MMC_PLMN_SEARCH_REQ_STRU                          *pstMsccPlmnSrchReq    = VOS_NULL_PTR;

    /* ��ȡ״̬���������Ϣ��Ϣ */
    pstEntryMsg  = NAS_MMC_GetCurrEntryMsg();

    *penSrchType = NAS_MMC_PLMN_SEARCH_TYPE_SPEC;

    if (VOS_FALSE == NAS_MML_IsGULModeConfigured())
    {
        if (pstEntryMsg->ulEventType == NAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_MMC_PLMN_SEARCH_REQ))
        {
            pstMsccPlmnSrchReq = (MSCC_MMC_PLMN_SEARCH_REQ_STRU *)pstEntryMsg->aucEntryMsgBuffer;

            if (NAS_MSCC_PIF_PLMN_SEARCH_HISTORY == pstMsccPlmnSrchReq->enSrchType)
            {
                *penSrchType = NAS_MMC_PLMN_SEARCH_TYPE_HISTORY;
            }
            else if (NAS_MSCC_PIF_PLMN_SEARCH_PREF_BAND == pstMsccPlmnSrchReq->enSrchType)
            {
                *penSrchType = NAS_MMC_PLMN_SEARCH_TYPE_PREFBAND;
            }
            else
            {
                *penSrchType = NAS_MMC_PLMN_SEARCH_TYPE_SPEC;
            }

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


#endif


VOS_UINT16 NAS_MMC_GetNeedPrefBandSearchSceneTblSize(VOS_VOID)
{
    VOS_UINT16                          usTblSize;

    usTblSize = sizeof(g_aulNasMmcNeedPrefBandSearchSceneTbl) / sizeof(g_aulNasMmcNeedPrefBandSearchSceneTbl[0]);

    return usTblSize;
}


VOS_UINT32 *NAS_MMC_GetNeedPrefBandSearchSceneTblAddr(VOS_VOID)
{
    return g_aulNasMmcNeedPrefBandSearchSceneTbl;
}


VOS_UINT32 NAS_MMC_IsSpecSearchSceneInNeedPrefBandSearchSceneTbl(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enSearchScene
)
{
    VOS_UINT16                          usTblSize;
    VOS_UINT32                          ulIndex;

    usTblSize = NAS_MMC_GetNeedPrefBandSearchSceneTblSize();

    for (ulIndex = 0; ulIndex < usTblSize; ulIndex++)
    {
        if (enSearchScene == g_aulNasMmcNeedPrefBandSearchSceneTbl[ulIndex])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_BuildRatPrioList_AreaLost(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstDestRatRrioList
)
{
    VOS_UINT32                                              i;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enCurRatType;                                                  /* ����Ľ��뼼�� */
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MMC_NON_OOS_PLMN_SEARCH_FEATURE_SUPPORT_CFG_STRU   *pstNonOosCfg  = VOS_NULL_PTR;

    pstNonOosCfg   = NAS_MMC_GetNonOosPlmnSearchFeatureSupportCfg();
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    enCurRatType   = NAS_MML_GetCurrNetRatType();

    PS_MEM_CPY(pstDestRatRrioList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �������PlmnList��Ϣ����ԭ�еĴ���һ�£��ѵ�ǰģ��������� */
    if (VOS_NULL_PTR == pstSearchedPlmnListInfo)
    {
        NAS_MML_SortSpecRatPrioLowest(enCurRatType, pstDestRatRrioList);
        return;
    }

    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (pstSearchedPlmnListInfo[i].enRatType != enCurRatType)
        {
            continue;
        }

        /* ���������ϱ�����ʱ�Ѿ����й�spec�ѻ�pref band�ѣ�����NV���ò��ѵ�ǰģ��ѵ�ǰģ��������� */
        if ((VOS_TRUE == pstSearchedPlmnListInfo[i].stSearchedType.ucPrefBandSearchedFlg)
         || (VOS_TRUE == pstSearchedPlmnListInfo[i].stSearchedType.ucSpecSearchedFlg))
        {
            /* NV����������ǰģ��spec�ѻ�pref band��ʱ���ѵ�ǰģ���б���ɾ�� */
            if (NAS_MMC_PLMN_SEARCH_TYPE_AFTER_HISTORY_NONE == pstNonOosCfg->enSearchTypeAfterHistoryInAreaLostScene)
            {
                NAS_MML_DelSpecRatInPrioRatList(enCurRatType, pstDestRatRrioList);
            }
            /* NV���ò�������ǰģ��spec�ѻ�pref band��ʱ���ѵ�ǰģ��������� */
            else
            {
                NAS_MML_SortSpecRatPrioLowest(enCurRatType, pstDestRatRrioList);
            }

            return;
        }

        /* ��������δ�����κ����������ϱ�������Ҫ�����ѵ�ǰģ��
           ���������ϱ�����ʱֻ���й�history�ѣ���ǰģ�ȷ�����ǰ�棬���б�־��¼��ǰģ�ѽ��й�history��history��ʱ������
        */
        NAS_MML_SortSpecRatPrioHighest(enCurRatType, pstDestRatRrioList);
        break;
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                          i;
    pNasMmcPlmnSelectionListInitFunc    pfunPlmnSearchInit;

    pfunPlmnSearchInit = VOS_NULL_PTR;

    /* ���ݳ�����g_astNasMmcPlmnSearchInitTbl���������б���ʼ������ */
    for (i = 0; i < (sizeof(g_astNasMmcPlmnSearchInitTbl)/sizeof(NAS_MMC_PLMN_SELECTION_LIST_INIT_TBL_STRU)); i++)
    {
        if (enPlmnSearchScene == g_astNasMmcPlmnSearchInitTbl[i].enPlmnSearchScene)
        {
            pfunPlmnSearchInit = g_astNasMmcPlmnSearchInitTbl[i].pfunPlmnSearchInit;
            break;
        }
    }

    /* ���ҳɹ�ʱ���ó�ʼ���������г�ʼ�� */
    if (VOS_NULL_PTR != pfunPlmnSearchInit)
    {
        PS_MEM_SET(pstPlmnSelectionListInfo, 0x0, sizeof(NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU));

        pfunPlmnSearchInit(pstPlmnSelectionListInfo, pstSearchedPlmnListInfo);
    }

    /* �û��б�����ʱ,��ʼ����Ϻ���Ҫ�����������б����и���,
       ��˴˴��ݲ�Log��Ϣ,��������Ϻ�Log */
    if (NAS_MMC_PLMN_SEARCH_SCENE_USER_LIST != enPlmnSearchScene)
    {
        /* ��ά�ɲ⣬���ѡ���б���Ϣ */
        NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionListInManualMode_SwitchOn(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enNvimLastRplmnRat
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_HPLMN_CFG_INFO_STRU        *pstHPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                stRPlmn;
    VOS_UINT32                          ulEplmnValidFlg;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
    PS_MEM_SET(&stRPlmn, 0, sizeof(NAS_MML_PLMN_ID_STRU));

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ȡHPLMN��������Ϣ */
    pstHPlmnCfg    = NAS_MML_GetHplmnCfg();

    /* ��ȡRPLMN��������Ϣ */
    pstRPlmnCfg    = NAS_MML_GetRplmnCfg();

    enNvimLastRplmnRat = pstRPlmnCfg->enLastRplmnRat;

    /* ��ȡ�û�ָ������ */
    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

#if (FEATURE_ON == FEATURE_CSG)
    if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON))
    {
        /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
        NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
        NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
        NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
#endif
    {

        /* last Rplmn rat����,��û�л������ڻ���ʱ��LastRplmnRatΪBUTTֵ */
        if ( (VOS_TRUE                  == pstRPlmnCfg->ucLastRplmnRatFlg)
          && (NAS_MML_NET_RAT_TYPE_BUTT != enNvimLastRplmnRat) )
        {
            /* ��ѡ���б��������û�ָ��PLMN,ֻ����last Rplmn ratָʾ�Ľ��뼼�� */
            stPrioRatList.ucRatNum      = 1;
            stPrioRatList.aucRatPrio[0] = pstRPlmnCfg->enLastRplmnRat;
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б��������û�ָ��PLMN */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* �û�ָ������ע��ɹ���,��������EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionListInManualMode_SwitchOn: Add EPLMN in PlmnSelectionList");
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
        }
    }

    /* ֧��R7��������Hplmn */
    if (VOS_TRUE == pstHPlmnCfg->ucManualSearchHplmnFlg)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionListInManualMode_SwitchOn: Add EHPLMN in PlmnSelectionList");
        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionListInManualMode_AreaLost(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    VOS_UINT32                                              ulIsSortPlmnSearchList,
    VOS_UINT32                                              ulIsLTESearched
)
{
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();


    /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
    ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

    if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
     && (VOS_TRUE == ulEplmnValidFlg))
    {
#if (FEATURE_ON == FEATURE_CSG)
        if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST))
        {
            /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), pstPrioRatList);
            NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);
            NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
#endif
        {
            /* ���뼼��˳��ĵ�������ǰ�� */

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       pstPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* ��ȡ�û�ָ������ */
        pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

        /* ���뼼��˳��ĵ�������ǰ�� */

        NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                               NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                               pstPrioRatList,
                                               pstPlmnSelectionListInfo);
    }
    if (VOS_TRUE == ulIsSortPlmnSearchList)
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_WCDMA,
                                                pstPlmnSelectionListInfo);
        if (VOS_TRUE == ulIsLTESearched)
        {
            NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE,
                                                    pstPlmnSelectionListInfo);
        }
    }

    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_SwitchOn(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                    stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU                   *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_HPLMN_CFG_INFO_STRU                  *pstHPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_RPLMN_CFG_INFO_STRU                  *pstRPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                   *pstGeoPlmn      = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                          stRPlmn;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU          stSearchedExistPlmnInfo[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                                    ulIsExistRplmnOrHplmn;
    VOS_UINT32                                    ulIsSearedPlmnValid;

    NAS_MML_NET_RAT_TYPE_ENUM_UINT8             enNvimLastRplmnRat;             /* NVIM�б�����ϴιػ�ǰRPLMN�Ľ��뼼�� */

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
    PS_MEM_SET(&stRPlmn, 0, sizeof(NAS_MML_PLMN_ID_STRU));

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ȡHPLMN��������Ϣ */
    pstHPlmnCfg    = NAS_MML_GetHplmnCfg();

    /* ��ȡRPLMN��������Ϣ */
    pstRPlmnCfg    = NAS_MML_GetRplmnCfg();

    enNvimLastRplmnRat = pstRPlmnCfg->enLastRplmnRat;
    NAS_MMC_ReadNvimLastRplmnRat(&enNvimLastRplmnRat);

    pstGeoPlmn  = NAS_MMC_GetGeoPlmn();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        if ( VOS_TRUE == NAS_MMC_IsAisRoamingEffect())
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_SwitchOn AisRoamingEffect is true");

            /* ��ѡ���б�����������Ҫ��������������*/
            NAS_MMC_AddAisRoamPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }


        /* ֧����Ӫ��P4�Ŀ�������Hplmn����֧����������Hplmn */
        if ((VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
         || (VOS_TRUE == pstHPlmnCfg->ucHplmnSearchPowerOn))
        {
            NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_SwitchOn HPLMN IS PRIO, ucHplmnSearchPowerOn is; NAS_MMC_IsHPlmnPrioSearch is;",
                                            pstHPlmnCfg->ucHplmnSearchPowerOn, NAS_MMC_IsHPlmnPrioSearch());

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }

        else if ( (VOS_TRUE                     == pstRPlmnCfg->ucLastRplmnRatFlg)
               && (NAS_MML_NET_RAT_TYPE_BUTT    != enNvimLastRplmnRat) ) /* last Rplmn rat���� */
        {
#if (FEATURE_ON == FEATURE_CSG)
            if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON))
            {
                /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
                NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
                NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
                NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
            }
            else
#endif
            {
                /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
                NAS_MML_SortSpecRatPrioHighest(pstRPlmnCfg->enLastRplmnRat, &stPrioRatList);
                NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

                /* ��ѡ���б�������EPLMN */
                NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);
            }

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
#if (FEATURE_ON == FEATURE_CSG)
            if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON))
            {
                /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
                NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
                NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
                NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
            }
            else
#endif
            {
                /* ��ѡ���б�������RPLMN */
                NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

                /* ��ѡ���б�������EPLMN */
                NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);
            }

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        NAS_MMC_InitPlmnSelectionListInManualMode_SwitchOn(pstPlmnSelectionListInfo, enNvimLastRplmnRat);
    }


    /* ��NAS_MML_PLMN_WITH_RAT_STRU�ṹ������ת��ΪNAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU�ṹ������ */
    PS_MEM_SET(stSearchedExistPlmnInfo, 0, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));
    stSearchedExistPlmnInfo[0].enRatType                 = pstGeoPlmn->enRat;
    stSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMcc  = pstGeoPlmn->stPlmnId.ulMcc;
    stSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMnc  = pstGeoPlmn->stPlmnId.ulMnc;
    stSearchedExistPlmnInfo[0].ulHighPlmnNum             = 1;

    /* ����������PLMN�б��м���Ƿ����Rplmn����Hplmn */
    ulIsExistRplmnOrHplmn = NAS_MMC_IsExistRplmnOrHplmnFlag_InitPlmnSelectionList(stSearchedExistPlmnInfo);

    /* �ѵ���PLMN�Ƿ���Ч */
    ulIsSearedPlmnValid   = NAS_MMC_IsSearchedExistPlmnValid(stSearchedExistPlmnInfo);

    /* �������Rplmn����Hplmn�ñ�־λ */
    if (VOS_TRUE == ulIsExistRplmnOrHplmn)
    {
        NAS_MMC_SetExistRplmnOrHplmnFlag_PlmnSelection(VOS_TRUE);
    }

    /* �������� NV�򿪲���MCC��RPLMN/HPLMN��ͬʱ�����������������б���ǰ�� */
    if ( (VOS_TRUE == NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON, ulIsExistRplmnOrHplmn))
      && (VOS_TRUE == ulIsSearedPlmnValid) )
    {
        /* ��ѡ���б��������� */
        NAS_MMC_SortRoamPlmnSelectionList_InitPlmnSelectionList(pstGeoPlmn, pstPlmnSelectionListInfo);
    }
    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_AreaLost(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enNetRatType;
    VOS_UINT32                          ulIsRoam;
    VOS_UINT32                          ulIsSortPlmnSearchList;
    VOS_UINT32                          ulIsLTESearched;
#endif

#if (FEATURE_ON == FEATURE_CSG)
    NAS_MML_PLMN_RAT_PRIO_STRU          stCsgPrioRatList;

    PS_MEM_SET(&stCsgPrioRatList, 0, sizeof(stCsgPrioRatList));
#endif

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));


    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

#if (FEATURE_ON == FEATURE_LTE)
    ulIsSortPlmnSearchList      = VOS_FALSE;
    enNetRatType                = NAS_MML_GetCurrNetRatType();
    ulIsRoam                    = NAS_MML_IsLastCampedPlmnIdRoam();
    ulIsLTESearched             = NAS_MML_IsLteSearched_AreaLost(pstSearchedPlmnListInfo);

    if ((VOS_TRUE == NAS_MML_IsNeedSortRatPrioForLteOos2GPrefPlmnSel(&stPrioRatList))
     && (NAS_MML_NET_RAT_TYPE_LTE == enNetRatType)
     && (VOS_FALSE == ulIsRoam))
    {
        ulIsSortPlmnSearchList = VOS_TRUE;
    }
#endif

    /* ����NV���õ��������Ե���������˳�� */
    NAS_MMC_BuildRatPrioList_AreaLost(pstSearchedPlmnListInfo, &stPrioRatList);

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ���뼼��˳��ĵ�������ǰ�� */

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);


        }
        else
        {
#if (FEATURE_ON == FEATURE_CSG)
            if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST))
            {
                /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
                NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
                NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
                NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
            }
            else
#endif
            {
                /* ���뼼��˳��ĵ�������ǰ�� */

                /* ��ѡ���б�������RPLMN */
                NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

                /* ��ѡ���б�������EPLMN */
                NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);
            }

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        if(VOS_TRUE == ulIsSortPlmnSearchList)
        {
            NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_WCDMA,
                                                    pstPlmnSelectionListInfo);
            if (VOS_TRUE == ulIsLTESearched)
            {
                NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE,
                                                        pstPlmnSelectionListInfo);
            }
        }
        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        NAS_MMC_InitPlmnSelectionListInManualMode_AreaLost(pstPlmnSelectionListInfo,
                                                           &stPrioRatList,
                                                           ulIsSortPlmnSearchList,
                                                           ulIsLTESearched);
    }

    /* ������������ʱ�������ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_Roaming_AreaLost(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;

    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /*���ѵ�ǰ���뼼��*/
        PS_MEM_SET(&stPrioRatList, 0x0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
        stPrioRatList.ucRatNum = 1;
        stPrioRatList.aucRatPrio[0] = NAS_MML_GetCurrNetRatType();
        /* ��ѡ���б�������RPLMN,���ѵ�ǰ���뼼�� */
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

    }
    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_InterSysOos(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��OOS���̽����������ϱ���PLMN���������б� */
    NAS_MMC_AddSearchedPlmnListInfoInPlmnSelectionList(pstPlmnSelectionListInfo, pstSearchedPlmnListInfo);

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
        NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

    }


    /* �����ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_UserAutoResel(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    /* ��ѡ���б�������UPLMN��OPLMN */
    NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);


    /* פ���ڷ�EHPLMNʱ���򽫵�ǰ����������� */
    if (VOS_TRUE == NAS_MMC_IsRoam())
    {
        /* ɾ����ǰפ�������� */
        NAS_MMC_DelSpecPlmnWithRatInPlmnList(NAS_MML_GetCurrCampPlmnId(),
                                             NAS_MML_GetCurrNetRatType(),
                                             pstPlmnSelectionListInfo);
    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}


NAS_MML_NET_RAT_TYPE_ENUM_UINT8 NAS_MMC_GetPrioRat_SyscfgSet(VOS_VOID)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg     = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrNetRatType;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;

    enRatType        = NAS_MML_NET_RAT_TYPE_BUTT;
    pstRPlmnCfg      = NAS_MML_GetRplmnCfg();
    enCurrNetRatType = NAS_MML_GetCurrNetRatType();
    pstPrioRatList   = NAS_MML_GetMsPrioRatList();

    /* SVLTE���Կ��Ļ�����ǰפ���Ľ��뼼������ */
    if ( (VOS_TRUE == NAS_MML_GetSvlteSupportFlag())
      && (NAS_MML_NET_RAT_TYPE_BUTT != enCurrNetRatType))
    {
        enRatType = enCurrNetRatType;
    }
    else
    {
        /* SVLTE���Թرգ���ԭ���߼����� */
        if ( VOS_TRUE == NAS_MML_GetSyscfgTriHighRatSrchFlg() )
        {
            enRatType = pstPrioRatList->aucRatPrio[0];
        }
        else if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
        {
            enRatType = pstRPlmnCfg->enLastRplmnRat;
        }
        else
        {
            ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

            if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
             && (VOS_TRUE == ulEplmnValidFlg))
            {
                enRatType = pstRPlmnCfg->enLastRplmnRat;
            }
            else
            {
                pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();
                enRatType       = pstUserSpecPlmn->enRat;
            }
        }
    }

    return enRatType;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_SyscfgSet(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU          *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           *pEplmnAddr      = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU          *pstGeoPlmn      = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU           stPrioRatList;
    VOS_UINT32                           ulEplmnValidFlg;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8      enRatType;

    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU stSearchedExistPlmnInfo[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                           ulIsExistRplmnOrHplmn;
    VOS_UINT32                           ulIsSearedPlmnValid;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstGeoPlmn  = NAS_MMC_GetGeoPlmn();

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));


    enRatType = NAS_MMC_GetPrioRat_SyscfgSet();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
        NAS_MML_SortSpecRatPrioHighest(enRatType, &stPrioRatList);
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(enRatType, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,���û�ָ����rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(enRatType, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }
    }

    /* ��NAS_MML_PLMN_WITH_RAT_STRU�ṹ������ת��ΪNAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU�ṹ������ */
    PS_MEM_SET(stSearchedExistPlmnInfo, 0, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));
    stSearchedExistPlmnInfo[0].enRatType                 = pstGeoPlmn->enRat;
    stSearchedExistPlmnInfo[0].ulHighPlmnNum             = 1;
    stSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMcc  = pstGeoPlmn->stPlmnId.ulMcc;
    stSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMnc  = pstGeoPlmn->stPlmnId.ulMnc;

    /* ����������PLMN�б��м���Ƿ����Rplmn����Hplmn */
    ulIsExistRplmnOrHplmn = NAS_MMC_IsExistRplmnOrHplmnFlag_InitPlmnSelectionList(stSearchedExistPlmnInfo);

    /* �ѵ���PLMN�Ƿ���Ч */
    ulIsSearedPlmnValid   = NAS_MMC_IsSearchedExistPlmnValid(stSearchedExistPlmnInfo);

    /* �������Rplmn����Hplmn�ñ�־λ */
    if (VOS_TRUE == ulIsExistRplmnOrHplmn)
    {
        NAS_MMC_SetExistRplmnOrHplmnFlag_PlmnSelection(VOS_TRUE);
    }

    /* �������� NV�򿪲���MCC��RPLMN/HPLMN��ͬʱ�����������������б���ǰ�� */
    if ( (VOS_TRUE == NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_SYSCFG_SET, ulIsExistRplmnOrHplmn))
      && (VOS_TRUE == ulIsSearedPlmnValid) )
    {
        /* ��ѡ���б��������� */
        NAS_MMC_SortRoamPlmnSelectionList_InitPlmnSelectionList(pstGeoPlmn, pstPlmnSelectionListInfo);
    }

    /* �����ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    return;

}


VOS_VOID NAS_MMC_BuildNcellPlmnSelectionPlmnList(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    VOS_UINT8                                               ucChangePrioRatOrder,
    NAS_MML_PLMN_ID_STRU                                   *pstPlmnId,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT8                                               ucRatNum;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstRatPrioList = VOS_NULL_PTR;
    VOS_UINT32                                              ulRatCnt;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;

    PS_MEM_CPY(&(stPlmnInfo.stPlmnId), pstPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
    stPlmnInfo.enPlmnType = enPlmnType;

    /* ��ȡ���ȼ��б� */
    pstRatPrioList     = NAS_MML_GetMsPrioRatList();

    for (ulRatCnt =0; ulRatCnt< pstRatPrioList->ucRatNum; ulRatCnt++)
    {
        ucRatNum                = 0x0;
        /* ��ʼ���ϱ��Ľ��뼼���б� */
        if (VOS_FALSE == ucChangePrioRatOrder)
        {
            enRatType        = pstRatPrioList->aucRatPrio[ulRatCnt];
        }

        /*���򹹽������б�*/
        else
        {
            enRatType        = pstRatPrioList->aucRatPrio[pstRatPrioList->ucRatNum -1 - ulRatCnt];
        }


        if (NAS_MML_NET_RAT_TYPE_LTE == enRatType)
        {
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_LTE;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
            ucRatNum++;
        }

        if (NAS_MML_NET_RAT_TYPE_WCDMA == enRatType)
        {
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_WCDMA;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
            ucRatNum++;
        }
        stPlmnInfo.ucRatNum = ucRatNum;

        /* ����ʣ��λ�ø�ֵΪ��Чֵ */
        for (; ucRatNum < NAS_MML_MAX_RAT_NUM; ucRatNum++)
        {
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_BUTT;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_BUTT;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_BUTT;
            stPlmnInfo.astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        }

        /* ���ӵ������б� */
        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);

    }
    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_AvailTimerExpireNcellSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_SIM_EHPLMN_INFO_STRU        *pstEHPlmnInfo     = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU          *pstUserSpecPlmn   = VOS_NULL_PTR;
    VOS_UINT8                            ucNeedChangeRatOrder;

    /* �ж��Ƿ���Ҫ����˳��*/
    ucNeedChangeRatOrder = NAS_MMC_IsNeedChangeNcellSearchRatOrder();

    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();
    pstEHPlmnInfo   = NAS_MML_GetSimEhplmnList();


    /* �Զ�����ģʽ ���� HPLMN ���б���*/
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        NAS_MMC_BuildNcellPlmnSelectionPlmnList(NAS_MMC_PLMN_TYPE_HPLMN,
                                       ucNeedChangeRatOrder,
                                       &(pstEHPlmnInfo->astEhPlmnInfo[0].stPlmnId),
                                       pstPlmnSelectionListInfo);

    }

    else
    {
        NAS_MMC_BuildNcellPlmnSelectionPlmnList(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                       ucNeedChangeRatOrder,
                                       &(pstUserSpecPlmn->stPlmnId),
                                       pstPlmnSelectionListInfo);
    }

    return;
}

#if (FEATURE_ON == FEATURE_CSG)


VOS_UINT16 NAS_MMC_GetNeedUseDuplicateRPlmnAndEplmnSearchSceneTblSize(VOS_VOID)
{
    VOS_UINT16                          usTblSize;

    usTblSize = sizeof(g_aulNasMmcNeedUseDuplicateRPlmnAndEplmnSearchSceneTbl) / sizeof(g_aulNasMmcNeedUseDuplicateRPlmnAndEplmnSearchSceneTbl[0]);

    return usTblSize;
}



VOS_UINT32 NAS_MMC_IsSpecSearchSceneInNeedUseDuplicateRPlmnAndEplmnSearchSceneTbl(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enSearchScene
)
{
    VOS_UINT16                          usTblSize;
    VOS_UINT32                          ulIndex;

    usTblSize = NAS_MMC_GetNeedUseDuplicateRPlmnAndEplmnSearchSceneTblSize();

    for (ulIndex = 0; ulIndex < usTblSize; ulIndex++)
    {
        if (enSearchScene == g_aulNasMmcNeedUseDuplicateRPlmnAndEplmnSearchSceneTbl[ulIndex])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

VOS_VOID NAS_MMC_InitPlmnSelectionList_CsgIdUnavailableSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pEplmnAddr      = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;
    VOS_UINT32                                              ulEplmnValidFlg;
    NAS_MML_RPLMN_CFG_INFO_STRU                            *pstRPlmnCfg     = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnSelectionInfo;
    VOS_UINT32                                              ulIsNeedAddDuplicateRplmnAndEplmn;

    pEplmnAddr                        = NAS_MML_GetEquPlmnList();
    pstRPlmnCfg                       = NAS_MML_GetRplmnCfg();
    pstPrioRatList                    = NAS_MML_GetMsPrioRatList();
    ulIsNeedAddDuplicateRplmnAndEplmn = VOS_FALSE;

    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
    PS_MEM_SET(&stPlmnSelectionInfo, 0, sizeof(stPlmnSelectionInfo));

    if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_USER_CSG_SPEC_SEARCH))
    {
        ulIsNeedAddDuplicateRplmnAndEplmn = VOS_TRUE;
    }

    /* �Զ�����ģʽ�£�rplmn(last rplmn���뼼������ǰ��)+eplmn+hplmn+uplmn+oplmn */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        if (VOS_TRUE == ulIsNeedAddDuplicateRplmnAndEplmn)
        {
            /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
            NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
            NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(pstRPlmnCfg->enLastRplmnRat, &stPrioRatList);
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        return;
    }

    /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
    ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

    if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
     && (VOS_TRUE == ulEplmnValidFlg))
    {
        if (VOS_TRUE == ulIsNeedAddDuplicateRplmnAndEplmn)
        {
            /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
            NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
            NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(pstRPlmnCfg->enLastRplmnRat, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б��������û�ָ������ */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        return;
    }

    /* ��ȡ�û�ָ������ */
    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ��ѡ���б��������û�ָ��PLMN,���û�ָ����rat������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(pstUserSpecPlmn->enRat, &stPrioRatList);
    NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                           NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

    return;

}


VOS_VOID NAS_MMC_InitPlmnSelectionList_UserCsgSpecSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pEplmnAddr      = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stCsgPrioRatList;
    VOS_UINT32                                              ulEplmnValidFlg;
    NAS_MML_RPLMN_CFG_INFO_STRU                            *pstRPlmnCfg     = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnSelectionInfo;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnWithCsgId = VOS_NULL_PTR;
    VOS_UINT32                                              ulIsNormalServiceStatus;
    VOS_UINT32                                              ulIsNeedAddDuplicateRplmnAndEplmn;
    NAS_MML_CELL_TYPE_ENUM_UINT8                            enCellType;

    pstPlmnWithCsgId                  = NAS_MMC_GetCsgSpecSrchPlmnWithCsgId();
    pEplmnAddr                        = NAS_MML_GetEquPlmnList();
    pstRPlmnCfg                       = NAS_MML_GetRplmnCfg();
    pstPrioRatList                    = NAS_MML_GetMsPrioRatList();
    ulIsNormalServiceStatus           = NAS_MMC_IsNormalServiceStatus();
    ulIsNeedAddDuplicateRplmnAndEplmn = VOS_FALSE;
    enCellType                        = NAS_MML_GetCurrCampCellType();

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
    PS_MEM_SET(&stCsgPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
    PS_MEM_SET(&stPlmnSelectionInfo, 0, sizeof(stPlmnSelectionInfo));

    if ((VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_USER_CSG_SPEC_SEARCH))
     && (NAS_MML_CELL_TYPE_CSG == enCellType)
     && (VOS_TRUE == ulIsNormalServiceStatus))
    {
        ulIsNeedAddDuplicateRplmnAndEplmn = VOS_TRUE;
    }

    /* �Զ�����ģʽ�£�ָ��CSG ID LTE����+rplmn(last rplmn���뼼������ǰ��)+eplmn+hplmn+uplmn+oplmn */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б��м���LTE CSG ID������ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_USER_CSG_SPEC_PLMN,
                                           NAS_MML_SIM_RAT_E_UTRN,
                                           &pstPlmnWithCsgId->stPlmnWithRat.stPlmnId,
                                           &stPlmnSelectionInfo);

        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnSelectionInfo, pstPlmnSelectionListInfo);

        /* UEפ����PLMN1�ϴ�����������CSGפ�������û�����PLMN2��CSG ID��ָ����,�����б�����Ϊ:
        PLMN2(L)-> RPLMN PLMN1(last rplmn rat only)->duplicate RPLMN(duplicate Rplmn Rat)
        ->duplicate EPLMN->HPLMN->UPLMN->OPLMN,
        ���˳���趨�ĺô����ڣ����PLMN2(L)����ʧ�ܣ������������ϴγɹ�פ����CSG ID��PLMN1(L)��
        ���֮ǰ��PLMN1(L)Ҳ����ʧ�ܣ��������ȥ��duplicate RPLMN�ع����������Ǽ�����PLMN1��Uģ
        �����ܸ߸��ʲ����ڣ�*/
        if (VOS_TRUE == ulIsNeedAddDuplicateRplmnAndEplmn)
        {
            stCsgPrioRatList.ucRatNum = 1;
            stCsgPrioRatList.aucRatPrio[0] = NAS_MML_NET_RAT_TYPE_LTE;
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stCsgPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
            NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
            NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(pstRPlmnCfg->enLastRplmnRat, &stPrioRatList);
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        return;
    }

    /* �ֶ�����ģʽ�£�ָ��CSG ID LTE����+�û�ָ�����磨last rplmn���뼼������ǰ�棩+�û�ָ������ĵ�ЧPLMN */
    /* ��ѡ���б��м���LTE CSG ID������ */
    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_USER_CSG_SPEC_PLMN,
                                       NAS_MML_SIM_RAT_E_UTRN,
                                       &pstPlmnWithCsgId->stPlmnWithRat.stPlmnId,
                                       &stPlmnSelectionInfo);

    NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnSelectionInfo, pstPlmnSelectionListInfo);

    /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
    ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

    if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
     && (VOS_TRUE == ulEplmnValidFlg))
    {
        if (VOS_TRUE == ulIsNeedAddDuplicateRplmnAndEplmn)
        {
            /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
            NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
            NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(pstRPlmnCfg->enLastRplmnRat, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б��������û�ָ������ */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        return;
    }

    /* ��ȡ�û�ָ������ */
    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ��ѡ���б��������û�ָ��PLMN,���û�ָ����rat������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(pstUserSpecPlmn->enRat, &stPrioRatList);
    NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                           NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

    return;

}




VOS_UINT32 NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enSearchScene
)
{
    NAS_MML_CSG_DUPLICATED_RPLMN_AND_EPLMN_LIST_STRU       *pstCsgDuplicateRplmnAndEplmnList = VOS_NULL_PTR;

    pstCsgDuplicateRplmnAndEplmnList = NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo();

    /* ��֧��CSG���ܣ�����FALSE */
    if (VOS_FALSE == NAS_MML_IsSupportLteCsg())
    {
        return VOS_FALSE;
    }

    /* duplicate Rplmn��Eplmn��Ч������FALSE */
    if (VOS_FALSE == NAS_MMC_GetCsgDuplicateRplmnAndEplmnValidFlag())
    {
        return VOS_FALSE;
    }

    /* duplicate rplmn���Ϸ�����ĿΪ0������FALSE */
    if (VOS_FALSE == NAS_MML_IsPlmnIdValid(&pstCsgDuplicateRplmnAndEplmnList->astPlmnId[0]))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList: DuplicateRPlmn is invalid");
        return VOS_FALSE;
    }

    /* ������������ʹ��csg duplicate rplmn��eplmn���������б��У�����false */
    if (VOS_FALSE == NAS_MMC_IsSpecSearchSceneInNeedUseDuplicateRPlmnAndEplmnSearchSceneTbl(enSearchScene))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList: Scene is not suitable to add duplicateRPlmn");
        return VOS_FALSE;
    }

    return VOS_TRUE;

}


VOS_VOID NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    NAS_MML_PLMN_ID_STRU                                    stRPlmn;
    NAS_MML_CSG_DUPLICATED_RPLMN_AND_EPLMN_LIST_STRU       *pstDuplicateRplmnAndEplmn = VOS_NULL_PTR;

    PS_MEM_SET(&stPlmnInfo, 0 ,sizeof(stPlmnInfo));
    PS_MEM_SET(&stRPlmn, 0, sizeof(stRPlmn));

    pstDuplicateRplmnAndEplmn = NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo();

    stRPlmn = pstDuplicateRplmnAndEplmn->astPlmnId[0];

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        switch (pstPrioRatList->aucRatPrio[i])
        {
#if   (FEATURE_ON == FEATURE_LTE)
            case NAS_MML_NET_RAT_TYPE_LTE:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_E_UTRN,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;
#endif

            case NAS_MML_NET_RAT_TYPE_WCDMA:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_UTRN,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            case NAS_MML_NET_RAT_TYPE_GSM:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_GSM,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            default:
                NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList: Rat not support.");
                break;
        }
    }

    return;
}



VOS_VOID NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_CSG_DUPLICATED_RPLMN_AND_EPLMN_LIST_STRU       *pstDuplicateRplmnAndEplmn = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;

    PS_MEM_SET(&stPlmnInfo, 0, sizeof(stPlmnInfo));

    pstDuplicateRplmnAndEplmn = NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo();

    for (i = 1; i < pstDuplicateRplmnAndEplmn->ucPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_EPLMN,
                                           NAS_MML_SIM_RAT_ALL_SUPPORT,
                                           &(pstDuplicateRplmnAndEplmn->astPlmnId[i]),
                                           &stPlmnInfo);

        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
    }
}


VOS_VOID NAS_MMC_InitPlmnSelectionListInManualMode_CsgBgSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList
)
{
    VOS_UINT32                                              ulCsgIdNum;
    NAS_MML_RPLMN_CFG_INFO_STRU                            *pstRPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                    stRPlmn;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn = VOS_NULL_PTR;

    PS_MEM_SET(&stRPlmn, 0, sizeof(NAS_MML_PLMN_ID_STRU));

    /* �ֶ�����ģʽ���б���ʼ�����ԣ��������б���CSG ID��������������:
    1. PLMN���û�ָ����PLMN����EPLMN��ͬ��
    2. ���뼼�����û�ָ���Ľ��뼼����ͬ��
    3. �����EPLMN������ҪSIM���д�PLMN֧���û�ָ���Ľ��뼼����
    ���������У�2��3�����Ż��ռ�ģ����������Ż�������Ӧ�Ĵ�������΢С�޸ļ��� */

    ulCsgIdNum      = ulPlmnCsgIdNum;

    /* ��ȡ�û�ָ��PLMN ID */
    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ��ȡ�û�ָ��PLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡRPLMN��������Ϣ */
    pstRPlmnCfg = NAS_MML_GetRplmnCfg();

    (VOS_VOID)NAS_MMC_GetRPlmn_PlmnSelectionStrategy(pstRPlmnCfg->enLastRplmnRat, &stRPlmn);

    /* �û�ָ��������RPLMN��ͬʱ��Ҫ��EPLMN���ӵ�ѡ���б� */
    if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stRPlmn, &pstUserSpecPlmn->stPlmnId))
    {
        /* ��ȡ�û�ָ��PLMN��EPLMN��CSG ID���浽�����б��� */
        NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN, pstPlmnSelectionListInfo,
                                                &ulCsgIdNum, pstPlmnCsgIdWithRatList);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_ASFCsgSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                                              i;
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU                     *pstAllowedCsgList       = VOS_NULL_PTR;
    VOS_UINT32                                              ulCsgIdNum;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stCsgIdInfo;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stPlmnWithCsgId;

    PS_MEM_SET(&stCsgIdInfo,     0, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    PS_MEM_SET(&stPlmnWithCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    /* ������CSG�����б���˳��Ϊ:
    User manual spec CSG ID -> HPLMN CSG ID -> UPLMN CSG ID -> OPLMN CSG ID -> Available PLMN CSG ID
    ע����б���ֻ����Allowed CSG List�е�CSG ID                                                         */

    pstAllowedCsgList = NAS_MML_GetAllowedCsgList();

    /* ѭ������CSG ID����ʱ�����У�Ŀǰֻ֧��LTE���뼼����CSG������������չ֧��GU������Ҫ
    ÿ�����뼼������һ����Ԫ����ʱ�����У�Ҳ���������GUL������Ҫ3��Allowed CSG List��CSG ID
    ��������ʱ�洢�ռ䣬ÿ��CSG ID����ʼ��ΪGUL���뼼����1����Ԫ���Ե�Ԫ��˳��û������Ҫ�� */
    ulCsgIdNum = NAS_MML_MIN(pstAllowedCsgList->ulPlmnWithCsgIdListNum, NAS_MML_MAX_PLMN_CSG_ID_NUM);

    /* ������������Allowed CSG List��Ϊ�գ�ֱ���˳� */
    if (0 == ulCsgIdNum)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_ASFCsgSearch: csg allowed list is empty");
        return;
    }

    /* �������ʱ�洢CSG ID�Ŀռ䣬����Allowed CSG List�����е�CSG ID��Ŀǰֻ֧��LTE�����1���洢�ռ伴�� */
    pstPlmnCsgIdWithRatList = (NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                        ulCsgIdNum*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    /* �����ڴ�ʧ�ܣ�˵�������쳣��ֱ���˳� */
    if (VOS_NULL_PTR == pstPlmnCsgIdWithRatList)
    {
        return;
    }

    /* Allowed CSG List��û�н��뼼����Ŀǰֻ֧��L������ֻ��ʼ��L */
    for (i = 0; i < ulCsgIdNum; i++)
    {
        pstPlmnCsgIdWithRatList[i].stPlmnWithRat.stPlmnId = pstAllowedCsgList->astPlmnWithCsgIdInfo[i].stPlmnWithCsgId.stPlmnId;
        pstPlmnCsgIdWithRatList[i].stPlmnWithRat.enRat    = NAS_MML_NET_RAT_TYPE_LTE;
        pstPlmnCsgIdWithRatList[i].ulCsgId                = pstAllowedCsgList->astPlmnWithCsgIdInfo[i].stPlmnWithCsgId.ulCsgId;
    }

    /* ��ʼ��ʼ�����������б� */

    /* ������ֶ�����ģʽ�����ֶ�����ģʽ���򴴽��б� */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        NAS_MMC_InitPlmnSelectionListInManualMode_CsgBgSearch(pstPlmnSelectionListInfo, ulCsgIdNum, pstPlmnCsgIdWithRatList);

        PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnCsgIdWithRatList);
        return;
    }

    /* ������Զ�����ģʽ */

    /* ��������User manual spec CSG ID��������һ�����ҵ��������û���δָ����CSG ID���Զ����������ɹ������ */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_USER_CSG_SPEC_PLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡHPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_HPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡUPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_UPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡOPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_OPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡAvailable PLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_AVAIL_PLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnCsgIdWithRatList);
    return;

}

VOS_VOID NAS_MMC_InitPlmnSelectionList_PSFCsgSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                                              i;
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU                     *pstPSFCsgList           = VOS_NULL_PTR;
    VOS_UINT32                                              ulCsgIdNum;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stCsgIdInfo;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stPlmnWithCsgId;

    PS_MEM_SET(&stCsgIdInfo,     0, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    PS_MEM_SET(&stPlmnWithCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    /* ������CSG�����б���˳��Ϊ:
    User manual spec CSG ID -> HPLMN CSG ID -> UPLMN CSG ID -> OPLMN CSG ID -> Available PLMN CSG ID
    ע����б���ֻ����Operator CSG List��UE based CSG List�е�CSG ID                                 */

    /* AT&T����Ҫ��:
    �����֧��Operator CSG List����Operator CSG List����Ϊ��
    �������֧��Operator CSG List����Allowed CSG List����Ϊ��
    (ע: ������UE based CSG List��ֱ����Allowed CSG List����) */
    if (PS_USIM_SERVICE_AVAILIABLE == NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_OP_CSG))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_PSFCsgSearch: csg operator list exist");
        pstPSFCsgList = NAS_MML_GetOperatorCsgList();
    }
    else
    {
        if (VOS_FALSE== NAS_MML_IsUeBasedOperatorCsgListEmpty())
        {
            pstPSFCsgList = NAS_MML_GetUeBasedOperatorCsgList();
        }
        else
        {
            pstPSFCsgList = NAS_MML_GetAllowedCsgList();
        }
    }

    ulCsgIdNum = NAS_MML_MIN(pstPSFCsgList->ulPlmnWithCsgIdListNum, NAS_MML_MAX_PLMN_CSG_ID_NUM);

    /* �������������Ϊ�գ�ֱ���˳� */
    if (0 == ulCsgIdNum)
    {
        return;
    }

    /* ѭ������CSG ID����ʱ�����У�Ŀǰֻ֧��LTE���뼼����CSG������������չ֧��GU������Ҫ
    ÿ�����뼼������һ����Ԫ����ʱ�����У�Ҳ���������GUL������Ҫ3��Operator CSG List��CSG ID
    ��������ʱ�洢�ռ䣬ÿ��CSG ID����ʼ��ΪGUL���뼼����1����Ԫ���Ե�Ԫ��˳��û������Ҫ�� */

    /* �������ʱ�洢CSG ID�Ŀռ䣬����Operator CSG List�����е�CSG ID��Ŀǰֻ֧��LTE�����1���洢�ռ伴�� */
    pstPlmnCsgIdWithRatList = (NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                        ulCsgIdNum*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    /* �����ڴ�ʧ�ܣ�˵�������쳣��ֱ���˳� */
    if (VOS_NULL_PTR == pstPlmnCsgIdWithRatList)
    {
        return;
    }

    /* Operator CSG List��û�н��뼼����Ŀǰֻ֧��L������ֻ��ʼ��L */
    for (i = 0; i < ulCsgIdNum; i++)
    {
        pstPlmnCsgIdWithRatList[i].stPlmnWithRat.stPlmnId = pstPSFCsgList->astPlmnWithCsgIdInfo[i].stPlmnWithCsgId.stPlmnId;
        pstPlmnCsgIdWithRatList[i].stPlmnWithRat.enRat    = NAS_MML_NET_RAT_TYPE_LTE;
        pstPlmnCsgIdWithRatList[i].ulCsgId                = pstPSFCsgList->astPlmnWithCsgIdInfo[i].stPlmnWithCsgId.ulCsgId;
    }

    /* ��ʼ��ʼ�����������б� */

    /* ������ֶ�����ģʽ�����ֶ�����ģʽ���򴴽��б� */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        NAS_MMC_InitPlmnSelectionListInManualMode_CsgBgSearch(pstPlmnSelectionListInfo, ulCsgIdNum, pstPlmnCsgIdWithRatList);

        PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnCsgIdWithRatList);
        return;
    }

    /* ������Զ�����ģʽ */

    /* ��������User manual spec CSG ID��������һ�����ҵ��������û���δָ����CSG ID���Զ����������ɹ������ */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_USER_CSG_SPEC_PLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡHPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_HPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡUPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_UPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡOPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_OPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡAvailable PLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_AVAIL_PLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnCsgIdWithRatList);
    return;
}


VOS_UINT32 NAS_MMC_GetNextSearchPlmnAndCsgId(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSearchListInfo,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdInfo,
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstVPlmn,
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnList
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              k;
    VOS_UINT32                                              ulIsCsgIdSearchingAllowed;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdSelectionInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       stCsgId;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stCsgId, 0, sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    /* ��ȡ��ǰ���뼼�����ȼ���Ϣ */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    for (i = 0 ; i < pstCsgIdSearchListInfo->usSearchPlmnNum; i++)
    {
        pstCsgIdSelectionInfo = &(pstCsgIdSearchListInfo->astPlmnSelectionList[i]);

        /* CSG ID�������ݲ�������VPLMN�Ĺ������Ƿ���ͬ��������Ҫ�ٽ����Ż� */

        /* �ݲ��Ƚ�Ҫ������CSG ID��PLMN��VPLMN�����ȼ���������Ҫ�ٽ����Ż� */

        /* ��SYSCFG���õĽ��뼼�����ȼ����β��� */
        for (k = 0; k < pstPrioRatList->ucRatNum; k++)
        {
            /* ����SYSCFG���ú͸�CSG ID֧�ֵĽ��뼼����ȡ��CSG ID�ͽ��뼼���Լ�����״̬ */
            for (j = 0; j < pstCsgIdSelectionInfo->ucRatNum; j++)
            {
                /* ������뼼��֧����״̬��Ϊδ�����������Ѵ��� */
                if (pstPrioRatList->aucRatPrio[k] == pstCsgIdSelectionInfo->astPlmnRatInfo[j].enRatType)
                {
                    stCsgId.stPlmnWithRat.stPlmnId = pstCsgIdSelectionInfo->stPlmnId;
                    stCsgId.ulCsgId                = pstCsgIdSelectionInfo->ulCsgId;
                    stCsgId.stPlmnWithRat.enRat    = pstCsgIdSelectionInfo->astPlmnRatInfo[j].enRatType;

                    ulIsCsgIdSearchingAllowed = NAS_MMC_IsCsgIdSearchingAllowed_PlmnSelectionStrategy(&stCsgId,
                                                            pstCsgIdSelectionInfo->astPlmnRatInfo[j].enNetStatus);
                    if (VOS_FALSE == ulIsCsgIdSearchingAllowed)
                    {
                        /* �洢�Ľ��뼼���������ظ� */
                        break;
                    }

#if (FEATURE_ON == FEATURE_LTE)
                    /* ��ʹ�Ѿ�Disable L��Ҳ�������� */
                    if (((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus)
                      || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus))
                      && (NAS_MML_NET_RAT_TYPE_LTE == pstCsgIdSelectionInfo->astPlmnRatInfo[j].enRatType))
                    {
                        /* �򵥴����������ǰ��DISABLE LTE������������CSG ID������������Ҫ�ٽ����Ż� */
                    }
#endif

                    pstCsgIdInfo->stPlmnId                      = pstCsgIdSelectionInfo->stPlmnId;
                    pstCsgIdInfo->ulCsgId                       = pstCsgIdSelectionInfo->ulCsgId;
                    pstCsgIdInfo->enPlmnType                    = pstCsgIdSelectionInfo->enPlmnType;
                    pstCsgIdInfo->ucRatNum                      = 1;
                    pstCsgIdInfo->astPlmnRatInfo[0].enRatType   = pstCsgIdSelectionInfo->astPlmnRatInfo[j].enRatType;
                    pstCsgIdInfo->astPlmnRatInfo[0].enNetStatus = pstCsgIdSelectionInfo->astPlmnRatInfo[j].enNetStatus;

                    /* ��ȡ�ɹ�����VOS_TRUE */
                    return VOS_TRUE;

                }
            }
        }

    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsCsgIdSearchingAllowed_PlmnSelectionStrategy(
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU  *pstCsgId,
    NAS_MMC_NET_STATUS_ENUM_UINT8       enNetStatus
)
{
    NAS_MML_PLMN_WITH_CSG_ID_STRU       stPlmnWithCsgId;

    /* ��ʼ��PLMN+CSG ID��Ϣ */
    stPlmnWithCsgId.stPlmnId            = pstCsgId->stPlmnWithRat.stPlmnId;
    stPlmnWithCsgId.ulCsgId             = pstCsgId->ulCsgId;

#if (FEATURE_ON == FEATURE_LTE)

    /* ��ǰ����ΪLTEʱ */
    if (NAS_MML_NET_RAT_TYPE_LTE == pstCsgId->stPlmnWithRat.enRat)
    {
        /* �ж��Ƿ�����LTE������������� */
        if (VOS_FALSE == NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy(&pstCsgId->stPlmnWithRat.stPlmnId))
        {
            return VOS_FALSE;
        }
    }
#endif

    /* CSG��������������PLMN�Ľ�ֹ����ΪCSG�Ľ�ֹ�����ɰ������ͽ�ֹOperator List���ƣ�PLMN��ֹ����������PLMN�µ�CSG������ */

    /* ���ж��Ƿ���forbid plmn for gprs�б��У�ԭ��ͬ���治�жϽ�ֹPLMN */

    /* �����ǰ���ԵĽ��뼼���ڽ�ֹ���뼼���б��У���׼������ */
    if (VOS_TRUE == NAS_MML_IsRatInForbiddenList(pstCsgId->stPlmnWithRat.enRat))
    {
        return VOS_FALSE;
    }

    /* ����״̬��Ϊδ���������������ڣ�������������CSG ID */
    if ((NAS_MMC_NET_STATUS_NO_SEARCHED != enNetStatus)
     && (NAS_MMC_NET_STATUS_SEARCHED_EXIST != enNetStatus))
    {
        return VOS_FALSE;
    }

    /* ����ǽ�ֹCSG ID��������������CSG ID */
    if (VOS_TRUE == NAS_MMC_IsPlmnWithCsgIdForbidden(&stPlmnWithCsgId))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCsgId,
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSelectionListInfo
)
{
    VOS_UINT32                          i;

    /* ����PLMN+CSG ID������뼼���������б���ÿ��CSG ID������뼼���Ƚ� */
    for (i = 0; i < pstCsgIdSelectionListInfo->usSearchPlmnNum; i++)
    {
        /* �Ƚ�PLMN��CSG ID��Ҫ����ȫ��ͬ */
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstCsgId->stPlmnWithRat.stPlmnId,
                                        &pstCsgIdSelectionListInfo->astPlmnSelectionList[i].stPlmnId))
           &&(pstCsgId->ulCsgId == pstCsgIdSelectionListInfo->astPlmnSelectionList[i].ulCsgId))
        {
            /* �����ͬ�����������б���PLMN��Ӧ���뼼������Ϣ */
            NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(pstCsgId->stPlmnWithRat.enRat,
                                        enNetStatus,
                                        &pstCsgIdSelectionListInfo->astPlmnSelectionList[i]);
        }
    }

    return;
}


VOS_VOID NAS_MMC_UpdateAllCsgIdNetStatusInCsgIdSelectionList(
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSelectionListInfo,
    VOS_UINT32                                              ulAppendFlg
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              ulUpdateRslt;
    VOS_UINT32                                              ulCsgIdNum;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       astTmpCsgSearchInfo[NAS_MML_MAX_PLMN_CSG_ID_NUM];

    /* ֮����û�и���ԭ����BG�ѵĴ�����ʽ����Ϊ��ԭ�����ڲ������б����ֻ��װ��16������������CSG�����35�� */

    /* ��ֹĿ���ڴ�Խ�磬Ҳ��ֹ����Դ�ڴ�Խ�� */
    ulCsgIdNum = NAS_MML_MIN(ulPlmnCsgIdNum, NAS_MML_MAX_PLMN_CSG_ID_NUM);

    PS_MEM_SET(astTmpCsgSearchInfo, 0, NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    /* ֻ�������������CSG ID */
    PS_MEM_CPY(astTmpCsgSearchInfo, pstPlmnCsgIdWithRatList, ulCsgIdNum*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    i = 0;

    if (0 == ulCsgIdNum)
    {
        for (i = 0; i < pstCsgIdSelectionListInfo->usSearchPlmnNum; i++)
        {
            for (j = 0; j < pstCsgIdSelectionListInfo->astPlmnSelectionList[i].ucRatNum; j++)
            {
                pstCsgIdSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST;

            }
        }
    }

    /* ѭ�������������б���ÿһ��CSG ID */
    while (i < ulCsgIdNum)
    {
        ulUpdateRslt = VOS_FALSE;

        /* ѭ��ѡ���б���ÿһ��CSG ID */
        for (j = 0; j < pstCsgIdSelectionListInfo->usSearchPlmnNum; j++)
        {
             /* ����������������Ϣ���������б��е�һ��CSG ID��������³ɹ�����Ҫ��������Ϣ�ж�ӦCSG ID+RATɾ�� */
             if (VOS_TRUE == NAS_MMC_UpdateCsgIdNetStatus(&pstCsgIdSelectionListInfo->astPlmnSelectionList[j],
                                                            &astTmpCsgSearchInfo[i]))
             {
                 /* ��¼���¹� */
                 ulUpdateRslt = VOS_TRUE;
             }
        }

        if (VOS_TRUE == ulUpdateRslt)
        {
            /* ���³ɹ���ɾ����ʱ�б��д�CSG ID+RAT����Ϣ�����1��CSG ID�ж�����뼼����1��ֻɾ��1�����뼼��*/
            NAS_MMC_DelPlmnCsgIdWithRatInSearchList(&astTmpCsgSearchInfo[i], &ulCsgIdNum, astTmpCsgSearchInfo);
        }
        else
        {
            /* δ���¹���˵�������б��в����ڴ�CSG ID������һ��CSG ID��ʼˢ�� */
            i++;
        }

    }

    /* ��������������б�ʣ��Ϊ�գ����½��� */
    if (0 == ulCsgIdNum)
    {
        return;
    }

    /* �����Ҫ�����������б���ϢpstPlmnCsgIdWithRatList�е�ʣ�����粹�䵽�����б��� */
    if (VOS_TRUE == ulAppendFlg)
    {
        NAS_MMC_SaveCsgIdListInCsgSelectionList(ulCsgIdNum, astTmpCsgSearchInfo, pstCsgIdSelectionListInfo);
    }

    return;
}


VOS_UINT32 NAS_MMC_UpdateCsgIdNetStatus(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdInfo,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRat
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulUpdateRslt;

    /* �����ķ�ʽ: ���ڴ�ˢ�µ������б��е�CSG ID��
    �������Ƚϵ���֪��CSG ID��ͬ����ˢ����Ӧ�Ľ��뼼��Ϊ���ڣ������������뼼�������״̬ΪNO SEARCHED������ΪNO EXIST��
    �������Ƚϵ���֪��CSG ID��ͬ��������״̬ΪNO SEARCHED�Ľ��뼼������ΪNO EXIST��*/

    /* ��������VOS_TRUE�������ˢ�¹����������غ󣬴�CSG ID��������������CSG ID�б��б�ɾ�� */

    ulUpdateRslt = VOS_FALSE;

    /* �������������б���Ѱ�Ҵ�ˢ�µ�CSG ID�����Ƿ����ҵ� */

    /* �Ƚ�PLMN��CSG ID��Ҫ����ȫ��ͬ */
    if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstCsgIdInfo->stPlmnId,
                                        &pstPlmnCsgIdWithRat->stPlmnWithRat.stPlmnId))
       &&(pstCsgIdInfo->ulCsgId == pstPlmnCsgIdWithRat->ulCsgId))
    {
        /* �ҵ���Ӧ�Ľ��뼼������ˢ�� */
        for (i = 0; i < pstCsgIdInfo->ucRatNum; i++)
        {
            if (pstCsgIdInfo->astPlmnRatInfo[i].enRatType == pstPlmnCsgIdWithRat->stPlmnWithRat.enRat)
            {
                /* ֻˢ�³ɴ��ڼ��ɣ�������Ŀǰ�ݲ���ע */
                pstCsgIdInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;

                /* ��CSG ID���ɹ�ˢ�¹� */
                ulUpdateRslt = VOS_TRUE;
            }
            else
            {
                /* �������뼼�������δ������ˢ��Ϊ������ */
                if (NAS_MMC_NET_STATUS_NO_SEARCHED == pstCsgIdInfo->astPlmnRatInfo[i].enNetStatus)
                {
                    pstCsgIdInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST;
                }
            }
        }

        return ulUpdateRslt;
    }

    /* ���CSG ID����ͬ����δ����״̬�Ľ��뼼����ˢ��Ϊ������ */
    for (i = 0; i < pstCsgIdInfo->ucRatNum; i++)
    {
        /* δˢ�¹��Ľ��뼼��������Ϊ������ */
        if (NAS_MMC_NET_STATUS_NO_SEARCHED == pstCsgIdInfo->astPlmnRatInfo[i].enNetStatus)
        {
            pstCsgIdInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST;
        }
    }

    return ulUpdateRslt;
}

/*lint -e429*/

VOS_VOID NAS_MMC_DelPlmnCsgIdWithRatInSearchList(
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU  *pstPlmnCsgIdWithRat,
    VOS_UINT32                         *pulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU  *pstPlmnCsgIdWithRatList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulCsgIdNum;

    ulCsgIdNum = *pulPlmnCsgIdNum;

    for (i = 0; i < ulCsgIdNum; i++)
    {
        /* �Ƚ�PLMN��CSG ID+RAT��Ҫ����ȫ��ͬ */
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnCsgIdWithRat->stPlmnWithRat.stPlmnId,
                                               &pstPlmnCsgIdWithRatList[i].stPlmnWithRat.stPlmnId))
         && (pstPlmnCsgIdWithRat->stPlmnWithRat.enRat == pstPlmnCsgIdWithRatList[i].stPlmnWithRat.enRat)
         && (pstPlmnCsgIdWithRat->ulCsgId == pstPlmnCsgIdWithRatList[i].ulCsgId))
        {
            /*lint -e961*/
            (VOS_VOID)PS_MEM_MOVE(&pstPlmnCsgIdWithRatList[i],
                                        &pstPlmnCsgIdWithRatList[i+1],
                                        (ulCsgIdNum-i-1)*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));
            /*lint +e961*/

            /* ɾ����1��CSG ID��������1 */
            ulCsgIdNum--;

            *pulPlmnCsgIdNum = ulCsgIdNum;
            return;
        }
    }

    return;

}


VOS_VOID NAS_MMC_DelCsgIdInSearchList(
    NAS_MML_PLMN_WITH_CSG_ID_STRU      *pstPlmnWithCsgId,
    VOS_UINT32                         *pulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU  *pstPlmnCsgIdWithRatList
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU   stPlmnCsgIdWithRat;

    PS_MEM_SET(&stPlmnCsgIdWithRat, 0, sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    stPlmnCsgIdWithRat.stPlmnWithRat.stPlmnId = pstPlmnWithCsgId->stPlmnId;
    stPlmnCsgIdWithRat.ulCsgId                = pstPlmnWithCsgId->ulCsgId;

    /* ɾ���б������н��뼼���Ĵ�CSG ID */
    for (enRat = NAS_MML_NET_RAT_TYPE_GSM; enRat < NAS_MML_NET_RAT_TYPE_BUTT; enRat++)
    {
        stPlmnCsgIdWithRat.stPlmnWithRat.enRat = enRat;

        NAS_MMC_DelPlmnCsgIdWithRatInSearchList(&stPlmnCsgIdWithRat, pulPlmnCsgIdNum, pstPlmnCsgIdWithRatList);
    }

    return;

}

VOS_VOID NAS_MMC_SaveCsgIdListInCsgSelectionList(
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              ulCsgIdNum;
    VOS_UINT32                                              ulSelectionListCsgIdNumBeforeAdd;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stPlmnWithCsgId;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stCsgIdInfo;
    VOS_UINT32                                              ulRslt;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn = VOS_NULL_PTR;

    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ����˼·: �Ƚ��Ȳ���Operator CSG List��Ҳ����Allowed CSG List��CSG IDɾ����
    Ȼ��ʣ���CSG ID��HPLMN->UPLMN->OPLMN->AVAILABLE PLMN��˳����������б� */

    ulCsgIdNum = ulPlmnCsgIdNum;

    /* ��¼������֮ǰ���е�CSG ID���������ڱ����ͳһˢ������״̬ */
    ulSelectionListCsgIdNumBeforeAdd = pstPlmnSelectionListInfo->usSearchPlmnNum;

    PS_MEM_SET(&stPlmnWithCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));
    PS_MEM_SET(&stCsgIdInfo,     0, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    /* �Ƚ�����������������CSG IDɾ������Ҫ����Բ���Allowed CSG List�е�
    hybrid CSG ID���������������ݣ�GCF����6.4.1 */
    for (i = ulCsgIdNum; i > 0; i--)
    {
        stPlmnWithCsgId.stPlmnId = pstPlmnCsgIdWithRatList[i - 1].stPlmnWithRat.stPlmnId;
        stPlmnWithCsgId.ulCsgId  = pstPlmnCsgIdWithRatList[i - 1].ulCsgId;

        /* ֧��Operator CSG List��ֻ��鲻�ڴ��б��о�ɾ�� */
        if (PS_USIM_SERVICE_AVAILIABLE == NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_OP_CSG))
        {
            ulRslt = NAS_MML_IsCsgIdInOperatorCsgIdList(&stPlmnWithCsgId);
        }
        else
        {
            /* ��֧��Operator CSG List��˵��ʹ�õ���UE based CSG List����ֻ���Allowed List */
            ulRslt = NAS_MML_IsCsgIdInAllowedCsgIdList(&stPlmnWithCsgId);
        }
        /* �ֶ�ģʽ�£�ֻ�������ѡPLMN��ͬ��*/
        if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
        {
            ulRslt = NAS_MML_CompareBcchPlmnwithSimPlmn(&stPlmnWithCsgId.stPlmnId, &pstUserSpecPlmn->stPlmnId);
        }

        if (VOS_FALSE == ulRslt)
        {
            /* ɾ����ʱ�б������д�CSG ID����Ϣ��ע��ɾ���Ŀ����Ƕ�������һ��ɾ��3�� */
            NAS_MMC_DelCsgIdInSearchList(&stPlmnWithCsgId, &ulCsgIdNum, pstPlmnCsgIdWithRatList);
        }
    }

    /* �����������ʱ�б���CSG IDΪ0�����ô���ֱ�ӷ��� */
    if (0 == ulCsgIdNum)
    {
        return;
    }

    /* ��ȡHPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_HPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡUPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_UPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡOPLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_OPLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* ��ȡAvailable PLMN��CSG ID���浽�����б��� */
    NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(NAS_MMC_PLMN_TYPE_AVAIL_PLMN, pstPlmnSelectionListInfo,
                                            &ulCsgIdNum, pstPlmnCsgIdWithRatList);

    /* �������ӵ����е�����CSG ID�����н��뼼����ˢ��Ϊ���� */
    for (i = ulSelectionListCsgIdNumBeforeAdd; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        for (j = 0; j < pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum; j++)
        {
            pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
        }
    }

    return;
}


VOS_VOID NAS_MMC_BuildCsgSelectionCsgIdInfo(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MML_PLMN_WITH_CSG_ID_STRU                          *pstPlmnWithCsgId,
    VOS_UINT16                                              usSimRat,
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnCsgIdInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulRatNum;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         aenRat[NAS_MML_NET_RAT_TYPE_BUTT];
    VOS_UINT16                                              usBuildRat;

    ulRatNum   = 0;
    PS_MEM_SET(aenRat,   NAS_MML_NET_RAT_TYPE_BUTT, NAS_MML_NET_RAT_TYPE_BUTT);

    usBuildRat = 0;

    /* �����д�CSG ID�Ľ��뼼�����ҳ����������1��CSG ID�ж�����뼼�������뼼����¼���� */
    for (i = 0; i < ulPlmnCsgIdNum; i++)
    {
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnWithCsgId->stPlmnId,
                                                            &pstPlmnCsgIdWithRatList[i].stPlmnWithRat.stPlmnId))
            &&(pstPlmnWithCsgId->ulCsgId == pstPlmnCsgIdWithRatList[i].ulCsgId))
        {
            aenRat[ulRatNum] = pstPlmnCsgIdWithRatList[i].stPlmnWithRat.enRat;
            ulRatNum++;

            /* �쳣��������ֹ������ϱ����ظ��Ľ��뼼����������Խ�� */
            if (NAS_MML_NET_RAT_TYPE_BUTT <= ulRatNum)
            {
                break;
            }
        }
    }

    /* ���еĽ��뼼�������ҵ�������1��CSG ID��Ԫ�˳� */

    /* ���ڵĽ��뼼��ת����SIM��ʽ */
    usBuildRat = NAS_MMC_ConvertMultiNetRatToSimFormat(ulRatNum, aenRat);

    /* ���н��뼼�����������Ҫ��SIM��֧�ֵĴ�PLMN�Ľ��뼼��ȡ���� */
    usBuildRat &= usSimRat;

    /* �ȴ���PLMN ID�ͽ��뼼������Ϣ */
    NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                       usBuildRat,
                                       &pstPlmnWithCsgId->stPlmnId,
                                       pstPlmnCsgIdInfo);

    /* ��дCSG ID */
    pstPlmnCsgIdInfo->ulCsgId = pstPlmnWithCsgId->ulCsgId;

    return;

}



VOS_UINT32 NAS_MMC_GetEHPlmnCsgIdInCsgIdList(
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MML_SIM_EHPLMN_INFO_STRU                           *pstEHPlmnInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stCsgId;

    PS_MEM_SET(&stCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    /* ��ȡEHPLMN */
    pstEHPlmnInfo = NAS_MML_GetSimEhplmnList();

    /* ���ÿ��HPLMN�����б��н��в��� */
    for (i = 0; i< pstEHPlmnInfo->ucEhPlmnNum; i++)
    {
        for (j = 0; j < ulPlmnCsgIdNum; j++)
        {
            /* �ҵ�HPLMN��CSG ID */
            if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnCsgIdWithRatList[j].stPlmnWithRat.stPlmnId,
                                        &pstEHPlmnInfo->astEhPlmnInfo[i].stPlmnId))
            {
                /* ��¼����Ӧ��PLMN+CSG ID */
                stCsgId.stPlmnId        = pstPlmnCsgIdWithRatList[j].stPlmnWithRat.stPlmnId;
                stCsgId.ulCsgId         = pstPlmnCsgIdWithRatList[j].ulCsgId;

                /* ����һ��������Ԫ */
                NAS_MMC_BuildCsgSelectionCsgIdInfo(NAS_MMC_PLMN_TYPE_HPLMN, &stCsgId, pstEHPlmnInfo->astEhPlmnInfo[i].usSimRat,
                                        ulPlmnCsgIdNum, pstPlmnCsgIdWithRatList, pstCsgIdInfo);

                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MMC_GetUPlmnCsgIdInCsgIdList(
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MML_SIM_USERPLMN_INFO_STRU                         *pstUserPlmnList = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stCsgId;

    PS_MEM_SET(&stCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    /* ��ȡUPLMN */
    pstUserPlmnList = NAS_MML_GetSimUserPlmnList();

    /* ���ÿ��UPLMN�����б��н��в��� */
    for (i = 0; i< pstUserPlmnList->ucUserPlmnNum; i++)
    {
        for (j = 0; j < ulPlmnCsgIdNum; j++)
        {
            /* �ҵ�UPLMN��CSG ID */
            if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnCsgIdWithRatList[j].stPlmnWithRat.stPlmnId,
                                        &pstUserPlmnList->astUserPlmnInfo[i].stPlmnId))
            {
                /* ��¼����Ӧ��PLMN+CSG ID */
                stCsgId.stPlmnId        = pstPlmnCsgIdWithRatList[j].stPlmnWithRat.stPlmnId;
                stCsgId.ulCsgId         = pstPlmnCsgIdWithRatList[j].ulCsgId;

                /* ����һ��������Ԫ */
                NAS_MMC_BuildCsgSelectionCsgIdInfo(NAS_MMC_PLMN_TYPE_UPLMN, &stCsgId, pstUserPlmnList->astUserPlmnInfo[i].usSimRat,
                                        ulPlmnCsgIdNum, pstPlmnCsgIdWithRatList, pstCsgIdInfo);

                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MMC_GetOPlmnCsgIdInCsgIdList(
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MML_SIM_OPERPLMN_INFO_STRU                         *pstOperPlmnList = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stCsgId;

    PS_MEM_SET(&stCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    /* ��ȡOPLMN */
    pstOperPlmnList = NAS_MML_GetSimOperPlmnList();

    /* ���ÿ��OPLMN�����б��н��в��� */
    for (i = 0; i< pstOperPlmnList->usOperPlmnNum; i++)
    {
        for (j = 0; j < ulPlmnCsgIdNum; j++)
        {
            /* �ҵ�OPLMN��CSG ID */
            if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnCsgIdWithRatList[j].stPlmnWithRat.stPlmnId,
                                        &pstOperPlmnList->astOperPlmnInfo[i].stPlmnId))
            {
                /* ��¼����Ӧ��PLMN+CSG ID */
                stCsgId.stPlmnId        = pstPlmnCsgIdWithRatList[j].stPlmnWithRat.stPlmnId;
                stCsgId.ulCsgId         = pstPlmnCsgIdWithRatList[j].ulCsgId;

                /* ����һ��������Ԫ */
                NAS_MMC_BuildCsgSelectionCsgIdInfo(NAS_MMC_PLMN_TYPE_OPLMN, &stCsgId, pstOperPlmnList->astOperPlmnInfo[i].usSimRat,
                                        ulPlmnCsgIdNum, pstPlmnCsgIdWithRatList, pstCsgIdInfo);

                return VOS_TRUE;

            }
        }
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MMC_GetAvailablePlmnCsgIdInCsgIdList(
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdInfo
)
{
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stCsgId;

    PS_MEM_SET(&stCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    /* ���ÿ��PLMN�����б��н��в��� */
    if (0 < ulPlmnCsgIdNum)
    {
        /* ��¼����Ӧ��PLMN+CSG ID */
        stCsgId.stPlmnId = pstPlmnCsgIdWithRatList[0].stPlmnWithRat.stPlmnId;
        stCsgId.ulCsgId  = pstPlmnCsgIdWithRatList[0].ulCsgId;

        /* ����һ��������Ԫ */
        NAS_MMC_BuildCsgSelectionCsgIdInfo(NAS_MMC_PLMN_TYPE_AVAIL_PLMN, &stCsgId, NAS_MML_SIM_RAT_ALL_SUPPORT,
                                        ulPlmnCsgIdNum, pstPlmnCsgIdWithRatList, pstCsgIdInfo);

        return VOS_TRUE;

    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MMC_GetUserSpecPlmnCsgIdInCsgIdList(
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdInfo
)
{
    VOS_UINT32                                              i;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stCsgId;

    PS_MEM_SET(&stCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    /* ��ȡ�û�ָ��PLMN ID */
    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ���б��н��в��� */
    for (i = 0; i < ulPlmnCsgIdNum; i++)
    {
        /* �ҵ���PLMN��CSG ID */
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnCsgIdWithRatList[i].stPlmnWithRat.stPlmnId,
                                        &pstUserSpecPlmn->stPlmnId))
        {
            /* ��ʱ���ж��û�ָ���Ľ��뼼������ʹ�û�ָ��G��Ҳ���Ա�����LTE��CSG ID��������Ҫ�������ڴ��޸� */

            /* ��¼����Ӧ��PLMN+CSG ID */
            stCsgId.stPlmnId = pstPlmnCsgIdWithRatList[i].stPlmnWithRat.stPlmnId;
            stCsgId.ulCsgId  = pstPlmnCsgIdWithRatList[i].ulCsgId;

            /* ����һ��������Ԫ */
            NAS_MMC_BuildCsgSelectionCsgIdInfo(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN, &stCsgId, NAS_MML_SIM_RAT_ALL_SUPPORT,
                                    ulPlmnCsgIdNum, pstPlmnCsgIdWithRatList, pstCsgIdInfo);

            return VOS_TRUE;

        }
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MMC_GetEPlmnCsgIdInCsgIdList(
    VOS_UINT32                                               ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   *pstCsgIdInfo
)
{
    VOS_UINT32                                               i;
    VOS_UINT32                                               j;
    VOS_UINT16                                               usSimRat;
    NAS_MML_EQUPLMN_INFO_STRU                               *pstEquPlmnList = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                            stCsgId;
    VOS_UINT16                                               usEPlmnSimRat;

    usSimRat      = NAS_MML_SIM_RAT_ALL_SUPPORT;
    usEPlmnSimRat = 0;
    PS_MEM_SET(&stCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    /* ��ȡEPLMN */
    pstEquPlmnList = NAS_MML_GetEquPlmnList();

    /* ���EPLMN��Ч�򷵻�ʧ�� */
    if (VOS_FALSE == pstEquPlmnList->ucValidFlg)
    {
        return VOS_FALSE;
    }

    /* ���ÿ��EPLMN�����б��н��в��� */
    for (i = 0; i< pstEquPlmnList->ucEquPlmnNum; i++)
    {
        for (j = 0; j < ulPlmnCsgIdNum; j++)
        {
            /* �ҵ�EPLMN��CSG ID */
            if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnCsgIdWithRatList[j].stPlmnWithRat.stPlmnId,
                                        &pstEquPlmnList->astEquPlmnAddr[i]))
            {
                /* ��¼����Ӧ��PLMN+CSG ID */
                stCsgId.stPlmnId        = pstPlmnCsgIdWithRatList[j].stPlmnWithRat.stPlmnId;
                stCsgId.ulCsgId         = pstPlmnCsgIdWithRatList[j].ulCsgId;

                /* ��ȡSIM���д�PLMN֧�ֵĽ��뼼�� */
                if (VOS_TRUE == NAS_MMC_GetSpecPlmnSimRat(&stCsgId.stPlmnId, &usEPlmnSimRat))
                {
                    usSimRat = usEPlmnSimRat;
                }

                /* ����һ��������Ԫ */
                NAS_MMC_BuildCsgSelectionCsgIdInfo(NAS_MMC_PLMN_TYPE_EPLMN, &stCsgId, usSimRat,
                                        ulPlmnCsgIdNum, pstPlmnCsgIdWithRatList, pstCsgIdInfo);

                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_GetUserManualSpecCsgIdInCsgIdList(
    VOS_UINT32                                              ulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstCsgIdInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT16                                              usSimRat;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstUserManualCsgIdInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stCsgId;
    VOS_UINT16                                              usUserSpecPlmnSimRat;

    usSimRat             = NAS_MML_SIM_RAT_ALL_SUPPORT;

    PS_MEM_SET(&stCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    usUserSpecPlmnSimRat = 0;

    /* ��ȡ�û�ָ����CSG ID */
    pstUserManualCsgIdInfo = NAS_MMC_GetCsgSpecSrchPlmnWithCsgId();

    /* ����û�ָ����CSG ID��PLMN��Ч������ʧ�� */
    if (VOS_FALSE == NAS_MML_IsPlmnIdValid(&pstUserManualCsgIdInfo->stPlmnWithRat.stPlmnId))
    {
        return VOS_FALSE;
    }

    /* ����������û�ָ����CSG ID������ʧ�� */
    if (NAS_MML_INVALID_CSG_ID_VALUE == pstUserManualCsgIdInfo->ulCsgId)
    {
        return VOS_FALSE;
    }

    /* ֱ�ӵ��б��в����û�ָ����CSG ID */
    for (i = 0; i < ulPlmnCsgIdNum; i++)
    {
        /* �ҵ��û�ָ����CSG ID */
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnCsgIdWithRatList[i].stPlmnWithRat.stPlmnId,
                                        &pstUserManualCsgIdInfo->stPlmnWithRat.stPlmnId))
          &&(pstPlmnCsgIdWithRatList[i].ulCsgId == pstUserManualCsgIdInfo->ulCsgId))
        {
            stCsgId.stPlmnId            = pstPlmnCsgIdWithRatList[i].stPlmnWithRat.stPlmnId;
            stCsgId.ulCsgId             = pstPlmnCsgIdWithRatList[i].ulCsgId;

            /* ��ȡSIM���д�PLMN֧�ֵĽ��뼼�� */
            if (VOS_TRUE == NAS_MMC_GetSpecPlmnSimRat(&stCsgId.stPlmnId, &usUserSpecPlmnSimRat))
            {
                usSimRat &= usUserSpecPlmnSimRat;
            }

            /* ����һ��������Ԫ */
            NAS_MMC_BuildCsgSelectionCsgIdInfo(NAS_MMC_PLMN_TYPE_USER_CSG_SPEC_PLMN, &stCsgId, usSimRat,
                                    ulPlmnCsgIdNum, pstPlmnCsgIdWithRatList, pstCsgIdInfo);

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

VOS_VOID NAS_MMC_AddPlmnCsgIdInPlmnSelectionList(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    VOS_UINT32                                             *pulPlmnCsgIdNum,
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstPlmnCsgIdWithRatList
)
{
    VOS_UINT32                                              ulCsgIdNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stCsgIdInfo;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stPlmnWithCsgId;
    pNasMmcGetPlmnCsgIdInCsgIdListFunc                      pfunGetPlmnCsgIdInList = VOS_NULL_PTR;

    PS_MEM_SET(&stCsgIdInfo,     0, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    PS_MEM_SET(&stPlmnWithCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));

    ulCsgIdNum = *pulPlmnCsgIdNum;

    switch (enPlmnType)
    {
        case   NAS_MMC_PLMN_TYPE_HPLMN:

            pfunGetPlmnCsgIdInList = NAS_MMC_GetEHPlmnCsgIdInCsgIdList;
            break;

        case   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN:

            pfunGetPlmnCsgIdInList = NAS_MMC_GetUserSpecPlmnCsgIdInCsgIdList;
            break;

        case   NAS_MMC_PLMN_TYPE_UPLMN:

            pfunGetPlmnCsgIdInList = NAS_MMC_GetUPlmnCsgIdInCsgIdList;
            break;

        case   NAS_MMC_PLMN_TYPE_OPLMN:

            pfunGetPlmnCsgIdInList = NAS_MMC_GetOPlmnCsgIdInCsgIdList;
            break;

        case   NAS_MMC_PLMN_TYPE_EPLMN:

            pfunGetPlmnCsgIdInList = NAS_MMC_GetEPlmnCsgIdInCsgIdList;
            break;

        case   NAS_MMC_PLMN_TYPE_AVAIL_PLMN:

            pfunGetPlmnCsgIdInList = NAS_MMC_GetAvailablePlmnCsgIdInCsgIdList;
            break;

        case   NAS_MMC_PLMN_TYPE_USER_CSG_SPEC_PLMN:

            pfunGetPlmnCsgIdInList = NAS_MMC_GetUserManualSpecCsgIdInCsgIdList;
            break;

        default:

            return;

    }

    /* ��ȡPLMN��CSG ID���浽�����б��� */
    while (VOS_TRUE == pfunGetPlmnCsgIdInList(ulCsgIdNum, pstPlmnCsgIdWithRatList, &stCsgIdInfo))
    {
        /* �ҵ������ӵ���ǰ�������б��� */
        NAS_MMC_AppendPlmnInPlmnSelectionList(&stCsgIdInfo, pstPlmnSelectionListInfo);

        /* �ڵ�ǰ�б���ɾ����CSG ID */
        stPlmnWithCsgId.stPlmnId = stCsgIdInfo.stPlmnId;
        stPlmnWithCsgId.ulCsgId  = stCsgIdInfo.ulCsgId;
        NAS_MMC_DelCsgIdInSearchList(&stPlmnWithCsgId, &ulCsgIdNum, pstPlmnCsgIdWithRatList);
    }

    *pulPlmnCsgIdNum = ulCsgIdNum;

    return;

}
/*lint +e429*/


#endif


VOS_VOID NAS_MMC_InitPlmnSelectionList_AvailTimerExpire(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_CSG)
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
#endif

    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_CSG)
    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
#endif

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
#if (FEATURE_ON == FEATURE_CSG)
            if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON))
            {
                /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
                PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(stPrioRatList));
                NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
                NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
                NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
            }
            else
#endif
            {
                /* ��ѡ���б�������RPLMN */
                NAS_MMC_AddRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);

                /* ��ѡ���б�������EPLMN */
                NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);
            }

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
#if (FEATURE_ON == FEATURE_CSG)
            if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON))
            {
                /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
                PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(stPrioRatList));
                NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
                NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
                NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
            }
            else
#endif
            {
                /* ��ѡ���б�������RPLMN */
                NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                           NAS_MMC_PLMN_TYPE_RPLMN,
                                           pstPrioRatList,
                                           pstPlmnSelectionListInfo);

                /* ��ѡ���б�������EPLMN */
                NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
            }

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }

    }

    /* �����ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC,
            "NAS_MMC_InitPlmnSelectionList_AvailTimerExpire: Move L mode NW to the end of PLMN Search List");
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_BgHistory(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU        *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU         *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                   *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                    *pEplmnAddr      = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                    stCampPlmnWithRat;
    VOS_UINT32                                    ulEplmnValidFlg;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU          stSearchedExistPlmnInfo[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                                    ulIsExistRplmnOrHplmn;
    VOS_UINT32                                    ulIsSearedPlmnValid;
    NAS_MML_PLMN_RAT_PRIO_STRU                    stPrioRatList;

    pEplmnAddr      = NAS_MML_GetEquPlmnList();
    pstPrioRatList  = NAS_MML_GetMsPrioRatList();

    PS_MEM_SET(&stPrioRatList, 0x00, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }

    }

    /* �����פ��̬����ǰפ������������������ʱ����DPLMN->UPLMN->OPLMN->APLMN���� */
    PS_MEM_SET(&stCampPlmnWithRat, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    stCampPlmnWithRat.enRat             = NAS_MML_GetCurrNetRatType();
    stCampPlmnWithRat.stPlmnId.ulMcc    = NAS_MML_GetCurrCampPlmnId()->ulMcc;
    stCampPlmnWithRat.stPlmnId.ulMnc    = NAS_MML_GetCurrCampPlmnId()->ulMnc;

    /* ��NAS_MML_PLMN_WITH_RAT_STRU�ṹ������ת��ΪNAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU�ṹ������ */
    PS_MEM_SET(stSearchedExistPlmnInfo, 0, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));
    stSearchedExistPlmnInfo[0].enRatType                 = stCampPlmnWithRat.enRat;
    stSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMcc  = stCampPlmnWithRat.stPlmnId.ulMcc;
    stSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMnc  = stCampPlmnWithRat.stPlmnId.ulMnc;
    stSearchedExistPlmnInfo[0].ulHighPlmnNum             = 1;

    /* ����������PLMN�б��м���Ƿ����Rplmn����Hplmn */
    ulIsExistRplmnOrHplmn = NAS_MMC_IsExistRplmnOrHplmnFlag_InitPlmnSelectionList(stSearchedExistPlmnInfo);

    /* �ѵ���PLMN�Ƿ���Ч */
    ulIsSearedPlmnValid   = NAS_MMC_IsSearchedExistPlmnValid(stSearchedExistPlmnInfo);

    /* �������� NV�򿪲���MCC��RPLMN/HPLMN��ͬʱ�����������������б���ǰ�� */
    if ( (VOS_TRUE == NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH, ulIsExistRplmnOrHplmn))
      && (VOS_TRUE == ulIsSearedPlmnValid) )
    {
        /* ��ѡ���б��������� */
        NAS_MMC_SortRoamPlmnSelectionList_InitPlmnSelectionList(&stCampPlmnWithRat, pstPlmnSelectionListInfo);
    }

    /* �����ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_BgHistory: exist cm service");
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedDelCurrCampPlmn_PlmnSelectionStrategy(NAS_MML_GetCurrCampLai()))
    {
        /* �ѵ�ǰפ����������б���ɾ�� */
        NAS_MMC_DelSpecPlmnWithRatInPlmnList(&stCampPlmnWithRat.stPlmnId, stCampPlmnWithRat.enRat, pstPlmnSelectionListInfo);
    }
    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_PrefBandList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList      = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pEplmnAddr          = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstAbortListPlmn    = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astSearchedExistPlmnInfo[NAS_MML_MAX_RAT_NUM];
    NAS_MML_PLMN_WITH_RAT_STRU                              stSearchedPlmn;
    VOS_UINT32                                              ulEplmnValidFlg;
    VOS_UINT32                                              ulIsExistRplmnOrHplmn;
    VOS_UINT32                                              ulIsSearedPlmnValid;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;

    pEplmnAddr      = NAS_MML_GetEquPlmnList();
    pstPrioRatList  = NAS_MML_GetMsPrioRatList();

    PS_MEM_SET(&stPrioRatList, 0x00, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }

    }

    pstAbortListPlmn    = NAS_MMC_GetAbortListSearchPlmn();

    NAS_MEM_SET_S(astSearchedExistPlmnInfo, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU),
                  0, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));

    NAS_MEM_SET_S(&stSearchedPlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU),
                  0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    stSearchedPlmn.enRat             = NAS_MML_NET_RAT_TYPE_BUTT;
    stSearchedPlmn.stPlmnId.ulMcc    = NAS_MML_INVALID_MCC;
    stSearchedPlmn.stPlmnId.ulMnc    = NAS_MML_INVALID_MNC;

    /* �������Ϊ���б��ѹ����н�����ϱ�searched info ind��ϵģ����ݴ�ϵ�MCC������������ */
    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&(pstAbortListPlmn->stPlmnId)))
    {
        NAS_NORMAL_LOG3(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_PrefBandList: Abort List Plmn is: ", pstAbortListPlmn->enRat, pstAbortListPlmn->stPlmnId.ulMcc, pstAbortListPlmn->stPlmnId.ulMnc);

        /* ��NAS_MML_PLMN_WITH_RAT_STRU�ṹ������ת��ΪNAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU�ṹ������ */
        astSearchedExistPlmnInfo[0].enRatType                 = pstAbortListPlmn->enRat;
        astSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMcc  = pstAbortListPlmn->stPlmnId.ulMcc;
        astSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMnc  = pstAbortListPlmn->stPlmnId.ulMnc;
        astSearchedExistPlmnInfo[0].ulHighPlmnNum             = 1;

        NAS_MEM_CPY_S(&stSearchedPlmn,  sizeof(NAS_MML_PLMN_WITH_RAT_STRU),
                      pstAbortListPlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    }
    else
    {
        /* �յ�searched info indʱû�д�ϣ������б��ѵĽ���������Ҫ�������б��еĵ�һ��PLMN��Ӧ��MCC�������� */
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_PrefBandList: No PLMN Abort List Search");

        if (VOS_NULL_PTR != pstSearchedPlmnListInfo)
        {
            NAS_MEM_CPY_S(astSearchedExistPlmnInfo, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU),
                          pstSearchedPlmnListInfo,  NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));

            NAS_MMC_BuildPlmnInfoWithRatBySearchedPlmnList(pstSearchedPlmnListInfo,&stSearchedPlmn);
        }
    }

    /* ����������PLMN�б��м���Ƿ����Rplmn����Hplmn */
    ulIsExistRplmnOrHplmn = NAS_MMC_IsExistRplmnOrHplmnFlag_InitPlmnSelectionList(astSearchedExistPlmnInfo);

    /* �ѵ���PLMN�Ƿ���Ч */
    ulIsSearedPlmnValid   = NAS_MMC_IsSearchedExistPlmnValid(astSearchedExistPlmnInfo);

    /* �������Rplmn����Hplmn�ñ�־λ */
    if (VOS_TRUE == ulIsExistRplmnOrHplmn)
    {
        NAS_MMC_SetExistRplmnOrHplmnFlag_PlmnSelection(VOS_TRUE);
    }

    /* �������� NV�򿪲���MCC��RPLMN/HPLMN��ͬʱ�����������������б���ǰ�� */
    if ( (VOS_TRUE == NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_PREFBAND_LIST_PLMN_SEARCH, ulIsExistRplmnOrHplmn))
      && (VOS_TRUE == ulIsSearedPlmnValid) )
    {
        NAS_MMC_SortRoamPlmnSelectionList_InitPlmnSelectionList(&stSearchedPlmn, pstPlmnSelectionListInfo);
    }

    /* �����ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_ListPrefBand: exist cm service");
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    NAS_MMC_InitAbortListSearchPlmn();

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_FullBandList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pEplmnAddr      = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstAbortListPlmn    = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astSearchedExistPlmnInfo[NAS_MML_MAX_RAT_NUM];
    NAS_MML_PLMN_WITH_RAT_STRU                              stSearchedPlmn;
    VOS_UINT32                                              ulEplmnValidFlg;
    VOS_UINT32                                              ulIsExistRplmnOrHplmn;
    VOS_UINT32                                              ulIsSearedPlmnValid;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;

    pEplmnAddr      = NAS_MML_GetEquPlmnList();
    pstPrioRatList  = NAS_MML_GetMsPrioRatList();

    PS_MEM_SET(&stPrioRatList, 0x00, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }

    }

    pstAbortListPlmn    = NAS_MMC_GetAbortListSearchPlmn();

    NAS_MEM_SET_S(astSearchedExistPlmnInfo, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU),
                  0, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));

    NAS_MEM_SET_S(&stSearchedPlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU),
                  0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    stSearchedPlmn.enRat             = NAS_MML_NET_RAT_TYPE_BUTT;
    stSearchedPlmn.stPlmnId.ulMcc    = NAS_MML_INVALID_MCC;
    stSearchedPlmn.stPlmnId.ulMnc    = NAS_MML_INVALID_MNC;

    /* �������Ϊ���б��ѹ����н�����ϱ�searched info ind��ϵģ����ݴ�ϵ�MCC������������ */
    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&(pstAbortListPlmn->stPlmnId)))
    {
        NAS_NORMAL_LOG3(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_FullBandList: Abort List Plmn is: ", pstAbortListPlmn->enRat, pstAbortListPlmn->stPlmnId.ulMcc, pstAbortListPlmn->stPlmnId.ulMnc);

        /* ��NAS_MML_PLMN_WITH_RAT_STRU�ṹ������ת��ΪNAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU�ṹ������ */
        PS_MEM_SET(astSearchedExistPlmnInfo, 0, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));
        astSearchedExistPlmnInfo[0].enRatType                 = pstAbortListPlmn->enRat;
        astSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMcc  = pstAbortListPlmn->stPlmnId.ulMcc;
        astSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMnc  = pstAbortListPlmn->stPlmnId.ulMnc;
        astSearchedExistPlmnInfo[0].ulHighPlmnNum             = 1;

        NAS_MEM_CPY_S(&stSearchedPlmn,  sizeof(NAS_MML_PLMN_WITH_RAT_STRU),
                      pstAbortListPlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    }
    else
    {
        /* �յ�searched info indʱû�д�ϣ������б��ѵĽ���������Ҫ�������б��еĵ�һ��PLMN��Ӧ��MCC�������� */
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_FullBandList: No PLMN Abort List Search");

        if (VOS_NULL_PTR != pstSearchedPlmnListInfo)
        {
            NAS_MEM_CPY_S(astSearchedExistPlmnInfo, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU),
                          pstSearchedPlmnListInfo,  NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));

            NAS_MMC_BuildPlmnInfoWithRatBySearchedPlmnList(pstSearchedPlmnListInfo,&stSearchedPlmn);
        }
    }

    /* ����������PLMN�б��м���Ƿ����Rplmn����Hplmn */
    ulIsExistRplmnOrHplmn = NAS_MMC_IsExistRplmnOrHplmnFlag_InitPlmnSelectionList(astSearchedExistPlmnInfo);

    /* �ѵ���PLMN�Ƿ���Ч */
    ulIsSearedPlmnValid   = NAS_MMC_IsSearchedExistPlmnValid(astSearchedExistPlmnInfo);

    /* �������Rplmn����Hplmn�ñ�־λ */
    if (VOS_TRUE == ulIsExistRplmnOrHplmn)
    {
        NAS_MMC_SetExistRplmnOrHplmnFlag_PlmnSelection(VOS_TRUE);
    }

    /* �������� NV�򿪲���MCC��RPLMN/HPLMN��ͬʱ�����������������б���ǰ�� */
    if ( (VOS_TRUE == NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_FULLBAND_LIST_PLMN_SEARCH, ulIsExistRplmnOrHplmn))
      && (VOS_TRUE == ulIsSearedPlmnValid) )
    {
        /* ���������б����� */
        NAS_MMC_SortRoamPlmnSelectionList_InitPlmnSelectionList(&stSearchedPlmn, pstPlmnSelectionListInfo);
    }

    /* �����ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_ListFullBand: exist cm service");
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    NAS_MMC_InitAbortListSearchPlmn();
    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_UserList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;

    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;

    VOS_UINT8                           ucPsAttachAllowFlg;
    VOS_UINT8                           ucCsAttachAllowFlg;

    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstRPlmnCfg    = NAS_MML_GetRplmnCfg();

    /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(pstRPlmnCfg->enLastRplmnRat, &stPrioRatList);

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }

    }

    /* �������б���������svlte ˫��ģʽ��g�Ľ��뼼���ŵ��б����,��������Ҳ���t��l��������� */
    ucPsAttachAllowFlg = NAS_MML_GetPsAttachAllowFlg();
    ucCsAttachAllowFlg = NAS_MML_GetCsAttachAllowFlg();

    if ((VOS_TRUE == NAS_MML_GetSvlteSupportFlag())
     && (VOS_FALSE == ucPsAttachAllowFlg)
     && (VOS_FALSE == ucCsAttachAllowFlg))
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_GSM,
                                                pstPlmnSelectionListInfo);
    }

    return;

}


VOS_VOID NAS_MMC_InitPlmnSelectionList_RegRejPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU            stPrioRatList;
    VOS_UINT8                             ucKeepSrchHplmnFlg;

    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU  stSearchedExistPlmnInfo[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                            ulIsExistRplmnOrHplmn;
    VOS_UINT32                            ulIsSearedPlmnValid;

    NAS_MML_PLMN_WITH_RAT_STRU            stCurrPlmnWithRat;
    NAS_MML_PLMN_RAT_PRIO_STRU           *pstPrioRatList = VOS_NULL_PTR;

    stCurrPlmnWithRat.enRat             = NAS_MML_GetCurrNetRatType();
    stCurrPlmnWithRat.stPlmnId.ulMcc    = NAS_MML_GetCurrCampPlmnId()->ulMcc;
    stCurrPlmnWithRat.stPlmnId.ulMnc    = NAS_MML_GetCurrCampPlmnId()->ulMnc;

    ucKeepSrchHplmnFlg = NAS_MMC_IsNeedSrchCurrCampHpmnRejByCause13();

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_RegRejPlmnSearch: ucKeepSrchHplmnFlg =", ucKeepSrchHplmnFlg);

    if (VOS_TRUE == ucKeepSrchHplmnFlg)
    {
        PS_MEM_SET(&stPrioRatList, 0x00, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
        stPrioRatList.ucRatNum = 1;
        stPrioRatList.aucRatPrio[0] = NAS_MML_GetCurrNetRatType();

        NAS_MMC_AddSpecPlmnInPlmnSelectionList(NAS_MML_GetCurrCampPlmnId(),
                                            NAS_MMC_PLMN_TYPE_HPLMN,
                                            &stPrioRatList,
                                            pstPlmnSelectionListInfo);
    }

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

        /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
        pstPrioRatList = NAS_MML_GetMsPrioRatList();
        PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

        /* ��ѡ���б�������RPLMN */
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        if (VOS_FALSE == ucKeepSrchHplmnFlg)
        {
            /* ɾ����ǰע�ᱻ�ܵ����� */
            NAS_MMC_DelSpecPlmnWithRatInPlmnList(NAS_MML_GetCurrCampPlmnId(),
                                                 NAS_MML_GetCurrNetRatType(),
                                                 pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* ��ѡ���б��������û�ָ������ */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    /* ��NAS_MML_PLMN_WITH_RAT_STRU�ṹ������ת��ΪNAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU�ṹ������ */
    PS_MEM_SET(stSearchedExistPlmnInfo, 0, NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));
    stSearchedExistPlmnInfo[0].enRatType                 = stCurrPlmnWithRat.enRat;
    stSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMcc  = stCurrPlmnWithRat.stPlmnId.ulMcc;
    stSearchedExistPlmnInfo[0].astHighPlmnList[0].ulMnc  = stCurrPlmnWithRat.stPlmnId.ulMnc;
    stSearchedExistPlmnInfo[0].ulHighPlmnNum             = 1;

    /* ����������PLMN�б��м���Ƿ����Rplmn����Hplmn */
    ulIsExistRplmnOrHplmn = NAS_MMC_IsExistRplmnOrHplmnFlag_InitPlmnSelectionList(stSearchedExistPlmnInfo);

    /* �ѵ���PLMN�Ƿ���Ч */
    ulIsSearedPlmnValid   = NAS_MMC_IsSearchedExistPlmnValid(stSearchedExistPlmnInfo);


    /* �������� NV�򿪲��ҵ�ǰפ��MCC��RPLMN/HPLMN��ͬʱ�����������������б���ǰ�� */
    if (VOS_TRUE == NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_PLMN_SEARCH, ulIsExistRplmnOrHplmn)
    && (VOS_TRUE == ulIsSearedPlmnValid))
    {
        /* ��ѡ���б��������� */
        NAS_MMC_SortRoamPlmnSelectionList_InitPlmnSelectionList(&stCurrPlmnWithRat, pstPlmnSelectionListInfo);
    }

    /* �����ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_RegRejPlmnSearch: CSFB exists, Move L RAT to the end of PLMN Selection List");
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    return;

}


VOS_VOID NAS_MMC_InitPlmnSelectionList_DisableLte(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList = VOS_NULL_PTR;
    VOS_UINT32                                              ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU                              *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {

        /* ��ѡ���б������� RPLMN */
        NAS_MMC_AddRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б������� EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);


        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {

        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б������� RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       pstPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б������� EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
        }


        /* ��ѡ���б��������û�ָ������ */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    return;

}


VOS_VOID NAS_MMC_InitPlmnSelectionList_RegRejCurPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8        enPlmnType;
    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnList = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU               *pstPlmnId = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstPlmnId      = NAS_MML_GetCurrCampPlmnId();
    pstEquPlmnList = NAS_MML_GetEquPlmnList();

    /* ���ӵ�Plmn������Ĭ��ΪAvailPlmn */
    enPlmnType = NAS_MMC_PLMN_TYPE_AVAIL_PLMN;


    /* Rplmn�� EplmnList[0]��ͬʱ������ΪRplmn���� */
    if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId, &(pstEquPlmnList->astEquPlmnAddr[0]) ))
    {
        enPlmnType = NAS_MMC_PLMN_TYPE_RPLMN;
    }


    /* ��ѡ���б������ӵ�ǰ����,��ǰ���뼼��������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);

    NAS_MMC_AddSpecPlmnInPlmnSelectionList(pstPlmnId,
                                           enPlmnType,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б��������û�ָ������ */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    /* �����ǰ����CSFBҵ���־��ѡ���б��������֮����Ҫ��LTE�����������ȼ�����GSM��WCDMA֮�� */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_UserSpecPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                stRPlmn;
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg     = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
    PS_MEM_SET(&stRPlmn, 0, sizeof(NAS_MML_PLMN_ID_STRU));

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ��ȡRPLMN��������Ϣ */
    pstRPlmnCfg    = NAS_MML_GetRplmnCfg();

    /* ��ѡ���б��������û�ָ������,�û�ָ�����뼼��������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(pstUserSpecPlmn->enRat, &stPrioRatList);
    NAS_MMC_AddSpecPlmnInPlmnSelectionList(&pstUserSpecPlmn->stPlmnId,
                                           NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

    (VOS_VOID)NAS_MMC_GetRPlmn_PlmnSelectionStrategy(pstRPlmnCfg->enLastRplmnRat, &stRPlmn);

    /* �û�ָ��������RPLMN��ͬʱ��Ҫ��EPLMN���ӵ�ѡ���б� */
    if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stRPlmn,
                                                       &(pstUserSpecPlmn->stPlmnId)))
    {
        NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_ManualChangeAutoMode(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList   = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserReselPlmn = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ��޸�Ϊ�ֶ�ʱ�ṩ������������� */
    pstUserReselPlmn = NAS_MMC_GetUserReselPlmnId();

    /* �Զ��޸�Ϊ�ֶ�ʱ������ṩ������������Ľ��뼼��������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(pstUserReselPlmn->enRat, &stPrioRatList);
    NAS_MMC_AddSpecPlmnInPlmnSelectionList(&pstUserReselPlmn->stPlmnId,
                                           NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

    /* ��ѡ���б�������EPLMN */
    NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    /* ��ѡ���б�������UPLMN��OPLMN */
    NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}

#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MMC_InitPlmnSelectionListCsfbServiceRej(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
#if (FEATURE_ON == FEATURE_CSG)
        if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_CSFB_SERVIEC_REJ))
        {
            /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
            NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
            NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
#endif
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
#if (FEATURE_ON == FEATURE_CSG)
            if (VOS_TRUE == NAS_MMC_IsNeedAddDuplicateRPlmnAndEplmnInPlmnList(NAS_MMC_PLMN_SEARCH_SCENE_CSFB_SERVIEC_REJ))
            {
                /* ��ѡ���б��м���duplicate rplmn��duplicate rplmn�д�Ľ��뼼������ */
                NAS_MML_SortSpecRatPrioHighest(NAS_MMC_GetCsgSpecSrchDuplicateRplmnRat(), &stPrioRatList);
                NAS_MMC_AddCsgSpecDuplicateRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);
                NAS_MMC_AddCsgSpecDuplicateEPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
            }
            else
#endif
            {
                /* ��ѡ���б�������RPLMN  */
                NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                           NAS_MMC_PLMN_TYPE_RPLMN,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

                /* ��ѡ���б�������EPLMN */
                NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

                /* ��ѡ���б��������û�ָ������ */
                NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
            }
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

    }

    if (VOS_TRUE == NAS_MMC_IsNetRatSupportedTdscdma())
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_WCDMA,
                                                pstPlmnSelectionListInfo);
    }

    /* �������б��������򣬽�L���뼼���ŵ��б����*/
    NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE,
                                                pstPlmnSelectionListInfo);
    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_EnableLte(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������RPLMN,LTE���뼼��������ǰ */
        NAS_MML_SortSpecRatPrioHighest(NAS_MML_NET_RAT_TYPE_LTE, &stPrioRatList);

        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,LTE���뼼��������ǰ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MML_NET_RAT_TYPE_LTE, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,LTE���뼼��������ǰ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MML_NET_RAT_TYPE_LTE, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }
    }
    return;
}




VOS_VOID NAS_MMC_InitPlmnSelectionList_CsfbAbort(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������RPLMN,LTE���뼼��������ǰ */
        NAS_MML_SortSpecRatPrioHighest(NAS_MML_NET_RAT_TYPE_LTE, &stPrioRatList);

        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,LTE���뼼��������ǰ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MML_NET_RAT_TYPE_LTE, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,LTE���뼼��������ǰ */
            NAS_MML_SortSpecRatPrioHighest(NAS_MML_NET_RAT_TYPE_LTE, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }
    }
    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_CsfbEnd(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        NAS_MMC_AddRPlmnInPlmnSelectionList(NAS_MML_GetMsPrioRatList(), pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       NAS_MML_GetMsPrioRatList(),
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   NAS_MML_GetMsPrioRatList(),
                                                   pstPlmnSelectionListInfo);
        }
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_MoCallAbortFR(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ѡ���б�������RPLMN,��ǰ���뼼��������ǰ */
    NAS_MML_SortSpecRatPrioHighest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();
        pEplmnAddr      = NAS_MML_GetEquPlmnList();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }
    }

    NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE, pstPlmnSelectionListInfo);

    return;
}

#endif




VOS_VOID NAS_MMC_InitPlmnSelectionListRfEnable(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stDestFirstPlmnRatInfo;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enNoRfPlmnRat;
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT8                                               ucExistNoRFRat;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList      = VOS_NULL_PTR;
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstRatSearcchInfo   = VOS_NULL_PTR;

    pstRatSearcchInfo   = NAS_MMC_GetSearchRatInfo_PlmnSelection();

    /* ������ʼ�� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    enNoRfPlmnRat  = NAS_MML_GetNoRfRatType();

    ucExistNoRFRat = VOS_FALSE;
    PS_MEM_SET(&stDestFirstPlmnRatInfo, 0X00, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    /* �����ǰMMC�б�����ѡ���б�����ָ����뱸�ݵ�ѡ���б� */
    if ( NAS_MMC_BACKUP_PLMN_SELECTION_LIST == NAS_MMC_GetPlmnSelectionListType() )
    {
        PS_MEM_CPY(pstPlmnSelectionListInfo, NAS_MMC_GetHighPrioPlmnList(), sizeof(NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU));

        /* �ָ�NO RF ��Դ���ú�,���������Դ�Ļָ� */
        NAS_MMC_RestoreNoRfHistorySearchedRatInfo_PlmnSelection(pstRatSearcchInfo);

        /* NO RF������,�����ǰ�Ǳ���ѡ���б���Ϣ�������³�ʼ��Ϊ�����ȼ��б���Ϣ */
        if (VOS_FALSE == NAS_MML_GetRfAvailFlg())
        {
            NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                          VOS_NULL_PTR,
                                          NAS_MMC_GetHighPrioPlmnList());
            NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
        }

        return;
    }

    /* ����������������ʱ����ʱ���������б�������NO RFʱ��Ľ��뼼������Ϊ��һ���ȼ� */
    NAS_MMC_InitPlmnSelectionList_AvailTimerExpire(pstPlmnSelectionListInfo, pstSearchedPlmnListInfo);

    /* �쳣�����������ǰĿ����뼼��ΪBUTT,�򲻽��е��� */
    if ( NAS_MML_NET_RAT_TYPE_BUTT == enNoRfPlmnRat )
    {
        return;
    }

    /* �쳣�����������ǰ���������ˣ��򲻽��й��� */
    if ( NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST == pstPlmnSelectionListInfo->usSearchPlmnNum )
    {
        return;
    }

    /* ��ǰ�Ľ��뼼������Ϊ1��������Ҫ���������б� */
    if ( 1 == pstPrioRatList->ucRatNum)
    {
        return;
    }

    /* ������ѡ���б��в���Ŀ����뼼����������Ϣ */
    for ( i = 0 ; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++ )
    {
        for (j = 0; j < pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum; j++)
        {
            /* �ҵ�Ŀ������뼼����������ڵ�һλ���ܸ�������Ҫ���� */
            if ( enNoRfPlmnRat == pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j].enRatType )
            {
                PS_MEM_CPY(&stDestFirstPlmnRatInfo,
                           &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]),
                           sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

                /* ��Ŀ������Ľ��뼼������Ϊ����һ�� */
                stDestFirstPlmnRatInfo.ucRatNum = 1;
                PS_MEM_CPY(&(stDestFirstPlmnRatInfo.astPlmnRatInfo[0]),
                           &(pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j]),
                           sizeof(NAS_MMC_PLMN_RAT_INFO_STRU) );

                ucExistNoRFRat = VOS_TRUE;

                break;
            }
        }

        /* �ҵ���Ŀ����뼼������ֱ������ */
        if ( VOS_TRUE == ucExistNoRFRat )
        {
            break;
        }
    }

    /* û�ҵ�Ŀ����뼼��������һ����Ӧ�Ľ��뼼�� */
    if ( VOS_FALSE == ucExistNoRFRat )
    {
        /* ����Ҳ�����Ӧ�Ľ��뼼��������Ϣ������������һ��Ŀ����뼼������ */
        PS_MEM_CPY(&stDestFirstPlmnRatInfo,
                   &(pstPlmnSelectionListInfo->astPlmnSelectionList[0]),
                   sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

        stDestFirstPlmnRatInfo.ucRatNum = 1;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].enRatType      = enNoRfPlmnRat;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].enNetStatus    = NAS_MMC_NET_STATUS_BUTT;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].enQuality      = NAS_MMC_NET_QUALITY_BUTT;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].ucReserve      = 0;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].lRscp          = NAS_MML_UTRA_RSCP_UNVALID;
    }

    /* ����ǰѡ���б�����һλ�󣬽�Ŀ����뼼���������ѡ���б� */
    (VOS_VOID)PS_MEM_MOVE(&(pstPlmnSelectionListInfo->astPlmnSelectionList[1]),
                &(pstPlmnSelectionListInfo->astPlmnSelectionList[0]),
                (pstPlmnSelectionListInfo->usSearchPlmnNum) * sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));


    PS_MEM_CPY(&(pstPlmnSelectionListInfo->astPlmnSelectionList[0]),
               &stDestFirstPlmnRatInfo,
               sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    pstPlmnSelectionListInfo->usSearchPlmnNum++;

    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_RegNCell(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_NCELL_SEARCH_INFO_STRU     *pstMmcNcellSearchInfo = VOS_NULL_PTR;

    pstMmcNcellSearchInfo = NAS_MMC_GetNcellSearchInfo();

    /* ĿǰNCELL��֧��LTE���뼼�� */
    stPrioRatList.ucRatNum      = 1;
    stPrioRatList.aucRatPrio[0] = NAS_MML_NET_RAT_TYPE_LTE;

    /* ��ѡ���б�������NCELL��Ϣ�е�PLMN ID */
    for (i = 0; i < pstMmcNcellSearchInfo->stOtherModemEplmnInfo.ucEquPlmnNum; i ++)
    {
        NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstMmcNcellSearchInfo->stOtherModemEplmnInfo.astEquPlmnAddr[0]),
                                   NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                   &stPrioRatList,
                                   pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_RegHPlmn(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_RegPrefPlmn(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    if (VOS_FALSE == NAS_MML_Get3GPP2UplmnNotPrefFlg())
    {
        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* ��ѡ���б�������OPLMN */
        NAS_MMC_AddOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_HighPrioLtePlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_ID_STRU                stTmpPlmnId;

    VOS_UINT32                                              ulHighPlmnIndex;
    VOS_UINT32                                              ulLowPlmnIndex;
    VOS_UINT8                                               ucRatIndex;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType;

    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    if (VOS_FALSE == NAS_MML_Get3GPP2UplmnNotPrefFlg())
    {
        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* ��ѡ���б�������OPLMN */
        NAS_MMC_AddOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    /* ������ѡ��LTE��PLMN�Ѿ�������һ�飬���������Ҫʹ���ѵ�����ѡ
       PLMN�б�ˢ����PLMN״̬�����ڲ����ڵ�PLMN�ڸ����ȼ�����ʱ
       ֱ���������������Ч��; */

    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        stTmpPlmnId = pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId;

        if (VOS_FALSE == NAS_MMC_IsPlmnIdInSearchedPlmnList(&stTmpPlmnId, pstSearchedPlmnListInfo))
        {
            /* δ���ҵ���ͬ����ѡPLMN,������ͬ���뼼����PLMN(Ŀǰ��������뼼����ȻΪLTE),����״
               ̬��Ҫ����Ϊ�ѹ�������,��֤����ʱ����������PLMN */
            NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(pstSearchedPlmnListInfo->enRatType,
                                                    NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                                    &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]));
        }
    }



    /* ��bsrʱhsd�������ѵ���plmn׷�ӵ�plmnѡ���б���(�ظ��Ĳ���) */

    PS_MEM_SET(&stPlmnInfo, 0x00, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    for (ucRatIndex = 0; ucRatIndex < NAS_MML_MAX_RAT_NUM; ucRatIndex++)
    {
        enRatType = pstSearchedPlmnListInfo[ucRatIndex].enRatType;

        if (NAS_MML_NET_RAT_TYPE_BUTT == enRatType)
        {
            continue;
        }

        /* �Ը��ź��������� */
        for (ulHighPlmnIndex = 0; ulHighPlmnIndex < pstSearchedPlmnListInfo[ucRatIndex].ulHighPlmnNum; ulHighPlmnIndex++)
        {
            stPlmnInfo.stPlmnId.ulMcc = pstSearchedPlmnListInfo[ucRatIndex].astHighPlmnList[ulHighPlmnIndex].ulMcc;
            stPlmnInfo.stPlmnId.ulMnc = pstSearchedPlmnListInfo[ucRatIndex].astHighPlmnList[ulHighPlmnIndex].ulMnc;

            /* �ѵ���plmn�������еĹ����������б��У�Ҫ׷�� */
            stPlmnInfo.ucRatNum                    = 1;
            stPlmnInfo.astPlmnRatInfo[0].enRatType = enRatType;

            if (VOS_FALSE == NAS_MMC_IsPlmnWithRatInSearchList(stPlmnInfo, pstPlmnSelectionListInfo))
            {
                NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_HighPrioLtePlmnSearch, high srched plmn is not in plmn selectionlist:",
                                    stPlmnInfo.stPlmnId.ulMcc, stPlmnInfo.stPlmnId.ulMnc);

                stPrioRatList.ucRatNum      = 1;
                stPrioRatList.aucRatPrio[0] = pstSearchedPlmnListInfo[ucRatIndex].enRatType;

                NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(stPlmnInfo.stPlmnId),
                                                        NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                        &stPrioRatList,
                                                        pstPlmnSelectionListInfo);


            }
        }

        /* �Ե��ź��������� */
        for (ulLowPlmnIndex = 0; ulLowPlmnIndex < pstSearchedPlmnListInfo[ucRatIndex].ulLowPlmnNum; ulLowPlmnIndex++)
        {
            stPlmnInfo.stPlmnId.ulMcc = pstSearchedPlmnListInfo[ucRatIndex].astLowPlmnList[ulLowPlmnIndex].stPlmnId.ulMcc;
            stPlmnInfo.stPlmnId.ulMnc = pstSearchedPlmnListInfo[ucRatIndex].astLowPlmnList[ulLowPlmnIndex].stPlmnId.ulMnc;

            /* �ѵ���plmn�������еĹ����������б��У�Ҫ׷�� */
            stPlmnInfo.ucRatNum                    = 1;
            stPlmnInfo.astPlmnRatInfo[0].enRatType = enRatType;

            if (VOS_FALSE == NAS_MMC_IsPlmnWithRatInSearchList(stPlmnInfo, pstPlmnSelectionListInfo))
            {
                NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_InitPlmnSelectionList_HighPrioLtePlmnSearch, low srched plmn is not in plmn selectionlist:",
                                    stPlmnInfo.stPlmnId.ulMcc, stPlmnInfo.stPlmnId.ulMnc);

                stPrioRatList.ucRatNum      = 1;
                stPrioRatList.aucRatPrio[0] = pstSearchedPlmnListInfo[ucRatIndex].enRatType;

                NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(stPlmnInfo.stPlmnId),
                                                        NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                        &stPrioRatList,
                                                        pstPlmnSelectionListInfo);
            }
        }
    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_DplmnSet(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    /* ͬ������������GEO��Ϣʱ��ʹ��AP�����������µ�DPLMN�б����������б�
       ������GEO��Ϣ����RPLMN/HPLMNʱ������㱨PLMN_INFO_IND��Ҫ��ϣ���ʱ�յ�DPLMN SET�ĳ���
       ��Ҫ��RPLMN/HPLMN��ʼ�ѣ����յ�PLMN_INFO_IND��Ҫ��ϣ���ʱ��ʹ��AP�����������µ�DPLMN�б� */

    NAS_MMC_InitPlmnSelectionList_SwitchOn(pstPlmnSelectionListInfo, pstSearchedPlmnListInfo);

    return;
}


VOS_VOID NAS_MMC_BuildPlmnSelectionPlmnInfo(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    VOS_UINT16                                              usSimRat,
    NAS_MML_PLMN_ID_STRU                                   *pstPlmnId,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo
)
{
    VOS_UINT8                           ucRatNum;

    ucRatNum                = 0x0;

    PS_MEM_CPY(&pstPlmnInfo->stPlmnId, pstPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
    pstPlmnInfo->enPlmnType = enPlmnType;

#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_SIM_RAT_E_UTRN == (NAS_MML_SIM_RAT_E_UTRN & usSimRat))
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_LTE;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        ucRatNum++;
    }
#endif

    if (NAS_MML_SIM_RAT_UTRN == (NAS_MML_SIM_RAT_UTRN & usSimRat))
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_WCDMA;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        ucRatNum++;
    }

    if (NAS_MML_SIM_RAT_GSM == (NAS_MML_SIM_RAT_GSM & usSimRat))
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_GSM;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        ucRatNum++;
    }

    pstPlmnInfo->ucRatNum = ucRatNum;

    /* ����ʣ��λ�ø�ֵΪ��Чֵ */
    for (; ucRatNum < NAS_MML_MAX_RAT_NUM; ucRatNum++)
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
    }

    return;
}



VOS_VOID NAS_MMC_AddEHPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_SIM_EHPLMN_INFO_STRU                           *pstEHPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;

    /* ��ȡEHPLMN */
    pstEHPlmnInfo = NAS_MML_GetSimEhplmnList();

    for (i = 0; i< pstEHPlmnInfo->ucEhPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_HPLMN,
                                           pstEHPlmnInfo->astEhPlmnInfo[i].usSimRat,
                                           &(pstEHPlmnInfo->astEhPlmnInfo[i].stPlmnId),
                                           &stPlmnInfo);

        /* ���ӵ������б� */
        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_AddEHPlmnInDestPlmnList(
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstDestPlmnList,
    NAS_MML_SIM_EHPLMN_INFO_STRU                           *pstEHPlmnInfo
)
{
    VOS_UINT32                                              i;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat = VOS_NULL_PTR;
    VOS_UINT32                                              ulIndexOfPlmnList;
    NAS_MML_PLMN_ID_STRU                                    stPlmnId;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnIdWithRat;
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledRatPlmnCfg   = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stTempPlmn;

    if ( 0 == pstDestPlmnList->ulPlmnNum )
    {
        return;
    }

    /* ��ȡ��ֹ���뼼����PLMN��Ϣ */
    pstDisabledRatPlmnCfg = NAS_MML_GetDisabledRatPlmnInfo();

    /* ���б��ĵ�0λΪRPLMN */
    pstPlmnWithRat = &pstDestPlmnList->astPlmnId[0];


    PS_MEM_SET(&stTempPlmn, 0x00, sizeof(stTempPlmn));

    for (i = 0, (ulIndexOfPlmnList = 1); i< NAS_MML_MIN(pstEHPlmnInfo->ucEhPlmnNum, NAS_MML_MAX_EHPLMN_NUM); i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        PS_MEM_SET(&stPlmnId, 0x00, sizeof(stPlmnId));
        PS_MEM_SET(&stPlmnIdWithRat, 0x00, sizeof(stPlmnIdWithRat));
        PS_MEM_CPY(&stPlmnId, &(pstEHPlmnInfo->astEhPlmnInfo[i].stPlmnId), sizeof(stPlmnId));
        PS_MEM_CPY(&stPlmnIdWithRat.stPlmnId, &stPlmnId, sizeof(stPlmnIdWithRat.stPlmnId));

        stPlmnIdWithRat.enRat = pstPlmnWithRat->enRat;

        if ( VOS_TRUE == NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList(&stPlmnIdWithRat, pstDestPlmnList->ulPlmnNum, pstDestPlmnList->astPlmnId) )
        {
            continue;
        }

        /* �жϸ�PLMN�Ƿ��ںڰ������� */
        if ( VOS_TRUE == NAS_MML_IsPlmnIdForbiddenByWhiteBlackList(&stPlmnId) )
        {
            continue;
        }

        /* ��װstTempPlmn�������ж��Ƿ񱻽����뼼�� */
        stTempPlmn.enPlmnType                  = NAS_MMC_PLMN_TYPE_HPLMN;
        stTempPlmn.ucRatNum                    = 1;
        stTempPlmn.astPlmnRatInfo[0].enRatType = pstPlmnWithRat->enRat;
        PS_MEM_CPY(&stTempPlmn.stPlmnId, &stPlmnId, sizeof(stTempPlmn.stPlmnId));

        /* ɾ���ڽ�ֹ���뼼��������Ľ��뼼�� */
        NAS_MMC_DelPlmnSelectionRatInfoInDisabledPlmnWithForbiddenPeriodList(&stTempPlmn,
                                                  pstDisabledRatPlmnCfg->ulDisabledRatPlmnNum,
                                                  pstDisabledRatPlmnCfg->astDisabledRatPlmnId);

        /* ɾ���ڽ�ֹ���뼼���б��еĽ��뼼�� */
        NAS_MMC_DeleteForbiddenRatInPlmnSeleInfo(&stTempPlmn);

        /* ����������Ľ��뼼������Ϊ�㣬������ */
        if (0 == stTempPlmn.ucRatNum)
        {
            continue;
        }

        if ( NAS_MML_MAX_EHPLMN_NUM <= pstDestPlmnList->ulPlmnNum )
        {
            break;
        }

        pstDestPlmnList->astPlmnId[ulIndexOfPlmnList].enRat          = pstPlmnWithRat->enRat;

        PS_MEM_CPY(&(pstDestPlmnList->astPlmnId[ulIndexOfPlmnList].stPlmnId), &stPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
        pstDestPlmnList->ulPlmnNum++;
        ulIndexOfPlmnList++;
    }

    return;
}



VOS_VOID NAS_MMC_AddEPlmnInDestPlmnList(
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstDestPlmnList
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulIndexOfPlmnList;
    VOS_UINT32                                              ulCurPlmnNum;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnIdWithRat;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));
    PS_MEM_SET(&stPlmnIdWithRat, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    ulCurPlmnNum = pstDestPlmnList->ulPlmnNum;

    if ( 0 == ulCurPlmnNum )
    {
        return;
    }

    pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
    PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
    NAS_MMC_BuildEquPlmnInfo(&stSndEquPlmnInfo);

    for (i = 0, (ulIndexOfPlmnList = ulCurPlmnNum); i< NAS_MML_MIN(stSndEquPlmnInfo.ucEquPlmnNum, NAS_MML_MAX_EHPLMN_NUM); i++)
    {
        stPlmnIdWithRat.stPlmnId.ulMcc = stSndEquPlmnInfo.astEquPlmnAddr[i].ulMcc;
        stPlmnIdWithRat.stPlmnId.ulMnc = stSndEquPlmnInfo.astEquPlmnAddr[i].ulMnc;
        stPlmnIdWithRat.enRat          = pstDestPlmnList->astPlmnId[0].enRat;

        /* ��Ҫ�����eplmn�Ѿ����б��������� */
        if ( VOS_TRUE == NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList(&stPlmnIdWithRat, pstDestPlmnList->ulPlmnNum, pstDestPlmnList->astPlmnId) )
        {
            continue;
        }

        pstDestPlmnList->astPlmnId[ulIndexOfPlmnList].enRat = stPlmnIdWithRat.enRat;
        PS_MEM_CPY(&(pstDestPlmnList->astPlmnId[ulIndexOfPlmnList].stPlmnId), &stPlmnIdWithRat.stPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
        pstDestPlmnList->ulPlmnNum++;
        ulIndexOfPlmnList++;

        if (ulIndexOfPlmnList >= NAS_MML_MAX_EHPLMN_NUM)
        {
            return;
        }
    }

    return;
}


VOS_VOID NAS_MMC_AddDplmnInDestPlmnList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstDestPlmn,
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstDestPlmnList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulIsSameCountryAsDestPlmn;
    NAS_MML_MCC_DESC_STRU                                  *pstEquMccList           = VOS_NULL_PTR;
    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU                      *pstDplmnNplmnCfgInfo    = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus;

    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stTempPlmn;

    PS_MEM_SET(&stPlmnWithRat, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ��ȡ��ЧMCC�б� */
    pstEquMccList                       = NAS_MML_GetEquivalentMccList(pstDestPlmn->stPlmnId.ulMcc);

    /* ѡ���б������Ǹ����ȼ��б����ɲ��������� */

    pstForbidPlmnInfo                   = NAS_MML_GetForbidPlmnInfo();
    PS_MEM_SET(&stTempPlmn, 0x00, sizeof(stTempPlmn));

    if ((0 == pstDestPlmnList->ulPlmnNum)
     || (VOS_NULL_PTR == pstEquMccList))
    {
        return;
    }

    pstDplmnNplmnCfgInfo  = NAS_MMC_GetDPlmnNPlmnCfgInfo();

    /* ��DPLMN List��ȡ������ͬ���ҵĵ�DPLMN��������뵽Ŀ���б��� */
    for (i = 0; i < pstDplmnNplmnCfgInfo->usDplmnListNum; i++)
    {
        /* ��ǰDPLMN��֧�ֵ�ǰ���뼼�� */
        if (VOS_FALSE == NAS_MMC_IsNetRatContainedInSimRat(pstDestPlmn->enRat, pstDplmnNplmnCfgInfo->astDPlmnList[i].stSimPlmnWithRat.usSimRat))
        {
            continue;
        }

        stPlmnWithRat.stPlmnId.ulMcc = pstDplmnNplmnCfgInfo->astDPlmnList[i].stSimPlmnWithRat.stPlmnId.ulMcc;
        stPlmnWithRat.stPlmnId.ulMnc = pstDplmnNplmnCfgInfo->astDPlmnList[i].stSimPlmnWithRat.stPlmnId.ulMnc;
        stPlmnWithRat.enRat = pstDestPlmn->enRat;

        ulIsSameCountryAsDestPlmn = NAS_MML_IsSpecMccInEquMccList(stPlmnWithRat.stPlmnId.ulMcc, pstEquMccList);
        enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&stPlmnWithRat, pstPlmnSelectionList);

        if ((VOS_TRUE == ulIsSameCountryAsDestPlmn)
         && (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED != enNetStatus))
        {
            /* ��Ҫ�����Dplmn�Ѿ����б��������� */
            if ( VOS_TRUE == NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList(&stPlmnWithRat, pstDestPlmnList->ulPlmnNum, pstDestPlmnList->astPlmnId) )
            {
                continue;
            }

            /* ��Ҫ�����Dplmn��forbid plmn�б��������� */
            if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(&(stPlmnWithRat.stPlmnId),
                                                  pstForbidPlmnInfo->ucForbPlmnNum,
                                                  pstForbidPlmnInfo->astForbPlmnIdList))
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_AddDplmnInDestPlmnList: dplmn in forbPlmnIdList");
                continue;
            }

            /* �жϸ�PLMN�Ƿ��ںڰ������� */
            if ( VOS_TRUE == NAS_MML_IsPlmnIdForbiddenByWhiteBlackList(&(stPlmnWithRat.stPlmnId)) )
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_AddDplmnInDestPlmnList: dplmn in WhiteBlackList");
                continue;
            }

            /* ��PLMN ID�Ƿ�������������PLMN �б��� */
            if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInLockPlmnList(&(stPlmnWithRat.stPlmnId)))
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_AddDplmnInDestPlmnList: dplmn in LockPlmnList");
                continue;
            }

            /* ����������ʱ */
            if (VOS_FALSE == NAS_MML_IsPlmnIdRoamingAllowed(&(stPlmnWithRat.stPlmnId)))
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_AddDplmnInDestPlmnList: dplmn RoamingAllowed is false");
                continue;
            }
            /* ��ǰplmn��MCC�ڽ�ֹLTE�������ε�MCC�б���,PLMN����������ע�� */
            if (VOS_TRUE == NAS_MMC_IsNeedDisableLteRoam(stPlmnWithRat.stPlmnId.ulMcc))
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_AddDplmnInDestPlmnList:  dplmn IsNeedDisableLteRoam is true");
                continue;
            }

            /* �жϵ�ǰPLMN�Ƿ��ڽ�ֹRAT��PLMN�б���ͬʱ��ǰ���뼼������ֹ,����Ϊ��ǰPLMNΪ��ֹ���� */
            if (VOS_TRUE == NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList(&stPlmnWithRat))
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_AddDplmnInDestPlmnList: NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList return true");
                continue;
            }

            /* �ж�PLMN��RAT�ڽ�ֹ���뼼���б��У�����Ϊ��ǰRAT��Ӧ����Ϊ��ֹ���� */
            if (VOS_TRUE == NAS_MML_IsRatInForbiddenList(stPlmnWithRat.enRat))
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_AddDplmnInDestPlmnList: NAS_MML_IsRatInForbiddenList return true");
                continue;
            }

            /* Ŀ���б����������˳��˺��� */
            if ( NAS_MML_MAX_EHPLMN_NUM <= pstDestPlmnList->ulPlmnNum )
            {
                return;
            }

            pstDestPlmnList->astPlmnId[pstDestPlmnList->ulPlmnNum].enRat = stPlmnWithRat.enRat;
            PS_MEM_CPY(&(pstDestPlmnList->astPlmnId[pstDestPlmnList->ulPlmnNum].stPlmnId), &stPlmnWithRat.stPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
            pstDestPlmnList->ulPlmnNum++;
        }
    }
}


VOS_VOID NAS_MMC_AddUOPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_SIM_USERPLMN_INFO_STRU                         *pstUserPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_OPERPLMN_INFO_STRU                         *pstOperPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_SELPLMN_INFO_STRU                          *pstSelPlmnList  = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    VOS_UINT16                                              usSimRat;

    /* ��ȡUOPLMN */
    pstUserPlmnList = NAS_MML_GetSimUserPlmnList();

    pstOperPlmnList = NAS_MML_GetSimOperPlmnList();

    /* UPLMN������Ϊ0����OPLMN������Ϊ0 */
    if ((pstUserPlmnList->ucUserPlmnNum > 0)
     || (pstOperPlmnList->usOperPlmnNum > 0))
    {
        for (i = 0; i< pstUserPlmnList->ucUserPlmnNum; i++)
        {

            /* UPLMN����Plmn��Ч����usSimRatΪ0ʱ��Ĭ�϶�֧�� */
            usSimRat = pstUserPlmnList->astUserPlmnInfo[i].usSimRat;

            if (0 == usSimRat)
            {
                usSimRat = NAS_MML_SIM_RAT_ALL_SUPPORT;
            }

            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_UPLMN,
                                               usSimRat,
                                               &(pstUserPlmnList->astUserPlmnInfo[i].stPlmnId),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }

        for (i = 0; i< pstOperPlmnList->usOperPlmnNum; i++)
        {
            /* OPLMN����Plmn��Ч����usSimRatΪ0ʱ��Ĭ�϶�֧�� */
            usSimRat = pstOperPlmnList->astOperPlmnInfo[i].usSimRat;

            if (0 == usSimRat)
            {
                usSimRat = NAS_MML_SIM_RAT_ALL_SUPPORT;
            }

            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_OPLMN,
                                               usSimRat,
                                               &(pstOperPlmnList->astOperPlmnInfo[i].stPlmnId),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }
    }
    else /* ��ʾ(U)SIM���в�����UPLMN��OPLMN��������PLMN Sel�ļ� */
    {
        pstSelPlmnList = NAS_MML_GetSimSelPlmnList();

        for (i = 0; i< pstSelPlmnList->ucSelPlmnNum; i++)
        {
            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_OPLMN,
                                               NAS_MML_SIM_RAT_ALL_SUPPORT,
                                               &(pstSelPlmnList->astPlmnId[i]),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }
    }

    return;
}


VOS_VOID NAS_MMC_AddOPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_SIM_USERPLMN_INFO_STRU                         *pstUserPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_OPERPLMN_INFO_STRU                         *pstOperPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_SELPLMN_INFO_STRU                          *pstSelPlmnList  = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    VOS_UINT16                                              usSimRat;

    /* ��ȡUOPLMN */
    pstUserPlmnList = NAS_MML_GetSimUserPlmnList();

    pstOperPlmnList = NAS_MML_GetSimOperPlmnList();

    /* UPLMN������Ϊ0����OPLMN������Ϊ0 */
    if ((pstUserPlmnList->ucUserPlmnNum > 0)
     || (pstOperPlmnList->usOperPlmnNum > 0))
    {
        for (i = 0; i< pstOperPlmnList->usOperPlmnNum; i++)
        {
            /* OPLMN����Plmn��Ч����usSimRatΪ0ʱ��Ĭ�϶�֧�� */
            usSimRat = pstOperPlmnList->astOperPlmnInfo[i].usSimRat;

            if (0 == usSimRat)
            {
                usSimRat = NAS_MML_SIM_RAT_ALL_SUPPORT;
            }

            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_OPLMN,
                                               usSimRat,
                                               &(pstOperPlmnList->astOperPlmnInfo[i].stPlmnId),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }
    }
    else /* ��ʾ(U)SIM���в�����UPLMN��OPLMN��������PLMN Sel�ļ� */
    {
        pstSelPlmnList = NAS_MML_GetSimSelPlmnList();

        for (i = 0; i< pstSelPlmnList->ucSelPlmnNum; i++)
        {
            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_OPLMN,
                                               NAS_MML_SIM_RAT_ALL_SUPPORT,
                                               &(pstSelPlmnList->astPlmnId[i]),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }
    }

    return;
}


VOS_VOID NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    NAS_MML_PLMN_ID_STRU                                    stPlmnId;

    /* ��ǰRplmn������ʱ��������Eplmn */
    if (VOS_FALSE == NAS_MML_GetPrefRPlmn(NAS_MML_GetLastRplmnRat(), &stPlmnId))
    {
        return;
    }

    pstEquPlmnList = NAS_MML_GetEquPlmnList();


    /* Rplmn�� EplmnList[0]����ͬʱ������Ҫ���� */
    if (VOS_FALSE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stPlmnId, &(pstEquPlmnList->astEquPlmnAddr[0])))
    {
        return;
    }

    for (i = 1; i < pstEquPlmnList->ucEquPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_EPLMN,
                                           NAS_MML_SIM_RAT_ALL_SUPPORT,
                                           &(pstEquPlmnList->astEquPlmnAddr[i]),
                                           &stPlmnInfo);

        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
    }
}


VOS_VOID NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;


    pstEquPlmnList = NAS_MML_GetEquPlmnList();


    for (i = 1; i < pstEquPlmnList->ucEquPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_EPLMN,
                                           NAS_MML_SIM_RAT_ALL_SUPPORT,
                                           &(pstEquPlmnList->astEquPlmnAddr[i]),
                                           &stPlmnInfo);

        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
    }
}


VOS_VOID NAS_MMC_AddRPlmnInPlmnSelectionList(
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    NAS_MML_PLMN_ID_STRU                                    stRPlmn;
    NAS_MML_RPLMN_CFG_INFO_STRU                            *pstRPlmnCfg  = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRPlmnRat;

    /* ��ȡRPLMN������Ϣ */
    pstRPlmnCfg = NAS_MML_GetRplmnCfg();

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        enRPlmnRat = pstPrioRatList->aucRatPrio[i];

        
        if ((NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
         || (VOS_FALSE == pstRPlmnCfg->ucMultiRATRplmnFlg))
        {
            enRPlmnRat = pstRPlmnCfg->enLastRplmnRat;
        }

        /* RPLMN��ȡʧ�� */
        if (VOS_FALSE == NAS_MMC_GetRPlmn_PlmnSelectionStrategy(enRPlmnRat,
                                                               &stRPlmn))
        {
            continue;
        }

        switch (pstPrioRatList->aucRatPrio[i])
        {
#if   (FEATURE_ON == FEATURE_LTE)
            case NAS_MML_NET_RAT_TYPE_LTE:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_E_UTRN,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;
#endif

            case NAS_MML_NET_RAT_TYPE_WCDMA:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_UTRN,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            case NAS_MML_NET_RAT_TYPE_GSM:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_GSM,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            default:
                NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_AddRPlmnInPlmnSelectionList: Rat not support.");
                break;
        }
    }

    return;
}


VOS_VOID NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;

    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ���ݽ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                       NAS_MML_SIM_RAT_ALL_SUPPORT,
                                       &(pstUserSpecPlmn->stPlmnId),
                                       &stPlmnInfo);

    NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);

    return;
}



VOS_VOID NAS_MMC_AddAisRoamPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_AIS_ROAMING_CFG_STRU           *pstRoamingCfg   = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU   stPlmnInfo;
    VOS_UINT16                              usSimRat;

    pstRoamingCfg  = NAS_MML_GetAisRoamingCfg();



#if (FEATURE_ON == FEATURE_LTE)
    if ( NAS_MML_NET_RAT_TYPE_LTE == pstRoamingCfg->enHighPrioRatType )
    {
        usSimRat = NAS_MML_SIM_RAT_E_UTRN;
    }
    else
#endif
    if ( NAS_MML_NET_RAT_TYPE_WCDMA == pstRoamingCfg->enHighPrioRatType )
    {
        usSimRat = NAS_MML_SIM_RAT_UTRN;
    }
    else if ( NAS_MML_NET_RAT_TYPE_GSM == pstRoamingCfg->enHighPrioRatType )
    {
        usSimRat = NAS_MML_SIM_RAT_GSM;
    }
    else
    {
        usSimRat = NAS_MML_SIM_NO_RAT;
    }

    /* ���ݽ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_HPLMN,
                                       usSimRat,
                                       &(pstRoamingCfg->stHighPrioPlmnId),
                                       &stPlmnInfo);

    NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);

}




VOS_VOID NAS_MMC_AddSpecPlmnInPlmnSelectionList(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmn,
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        switch (pstPrioRatList->aucRatPrio[i])
        {
#if   (FEATURE_ON == FEATURE_LTE)
            case NAS_MML_NET_RAT_TYPE_LTE:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_E_UTRN,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;
#endif

            case NAS_MML_NET_RAT_TYPE_WCDMA:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_UTRN,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            case NAS_MML_NET_RAT_TYPE_GSM:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_GSM,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            default:
                NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_AddSpecPlmnInPlmnSelectionList: Rat not support.");
                break;
        }
    }

    return;
}



VOS_VOID NAS_MMC_AppendPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmn,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT16                                              usSearchPlmnNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stTempPlmn;

    NAS_MML_PLMN_LOCK_CFG_INFO_STRU                        *pstPlmnLockCfg          = VOS_NULL_PTR;
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledRatPlmnCfg   = VOS_NULL_PTR;

    PS_MEM_SET(&stTempPlmn, 0, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    /* ��ȡ��ֹ���뼼����PLMN��Ϣ */
    pstDisabledRatPlmnCfg = NAS_MML_GetDisabledRatPlmnInfo();

    /* ���ڻ�ȡ�ڰ����� */
    pstPlmnLockCfg = NAS_MML_GetPlmnLockCfg();

    /* ��������������pstPlmn����ʱ�ֲ�������ȥ����֤pstPlmn���ݲ��ᱻ��ǰ�����޸� */
    PS_MEM_CPY(&stTempPlmn, pstPlmn, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU) );

    /* ��PLMN��Ч */
    if (VOS_FALSE == NAS_MML_IsPlmnIdValid(&(stTempPlmn.stPlmnId)))
    {
        return;
    }

    /* �жϵ�ǰ�������Ƿ��ں������б��У�������ֱ�ӷ���VOS_FALSE */
    if ( VOS_TRUE == NAS_MMC_IsSpecTypePlmnIdInDestPlmnList(&(stTempPlmn.stPlmnId),
                                           stTempPlmn.enPlmnType,
                                           pstPlmnLockCfg->ucBlackPlmnLockNum,
                                           pstPlmnLockCfg->astBlackPlmnId) )     /* ��PLMN�ں������б��� */
    {
        return;
    }

    if (pstPlmnLockCfg->ucWhitePlmnLockNum != 0)
    {
        /* �жϵ�ǰ�������Ƿ��ڰ������б��У���������ֱ�ӷ���VOS_FALSE */
        if (VOS_FALSE == NAS_MMC_IsSpecTypePlmnIdInDestPlmnList(&(stTempPlmn.stPlmnId),
                                               stTempPlmn.enPlmnType,
                                               pstPlmnLockCfg->ucWhitePlmnLockNum,
                                               pstPlmnLockCfg->astWhitePlmnId))     /* ��PLMN���ڰ������б��� */
        {
            return;
        }
    }

    if (VOS_FALSE == NAS_MML_IsPlmnIdRoamingAllowed(&(stTempPlmn.stPlmnId))) /* ��PLMN�������������б��� */
    {
        return;
    }

    /* ɾ���ڽ�ֹ���뼼��������Ľ��뼼�� */
    NAS_MMC_DelPlmnSelectionRatInfoInDisabledPlmnWithForbiddenPeriodList(&stTempPlmn,
                                              pstDisabledRatPlmnCfg->ulDisabledRatPlmnNum,
                                              pstDisabledRatPlmnCfg->astDisabledRatPlmnId);

    /* ɾ���ڽ�ֹ���뼼���б��еĽ��뼼�� */
    NAS_MMC_DeleteForbiddenRatInPlmnSeleInfo(&stTempPlmn);

    /* ����������Ľ��뼼������Ϊ�㣬������ */
    if (0 == stTempPlmn.ucRatNum)
    {
        return;
    }

    usSearchPlmnNum = pstPlmnSelectionListInfo->usSearchPlmnNum;

    if (usSearchPlmnNum >= NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_AppendPlmnInPlmnSelectionList: list full.");

        return;
    }

    /*����PLMN�洢���б�β��,PLMN������1*/
    PS_MEM_CPY(&(pstPlmnSelectionListInfo->astPlmnSelectionList[usSearchPlmnNum]),
               &stTempPlmn, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    pstPlmnSelectionListInfo->usSearchPlmnNum++;

    return;
}


VOS_VOID NAS_MMC_AppendPlmnInRoamPlmnSelectionList(
    NAS_MMC_ROAM_PLMN_INFO_STRU                            *pstPlmn,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstSearchedPlmnListInfo
)
{
    VOS_UINT16                                              usSearchPlmnNum;
    NAS_MMC_ROAM_PLMN_INFO_STRU                             stTempPlmn;
    NAS_MML_LAI_STRU                                        stLai;
    VOS_UINT8                                               ucAllBandSearch;
    VOS_UINT8                                               ucPlmnSortedFlag;
    VOS_UINT32                                              i;

    PS_MEM_SET(&stTempPlmn, 0, sizeof(stTempPlmn));
    PS_MEM_SET(&stLai, 0, sizeof(stLai));
    i               = 0;
    usSearchPlmnNum = 0;

    ucAllBandSearch = VOS_FALSE;
    ucPlmnSortedFlag = VOS_FALSE;

    /* ��������������pstPlmn����ʱ�ֲ�������ȥ����֤pstPlmn���ݲ��ᱻ��ǰ�����޸� */
    PS_MEM_CPY(&stTempPlmn, pstPlmn, sizeof(NAS_MMC_ROAM_PLMN_INFO_STRU) );

    /* plmn��Ч������roam�����б� */
    if (VOS_FALSE == NAS_MML_IsPlmnIdValid(&stTempPlmn.stPlmnWithRat.stPlmnId))
    {
        return;
    }

    if (NAS_MMC_FSM_PLMN_SELECTION == NAS_MMC_GetCurrFsmId())
    {
        ucAllBandSearch = NAS_MMC_GetAllBandSearch_PlmnSelection(stTempPlmn.stPlmnWithRat.enRat);
        ucPlmnSortedFlag = NAS_MMC_GetSearchedRoamPlmnSortedFlag_PlmnSelection(stTempPlmn.stPlmnWithRat.enRat);
    }

    /* ����ý��뼼��֮ǰ�Ѿ��յ���searched plmn info ind���Ź�һ����,���Ѿ����й�һ��ȫƵ��������
       �����Ӹý��뼼��plmn��roam�����б� */
    if (VOS_TRUE == ucPlmnSortedFlag)
    {
        return;
    }

    /* ���ĳ��ģ�Ǵ�ģ�����Ҹ�ģ���Ѿ����й�һ��ȫƵ����������������ʱֻ�ŵ�ǰ��ģ�����������ģ�������� */
    if ((pstPlmn->stPlmnWithRat.enRat != NAS_MML_GetCurrNetRatType())
     && (VOS_TRUE == ucAllBandSearch))
    {
        return;
    }

    usSearchPlmnNum = pstSearchedPlmnListInfo->usSearchPlmnNum;

    if (usSearchPlmnNum >= NAS_MMC_MAX_ROAM_PLMN_NUM_IN_SELECTION_LIST)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_AppendPlmnInRoamPlmnSelectionList: list full.");

        return;
    }

    /* ����Ѿ���roam�����б��У����������� */
    for (i = 0; i < pstSearchedPlmnListInfo->usSearchPlmnNum; i++)
    {
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stTempPlmn.stPlmnWithRat.stPlmnId, &pstSearchedPlmnListInfo->astPlmnSelectionList[i].stPlmnWithRat.stPlmnId))
         && (stTempPlmn.stPlmnWithRat.enRat == pstSearchedPlmnListInfo->astPlmnSelectionList[i].stPlmnWithRat.enRat))
        {
            return;
        }
    }

    stLai.stPlmnId         = stTempPlmn.stPlmnWithRat.stPlmnId;
    stLai.enCampPlmnNetRat = stTempPlmn.stPlmnWithRat.enRat;
    stLai.aucLac[0]        = stTempPlmn.aucLac[0];
    stLai.aucLac[1]        = stTempPlmn.aucLac[1];
    stLai.ucRac            = NAS_MML_RAC_INVALID;

    /* ��ȡ�����ֹ���ͣ�����ǽ�ֹ�����򲻼���roam�����б� */
    if (NAS_MML_PLMN_FORBID_NULL != NAS_MML_GetLaiForbType(&stLai))
    {
        return;
    }

    /*����PLMN�洢���б�β��,PLMN������1*/
    PS_MEM_CPY(&(pstSearchedPlmnListInfo->astPlmnSelectionList[usSearchPlmnNum]),
               &stTempPlmn, sizeof(NAS_MMC_ROAM_PLMN_INFO_STRU));

    pstSearchedPlmnListInfo->usSearchPlmnNum++;

    return;
}


VOS_UINT32 NAS_MMC_InsertPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmn,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    VOS_UINT32                                              ulPos
)
{
    VOS_UINT16                          usSearchPlmnNum;

    usSearchPlmnNum = pstPlmnSelectionListInfo->usSearchPlmnNum;

    if ((usSearchPlmnNum >= NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST)
     || (ulPos >= NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_InsertPlmnInPlmnSelectionList: list full.");

        return VOS_FALSE;
    }

    if ((NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST - 1) != ulPos)
    {
        /* ����ulPosλ�ÿճ� */
        /*lint -e961*/
        (VOS_VOID)PS_MEM_MOVE(&(pstPlmnSelectionListInfo->astPlmnSelectionList[ulPos+1]),
                    &(pstPlmnSelectionListInfo->astPlmnSelectionList[ulPos]),
                    (NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST-ulPos-1)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
        /*lint +e961*/
    }

    /* ����PLMN���뵽�����б���ulPosλ�� */
    PS_MEM_CPY(&(pstPlmnSelectionListInfo->astPlmnSelectionList[ulPos]),
               pstPlmn, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    pstPlmnSelectionListInfo->usSearchPlmnNum++;

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_InsertPlmnToPlmnSelectionListHead(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmn,
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulRet;
    VOS_UINT32                                              ulPos;

    ulRet                               = VOS_FALSE;
    ulPos                               = 0;

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        switch (pstPrioRatList->aucRatPrio[i])
        {
#if   (FEATURE_ON == FEATURE_LTE)
            case NAS_MML_NET_RAT_TYPE_LTE:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_E_UTRN,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                ulRet = NAS_MMC_InsertPlmnInPlmnSelectionList(&stPlmnInfo,
                                                              pstPlmnSelectionListInfo,
                                                              ulPos);
                break;
#endif

            case NAS_MML_NET_RAT_TYPE_WCDMA:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_UTRN,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                ulRet = NAS_MMC_InsertPlmnInPlmnSelectionList(&stPlmnInfo,
                                                              pstPlmnSelectionListInfo,
                                                              ulPos);
                break;

            case NAS_MML_NET_RAT_TYPE_GSM:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_GSM,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                ulRet = NAS_MMC_InsertPlmnInPlmnSelectionList(&stPlmnInfo,
                                                              pstPlmnSelectionListInfo,
                                                              ulPos);
                break;

            default:
                NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_InsertSpecPlmnInPlmnSelectionList: Rat not support.");
                break;
        }

        if (VOS_TRUE == ulRet)
        {
            ulPos++;
            ulRet = VOS_FALSE;
        }
    }

    return;
}


VOS_VOID NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRat,
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstDstPlmn
)
{
    VOS_UINT32                             i;

    /* ��ԭPLMN��ÿһ��֧�ֵĽ��뼼����Ŀ��PLMN��ÿһ��֧�ֵĽ��뼼�����бȽ� */
    for (i = 0; i < pstDstPlmn->ucRatNum; i++)
    {
        /* �����ͬ�������Ŀ��PLMN��Ϣ�е�״̬��Ϣ */
        if (pstDstPlmn->astPlmnRatInfo[i].enRatType == enRat)
        {
            pstDstPlmn->astPlmnRatInfo[i].enNetStatus = enNetStatus;

            return;
        }
    }

    return;
}


NAS_MMC_PLMN_TYPE_ENUM_UINT8 NAS_MMC_GetRoamPlmnTypeInPlmnSelectionList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType;

    enPlmnType = NAS_MMC_GetPlmnTypeInPlmnSelectionList(pstPlmnWithRat, pstPlmnSelectionListInfo);

    if (NAS_MMC_PLMN_TYPE_BUTT == enPlmnType)
    {
        enPlmnType = NAS_MMC_PLMN_TYPE_AVAIL_PLMN;
    }

    /* ���������б���plmn���ظ����������п��ܼ���rplmn����uplmn����Ҫ���ж������Ƿ���UPLMN */
    if (VOS_TRUE == NAS_MMC_ComparePlmnIdWithRatWithUplmn(pstPlmnWithRat))
    {
        enPlmnType = NAS_MMC_PLMN_TYPE_UPLMN;
        return enPlmnType;
    }

    /* ���������б���plmn���ظ����������п��ܼ���rplmn����oplmn����Ҫ���ж������Ƿ���OPLMN */
    if (VOS_TRUE == NAS_MMC_ComparePlmnIdWithRatWithOplmn(pstPlmnWithRat))
    {
        enPlmnType = NAS_MMC_PLMN_TYPE_OPLMN;
        return enPlmnType;
    }

    return enPlmnType;

}


NAS_MMC_NET_STATUS_ENUM_UINT8 NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;
    NAS_MMC_NET_STATUS_ENUM_UINT8                           aucNetStatus[NAS_MML_MAX_RAT_NUM];
    VOS_UINT8                                               i;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(stPrioRatList));

    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        aucNetStatus[i] = NAS_MMC_NET_STATUS_BUTT;
    }

    stPrioRatList.ucRatNum      = 1;
    stPrioRatList.aucRatPrio[0] = pstPlmnWithRat->enRat;
    NAS_MMC_GetSpecPlmnNetStatusInPlmnSelectionList(&pstPlmnWithRat->stPlmnId,
                                                    &stPrioRatList,
                                                    aucNetStatus,
                                                    pstPlmnSelectionListInfo);

    return  aucNetStatus[0];
}


VOS_VOID NAS_MMC_AddSearchedPlmnInRoamPlmnSelectionList(
    NAS_MMC_ROAM_PLMN_INFO_STRU                            *pstRoamPlmnInfo,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstNewRoamPlmnSelectionList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSrchList
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU                      *pstDPlmnNPlmnCfgInfo = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    NAS_MMC_ROAM_PLMN_INFO_STRU                             stRoamPlmnInfo;
    VOS_UINT16                                              usSimRat;

    /* ��ȡѡ���б� */
    enPlmnType      = NAS_MMC_PLMN_TYPE_BUTT;
    i               = 0;

    PS_MEM_SET(&stPlmnInfo, 0, sizeof(stPlmnInfo));
    PS_MEM_SET(&stRoamPlmnInfo, 0, sizeof(stRoamPlmnInfo));
    PS_MEM_SET(&stPrioRatList, 0, sizeof(stPrioRatList));

    /* ��ȡ�洢DPLMN��NPLMN��ָ�� */
    pstDPlmnNPlmnCfgInfo = NAS_MMC_GetDPlmnNPlmnCfgInfo();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ѡ���б������ӽ���㱨���������磬��ǰ�������뼼������ */
    NAS_MML_SortSpecRatPrioHighest(pstRoamPlmnInfo->stPlmnWithRat.enRat, &stPrioRatList);


    /* ��������������NPLMN�У������ӵ�roam�����б���,��������б���û������������б������ */
    if (VOS_TRUE == NAS_MMC_IsPlmnIdWithRatInSimPlmnWithRegDomainList(&pstRoamPlmnInfo->stPlmnWithRat,
                    pstDPlmnNPlmnCfgInfo->usNplmnListNum, pstDPlmnNPlmnCfgInfo->astNPlmnList))
    {
        if (VOS_FALSE == NAS_MMC_IsPlmnWithRatInPlmnList(&pstRoamPlmnInfo->stPlmnWithRat.stPlmnId,
                                                         pstRoamPlmnInfo->stPlmnWithRat.enRat,
                                                         pstPlmnSrchList,
                                                         pstPlmnSrchList->usSearchPlmnNum))
        {
            usSimRat = NAS_MMC_ConvertNetRatToSimRat(pstRoamPlmnInfo->stPlmnWithRat.enRat);

            NAS_MMC_BuildPlmnSelectionPlmnInfo(pstRoamPlmnInfo->enPlmnType,
                                               usSimRat,
                                               &(pstRoamPlmnInfo->stPlmnWithRat.stPlmnId),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSrchList);
        }
    }
    else
    {
        NAS_MMC_AppendPlmnInRoamPlmnSelectionList(pstRoamPlmnInfo, pstNewRoamPlmnSelectionList);
    }

    /* �����������뼼��plmn��roam�����б� */
    for (i = 1; i < stPrioRatList.ucRatNum; i++)
    {
        stRoamPlmnInfo.stPlmnWithRat.stPlmnId.ulMcc = pstRoamPlmnInfo->stPlmnWithRat.stPlmnId.ulMcc;
        stRoamPlmnInfo.stPlmnWithRat.stPlmnId.ulMnc = pstRoamPlmnInfo->stPlmnWithRat.stPlmnId.ulMnc;
        stRoamPlmnInfo.stPlmnWithRat.enRat = stPrioRatList.aucRatPrio[i];

        enPlmnType                 = NAS_MMC_GetRoamPlmnTypeInPlmnSelectionList(&stRoamPlmnInfo.stPlmnWithRat, pstPlmnSrchList);
        stRoamPlmnInfo.enPlmnType  = enPlmnType;
        stRoamPlmnInfo.enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&stRoamPlmnInfo.stPlmnWithRat, pstPlmnSrchList);

        if (NAS_MMC_NET_STATUS_BUTT == stRoamPlmnInfo.enNetStatus)
        {
            stRoamPlmnInfo.enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        }

        /* �����������NPLMN�������Ӹ����絽roam�����б�,��������б��в����ڣ�����������б������ */
        if (VOS_TRUE == NAS_MMC_IsPlmnIdWithRatInSimPlmnWithRegDomainList(&stRoamPlmnInfo.stPlmnWithRat,
                        pstDPlmnNPlmnCfgInfo->usNplmnListNum, pstDPlmnNPlmnCfgInfo->astNPlmnList))
        {
            if (VOS_FALSE == NAS_MMC_IsPlmnWithRatInPlmnList(&stRoamPlmnInfo.stPlmnWithRat.stPlmnId,
                                                         stRoamPlmnInfo.stPlmnWithRat.enRat,
                                                         pstPlmnSrchList,
                                                         pstPlmnSrchList->usSearchPlmnNum))
            {
                usSimRat = NAS_MMC_ConvertNetRatToSimRat(stRoamPlmnInfo.stPlmnWithRat.enRat);

                NAS_MMC_BuildPlmnSelectionPlmnInfo(stRoamPlmnInfo.enPlmnType,
                                                   usSimRat,
                                                   &(stRoamPlmnInfo.stPlmnWithRat.stPlmnId),
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSrchList);
            }

            continue;
        }

        NAS_MMC_AppendPlmnInRoamPlmnSelectionList(&stRoamPlmnInfo, pstNewRoamPlmnSelectionList);

    }

    return;
}


NAS_MMC_ROAM_PLMN_TYPE_ENUM_UINT8 NAS_MMC_GetRoamPlmnType(
    NAS_MMC_ROAM_PLMN_INFO_STRU        *pstPlmnInfo
)
{
    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU                      *pstDPlmnNPlmnCfgInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;
    NAS_MML_LAI_STRU                                        stLai;

    PS_MEM_SET(&stPlmnWithRat, 0, sizeof(stPlmnWithRat));
    PS_MEM_SET(&stLai, 0, sizeof(stLai));

    /* ��ȡ�洢DPLMN��NPLMN��ָ�� */
    pstDPlmnNPlmnCfgInfo   = NAS_MMC_GetDPlmnNPlmnCfgInfo();

    stLai.stPlmnId         = pstPlmnInfo->stPlmnWithRat.stPlmnId;
    stLai.enCampPlmnNetRat = pstPlmnInfo->stPlmnWithRat.enRat;
    stLai.aucLac[0]        = pstPlmnInfo->aucLac[0];
    stLai.aucLac[1]        = pstPlmnInfo->aucLac[1];
    stLai.ucRac            = NAS_MML_RAC_INVALID;

    /* �ж������Ƿ�Ϊ��ֹ���� */
    if (NAS_MML_PLMN_FORBID_NULL != NAS_MML_GetLaiForbType(&stLai))
    {
        return NAS_MMC_ROAM_PLMN_TYPE_FORBIDDEN;
    }

    /* �ж������Ƿ���NPLMN�б��� */
    if (VOS_TRUE == NAS_MMC_IsPlmnIdWithRatInSimPlmnWithRegDomainList(&pstPlmnInfo->stPlmnWithRat,
                      pstDPlmnNPlmnCfgInfo->usNplmnListNum, pstDPlmnNPlmnCfgInfo->astNPlmnList))
    {
        return NAS_MMC_ROAM_PLMN_TYPE_NPLMN;
    }

    if (VOS_TRUE == NAS_MMC_GetSrchUOplmnPriorToDplmnFlg())
    {
        /* �ж������Ƿ���UPLMN */
        if (VOS_TRUE == NAS_MMC_ComparePlmnIdWithRatWithUplmn(&pstPlmnInfo->stPlmnWithRat))
        {
            return NAS_MMC_ROAM_PLMN_TYPE_UPLMN;
        }

        /* �ж������Ƿ���OPLMN */
        if (VOS_TRUE == NAS_MMC_ComparePlmnIdWithRatWithOplmn(&pstPlmnInfo->stPlmnWithRat))
        {
            return NAS_MMC_ROAM_PLMN_TYPE_OPLMN;
        }

        /* �ж������Ƿ���DPLMN�� */
        if (VOS_TRUE == NAS_MMC_IsPlmnIdWithRatInSimPlmnWithRegDomainList(&pstPlmnInfo->stPlmnWithRat,
                               pstDPlmnNPlmnCfgInfo->usDplmnListNum, pstDPlmnNPlmnCfgInfo->astDPlmnList))
        {
            return NAS_MMC_ROAM_PLMN_TYPE_DPLMN;
        }
    }
    else
    {
        /* �ж������Ƿ���DPLMN�� */
        if (VOS_TRUE == NAS_MMC_IsPlmnIdWithRatInSimPlmnWithRegDomainList(&pstPlmnInfo->stPlmnWithRat,
                               pstDPlmnNPlmnCfgInfo->usDplmnListNum, pstDPlmnNPlmnCfgInfo->astDPlmnList))
        {
            return NAS_MMC_ROAM_PLMN_TYPE_DPLMN;
        }

        /* �ж������Ƿ���UPLMN */
        if (VOS_TRUE == NAS_MMC_ComparePlmnIdWithRatWithUplmn(&pstPlmnInfo->stPlmnWithRat))
        {
            return NAS_MMC_ROAM_PLMN_TYPE_UPLMN;
        }

        /* �ж������Ƿ���OPLMN */
        if (VOS_TRUE == NAS_MMC_ComparePlmnIdWithRatWithOplmn(&pstPlmnInfo->stPlmnWithRat))
        {
            return NAS_MMC_ROAM_PLMN_TYPE_OPLMN;
        }
    }

    return NAS_MMC_ROAM_PLMN_TYPE_AVAIL_PLMN;
}

VOS_UINT8 NAS_MMC_GetRoamPlmnPrio(
    NAS_MMC_ROAM_PLMN_INFO_STRU        *pstRoamPlmnInfo
)
{
    NAS_MMC_ROAM_PLMN_TYPE_ENUM_UINT8             enRoamPlmnType;
    NAS_MMC_ROAM_PLMN_PRIO_ENUM_UINT8             enRoamPlmnPrio;

    enRoamPlmnType      = NAS_MMC_GetRoamPlmnType(pstRoamPlmnInfo);

    if (VOS_FALSE == NAS_MMC_GetSrchUOplmnPriorToDplmnFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_GetRoamPlmnPrio: UOplmnPriorToDplmnFlg is false");
        return enRoamPlmnType;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_GetRoamPlmnPrio: UOplmnPriorToDplmnFlg is true");

    switch(enRoamPlmnType)
    {
        case NAS_MMC_ROAM_PLMN_TYPE_UPLMN:
            enRoamPlmnPrio = NAS_MMC_ROAM_PLMN_PRIO_UPLMN;
            break;

        case NAS_MMC_ROAM_PLMN_TYPE_OPLMN:
            enRoamPlmnPrio = NAS_MMC_ROAM_PLMN_PRIO_OPLMN;
            break;

        case NAS_MMC_ROAM_PLMN_TYPE_DPLMN:
            enRoamPlmnPrio = NAS_MMC_ROAM_PLMN_PRIO_DPLMN;
            break;

        case NAS_MMC_ROAM_PLMN_TYPE_AVAIL_PLMN:
            enRoamPlmnPrio = NAS_MMC_ROAM_PLMN_PRIO_AVAIL_PLMN;
            break;

        case NAS_MMC_ROAM_PLMN_TYPE_NPLMN:
            enRoamPlmnPrio = NAS_MMC_ROAM_PLMN_PRIO_NPLMN;
            break;

        case NAS_MMC_ROAM_PLMN_TYPE_FORBIDDEN:
            enRoamPlmnPrio = NAS_MMC_ROAM_PLMN_PRIO_FORBIDDEN;
            break;

        default:
            enRoamPlmnPrio = NAS_MMC_ROAM_PLMN_PRIO_FORBIDDEN;
            break;
    }

    return enRoamPlmnPrio;
}

VOS_INT32  NAS_MMC_CompareRoamPlmnPrio(
    const VOS_VOID                  *pPlmn1Info,
    const VOS_VOID                  *pPlmn2Info,
    NAS_MML_PLMN_RAT_PRIO_STRU      *pstPrioRatList
)
{
    NAS_MMC_ROAM_PLMN_INFO_STRU                            *pstFirstPlmnInfo;
    NAS_MMC_ROAM_PLMN_INFO_STRU                            *pstSecondPlmnInfo;
    VOS_UINT8                                               ucFirstRoamPlmnPrio;
    VOS_UINT8                                               ucSecondRoamPlmnPrio;
    VOS_UINT8                                               i;
    VOS_UINT8                                               ucFirstPlmnRatIndex;
    VOS_UINT8                                               ucSecondPlmnRatIndex;

    pstFirstPlmnInfo         = (NAS_MMC_ROAM_PLMN_INFO_STRU *)pPlmn1Info;
    pstSecondPlmnInfo        = (NAS_MMC_ROAM_PLMN_INFO_STRU *)pPlmn2Info;
    ucFirstRoamPlmnPrio      = NAS_MMC_GetRoamPlmnPrio(pstFirstPlmnInfo);
    ucSecondRoamPlmnPrio     = NAS_MMC_GetRoamPlmnPrio(pstSecondPlmnInfo);
    ucFirstPlmnRatIndex      = 0;
    ucSecondPlmnRatIndex     = 0;


    /* ��ӡ���ȼ� */
    NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MMC_CompareRoamPlmnPrio: the Prios of roam plmns are:", ucFirstRoamPlmnPrio, ucSecondRoamPlmnPrio);

    /* �Ƚ�����plmn�����ȼ���plmn typeԽ�����ȼ�Խ�� */
    if (ucFirstRoamPlmnPrio > ucSecondRoamPlmnPrio)
    {
        return 1;
    }

    if (ucFirstRoamPlmnPrio < ucSecondRoamPlmnPrio)
    {
        return -1;
    }

    /* ��ӡ����״̬ */
    NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MMC_CompareRoamPlmnPrio: the NetStatuses of roam plmns are:", pstFirstPlmnInfo->enNetStatus, pstSecondPlmnInfo->enNetStatus);

    /* �������plmn���ȼ���ͬ���ٱȽ�����״̬�����ڵ��������ȼ���,
       ��������״̬����Ϊ���ȼ��Ƚ����� */
    if ((NAS_MMC_NET_STATUS_SEARCHED_EXIST == pstFirstPlmnInfo->enNetStatus)
     && (NAS_MMC_NET_STATUS_SEARCHED_EXIST == pstSecondPlmnInfo->enNetStatus))
    {
        return 0;
    }

    if (NAS_MMC_NET_STATUS_SEARCHED_EXIST == pstFirstPlmnInfo->enNetStatus)
    {
        return 1;
    }

    if (NAS_MMC_NET_STATUS_SEARCHED_EXIST == pstSecondPlmnInfo->enNetStatus)
    {
        return -1;
    }

    /* ��һ��plmn�͵ڶ���plmn��ͬʱ�ſ��ǽ��뼼�����ȼ�*/
    if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstFirstPlmnInfo->stPlmnWithRat.stPlmnId, &pstSecondPlmnInfo->stPlmnWithRat.stPlmnId))
    {
        for (i = 0; i < pstPrioRatList->ucRatNum; i++)
        {
            if (pstFirstPlmnInfo->stPlmnWithRat.enRat == pstPrioRatList->aucRatPrio[i])
            {
                ucFirstPlmnRatIndex = i;
            }

            if (pstSecondPlmnInfo->stPlmnWithRat.enRat == pstPrioRatList->aucRatPrio[i])
            {
                ucSecondPlmnRatIndex = i;
            }
        }

        /* ��ӡ���뼼�����ȼ� */
        NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MMC_CompareRoamPlmnPrio: the RatIndexes of roam plmns are:", ucFirstPlmnRatIndex, ucSecondPlmnRatIndex);

        /* ��һ��plmn���뼼�����ȣ��������ȼ��� */
        if (ucFirstPlmnRatIndex < ucSecondPlmnRatIndex)
        {
            return 1;
        }

        if (ucSecondPlmnRatIndex < ucFirstPlmnRatIndex)
        {
            return -1;
        }
    }

    return 0;
}


VOS_VOID NAS_MMC_AddRoamPlmnSelectionListInPlmnSelectionList(
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstNewRoamPlmnSelectionList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSrchList
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stInsertPlmnInfo;
    VOS_UINT16                                              usSimRat;
    VOS_INT32                                               i;

    PS_MEM_SET(&stInsertPlmnInfo, 0, sizeof(stInsertPlmnInfo));
    i = 0;

    if (0 == pstNewRoamPlmnSelectionList->usSearchPlmnNum)
    {
        return;
    }

    /* ������˳��ӵ͵��߲��������б�ͷ�� */
    for (i = pstNewRoamPlmnSelectionList->usSearchPlmnNum - 1; i >= 0 ; i--)
    {
        usSimRat = NAS_MMC_ConvertNetRatToSimRat(pstNewRoamPlmnSelectionList->astPlmnSelectionList[i].stPlmnWithRat.enRat);

        NAS_MMC_BuildPlmnSelectionPlmnInfo(
              pstNewRoamPlmnSelectionList->astPlmnSelectionList[i].enPlmnType,
              usSimRat,
              &pstNewRoamPlmnSelectionList->astPlmnSelectionList[i].stPlmnWithRat.stPlmnId,
              &stInsertPlmnInfo);

        (VOS_VOID)NAS_MMC_InsertPlmnInPlmnSelectionList(&stInsertPlmnInfo,
                 pstPlmnSrchList, 0);

        /* ���roam�����б�������״̬�ǲ�Ϊno searched����Ҫ�������������б�������״̬���������б���Ӧ����״̬��
           Ĭ��NAS_MMC_BuildPlmnSelectionPlmnInfo����������״̬��δ����״̬ */
        if (NAS_MMC_NET_STATUS_NO_SEARCHED != pstNewRoamPlmnSelectionList->astPlmnSelectionList[i].enNetStatus)
        {
            NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&pstNewRoamPlmnSelectionList->astPlmnSelectionList[i].stPlmnWithRat,
                                               pstNewRoamPlmnSelectionList->astPlmnSelectionList[i].enNetStatus,
                                               pstPlmnSrchList);
        }
    }

    return;
}

VOS_VOID NAS_MMC_BuildSearchedPlmnListInfoByRrcSearchCnfFail(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRat,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnIdList,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstSearchedExistPlmnInfo
)
{
    VOS_UINT32                                             i;
    VOS_UINT32                                             ulPlmnNum;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                           enPlmnType;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                 *pstPlmnSrchList   = VOS_NULL_PTR;

     /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �������������*/
    for (i = 0; i < pstPlmnIdList->ulHighPlmnNum; i++)
    {
        ulPlmnNum = pstSearchedExistPlmnInfo->usSearchPlmnNum;

        if (ulPlmnNum >= NAS_MMC_MAX_ROAM_PLMN_NUM_IN_SELECTION_LIST)
        {
            return;
        }

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.stPlmnId = pstPlmnIdList->astHighPlmnList[i];
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.enRat    = enRat;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].aucLac[0]              = NAS_MML_LAC_LOW_BYTE_INVALID;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].aucLac[1]              = NAS_MML_LAC_HIGH_BYTE_INVALID;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus            = NAS_MMC_NET_STATUS_SEARCHED_EXIST;

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus  =  NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat, pstPlmnSrchList);

        if (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED != pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus)
        {
            pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
        }

        enPlmnType = NAS_MMC_GetRoamPlmnTypeInPlmnSelectionList(&pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat, pstPlmnSrchList);

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enPlmnType  = enPlmnType;
        pstSearchedExistPlmnInfo->usSearchPlmnNum++;
    }

    /* ����ѵ���������*/
    for (i = 0; i < pstPlmnIdList->ulLowPlmnNum; i++)
    {
        ulPlmnNum = pstSearchedExistPlmnInfo->usSearchPlmnNum;

        if (ulPlmnNum >= NAS_MMC_MAX_ROAM_PLMN_NUM_IN_SELECTION_LIST)
        {
            return;
        }

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.stPlmnId = pstPlmnIdList->astLowPlmnList[i].stPlmnId;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.enRat    = enRat;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].aucLac[0]              = NAS_MML_LAC_LOW_BYTE_INVALID;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].aucLac[1]              = NAS_MML_LAC_HIGH_BYTE_INVALID;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus            = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
        enPlmnType = NAS_MMC_GetRoamPlmnTypeInPlmnSelectionList(&pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat, pstPlmnSrchList);

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enPlmnType  = enPlmnType;
        pstSearchedExistPlmnInfo->usSearchPlmnNum++;
    }

    return;
}


VOS_VOID NAS_MMC_BuildSearchedPlmnInfoByLteSearchedPlmnInfoInd(
    LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU                    *pstLteSearchedPlmnInfo,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstSearchedExistPlmnInfo
)
{
    VOS_UINT32                                             i;
    VOS_UINT32                                             ulPlmnNum;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                           enPlmnType;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                 *pstPlmnSrchList   = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   stGuNasPlmn;

    /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();
    ulPlmnNum       = 0;
    i               = 0;
    enPlmnType      = NAS_MMC_PLMN_TYPE_BUTT;
    PS_MEM_SET(&stGuNasPlmn, 0, sizeof(stGuNasPlmn));


    /*  �����ڵ��������roam�����б� */
    for (i = 0; i < pstLteSearchedPlmnInfo->ulTaiNum; i++)
    {
        NAS_MMC_ConvertLmmPlmnToGUNasFormat(&pstLteSearchedPlmnInfo->stTaiList[i].stPlmnId,
                                            &stGuNasPlmn);

        ulPlmnNum = pstSearchedExistPlmnInfo->usSearchPlmnNum;

        if (ulPlmnNum >= NAS_MMC_MAX_ROAM_PLMN_NUM_IN_SELECTION_LIST)
        {
            return;
        }

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.stPlmnId = stGuNasPlmn;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].aucLac[0]              = pstLteSearchedPlmnInfo->stTaiList[i].stTac.ucTac;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].aucLac[1]              = pstLteSearchedPlmnInfo->stTaiList[i].stTac.ucTacCnt;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus  =  NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat, pstPlmnSrchList);

        if (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED != pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus)
        {
            pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
        }

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.enRat    = NAS_MML_NET_RAT_TYPE_LTE;

        enPlmnType = NAS_MMC_GetRoamPlmnTypeInPlmnSelectionList(&pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat, pstPlmnSrchList);

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enPlmnType  = enPlmnType;
        pstSearchedExistPlmnInfo->usSearchPlmnNum++;
    }

    return;
}


VOS_VOID NAS_MMC_BuildSearchedPlmnInfoByRrcSearchedPlmnInfoInd(
    RRMM_SEARCHED_PLMN_INFO_IND_STRU   *pstSearchedPlmnInfoMsg,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU   *pstSearchedExistPlmnInfo
)
{
    VOS_UINT32                                             i;
    VOS_UINT32                                             ulPlmnNum;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                           enPlmnType;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                 *pstPlmnSrchList   = VOS_NULL_PTR;
    RRC_NAS_RAT_TYPE_ENUM_UINT32                           enRrcRatType;
    NAS_MMC_NET_STATUS_ENUM_UINT8                          enNetStatus;

    enPlmnType   = NAS_MMC_PLMN_TYPE_BUTT;
    ulPlmnNum    = 0;
    enRrcRatType = RRC_NAS_RAT_TYPE_BUTT;
    i            = 0;

    /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    for (i = 0; i < pstSearchedPlmnInfoMsg->ulAvailPlmnNum; i++)
    {
        ulPlmnNum = pstSearchedExistPlmnInfo->usSearchPlmnNum;

        if (ulPlmnNum >= NAS_MMC_MAX_ROAM_PLMN_NUM_IN_SELECTION_LIST)
        {
            return;
        }

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.stPlmnId.ulMcc = pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].stPlmnId.ulMcc;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.stPlmnId.ulMnc = pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].stPlmnId.ulMnc;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].aucLac[0]              = (pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].usLac & 0xFF00) >> 8;
        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].aucLac[1]              = pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].usLac & 0x00FF;

        enRrcRatType = pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].ulRat;
        NAS_MMC_ConvertRrcRatToNasFormat(enRrcRatType, &pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat.enRat);

        /* ������״̬��Ҫ���ܵ������б����Ȳ�ѯ�£����״̬�Ѿ���register��
           ���²���ʱ��Ҫʹ�������б���״̬������Ĭ�ϸ���Ϊsearched exist����ᵼ�¶������*/
        enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat, pstPlmnSrchList);

        if (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED == enNetStatus)
        {
            pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_REGISTERED;
        }
        else
        {
            pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
        }

        enPlmnType = NAS_MMC_GetRoamPlmnTypeInPlmnSelectionList(&pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].stPlmnWithRat, pstPlmnSrchList);

        pstSearchedExistPlmnInfo->astPlmnSelectionList[ulPlmnNum].enPlmnType  = enPlmnType;
        pstSearchedExistPlmnInfo->usSearchPlmnNum++;
    }

    return;
}

VOS_UINT32 NAS_MMC_IsPlmnExistInRoamPlmnList(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstPlmnId,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU   *pstNewRoamPlmnSelectionList
)
{
    VOS_UINT32                          i;

    for (i = 0; i < pstNewRoamPlmnSelectionList->usSearchPlmnNum; i++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnId->stPlmnId,
                      &pstNewRoamPlmnSelectionList->astPlmnSelectionList[i].stPlmnWithRat.stPlmnId))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_BuildRoamPlmnSelectionPlmnInfo(
    NAS_MML_PLMN_WITH_RAT_STRU                   *pstPlmnWithRat,
    NAS_MMC_ROAM_PLMN_INFO_STRU                  *pstPlmnInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU        *pstPlmnSelectionList
)
{
    NAS_MMC_ROAM_PLMN_INFO_STRU                             stRoamPlmnInfo;
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus;

    PS_MEM_SET(&stRoamPlmnInfo, 0, sizeof(stRoamPlmnInfo));

    stRoamPlmnInfo.stPlmnWithRat.stPlmnId.ulMcc = pstPlmnWithRat->stPlmnId.ulMcc;
    stRoamPlmnInfo.stPlmnWithRat.stPlmnId.ulMnc = pstPlmnWithRat->stPlmnId.ulMnc;
    stRoamPlmnInfo.stPlmnWithRat.enRat          = pstPlmnWithRat->enRat;

    /* ������״̬��Ҫ���ܵ������б����Ȳ�ѯ�£����״̬�Ѿ���searched no exist��register
       ��exist�����²���ʱ��Ҫʹ�������б���״̬������Ĭ�ϸ���Ϊno searched����ᵼ�¶������*/
    enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&stRoamPlmnInfo.stPlmnWithRat, pstPlmnSelectionList);

    if (NAS_MMC_NET_STATUS_BUTT != enNetStatus)
    {
        stRoamPlmnInfo.enNetStatus = enNetStatus;
    }
    else
    {
        stRoamPlmnInfo.enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
    }

    stRoamPlmnInfo.enPlmnType = NAS_MMC_GetRoamPlmnTypeInPlmnSelectionList(&stRoamPlmnInfo.stPlmnWithRat, pstPlmnSelectionList);
    stRoamPlmnInfo.aucLac[0]  = NAS_MML_LAC_LOW_BYTE_INVALID;
    stRoamPlmnInfo.aucLac[1]  = NAS_MML_LAC_HIGH_BYTE_INVALID;

    PS_MEM_CPY(pstPlmnInfo, &stRoamPlmnInfo, sizeof(NAS_MMC_ROAM_PLMN_INFO_STRU));
    return;
}


VOS_VOID NAS_MMC_SortRoamPlmnSelectionList(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList,
    VOS_UINT32                          ulRoamPlmnListNum,
    NAS_MMC_ROAM_PLMN_INFO_STRU        *pstRoamPlmnList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j;
    VOS_UINT32                          ulHighPrioRoamPlmnIndex;
    NAS_MMC_ROAM_PLMN_INFO_STRU         stHighPrioRoamPlmnInfo;

    PS_MEM_SET(&stHighPrioRoamPlmnInfo, 0, sizeof(NAS_MMC_ROAM_PLMN_INFO_STRU));

    if (0 == ulRoamPlmnListNum)
    {
        return;
    }

    for (i = 0; i < ulRoamPlmnListNum - 1; ++i)
    {
        ulHighPrioRoamPlmnIndex = i;

        for (j = i + 1; j < ulRoamPlmnListNum; ++j)
        {
            /* �޸ĺ����� */
            if (1 == NAS_MMC_CompareRoamPlmnPrio(&pstRoamPlmnList[j],&pstRoamPlmnList[ulHighPrioRoamPlmnIndex],pstPrioRatList))
            {
                ulHighPrioRoamPlmnIndex = j;
            }
        }

        PS_MEM_CPY(&stHighPrioRoamPlmnInfo, &pstRoamPlmnList[ulHighPrioRoamPlmnIndex],sizeof(NAS_MMC_ROAM_PLMN_INFO_STRU));
        PS_MEM_CPY(&pstRoamPlmnList[ulHighPrioRoamPlmnIndex], &pstRoamPlmnList[i],sizeof(NAS_MMC_ROAM_PLMN_INFO_STRU));
        PS_MEM_CPY(&pstRoamPlmnList[i], &stHighPrioRoamPlmnInfo, sizeof(NAS_MMC_ROAM_PLMN_INFO_STRU));
    }

    return;
}


VOS_VOID NAS_MMC_AddDplmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstSearchedExistPlmnInfo,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstNewRoamPlmnSelectionList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU                      *pstDPlmnNPlmnCfgInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;
    NAS_MMC_ROAM_PLMN_INFO_STRU                             stRoamPlmnInfo;
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;

    pstDPlmnNPlmnCfgInfo     = NAS_MMC_GetDPlmnNPlmnCfgInfo();
    i                        = 0;
    j                        = 0;

    PS_MEM_SET(&stPlmnWithRat, 0, sizeof(stPlmnWithRat));
    PS_MEM_SET(&stRoamPlmnInfo, 0, sizeof(stRoamPlmnInfo));

    /* ���TAF_MMA_GetNetworkNameTblAddr����Ϣ��ȫ��DPLMN�е����粻��
       TAF_MMA_GetNetworkNameTblAddr�У�����DPLMNû�м��������б���������ȼ���DPLMN������*/
    for (i = 0; i < pstSearchedExistPlmnInfo->usSearchPlmnNum; i++)
    {
        for (j = 0; j < pstDPlmnNPlmnCfgInfo->usDplmnListNum; j++)
        {
            stPlmnWithRat.stPlmnId.ulMcc = pstDPlmnNPlmnCfgInfo->astDPlmnList[j].stSimPlmnWithRat.stPlmnId.ulMcc;
            stPlmnWithRat.stPlmnId.ulMnc = pstDPlmnNPlmnCfgInfo->astDPlmnList[j].stSimPlmnWithRat.stPlmnId.ulMnc;
            stPlmnWithRat.enRat = NAS_MMC_ConvertSimRatToNetRat(pstDPlmnNPlmnCfgInfo->astDPlmnList[j].stSimPlmnWithRat.usSimRat);

            if (VOS_TRUE == NAS_MML_IsPlmnMccBelongToTheSameCountry(stPlmnWithRat.stPlmnId.ulMcc,
                                                pstSearchedExistPlmnInfo->astPlmnSelectionList[i].stPlmnWithRat.stPlmnId.ulMcc))
            {
                if (VOS_TRUE == NAS_MMC_IsPlmnExistInRoamPlmnList(&stPlmnWithRat, pstNewRoamPlmnSelectionList))
                {
                    continue;
                }

                if (NAS_MML_NET_RAT_TYPE_BUTT != stPlmnWithRat.enRat)
                {
                    NAS_MMC_BuildRoamPlmnSelectionPlmnInfo(&stPlmnWithRat, &stRoamPlmnInfo, pstPlmnSelectionList);
                    NAS_MMC_AddSearchedPlmnInRoamPlmnSelectionList(&stRoamPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);
                }
            }
        }
    }

}


VOS_VOID NAS_MMC_AddUOplmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstSearchedExistPlmnInfo,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstNewRoamPlmnSelectionList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;
    NAS_MMC_ROAM_PLMN_INFO_STRU                             stRoamPlmnInfo;
    VOS_UINT32                                              i = 0;
    VOS_UINT32                                              j = 0;
    VOS_UINT32                                              k = 0;

    PS_MEM_SET(&stPlmnWithRat, 0, sizeof(stPlmnWithRat));
    PS_MEM_SET(&stRoamPlmnInfo, 0, sizeof(stRoamPlmnInfo));

    /* ���TAF_MMA_GetNetworkNameTblAddr����Ϣ��ȫ��UPLMN/OPLMN�е����粻��
     * TAF_MMA_GetNetworkNameTblAddr�У�����UPLMN/OPLMNû�м��������б���������ȼ���UPLMN/OPLMN������*/
    for (i = 0; i < pstSearchedExistPlmnInfo->usSearchPlmnNum; i++)
    {
        for (j = 0; j < pstPlmnSelectionList->usSearchPlmnNum; j++)
        {
            if ((NAS_MMC_PLMN_TYPE_UPLMN == pstPlmnSelectionList->astPlmnSelectionList[j].enPlmnType)
             || (NAS_MMC_PLMN_TYPE_OPLMN == pstPlmnSelectionList->astPlmnSelectionList[j].enPlmnType))
            {
                stPlmnWithRat.stPlmnId.ulMcc = pstPlmnSelectionList->astPlmnSelectionList[j].stPlmnId.ulMcc;
                stPlmnWithRat.stPlmnId.ulMnc = pstPlmnSelectionList->astPlmnSelectionList[j].stPlmnId.ulMnc;

                if (VOS_TRUE == NAS_MML_IsPlmnMccBelongToTheSameCountry(stPlmnWithRat.stPlmnId.ulMcc,
                                                pstSearchedExistPlmnInfo->astPlmnSelectionList[i].stPlmnWithRat.stPlmnId.ulMcc))
                {
                    if (VOS_TRUE == NAS_MMC_IsPlmnExistInRoamPlmnList(&stPlmnWithRat, pstNewRoamPlmnSelectionList))
                    {
                        continue;
                    }

                    for(k = 0; k< pstPlmnSelectionList->astPlmnSelectionList[j].ucRatNum; k++)
                    {
                        stPlmnWithRat.enRat          = pstPlmnSelectionList->astPlmnSelectionList[j].astPlmnRatInfo[k].enRatType;

                        if (NAS_MML_NET_RAT_TYPE_BUTT != stPlmnWithRat.enRat)
                        {
                            NAS_MMC_BuildRoamPlmnSelectionPlmnInfo(&stPlmnWithRat, &stRoamPlmnInfo, pstPlmnSelectionList);
                            NAS_MMC_AddSearchedPlmnInRoamPlmnSelectionList(&stRoamPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);
                        }
                    }
                }
            }
        }
    }
}


VOS_VOID NAS_MMC_AddOtherPlmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstSearchedExistPlmnInfo,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstNewRoamPlmnSelectionList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    TAF_PH_OPERATOR_NAME_TBL_STRU                          *pstNetworkNameTbl = VOS_NULL_PTR;
    NAS_MMC_ROAM_PLMN_INFO_STRU                             stRoamPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;
    VOS_UINT16                                              usTotalNtwkNum;
    VOS_UINT8                                               ucPlmnMncNum;
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;

    usTotalNtwkNum           = TAF_MMA_GetNetWorkNameTblSize();
    pstNetworkNameTbl        = TAF_MMA_GetNetworkNameTblAddr();
    i                        = 0;
    j                        = 0;
    ucPlmnMncNum             = 0;
    PS_MEM_SET(&stPlmnWithRat, 0, sizeof(stPlmnWithRat));
    PS_MEM_SET(&stRoamPlmnInfo, 0, sizeof(stRoamPlmnInfo));

    /* ���Һʹ���������ͬ�����������plmn������roam�����б����������������б� */
    for (i = 0; i < pstSearchedExistPlmnInfo->usSearchPlmnNum; i++)
    {
        for (j = 0; j < usTotalNtwkNum; j++)
        {
            stPlmnWithRat.stPlmnId.ulMcc = pstNetworkNameTbl[j].PlmnId.Mcc;
            stPlmnWithRat.stPlmnId.ulMnc = pstNetworkNameTbl[j].PlmnId.Mnc;
            stPlmnWithRat.enRat          = pstSearchedExistPlmnInfo->astPlmnSelectionList[i].stPlmnWithRat.enRat;

            ucPlmnMncNum = pstNetworkNameTbl[j].ucMncLength;
            NAS_MMC_ConvertPlmnId2NasFormat(&stPlmnWithRat.stPlmnId,ucPlmnMncNum);

            if (VOS_TRUE == NAS_MML_IsPlmnMccBelongToTheSameCountry(stPlmnWithRat.stPlmnId.ulMcc,
                                            pstSearchedExistPlmnInfo->astPlmnSelectionList[i].stPlmnWithRat.stPlmnId.ulMcc))
            {
                if (VOS_TRUE == NAS_MMC_IsPlmnExistInRoamPlmnList(&stPlmnWithRat, pstNewRoamPlmnSelectionList))
                {
                    continue;
                }
                NAS_MMC_BuildRoamPlmnSelectionPlmnInfo(&stPlmnWithRat, &stRoamPlmnInfo, pstPlmnSelectionList);
                NAS_MMC_AddSearchedPlmnInRoamPlmnSelectionList(&stRoamPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);
            }
        }
    }
}


VOS_VOID NAS_MMC_BuildRoamPlmnSelectionListBySearchedExistPlmnInfo(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRat,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstSearchedExistPlmnInfo,
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                       *pstNewRoamPlmnSelectionList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList = VOS_NULL_PTR;
    VOS_UINT32                                              i;

    i                        = 0;
    PS_MEM_SET(&stPrioRatList, 0, sizeof(stPrioRatList));

    /*  �����ڵ��������roam�����б���ǰ�� */
    for (i = 0; i < pstSearchedExistPlmnInfo->usSearchPlmnNum; i++)
    {
        NAS_MMC_AddSearchedPlmnInRoamPlmnSelectionList(&pstSearchedExistPlmnInfo->astPlmnSelectionList[i], pstNewRoamPlmnSelectionList, pstPlmnSelectionList);
    }

    if (VOS_TRUE == NAS_MMC_GetSrchUOplmnPriorToDplmnFlg())
    {
        NAS_MMC_AddUOplmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(pstSearchedExistPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);

        NAS_MMC_AddDplmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(pstSearchedExistPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);

        NAS_MMC_AddOtherPlmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(pstSearchedExistPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);
    }
    else
    {
        NAS_MMC_AddDplmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(pstSearchedExistPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);

        NAS_MMC_AddUOplmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(pstSearchedExistPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);

        NAS_MMC_AddOtherPlmnToRoamPlmnSelectionListBySearchedExistPlmnInfo(pstSearchedExistPlmnInfo, pstNewRoamPlmnSelectionList, pstPlmnSelectionList);
    }

    /* ����ʱ�����б����������������DPLMNǰ������UPLMN/OPLMN���Դ򿪣�������ԭ��Ϊ:DPLMN->UPLMN->OPLMN->AVAILABLE
     * ��������ԭ��Ϊ:UPLMN->OPLMN->DPLMN->AVAILABLE
     */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ѡ���б������ӽ���㱨���������磬��ǰ�������뼼������ */
    NAS_MML_SortSpecRatPrioHighest(enRat, &stPrioRatList);

    NAS_MMC_SortRoamPlmnSelectionList(&stPrioRatList, pstNewRoamPlmnSelectionList->usSearchPlmnNum,
                  pstNewRoamPlmnSelectionList->astPlmnSelectionList);

    /* ����֮�����Ϊ0ʱ�����쳣��ӡ */
    if (0 == pstNewRoamPlmnSelectionList->usSearchPlmnNum)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_BuildRoamPlmnSelectionListBySearchedExistPlmnInfo: usSearchPlmnNum is 0");
    }

    return;
}



VOS_VOID NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmn,
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                             i;

    /* ����PLMN������뼼���������б���ÿ��PLMN������뼼���Ƚ� */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        /* �Ƚ�pstPlmn->stPlmnId,��pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId */
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmn->stPlmnId),
                                                           &(pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId)))
        {
            /* �����ͬ�����������б���PLMN��Ӧ���뼼������Ϣ */
            NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(pstPlmn->enRat,
                                                    enNetStatus,
                                                    &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]));
        }
    }

    return;
}


VOS_UINT32 NAS_MMC_GetSpecQualAvailPlmnInPlmnSelectionList(
    VOS_UINT32                                              ulAvailPlmnListNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstAvailPlmnList,
    NAS_MMC_NET_QUALITY_ENUM_UINT8                          enPlmnQuality,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              ulPlmnNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionPlmnInfo = VOS_NULL_PTR;

    ulPlmnNum = 0x0;

    /* �������б�������ΪAVAIL�ź�����ΪenPlmnQuality�������ҳ� */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnSelectionPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        if (NAS_MMC_PLMN_TYPE_AVAIL_PLMN == pstPlmnSelectionPlmnInfo->enPlmnType)
        {
            /* ѭ���������еĽ��뼼�����ҳ���Ӧ���뼼������ΪenPlmnQuality������ */
            for (j = 0; j < pstPlmnSelectionPlmnInfo->ucRatNum; j++)
            {
                if (enPlmnQuality == pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enQuality)
                {
                    /* ��ʼ��ѡ����Ϣ */
                    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                       NAS_MML_INVALID_SIM_RAT,
                                                       &(pstPlmnSelectionPlmnInfo->stPlmnId),
                                                       &(pstAvailPlmnList[ulPlmnNum]));

                    /* ��ȡ��PLMN�����Ϣ */
                    PS_MEM_CPY(&pstAvailPlmnList[ulPlmnNum].astPlmnRatInfo[0],
                               &(pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j]),
                               sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));

                    pstAvailPlmnList[ulPlmnNum].ucRatNum++;

                    ulPlmnNum ++;

                    if (ulPlmnNum >= ulAvailPlmnListNum)
                    {
                       return ulPlmnNum;
                    }
                }
            }
        }
    }

    return ulPlmnNum;
}


VOS_VOID NAS_MMC_SortRatPrioSpecRat(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enPrioRat,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulPrioRatIndex;
    VOS_UINT32                          ulRatIndex;
    VOS_UINT32                          ulFoundPrioRatFlg;
    VOS_UINT32                          ulFoundRatFlg;

    ulPrioRatIndex    = 0x0;
    ulRatIndex        = 0x0;

    ulFoundPrioRatFlg = VOS_FALSE;
    ulFoundRatFlg     = VOS_FALSE;

    /* �ҳ����Ƚ��뼼���ͷ����Ƚ��뼼����λ�� */
    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        if (enPrioRat == pstPrioRatList->aucRatPrio[i])
        {
            ulPrioRatIndex    = i;
            ulFoundPrioRatFlg = VOS_TRUE;
        }

        if (enRat == pstPrioRatList->aucRatPrio[i])
        {
            ulRatIndex        = i;
            ulFoundRatFlg     = VOS_TRUE;
        }
    }

    /* �����ǰ��֧�����Ƚ��뼼��,����Ҫ���� */
    if ((VOS_FALSE == ulFoundPrioRatFlg)
     || (VOS_FALSE == ulFoundRatFlg))
    {
        return;
    }

    /* ���ǰ�����ȼ��ͣ���Ҫ�����������ȵ��Ƶ�ǰ�� */
    if (ulPrioRatIndex > ulRatIndex)
    {
        (VOS_VOID)PS_MEM_MOVE(&(pstPrioRatList->aucRatPrio[ulRatIndex+1]),
                    &(pstPrioRatList->aucRatPrio[ulRatIndex]),
                    (ulPrioRatIndex-ulRatIndex)*sizeof(NAS_MML_NET_RAT_TYPE_ENUM_UINT8));

        pstPrioRatList->aucRatPrio[ulRatIndex] = enPrioRat;
    }

    return;

}


VOS_VOID NAS_MMC_SortPlmnListRandom(
    VOS_UINT32                                              ulPlmnNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnList
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU  stTempPlmnInfo;
    VOS_UINT32                             ulPlmnListNum;
    VOS_UINT32                             ulRand;

    ulPlmnListNum = ulPlmnNum;

    /* ������� */
    VOS_SetSeed(VOS_GetTick());

    for (; ulPlmnListNum > 1; ulPlmnListNum--)
    {
        /* ��ȡ����� */
        ulRand = VOS_Rand(ulPlmnListNum);
        if (ulRand >  (ulPlmnListNum - 1))
        {
            continue;
        }

        stTempPlmnInfo               = pstPlmnList[ulRand];
        pstPlmnList[ulRand]          = pstPlmnList[ulPlmnListNum-1];
        pstPlmnList[ulPlmnListNum-1] = stTempPlmnInfo;
    }

    return;
}


VOS_VOID NAS_MMC_SortPlmnListPrioRat(
    VOS_UINT32                                             *pulPlmnNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnList,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              ulTmpPlmnNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstTmpPlmnList = VOS_NULL_PTR;

    /* PlmnNum����Ϊ0ʱ���������� */
    if (0 == (*pulPlmnNum))
    {
        return;
    }

    pstTmpPlmnList = (NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                   ((*pulPlmnNum)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU)));

    if (VOS_NULL_PTR == pstTmpPlmnList)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SortPlmnListPrioRat: Mem alloc fail.");

        return;
    }

    ulTmpPlmnNum = 0x0;

    /*���򷽷���,ѭ�����еĽ��뼼��,��������ȼ��Ľ��뼼������ŵ����һ����ѭ����֤�����
      �ȼ����뼼�������綼����ǰ�棬���Ŷ����������������ͬ��ʽ���� */
    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        for (j = 0; j < (*pulPlmnNum); j++)
        {
            if (pstPrioRatList->aucRatPrio[i] == pstPlmnList[j].astPlmnRatInfo[0].enRatType)
            {
                PS_MEM_CPY(&pstTmpPlmnList[ulTmpPlmnNum], &pstPlmnList[j], sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
                ulTmpPlmnNum++;
            }
        }
    }

    *pulPlmnNum = ulTmpPlmnNum;

    PS_MEM_CPY(pstPlmnList, pstTmpPlmnList, (ulTmpPlmnNum*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU)));

    PS_MEM_FREE(WUEPS_PID_MMC, pstTmpPlmnList);

    return;
}


VOS_VOID NAS_MMC_SortPlmnListSignalDecrease(
    VOS_UINT32                                              ulPlmnNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnList
)
{
    /* ��ͬ���뼼����������׼��һ�£���δȷ�����򷽷� */

    return;
}


VOS_VOID NAS_MMC_SortAvailPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *pstHighQualityAvailPlmnList  = VOS_NULL_PTR;
    VOS_UINT32                             ulHighQualityAvailPlmnNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *pstLowQualityAvailPlmnList   = VOS_NULL_PTR;
    VOS_UINT32                             ulLowQualityAvailPlmnNum;
    NAS_MML_PLMN_RAT_PRIO_STRU             stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU            *pstPrioRatList;

    VOS_UINT32                             ulAvailPlmnListNum;

    NAS_MML_MISCELLANEOUS_CFG_INFO_STRU   *pstMiscellaneousCfgInfo = VOS_NULL_PTR;

    pstMiscellaneousCfgInfo = NAS_MML_GetMiscellaneousCfgInfo();

    ulAvailPlmnListNum = NAS_MML_MAX_RAT_NUM * RRC_MAX_HIGH_PLMN_NUM;

    pstHighQualityAvailPlmnList = (NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                   (NAS_MML_MAX_RAT_NUM*RRC_MAX_HIGH_PLMN_NUM)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    if (VOS_NULL_PTR == pstHighQualityAvailPlmnList)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SortAvailPlmnInPlmnSelectionList: Mem alloc fail.");

        return;
    }

    pstLowQualityAvailPlmnList  = (NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                   (NAS_MML_MAX_RAT_NUM*RRC_MAX_HIGH_PLMN_NUM)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    if (VOS_NULL_PTR == pstLowQualityAvailPlmnList)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SortAvailPlmnInPlmnSelectionList: Mem alloc fail.");

        PS_MEM_FREE(WUEPS_PID_MMC, pstHighQualityAvailPlmnList);

        return;
    }

    /* �������б�������ΪAvailabe����Ϊ�ߵ�PLMNȡ�� */
    ulHighQualityAvailPlmnNum = NAS_MMC_GetSpecQualAvailPlmnInPlmnSelectionList(ulAvailPlmnListNum,
                                                                            pstHighQualityAvailPlmnList,
                                                                            NAS_MMC_NET_HIGH_QUALITY,
                                                                            pstPlmnSelectionListInfo);

    /* �������б�������ΪAvailabe����Ϊ�͵�PLMNȡ�� */
    ulLowQualityAvailPlmnNum = NAS_MMC_GetSpecQualAvailPlmnInPlmnSelectionList(ulAvailPlmnListNum,
                                                                            pstLowQualityAvailPlmnList,
                                                                            NAS_MMC_NET_LOW_QUALITY,
                                                                            pstPlmnSelectionListInfo);


    /* �������б�������ΪAvailabe����Ϊ�ߵ�PLMNɾ�� */
    NAS_MMC_DeleteSpecQualAvailPlmnInPlmnSelectionList(NAS_MMC_NET_HIGH_QUALITY, pstPlmnSelectionListInfo);

    /* �������б�������ΪAvailabe����Ϊ�͵�PLMNɾ�� */
    NAS_MMC_DeleteSpecQualAvailPlmnInPlmnSelectionList(NAS_MMC_NET_LOW_QUALITY, pstPlmnSelectionListInfo);

    /* �Ը�����PLMN����������� */
    NAS_MMC_SortPlmnListRandom(ulHighQualityAvailPlmnNum, pstHighQualityAvailPlmnList);

    if (NAS_MMC_NV_ITEM_ACTIVE == pstMiscellaneousCfgInfo->ucWcdmaPriorityGsmFlg)
    {
        pstPrioRatList = NAS_MML_GetMsPrioRatList();
        stPrioRatList  = *pstPrioRatList;

        /* �� W�Ľ��뼼���ƶ��� G��ǰ�� */
        NAS_MMC_SortRatPrioSpecRat(NAS_MML_NET_RAT_TYPE_WCDMA,
                                   NAS_MML_NET_RAT_TYPE_GSM,
                                  &stPrioRatList);

        NAS_MMC_SortPlmnListPrioRat(&ulHighQualityAvailPlmnNum,
                                        pstHighQualityAvailPlmnList,
                                        &stPrioRatList);
    }
    else if (NAS_MMC_NV_ITEM_ACTIVE == pstMiscellaneousCfgInfo->ucSortAvailalePlmnListRatPrioFlg)
    {
        pstPrioRatList = NAS_MML_GetMsPrioRatList();
        stPrioRatList  = *pstPrioRatList;

        /* ���������簴syscfg�������ȼ����� */
        NAS_MMC_SortPlmnListPrioRat(&ulHighQualityAvailPlmnNum,
                                     pstHighQualityAvailPlmnList,
                                    &stPrioRatList);

        /* ���������簴syscfg�������ȼ����� */
        NAS_MMC_SortPlmnListPrioRat(&ulLowQualityAvailPlmnNum,
                                     pstLowQualityAvailPlmnList,
                                    &stPrioRatList);
    }
    else
    {
    }

    /* �Ե�����PLMN���а��źŵݼ����� */
    NAS_MMC_SortPlmnListSignalDecrease(ulLowQualityAvailPlmnNum, pstLowQualityAvailPlmnList);



    /* ��������PLMN�ٷŻ�ѡ���б� */
    NAS_MMC_SaveAvailablePlmnInPlmnSelectionList(ulHighQualityAvailPlmnNum,
                                                 pstHighQualityAvailPlmnList,
                                                 pstPlmnSelectionListInfo);

    /* ��������PLMN�ڷŻ������б� */
    NAS_MMC_SaveAvailablePlmnInPlmnSelectionList(ulLowQualityAvailPlmnNum,
                                                 pstLowQualityAvailPlmnList,
                                                 pstPlmnSelectionListInfo);

    PS_MEM_FREE(WUEPS_PID_MMC, pstHighQualityAvailPlmnList);

    PS_MEM_FREE(WUEPS_PID_MMC, pstLowQualityAvailPlmnList);

    return;
}


VOS_VOID NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                             i;

    i = 0;

    /* �������б�������ΪenPlmnType������ɾ�� */
    while (i < pstPlmnSelectionListInfo->usSearchPlmnNum)
    {
        if (enPlmnType == pstPlmnSelectionListInfo->astPlmnSelectionList[i].enPlmnType)
        {
            pstPlmnSelectionListInfo->usSearchPlmnNum--;

            (VOS_VOID)PS_MEM_MOVE(&pstPlmnSelectionListInfo->astPlmnSelectionList[i],
                        &pstPlmnSelectionListInfo->astPlmnSelectionList[i+1],
                        (pstPlmnSelectionListInfo->usSearchPlmnNum-i)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

            continue;
        }

        i++;
    }

    return;
}


VOS_VOID NAS_MMC_DeleteSpecQualAvailPlmnInPlmnSelectionList(
    NAS_MMC_NET_QUALITY_ENUM_UINT8                          enNetQuality,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    i = 0;

    /* �������б�������ΪAVAIL�ź�����ΪenNetQuality�������ҳ� */
    while (i < pstPlmnSelectionListInfo->usSearchPlmnNum)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);
        if (NAS_MMC_PLMN_TYPE_AVAIL_PLMN == pstPlmnInfo->enPlmnType)
        {
            j = 0;

            /* ѭ���������еĽ��뼼�����ҳ���Ӧ���뼼������ΪenPlmnQuality������ */
            while (j < pstPlmnInfo->ucRatNum)
            {
                if (enNetQuality == pstPlmnInfo->astPlmnRatInfo[j].enQuality)
                {
                    /* �������б��н���PLMN��Ӧ�Ľ��뼼����Ϣ��� */
                    pstPlmnInfo->ucRatNum --;

                    if (j < (NAS_MML_MAX_RAT_NUM - 1))
                    {
                        (VOS_VOID)PS_MEM_MOVE(&pstPlmnInfo->astPlmnRatInfo[j],
                                    &pstPlmnInfo->astPlmnRatInfo[j+1],
                                    (pstPlmnInfo->ucRatNum - j)*sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));
                    }

                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enRatType     = NAS_MML_NET_RAT_TYPE_BUTT;
                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enQuality     = NAS_MMC_NET_QUALITY_BUTT;
                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enNetStatus   = NAS_MMC_NET_STATUS_BUTT;

                    continue;
                }
                j++;
            }

            /* ɾ���������Ӧ���뼼����Ϣ������䲻֧���κν��뼼��������б���ɾ�������� */
            if (0 == pstPlmnInfo->ucRatNum)
            {
                pstPlmnSelectionListInfo->usSearchPlmnNum--;

                if (i < (NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST - 1))
                {
                    (VOS_VOID)PS_MEM_MOVE(&pstPlmnSelectionListInfo->astPlmnSelectionList[i],
                                &pstPlmnSelectionListInfo->astPlmnSelectionList[i+1],
                                (pstPlmnSelectionListInfo->usSearchPlmnNum - i)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
                }
                continue;
            }
        }

        i++;
    }

    return;
}


VOS_VOID NAS_MMC_DeleteSpecNetStatusAvailPlmnInPlmnSelectionList(
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    i = 0;

    /* �������б�������ΪAVAIL,����״̬ΪNO SEARCHED�������ҳ� */
    while (i < pstPlmnSelectionListInfo->usSearchPlmnNum)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);
        if (NAS_MMC_PLMN_TYPE_AVAIL_PLMN == pstPlmnInfo->enPlmnType)
        {
            j = 0;

            /* ѭ���������еĽ��뼼�����ҳ�����״̬ΪNO SEARCHED�������ҳ� */
            while (j < pstPlmnInfo->ucRatNum)
            {
                if (enNetStatus == pstPlmnInfo->astPlmnRatInfo[j].enNetStatus)
                {
                    /* �������б��н���PLMN��Ӧ�Ľ��뼼����Ϣ��� */
                    pstPlmnInfo->ucRatNum --;

                    if (j < (NAS_MML_MAX_RAT_NUM - 1))
                    {
                        (VOS_VOID)PS_MEM_MOVE(&pstPlmnInfo->astPlmnRatInfo[j],
                                    &pstPlmnInfo->astPlmnRatInfo[j+1],
                                    (pstPlmnInfo->ucRatNum - j)*sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));
                    }

                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enRatType     = NAS_MML_NET_RAT_TYPE_BUTT;
                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enQuality     = NAS_MMC_NET_QUALITY_BUTT;
                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enNetStatus   = NAS_MMC_NET_STATUS_BUTT;

                    continue;
                }
                j++;
            }

            /* ɾ���������Ӧ���뼼����Ϣ������䲻֧���κν��뼼��������б���ɾ�������� */
            if (0 == pstPlmnInfo->ucRatNum)
            {
                pstPlmnSelectionListInfo->usSearchPlmnNum--;

                if (i < (NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST - 1))
                {
                    (VOS_VOID)PS_MEM_MOVE(&pstPlmnSelectionListInfo->astPlmnSelectionList[i],
                                &pstPlmnSelectionListInfo->astPlmnSelectionList[i+1],
                                (pstPlmnSelectionListInfo->usSearchPlmnNum - i)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
                }

                continue;
            }
        }

        i++;
    }

    return;
}


VOS_VOID NAS_MMC_DelSpecNetStatusRat(
    NAS_MMC_NET_STATUS_ENUM_UINT8       enNetStatus,
    NAS_MMC_NET_STATUS_ENUM_UINT8      *pstNetStatus,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList
)
{
    VOS_UINT32                          i;

    i = 0;

    /* �쳣���� */
    if (0x0 == pstPrioRatList->ucRatNum)
    {
        return;

    }

    /* �������״̬ΪenNetStatus���Ѹ�״̬�Ľ��뼼����stPrioRatList��ɾ�� */
    while (i < (VOS_UINT32)(pstPrioRatList->ucRatNum-1))
    {
        if (enNetStatus == pstNetStatus[i])
        {
            pstPrioRatList->aucRatPrio[i] = NAS_MML_NET_RAT_TYPE_BUTT;
            /*lint -e961*/
            (VOS_VOID)PS_MEM_MOVE(&(pstPrioRatList->aucRatPrio[i]),
                        &(pstPrioRatList->aucRatPrio[i+1]),
                        (NAS_MML_MAX_RAT_NUM-i-1));

            (VOS_VOID)PS_MEM_MOVE(&(pstNetStatus[i]),
                        &(pstNetStatus[i+1]),
                        (NAS_MML_MAX_RAT_NUM-i-1));
            /*lint +e961*/
            (pstPrioRatList->ucRatNum)--;

            continue;
        }
        i++;
    }

    /* ���һ���Ĵ��� */
    if ((pstPrioRatList->ucRatNum > 0)
     && (enNetStatus == pstNetStatus[i]))
    {
        pstPrioRatList->aucRatPrio[i] = NAS_MML_NET_RAT_TYPE_BUTT;

        (pstPrioRatList->ucRatNum)--;
    }

    return;
}


VOS_VOID NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enSrcPlmnType,
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enDestPlmnType,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                             i;

    /* �������б�������ΪenSrcPlmnType������,�޸�������ΪenDestPlmnType */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        if (enSrcPlmnType == pstPlmnSelectionListInfo->astPlmnSelectionList[i].enPlmnType)
        {
            pstPlmnSelectionListInfo->astPlmnSelectionList[i].enPlmnType = enDestPlmnType;
        }
    }

    return;
}


VOS_UINT32 NAS_MMC_GetNextSearchPlmn(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstDestPlmn,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    VOS_UINT32                                              ulUserSpecPlmnSearch,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              k;
    VOS_UINT32                                              ulIsPlmnSearchingAllowed;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionPlmnInfo = VOS_NULL_PTR;

    /* ���β��������б��е�ÿһ������ */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnSelectionPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        /* ��SYSCFG���õĽ��뼼�����ȼ����β��� */
        for (k = 0; k < pstPrioRatList->ucRatNum; k++)
        {
            /* ���β���һ�����������֧�ֵĽ��뼼�� */
            for (j = 0; j < pstPlmnSelectionPlmnInfo->ucRatNum; j++)
            {
                /* ������ȵĽ��뼼���뵱ǰ���ҵ�����֧�ֵĽ��뼼����ͬ */
                if (pstPrioRatList->aucRatPrio[k] == pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType)
                {
                    ulIsPlmnSearchingAllowed = NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy(&(pstPlmnSelectionPlmnInfo->stPlmnId),
                                                                                         pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType,
                                                                                         pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enNetStatus,
                                                                                         ulUserSpecPlmnSearch);
                    if (VOS_FALSE == ulIsPlmnSearchingAllowed)
                    {
                        /* �洢�Ľ��뼼���������ظ� */
                        break;
                    }

                    pstDestPlmn->stPlmnId = pstPlmnSelectionPlmnInfo->stPlmnId;
                    pstDestPlmn->enRat    = pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType;

                    return VOS_TRUE;
                }
            }
        }
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_GetNextSearchPlmn: return FALSE");

    return VOS_FALSE;
}


VOS_VOID  NAS_MMC_UpdatePlmnIdWithBCCHMnc(
    VOS_UINT32                          ulBcchMnc,
    NAS_MML_PLMN_ID_STRU               *pstDestPlmnId
)
{
    VOS_UINT8                           ucBcchMnc3;

    ucBcchMnc3 = (VOS_UINT8)((ulBcchMnc & 0x00ff0000) >> 16);

    if (0x0F == ucBcchMnc3)
    {
        pstDestPlmnId->ulMnc |= 0x0F0000;
    }
}


VOS_UINT32 NAS_MMC_UpdatePlmnRatInfoInSearchList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo,
    VOS_UINT8                                               ucIsAllBandSearch

)
{
    VOS_UINT32                               i;
    VOS_UINT32                               j;

    for (i = 0; i < pstPlmnSelectionInfo->ucRatNum; i++)
    {
        if (pstPlmnSearchInfo->enRatType == pstPlmnSelectionInfo->astPlmnRatInfo[i].enRatType)
        {
            break;
        }
    }

    if (i == pstPlmnSelectionInfo->ucRatNum)
    {
        return VOS_FALSE;
    }

    /* pstPlmnSelectionInfo->stPlmnId�Ƿ��ڸ������б��� */
    for (j = 0; j < pstPlmnSearchInfo->ulHighPlmnNum; j++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmnSearchInfo->astHighPlmnList[j]),
                                                           &(pstPlmnSelectionInfo->stPlmnId)))
        {
            /* �����ǰ���統ǰ���뼼��״̬Ϊinitial��not exist�������״̬Ϊ���ڣ�����Ϊ������ */
            if (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED != pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus)
            {
                pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
            }
            pstPlmnSelectionInfo->astPlmnRatInfo[i].enQuality   = NAS_MMC_NET_HIGH_QUALITY;
            pstPlmnSelectionInfo->astPlmnRatInfo[i].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;

            /* BCCH���ϱ���Mnc�п��ܳ��Ⱥ�SIM���д洢��Mnc��һ����ͳһ��ΪBCCH�е�Mnc */
            pstPlmnSelectionInfo->stPlmnId.ulMnc = pstPlmnSearchInfo->astHighPlmnList[j].ulMnc;
            return VOS_TRUE;
        }
    }

    for (j = 0; j < pstPlmnSearchInfo->ulLowPlmnNum; j++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn((NAS_MML_PLMN_ID_STRU*)&(pstPlmnSearchInfo->astLowPlmnList[j].stPlmnId),
                                                           &(pstPlmnSelectionInfo->stPlmnId)))
        {
            /* �����ǰ���統ǰ���뼼��״̬Ϊinitial��not exist�������״̬Ϊ���ڣ�����Ϊ����������¼��������ֵ */
            if (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED != pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus)
            {
                pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
            }
            pstPlmnSelectionInfo->astPlmnRatInfo[i].enQuality   = NAS_MMC_NET_LOW_QUALITY;
            pstPlmnSelectionInfo->astPlmnRatInfo[i].lRscp       = pstPlmnSearchInfo->astLowPlmnList[j].lRscp;

            /* BCCH���ϱ���Mnc�п��ܳ��Ⱥ�SIM���д洢��Mnc��һ����ͳһ��ΪBCCH�е�Mnc */
            pstPlmnSelectionInfo->stPlmnId.ulMnc = pstPlmnSearchInfo->astLowPlmnList[j].stPlmnId.ulMnc;
            return VOS_TRUE;
        }
    }


    /* ��ȫƵ������ʱ�򣬲��ܽ�δ�������ĸ���Ϊ�����ڣ�����ʷƵ���ѣ��������ѵȷ�ȫƵ����ʱ��ֻ�ܸ��´��ڵ� */
    if (VOS_TRUE == ucIsAllBandSearch )
    {
        /* �����ǰ���統ǰ���뼼��״̬Ϊinitial��exist��exist seached�������״̬Ϊ������ */
        pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST;
        pstPlmnSelectionInfo->astPlmnRatInfo[i].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_DelPlmnInSearchList(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmn,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo
)
{
    VOS_UINT32                           i;

    /* �Ӹ������б���ɾ�������� */
    for (i = 0; i < pstPlmnSearchInfo->ulHighPlmnNum; i++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmnSearchInfo->astHighPlmnList[i]),
                                                           pstPlmn))
        {
            /*lint -e961*/
            (VOS_VOID)PS_MEM_MOVE(&(pstPlmnSearchInfo->astHighPlmnList[i]),
                        &(pstPlmnSearchInfo->astHighPlmnList[i+1]),
                        (pstPlmnSearchInfo->ulHighPlmnNum-i-1)*sizeof(NAS_MML_PLMN_ID_STRU));
            /*lint +e961*/
            (pstPlmnSearchInfo->ulHighPlmnNum)--;

            return;
        }
    }

    /* �ӵ������б���ɾ�������� */
    for (i = 0; i < pstPlmnSearchInfo->ulLowPlmnNum; i++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn((NAS_MML_PLMN_ID_STRU*)&(pstPlmnSearchInfo->astLowPlmnList[i].stPlmnId),
                                                           pstPlmn))
        {
            /*lint -e961*/
            (VOS_VOID)PS_MEM_MOVE(&(pstPlmnSearchInfo->astLowPlmnList[i]),
                        &(pstPlmnSearchInfo->astLowPlmnList[i+1]),
                        (pstPlmnSearchInfo->ulLowPlmnNum-i-1)*sizeof(NAS_MMC_LOW_PLMN_INFO_STRU));
            /*lint +e961*/
            (pstPlmnSearchInfo->ulLowPlmnNum)--;

            return;
        }
    }

    return;

}



VOS_UINT16 NAS_MMC_GetSpecPlmnTypeIndexInPlmnSelectionList(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enSpecPlmnType,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{

    VOS_UINT16                                              i;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        if (enSpecPlmnType == pstPlmnInfo->enPlmnType)
        {
            return i;
        }
    }

    /* ������ָ�����͵����磬������Чֵ0xffff */
    return NAS_MMC_PLMN_SELECTION_LIST_INVALID_INDEX;
}

VOS_VOID NAS_MMC_InsertRoamPlmnListInPlmnSelectionList(
    VOS_UINT32                                              ulPlmnSearchNum,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRat,
    NAS_MML_PLMN_ID_STRU                                   *pstPlmnIdList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT16                                              usIndex;
    VOS_UINT16                                              usSimRat;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;

    /* ѭ��������������ÿ������,�ж��Ƿ��������б����Ѵ��ڣ�����������ҹ�������rplmn��hplmn��ͬ��
       ����������б���rplmn��hplmnǰ */
    for (i = 0; i < ulPlmnSearchNum; i++)
    {
        if (VOS_TRUE == NAS_MMC_IsPlmnWithRatInPlmnList(&(pstPlmnIdList[i]),
                                                          enRat,
                                                          pstPlmnSelectionListInfo,
                                                          pstPlmnSelectionListInfo->usSearchPlmnNum))
        {
            continue;
        }


        if (VOS_TRUE == NAS_MMC_IsPlmnMccSameWithRplmn(pstPlmnIdList[i].ulMcc))
        {
            continue;
        }

        if (VOS_TRUE == NAS_MMC_IsPlmnMccSameWithHplmn(pstPlmnIdList[i].ulMcc))
        {
            continue;
        }

        usSimRat = NAS_MMC_ConvertNetRatToSimRat(enRat);

        /* ��ȡrplmn�������б��е�index�����rplmn�����ڣ����ȡhplmn�������б��е�index,���������б�indexλ�� */
        usIndex = NAS_MMC_GetSpecPlmnTypeIndexInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN, pstPlmnSelectionListInfo);

        if (NAS_MMC_PLMN_SELECTION_LIST_INVALID_INDEX != usIndex)
        {

            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                               usSimRat,
                                               &pstPlmnIdList[i],
                                               &stPlmnInfo);

            (VOS_VOID)NAS_MMC_InsertPlmnInPlmnSelectionList(&stPlmnInfo,
                                                   pstPlmnSelectionListInfo,
                                                   usIndex);
        }
        else
        {
            usIndex = NAS_MMC_GetSpecPlmnTypeIndexInPlmnSelectionList(NAS_MMC_PLMN_TYPE_HPLMN, pstPlmnSelectionListInfo);

            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                               usSimRat,
                                               &pstPlmnIdList[i],
                                               &stPlmnInfo);

            (VOS_VOID)NAS_MMC_InsertPlmnInPlmnSelectionList(&stPlmnInfo,
                                                   pstPlmnSelectionListInfo,
                                                   usIndex);
        }
    }
}


VOS_VOID NAS_MMC_UpdateRoamPlmnListInPlmnSelectionList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    VOS_UINT32                                              ulAppendFlg,
    VOS_UINT8                                                ucIsAllBandSearch

)
{
    VOS_UINT32                                              i;
    NAS_MML_PLMN_ID_STRU                                    astLowPlmnInfo[NAS_MMC_INTRA_MAX_LOW_QUALITY_PLMN_NUM];

    /*���������PLMN*/
    NAS_MMC_InsertRoamPlmnListInPlmnSelectionList(pstPlmnSearchInfo->ulHighPlmnNum,pstPlmnSearchInfo->enRatType,pstPlmnSearchInfo->astHighPlmnList,pstPlmnSelectionListInfo);

    /*���������PLMN*/
    PS_MEM_SET(astLowPlmnInfo, 0xFF, sizeof(astLowPlmnInfo));
    for (i = 0; i < pstPlmnSearchInfo->ulLowPlmnNum; i++)
    {
        astLowPlmnInfo[i].ulMcc = pstPlmnSearchInfo->astLowPlmnList[i].stPlmnId.ulMcc;
        astLowPlmnInfo[i].ulMnc = pstPlmnSearchInfo->astLowPlmnList[i].stPlmnId.ulMnc;
    }
    NAS_MMC_InsertRoamPlmnListInPlmnSelectionList(pstPlmnSearchInfo->ulLowPlmnNum,pstPlmnSearchInfo->enRatType,astLowPlmnInfo,pstPlmnSelectionListInfo);

    /* ������Ͻ�ʣ�����粹�䵽ѡ���б� */
    NAS_MMC_UpdatePlmnListInPlmnSelectionList(pstPlmnSearchInfo,
                                              pstPlmnSelectionListInfo,
                                              ulAppendFlg,
                                              ucIsAllBandSearch);

    return;
}






VOS_VOID NAS_MMC_UpdatePlmnListInPlmnSelectionList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    VOS_UINT32                                              ulAppendFlg,
    VOS_UINT8                                                ucIsAllBandSearch

)
{
    VOS_UINT32                                              i;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstTmpPlmnSearchInfo = VOS_NULL_PTR;


    pstTmpPlmnSearchInfo = (NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                   sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));

    if (VOS_NULL_PTR == pstTmpPlmnSearchInfo)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_UpdatePlmnListInPlmnSelectionList: Mem alloc fail.");

        return;
    }

    PS_MEM_CPY(pstTmpPlmnSearchInfo, pstPlmnSearchInfo, sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));

    /* ѭ��ѡ���б���ÿһ������ */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {

        /* ����һ�����뼼����������Ϣ���������б��е�һ�����磬������³ɹ�����Ҫ��������Ϣ�ж�Ӧ����ɾ�� */
        if (VOS_TRUE == NAS_MMC_UpdatePlmnRatInfoInSearchList(&(pstPlmnSelectionListInfo->astPlmnSelectionList[i]),
                                               pstPlmnSearchInfo, ucIsAllBandSearch))
        {
            NAS_MMC_DelPlmnInSearchList(&(pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId), pstTmpPlmnSearchInfo);
        }
    }

    /* �����Ҫ�����������б���ϢpastPlmnSearchInfo�е�ʣ�����粹�䵽�����б��� */
    if (VOS_TRUE == ulAppendFlg)
    {
        NAS_MMC_SavePlmnListInPlmnSelectionList(pstTmpPlmnSearchInfo, pstPlmnSelectionListInfo);
    }

    PS_MEM_FREE(WUEPS_PID_MMC, pstTmpPlmnSearchInfo);

    return;
}


NAS_MMC_PLMN_TYPE_ENUM_UINT8 NAS_MMC_GetPlmnTypeInPlmnSelectionList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{

    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    /* ����PLMN������뼼���������б���ÿ��PLMN������뼼���Ƚ� */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        /* �Ƚ�pstPlmn->stPlmnId,��pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId */
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmnWithRat->stPlmnId),
                                                           &(pstPlmnInfo->stPlmnId)))
        {
            /* �����ͬ���ȽϽ��뼼�� */
            for (j = 0; j < pstPlmnInfo->ucRatNum; j++)
            {
                if (pstPlmnWithRat->enRat == pstPlmnInfo->astPlmnRatInfo[j].enRatType)
                {
                    return pstPlmnInfo->enPlmnType;
                }
            }
        }
    }

    return NAS_MMC_PLMN_TYPE_BUTT;
}


VOS_VOID NAS_MMC_GetSpecPlmnNetStatusInPlmnSelectionList(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmn,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_NET_STATUS_ENUM_UINT8                          *paucNetStatus,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              k;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    /* ����PLMN������뼼���������б���ÿ��PLMN������뼼���Ƚ� */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        /* �Ƚ�pstPlmn->stPlmnId,��pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId */
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmn,
                                                           &(pstPlmnInfo->stPlmnId)))
        {
            /* �����ͬ���ȽϽ��뼼�� */
            for (j = 0; j < pstPrioRatList->ucRatNum; j++)
            {
                for (k = 0; k < pstPlmnInfo->ucRatNum; k++)
                {
                    if (pstPrioRatList->aucRatPrio[j] == pstPlmnInfo->astPlmnRatInfo[k].enRatType)
                    {
                        paucNetStatus[j] = pstPlmnInfo->astPlmnRatInfo[k].enNetStatus;
                        break;
                    }
                }
            }
        }
    }

    return;
}


VOS_UINT32 NAS_MMC_GetRPlmn_PlmnSelectionStrategy(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg  = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                stRPlmn;

    /* ��ʼ��Ϊ��ЧPLMN */
    pstPlmnId->ulMcc = NAS_MML_INVALID_MCC;
    pstPlmnId->ulMnc = NAS_MML_INVALID_MNC;

    /* ��ȡRPLMN������Ϣ */
    pstRPlmnCfg = NAS_MML_GetRplmnCfg();

#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_NET_RAT_TYPE_LTE == enRat)
    {
        /* ��ȡLTE��RPLMN */
        return NAS_MML_GetPrefRPlmn(enRat, pstPlmnId);
    }
#endif

    /* ��ȡGU��RPLMN */
    if (VOS_FALSE == NAS_MML_GetPrefRPlmn(enRat, pstPlmnId))
    {
        return VOS_FALSE;
    }

    /* �����֧�ֶ���뼼��RPLMN,����Ҫ��ȡ���Խ��뼼���±�����NV�е�RPLMN */
    if (VOS_FALSE == pstRPlmnCfg->ucMultiRATRplmnFlg)
    {
        return VOS_TRUE;
    }

    if (NAS_MML_NET_RAT_TYPE_WCDMA == enRat)
    {
        stRPlmn = pstRPlmnCfg->stWRplmnInNV;
    }
    else
    {
        stRPlmn = pstRPlmnCfg->stGRplmnInNV;
    }

    /* ��NV�б����RPLMN��Ч�Ҳ��ڽ�ֹ�����У�����NV�д洢������,���򷵻�֮ǰ��SIM���л�ȡ������ */
    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&stRPlmn))
    {
        if (VOS_FALSE == NAS_MML_IsPlmnIdInForbidPlmnList(&stRPlmn))
        {
           *pstPlmnId = stRPlmn;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MMC_NET_STATUS_ENUM_UINT8       enNetStatus,
    VOS_UINT32                          ulUserSpecPlmnSearch
)
{
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulAllowed;
#endif
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo     = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRatId;


    /* ��ʼ��PLMN WITH RAT��Ϣ */
    stPlmnWithRatId.stPlmnId.ulMcc      = pstPlmnId->ulMcc;
    stPlmnWithRatId.stPlmnId.ulMnc      = pstPlmnId->ulMnc;
    stPlmnWithRatId.enRat               = enRat;

#if (FEATURE_ON == FEATURE_LTE)
    ulAllowed = VOS_TRUE;

    /* ��ǰ����ΪLTEʱ */
    if (NAS_MML_NET_RAT_TYPE_LTE == enRat)
    {
        /* �ж��Ƿ�����LTE������������� */
        ulAllowed   = NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy(pstPlmnId);
    }

    if (VOS_FALSE == ulAllowed)
    {
        return VOS_FALSE;
    }

#endif

    /* ���USIM��GSM��ֹ���룬��ѡ��GSM���� */
    if ( (VOS_TRUE == NAS_MML_GetGsmForbidFlg())
      && (NAS_MML_NET_RAT_TYPE_GSM == enRat) )
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy GsmForbidFlg");

        return VOS_FALSE;
    }

    /* �����ǰ���������������ڽ�ֹ���뼼�������б���,��׼������ */
    if (VOS_TRUE == NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList(&stPlmnWithRatId))
    {
       return VOS_FALSE;
    }

    /* �����ǰ���ԵĽ��뼼���ڽ�ֹ���뼼���б��У���׼������ */
    if (VOS_TRUE == NAS_MML_IsRatInForbiddenList(stPlmnWithRatId.enRat))
    {
        return VOS_FALSE;
    }


    /* ����״̬��Ϊδ����������������,���������������� */
    if ((NAS_MMC_NET_STATUS_NO_SEARCHED != enNetStatus)
     && (NAS_MMC_NET_STATUS_SEARCHED_EXIST != enNetStatus))
    {
        return VOS_FALSE;
    }

    /* �ֶ�ģʽ����Ҫ���ж��Ƿ��ڽ�ֹ�����б��� */
    if (VOS_TRUE == ulUserSpecPlmnSearch)
    {
        return VOS_TRUE;
    }

    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();


    /* ��ֹ���磬��Ϊ�϶���BCCH�������磬��forbid plmn�б��� */
    if (VOS_TRUE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstPlmnId,
                                          pstForbidPlmnInfo->ucForbPlmnNum,
                                          pstForbidPlmnInfo->astForbPlmnIdList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy auto in astForbPlmnIdList");

        return VOS_FALSE;
    }

    /* ����forbid plmn for gprs�б��� */
    if (VOS_FALSE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstPlmnId,
                                          pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                          pstForbidPlmnInfo->astForbGprsPlmnList))
    {
        return VOS_TRUE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy auto in astForbGprsPlmnList");

#if (FEATURE_ON == FEATURE_LTE)

    /* LTE��֧��EPS,������PS����,��ʾ�����粻������ */
    if (NAS_MML_NET_RAT_TYPE_LTE == enRat)
    {
        return VOS_FALSE;
    }
#endif

    /* ����ΪPS ONLYʱ,��������ֹGPRS�б��е����� */
    if (NAS_MML_MS_MODE_PS_ONLY == NAS_MML_GetMsMode())
    {
        return VOS_FALSE;
    }

    /* CS����Чʱ */
    if (VOS_FALSE == NAS_MML_GetSimCsRegStatus())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetSimCsRegStatus is false");

        return VOS_FALSE;
    }

    return VOS_TRUE;
}


/*lint -e429*/
VOS_VOID NAS_MMC_SaveAvailablePlmnInPlmnSelectionList(
    VOS_UINT32                                              ulAvailPlmnNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstAvailPlmnList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    /* �ж�����������Ƿ�ᵼ��ѡ���б���� */
    if ((pstPlmnSelectionListInfo->usSearchPlmnNum + ulAvailPlmnNum) < NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST)
    {
        PS_MEM_CPY(&pstPlmnSelectionListInfo->astPlmnSelectionList[pstPlmnSelectionListInfo->usSearchPlmnNum],
                   pstAvailPlmnList,
                   ulAvailPlmnNum*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
        pstPlmnSelectionListInfo->usSearchPlmnNum += (VOS_UINT16)ulAvailPlmnNum;
    }
    else
    {
        PS_MEM_CPY(&pstPlmnSelectionListInfo->astPlmnSelectionList[pstPlmnSelectionListInfo->usSearchPlmnNum],
                   pstAvailPlmnList,
                   (NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST-pstPlmnSelectionListInfo->usSearchPlmnNum)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
        pstPlmnSelectionListInfo->usSearchPlmnNum = NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST;

        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SaveAvailablePlmnInPlmnSelectionList: Plmn selection list full.");
    }

    return;
}


VOS_VOID NAS_MMC_SavePlmnListInPlmnSelectionList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{

    VOS_UINT32                                              i;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmn;

    /* ��ʼ��ѡ����Ϣ */
    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_AVAIL_PLMN, NAS_MML_INVALID_SIM_RAT,
                                       &(stPlmn.stPlmnId), &stPlmn);

    stPlmn.ucRatNum++;
    stPlmn.astPlmnRatInfo[0].enRatType     = pstPlmnSearchInfo->enRatType;
    stPlmn.astPlmnRatInfo[0].enNetStatus   = NAS_MMC_NET_STATUS_SEARCHED_EXIST;

    /* ѭ���������б��л����ڵĸ��������� */
    for (i = 0; i < pstPlmnSearchInfo->ulHighPlmnNum; i++)
    {
        stPlmn.stPlmnId                        = pstPlmnSearchInfo->astHighPlmnList[i];

        stPlmn.astPlmnRatInfo[0].enQuality     = NAS_MMC_NET_HIGH_QUALITY;

        /* ���뵽ѡ���б��� */
        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmn, pstPlmnSelectionListInfo);
    }

    /* ѭ���������б��л����ڵĵ��������� */
    for (i = 0; i < pstPlmnSearchInfo->ulLowPlmnNum; i++)
    {
        stPlmn.stPlmnId.ulMcc                  = pstPlmnSearchInfo->astLowPlmnList[i].stPlmnId.ulMcc;
        stPlmn.stPlmnId.ulMnc                  = pstPlmnSearchInfo->astLowPlmnList[i].stPlmnId.ulMnc;

        stPlmn.astPlmnRatInfo[0].enQuality     = NAS_MMC_NET_LOW_QUALITY;
        stPlmn.astPlmnRatInfo[0].lRscp         = pstPlmnSearchInfo->astLowPlmnList[i].lRscp;

        /* ���뵽ѡ���б��� */
        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmn, pstPlmnSelectionListInfo);
    }

    return;
}
/*lint +e429*/

VOS_VOID NAS_MMC_InitNetStatusInRatSearchInfo(
    NAS_MMC_NET_STATUS_ENUM_UINT8      *paucNetStatus,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList,
    NAS_MMC_RAT_SEARCH_INFO_STRU       *pstSearchRatInfo
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j;
    VOS_UINT8                           ucSearchAllBandFlg;

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        ucSearchAllBandFlg = VOS_FALSE;

        for (j = 0; j < NAS_MML_MAX_RAT_NUM; j++)
        {
            if (pstPrioRatList->aucRatPrio[i] == pstSearchRatInfo[j].enRatType)
            {
                ucSearchAllBandFlg = pstSearchRatInfo[j].ucSearchAllBand;
                break;
            }
        }

        if (VOS_TRUE == ucSearchAllBandFlg)
        {
            paucNetStatus[i] = NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST;
        }
        else
        {
            paucNetStatus[i] = NAS_MMC_NET_STATUS_NO_SEARCHED;
        }
    }
}


#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MMC_RefreshPlmnSelectionList_DisableLte_CsPsMode1RegSuccess(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList,
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo
)
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEPlmnList    = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_NET_STATUS_ENUM_UINT8       aucNetStatus[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                          i;

    PS_MEM_SET(aucNetStatus, 0x00, sizeof(aucNetStatus));
    PS_MEM_SET(&stPrioRatList, 0x00, sizeof(stPrioRatList));

    /* NAS_MML_GetEquPlmnList ��ȡ��ǰ�����EPLMN */
    pstEPlmnList   = NAS_MML_GetEquPlmnList();
    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* ��ԭ�������б���EPLMNȥ��*/
    /* �ֶ�ѡ��ģʽɾ��RPLMN��EPLMN,�û�ָ�����粻ɾ��,Ŀ�������������뼼���ϼ��������û�ָ������ */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б��н��û�ָ��PLMN�޸�ΪAvail plmn,��֤������ȡ��һ����������ʱ,
           ����䴦�ڽ�ֹ�б���,�������������� */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);

        /* ��ѡ���б���ɾ��RPLMN */
        NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN,
                                                      pstPlmnSelectionList);

        /* ��ѡ���б���ɾ��EPLMN */
        NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN,
                                                      pstPlmnSelectionList);
    }
    else
    {
        /* ��ѡ���б��н�RPLMN�޸�ΪAvail plmn */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);

        /* ��ѡ���б��н�EPLMN�޸�ΪAvail plmn */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);

        /* ��ѡ���б���ɾ������ΪUNKNOW��Avail plmn */
        NAS_MMC_DeleteSpecQualAvailPlmnInPlmnSelectionList(NAS_MMC_NET_QUALITY_UNKNOWN,
                                                           pstPlmnSelectionList);

        /* ��������available plmn */
        NAS_MMC_SortAvailPlmnInPlmnSelectionList(pstPlmnSelectionList);
    }

    /* ��EPLMN�Ӻ���ǰ���ӵ�ѡ���б� */
    for ( i = pstEPlmnList->ucEquPlmnNum; i > 0; i-- )
    {
        NAS_MMC_InitNetStatusInRatSearchInfo(aucNetStatus, pstPrioRatList, pstSearchRatInfo);

        NAS_MMC_GetSpecPlmnNetStatusInPlmnSelectionList(&(pstEPlmnList->astEquPlmnAddr[i-1]),
                                                          pstPrioRatList,
                                                          aucNetStatus,
                                                          pstPlmnSelectionList);

        PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(stPrioRatList));

        /* ����״̬ΪNAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,NAS_MMC_NET_STATUS_SEARCHED_REGISTERED�Ľ��뼼��,
           ��ʾ����û��Ҫ�ڽ���������,��˴�stPrioRatList��ɾ�� */
        NAS_MMC_DelSpecNetStatusRat(NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST, aucNetStatus, &stPrioRatList);

        NAS_MMC_DelSpecNetStatusRat(NAS_MMC_NET_STATUS_SEARCHED_REGISTERED, aucNetStatus, &stPrioRatList);

        if (i==1)
        {
            /* ��RPLMN������뵽ѡ���б���ǰ�� */
            NAS_MMC_InsertPlmnToPlmnSelectionListHead(&(pstEPlmnList->astEquPlmnAddr[i-1]),
                                                        NAS_MMC_PLMN_TYPE_RPLMN,
                                                       &stPrioRatList,
                                                        pstPlmnSelectionList);
        }
        else
        {
            /* ��EPLMN������뵽ѡ���б���ǰ�� */
            NAS_MMC_InsertPlmnToPlmnSelectionListHead(&(pstEPlmnList->astEquPlmnAddr[i-1]),
                                                        NAS_MMC_PLMN_TYPE_EPLMN,
                                                       &stPrioRatList,
                                                        pstPlmnSelectionList);
        }

    }

    return;
}

#endif



VOS_VOID NAS_MMC_RefreshEPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    /* EPLMN��Чֱ�ӷ���,ֻ����EPLMN��Ч������²Ÿ���ѡ���б� */
    if (VOS_TRUE == NAS_MML_GetEplmnValidFlg())
    {
        return;
    }


    /* �ֶ�ѡ��ģʽɾ��RPLMN��EPLMN,�û�ָ�����粻ɾ��,Ŀ�������������뼼���ϼ��������û�ָ������ */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б��н��û�ָ��PLMN�޸�ΪAvail plmn,��֤������ȡ��һ����������ʱ,
           ����䴦�ڽ�ֹ�б���,�������������� */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);


        if (VOS_TRUE == NAS_MML_GetDeleteRplmnWhenEplmnInvalidFlg())
        {
            /* ��ѡ���б���ɾ��RPLMN */
            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN,
                                                          pstPlmnSelectionList);
        }

        /* ��ѡ���б���ɾ��EPLMN */
        NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN,
                                                      pstPlmnSelectionList);

        return;
    }

    if (VOS_TRUE == NAS_MML_GetDeleteRplmnWhenEplmnInvalidFlg())
    {
        /* ��ѡ���б��н�RPLMN�޸�ΪAvail plmn */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);
    }

    /* ��ѡ���б��н�EPLMN�޸�ΪAvail plmn */
    NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN,
                                              NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                              pstPlmnSelectionList);


    /* ��ѡ���б���ɾ������״̬Ϊδ���������� */
    NAS_MMC_DeleteSpecNetStatusAvailPlmnInPlmnSelectionList(NAS_MMC_NET_STATUS_NO_SEARCHED,
                                               pstPlmnSelectionList);


    /* ��������available plmn */
    NAS_MMC_SortAvailPlmnInPlmnSelectionList(pstPlmnSelectionList);

    return;
}


VOS_UINT32 NAS_MMC_IsHPlmnPrioSearch( VOS_VOID )
{
    NAS_MML_HPLMN_CFG_INFO_STRU        *pstHplmnCfgInfo = VOS_NULL_PTR;
    NAS_MML_SIM_EHPLMN_INFO_STRU       *pstEHPlmnInfo   = VOS_NULL_PTR;

    /* ��ȡHPLMN��������Ϣ */
    pstHplmnCfgInfo = NAS_MML_GetHplmnCfg();

    /* ��ȡSim����Ehplmn��ص���Ϣ */
    pstEHPlmnInfo  = NAS_MML_GetSimEhplmnList();

    /* �Զ�����ģʽ ֧�ֿ�����������Hplmn */
    if ((NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
     && ( ( (NAS_MML_AUTO_SRCH_FLG_TYPE_PLMN_SEL_IND == pstHplmnCfgInfo->enAutoSearchHplmnFlg)
       && (NAS_MML_LAST_RPLMN_SEL_IND_HPLMN == pstEHPlmnInfo->enPLMNSelInd))
     || (NAS_MML_AUTO_SRCH_FLG_TYPE_HPLMN_FIRST == pstHplmnCfgInfo->enAutoSearchHplmnFlg) ))
    {
        return VOS_TRUE;
    }
    else
    {
       return VOS_FALSE;
    }

}


VOS_UINT32 NAS_MMC_IsRPlmnFirstSearchWithHPlmn(VOS_VOID)
{
    NAS_MML_HPLMN_CFG_INFO_STRU        *pstHplmnCfgInfo = VOS_NULL_PTR;

    /* ��ȡHPLMN��������Ϣ */
    pstHplmnCfgInfo = NAS_MML_GetHplmnCfg();

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsRPlmnFirstSearchWithHPlmn: enAutoSearchHplmnFlg is: ", pstHplmnCfgInfo->enAutoSearchHplmnFlg);

    /* �Զ�����ģʽ ֧�ֿ�����������Hplmn */
    if ((NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
     && (NAS_MML_AUTO_SRCH_FLG_TYPE_RPLMN_WITH_HPLMN_FIRST == pstHplmnCfgInfo->enAutoSearchHplmnFlg))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsRPlmnFirstSearchWithHPlmn: return VOS_TRUE");
        return VOS_TRUE;
    }
    else
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsRPlmnFirstSearchWithHPlmn: return VOS_FALSE");
       return VOS_FALSE;
    }
}




VOS_VOID NAS_MMC_RefreshCurPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList,
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo,
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList       = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_NET_STATUS_ENUM_UINT8       aucNetStatus[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                          i;

    PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
    PS_MEM_SET(aucNetStatus, 0x00, sizeof(aucNetStatus));

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ѡ���б��л�ȡ��ǰ���������״̬ */
    NAS_MMC_InitNetStatusInRatSearchInfo(aucNetStatus, pstPrioRatList, pstSearchRatInfo);

    NAS_MMC_GetSpecPlmnNetStatusInPlmnSelectionList(NAS_MML_GetCurrCampPlmnId(),
                                                    pstPrioRatList,
                                                    aucNetStatus,
                                                    pstPlmnSelectionList);

    i = 0;

    /* ����״̬ΪNAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,NAS_MMC_NET_STATUS_SEARCHED_REGISTERED�Ľ��뼼��,
       ��ʾ����û��Ҫ�ڽ���������,��˴�stPrioRatList��ɾ�� */
    while (i < stPrioRatList.ucRatNum)
    {
        if ((NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST == aucNetStatus[i])
         || (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED == aucNetStatus[i]))
        {
            stPrioRatList.aucRatPrio[i] = NAS_MML_NET_RAT_TYPE_BUTT;

            if (i < (NAS_MML_MAX_RAT_NUM - 1))
            {
                /*lint -e961*/
                (VOS_VOID)PS_MEM_MOVE(&(stPrioRatList.aucRatPrio[i]),
                            &(stPrioRatList.aucRatPrio[i+1]),
                            (NAS_MML_MAX_RAT_NUM-i-1));

                (VOS_VOID)PS_MEM_MOVE(&(aucNetStatus[i]),
                            &(aucNetStatus[i+1]),
                            (NAS_MML_MAX_RAT_NUM-i-1));
                /*lint +e961*/
            }
            stPrioRatList.ucRatNum--;

            continue;
        }
        i++;
    }

    /* �����������ΪButt,��ʾѡ���б���δ�ҵ��˽��뼼������,���������;�Ϊavail����,
       ��ʱֻ��Ҫ����ǰ���뼼��������ǰ�漴�� */
    if (NAS_MMC_PLMN_TYPE_BUTT == enPlmnType)
    {
        enPlmnType = NAS_MMC_PLMN_TYPE_AVAIL_PLMN;
        NAS_MML_SortSpecRatPrioHighest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
    }
    else
    {
        /* ����ǰ���뼼�����뵽��ǰ�� */
        (VOS_VOID)PS_MEM_MOVE(&(stPrioRatList.aucRatPrio[1]),
                    &(stPrioRatList.aucRatPrio[0]),
                    (stPrioRatList.ucRatNum*sizeof(NAS_MML_NET_RAT_TYPE_ENUM_UINT8)));

        stPrioRatList.aucRatPrio[0] = NAS_MML_GetCurrNetRatType();

        stPrioRatList.ucRatNum++;
    }

    /* ����ǰ������뵽ѡ���б���ǰ�� */
    NAS_MMC_InsertPlmnToPlmnSelectionListHead(NAS_MML_GetCurrCampPlmnId(),
                                              enPlmnType,
                                              &stPrioRatList,
                                              pstPlmnSelectionList);

    return;
}


VOS_VOID NAS_MMC_LogPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList       = VOS_NULL_PTR;

    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* ��ѡ���б���Ϣ���͵�OM��� */
    NAS_MMC_SndOmPlmnSelectionList(pstPlmnSelectionList, pstPrioRatList);

    NAS_MMC_LogRplmnRelatedInfo();
    NAS_MMC_LogForbiddenPlmnRelatedInfo();


    /* ��ȡѡ��״̬����������Ϣ���������ⶨλ */
    if (NAS_MMC_FSM_PLMN_SELECTION == NAS_MMC_GetCurrFsmId())
    {
        NAS_MMC_LogFsmPlmnSelectionCtxRelatedInfo(NAS_MMC_GetPlmnSelectionCtx_PlmnSelection());
    }


    return;
}


VOS_VOID NAS_MMC_LogAsPlmnList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    *pstSrchedPlmn
)
{
    NAS_MMC_LOG_AS_PLMN_LIST_MSG_STRU                      *pstMsg  = VOS_NULL_PTR;

    pstMsg = (NAS_MMC_LOG_AS_PLMN_LIST_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_AS_PLMN_LIST_MSG_STRU)));
    if (TAF_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogAsPlmnList:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_AS_PLMN_SELECTION_LIST;

    pstMsg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_AS_PLMN_LIST_MSG_STRU) - VOS_MSG_HEAD_LENGTH;

    PS_MEM_CPY(&pstMsg->stSrchedPlmn, pstSrchedPlmn,sizeof(pstMsg->stSrchedPlmn));

    DIAG_TraceReport(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

    return;
}



VOS_VOID NAS_MMC_LogDplmnNplmnList(VOS_VOID)
{
    NAS_MMC_LOG_DPLMN_LIST_STRU                            *pstLogDplmnMsg       = VOS_NULL_PTR;
    NAS_MMC_LOG_NPLMN_LIST_STRU                            *pstLogNplmnMsg       = VOS_NULL_PTR;
    NAS_MMC_LOG_DPLMN_NPLMN_CFG_STRU                       *pstLogDplmnNplmnCfg  = VOS_NULL_PTR;
    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU                      *pstDPlmnNPlmnCfgInfo = VOS_NULL_PTR;
    VOS_UINT16                                              i;

    pstDPlmnNPlmnCfgInfo     = NAS_MMC_GetDPlmnNPlmnCfgInfo();


    /* ========================== DPLMN-NPLMN-CFG��ʼ ========================== */
    pstLogDplmnNplmnCfg = (NAS_MMC_LOG_DPLMN_NPLMN_CFG_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_DPLMN_NPLMN_CFG_STRU)));

    if (VOS_NULL_PTR == pstLogDplmnNplmnCfg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogDplmnNplmnList:ERROR:Alloc Mem Fail.");
        return;
    }

    pstLogDplmnNplmnCfg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstLogDplmnNplmnCfg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstLogDplmnNplmnCfg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstLogDplmnNplmnCfg->stMsgHeader.ulMsgName       = MMCOM_LOG_DPLMN_NPLMN_CFG;
    pstLogDplmnNplmnCfg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_DPLMN_NPLMN_CFG_STRU) - VOS_MSG_HEAD_LENGTH;

    /* �汾�� */
    PS_MEM_CPY(pstLogDplmnNplmnCfg->aucVersionId, pstDPlmnNPlmnCfgInfo->aucVersionId, NAS_MCC_INFO_VERSION_LEN);

    /* EHPLMN���� */
    pstLogDplmnNplmnCfg->ucEHplmnNum    = pstDPlmnNPlmnCfgInfo->ucEHplmnNum;

    /* EHPLMN�б� */
    PS_MEM_CPY(pstLogDplmnNplmnCfg->astEHplmnList, pstDPlmnNPlmnCfgInfo->astEHplmnList, sizeof(pstDPlmnNPlmnCfgInfo->astEHplmnList));

    DIAG_TraceReport(pstLogDplmnNplmnCfg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstLogDplmnNplmnCfg);

    /* ========================== DPLMN-NPLMN-CFG���� ========================== */


    /* ========================== DPLMN LIST��ʼ ========================== */


    pstLogDplmnMsg = (NAS_MMC_LOG_DPLMN_LIST_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_DPLMN_LIST_STRU)));

    if (VOS_NULL_PTR == pstLogDplmnMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogDplmnNplmnList:ERROR:Alloc Mem Fail.");
        return;
    }

    pstLogDplmnMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstLogDplmnMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstLogDplmnMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstLogDplmnMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_DPLMN_LIST;

    for (i = 0; i < pstDPlmnNPlmnCfgInfo->usDplmnListNum; i++)
    {
        PS_MEM_CPY(&pstLogDplmnMsg->astDPlmnList[i], &pstDPlmnNPlmnCfgInfo->astDPlmnList[i], sizeof(NAS_MMC_SIM_PLMN_WITH_REG_DOMAIN_STRU));
    }

    pstLogDplmnMsg->usDplmnListNum = pstDPlmnNPlmnCfgInfo->usDplmnListNum;

    pstLogDplmnMsg->stMsgHeader.ulLength = sizeof(pstLogDplmnMsg->stMsgHeader.ulMsgName)
                                      + sizeof(pstLogDplmnMsg->usDplmnListNum)
                                      + (pstLogDplmnMsg->usDplmnListNum * sizeof(NAS_MMC_SIM_PLMN_WITH_REG_DOMAIN_STRU));

    DIAG_TraceReport(pstLogDplmnMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstLogDplmnMsg);

    /* ========================== DPLMN LIST���� ========================== */



    /* ========================== NPLMN LIST��ʼ ========================== */

    pstLogNplmnMsg = (NAS_MMC_LOG_NPLMN_LIST_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_NPLMN_LIST_STRU)));

    if (VOS_NULL_PTR == pstLogNplmnMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogDplmnNplmnList:ERROR:Alloc Mem Fail.");
        return;
    }

    pstLogNplmnMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstLogNplmnMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstLogNplmnMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstLogNplmnMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_NPLMN_LIST;

    for (i = 0; i < pstDPlmnNPlmnCfgInfo->usNplmnListNum; i++)
    {
        PS_MEM_CPY(&pstLogNplmnMsg->astNPlmnList[i], &pstDPlmnNPlmnCfgInfo->astNPlmnList[i], sizeof(NAS_MMC_SIM_PLMN_WITH_REG_DOMAIN_STRU));
    }

    pstLogNplmnMsg->usNplmnListNum = pstDPlmnNPlmnCfgInfo->usNplmnListNum;

    pstLogNplmnMsg->stMsgHeader.ulLength = sizeof(pstLogNplmnMsg->stMsgHeader.ulMsgName)
                                      + sizeof(pstLogNplmnMsg->usNplmnListNum)
                                      + (pstLogNplmnMsg->usNplmnListNum * sizeof(NAS_MMC_SIM_PLMN_WITH_REG_DOMAIN_STRU));

    DIAG_TraceReport(pstLogNplmnMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstLogNplmnMsg);
    /* ========================== NPLMN LIST���� ========================== */

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_HighPrioPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT8                           uc3GPPUplmnNotPrefFlg;

    uc3GPPUplmnNotPrefFlg = NAS_MML_Get3GPP2UplmnNotPrefFlg();



    if ( VOS_TRUE == NAS_MMC_IsAisRoamingEffect())
    {
        /* ��ѡ���б�����������Ҫ��������������*/
        NAS_MMC_AddAisRoamPlmnInPlmnSelectionList(pstHighPrioPlmnSearchListInfo);
    }

    /* ����EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstHighPrioPlmnSearchListInfo);

    if ((NAS_MMC_REG_CONTROL_BY_3GPP_MMC == NAS_MMC_GetRegCtrl())
     || (VOS_FALSE == uc3GPPUplmnNotPrefFlg))
    {
        /* ����UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstHighPrioPlmnSearchListInfo);
    }
    else
    {
        /* ����OPLMN */
        NAS_MMC_AddOPlmnInPlmnSelectionList(pstHighPrioPlmnSearchListInfo);
    }

    return;
}


VOS_UINT32 NAS_MMC_IsPlmnRatSupported(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo
)
{
    VOS_UINT32                         i;

    for (i = 0; i < pstPlmnInfo->ucRatNum; i++)
    {
        if (VOS_TRUE == NAS_MML_IsNetRatSupported(pstPlmnInfo->astPlmnRatInfo[i].enRatType))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MMC_IsHighPrioPlmnSameCountry(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo,
    NAS_MML_PLMN_ID_STRU                                   *pstVPlmnId
)
{
    VOS_UINT32                                      ulIsSameCountryFlg;
    NAS_MML_BG_SEARCH_CFG_INFO_STRU                 *pstBgSearchCfg = VOS_NULL_PTR;

    NAS_MMC_HPLMN_SEARCH_REGARDLESS_MCC_CTX_STRU    *pstBgSearchRegardlessMccCtx = VOS_NULL_PTR;

    pstBgSearchCfg              = NAS_MML_GetBgSearchCfg();

    pstBgSearchRegardlessMccCtx = NAS_MML_GetBgSearchRegardlessMccCtx();


    ulIsSameCountryFlg  = NAS_MML_ComparePlmnMcc(pstVPlmnId->ulMcc, pstHighPrioPlmnInfo->stPlmnId.ulMcc);

    /* ucSpecialRoamFlg�ر�ʱ,��������벻ͬ,����VOS_FALSE��
       ����ucSpecialRoamFlg��ʱ,plmn����Ϊplmn����ΪUPLMN��OPLMN,���������
       ��ͬ����VOS_FALSE,EHPLMN�޴�����*/

    if ((VOS_TRUE == pstBgSearchCfg->ucSpecialRoamFlg)
     && (NAS_MMC_PLMN_TYPE_HPLMN == pstHighPrioPlmnInfo->enPlmnType))
    {
        return VOS_TRUE;
    }

    /* ��ǰ�������ڶ��Ƶ�BG�Ѳ����ǹ�������б��� */
    if (VOS_TRUE == NAS_MML_IsMccInDestMccList(pstVPlmnId->ulMcc, pstBgSearchRegardlessMccCtx->ucCustomMccNum, pstBgSearchRegardlessMccCtx->aulCostumMccList))
    {
        return VOS_TRUE;
    }

    return ulIsSameCountryFlg;
}


VOS_UINT32 NAS_MMC_CompareVPlmnPrioWithHighPrioPlmn(
    NAS_MML_PLMN_ID_STRU                                   *pstVPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRat,
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulIsPlmnInEplmnList;
    VOS_UINT32                          ulIsPlmnRatSupported;
    VOS_UINT32                          ulIsEPlmnListValid;

    ulIsPlmnInEplmnList = VOS_FALSE;

    ulIsEPlmnListValid  = VOS_FALSE;

    /* �ж�EPLMN�б��Ƿ�ΪVPLMN�ĵ�ЧPLMN�б�,����֮ǰע��ɹ�,��ѡ�������ֹ����,��ʱEPLMN�б���Ϊ��ǰ�����EPLMN�б� */
    if (pstEplmnList->ucEquPlmnNum != 0)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstVPlmnId, &(pstEplmnList->astEquPlmnAddr[0])))
        {
            ulIsEPlmnListValid = VOS_TRUE;
        }
    }

    /* �жϸ����ȼ�����Ľ��뼼���Ƿ�֧��,��������ȼ�������EPLMN���ҽ��뼼��
       ��ǰ֧�֣��򷵻�VOS_TRUE,��ģʱEPLMN���ֽ��뼼�� */
    ulIsPlmnRatSupported = NAS_MMC_IsPlmnRatSupported(pstHighPrioPlmnInfo);

    /* ����23.122 4.4.3.3:In the case
    that the mobile has a stored "Equivalent PLMNs" list the mobile shall
    only select a PLMN if it is of a higher priority than those of the same
    country as the current serving PLMN which are stored in the "Equivalent
    PLMNs" list,��ʱ����Ҫ����,������ȼ��������Ѿ���EPLMN�б��з���VOS_TRUE,
    EPLMN�Ǵ������±�1��ʼ��ŵģ������±�0��ŵ���RPLMN */
    if (VOS_TRUE == ulIsEPlmnListValid)
    {
        ulIsPlmnInEplmnList = NAS_MML_IsSimPlmnIdInDestBcchPlmnList(&(pstHighPrioPlmnInfo->stPlmnId),
                                                         pstEplmnList->ucEquPlmnNum - 1,
                                                         &pstEplmnList->astEquPlmnAddr[1]);
    }

    if ((VOS_TRUE == ulIsPlmnInEplmnList)
     && (VOS_TRUE == ulIsPlmnRatSupported))
    {
        return VOS_TRUE;
    }

    /* h) If the PLMN of the highest priority PLMN/access technology combination
    available is the current VPLMN,or one of the PLMNs in the ""Equivalent PLMNs""
    list,the MS shall remain on the current PLMN/access technology combination. */
    /* ��ǰפ��������PLMN ID��������ȼ�������һ��,
       �Ҹø����ȼ�����֧�ֵĽ��뼼����ǰ֧�ַ���VOS_TRUE */
    if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstVPlmnId, &(pstHighPrioPlmnInfo->stPlmnId)))
    {
        /* ���VPLMN��HPLMN, ��ʹ��ǰ�Ľ��뼼����������ȼ���ͬҲ��Ϊ����������ȼ� */
        if (VOS_TRUE == NAS_MMC_IsCampOnHPlmn())
        {
            return VOS_TRUE;
        }

        for (i = 0; i < pstHighPrioPlmnInfo->ucRatNum; i++)
        {
            if (pstHighPrioPlmnInfo->astPlmnRatInfo[i].enRatType == enRat)
            {
                return VOS_TRUE;
            }
        }

        /* If the PLMN of the highest priority PLMN/access technology combination
          available is the current VPLMN,or one of the PLMNs in the ""Equivalent PLMNs"" list,
          the MS shall remain on the current PLMN/access technology combination.
          ��VPLMN��ͬ�������������ȼ���������ڵ�ǰ���绷���д��ڣ���VPLMN��������ȼ�������
          ��PLMN��ͬ�������磺VPLMNΪG��23002��USIM����������ȼ�����ΪW��23002��״̬
          ΪSEARCHED_EXIST������Ϊ��פ����������ȼ�������*/
        for (i = 0; i < pstHighPrioPlmnInfo->ucRatNum; i++)
        {
            if ((VOS_TRUE ==  NAS_MML_IsNetRatSupported(pstHighPrioPlmnInfo->astPlmnRatInfo[i].enRatType))
             && (NAS_MMC_NET_STATUS_SEARCHED_EXIST == pstHighPrioPlmnInfo->astPlmnRatInfo[i].enNetStatus))
            {
                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_GetNextSearchHighPrioPlmn(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionPlmnInfo,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo,
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstVPlmn,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList
)
{
    VOS_UINT32                                              j;
    VOS_UINT32                                              k;
    VOS_UINT32                                              ulIsPlmnSearchingAllowed;

#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                                              ulIsPlmnSupportDam;
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
    VOS_UINT8                                               ucHighRatBgProc;
    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
#endif



    /* �����뼼�����ȼ����β���,����Ҫ�����ĵ�ǰ�ĸ����ȼ�PLMNʱ���˳� */
    for (k = 0; k < pstPrioRatList->ucRatNum; k++)
    {
        /* ����SYSCFG���ú͸�Plmn֧�ֵĽ��뼼����ȡ��plmn Id�ͽ��뼼���Լ�����״̬ */
        for (j = 0; j < pstPlmnSelectionPlmnInfo->ucRatNum; j++)
        {
            /* ������뼼��֧����״̬��Ϊδ�����������Ѵ��� */
            if (pstPrioRatList->aucRatPrio[k] == pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType)
            {
                ulIsPlmnSearchingAllowed = NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy(&(pstPlmnSelectionPlmnInfo->stPlmnId),
                                                                                     pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType,
                                                                                     pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enNetStatus,
                                                                                     VOS_FALSE);
                if (VOS_FALSE == ulIsPlmnSearchingAllowed)
                {
                    /* �洢�Ľ��뼼���������ظ� */
                    break;
                }

#if (FEATURE_ON == FEATURE_LTE)
                /* �Ѿ�Disable L,�Ҹ����ȼ�������lte�ģ���DAM������Ч��bg��lte�����磬��enable lte */
                if (((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
                  || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus))
                  && (NAS_MML_NET_RAT_TYPE_LTE == pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType))
                {
                    /* ���֮ǰdisable lte��������֧��DAM�����磬�ҽ�ֹ���뼼�������б�����9014 nvδ������
                      ������enable lte,AT&T LTE-BTR-1-1840����Ҫ��ֻ��ֹ310410 lte���磬����lte������ã�
                      �������ɾ���������������Ҫ���ԣ�����DAM nv����в��� */
                    ulIsPlmnSupportDam = NAS_MML_IsPlmnSupportDam(NAS_MMC_GetDisabledLtePlmnId());

                    if ((VOS_FALSE == NAS_MML_GetAddDamPlmnInDisablePlmnWithRatListFlag())
                      && (VOS_TRUE == ulIsPlmnSupportDam))
                    {
                        continue;
                    }

                    ucHighRatBgProc = NAS_MMC_IsHighPrioRatHplmnSearch_BgPlmnSearch();
                    /* �����ȼ�RAT��ENABLE LTE������Ч��bg��lte�����磬��enable lte */
                    if ( (NAS_MML_HIGH_PRIO_RAT_BG_SEARCH_PROC_LTE_NOT_ENABLE_LTE == NAS_MML_GetHighPrioRatBgEnableLteFlag())
                      && (VOS_TRUE == ucHighRatBgProc))
                    {
                        continue;
                    }
                }

#endif

                /* פ��HPLMN���һ�ȡ���ĸ����ȼ�RAT��VPLMN��RATһ����������ȡ�����ȼ������б��е��¸�������PLMN */
                /* ֱ���ڹ��������б�ʱ���� */

                pstHighPrioPlmnInfo->stPlmnId                      = pstPlmnSelectionPlmnInfo->stPlmnId;
                pstHighPrioPlmnInfo->ucRatNum                      = 1;
                pstHighPrioPlmnInfo->astPlmnRatInfo[0].enRatType   = pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType;
                pstHighPrioPlmnInfo->astPlmnRatInfo[0].enNetStatus = pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enNetStatus;

                /* ��ȡ�ɹ�����VOS_TRUE */
                return VOS_TRUE;

            }
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_GetHighestPrioPlmnInHighPrioPlmnList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo
)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_ID_STRU               *pstCurrCampPlmnId = VOS_NULL_PTR;

    /* ȡ�õ�ǰפ����PLMN ID */
    pstCurrCampPlmnId = NAS_MML_GetCurrCampPlmnId();

    for ( i = 0 ; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++ )
    {
        /* ֻȡ��������ͬ��PLMN */
        if (VOS_FALSE == NAS_MML_ComparePlmnMcc(pstCurrCampPlmnId->ulMcc, pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId.ulMcc))
        {
            continue;
        }

        /* ���������ȼ�������뼼����֧��,����ȡ��һ�� */
        if (VOS_FALSE == NAS_MMC_IsPlmnRatSupported(&pstPlmnSelectionListInfo->astPlmnSelectionList[i]))
        {
            continue;
        }

        PS_MEM_CPY(pstHighPrioPlmnInfo, &pstPlmnSelectionListInfo->astPlmnSelectionList[i], sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

        return VOS_TRUE;
    }

    return VOS_FALSE;
}




VOS_VOID  NAS_MMC_DelSpecPlmnWithRatInPlmnList (
    NAS_MML_PLMN_ID_STRU                   *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8         enRat,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j;

    /* ɾ���ظ���PLMN */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; )
    {
        for (j = 0; j < pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum; )
        {
            if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId,
                                                               &(pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId)))
             && (enRat == pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j].enRatType))
            {
                /*lint -e961*/
                /* ɾ����Ӧ��RAT */
                (VOS_VOID)PS_MEM_MOVE(&(pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j]),
                            &(pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j + 1]),
                            ((pstPlmnSelectionListInfo->astPlmnSelectionList[i]).ucRatNum - j - 1) * sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));
                /*lint +e961*/
                pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum--;

            }
            else
            {
                j++;
            }
        }

        /* ���뼼������Ϊ0ʱ��ɾ����Ӧ��PLMN ID */
        if (0 == pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum)
        {
            /*lint -e961*/
            (VOS_VOID)PS_MEM_MOVE(&(pstPlmnSelectionListInfo->astPlmnSelectionList[i]),
                        &(pstPlmnSelectionListInfo->astPlmnSelectionList[i + 1]),
                        (pstPlmnSelectionListInfo->usSearchPlmnNum - i - 1) * sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
            /*lint +e961*/
            pstPlmnSelectionListInfo->usSearchPlmnNum--;
        }
        else
        {
            i++;
        }
    }
}


VOS_VOID NAS_MMC_AddSearchedPlmnListInfoInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;

    if (VOS_NULL_PTR == pstSearchedPlmnListInfo)
    {
        return;
    }

    /* ���������AREA_LOST��Ϣ���ϱ�������ģ���������ѡ���б� */
    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (NAS_MML_NET_RAT_TYPE_BUTT == pstSearchedPlmnListInfo[i].enRatType)
        {
            continue;
        }

        stPrioRatList.ucRatNum      = 1;
        stPrioRatList.aucRatPrio[0] = pstSearchedPlmnListInfo[i].enRatType;

        /* ���������������ѡ���б� */
        for (j = 0; j < pstSearchedPlmnListInfo[i].ulHighPlmnNum; j++)
        {
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstSearchedPlmnListInfo[i].astHighPlmnList[j]),
                                                   NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

        /* ���������������ѡ���б� */
        for (j = 0; j < pstSearchedPlmnListInfo[i].ulLowPlmnNum; j++)
        {
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstSearchedPlmnListInfo[i].astLowPlmnList[j].stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }
    }

    return;
}


VOS_VOID NAS_MMC_GetSpecRatCoverageTypeInSearchedPlmnList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType,
    NAS_MMC_COVERAGE_TYPE_ENUM_UINT8                       *penCoverType
)
{
    VOS_UINT32                                              i;

    /* ���������AREA_LOST��Ϣ���ϱ�������ģ���������ѡ���б� */
    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (enRatType == pstSearchedPlmnListInfo[i].enRatType)
        {
            *penCoverType = pstSearchedPlmnListInfo[i].enCoverType;
            break;
        }
    }

    return;
}



NAS_MMC_COVERAGE_TYPE_ENUM_UINT8 NAS_MMC_GetSpecRatCoverageTypeInRatSearchInfoList(
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType
)
{
    VOS_UINT32                                              i;

    /* ���������AREA_LOST��Ϣ���ϱ�������ģ���������ѡ���б� */
    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (enRatType == pstSearchRatInfo[i].enRatType)
        {
            return pstSearchRatInfo[i].enCoverageType;
        }
    }

    return NAS_MMC_COVERAGE_TYPE_BUTT;
}


VOS_UINT8  NAS_MMC_IsNeedNcellSearch(VOS_VOID)
{
    VOS_UINT32                          ulCurNcellSearchTimerCount;
    VOS_UINT32                          ulTotalNcellSearchTimerCount;
    VOS_UINT8                           ucNcellSearchFlag;


    VOS_UINT8                           ucNcellSearchTimerLen;

    VOS_UINT8                           ucPsAttachAllowFlag;
    VOS_UINT8                           ucCsAttachAllowFlag;

    ulCurNcellSearchTimerCount   = NAS_MMC_GetCurNcellSearchTimerCount_OnPlmn();
    ucNcellSearchFlag            = NAS_MMC_GetNcellSearchFlag();
    ulTotalNcellSearchTimerCount = 0;
    ucPsAttachAllowFlag          = NAS_MML_GetPsAttachAllowFlg();
    ucCsAttachAllowFlag          = NAS_MML_GetCsAttachAllowFlg();

    /* �����ǰ�����Ĵ���������available ��ʱ���ĵ�һ�δ���,ʹ�õ�һ��ʱ��,����ʹ�õڶ��ε� */
    if (NAS_MMC_OOS_PHASE_ONE == NAS_MMC_GetOosPhaseNum())
    {
        ucNcellSearchTimerLen        = NAS_MMC_GetNcellSearchFirstTimerLen();

        if (0 != ucNcellSearchTimerLen)
        {
            ulTotalNcellSearchTimerCount = ((NAS_MMC_GetPhaseOnePeriodicSleepTimerLen()) / ucNcellSearchTimerLen);
        }
    }
    else
    {
        ucNcellSearchTimerLen        = NAS_MMC_GetNcellSearchSecondTimerLen();

        if (0 != ucNcellSearchTimerLen)
        {
            ulTotalNcellSearchTimerCount = ((NAS_MMC_GetPhaseTwoPeriodicSleepTimerLen()) / ucNcellSearchTimerLen);
        }
    }

    /* 1��svlte ���Կ���
       2��֧��ncell��������nv����
       3��modem1 gas�ϱ�����t��l������Ƶ����Ϣ����������Ϊ0ʱҲ������ncell�Ѷ�ʱ��
       4����ǰΪPS attach not allow/cs attach not allow,��ֹ������������ù���
       5��ncell��������δ�ﵽn(��M�׶ε��������ʱ��/ NcellQuickSearchTimer)��
          ��available��ʱ��ʱ������Ϊncell����ʱ�� */
    if ((VOS_TRUE == ucNcellSearchFlag)
     && (ulCurNcellSearchTimerCount < ulTotalNcellSearchTimerCount)
     && (VOS_FALSE == ucPsAttachAllowFlag)
     && (VOS_FALSE == ucCsAttachAllowFlag))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}




VOS_UINT8 NAS_MMC_IsNeedChangeNcellSearchRatOrder(VOS_VOID)
{
    VOS_UINT8                                               ucCampOnHighRatFlag;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstRatPrioList        = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enCampOnRatType;
    VOS_UINT8                                               ucSvlteSupportFlag;
    VOS_UINT8                                               ucPsTransferOffAreaModeFlag;
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8 enSpecPlmnSearchState;

    ucCampOnHighRatFlag             = VOS_FALSE;
    ucSvlteSupportFlag                  = NAS_MML_GetSvlteSupportFlag();
    ucPsTransferOffAreaModeFlag	= NAS_MML_GetPsTransferOffAreaModeFlag();
    enSpecPlmnSearchState          = NAS_MMC_GetSpecPlmnSearchState();

    /* ��ȡ���ȼ��б� */
    pstRatPrioList     = NAS_MML_GetMsPrioRatList();

    /*��ǰ֧��һ��RAT������Ҫ��ת*/
    if (2 > pstRatPrioList->ucRatNum)
    {
         return VOS_FALSE;
    }


    /* ��ȡפ��RAT */
    enCampOnRatType = NAS_MML_GetCurrNetRatType();

    /*פ���ڷ�������ȼ������ϲ�����Ҫ��ת*/
    if ((pstRatPrioList->aucRatPrio[pstRatPrioList->ucRatNum -1] != enCampOnRatType )
    &&  ( NAS_MML_NET_RAT_TYPE_BUTT != enCampOnRatType ))
    {
         ucCampOnHighRatFlag = VOS_TRUE;
    }

    /*��������������Ҫ����:
          1��svlte ���Կ���
          2��֧����������NV ����
          3����ǰ�ڸ����ȼ���RAT ��פ��
          4 ����ǰ�Ѿ����פ��
    */
    if ((VOS_TRUE == ucSvlteSupportFlag)
          && (VOS_TRUE == ucPsTransferOffAreaModeFlag)
          && (VOS_TRUE == ucCampOnHighRatFlag)
          && (NAS_MMC_SPEC_PLMN_SEARCH_STOP == enSpecPlmnSearchState))
    {
        return VOS_TRUE;
    }

   return VOS_FALSE;

}


#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;
    VOS_UINT32                                              ulEnableRoaming;
    VOS_UINT32                                              aulComparedMccList[NAS_MML_ROAM_ENABLED_MAX_MCC_NUMBER];
    VOS_UINT32                                              ulState;
    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;

    enSimType = NAS_MML_GetSimType();

    ulEnableRoaming   = VOS_TRUE;
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    /* �жϵ�ǰѡ���PLMN�Ƿ��������ȼ�Ҫ�� */
    if (VOS_FALSE == NAS_MMC_IsCurrLtePlmnAllowSrchInCLMode(pstPlmnId))
    {
        NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy:WARNING: Current Lte Plmn is not Allowed Srch In CL Mode,ulMcc and ulMnc are",
                                             pstPlmnId->ulMcc, pstPlmnId->ulMnc);
        return VOS_FALSE;
    }
#endif

    /* ��ֹLTE��������ʱ */
    if (VOS_FALSE == NAS_MML_GetLteRoamAllowedFlg())
    {
        NAS_MML_GetRoamEnabledMccList(aulComparedMccList);

        /* �жϸ�PLMN ID��MCC�Ƿ�������פ����MCC�б��� */
        ulEnableRoaming = NAS_MML_IsMccInDestMccList(pstPlmnId->ulMcc, NAS_MML_ROAM_ENABLED_MAX_MCC_NUMBER, aulComparedMccList);
    }

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus = NAS_MML_GetLteCapabilityStatus();

    /* ������������,����Ҫ����LTE������:
        1)��ǰ����Ľ��뼼��ΪL�ҵ�ǰPS����Ч
        2)��ǰ����Ľ��뼼��ΪL�ҵ�ǰDISABLE LTE
        3)LTE��ֹ��������
        4)������ΪSIM��:��Ӧ�ĳ���Ϊ:SIM��ʱ���ó�LTE only����ȥ��Lģ������ʱ����ʱ����Lģ
    */
    if ((VOS_FALSE                             == NAS_MML_GetSimPsRegStatus())
     || (VOS_FALSE == ulEnableRoaming)
     || (NAS_MML_SIM_TYPE_SIM == enSimType))
    {
        NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy ulEnableRoaming,enSimType",ulEnableRoaming,enSimType);

        return VOS_FALSE;
    }

    /* ����CMҵ��ʱ��������L������ */
    if (VOS_TRUE == NAS_MML_GetCmServiceSetupFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy: Skip Lte when CM service exists");
        return VOS_FALSE;
    }

    ulState = NAS_MMC_GetFsmTopState();

    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLCapabilityStatus)
     || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLCapabilityStatus))
    {
        /* BG���������л�ȡ�¸������ȼ�����ʱ������disable Lģ������ */
        if ((NAS_MMC_FSM_BG_PLMN_SEARCH                             == NAS_MMC_GetCurrFsmId())
         && ((NAS_MMC_BG_PLMN_SEARCH_STA_INIT                        == ulState)
          || (NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_SEARCH_CNF == ulState)
          || (NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_BG_PLMN_SEARCH_CNF == ulState)
          || (NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_SEARCH_CNF == ulState)))
        {
            /* PS��������ע��,����Ҫ���� Enable L */
            if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy: Do not enable L as PS attach not allowed");
                return VOS_FALSE;
            }

            /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
            if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy: L is configed as forbidden");
                return VOS_FALSE;
            }

            return VOS_TRUE;
        }

        return VOS_FALSE;
    }

    return VOS_TRUE;
}

#endif



VOS_VOID NAS_MMC_InitPlmnSelectionList_HighPrioRatHplmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_SIM_EHPLMN_INFO_STRU                           *pstEHPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;



    PS_MEM_SET(&stPlmnInfo, 0x0, sizeof(stPlmnInfo));

    /* ��ȡEHPLMN */
    pstEHPlmnInfo = NAS_MML_GetSimEhplmnList();


    for (i = 0; i< pstEHPlmnInfo->ucEhPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo_ForHighPrioRatHplmnSearch(NAS_MMC_PLMN_TYPE_HPLMN,
                                           &(pstEHPlmnInfo->astEhPlmnInfo[i].stPlmnId),
                                           &stPlmnInfo);

        /* ���ӵ������б� */
        if (VOS_FALSE == NAS_MMC_IsPlmnInSearchList(stPlmnInfo, pstHighPrioPlmnSearchListInfo))
        {
            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstHighPrioPlmnSearchListInfo);
        }
    }

    return;
}



VOS_UINT32 NAS_MMC_IsPlmnInSearchList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo
)
{
    VOS_UINT32                          ulPlmnNum;
    NAS_MML_PLMN_ID_STRU                stPlmnId;

    PS_MEM_SET(&stPlmnId, 0, sizeof(NAS_MML_PLMN_ID_STRU));

    for (ulPlmnNum = 0; ulPlmnNum < pstHighPrioPlmnSearchListInfo->usSearchPlmnNum; ulPlmnNum++)
    {
        PS_MEM_CPY(&stPlmnId, &(pstHighPrioPlmnSearchListInfo->astPlmnSelectionList[ulPlmnNum].stPlmnId), sizeof(stPlmnId));
        if ((stPlmnId.ulMcc == stPlmnInfo.stPlmnId.ulMcc)
         && (stPlmnId.ulMnc == stPlmnInfo.stPlmnId.ulMnc))
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsPlmnWithRatInSearchList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSearchListInfo
)
{
    VOS_UINT32                                              ulPlmnNum;
    NAS_MML_PLMN_ID_STRU                                    stPlmnId;
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MMC_PLMN_RAT_INFO_STRU                              astPlmnRatInfo[NAS_MML_MAX_RAT_NUM];
    VOS_UINT8                                               ucRatNum;
    VOS_UINT8                                               ucRatMatched;

    ucRatMatched = VOS_FALSE;

    PS_MEM_SET(&stPlmnId, 0, sizeof(NAS_MML_PLMN_ID_STRU));

    for (ulPlmnNum = 0; ulPlmnNum < pstPlmnSearchListInfo->usSearchPlmnNum; ulPlmnNum++)
    {
        PS_MEM_CPY(&stPlmnId, &(pstPlmnSearchListInfo->astPlmnSelectionList[ulPlmnNum].stPlmnId), sizeof(stPlmnId));
        PS_MEM_CPY(&astPlmnRatInfo[0], &(pstPlmnSearchListInfo->astPlmnSelectionList[ulPlmnNum].astPlmnRatInfo[0]), NAS_MML_MAX_RAT_NUM * sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));

        ucRatNum = pstPlmnSearchListInfo->astPlmnSelectionList[ulPlmnNum].ucRatNum;

        /* plmnƥ�� */
        if ((stPlmnId.ulMcc != stPlmnInfo.stPlmnId.ulMcc)
         || (stPlmnId.ulMnc != stPlmnInfo.stPlmnId.ulMnc))
        {
            continue;
        }

        /* ���뼼��ƥ�� */
        for (i = 0; i < stPlmnInfo.ucRatNum; i++)
        {
            /* ��ÿ�����뼼����ƥ��������Ϊfalse */
            ucRatMatched = VOS_FALSE;

            for (j= 0; j < ucRatNum; j++)
            {
                if (stPlmnInfo.astPlmnRatInfo[i].enRatType == astPlmnRatInfo[j].enRatType)
                {
                    ucRatMatched = VOS_TRUE;

                    break;
                }
            }

            if (VOS_FALSE == ucRatMatched)
            {
                break;
            }
        }

        /* �����н��뼼����ƥ��ɹ� */
        if (i == stPlmnInfo.ucRatNum)
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_BuildPlmnSelectionPlmnInfo_ForHighPrioRatHplmnSearch(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MML_PLMN_ID_STRU                                   *pstPlmnId,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo
)
{
    VOS_UINT8                           ucRatNum;
    VOS_UINT8                           i;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPlmnRatPrio = VOS_NULL_PTR;

    ucRatNum                = 0x0;
    pstPlmnRatPrio = NAS_MML_GetMsPrioRatList();

    PS_MEM_CPY(&pstPlmnInfo->stPlmnId, pstPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
    pstPlmnInfo->enPlmnType = enPlmnType;

    for (i = 0; i < pstPlmnRatPrio->ucRatNum; i++)
    {
        /* ��������ȼ����뼼��HPLMN�б�ʱ��ֱ�ӽ����ȼ��ȵ�ǰ���뼼���͵Ľ��뼼�����˵� */
        if ( pstPlmnRatPrio->aucRatPrio[i] == NAS_MML_GetCurrNetRatType() )
        {
            break;
        }

        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = pstPlmnRatPrio->aucRatPrio[i];
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        ucRatNum++;

    }

    pstPlmnInfo->ucRatNum = ucRatNum;
    /* ����ʣ��λ�ø�ֵΪ��Чֵ */
    for (; ucRatNum < NAS_MML_MAX_RAT_NUM; ucRatNum++)
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
    }

    return;
}


VOS_UINT32 NAS_MMC_IsPlmnIdInSearchedPlmnList(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmnId,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                          ulHighPlmnIndex;
    VOS_UINT32                          ulLowPlmnIndex;
    VOS_UINT8                           ucRatIndex;

    if (VOS_NULL_PTR == pstSearchedPlmnListInfo)
    {
        return VOS_FALSE;
    }

    for (ucRatIndex = 0; ucRatIndex < NAS_MML_MAX_RAT_NUM; ucRatIndex++)
    {
        if (NAS_MML_NET_RAT_TYPE_BUTT == pstSearchedPlmnListInfo[ucRatIndex].enRatType)
        {
            continue;
        }

        for (ulHighPlmnIndex = 0; ulHighPlmnIndex < pstSearchedPlmnListInfo[ucRatIndex].ulHighPlmnNum; ulHighPlmnIndex++)
        {
            if ((pstPlmnId->ulMcc == pstSearchedPlmnListInfo[ucRatIndex].astHighPlmnList[ulHighPlmnIndex].ulMcc)
             && (pstPlmnId->ulMnc == pstSearchedPlmnListInfo[ucRatIndex].astHighPlmnList[ulHighPlmnIndex].ulMnc))
            {
                return VOS_TRUE;
            }
        }

        for (ulLowPlmnIndex = 0; ulLowPlmnIndex < pstSearchedPlmnListInfo[ucRatIndex].ulLowPlmnNum; ulLowPlmnIndex++)
        {
            if ((pstPlmnId->ulMcc == pstSearchedPlmnListInfo[ucRatIndex].astLowPlmnList[ulLowPlmnIndex].stPlmnId.ulMcc)
             && (pstPlmnId->ulMnc == pstSearchedPlmnListInfo[ucRatIndex].astLowPlmnList[ulLowPlmnIndex].stPlmnId.ulMnc))
            {
                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}


/*lint -restore */


VOS_UINT8 NAS_MMC_GetSpecSceneSrchRplmnCarryEplmnFlag(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32 enPlmnSearchScene,
    VOS_UINT8                             ucIsRoam
)
{
    NAS_MML_CARRY_EPLMN_WHEN_SRCH_RPLMN_CFG_STRU *pstCarryEplmnWhenSrchRplmnCfg;
    VOS_UINT8                                     ucRetVal;

    pstCarryEplmnWhenSrchRplmnCfg = NAS_MML_GetCarryEplmnWhenSrchRplmnCfg();

    /* ���Կ����� */
    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_GetSpecSceneSrchRplmnCarryEplmnFlag(): is test card");
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetSpecSceneSrchRplmnCarryEplmnFlag(): enSwitchFlag is ", pstCarryEplmnWhenSrchRplmnCfg->enSwitchFlag);

    /* ���Ż����عرյĳ��� */
    if (NAS_MML_CARRY_EPLMN_SWITCH_OFF == pstCarryEplmnWhenSrchRplmnCfg->enSwitchFlag)
    {
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetSpecSceneSrchRplmnCarryEplmnFlag(): enSwitchFlag is ", pstCarryEplmnWhenSrchRplmnCfg->enSwitchFlag);
    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetSpecSceneSrchRplmnCarryEplmnFlag(): ucIsRoam is ", ucIsRoam);

    /* ���Ż�ֻ�����γ�������Ч������ʱ���ڷ����γ����� */
    if ((NAS_MML_CARRY_EPLMN_SWITCH_ON_FOR_ROAM == pstCarryEplmnWhenSrchRplmnCfg->enSwitchFlag)
     && (VOS_FALSE == ucIsRoam))
    {
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetSpecSceneSrchRplmnCarryEplmnFlag(): enPlmnSearchScene is ", enPlmnSearchScene);

    /* �Ż������δ򿪲��Ҵ�ʱ�����γ����������Ż������η����ζ��򿪣�����ݳ���������ȷ���Ƿ���Ż� */
    switch(enPlmnSearchScene)
    {
    case NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON:
        ucRetVal = pstCarryEplmnWhenSrchRplmnCfg->ucCarryEplmnSceneSwitchOn;
        break;

    case NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST:
        ucRetVal = pstCarryEplmnWhenSrchRplmnCfg->ucCarryEplmnSceneAreaLost;
        break;

    case NAS_MMC_PLMN_SEARCH_SCENE_SYSCFG_SET:
        ucRetVal = pstCarryEplmnWhenSrchRplmnCfg->ucCarryEplmnSceneSysCfgSet;
        break;

    case NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED:
    case NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_HISTORY_PLMN_SEARCH:
    case NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_PREFBAND_PLMN_SEARCH:
    case NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH:
    case NAS_MMC_PLMN_SEARCH_SCENE_PREFBAND_LIST_PLMN_SEARCH:
    case NAS_MMC_PLMN_SEARCH_SCENE_FULLBAND_LIST_PLMN_SEARCH:
        ucRetVal = pstCarryEplmnWhenSrchRplmnCfg->ucCarryEplmnSceneAvailableTimerExpired;
        break;

    case NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE:
        ucRetVal = pstCarryEplmnWhenSrchRplmnCfg->ucCarryEplmnSceneDisableLte;
        break;

    case NAS_MMC_PLMN_SEARCH_SCENE_CSFB_SERVIEC_REJ:
        ucRetVal = pstCarryEplmnWhenSrchRplmnCfg->ucCarryEplmnSceneCSFBServiceRej;
        break;

    case NAS_MMC_PLMN_SEARCH_SCENE_ENABLE_LTE_PLMN_SEARCH:
        ucRetVal = pstCarryEplmnWhenSrchRplmnCfg->ucCarryEplmnSceneEnableLte;
        break;

    default:
        ucRetVal = VOS_FALSE;
        break;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetSpecSceneSrchRplmnCarryEplmnFlag(): ucRetVal is ", ucRetVal);

    return ucRetVal;
}



VOS_UINT32 NAS_MMC_IsExistRplmnOrHplmnFlag_InitPlmnSelectionList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU           *pstSearchedExistPlmnInfo
)
{
    VOS_UINT32                                      ulRatIndex;
    VOS_UINT32                                      ulIndex;
    NAS_MML_PLMN_ID_STRU                            stPlmnId;   /* ������PLMN�б�   */

    if (VOS_NULL_PTR == pstSearchedExistPlmnInfo)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_IsExistRplmnOrHplmnFlag_InitPlmnSelectionList:ERROR:pstSearchedExistPlmnInfo is VOS_NULL_PTR");

        return VOS_FALSE;
    }

    for(ulRatIndex = 0; ulRatIndex < NAS_MML_MAX_RAT_NUM; ulRatIndex++)
    {
        for (ulIndex = 0; ulIndex < NAS_MML_MIN(pstSearchedExistPlmnInfo->ulHighPlmnNum,NAS_MMC_INTRA_MAX_HIGH_QUALITY_PLMN_NUM); ulIndex++)
        {
            PS_MEM_SET(&stPlmnId, 0, sizeof(NAS_MML_PLMN_ID_STRU));
            stPlmnId.ulMcc  = pstSearchedExistPlmnInfo->astHighPlmnList[ulIndex].ulMcc;
            stPlmnId.ulMnc  = pstSearchedExistPlmnInfo->astHighPlmnList[ulIndex].ulMnc;

            if (VOS_TRUE == NAS_MMC_IsPlmnSameCountryWithRplmnOrHplmn(&stPlmnId))
            {
                return VOS_TRUE;
            }
        }

        for (ulIndex = 0; ulIndex < NAS_MML_MIN(pstSearchedExistPlmnInfo->ulLowPlmnNum,NAS_MMC_INTRA_MAX_LOW_QUALITY_PLMN_NUM); ulIndex++)
        {
            PS_MEM_SET(&stPlmnId, 0, sizeof(NAS_MML_PLMN_ID_STRU));
            stPlmnId.ulMcc  = pstSearchedExistPlmnInfo->astLowPlmnList[ulIndex].stPlmnId.ulMcc;
            stPlmnId.ulMnc  = pstSearchedExistPlmnInfo->astLowPlmnList[ulIndex].stPlmnId.ulMnc;

            if (VOS_TRUE == NAS_MMC_IsPlmnSameCountryWithRplmnOrHplmn(&stPlmnId))
            {
                return VOS_TRUE;
            }
        }

        pstSearchedExistPlmnInfo++;
    }
    return VOS_FALSE;
}


VOS_VOID NAS_MMC_BuildPlmnInfoWithRatBySearchedPlmnList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU         *pstSearchedPlmnList,
    NAS_MML_PLMN_WITH_RAT_STRU                   *pstPlmnInfo
)
{
    VOS_UINT32                                    ulRatIndex;
    VOS_UINT32                                    i;

    if(VOS_NULL_PTR == pstPlmnInfo)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_BuildPlmnInfoWithRatBySearchedPlmnList:ERROR:pstPlmnInfo is VOS_NULL_PTR");
        return;
    }

    for(ulRatIndex = 0; ulRatIndex < NAS_MML_MAX_RAT_NUM; ulRatIndex++)
    {
        if(VOS_NULL_PTR == pstSearchedPlmnList)
        {
            NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_BuildPlmnInfoWithRatBySearchedPlmnList:ERROR:pstSearchedPlmnList is VOS_NULL_PTR");
            return;
        }

        for (i = 0; i < NAS_MML_MIN(pstSearchedPlmnList->ulHighPlmnNum,NAS_MMC_INTRA_MAX_HIGH_QUALITY_PLMN_NUM); i++)
        {
            if (VOS_FALSE == NAS_MML_IsPlmnIdValid(&(pstSearchedPlmnList->astHighPlmnList[i])))
            {
                continue;
            }

            pstPlmnInfo->enRat          = pstSearchedPlmnList->enRatType;
            pstPlmnInfo->stPlmnId.ulMcc = pstSearchedPlmnList->astHighPlmnList[i].ulMcc;
            pstPlmnInfo->stPlmnId.ulMnc = pstSearchedPlmnList->astHighPlmnList[i].ulMnc;

            return;
        }

        for (i = 0; i < NAS_MML_MIN(pstSearchedPlmnList->ulLowPlmnNum,NAS_MMC_INTRA_MAX_LOW_QUALITY_PLMN_NUM); i++)
        {
            if(VOS_FALSE == NAS_MML_IsPlmnIdValid(&(pstSearchedPlmnList->astLowPlmnList[i].stPlmnId)))
            {
                continue;
            }

            pstPlmnInfo->enRat          = pstSearchedPlmnList->enRatType;
            pstPlmnInfo->stPlmnId.ulMcc = pstSearchedPlmnList->astLowPlmnList[i].stPlmnId.ulMcc;
            pstPlmnInfo->stPlmnId.ulMnc = pstSearchedPlmnList->astLowPlmnList[i].stPlmnId.ulMnc;

            return;
        }

        pstSearchedPlmnList++;
    }
    return;
}


VOS_UINT32  NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene,
    VOS_UINT32                                              ulIsExistRplmnOrHplmn
)
{
    /* ���Կ� */
    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList: Test SIM, No need sort");
        return VOS_FALSE;
    }

    /* ���������Ż������ƹرղ����� */
    if (VOS_FALSE == NAS_MMC_GetRoamPlmnSelectionSortActiveFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList: NV not active, No need sort");
        return VOS_FALSE;
    }

    /* �ֶ�����ģʽ����������Ϣ */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList: Manual Selection Mode, No need sort");
        return VOS_FALSE;
    }

    /* ����Rplmn����Hplmn����ʱ���������� */
    if (VOS_TRUE == ulIsExistRplmnOrHplmn)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList: Exist Rplmn or Hplmn");
        return VOS_FALSE;
    }

    /* �����������ƿ�����Ŀǰֻ�п���ѡ����SYSCFG��ע�ᱻ�ܡ�BG_HISTORY��PREFBAND_LIST��FULLBAND_LIST������ѡ�����Ż��ڳ�ʼ���б�ʱ������������
       ������������������ڳ�ʼ���б�ʱ��Ҫ���������� */
    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList: enPlmnSearchScene:", enPlmnSearchScene);
    if (VOS_FALSE == NAS_MMC_IsPlmnSearchSceneInNeedSortRoamPlmnSelectionListSceneTbl(enPlmnSearchScene))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList: Not InNeedSortRoamPlmnSelectionListScene, No need sort");
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedSortRoamPlmnSelectionList_InitPlmnSelectionList: return TRUE");

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_SortRoamPlmnSelectionList_InitPlmnSelectionList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstSearchedPlmn,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                        stNewRoamPlmnSelectionList;
    NAS_MMC_ROAM_PLMN_LIST_INFO_STRU                        stSearchedExistPlmnInfo;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType;
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList  = VOS_NULL_PTR;


    /* �������γ�����syscfg���������������������б�����ԭ������:
     1����������MMA������������������������б�ǰ��,��NPLMN�е����粻���ӣ�
        ֧�ֵ����н��뼼�������ӣ���ǰ�����Ľ��뼼������
     2����gastNetworkNameTbl���ҳ�����ǰ����������ͬ�������plmn�������������б���
        ��NPLMN�е����粻���ӣ�֧�ֵ����н��뼼�������ӣ���ǰ�����Ľ��뼼������
     3������DPLMN->UPLMN->OPLMN->AVAILABLE PLMN˳������������б���������
    */

    pstPrioRatList  = NAS_MML_GetMsPrioRatList();

    /* ����pstSearchedPlmn����stSearchedExistPlmnInfo */
    PS_MEM_SET(&stSearchedExistPlmnInfo, 0, sizeof(stSearchedExistPlmnInfo));

    enPlmnType  = NAS_MMC_GetRoamPlmnTypeInPlmnSelectionList(pstSearchedPlmn, pstPlmnSelectionListInfo);

    enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(pstSearchedPlmn, pstPlmnSelectionListInfo);

    if (NAS_MMC_NET_STATUS_BUTT == enNetStatus)
    {
        enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
    }

    stSearchedExistPlmnInfo.usSearchPlmnNum = 1;
    stSearchedExistPlmnInfo.astPlmnSelectionList[0].stPlmnWithRat.enRat             = pstPrioRatList->aucRatPrio[0];
    stSearchedExistPlmnInfo.astPlmnSelectionList[0].stPlmnWithRat.stPlmnId.ulMcc    = pstSearchedPlmn->stPlmnId.ulMcc;
    stSearchedExistPlmnInfo.astPlmnSelectionList[0].stPlmnWithRat.stPlmnId.ulMnc    = pstSearchedPlmn->stPlmnId.ulMnc;
    stSearchedExistPlmnInfo.astPlmnSelectionList[0].enPlmnType                      = enPlmnType;
    stSearchedExistPlmnInfo.astPlmnSelectionList[0].enNetStatus                     = enNetStatus;


    /* ��DPLMN/UPLMN/OPLMN/AVAILABLE PLMN���ҳ�MCC��ͬ�����磬���������б� */
    PS_MEM_SET(&stNewRoamPlmnSelectionList, 0, sizeof(stNewRoamPlmnSelectionList));

    /* ��ʼ���б�����ʱ���õ�ǰsyscfg���õ�������ȼ��Ľ��뼼���������������LTE���ȼ�ʱ�����ܻ�������WCDMA */
    NAS_MMC_BuildRoamPlmnSelectionListBySearchedExistPlmnInfo(pstPrioRatList->aucRatPrio[0],
                                                              &stSearchedExistPlmnInfo,
                                                              &stNewRoamPlmnSelectionList,
                                                              pstPlmnSelectionListInfo);
    /* ���Ź����roam�����б����������б�ǰ�� */
    NAS_MMC_AddRoamPlmnSelectionListInPlmnSelectionList(&stNewRoamPlmnSelectionList, pstPlmnSelectionListInfo);

    /* ���øý��뼼�����й�һ������ */
    NAS_MMC_SetSearchedRoamPlmnSortedFlag_PlmnSelection(pstPrioRatList->aucRatPrio[0], VOS_TRUE);

    /* ��ά�ɲ�,��ѡ���б���� */
    NAS_MMC_LogDplmnNplmnList();

    return;
}


VOS_UINT32 NAS_MMC_IsNeedDelCurrCampPlmn_PlmnSelectionStrategy(
    NAS_MML_LAI_STRU                   *pstCurrLai
)
{
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbPlmnInfo = VOS_NULL_PTR;

    /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
    pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();

    /* ��ǰפ����LA��forb LA�Ƿ���ͬ:һ���Ǳ�#13,#15,#17(�ж��Ƽӵ�forb LA�Ĵ���)�����Լ������Ե�ǰ���磬����ʱ��forb LA */
    if ( VOS_TRUE == NAS_MML_IsLaiInDestLaiList(pstCurrLai, pstForbPlmnInfo->ucForbRoamLaNum, pstForbPlmnInfo->astForbRomLaList) )
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedDelCurrCampPlmn_PlmnSelectionStrategy: Curr Camp LA is in ForbRoamLaList");
        return VOS_FALSE;
    }

   return VOS_TRUE;
}


VOS_UINT8  NAS_MMC_IsNeedSrchCurrCampHpmnRejByCause13(VOS_VOID)
{
    VOS_UINT8                           ucKeepSrchHplmnEvenRejByCause13Flg;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegRejCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegRejCause;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbPlmnInfo = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                   *pstCurrLai      = VOS_NULL_PTR;

    /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
    pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();

    enCsRegRejCause = NAS_MML_REG_FAIL_CAUSE_BUTT;
    enPsRegRejCause = NAS_MML_REG_FAIL_CAUSE_BUTT;

    ucKeepSrchHplmnEvenRejByCause13Flg  = NAS_MML_GetKeepSrchHplmnEvenRejByCause13Flg();

    /* �Ż����ԶԲ��Կ�����Ч */
    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        return VOS_FALSE;
    }

    /* �Ż�����δ��ʱ�������ѵ�ǰHPLMN */
    if (VOS_FALSE   == ucKeepSrchHplmnEvenRejByCause13Flg)
    {
        return VOS_FALSE;
    }

    /* δפ��HPLMN�򲻼���������ǰPLMN */
    if (VOS_FALSE   == NAS_MMC_IsCampOnHPlmn())
    {
        return VOS_FALSE;
    }

    /* ��ȡ�ڸ�PLMN�ϵ�ע�ᱻ��ԭ��ֵ */
    NAS_MMC_GetDomainRegStatusInRegInfoList(NAS_MML_GetCurrCampPlmnId(),
                                            NAS_MML_GetCurrNetRatType(),
                                            &enCsRegRejCause,
                                            &enPsRegRejCause);


    /* ��һ����#11�ܾ��򲻼���������ǰHPLMN */
    if ((NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW == enCsRegRejCause)
     || (NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW == enPsRegRejCause))
    {
        return VOS_FALSE;
    }

    /* ��ȡ��ǰפ����LAI */
    pstCurrLai          = NAS_MML_GetCurrCampLai();

    /* ��ǰפ����PLMN��forb LA�Ƿ���ͬ����ͬʱ��ζ����α��վ��#13�ܾ�����Ҫ������HPLMN;���򲻼����� */
    if ( VOS_TRUE == NAS_MML_IsLaiInDestLaiList(pstCurrLai, pstForbPlmnInfo->ucForbRoamLaNum, pstForbPlmnInfo->astForbRomLaList) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_VOID NAS_MMC_UpdateRegFinishedPlmnNetStatusInPlmnSelectionList(
    NAS_MML_PLMN_WITH_RAT_STRU                   *pstPlmnIdWithRat,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU        *pstPlmnSelectionListInfo
)
{

    /* ѡ��״̬���£�HPLMNע�ᱻ#13�ܾ������Ҫ���ѵ�ǰHPLMN���򲻸��µ�ǰ���������״̬ */
    if (VOS_TRUE == NAS_MMC_IsNeedSrchCurrCampHpmnRejByCause13())
    {
        /* ���µ�ǰ���������״̬Ϊ���������Ҵ��ڣ��ڴ��ָ����ȼ������У��������Ѵ����� */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(pstPlmnIdWithRat,
                                                NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                                pstPlmnSelectionListInfo);
    }
    else
    {
        /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��ָ����ȼ������У��������Ѵ����� */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(pstPlmnIdWithRat,
                                                NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                pstPlmnSelectionListInfo);
    }

    return;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

