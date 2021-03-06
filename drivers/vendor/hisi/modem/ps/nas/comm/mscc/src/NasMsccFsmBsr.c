


/*****************************************************************************
  1 头文件包含
*****************************************************************************/

#include "NasMsccFsmBsr.h"
#include "NasMntn.h"
#include "NasMsccCtx.h"
#include "NasMsccTimerMgmt.h"
#include "NasMsccFsmBsrTbl.h"
#include "NasMsccSndHsd.h"
#include "NasMsccSndMmc.h"
#include "NasMsccSndInternalMsg.h"
#include "MsccMmcInterface.h"
#include "NasMsccProcNvim.h"
#include "NasMsccComFunc.h"
#include "NasMsccSysAcqStrategy.h"
#include "NasComm.h"
#include "NasMsccSndMma.h"

#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_NAS_MSCC_FSM_BSR_C


/*****************************************************************************
  2 全局变量定义
*****************************************************************************/


/*****************************************************************************
  3 函数定义
*****************************************************************************/
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)


VOS_VOID NAS_MSCC_SndBgSearchReq_BSR(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPlmnNum;

    NAS_MSCC_PIF_PLMN_ID_STRU           astPlmnListEntry[NAS_MSCC_SYS_ACQ_CUSTOM_PLMN_NUM];

    NAS_MEM_SET_S(&astPlmnListEntry[0],
                  sizeof(NAS_MSCC_PIF_PLMN_ID_STRU) * NAS_MSCC_SYS_ACQ_CUSTOM_PLMN_NUM,
                  0x00,
                  sizeof(NAS_MSCC_PIF_PLMN_ID_STRU) * NAS_MSCC_SYS_ACQ_CUSTOM_PLMN_NUM);


    /* Save the entry message */
    NAS_MSCC_SaveCurEntryMsg(ulEventType, pstMsg);

    if (VOS_TRUE == NAS_MSCC_GetHrpdCampOnFlag())
    {
       ucPlmnNum = 0;

       NAS_MSCC_GetBsrReqPlmnList_BSR(&ucPlmnNum, &astPlmnListEntry[0]);

       NAS_MSCC_SndHsdBgSearchReq(VOS_RATMODE_LTE, NAS_MSCC_GetCurBsrSrchType(), ucPlmnNum, astPlmnListEntry);

       NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_BG_SEARCH_CNF);

       NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_BG_SEARCH_CNF,
                           TI_NAS_MSCC_WAIT_HSD_BG_SEARCH_CNF_LEN);
    }
    else
    {
        NAS_MSCC_SndMmcBgSearchReq(VOS_RATMODE_HRPD);

        NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_BG_SEARCH_CNF);

        NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_BG_SEARCH_CNF,
                            TI_NAS_MSCC_WAIT_MMC_BG_SEARCH_CNF_LEN);
    }

    return;
}


VOS_UINT32 NAS_MSCC_RcvTiBsrTimerExpired_BSR_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MSCC_SndBgSearchReq_BSR(ulEventType, pstMsg);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiRetryTimerExpired_BSR_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MSCC_SndBgSearchReq_BSR(ulEventType, pstMsg);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcRfAvailableInd_BSR_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MSCC_SndBgSearchReq_BSR(ulEventType, pstMsg);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MSCC_RcvHsdBgSearchCnf_BSR_WaitHsdBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    HSD_MSCC_BG_SEARCH_CNF_STRU                            *pstHsdMsccBgSearchCnf;

    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_BG_SEARCH_CNF);

    pstHsdMsccBgSearchCnf = (HSD_MSCC_BG_SEARCH_CNF_STRU *)pstMsg;

    if ((HSD_MSCC_HRPD_BSR_RSLT_FAIL      == pstHsdMsccBgSearchCnf->enRslt)
     || (HSD_MSCC_HRPD_BSR_RSLT_ENUM_BUTT == pstHsdMsccBgSearchCnf->enRslt))
    {
        if (NAS_MSCC_BSR_SRCH_TYPE_HISTORY == NAS_MSCC_GetCurBsrSrchType())
        {
            NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                  NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                  NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

            /* Quit the SubFSM */
            NAS_MSCC_QuitFsmL2();

            return VOS_TRUE;
        }
    }

    if (HSD_MSCC_HRPD_BSR_RSLT_ABORT == pstHsdMsccBgSearchCnf->enRslt)
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    if ((0 == pstHsdMsccBgSearchCnf->stPlmnList.ucHighPlmnNum)
     && (0 == pstHsdMsccBgSearchCnf->stPlmnList.ucLowPlmnNum))
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* query bg searched plmn list pri class type */
    NAS_MSCC_SndMmcPlmnPriClassQueryReq(&pstHsdMsccBgSearchCnf->stPlmnList);

    /* Switch state to NAS_MSCC_BSR_STA_WAIT_MMC_QRY_PLMN_PRI_CLASS_CNF */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_QRY_PLMN_PRI_CLASS_CNF);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_QRY_PLMN_PRI_CLASS_CNF, TI_NAS_MSCC_WAIT_MMC_QRY_PLMN_PRI_CLASS_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvHsdStopBgSearchInd_BSR_WaitHsdBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_BG_SEARCH_CNF);

    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvHsdStopBgSearchInd_BSR_WaitHsdStopBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_STOP_BG_SEARCH_CNF);

    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitHsdBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_BG_SEARCH_CNF);

    /* store the abort flag */
    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    /* stop BG search */
    NAS_MSCC_SndHsdStopBgSearchReq();

    /* change state */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_STOP_BG_SEARCH_CNF);

    /* start the protective timer */
    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_STOP_BG_SEARCH_CNF,
                        TI_NAS_MSCC_WAIT_HSD_STOP_BG_SEARCH_CNF_LEN);

    return VOS_TRUE;

}


VOS_UINT32 NAS_MSCC_RcvTiWaitHsdBgSearchCnfTimeExpired_BSR_WaitHsdBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitHsdBgSearchCnfTimeExpired_BSR_WaitHsdBgSearchCnf ENTERED...");

    /* stop BG search */
    NAS_MSCC_SndHsdStopBgSearchReq();

    /* change state */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_STOP_BG_SEARCH_CNF);

    /* start the protective timer */
    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_STOP_BG_SEARCH_CNF,
                        TI_NAS_MSCC_WAIT_HSD_STOP_BG_SEARCH_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvHsdStopBgSearchCnf_BSR_WaitHsdStopBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* stop the protective timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_STOP_BG_SEARCH_CNF);

    /* has been aborted */
    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NEED_START_RETRY_TIMER);
    }
    else
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
    }

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitHsdStopBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* store the abort flag */
    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitHsdStopBgSearchCnfTimeExpired_BSR_WaitHsdStopBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitHsdStopBgSearchCnfTimeExpired_BSR_WaitHsdStopBgSearchCnf ENTERED...");

    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NEED_START_RETRY_TIMER);
    }
    else
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
    }

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MSCC_RcvMmcPlmnPriClassQueryCnf_BSR_WaitMmcPlmnPriClassQueryCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MSCC_PLMN_PRI_CLASS_QUERY_CNF_STRU                 *pstPlmnPriClassQryCnf;
    NAS_MSCC_MSPL_SYS_PRI_CLASS_ENUM_UINT8                  enDestBsrSysPriClass;
    NAS_MSCC_PIF_SEARCHED_PLMN_LIST_INFO_STRU               stTmpfPlmnList;
    NAS_MSCC_PIF_SEARCHED_PLMN_LIST_INFO_STRU              *pstLtePrefPlmnList;

    pstPlmnPriClassQryCnf = (MMC_MSCC_PLMN_PRI_CLASS_QUERY_CNF_STRU *)pstMsg;

    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_QRY_PLMN_PRI_CLASS_CNF);

    enDestBsrSysPriClass = NAS_MSCC_GetDestBsrSysPriClass_BSR();

    NAS_NORMAL_LOG2(UEPS_PID_MSCC, "NAS_MSCC_RcvMmcPlmnPriClassQueryCnf_BSR_WaitMmcPlmnPriClassQueryCnf:enDestBsrSysPriClass and ucOrigSysMsplIndex",
                                    enDestBsrSysPriClass, NAS_MSCC_GetOrigSysMsplIndex_BSR());

    /* according to the pri class to select the required plmn list */
    PS_MEM_SET(&stTmpfPlmnList, 0x0, sizeof(NAS_MSCC_PIF_SEARCHED_PLMN_LIST_INFO_STRU));
    NAS_MSCC_GetPlmnListInPlmnPriClassListWithPriClass(&pstPlmnPriClassQryCnf->stPlmnPriClassList,
                                                       enDestBsrSysPriClass,
                                                       &stTmpfPlmnList);

    pstLtePrefPlmnList = NAS_MSCC_GetLtePrefPlmnListInfo_BSR();


    /* 原始系统在def mspl rec中,直接使用搜上来的系统 */
    if (VOS_TRUE == NAS_MSCC_GetOrigSysDefMsplRecFlg_BSR())
    {
        NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvMmcPlmnPriClassQueryCnf_BSR_WaitMmcPlmnPriClassQueryCnf orig sys in default mspl rec!");

        PS_MEM_CPY(pstLtePrefPlmnList, &stTmpfPlmnList,sizeof(NAS_MSCC_PIF_SEARCHED_PLMN_LIST_INFO_STRU));
    }
    else
    {
        /* filter the plmn list according to the orignal sys mspl index,if the plmn is not
           in the mspl index,could not add the plmn to the dest lte prefered plmn list */
        NAS_MSCC_GetPlmnListAccordingtoSpecMsplIndex(NAS_MSCC_GetOrigSysMsplIndex_BSR(),
                                                     &stTmpfPlmnList,
                                                     pstLtePrefPlmnList);
    }

    /* if there not exists any plmn with BG search result */
    if ((0 == pstLtePrefPlmnList->ucHighPlmnNum)
     && (0 == pstLtePrefPlmnList->ucLowPlmnNum))
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* send ID_MSCC_HSD_POWER_SAVE_REQ */
    NAS_MSCC_SndHsdPowerSaveReq();

    /* Switch state to NAS_MSCC_BSR_STA_WAIT_PREF_PLMN_QUERY_CNF */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_POWER_SAVE_CNF);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF,
                        TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitMmcPlmnPriClassQueryCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_QRY_PLMN_PRI_CLASS_CNF);

    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitMmcPlmnPriClassQueryCnfTimeExpired_BSR_WaitMmcPlmnPriClassQueryCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitMmcPlmnPriClassQueryCnfTimeExpired_BSR_WaitMmcPlmnPriClassQueryCnf ENTERED...");

    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvHsdPowerSaveCnf_BSR_WaitHsdPowerSaveCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF);

    /* if this proces has been aborted */
    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MSCC_SndMmcPlmnSearchReq(NAS_MSCC_PIF_ACQ_REASON_HIGH_PRIORITY,
                                 NAS_MSCC_PIF_PLMN_SEARCH_SPEC,
                                 VOS_NULL_PTR,
                                 NAS_MSCC_GetLtePrefPlmnListInfo_BSR());

    NAS_MSCC_Set3gppCampOnFlag(VOS_FALSE);

    NAS_MSCC_Set3gppActiveFlg(VOS_TRUE);

    /* change state */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_PLMN_SEARCH_CNF);

    /* start the protective timer */
    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF,
                        TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitHsdPowerSaveCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* record the abort flag */
    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitHsdPowerSaveCnfTimeExpired_BSR_WaitHsdPowerSaveCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitHsdPowerSaveCnfTimeExpired_BSR_WaitHsdPowerSaveCnf ENTERED...");

    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcPlmnSearchCnf_BSR_WaitMmcPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MSCC_PLMN_SELECTION_RLST_IND_STRU                  *pstPlmnSelRlstInd = VOS_NULL_PTR;

    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF);

    pstPlmnSelRlstInd = (MMC_MSCC_PLMN_SELECTION_RLST_IND_STRU *)pstMsg;

    switch (pstPlmnSelRlstInd->enResult)
    {
        case NAS_MSCC_PIF_PLMN_SELECTION_RESULT_NORMAL_SERVICE:
            {
                /* BG search successfully,so send the result as success to Main FSM */
                NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_SUCCESS,
                                      NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                      NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

                /* Quit the SubFSM */
                NAS_MSCC_QuitFsmL2();
            }
            break;

        case NAS_MSCC_PIF_PLMN_SELECTION_RESULT_INTERRUPT:
            {
                NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                      NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                      NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

                /* Quit the SubFSM */
                NAS_MSCC_QuitFsmL2();
            }
            break;

        case NAS_MSCC_PIF_PLMN_SELECTION_RESULT_NO_RF:
            {
                /* SRLTE处理 */
                if (VOS_TRUE == NAS_MSCC_IsSrlte())
                {
                    /***************************************************************************************
                     * BSR背景搜LTE成功后，指定搜L时，原因值NO RF，如果是SRLTE, 退出状态机后power save LTE；
                     **************************************************************************************/
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_SRCH_NO_RF,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

                    /* Quit the SubFSM */
                    NAS_MSCC_QuitFsmL2();
                }
                /* SVLTE处理 */
                else
                {
                    NAS_MSCC_SndMmcPowerSaveReq();

                    /* change state */
                    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_POWER_SAVE_CNF_REACQ_HRPD);

                    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF,
                                        TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF_LEN);
                }
            }
            break;

        case NAS_MSCC_PIF_PLMN_SELECTION_RESULT_REG_NO_RF:
            {
                /* LTE注册NO RF场景，记录注册失败标志，重回HRPD原系统成功后，需要启动10秒周期性尝试搜索定时器 */
                NAS_MSCC_SetBsrLteNoRfType_BSR(NAS_MSCC_BSR_LTE_NORF_TYPE_ATTACH);

                NAS_MSCC_SndMmcPowerSaveReq();

                /* change state */
                NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_POWER_SAVE_CNF_REACQ_HRPD);

                NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF,
                                    TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF_LEN);
            }
            break;

        default:
            {
                NAS_MSCC_SndMmcPowerSaveReq();

                /* change state */
                NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_POWER_SAVE_CNF_REACQ_HRPD);

                NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF,
                                    TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF_LEN);
            }
            break;
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitMmcPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF);

    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    /* send MSCC_MMC_POWER_SAVE_REQ */
    NAS_MSCC_SndMmcPowerSaveReq();

    /* Switch state to NAS_MSCC_BSR_STA_WAIT_HSD_SYSTEM_ACQUIRE_CNF */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_POWER_SAVE_CNF);

    /* start the relative protective timer relative */
    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF,
                        TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF_LEN);


    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitMmcPlmnSearchCnfTimeExpired_BSR_WaitMmcPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitMmcPlmnSearchCnfTimeExpired_BSR_WaitMmcPlmnSearchCnf ENTERED...");

    NAS_MSCC_SndMmcPowerSaveReq();

    /* change state */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_POWER_SAVE_CNF_REACQ_HRPD);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF,
                        TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcPowerSaveCnf_BSR_WaitMmcPowerSaveReAcqHrpd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MSCC_POWER_SAVE_CNF_STRU       *pstPowerSaveCnf;

    pstPowerSaveCnf = (MMC_MSCC_POWER_SAVE_CNF_STRU*)pstMsg;

    if (NAS_MSCC_PIF_POWER_SAVE_FAILURE == pstPowerSaveCnf->enResult)
    {
        NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvMmcPowerSaveCnf_BSR_WaitMmcPowerSaveReAcqHrpd:power save failure!");

        /* Stop the Protection timer */
        NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF);

        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                              NAS_MSCC_BSR_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }


    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF);

    /* if this proces has been aborted */
    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {

        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* send ID_MSCC_HSD_SYSTEM_ACQUIRE_REQ */
    NAS_MSCC_SndHsdSysAcqReq(MSCC_HSD_ACQUIRE_REASON_OTHER,
                             0,
                             VOS_NULL_PTR);

    /* Set the Camp On flag to FALSE */
    NAS_MSCC_SetHrpdCampOnFlag(VOS_FALSE);

    NAS_MSCC_SetHrpdActiveFlg(VOS_TRUE);

    /* change state */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_SYSTEM_ACQUIRE_CNF_REACQ_HRPD);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_ACQUIRE_CNF,
                        TI_NAS_MSCC_WAIT_HSD_ACQUIRE_CNF_LEN);


    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitMmcPowerSaveReAcqHrpd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* record abort flag */
    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitMmcPowerSaveReAcqHrpdTimeExpired_BSR_WaitMmcPowerSaveReAcqHrpd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitMmcPowerSaveReAcqHrpdTimeExpired_BSR_WaitMmcPowerSaveReAcqHrpd ENTERED...");

    /* wait expired, send result as failure */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvHsdSystemAcquireCnf_BSR_WaitHsdAcquireCnfReAcqHrpd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    HSD_MSCC_SYSTEM_ACQUIRE_CNF_STRU   *pstSysAcqCnf;

    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_ACQUIRE_CNF);

    pstSysAcqCnf = (HSD_MSCC_SYSTEM_ACQUIRE_CNF_STRU *)pstMsg;

    /* 需要上报HRPD无服务 */
    if (NAS_MSCC_PIF_ACQUIRED_RESULT_SUCC != pstSysAcqCnf->enRslt)
    {
        NAS_MSCC_SndMmaHrpdServiceStatusInd(MSCC_MMA_HRPD_SERVICE_STATUS_NO_SERVICE,
                                            VOS_FALSE,
                                            VOS_FALSE,
                                            MSCC_MMA_SESSION_RELEASE_TYPE_BUTT);
    }

    switch (pstSysAcqCnf->enRslt)
    {
        case NAS_MSCC_PIF_ACQUIRED_RESULT_SUCC:
            {
                if (NAS_MSCC_BSR_LTE_NORF_TYPE_ATTACH == NAS_MSCC_GetBsrLteNoRfType_BSR())
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_REG_NO_RF,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NEED_START_RETRY_TIMER);
                }
                else
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
                }
            }
            break;

        case NAS_MSCC_BSR_RESULT_ABORT:
            {
                NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                      NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                      NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
            }
            break;

        case NAS_MSCC_PIF_ACQUIRED_RESULT_REG_NO_RF:
            {
                /* SRLTE处理 */
                NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_REG_NO_RF,
                                      NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                      NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
            }
            break;

        case NAS_MSCC_PIF_ACQUIRED_RESULT_NO_RF:
            {
                NAS_MSCC_SetHrpdNoRfFlg_BSR(VOS_TRUE);

                if (VOS_TRUE == NAS_MSCC_IsSrlte())
                {
                    /* SRLTE处理 */
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_SRCH_NO_RF,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
                }
                else
                {
                    /* SVLTE处理 */
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                          NAS_MSCC_BSR_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
                }
            }
            break;

        default:
            NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                  NAS_MSCC_BSR_NEED_ACQ_SYS,
                                  NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
            break;
    }

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitHsdAcquireCnfReAcqHrpd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_ACQUIRE_CNF);

    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    NAS_MSCC_SndHsdPowerSaveReq();

    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_POWER_SAVE_CNF);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF,
                        TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitHsdAcqCnfReAcqHrpdTimeExpired_BSR_WaitHsdAcquireCnfReAcqHrpd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitHsdAcquireCnfReAcqHrpdTimeExpired_BSR_WaitHsdAcquireCnfReAcqHrpd ENTERED...");

    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcBgSearchCnf_BSR_WaitMmcBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MSCC_BG_SEARCH_CNF_STRU                            *pstBgSearchCnf    = VOS_NULL_PTR;
    NAS_MSCC_MLPL_MSPL_SYS_TYPE_ENUM_UINT8                  enDestBsrSysType;

    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_BG_SEARCH_CNF);

    pstBgSearchCnf         =  (MMC_MSCC_BG_SEARCH_CNF_STRU *)pstMsg;

    /* bg search cnf with fail, send message to L1 FSM with fail */
    if ((MSCC_MMC_BG_SRCH_RESULT_FAIL == pstBgSearchCnf->enRslt)
     || (MSCC_MMC_BG_SRCH_RESULT_BUTT == pstBgSearchCnf->enRslt))
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }
    else if (MSCC_MMC_BG_SRCH_RESULT_ABORT == pstBgSearchCnf->enRslt)
    {
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }
    else
    {
        if (0 == pstBgSearchCnf->ucHrpdSysNum)
        {
            NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                  NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                  NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

            /* Quit the Sub FSM */
            NAS_MSCC_QuitFsmL2();

            return VOS_TRUE;
        }

        enDestBsrSysType = NAS_MSCC_GetDestBsrSysType_BSR();

        /* send ID_MSCC_HSD_QRY_HRPD_SYS_INFO_REQ */
        NAS_MSCC_SndHsdQryHrpdSysInfoReq(enDestBsrSysType, pstBgSearchCnf->ucHrpdSysNum, pstBgSearchCnf->astHrpdSysList);

        /* Switch state to NAS_MSCC_BSR_STA_WAIT_HSD_QRY_HRPD_SYS_INFO_CNF */
        NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_QRY_HRPD_SYS_INFO_CNF);

        NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_QRY_HRPD_SYS_INFO_CNF,
                            TI_NAS_MSCC_WAIT_HSD_QRY_HRPD_SYS_INFO_CNF_LEN);

        return VOS_TRUE;
    }
}


VOS_UINT32 NAS_MSCC_RcvMmcCoverageAreaInd_BSR_WaitMmcBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* stop the prptectove timer relative */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_BG_SEARCH_CNF);

    /* Set the Camp On flag to FALSE */
    NAS_MSCC_Set3gppCampOnFlag(VOS_FALSE);

    /* send a bg search result as fail to main FSM */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsmMsg_BSR_WaitMmcBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* stop the prptectove timer relative */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_BG_SEARCH_CNF);

    /* record the abort flag */
    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    /* send MSCC_MMC_STOP_BG_SEARCH_HRPD_REQ */
    NAS_MSCC_SndMmcStopBgSearchReq();

    /* Switch state to NAS_MSCC_BSR_STA_WAIT_MMC_STOP_BG_SEARCH_CNF */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_STOP_BG_SEARCH_CNF);

    /* start the relative protective timer relative */
    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_STOP_BG_SEARCH_CNF,
                        TI_NAS_MSCC_WAIT_MMC_STOP_BG_SEARCH_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitMmcBgSearchCnfExpired_BSR_WaitMmcBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitMmcBgSearchCnfExpired_BSR_WaitMmcBgSearchCnf ENTERED");

    /* send MSCC_MMC_STOP_BG_SEARCH_HRPD_REQ */
    NAS_MSCC_SndMmcStopBgSearchReq();

    /* Switch state to NAS_MSCC_BSR_STA_WAIT_MMC_STOP_BG_SEARCH_CNF */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_STOP_BG_SEARCH_CNF);

    /* start the relative protective timer relative */
    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_STOP_BG_SEARCH_CNF,
                        TI_NAS_MSCC_WAIT_MMC_STOP_BG_SEARCH_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcStopBgSearchCnf_BSR_WaitMmcStopBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* stop the prptectove timer relative */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_STOP_BG_SEARCH_CNF);

    /* if this proces has been aborted */
    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        /* if bg search timer out, send a bg search result as fail to main FSM,need start retry timer */
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* if bg search timer out, send a bg search result as fail to main FSM, need not start retry timer */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcCoverageAreaInd_BSR_WaitMmcStopBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* stop the prptectove timer relative */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_STOP_BG_SEARCH_CNF);

    /* Set the Camp On flag to FALSE */
    NAS_MSCC_Set3gppCampOnFlag(VOS_FALSE);

    /* send a bg search result as fail to main FSM */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsmMsg_BSR_WaitMmcStopBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* record the abort flag */
    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitMmcStopBgSearchCnfExpired_BSR_WaitMmcStopBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitMmcStopBgSearchCnfExpired_BSR_WaitMmcStopBgSearchCnf ENTERED");

    /* if this proces has been aborted */
    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        /* if bg search timer out, send a bg search result as abort to main FSM,need start retry timer */
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* send a bg search result as fail to main FSM */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvHsdQryHrpdSysInfoCnf_BSR_WaitHsdQryHrpdSysInfoCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    HSD_MSCC_QRY_HRPD_SYS_INFO_CNF_STRU                    *pstQryCnfMsg      = VOS_NULL_PTR;
    NAS_MSCC_MSPL_SYS_PRI_CLASS_ENUM_UINT8                  enDestBsrSysPriClass;      /* 需要发起BSR的系统的pri class */
    NAS_MSCC_HIGH_PRIORITY_SYSTEM_INFO_STRU                *pstHighPriSysInfo;

    /* stop the prptectove timer relative */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_QRY_HRPD_SYS_INFO_CNF);

    pstQryCnfMsg         = (HSD_MSCC_QRY_HRPD_SYS_INFO_CNF_STRU *)pstMsg;

    pstHighPriSysInfo    = NAS_MSCC_GetHrpdHighPrioSysInfo_BSR();
    enDestBsrSysPriClass = NAS_MSCC_GetDestBsrSysPriClass_BSR();

    NAS_MSCC_GetHighPriSysInfoInHrpdSysInfoListWithPriClass(pstQryCnfMsg->ucHrpdSysNum,
                                                            pstQryCnfMsg->astHrpdSysInfoList,
                                                            enDestBsrSysPriClass,
                                                            pstHighPriSysInfo);

    if (0 == pstHighPriSysInfo->ucFreqNum)
    {
        /* send a bg search result as fail to main FSM */
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* send MSCC_MMC_POWER_SAVE_REQ */
    NAS_MSCC_SndMmcPowerSaveReq();

    /* Switch state to NAS_MSCC_BSR_STA_WAIT_HSD_SYSTEM_ACQUIRE_CNF */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_POWER_SAVE_CNF);

    /* start the relative protective timer relative */
    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF,
                        TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsmMsg_BSR_WaitHsdQryHrpdSysInfoCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* stop the prptectove timer relative */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_QRY_HRPD_SYS_INFO_CNF);

    /* send a bg search result as abort to main FSM */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitHsdQryHrpdSysInfoCnfExpired_WaitHsdQryHrpdSysInfoCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitHsdQryHrpdSysInfoCnfExpired_WaitHsdQryHrpdSysInfoCnf ENTERED");

    /* send a bg search result as fail to main FSM */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcPowerSaveCnf_BSR_WaitMmcPowerSaveCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MSCC_HIGH_PRIORITY_SYSTEM_INFO_STRU                *pstHighPriSysInfo = VOS_NULL_PTR;

    MMC_MSCC_POWER_SAVE_CNF_STRU       *pstPowerSaveCnf;

    pstPowerSaveCnf = (MMC_MSCC_POWER_SAVE_CNF_STRU*)pstMsg;

    if (NAS_MSCC_PIF_POWER_SAVE_FAILURE == pstPowerSaveCnf->enResult)
    {
        NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvMmcPowerSaveCnf_BSR_WaitMmcPowerSaveCnf:power save failure!");

        /* stop the prptectove timer relative */
        NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF);

        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                              NAS_MSCC_BSR_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* stop the prptectove timer relative */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF);

    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        /* send a bg search result as abort to main FSM */
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    pstHighPriSysInfo = NAS_MSCC_GetHrpdHighPrioSysInfo_BSR();

    /* send MSCC_HSD_SYSTEM_ACQUIRE_REQ */
    NAS_MSCC_SndHsdSysAcqReq(MSCC_HSD_ACQUIRE_REASON_HIGH_PRIORITY,
                             pstHighPriSysInfo->ucFreqNum,
                             (VOS_VOID*)pstHighPriSysInfo->astFreqInfo);

    /* Set the Camp On flag to FALSE */
    NAS_MSCC_SetHrpdCampOnFlag(VOS_FALSE);

    NAS_MSCC_SetHrpdActiveFlg(VOS_TRUE);

    /* Switch state to NAS_MSCC_BSR_STA_WAIT_HSD_SYSTEM_ACQUIRE_CNF */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_SYSTEM_ACQUIRE_CNF);

    /* start the relative protective timer relative */
    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_ACQUIRE_CNF,
                        TI_NAS_MSCC_WAIT_HSD_ACQUIRE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsmMsg_BSR_WaitMmcPowerSaveCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* record the abort flag */
    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitMmcPowerSaveCnfExpired_BSR_WaitMmcPowerSaveCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitMmcPowerSaveCnfExpired_BSR_WaitMmcPowerSaveCnf ENTERED");

    /* if this proces has been aborted */
    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        /* if bg search timer out, send a bg search result as abort to main FSM */
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* send a bg search result as fail to main FSM */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MSCC_RcvHsdSystemAcquireCnf_BSR_WaitHsdSystemAcquireCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    HSD_MSCC_SYSTEM_ACQUIRE_CNF_STRU   *pstSysAcqCnf;

    pstSysAcqCnf = (HSD_MSCC_SYSTEM_ACQUIRE_CNF_STRU *)pstMsg;

    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_ACQUIRE_CNF);

    /* 需要上报HRPD无服务 */
    if (NAS_MSCC_PIF_ACQUIRED_RESULT_SUCC != pstSysAcqCnf->enRslt)
    {
        NAS_MSCC_SndMmaHrpdServiceStatusInd(MSCC_MMA_HRPD_SERVICE_STATUS_NO_SERVICE,
                                            VOS_FALSE,
                                            VOS_FALSE,
                                            MSCC_MMA_SESSION_RELEASE_TYPE_BUTT);
    }

    switch (pstSysAcqCnf->enRslt)
    {
        case NAS_MSCC_PIF_ACQUIRED_RESULT_SUCC:
            {
                NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_SUCCESS,
                                      NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                      NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

                NAS_MSCC_QuitFsmL2();
            }
            break;

        case NAS_MSCC_PIF_ACQUIRED_RESULT_ABORTED:
            {
                NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                      NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                      NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

                NAS_MSCC_QuitFsmL2();
            }
            break;

        case NAS_MSCC_PIF_ACQUIRED_RESULT_NO_RF:
            {
                NAS_MSCC_SetHrpdNoRfFlg_BSR(VOS_TRUE);

                /* SRLTE处理 */
                if (VOS_TRUE == NAS_MSCC_IsSrlte())
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_SRCH_NO_RF,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

                    NAS_MSCC_QuitFsmL2();
                }
                /* SVLTE处理 */
                else
                {
                    NAS_MSCC_SndHsdPowerSaveReq();

                    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_POWER_SAVE_CNF_REACQ_LTE);

                    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF,
                                        TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF_LEN);
                }
            }
            break;

        case NAS_MSCC_PIF_ACQUIRED_RESULT_REG_NO_RF:
            {
                /* 记录HRPD注册失败原因为NO RF */
                NAS_MSCC_SetBsrHrpdNoRfFlag_BSR(NAS_MSCC_BSR_HRPD_NORF_TYPE_NEG);

                NAS_MSCC_SndHsdPowerSaveReq();

                NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_POWER_SAVE_CNF_REACQ_LTE);

                NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF,
                                    TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF_LEN);
            }
            break;

        case NAS_MSCC_PIF_ACQUIRED_RESULT_FAIL:
        default:
            {
                NAS_MSCC_SndHsdPowerSaveReq();

                NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_POWER_SAVE_CNF_REACQ_LTE);

                NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF,
                                    TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF_LEN);
            }
            break;
    }

    return VOS_TRUE;
}

VOS_UINT32 NAS_MSCC_RcvTiWaitHsdSystemAcquireCnfExpired_BSR_WaitHsdSystemAcquireCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitHsdSystemAcquireCnfExpired_BSR_WaitHsdSystemAcquireCnf ENTERED");

    NAS_MSCC_SndHsdPowerSaveReq();

    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_POWER_SAVE_CNF_REACQ_LTE);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF,
                        TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitHsdSystemAcquireCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_ACQUIRE_CNF);

    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    NAS_MSCC_SndHsdPowerSaveReq();

    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_HSD_POWER_SAVE_CNF);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF,
                        TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvHsdPowerSaveCnf_BSR_WaitHsdPowerSaveCnfReAcqLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_HSD_POWER_SAVE_CNF);

    /* if this proces has been aborted */
    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        /* send a bg search result as abort to main FSM */
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* send ID_MSCC_MMC_PLMN_SEARCH_REQ */
    NAS_MSCC_SndMmcPlmnSearchReq(NAS_MSCC_PIF_ACQ_REASON_OTHER,
                                 NAS_MSCC_PIF_PLMN_SEARCH_SPEC,
                                 VOS_NULL_PTR,
                                 VOS_NULL_PTR);

    NAS_MSCC_Set3gppCampOnFlag(VOS_FALSE);

    NAS_MSCC_Set3gppActiveFlg(VOS_TRUE);

    /* change state */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_PLMN_SEARCH_CNF_REACQ_LTE);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF,
                        TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitHsdPowerSaveCnfReAcqLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* record the abort flag */
    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitHsdPowerSaveCnfExpired_BSR_WaitHsdPowerSaveCnfReAcqLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitHsdPowerSaveCnfExpired_BSR_WaitHsdPowerSaveCnfReAcqLte ENTERED");

    /* if this proces has been aborted */
    if (VOS_TRUE == NAS_MSCC_GetAbortFlag_BSR())
    {
        /* if bg search timer out, send a bg search result as abort to main FSM */
        NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_ABORT,
                              NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                              NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

        /* Quit the SubFSM */
        NAS_MSCC_QuitFsmL2();

        return VOS_TRUE;
    }

    /* send a bg search result as fail to main FSM */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcPlmnSeachCnf_BSR_WaitMmcPlmnSearchCnfReAcqLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MSCC_PLMN_SELECTION_RLST_IND_STRU                  *pstPlmnSelRlstInd = VOS_NULL_PTR;

    pstPlmnSelRlstInd = (MMC_MSCC_PLMN_SELECTION_RLST_IND_STRU *)pstMsg;

    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF);

    switch (pstPlmnSelRlstInd->enResult)
    {
        case NAS_MSCC_PIF_PLMN_SELECTION_RESULT_NORMAL_SERVICE:
            {
                /* HRPD协商NO RF导致的重回原系统失败，需要启动尝试定时器 */
                if (NAS_MSCC_BSR_HRPD_NORF_TYPE_NEG == NAS_MSCC_GetBrsHrpdNoRfType_BSR())
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_REG_NO_RF,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NEED_START_RETRY_TIMER);
                }
                else
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
                }
            }
            break;

        case NAS_MSCC_PIF_PLMN_SELECTION_RESULT_INTERRUPT:
            {
                NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                      NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                      NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
            }
            break;

        case NAS_MSCC_PIF_PLMN_SELECTION_RESULT_NO_RF:
            {
                /* SRLTE处理 */
                if (VOS_TRUE == NAS_MSCC_IsSrlte())
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_SRCH_NO_RF,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
                }
                /* SVLTE处理 */
                else
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                          NAS_MSCC_BSR_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
                }
            }
            break;

        case NAS_MSCC_PIF_PLMN_SELECTION_RESULT_REG_NO_RF:
            {
                /* SRLTE处理 */
                if (VOS_TRUE == NAS_MSCC_IsSrlte())
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_REG_NO_RF,
                                          NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
                }
                /* SVLTE处理 */
                else
                {
                    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                          NAS_MSCC_BSR_NEED_ACQ_SYS,
                                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
                }
            }
            break;

        default:
            {
                NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                                      NAS_MSCC_BSR_NEED_ACQ_SYS,
                                      NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);
            }
            break;
    }

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMsccAbortFsm_BSR_WaitMmcPlmnSearchCnfReAcqLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Stop the Protection timer */
    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF);

    NAS_MSCC_SetAbortFlag_BSR(VOS_TRUE);

    NAS_MSCC_SndMmcPowerSaveReq();

    /* change state */
    NAS_MSCC_SetCurrFsmState(NAS_MSCC_BSR_STA_WAIT_MMC_POWER_SAVE_CNF);

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF,
                        TI_NAS_MSCC_WAIT_MMC_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvTiWaitMmcPlmnSeachCnfExpired_BSR_WaitMmcPlmnSearchCnfReAcqLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(UEPS_PID_MSCC, "NAS_MSCC_RcvTiWaitMmcPlmnSeachCnfExpired_BSR_WaitMmcPlmnSearchCnfReAcqLte ENTERED");

    /* send a bg search result as abort to main FSM */
    NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_FAILURE,
                          NAS_MSCC_BSR_NEED_ACQ_SYS,
                          NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

    /* Quit the SubFSM */
    NAS_MSCC_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MSCC_RcvMmcIntersysStartInd_BSR_WaitMmcPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulReaminTimerLen;
    VOS_UINT32                                              ulInterSysLen;
    MMC_MSCC_INTERSYS_START_IND_STRU                       *pstInterSysStartInd;

    pstInterSysStartInd = (MMC_MSCC_INTERSYS_START_IND_STRU *)pstMsg;

    NAS_MSCC_GetRemainTimerLen(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF, &ulReaminTimerLen);

    /* 重选 */
    if (NAS_MSCC_PIF_INTERSYS_CAUSE_RESELECT == pstInterSysStartInd->enInterSysCause)
    {
        ulInterSysLen = TI_NAS_MSCC_WAIT_CL_INTERSYS_END_IND_L2C_RESEL_LEN;
    }
    /* 重定向 */
    else
    {
        ulInterSysLen = TI_NAS_MSCC_WAIT_CL_INTERSYS_END_IND_L2C_REDIR_LEN;
    }

    NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF);

    /* 延长等待MMC的搜网回复定时器 */
    ulInterSysLen += ulReaminTimerLen;

    NAS_MSCC_StartTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF, ulInterSysLen);

    NAS_MSCC_InitFsmL2(NAS_MSCC_FSM_CL_INTERSYS);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MSCC_RcvMsccCLInterSysRsltCnf_BSR_WaitMmcPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MSCC_MSCC_CL_INTERSYS_RSLT_CNF_STRU                *pstClInterSysRslt;

    pstClInterSysRslt = (NAS_MSCC_MSCC_CL_INTERSYS_RSLT_CNF_STRU *)pstMsg;

    if (NAS_MSCC_CL_INTERSYS_RESULT_SUCC == pstClInterSysRslt->enRslt)
    {
        /* LTE异系统变换到HRPD退出状态机 */
        if (VOS_TRUE == NAS_MSCC_GetHrpdActiveFlg())
        {
            NAS_MSCC_StopTimer(TI_NAS_MSCC_WAIT_MMC_PLMN_SEARCH_CNF);

            /* send a bg search result as abort to main FSM */
            NAS_MSCC_SndBsrResult(NAS_MSCC_BSR_RESULT_L2C,
                                  NAS_MSCC_BSR_NOT_NEED_ACQ_SYS,
                                  NAS_MSCC_BSR_NOT_NEED_START_RETRY_TIMER);

            /* Quit the SubFSM */
            NAS_MSCC_QuitFsmL2();
        }
    }

    return VOS_TRUE;
}


VOS_VOID NAS_MSCC_GetBsrReqPlmnList_BSR(
    VOS_UINT8                         *pucPlmnNum,
    NAS_MSCC_PIF_PLMN_ID_STRU         *pstPlmnListEntry
)
{
    NAS_MSCC_PIF_SIM_OPERPLMN_INFO_STRU                    *pstOPlmnInfo;
    NAS_MSCC_PIF_SIM_EHPLMN_INFO_STRU                      *pstEhplmnInfo;
    VOS_UINT32                                              i;
    VOS_UINT8                                               ucIndex;
    VOS_UINT32                                              ulCurMcc;

    pstEhplmnInfo     = NAS_MSCC_GetEhPlmnInfoAddr();
    pstOPlmnInfo      = NAS_MSCC_GetOperPlmnInfoAddr();

    if (VOS_TRUE == NAS_MSCC_Get1xCampOnFlag())
    {
        ulCurMcc = NAS_MSCC_Get1xSysInfoAddr()->ulMcc;
    }
    else if (VOS_TRUE == NAS_MSCC_GetHrpdCampOnFlag())
    {
        ulCurMcc = NAS_MSCC_GetHrpdSysInfoAddr()->ulMcc;
    }
    else
    {
        return ;
    }

    for (i = 0, ucIndex = 0; i < pstEhplmnInfo->ucEhPlmnNum; i ++)
    {
        if (ulCurMcc == pstEhplmnInfo->astEhPlmnInfo[i].stPlmnId.ulMcc)
        {
            pstPlmnListEntry[ucIndex] = pstEhplmnInfo->astEhPlmnInfo[i].stPlmnId;

            ucIndex++;

            if (ucIndex >= NAS_MSCC_SYS_ACQ_CUSTOM_PLMN_NUM)
            {
                *pucPlmnNum = ucIndex;

                return;
            }
        }
    }

    /* 如果目标系统的PRI CLASS是HOME,则只加HOME网络,不加OPLMN网络;否则对于
       HOME+PREF或ANY的,都将HOME+OPLMN加到BSR请求的PLMN列表中去 */
    if (NAS_MSCC_MSPL_SYS_PRI_CLASS_HOME == NAS_MSCC_GetDestBsrSysPriClass_BSR())
    {
        *pucPlmnNum = ucIndex;

        return;
    }

    for (i = 0; i < pstOPlmnInfo->usOperPlmnNum; i ++)
    {
        if (ulCurMcc == pstOPlmnInfo->astOperPlmnInfo[i].stPlmnId.ulMcc)
        {
            pstPlmnListEntry[ucIndex] = pstOPlmnInfo->astOperPlmnInfo[i].stPlmnId;

            ucIndex++;

            if (ucIndex >= NAS_MSCC_SYS_ACQ_CUSTOM_PLMN_NUM)
            {
                *pucPlmnNum = ucIndex;

                return;
            }
        }
    }

    *pucPlmnNum = ucIndex;

    return;
}


#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

