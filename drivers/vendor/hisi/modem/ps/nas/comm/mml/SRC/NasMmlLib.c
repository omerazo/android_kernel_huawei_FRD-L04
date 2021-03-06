

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "PsLib.h"

#include "PsCommonDef.h"
#include "NasComm.h"
#include "NasMmlCtx.h"
#include "UsimPsInterface.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "MmcLmmInterface.h"
#include "MmLmmInterface.h"
#endif
#include "NasMmlLib.h"
#include "GmmExt.h"
#include "NasMmcSndLmm.h"

#include "zlib.h"
#include "NVIM_Interface.h"
#include "NasUsimmApi.h"
#include "NasMmlCtx.h"

#include "NasMmcSndOm.h"

#include "NasMmcFsmPlmnSelection.h"

#include "MtcInterface.h"

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

/*****************************************************************************
  2 常量定义
*****************************************************************************/

/*lint -e767 修改人:罗建 107747;检视人:孙少华65952;原因:Log打印*/
#define THIS_FILE_ID PS_FILE_ID_NASMML_LIB_C
/*lint +e767 修改人:罗建 107747;检视人:sunshaohua*/

/*****************************************************************************
  3 类型定义
*****************************************************************************/

/*****************************************************************************
  4 函数声明
*****************************************************************************/

/*****************************************************************************
  5 变量定义
*****************************************************************************/
VOS_UINT32 g_aulNasMmlNortAmericanMcc[]       = {0x00000103, 0x00010103, 0x00020103, 0x00030103, 0x00040103, 0x00050103, 0x00060103};
VOS_UINT32 g_aulNasMmlBritainMcc[]            = {0x00040302, 0x00050302};
VOS_UINT32 g_aulNasMmlIndianMcc[]             = {0x00040004, 0x00050004, 0x00060004};
VOS_UINT32 g_aulNasMmlJapanMcc[]              = {0x00000404, 0x00010404};
VOS_UINT32 g_aulNasMmlSudanMcc[]              = {0x00040306, 0x00090506};
VOS_UINT32 g_aulNasMmlDefaultSingleMcc[]      = {0xFFFFFFFF};

NAS_MML_MCC_DESC_STRU  g_astNasMmlMccList[] =
{
    {sizeof(g_aulNasMmlNortAmericanMcc)/sizeof(VOS_UINT32), g_aulNasMmlNortAmericanMcc},
    {sizeof(g_aulNasMmlBritainMcc)/sizeof(VOS_UINT32), g_aulNasMmlBritainMcc},
    {sizeof(g_aulNasMmlIndianMcc)/sizeof(VOS_UINT32), g_aulNasMmlIndianMcc},
    {sizeof(g_aulNasMmlJapanMcc)/sizeof(VOS_UINT32), g_aulNasMmlJapanMcc},
    {sizeof(g_aulNasMmlSudanMcc)/sizeof(VOS_UINT32), g_aulNasMmlSudanMcc},
    {sizeof(g_aulNasMmlDefaultSingleMcc)/sizeof(VOS_UINT32), g_aulNasMmlDefaultSingleMcc},
};

/* 复位分析需要忽略的消息列表 */
NAS_MML_DISCARD_MSG_STRU g_stNasMmlDiscardMsgTbl[]=
{
    /* GUNAS收到的消息 */
    {UEPS_PID_GAS,                      WUEPS_PID_MMC,                          GRRMM_SCELL_MEAS_IND},
    {WUEPS_PID_WRR,                     WUEPS_PID_MMC,                          RRMM_AT_MSG_IND},
    {WUEPS_PID_WRR,                     WUEPS_PID_MMC,                          RRMM_AT_MSG_CNF},
    {UEPS_PID_GAS,                      WUEPS_PID_MMC,                          RRMM_USING_FREQ_IND},
    {WUEPS_PID_WRR,                     WUEPS_PID_MMC,                          RRMM_USING_FREQ_IND},
    {WUEPS_PID_WRR,                     WUEPS_PID_MMC,                          RRMM_SEARCHED_PLMN_INFO_IND},
    {WUEPS_PID_RABM,                    WUEPS_PID_GMM,                          ID_RABM_GMM_MML_PROC_STATUS_QRY_REQ},
    {PS_PID_MM,                         WUEPS_PID_MMC,                          ID_LMM_MMC_STATUS_IND},
    {PS_PID_MM,                         WUEPS_PID_MMC,                          ID_LMM_MMC_TIMER_STATE_NOTIFY},
    {PS_PID_MM,                         WUEPS_PID_MMC,                          ID_LMM_MMC_NOT_CAMP_ON_IND},
    {PS_PID_MM,                         WUEPS_PID_MMC,                          ID_LMM_MMC_CELL_SIGN_REPORT_IND},
    {WUEPS_PID_WRR,                     WUEPS_PID_MMC,                          RRMM_BG_PLMN_SEARCH_STATUS_IND},

    /* GUNAS发送的消息 */
    {WUEPS_PID_MM,                      WUEPS_PID_WRR,                          RRMM_NAS_INFO_CHANGE_REQ},
    {WUEPS_PID_GMM,                     WUEPS_PID_WRR,                          RRMM_NAS_INFO_CHANGE_REQ},
    {WUEPS_PID_MM,                      UEPS_PID_GAS,                           GRRMM_NAS_INFO_CHANGE_REQ},
    {WUEPS_PID_GMM,                     UEPS_PID_GAS,                           GRRMM_NAS_INFO_CHANGE_REQ},
    {WUEPS_PID_MMC,                     WUEPS_PID_WRR,                          RRMM_AT_MSG_REQ},
    {WUEPS_PID_MMC,                     UEPS_PID_GAS,                           GRRMM_AT_MSG_REQ},
    {WUEPS_PID_MMC,                     WUEPS_PID_WRR,                          RRMM_NAS_COMM_INFO_CHANGE_REQ},
    {WUEPS_PID_MMC,                     UEPS_PID_GAS,                           RRMM_NAS_COMM_INFO_CHANGE_REQ},
    {WUEPS_PID_GMM,                     WUEPS_PID_RABM,                         ID_GMM_RABM_MML_PROC_STATUS_QRY_CNF},
    {WUEPS_PID_GMM,                     WUEPS_PID_WRR,                          RRMM_BEGIN_SESSION_NOTIFY},
    {WUEPS_PID_GMM,                     WUEPS_PID_WRR,                          RRMM_END_SESSION_NOTIFY},
    {WUEPS_PID_GMM,                     TPS_PID_RRC,                            RRMM_BEGIN_SESSION_NOTIFY},
    {WUEPS_PID_GMM,                     TPS_PID_RRC,                            RRMM_END_SESSION_NOTIFY},
    {WUEPS_PID_GMM,                     UEPS_PID_GAS,                           RRMM_BEGIN_SESSION_NOTIFY},
    {WUEPS_PID_GMM,                     UEPS_PID_GAS,                           RRMM_END_SESSION_NOTIFY},
    {WUEPS_PID_MM,                      WUEPS_PID_WRR,                          RRMM_BEGIN_SESSION_NOTIFY},
    {WUEPS_PID_MM,                      WUEPS_PID_WRR,                          RRMM_END_SESSION_NOTIFY},
    {WUEPS_PID_MM,                      TPS_PID_RRC,                            RRMM_BEGIN_SESSION_NOTIFY},
    {WUEPS_PID_MM,                      TPS_PID_RRC,                            RRMM_END_SESSION_NOTIFY},
    {WUEPS_PID_MM,                      UEPS_PID_GAS,                           RRMM_BEGIN_SESSION_NOTIFY},
    {WUEPS_PID_MM,                      UEPS_PID_GAS,                           RRMM_END_SESSION_NOTIFY},
    {WUEPS_PID_MMC,                     PS_PID_MM,                              ID_MMC_LMM_OTHER_MODEM_INFO_NOTIFY},
    {WUEPS_PID_MMC,                     PS_PID_MM,                              ID_MMC_LMM_CELL_SIGN_REPORT_NOTIFY},
};

/*****************************************************************************
  6 函数定义
*****************************************************************************/

NAS_MML_MCC_DESC_STRU* NAS_MML_GetEquivalentMccList(VOS_UINT32 ulMcc)
{
    VOS_UINT32                i;
    VOS_UINT32                j;
    VOS_UINT32                ulArraySize;

    ulArraySize = sizeof(g_astNasMmlMccList)/sizeof(NAS_MML_MCC_DESC_STRU);

    /* 将数组最后一个结构体的国家码赋值为ulMcc，从而使得如下查找始终可以成功 */
    g_astNasMmlMccList[ulArraySize - 1].pulMccList[0] = ulMcc;

    for (i = 0; i < ulArraySize; i++)
    {
        for (j = 0; j < g_astNasMmlMccList[i].ulMccNum; j++)
        {
            if (ulMcc == g_astNasMmlMccList[i].pulMccList[j])
            {
                return &(g_astNasMmlMccList[i]);
            }
        }
    }

    return VOS_NULL_PTR;
}


VOS_UINT32 NAS_MML_IsSpecMccInEquMccList(VOS_UINT32 ulMcc, NAS_MML_MCC_DESC_STRU *pstMccList)
{
    VOS_UINT32                ulIndex;

    for (ulIndex = 0; ulIndex < pstMccList->ulMccNum; ulIndex++)
    {
        if (ulMcc == pstMccList->pulMccList[ulIndex])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
/*lint -save -e958 */


VOS_UINT32  NAS_MML_GetCsServiceExistFlg( VOS_VOID )
{
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus = VOS_NULL_PTR;

    pstConnStatus   = NAS_MML_GetConnStatus();

    /* CS域有缓存业务存在 */
    if ( VOS_TRUE == pstConnStatus->ucCsServiceBufferFlg)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetCsServiceExistFlg: pstConnStatus->ucCsServiceBufferFlg is true");

        return VOS_TRUE;
    }

    /* CS域有当前业务存在 */
    if ( VOS_TRUE == pstConnStatus->ucCsServiceConnStatusFlg)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetCsServiceExistFlg: pstConnStatus->ucCsServiceConnStatusFlg is true");

        return VOS_TRUE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetCsServiceExistFlg: is false");

    return VOS_FALSE;
}


VOS_UINT32  NAS_MML_GetPsServiceExistFlg( VOS_VOID )
{
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus;

    pstConnStatus     = NAS_MML_GetConnStatus();


    /* L下的判断规则完全依赖于LMM上报的链路状态 */
#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        /* PS域有当前业务存在 */
        if ( VOS_TRUE == pstConnStatus->ucEpsServiceConnStatusFlg)
        {
            return VOS_TRUE;
        }

        return VOS_FALSE;
    }
#endif

    /* GU下的判断规则在需要根据PDP激活状态和RRC链路状态 */

    /* PS域有缓存业务存在,此标志暂时不用更新,主要原因为当前MMC搜网不需要判断GMM的缓存业务 */
    if ( VOS_TRUE == pstConnStatus->ucPsServiceBufferFlg)
    {
        return VOS_TRUE;
    }


    /* GSM下当前PDP状态存在且TBF链路存在认为当前PS有业务存在 */
    if ((NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
     && (VOS_TRUE == pstConnStatus->ucPdpStatusFlg)
     && (VOS_TRUE == pstConnStatus->ucPsTbfStatusFlg))
    {
        return VOS_TRUE;
    }

    /* W下当前PDP状态存在时有PS信令连接和RRC连接则认为存在PS业务 */
    if ((NAS_MML_NET_RAT_TYPE_WCDMA == NAS_MML_GetCurrNetRatType())
     && (VOS_TRUE == pstConnStatus->ucPdpStatusFlg)
     && (VOS_TRUE == pstConnStatus->ucRrcStatusFlg))
    {
        return VOS_TRUE;
    }

    /* PS域触发的TC环回业务存在 */
    if ( VOS_TRUE == pstConnStatus->ucPsTcServiceFlg )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsNorthAmericanMcc(
    VOS_UINT32                          ulMcc
)
{
    if ((NAS_MML_PCS1900_MCC_LOW_VALUE == (ulMcc & NAS_MML_MCC_LOW_VALUE_MASK))
     && (NAS_MML_PCS1900_MCC_HIGH_VALUE_RANGE >= (ulMcc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK)))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsIndianMcc(
    VOS_UINT32                          ulMcc
)
{
    /* 404-406为印度国家码 */
    if ((NAS_MML_INDIA_MCC_LOW_VALUE == (ulMcc & NAS_MML_MCC_LOW_VALUE_MASK))
     && (NAS_MML_INDIA_MCC_LOW_VALUE_RANGE <= (ulMcc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK))
     && (NAS_MML_INDIA_MCC_HIGH_VALUE_RANGE >= (ulMcc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK)))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MML_SetBitValueToBuffer(
    VOS_UINT8                          *pucDataBuf,
    VOS_UINT32                          ulBitNo,
    VOS_UINT8                           ucValue
)
{
    VOS_UINT32                          ulOffset;
    VOS_UINT8                           ucBit;

    if (0 == ulBitNo)
    {
        return;
    }

    ulOffset = (ulBitNo - 1) / NAS_MML_OCTET_EIGHT_BITS;

    ucBit    = (VOS_UINT8)((ulBitNo - 1)% NAS_MML_OCTET_EIGHT_BITS);

    if (0 != ucValue)
    {
        /*lint -e701 */
        pucDataBuf[ulOffset] |= (VOS_UINT8)(NAS_MML_OCTET_MOVE_ONE_BITS << ucBit);
        /*lint +e701 */
    }
    else
    {
        /*lint -e701 */
        pucDataBuf[ulOffset] &= (~(VOS_UINT8)(NAS_MML_OCTET_MOVE_ONE_BITS << ucBit));
        /*lint +e701 */
    }

    return;
}


VOS_UINT8 NAS_MML_GetBitValueFromBuffer(
    VOS_UINT8                          *pucDataBuf,
    VOS_UINT32                          ulBitNo
)
{
    VOS_UINT32                          ulOffset;
    VOS_UINT8                           ucBit;

    if (0 == ulBitNo)
    {
        return 0;
    }

    ulOffset = (ulBitNo - 1)/ NAS_MML_OCTET_EIGHT_BITS;

    ucBit    = (VOS_UINT8)((ulBitNo - 1) % NAS_MML_OCTET_EIGHT_BITS);

    return (pucDataBuf[ulOffset] >> ucBit)& 0x1;
}


VOS_UINT8 NAS_MML_IsUeSupportImsVoice(VOS_VOID)
{
    VOS_UINT8                                               ucVoiceOnIms;
    NAS_MML_VOICE_DOMAIN_PREFERENCE_ENUM_UINT8              enVoiceDomainPreference;


    enVoiceDomainPreference = NAS_MML_GetVoiceDomainPreference();
    ucVoiceOnIms    = NAS_MML_GetVoiceCallOnImsSupportFlag();

    /* 总控开关未打开时，认为UE不支持IMS */
    if (VOS_FALSE == NAS_MML_GetLteImsSupportFlg())
    {
        return VOS_FALSE;
    }

    /* 支持IMS 语音且voice domain nv配置不是cs only则返回VOS_TRUE */
    if ((VOS_TRUE == ucVoiceOnIms)
     && (NAS_MML_CS_VOICE_ONLY != enVoiceDomainPreference))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsPlmnMccBelongToTheSameCountry(
    VOS_UINT32                          ulSrcMcc,
    VOS_UINT32                          ulDestMcc
)
{
    NAS_MML_MCC_DESC_STRU              *pstEquMccList = VOS_NULL_PTR;
    VOS_UINT32                          i;

    if (ulSrcMcc == ulDestMcc)
    {
        return VOS_TRUE;
    }

    pstEquMccList = NAS_MML_GetEquivalentMccList(ulSrcMcc);

    if (VOS_NULL_PTR == pstEquMccList)
    {
        return VOS_FALSE;
    }

    for (i = 0; i < pstEquMccList->ulMccNum; i++)
    {
        if (ulDestMcc == pstEquMccList->pulMccList[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_ComparePlmnMcc(
    VOS_UINT32                          ulSrcMcc,
    VOS_UINT32                          ulDestMcc
)
{
    VOS_UINT32                          ulSrcMccUsaMccFlag;
    VOS_UINT32                          ulDestMccUsaMccFlag;
    VOS_UINT32                          ulSrcMccIndianMccFlag;
    VOS_UINT32                          ulDestMccIndianMccFlag;

    /* 23122_CR0207R4_(Rel-11)_C1-113664-revc1-113621-revc1-113196-revc1-112708-revc1-112635 23122 1.2章节描述如下:
    Country: A country is identified by a single MCC value, with the exception that MCC
    values 310 through 316 identify a single country (USA) and MCC values 404 through
    406 identify a single country (India). pstBcchPLmnId属于 NA PCS1900,北美的国家码在310和316之间，要进行特殊处理。
    印度国家码在404-406之间，其它国家的PLMN的国家码都相同，可以直接比较*/
    ulSrcMccUsaMccFlag     = NAS_MML_IsNorthAmericanMcc(ulSrcMcc);
    ulDestMccUsaMccFlag    = NAS_MML_IsNorthAmericanMcc(ulDestMcc);
    ulSrcMccIndianMccFlag  = NAS_MML_IsIndianMcc(ulSrcMcc);
    ulDestMccIndianMccFlag = NAS_MML_IsIndianMcc(ulDestMcc);

    if (((VOS_TRUE == ulSrcMccUsaMccFlag)
       && (VOS_TRUE == ulDestMccUsaMccFlag))
      || ((VOS_TRUE == ulSrcMccIndianMccFlag)
       && (VOS_TRUE == ulDestMccIndianMccFlag)))
    {
        return VOS_TRUE;
    }
    else
    {
        if (ulDestMcc != ulSrcMcc)
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MML_ExactlyCompareBcchPlmnwithSimPlmn(
    NAS_MML_PLMN_ID_STRU                *pstBccPlmnId,
    NAS_MML_PLMN_ID_STRU                *pstSimPlmnId
)
{
    VOS_UINT8                           ucBcchMnc3;
    VOS_UINT8                           ucSimMnc3;

    if (pstBccPlmnId->ulMcc == pstSimPlmnId->ulMcc)
    {
        if (pstBccPlmnId->ulMnc == pstSimPlmnId->ulMnc)
        {
            return VOS_TRUE;
        }
        else if ((pstBccPlmnId->ulMnc & NAS_MML_MCC_LOW_VALUE_MASK)
              == (pstSimPlmnId->ulMnc & NAS_MML_MCC_LOW_VALUE_MASK))
        {
            ucBcchMnc3 = (VOS_UINT8)((pstBccPlmnId->ulMnc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK) >> 16);
            ucSimMnc3  = (VOS_UINT8)((pstSimPlmnId->ulMnc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK) >> 16);

            /* PLMN 属于 NA PCS1900*/
            if ((NAS_MML_PCS1900_MCC_LOW_VALUE == (pstBccPlmnId->ulMcc & NAS_MML_MCC_LOW_VALUE_MASK))
             && (NAS_MML_PCS1900_MCC_HIGH_VALUE_RANGE >= (pstBccPlmnId->ulMcc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK)))
            {
                if ( ((NAS_MML_PCS1900_MNC_THIRD_BYTE_VALUE_F == ucBcchMnc3)
                 && (NAS_MML_PCS1900_MNC_THIRD_BYTE_VALUE_0 == ucSimMnc3))
                 || ((NAS_MML_PCS1900_MNC_THIRD_BYTE_VALUE_0 == ucBcchMnc3)
                 && (NAS_MML_PCS1900_MNC_THIRD_BYTE_VALUE_F == ucSimMnc3)) )
                {
                    return VOS_TRUE;
                }
            }
        }
        else
        {
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_CompareBcchPlmnwithSimPlmn(
    NAS_MML_PLMN_ID_STRU                *pstBccPlmnId,
    NAS_MML_PLMN_ID_STRU                *pstSimPlmnId
)
{
    VOS_BOOL                            bCmpRlst;
    VOS_UINT8                           ucBcchMnc3;
    VOS_UINT8                           ucSimMnc3;

    bCmpRlst   = VOS_FALSE;

    /* 如果需要精确比较MNC*/
    if (VOS_TRUE == NAS_MML_GetPlmnExactlyComparaFlg())
    {
        bCmpRlst = NAS_MML_ExactlyCompareBcchPlmnwithSimPlmn(pstBccPlmnId, pstSimPlmnId);

        return bCmpRlst;
    }

    /* HPLMN Matching Criteria for mobiles which support PCS1900 for NA:
    1) The MS shall compare using all 3 digits the SIM-MCC with the BCCH-MCC.
    If the values do not match, then the HPLMN match fails*/
    if (pstBccPlmnId->ulMcc == pstSimPlmnId->ulMcc)
    {
        /*The MS shall compare using all 3 digits the SIM-MNC with the BCCH-MNC.
          If the values match, then the HPLMN match succeeds*/
        if (pstBccPlmnId->ulMnc == pstSimPlmnId->ulMnc)
        {
            bCmpRlst = VOS_TRUE;
        }
        else if ((pstBccPlmnId->ulMnc & NAS_MML_MCC_LOW_VALUE_MASK)
              == (pstSimPlmnId->ulMnc & NAS_MML_MCC_LOW_VALUE_MASK))
        {
            /* 取MNC的最高位 ,若最高位是0或F，低2位Mnc相同则认为Plmn相等 */
            ucBcchMnc3 = (VOS_UINT8)((pstBccPlmnId->ulMnc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK) >> 16);
            ucSimMnc3  = (VOS_UINT8)((pstSimPlmnId->ulMnc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK) >> 16);

            /* PLMN 属于 NA PCS1900*/
            if ((NAS_MML_PCS1900_MCC_LOW_VALUE == (pstBccPlmnId->ulMcc & NAS_MML_MCC_LOW_VALUE_MASK))
             && (NAS_MML_PCS1900_MCC_HIGH_VALUE_RANGE >= (pstBccPlmnId->ulMcc & NAS_MML_MCC_THIRD_BYTE_VALUE_MASK)))
            {
                if ( ((NAS_MML_PCS1900_MNC_THIRD_BYTE_VALUE_F == ucBcchMnc3)
                   && (NAS_MML_PCS1900_MNC_THIRD_BYTE_VALUE_0 == ucSimMnc3))
                  || ((NAS_MML_PCS1900_MNC_THIRD_BYTE_VALUE_0 == ucBcchMnc3)
                   && (NAS_MML_PCS1900_MNC_THIRD_BYTE_VALUE_F == ucSimMnc3)) )
                {
                    bCmpRlst = VOS_TRUE;
                }
            }
            else
            {
                /* The MS shall compare using just the 1st 2 digits of the SIM-MNC
                with the BCCH-MNC. If the values match, then the HPLMN match succeeds,
                otherwise the HPLMN match fails.*/
                if (NAS_MML_LOW_BYTE_INVALID == ucBcchMnc3)
                {
                    bCmpRlst = VOS_TRUE;
                }
            }
        }
        else
        {
        }

    }

    return bCmpRlst;
}


VOS_UINT32 NAS_MML_IsNetRatSupported(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enSpecRat
)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatList = VOS_NULL_PTR;

    pstRatList = NAS_MML_GetMsPrioRatList();

    /* 指定的接入技术是否在当前用户设置的RAT列表中 */
    for (i = 0 ; i < pstRatList->ucRatNum; i++)
    {
        if ( (enSpecRat == pstRatList->aucRatPrio[i])
          && (enSpecRat < NAS_MML_NET_RAT_TYPE_BUTT))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsSpecRatInRatList(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enSpecRat,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatList
)
{
    VOS_UINT32                          i;

    /* 指定的接入技术是否在当前用户设置的RAT列表中 */
    for (i = 0 ; i < pstRatList->ucRatNum; i++)
    {
        if ( (enSpecRat == pstRatList->aucRatPrio[i])
          && (enSpecRat < NAS_MML_NET_RAT_TYPE_BUTT))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsSpec3Gpp2RatInRatList(
    NAS_MML_3GPP2_RAT_TYPE_ENUM_UINT8   enSpec3Gpp2Rat,
    NAS_MML_3GPP2_RAT_PRIO_STRU        *pst3Gpp2RatList
)
{
    VOS_UINT32                          i;

    /* 指定的接入技术是否在当前用户设置的RAT列表中 */
    for (i = 0 ; i < pst3Gpp2RatList->ucRatNum; i++)
    {
        if ( (enSpec3Gpp2Rat == pst3Gpp2RatList->aucRatPrio[i])
          && (enSpec3Gpp2Rat < NAS_MML_3GPP2_RAT_TYPE_BUTT))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsPlmnIdValid(
    NAS_MML_PLMN_ID_STRU                *pstPlmnId
)
{
    VOS_UINT32                          ulResult;

    /*
    The coding of this field is the responsibility of each administration but
    BCD coding shall be used. The MNC shall consist of 2 or 3 digits.
    If a network operator decides to use only two digits in the MNC,
    bits 5 to 8 of octet 7 shall be coded as "1111".
    MCC digit 2     MCC digit 1
    MNC digit 3     MCC digit 3
    MNC digit 2     MNC digit 1
    */

    ulResult = VOS_TRUE;

    if ( ((pstPlmnId->ulMcc & NAS_MML_BYTE_MASK) > NAS_MML_MAX_DIGIT_VALUE)
      || (((pstPlmnId->ulMcc >> 8) & NAS_MML_BYTE_MASK) > NAS_MML_MAX_DIGIT_VALUE)
      || (((pstPlmnId->ulMcc >> 16) & NAS_MML_BYTE_MASK) > NAS_MML_MAX_DIGIT_VALUE))
    {
        ulResult = VOS_FALSE;
    }

    if ((((pstPlmnId->ulMnc) & NAS_MML_BYTE_MASK) > NAS_MML_MAX_DIGIT_VALUE)
     || (((pstPlmnId->ulMnc >> 8) & NAS_MML_BYTE_MASK) > NAS_MML_MAX_DIGIT_VALUE))
    {
        ulResult = VOS_FALSE;
    }

    if ((((pstPlmnId->ulMnc >> 16) & NAS_MML_BYTE_MASK) > NAS_MML_MAX_DIGIT_VALUE)
     && (((pstPlmnId->ulMnc >> 16) & NAS_MML_BYTE_MASK) != NAS_MML_LOW_BYTE_INVALID))
    {
        ulResult = VOS_FALSE;
    }

    return ulResult;
}


VOS_UINT32 NAS_MML_IsMccValid(
    VOS_UINT32                          ulMcc
)
{
    VOS_UINT32                          ulResult;

    /*
    The coding of this field is the responsibility of each administration but
    BCD coding shall be used. The MNC shall consist of 2 or 3 digits.
    If a network operator decides to use only two digits in the MNC,
    bits 5 to 8 of octet 7 shall be coded as "1111".
    MCC digit 2     MCC digit 1
    MNC digit 3     MCC digit 3
    MNC digit 2     MNC digit 1
    */

    ulResult = VOS_TRUE;

    if ( ((ulMcc & NAS_MML_BYTE_MASK) > NAS_MML_MAX_DIGIT_VALUE)
      || (((ulMcc >> NAS_MML_OCTET_MOVE_EIGHT_BITS) & NAS_MML_BYTE_MASK)  > NAS_MML_MAX_DIGIT_VALUE)
      || (((ulMcc >> NAS_MML_OCTET_MOVE_SIXTEEN_BITS) & NAS_MML_BYTE_MASK) > NAS_MML_MAX_DIGIT_VALUE)
      || (((ulMcc >> NAS_MML_OCTET_MOVE_TWENTY_FOUR_BITS) & NAS_MML_BYTE_MASK) != NAS_MML_MCC_FOURTH_BYTES_VALID) )
    {
        ulResult = VOS_FALSE;
    }

    return ulResult;
}



VOS_UINT32 NAS_MML_IsBcchPlmnIdInDestSimPlmnList (
    NAS_MML_PLMN_ID_STRU               *pstBcchPlmnId,
    VOS_UINT8                           ucSimPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstSimPlmnIdList
)
{
    VOS_UINT32                          i;

    for ( i = 0; i < ucSimPlmnNum; i++ )
    {
        if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstBcchPlmnId,
                                                            pstSimPlmnIdList+i) )
        {
            return VOS_TRUE;
        }
    }


    return VOS_FALSE;
}




VOS_UINT32 NAS_MML_IsSimPlmnIdInDestBcchPlmnList (
    NAS_MML_PLMN_ID_STRU               *pstSimPlmnId,
    VOS_UINT8                           ucBcchPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstBcchPlmnIdList
)
{
    VOS_UINT32                          i;

    for ( i = 0; i < ucBcchPlmnNum; i++ )
    {
        if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstBcchPlmnIdList + i,
                                                            pstSimPlmnId) )
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}




VOS_UINT32 NAS_MML_IsBcchPlmnIdInDestSimBcdPlmnList(
    NAS_MML_PLMN_ID_STRU               *pstBcchPlmnId,
    VOS_UINT8                           ucSimPlmnNum,
    VOS_UINT8                          *pstSimBcdPlmnIdList
)
{
    NAS_MML_SIM_FORMAT_PLMN_ID          stSimPlmnId;
    NAS_MML_PLMN_ID_STRU                stUserPlmnId;
    VOS_UINT8                           i;

    for (i = 0; i < ucSimPlmnNum; i++)
    {
        PS_MEM_SET(&stSimPlmnId, 0x00, sizeof(NAS_MML_SIM_FORMAT_PLMN_ID));

        stSimPlmnId.aucSimPlmn[0] = pstSimBcdPlmnIdList[(i * NAS_MML_SIM_FORMAT_PLMN_LEN)];
        stSimPlmnId.aucSimPlmn[1] = pstSimBcdPlmnIdList[(i * NAS_MML_SIM_FORMAT_PLMN_LEN) + 1];
        stSimPlmnId.aucSimPlmn[2] = pstSimBcdPlmnIdList[(i * NAS_MML_SIM_FORMAT_PLMN_LEN) + 2];

        NAS_MML_ConvertSimPlmnToNasPLMN(&stSimPlmnId, &stUserPlmnId);

        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstBcchPlmnId, &stUserPlmnId))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsTheIntersectionOfSpecPlmnListsEmpty(
    VOS_UINT8                           ucSrcPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstSrcPlmnIdList,
    VOS_UINT8                           ucDstPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstDstPlmnIdList
)
{
    VOS_UINT32                i;

    for (i = 0; i < ucSrcPlmnNum; i++)
    {
        if (VOS_TRUE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstSrcPlmnIdList + i, ucDstPlmnNum, pstDstPlmnIdList))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsBcchPlmnIdInDestSimPlmnWithRatList (
    NAS_MML_PLMN_ID_STRU               *pstBcchPlmnId,
    VOS_UINT16                          usSimPlmnWithRatNum,
    NAS_MML_SIM_PLMN_WITH_RAT_STRU     *pstSimPlmnWithRatList
)
{
    VOS_UINT32                          i;

    for ( i = 0; i < usSimPlmnWithRatNum; i++ )
    {
        if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstBcchPlmnId,
                                                            &pstSimPlmnWithRatList[i].stPlmnId) )
        {
            return VOS_TRUE;
        }
    }


    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsLaiInDestLaiList (
    NAS_MML_LAI_STRU                   *pstLai,
    VOS_UINT32                          ulLaiNum,
    NAS_MML_LAI_STRU                   *pstLaiList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j;
    VOS_BOOL                            bFlg;

    bFlg = VOS_FALSE;

    for ( i = 0; i < ulLaiNum; i++ )
    {
        if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstLai->stPlmnId),
                                                  &(pstLaiList[i].stPlmnId)) )
        {

            for ( j = 0 ; j < NAS_MML_MAX_LAC_LEN ; j++ )
            {
                if (pstLai->aucLac[j] != pstLaiList[i].aucLac[j])
                {
                    break;
                }
            }

            if (NAS_MML_MAX_LAC_LEN == j)
            {
                bFlg = VOS_TRUE;
                break;
            }
        }
    }


    return (bFlg);
}


VOS_UINT32 NAS_MML_GetLaiIndexInDestLaiList (
    NAS_MML_LAI_STRU                   *pstLai,
    VOS_UINT8                          *pucLaiIdx,
    VOS_UINT8                           ulLaiNum,
    NAS_MML_LAI_STRU                   *pstLaiList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j;
    VOS_UINT32                          ulFlg;

    ulFlg      = VOS_FALSE;
    *pucLaiIdx = 0;

    for ( i = 0; i < ulLaiNum; i++ )
    {
        if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstLai->stPlmnId),
                                                  &(pstLaiList[i].stPlmnId)) )
        {
            for ( j = 0 ; j < NAS_MML_MAX_LAC_LEN; j++ )
            {
                if (pstLai->aucLac[j] != pstLaiList[i].aucLac[j])
                {
                    break;
                }
            }

            if (NAS_MML_MAX_LAC_LEN == j)
            {
                /* 返回查找的LAI在列表中的index */
                *pucLaiIdx = (VOS_UINT8)i;
                ulFlg = VOS_TRUE;
                break;
            }
        }
    }


    return (ulFlg);
}


VOS_UINT32 NAS_MML_GetLaiIndexInRegFailForbLaList (
    NAS_MML_LAI_STRU                                       *pstLai,
    VOS_UINT8                                              *pucLaiIdx,
    VOS_UINT32                                              ulLaiNum,
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_STRU                 *pstRegFailForbLaList
)
{
    NAS_MML_LAI_STRU                   *pstNasMmlLai = VOS_NULL_PTR;
    VOS_UINT32                          i;
    VOS_UINT32                          ulFlg;

    ulFlg      = VOS_FALSE;
    *pucLaiIdx = 0;

    for ( i = 0; i < ulLaiNum; i++ )
    {
        pstNasMmlLai = &(pstRegFailForbLaList[i].stForbLaWithValidPeriodLai);
        if (VOS_TRUE == NAS_MML_CompareLai(pstLai, pstNasMmlLai))
        {
            /* 返回查找的LAI在列表中的index */
            *pucLaiIdx = (VOS_UINT8)i;
            ulFlg = VOS_TRUE;
            break;
        }
    }

    return (ulFlg);
}


VOS_VOID NAS_MML_AddCustomizedForbLaToDestLaList(
    VOS_UINT8                          *pucDestLaNum,
    NAS_MML_LAI_STRU                   *pstDestLaList
)
{
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriodList = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                                       *pstLai                       = VOS_NULL_PTR;
    VOS_UINT32                                              i;

    pstForbLaWithValidPeriodList = NAS_MML_GetForbLaWithValidPeriodList();
    for ( i = 0; i < pstForbLaWithValidPeriodList->ulForbLaWithValidPeriodNum; i++ )
    {
        pstLai = &(pstForbLaWithValidPeriodList->astForbLaWithValidPeriodList[i].stForbLaWithValidPeriodLai);
        if ( (VOS_FALSE == NAS_MML_IsLaiInDestLaiList(pstLai, *pucDestLaNum, pstDestLaList))
          && ( (*pucDestLaNum) < RRC_MAX_FORBLA_NUM) )
        {
            PS_MEM_CPY(&(pstDestLaList[*pucDestLaNum]) ,
                pstLai, sizeof(NAS_MML_LAI_STRU));
            (*pucDestLaNum)++;
        }

        /* 列表满了之后直接退出 */
        if ( (*pucDestLaNum) >= RRC_MAX_FORBLA_NUM )
        {
            break;
        }
    }

    return;
}


VOS_UINT32 NAS_MML_IsInForbLaWithValidPeriodList(
    NAS_MML_LAI_STRU                    *pstLai
)
{
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriod    = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                                       *pstTmpLai                   = VOS_NULL_PTR;
    VOS_UINT32                                              i;

    pstForbLaWithValidPeriod = NAS_MML_GetForbLaWithValidPeriodList();

    /* LA在"forbidden las for regional" list */
    for (i = 0; i < pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum; i++)
    {
        pstTmpLai = &(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].stForbLaWithValidPeriodLai);
        if (VOS_TRUE == NAS_MML_CompareLai(pstLai, pstTmpLai))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_ComparePlmnIdWithHplmn (
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_SIM_EHPLMN_INFO_STRU       *pstEhplmnInfo = VOS_NULL_PTR;
    VOS_UINT32                          i;

    pstEhplmnInfo = NAS_MML_GetSimEhplmnList();

    for (i = 0; i < pstEhplmnInfo->ucEhPlmnNum; i++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId,
                                                           &(pstEhplmnInfo->astEhPlmnInfo[i].stPlmnId)))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_ComparePlmnIdWithUplmn (
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_SIM_USERPLMN_INFO_STRU     *pstUserPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_OPERPLMN_INFO_STRU     *pstOperPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_SELPLMN_INFO_STRU      *pstSelPlmnList  = VOS_NULL_PTR;

    /* 获取UOPLMN */
    pstUserPlmnList = NAS_MML_GetSimUserPlmnList();
    pstOperPlmnList = NAS_MML_GetSimOperPlmnList();

    /* UPLMN个数不为0或者OPLMN个数不为0 */
    if ((pstUserPlmnList->ucUserPlmnNum > 0)
     || (pstOperPlmnList->usOperPlmnNum > 0))
    {
        if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnWithRatList(pstPlmnId,
                                                      pstUserPlmnList->ucUserPlmnNum,
                                                      pstUserPlmnList->astUserPlmnInfo))
        {
            return VOS_TRUE;
        }
    }
    else /* 表示(U)SIM卡中不存在UPLMN、OPLMN，使用PLMN Sel文件 */
    {
        pstSelPlmnList = NAS_MML_GetSimSelPlmnList();

        if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                                      pstSelPlmnList->ucSelPlmnNum,
                                                      pstSelPlmnList->astPlmnId))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_ComparePlmnIdWithOplmn (
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_SIM_USERPLMN_INFO_STRU     *pstUserPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_OPERPLMN_INFO_STRU     *pstOperPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_SELPLMN_INFO_STRU      *pstSelPlmnList  = VOS_NULL_PTR;

    /* 获取UOPLMN */
    pstUserPlmnList = NAS_MML_GetSimUserPlmnList();
    pstOperPlmnList = NAS_MML_GetSimOperPlmnList();

    /* UPLMN个数不为0或者OPLMN个数不为0 */
    if ((pstUserPlmnList->ucUserPlmnNum > 0)
     || (pstOperPlmnList->usOperPlmnNum > 0))
    {
        if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnWithRatList(pstPlmnId,
                                                      pstOperPlmnList->usOperPlmnNum,
                                                      pstOperPlmnList->astOperPlmnInfo))
        {
            return VOS_TRUE;
        }
    }
    else /* 表示(U)SIM卡中不存在UPLMN、OPLMN，使用PLMN Sel文件 */
    {
        pstSelPlmnList = NAS_MML_GetSimSelPlmnList();

        if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                                      pstSelPlmnList->ucSelPlmnNum,
                                                      pstSelPlmnList->astPlmnId))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsPlmnIdRoamingAllowed(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_ROAM_CFG_INFO_STRU         *pstRoamCfg = VOS_NULL_PTR;
    VOS_UINT32                          ulFlg;

    pstRoamCfg        = NAS_MML_GetRoamCfg();

    /*与HPLMN相同，直接返回允许*/
    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithHplmn(pstPlmnId))
    {
        return VOS_TRUE;
    }

    /*漫游特性开关关闭，直接返回允许*/
    if (VOS_FALSE == pstRoamCfg->ucRoamFeatureFlg )
    {
        return VOS_TRUE;
    }

    /*允许漫游时直接返回允许*/
    if (NAS_MML_ROAM_NATION_ON_INTERNATION_ON == pstRoamCfg->enRoamCapability)
    {
        return VOS_TRUE;
    }

    /*允许国内漫游时,如果是用户列表搜索结果显示,则搜索和显示所有国内网，
      包括50506，50599，00101，50501, 以及Optus 2G,3G, Vodafone 2G 3G；
      如果不是用户列表搜索结果显示，可以注册或漫游到50501 */
    ulFlg = NAS_MML_IsSimPlmnIdInDestBcchPlmnList( pstPlmnId,
                                            pstRoamCfg->ucNationalRoamNum,
                                            pstRoamCfg->astRoamPlmnIdList );
    if ( (NAS_MML_ROAM_NATION_ON_INTERNATION_OFF == pstRoamCfg->enRoamCapability)
      && (VOS_TRUE                               == ulFlg) )
    {
        return VOS_TRUE;
    }

    /*不允许国内漫游时，不能漫游到任何网络 */
    ulFlg = NAS_MML_IsSimPlmnIdInDestBcchPlmnList( pstPlmnId,
                                                   pstRoamCfg->ucNotRoamNum,
                                                   pstRoamCfg->astRoamPlmnIdList );
    if ( ( (NAS_MML_ROAM_NATION_OFF_INTERNATION_ON == pstRoamCfg->enRoamCapability)
        || (NAS_MML_ROAM_NATION_OFF_INTERNATION_OFF == pstRoamCfg->enRoamCapability) )
      && (VOS_TRUE == ulFlg) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

#if (FEATURE_ON == FEATURE_CSG)

VOS_UINT32 NAS_MML_IsPlmnWithCsgIdInDestPlmnWithCsgIdList(
    NAS_MML_PLMN_WITH_CSG_ID_STRU      *pstPlmnWithCsgId,
    VOS_UINT32                          ulPlmnWithCsgIdListNum,
    NAS_MML_PLMN_WITH_CSG_ID_STRU      *pstPlmnWithCsgIdList
)
{
    VOS_UINT32                          ulIndex;

    for (ulIndex = 0; ulIndex < NAS_MML_MIN(ulPlmnWithCsgIdListNum,NAS_MML_MAX_PLMN_CSG_ID_NUM); ulIndex++)
    {
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnWithCsgId->stPlmnId, &(pstPlmnWithCsgIdList[ulIndex].stPlmnId)))
          && (pstPlmnWithCsgId->ulCsgId == pstPlmnWithCsgIdList[ulIndex].ulCsgId))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_GetCsgIdIndexInDestCsgIdList(
    NAS_MML_PLMN_WITH_CSG_ID_STRU      *pstPlmnWithCsgId,
    VOS_UINT32                          ulDestCsgIdNum,
    NAS_MML_PLMN_WITH_CSG_ID_INFO_STRU *pstDestCsgIdlist
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulIndex;

    ulIndex = ulDestCsgIdNum;

    if (ulIndex >= NAS_MML_MAX_PLMN_CSG_ID_NUM)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MML_GetCsgIdIndexInDestCsgIdList, ulDestCsgIdNum invalid");
        ulIndex = NAS_MML_MAX_PLMN_CSG_ID_NUM;
    }

    for ( i = 0; i < ulIndex; i++ )
    {
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmnWithCsgId->stPlmnId), &(pstDestCsgIdlist[i].stPlmnWithCsgId.stPlmnId)))
         && (pstPlmnWithCsgId->ulCsgId == pstDestCsgIdlist[i].stPlmnWithCsgId.ulCsgId))
        {
            break;
        }
    }

    return i;
}



VOS_UINT32 NAS_MML_IsSupportLteCsg(VOS_VOID)
{
    /* syscfg设置不支持Lte,返回false */
    if ( VOS_FALSE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
    {
        return VOS_FALSE;
    }

    /* CSG NV项关闭，返回false */
    if (VOS_FALSE == NAS_MML_GetCsgSupportFlag())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "CSG NV Item is closed");
        return VOS_FALSE;
    }

    /* CSG动态开关关闭，返回false，待迭代II实现 */

    return VOS_TRUE;
}



VOS_UINT32 NAS_MML_IsCsgIdInForbiddenCsgIdList(
    NAS_MML_PLMN_WITH_CSG_ID_STRU      *pstPlmnWithCsgId
)
{
    NAS_MML_CSG_ID_WITH_FORBIDDEN_PERIOD_LIST_STRU         *pstForbiddenCsgIdList = VOS_NULL_PTR;
    VOS_UINT32                                              ulIndex;

    pstForbiddenCsgIdList = NAS_MML_GetForbiddenCsgList();

    for (ulIndex = 0; ulIndex < NAS_MML_MIN(pstForbiddenCsgIdList->ulForbiddenCsgIdNum,NAS_MML_MAX_PLMN_CSG_ID_NUM); ulIndex++)
    {
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnWithCsgId->stPlmnId, &(pstForbiddenCsgIdList->astForbiddenCsgIdPeriodInfo[ulIndex].stPlmnWithCsgIdInfo.stPlmnId)))
          && (pstPlmnWithCsgId->ulCsgId == pstForbiddenCsgIdList->astForbiddenCsgIdPeriodInfo[ulIndex].stPlmnWithCsgIdInfo.ulCsgId))
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsCsgIdInAllowedCsgIdList(
    NAS_MML_PLMN_WITH_CSG_ID_STRU      *pstPlmnWithCsgId
)
{
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU  *pstAllowedCsgList = VOS_NULL_PTR;
    VOS_UINT32                           ulIndex;

    pstAllowedCsgList = NAS_MML_GetAllowedCsgList();

    for (ulIndex = 0; ulIndex < NAS_MML_MIN(pstAllowedCsgList->ulPlmnWithCsgIdListNum, NAS_MML_MAX_PLMN_CSG_ID_NUM); ulIndex++)
    {
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnWithCsgId->stPlmnId, &(pstAllowedCsgList->astPlmnWithCsgIdInfo[ulIndex].stPlmnWithCsgId.stPlmnId)))
          && (pstPlmnWithCsgId->ulCsgId == pstAllowedCsgList->astPlmnWithCsgIdInfo[ulIndex].stPlmnWithCsgId.ulCsgId))
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsCsgIdInOperatorCsgIdList(
    NAS_MML_PLMN_WITH_CSG_ID_STRU      *pstPlmnWithCsgId
)
{
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU  *pstOperatorCsgList = VOS_NULL_PTR;
    VOS_UINT32                           ulIndex;

    pstOperatorCsgList = NAS_MML_GetOperatorCsgList();

    for (ulIndex = 0; ulIndex < NAS_MML_MIN(pstOperatorCsgList->ulPlmnWithCsgIdListNum, NAS_MML_MAX_PLMN_CSG_ID_NUM); ulIndex++)
    {
        if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstPlmnWithCsgId->stPlmnId, &(pstOperatorCsgList->astPlmnWithCsgIdInfo[ulIndex].stPlmnWithCsgId.stPlmnId)))
          && (pstPlmnWithCsgId->ulCsgId == pstOperatorCsgList->astPlmnWithCsgIdInfo[ulIndex].stPlmnWithCsgId.ulCsgId))
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}

VOS_UINT32 NAS_MML_IsCsgIdInCsgWhiteList(
    NAS_MML_PLMN_WITH_CSG_ID_STRU      *pstPlmnWithCsgId,
    MMC_LMM_PLMN_ID_STRU               *pstPlmn,
    VOS_UINT32                          ulCsgId
)
{
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stPlmnWithCsgId;
    NAS_MML_PLMN_ID_STRU                                    stSourcePlmnId;
    VOS_UINT32                                              ulCsgInOperatorList;
    VOS_UINT32                                              ulCsgInAllowedList;

    PS_MEM_SET(&stPlmnWithCsgId, 0, sizeof(NAS_MML_PLMN_WITH_CSG_ID_STRU));
    PS_MEM_SET(&stSourcePlmnId, 0, sizeof(NAS_MML_PLMN_ID_STRU));

    if (VOS_NULL_PTR != pstPlmn)
    {
        NAS_MMC_ConvertLmmPlmnToGUNasFormat(pstPlmn, &stSourcePlmnId);
        NAS_NORMAL_LOG3(WUEPS_PID_MMC, "LNAS PLMN",pstPlmn->aucPlmnId[0],pstPlmn->aucPlmnId[1],pstPlmn->aucPlmnId[2] );

        stPlmnWithCsgId.stPlmnId.ulMcc = stSourcePlmnId.ulMcc;
        stPlmnWithCsgId.stPlmnId.ulMnc = stSourcePlmnId.ulMnc;
        stPlmnWithCsgId.ulCsgId        = ulCsgId;
        NAS_NORMAL_LOG(WUEPS_PID_MMC,"CSG USERD BY LNAS");
    }
    else
    {
        stPlmnWithCsgId.stPlmnId.ulMcc = pstPlmnWithCsgId->stPlmnId.ulMcc;
        stPlmnWithCsgId.stPlmnId.ulMnc = pstPlmnWithCsgId->stPlmnId.ulMnc;
        stPlmnWithCsgId.ulCsgId        = pstPlmnWithCsgId->ulCsgId;
    }
    NAS_NORMAL_LOG3(WUEPS_PID_MMC, "NAS_MML_IsCsgIdInCsgWhiteList ,MCC,MNC,CSG",stPlmnWithCsgId.stPlmnId.ulMcc,stPlmnWithCsgId.stPlmnId.ulMnc,stPlmnWithCsgId.ulCsgId );

    ulCsgInAllowedList          = NAS_MML_IsCsgIdInAllowedCsgIdList(&stPlmnWithCsgId);
    ulCsgInOperatorList         = NAS_MML_IsCsgIdInOperatorCsgIdList(&stPlmnWithCsgId);
    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "CSG IS IN ALLOWED LIST == ",ulCsgInAllowedList);
    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "CSG IS IN OPERATOR LIST == ",ulCsgInOperatorList);

    if (VOS_TRUE == NAS_MML_IsCsgIdInForbiddenCsgIdList(&stPlmnWithCsgId))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC,"CSG IN FORBBIDEN LIST");
        return VOS_FALSE;
    }

    if ((VOS_FALSE == ulCsgInAllowedList)
     && (VOS_FALSE == ulCsgInOperatorList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC,"CSG WHITE LIST IS EMPTY");
        return VOS_FALSE;
    }
    return VOS_TRUE;
}


VOS_UINT32 NAS_MML_IsPlmnInOperatorCsgIdList(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU  *pstOperatorCsgList = VOS_NULL_PTR;
    VOS_UINT32                           ulIndex;

    pstOperatorCsgList = NAS_MML_GetOperatorCsgList();

    for (ulIndex = 0; ulIndex < NAS_MML_MIN(pstOperatorCsgList->ulPlmnWithCsgIdListNum, NAS_MML_MAX_PLMN_CSG_ID_NUM); ulIndex++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId, &(pstOperatorCsgList->astPlmnWithCsgIdInfo[ulIndex].stPlmnWithCsgId.stPlmnId)))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MML_IsPlmnInAllowedCsgIdList(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU  *pstAllowedCsgList = VOS_NULL_PTR;
    VOS_UINT32                           ulIndex;

    pstAllowedCsgList = NAS_MML_GetAllowedCsgList();

    for (ulIndex = 0; ulIndex < NAS_MML_MIN(pstAllowedCsgList->ulPlmnWithCsgIdListNum, NAS_MML_MAX_PLMN_CSG_ID_NUM); ulIndex++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId, &(pstAllowedCsgList->astPlmnWithCsgIdInfo[ulIndex].stPlmnWithCsgId.stPlmnId)))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsAllowedCsgListEmpty(VOS_VOID)
{
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU  *pstAllowedCsgList = VOS_NULL_PTR;

    pstAllowedCsgList = NAS_MML_GetAllowedCsgList();

    if (0 == pstAllowedCsgList->ulPlmnWithCsgIdListNum)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MML_IsUeBasedOperatorCsgListEmpty(VOS_VOID)
{
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU  *pstBasedCsgList = VOS_NULL_PTR;

    pstBasedCsgList = NAS_MML_GetUeBasedOperatorCsgList();

    if (0 == pstBasedCsgList->ulPlmnWithCsgIdListNum)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MML_IsOperatorCsgListEmpty(VOS_VOID)
{
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU  *pstOperatorCsgList = VOS_NULL_PTR;

    pstOperatorCsgList = NAS_MML_GetOperatorCsgList();

    if (0 == pstOperatorCsgList->ulPlmnWithCsgIdListNum)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MML_IsWhiteCsgListEmpty(VOS_VOID)
{
    NAS_MML_PLMN_WITH_CSG_ID_LIST_STRU                     *pstOperatorCsgList          = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stPlmnWithCsgId;
    VOS_UINT32                                              ulLoopMax;
    VOS_UINT32                                              ulLoop;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    if (PS_USIM_SERVICE_AVAILIABLE == NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_OP_CSG))
    {
        pstOperatorCsgList = NAS_MML_GetOperatorCsgList();
    }
    else
    {
        pstOperatorCsgList = NAS_MML_GetUeBasedOperatorCsgList();
    }

    if (VOS_FALSE == NAS_MML_IsAllowedCsgListEmpty())
    {
        return VOS_FALSE;
    }

    if (0 != pstOperatorCsgList->ulPlmnWithCsgIdListNum)
    {
        ulLoopMax = NAS_MML_MIN(pstOperatorCsgList->ulPlmnWithCsgIdListNum, NAS_MML_MAX_PLMN_CSG_ID_NUM);

        for (ulLoop = 0; ulLoop < ulLoopMax; ulLoop++)
        {
            stPlmnWithCsgId = pstOperatorCsgList->astPlmnWithCsgIdInfo[ulLoop].stPlmnWithCsgId;

            if (VOS_FALSE == NAS_MML_IsCsgIdInForbiddenCsgIdList(&stPlmnWithCsgId))
            {
                /* 构造带接入技术的PLMN ID */
                stPlmnWithRat.enRat         = NAS_MML_NET_RAT_TYPE_LTE;
                stPlmnWithRat.stPlmnId      = stPlmnWithCsgId.stPlmnId;

                /* 判断是否在禁止接入技术的PLMN列表中,如果在则不将该PLMN加入白名单列表 */
                if (VOS_FALSE == NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList(&stPlmnWithRat))
                {
                    return VOS_FALSE;
                }
            }
        }
    }

    return VOS_TRUE;
}

#endif





VOS_UINT32 NAS_MML_GetVoiceNotPreferDisabledPlmnForbiddenTimeSlice(VOS_VOID)
{
    VOS_UINT32                          ulDisabledPlmnInvalidPeriod;
    VOS_UINT32                          ulInvalidTimeSlice;

    ulDisabledPlmnInvalidPeriod = NAS_MML_GetVoiceNotAvailPlmnForbiddenPeriod();
    ulInvalidTimeSlice          = 0;

    if (NAS_MML_MAX_TIME_SLICE == ulDisabledPlmnInvalidPeriod)
    {
        return NAS_MML_MAX_TIME_SLICE;
    }

    ulInvalidTimeSlice = VOS_GetSliceUnit() * (ulDisabledPlmnInvalidPeriod / 1000);
    return ulInvalidTimeSlice;
}


VOS_UINT32 NAS_MML_GetDisabledPlmnRejCause42ForbiddenTimeSlice(VOS_VOID)
{
    VOS_UINT32                          ulInvalidTimeSlice;
    VOS_UINT32                          ulDisabledPlmnForbiddenPeriod;

    ulInvalidTimeSlice          = 0;
    ulDisabledPlmnForbiddenPeriod = 2 * NAS_MML_GetSimHplmnTimerLen();

    ulInvalidTimeSlice = VOS_GetSliceUnit() * (ulDisabledPlmnForbiddenPeriod / 1000);
    return ulInvalidTimeSlice;
}



VOS_UINT32 NAS_MML_IsNeedAddVoiceNotAvailDisabledPlmnInDisabledPlmnWithForbiddenPeriodList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegCause,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegCause,
    NAS_MML_ADDITION_UPDATE_RSLT_INFO_ENUM_UINT8            enAdditionUpdatRsltInfo
)
{
    /* 如果协议版本小于r11，无需加入，返回false */
    if (NAS_MML_GetLte3gppRelVersion() < NAS_MML_3GPP_REL_R11)
    {
        return VOS_FALSE;
    }

    /* 如果pstPlmnWithRat已经在禁止接入技术网络列表中则无需处理 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList(pstPlmnWithRat))
    {
        return VOS_FALSE;
    }


    /* 24301_CR1460R4_(Rel-11)_C1-123383 和24301_CR1527R1_(Rel-11)_C1-123937和
      24301_CR1504R1_(Rel-11)_C1-123388 rev of C1-122853 E-UTRA disabling clarification - v6
      对应24301 4.5章节:If UE that has disabled its E-UTRA capability due to IMS
      voice not available and CS fallback not available re-enables it when PLMN
      selection is performed, then it should memorize the identity of the PLMNs
      where E-UTRA capability was disabled and use that stored information in
      subsequent PLMN selections as specified in 3GPP TS 23.122 [6]. 该CR受协议版本控制 */
    if ((((NAS_MML_REG_FAIL_CAUSE_MSC_UNREACHABLE == enCsRegCause)
       || (NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE == enCsRegCause)
       || (NAS_MML_REG_FAIL_CAUSE_CS_DOMAIN_NOT_AVAILABLE == enCsRegCause))
     && (NAS_MML_REG_FAIL_CAUSE_NULL == enPsRegCause))
     || (NAS_MML_ADDITION_UPDATE_RSLT_SMS_ONLY == enAdditionUpdatRsltInfo)
     || (NAS_MML_ADDITION_UPDATE_RSLT_CSFB_NOT_PREFERRED == enAdditionUpdatRsltInfo))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_GetPlmnWithRatIndexInDestDisabledPlmnWithForbiddenPeriodList (
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstSrcPlmnId,
    VOS_UINT32                                              ulDestPlmnNum,
    NAS_MML_DISABLED_PLMN_WITH_FORBIDDEN_PERIOD_STRU       *pstDestPlmnIdList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulIndex;

    ulIndex = ulDestPlmnNum;

    if (ulIndex > NAS_MML_MAX_DISABLED_RAT_PLMN_NUM)
    {
        ulIndex = NAS_MML_MAX_DISABLED_RAT_PLMN_NUM;
    }

    for ( i = 0; i < ulIndex; i++ )
    {
        if ((VOS_TRUE               == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstSrcPlmnId->stPlmnId), &(pstDestPlmnIdList[i].stPlmnWithRat.stPlmnId)))
         && (pstSrcPlmnId->enRat    == pstDestPlmnIdList[i].stPlmnWithRat.enRat))
        {
            break;
        }
    }

    return i;
}


VOS_VOID NAS_MML_DeleteSpecDisabledPlmnWithValidPeriodInPlmnList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat
)
{
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledPlmnIdWithRatList = VOS_NULL_PTR;
    VOS_UINT32                                              ulIdx;

    pstDisabledPlmnIdWithRatList = NAS_MML_GetDisabledRatPlmnInfo();

    /* 获取删除plmn在禁止接入技术网络列表中 */
    ulIdx = NAS_MML_GetPlmnWithRatIndexInDestDisabledPlmnWithForbiddenPeriodList(pstPlmnWithRat,
                                                 pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum,
                                                 pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId);

    /* 找到对应的PlmnId,删除 */
    if (ulIdx < pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum)
    {
        if (ulIdx == (pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum - 1))
        {
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIdx].stPlmnWithRat.stPlmnId.ulMcc = NAS_MML_INVALID_MCC;
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIdx].stPlmnWithRat.stPlmnId.ulMnc = NAS_MML_INVALID_MNC;
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIdx].stPlmnWithRat.enRat = NAS_MML_NET_RAT_TYPE_BUTT;
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIdx].ulPlmnDisabledForbiddenSlice   = NAS_MML_MAX_TIME_SLICE;
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIdx].ulPlmnDisabledBeginSlice = 0;
        }
        else
        {
            PS_MEM_MOVE(pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId + ulIdx,
              (pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId + (ulIdx + 1)),
              (NAS_MML_MAX_DISABLED_RAT_PLMN_NUM - (ulIdx + 1)) * sizeof(NAS_MML_DISABLED_PLMN_WITH_FORBIDDEN_PERIOD_STRU));
        }

        pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum--;
    }

    NAS_MMC_LogForbiddenPlmnRelatedInfo();
    return;
}



VOS_UINT32 NAS_MML_DeleteExpireValidPeriodPlmnInDisabledRatPlmnList(VOS_VOID)
{
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulIndex;
    VOS_UINT32                                              ulIsNeedDeleteDisabledPlmnWithRat;

    pstDisabledPlmnInfo               = NAS_MML_GetDisabledRatPlmnInfo();
    ulIsNeedDeleteDisabledPlmnWithRat = VOS_FALSE;

    for (ulIndex = 0; ulIndex < pstDisabledPlmnInfo->ulDisabledRatPlmnNum;)
    {
        if (VOS_TRUE == NAS_MML_IsNeedDeleteDisabledPlmnWithValidPeriodInPlmnList(&pstDisabledPlmnInfo->astDisabledRatPlmnId[ulIndex]))
        {
            if (ulIndex < pstDisabledPlmnInfo->ulDisabledRatPlmnNum - 1)
            {
                PS_MEM_MOVE(&(pstDisabledPlmnInfo->astDisabledRatPlmnId[ulIndex]),
                            &(pstDisabledPlmnInfo->astDisabledRatPlmnId[ulIndex+1]),
                             (pstDisabledPlmnInfo->ulDisabledRatPlmnNum-ulIndex-1)*sizeof(NAS_MML_DISABLED_PLMN_WITH_FORBIDDEN_PERIOD_STRU));
            }

            (pstDisabledPlmnInfo->ulDisabledRatPlmnNum)--;

            ulIsNeedDeleteDisabledPlmnWithRat = VOS_TRUE;

            NAS_MMC_LogForbiddenPlmnRelatedInfo();

            if (ulIndex > 0 )
            {
                ulIndex--;
            }
            else
            {
                continue;
            }

        }

        ulIndex++;
    }

    /* 如果禁止接入技术网络列表中还存在非NV配置的网络禁止期限为全F，则继续启动周期删除禁止接入技术网络列表定时器 */
    for (ulIndex = 0; ulIndex < pstDisabledPlmnInfo->ulDisabledRatPlmnNum; ulIndex++)
    {
        if (NAS_MML_MAX_TIME_SLICE != pstDisabledPlmnInfo->astDisabledRatPlmnId[ulIndex].ulPlmnDisabledForbiddenSlice)
        {
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_DELETE_DISABLED_PLMN_WITH_RAT_TIMER,
                              TI_NAS_MMC_PERIOD_DELETE_DISABLED_PLMN_WITH_RAT_TIMER_LEN);
            break;
        }
    }

    return ulIsNeedDeleteDisabledPlmnWithRat;
}


VOS_UINT32 NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList (
    NAS_MML_PLMN_WITH_RAT_STRU         *pstSrcPlmnId
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulIndex;

    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledPlmnInfo = VOS_NULL_PTR;

    pstDisabledPlmnInfo = NAS_MML_GetDisabledRatPlmnInfo();

    /* 使用禁止接入技术网络列表前先判断下是否有惩罚时间到的网络，如果有先删除 */
    (VOS_VOID)NAS_MML_DeleteExpireValidPeriodPlmnInDisabledRatPlmnList();

    ulIndex = pstDisabledPlmnInfo->ulDisabledRatPlmnNum;

    if (ulIndex > NAS_MML_MAX_DISABLED_RAT_PLMN_NUM)
    {
        ulIndex = NAS_MML_MAX_DISABLED_RAT_PLMN_NUM;
    }

    for ( i = 0; i < ulIndex; i++ )
    {
        if ((VOS_TRUE               == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstSrcPlmnId->stPlmnId), &(pstDisabledPlmnInfo->astDisabledRatPlmnId[i].stPlmnWithRat.stPlmnId)))
         && (pstSrcPlmnId->enRat    == pstDisabledPlmnInfo->astDisabledRatPlmnId[i].stPlmnWithRat.enRat))
        {
            NAS_NORMAL_LOG3(WUEPS_PID_MMC, "NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList IS TRUE,MCC,MNC,RAT",pstSrcPlmnId->stPlmnId.ulMcc,pstSrcPlmnId->stPlmnId.ulMnc,pstSrcPlmnId->enRat);

            return VOS_TRUE;
        }
    }


    return VOS_FALSE;
}





VOS_UINT32 NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnRatList (
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstSrcPlmnId,
    VOS_UINT32                                              ulDestPlmnNum,
    NAS_MML_DISABLED_PLMN_WITH_FORBIDDEN_PERIOD_STRU       *pstDestPlmnIdList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulIndex;

    ulIndex = ulDestPlmnNum;

    if (ulIndex > NAS_MML_MAX_DISABLED_RAT_PLMN_NUM)
    {
        ulIndex = NAS_MML_MAX_DISABLED_RAT_PLMN_NUM;
    }

    for ( i = 0; i < ulIndex; i++ )
    {
        if ((VOS_TRUE               == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstSrcPlmnId->stPlmnId), &(pstDestPlmnIdList[i].stPlmnWithRat.stPlmnId)))
         && (pstSrcPlmnId->enRat    == pstDestPlmnIdList[i].stPlmnWithRat.enRat))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}




VOS_UINT32 NAS_MML_IsBcchPlmnIdInLockPlmnList (
    NAS_MML_PLMN_ID_STRU               *pstBcchPlmnId
)
{
    NAS_MML_PLMN_LOCK_CFG_INFO_STRU                        *pstPlmnLockInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList       = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;
    VOS_UINT32                                              ulIndex;
    VOS_UINT32                                              ulDisabledPlmnFlg;
    VOS_UINT32                                              ulRatDisabledFlg;

    /* 获取当前接入技术优先级信息 */
    pstPrioRatList          = NAS_MML_GetMsPrioRatList();

    pstPlmnLockInfo         = NAS_MML_GetPlmnLockCfg();

    ulDisabledPlmnFlg       = VOS_TRUE;

    ulRatDisabledFlg        = VOS_TRUE;

    /* E5形态PLMN ID在黑名单中 */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstBcchPlmnId,
                                                          pstPlmnLockInfo->ucBlackPlmnLockNum,
                                                          pstPlmnLockInfo->astBlackPlmnId))
    {
        return VOS_TRUE;
    }

    /* 构造带接入技术的PLMN ID */
    stPlmnWithRat.stPlmnId.ulMcc    = pstBcchPlmnId->ulMcc;
    stPlmnWithRat.stPlmnId.ulMnc    = pstBcchPlmnId->ulMnc;

    /* 遍历UE支持的接入技术,如果PLMN ID在禁止接入技术的黑名单网络里, 而且当前UE
       支持的接入技术都被禁止,则认为该PLMN ID是禁止网络 */
    for (ulIndex = 0; ulIndex < pstPrioRatList->ucRatNum; ulIndex++)
    {
        stPlmnWithRat.enRat   = pstPrioRatList->aucRatPrio[ulIndex];

        if (VOS_FALSE == NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList(&stPlmnWithRat))
        {
            ulDisabledPlmnFlg = VOS_FALSE;
            break;
        }
    }

    /* 该PLMN ID支持的接入技术都在禁止RAT的PLMN 列表中 */
    if (VOS_TRUE == ulDisabledPlmnFlg)
    {
        return VOS_TRUE;
    }

    for (ulIndex = 0; ulIndex < pstPrioRatList->ucRatNum; ulIndex++)
    {
        stPlmnWithRat.enRat   = pstPrioRatList->aucRatPrio[ulIndex];

        if (VOS_FALSE == NAS_MML_IsRatInForbiddenList(stPlmnWithRat.enRat))
        {
            ulRatDisabledFlg = VOS_FALSE;
            break;
        }
    }

    if (VOS_TRUE == ulRatDisabledFlg)
    {
        return VOS_TRUE;
    }

    /* 白名单个数为0表示未开启白名单特性,E5形态PLMN ID不在白名单中 */
    if (pstPlmnLockInfo->ucWhitePlmnLockNum > 0)
    {
        if (VOS_FALSE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstBcchPlmnId,
                                                               pstPlmnLockInfo->ucWhitePlmnLockNum,
                                                               pstPlmnLockInfo->astWhitePlmnId))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsPlmnIdInForbidPlmnList(
    NAS_MML_PLMN_ID_STRU                *pstPlmnId
)
{
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbidPlmnInfo = VOS_NULL_PTR;
    NAS_MML_MS_MODE_ENUM_UINT8          enMsMode;
    VOS_UINT32                          ulRet;

    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();
    enMsMode          = NAS_MML_GetMsMode();

    /* 在forbid plmn列表中 */
    if(VOS_TRUE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstPlmnId,
                                          pstForbidPlmnInfo->ucForbPlmnNum,
                                          pstForbidPlmnInfo->astForbPlmnIdList))
    {
        return VOS_TRUE;
    }

    /* 该PLMN ID是否在锁网锁卡的PLMN 列表中 */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInLockPlmnList(pstPlmnId))
    {
        return VOS_TRUE;
    }


    /* 不允许漫游时 */
    if (VOS_FALSE == NAS_MML_IsPlmnIdRoamingAllowed(pstPlmnId))
    {
        return VOS_TRUE;
    }

    ulRet = NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstPlmnId,
                                              pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                              pstForbidPlmnInfo->astForbGprsPlmnList);

    /* 在forbid plmn for gprs列表中 */
    if ( ( NAS_MML_MS_MODE_PS_ONLY == enMsMode)
      && ( VOS_TRUE == ulRet ))
    {
        return VOS_TRUE;
    }

    /* A模式时CS域卡无效时 */
    if ( ( VOS_FALSE             == NAS_MML_GetSimCsRegStatus())
      && ( NAS_MML_MS_MODE_PS_CS == enMsMode)
      && ( VOS_TRUE              == ulRet))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsRrcConnExist( VOS_VOID )
{
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrRat;

    /* 取得当前的接入模式 */
    enCurrRat     = NAS_MML_GetCurrNetRatType();

    /* 取得当前的链接信息 */
    pstConnStatus = NAS_MML_GetConnStatus();

    switch ( enCurrRat )
    {
#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:

            /* 当前模在L下，存在信令链接或者数据链接 */
            if ( (VOS_TRUE == pstConnStatus->ucEpsSigConnStatusFlg)
              || (VOS_TRUE == pstConnStatus->ucEpsServiceConnStatusFlg) )
            {
                return VOS_TRUE;
            }

            /* 不存在返回FALSE */
            return VOS_FALSE;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:

            /* 当前模在W下，存在信令链接或者数据链接 */
            if ( (VOS_TRUE == pstConnStatus->ucRrcStatusFlg)
              || (VOS_TRUE == pstConnStatus->ucPsSigConnStatusFlg)
              || (VOS_TRUE == pstConnStatus->ucCsSigConnStatusFlg) )
            {
                return VOS_TRUE;
            }

            /* 不存在返回FALSE */
            return VOS_FALSE;

        case NAS_MML_NET_RAT_TYPE_GSM:

            /* 当前模在G下，存在信令链接或者数据链接 */
            if ( (VOS_TRUE == pstConnStatus->ucRrcStatusFlg)
              || (VOS_TRUE == pstConnStatus->ucCsSigConnStatusFlg)
              || (VOS_TRUE == pstConnStatus->ucPsTbfStatusFlg) )
            {
                return VOS_TRUE;
            }

            /* 不存在返回FALSE */
            return VOS_FALSE;

        default:

            /* 无法解析的接入模式，打印异常，按照链接不存在返回 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MML_IsRrcConnExist,WARNING:无法解析的接入模式");

            return VOS_FALSE;
    }

}



VOS_UINT32  NAS_MML_IsQuickStartFlg( VOS_VOID )
{
    VOS_UINT8                           ucCardStatus;
    VOS_UINT8                           ucCardType;
    VOS_UINT32                          ulQuickStartFlag;


    ucCardStatus = USIMM_CARD_SERVIC_BUTT;
    ucCardType   = USIMM_CARD_NOCARD;

    if ( USIMM_API_SUCCESS != NAS_USIMMAPI_GetCardType(&ucCardStatus, &ucCardType) )
    {
        return VOS_TRUE;
    }
    if ( USIMM_CARD_ROM_SIM == ucCardType )
    {
        ulQuickStartFlag    = VOS_TRUE;
    }
    else
    {
        ulQuickStartFlag    = VOS_FALSE;
    }

    return ulQuickStartFlag;
}


VOS_UINT32 NAS_MML_IsTmsiValid(VOS_VOID)
{
    VOS_UINT8                          *pucTmsi = VOS_NULL_PTR;
    VOS_UINT32                          i;

    pucTmsi = NAS_MML_GetUeIdTmsi();

    for ( i = 0; i < NAS_MML_MAX_TMSI_LEN; i++ )
    {
        if ( NAS_MML_TMSI_INVALID != pucTmsi[i] )
        {
            return VOS_TRUE;
        }
    }


    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsPtmsiValid(VOS_VOID)
{
    VOS_UINT8                          *pucPtmsi = VOS_NULL_PTR;
    VOS_UINT32                          i;

    pucPtmsi = NAS_MML_GetUeIdPtmsi();

    for ( i = 0; i < NAS_MML_MAX_PTMSI_LEN; i++ )
    {
        if ( NAS_MML_PTMSI_INVALID != pucPtmsi[i] )
        {
            return VOS_TRUE;
        }
    }


    return VOS_FALSE;
}


VOS_UINT32  NAS_MML_GetGsmForbidFlg( VOS_VOID )
{
    NAS_MML_MISCELLANEOUS_CFG_INFO_STRU *pstMiscellaneousCfg = VOS_NULL_PTR;
    NAS_MML_SIM_TYPE_ENUM_UINT8          enSimType;
    VOS_UINT8                            ucSimPresentStatus;

    /* 快速开机模式直接返回 */
    if ( VOS_TRUE == NAS_MML_IsQuickStartFlg())
    {
        return VOS_FALSE;
    }

    pstMiscellaneousCfg = NAS_MML_GetMiscellaneousCfgInfo();
    enSimType           = NAS_MML_GetSimType();
    ucSimPresentStatus  = NAS_MML_GetSimPresentStatus();

    /* 卡类型为USIM且卡在位 */
    if ((NAS_MML_SIM_TYPE_USIM == enSimType)
     && (VOS_TRUE == ucSimPresentStatus))
    {
        /* USIM和NV中都指示禁止GSM接入时，需要记录在GSM下不能进行接入 */
        if (VOS_FALSE == NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_GSM_ACCESS_IN_USIM))
        {
            if ((VOS_FALSE == NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_GSM_SECURITY_CONTEXT))
            && (VOS_TRUE  == pstMiscellaneousCfg->ucNvGsmForbidFlg))
            {
                return VOS_TRUE;
            }
        }
    }

    /* 该NV已修改为W和G都为UINT32了, 原为UINT16, 即 aulSptBand[0]为W的BAND, aulSptBand[1]为G的BAND */

    if (0 == NAS_MML_GetGsmBandCapability())
    {
        return VOS_TRUE;
    }


    return VOS_FALSE;
}


VOS_VOID  NAS_MML_InitSimPlmnInfo(
    VOS_UINT32                          ulPlmnNum,
    NAS_MML_SIM_PLMN_WITH_RAT_STRU     *pstPlmnInfo
)
{
    VOS_UINT32                          i;

    for ( i = 0 ; i < ulPlmnNum ; i++ )
    {
        pstPlmnInfo[i].usSimRat         = NAS_MML_INVALID_SIM_RAT;
        pstPlmnInfo[i].stPlmnId.ulMcc   = NAS_MML_INVALID_MCC;
        pstPlmnInfo[i].stPlmnId.ulMnc   = NAS_MML_INVALID_MNC;
    }
}


VOS_VOID  NAS_MML_ResumeSimEhplmnList(
    VOS_UINT8                          *pucPlmnNum,
    NAS_MML_SIM_PLMN_WITH_RAT_STRU     *pstPlmnInfo
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulTempPlmnNum;
    NAS_MML_SIM_PLMN_WITH_RAT_STRU      astTempPlmnInfo[NAS_MML_MAX_EHPLMN_NUM];
    VOS_UINT16                          usUeSptRat;

    PS_MEM_SET(astTempPlmnInfo, 0x00, sizeof(astTempPlmnInfo));
#if (FEATURE_ON == FEATURE_LTE)
    usUeSptRat          = NAS_MML_SIM_UE_SUPPORT_RAT_SUPPORT_LTE;
#else
    usUeSptRat          = NAS_MML_SIM_UE_SUPPORT_RAT;
#endif

    /* 将要重新构建的PLMN WITH RAT信息拷贝到临时缓存中 */
    ulTempPlmnNum = *pucPlmnNum;
    PS_MEM_CPY(astTempPlmnInfo, pstPlmnInfo, sizeof(astTempPlmnInfo));

    /* 将源PLMN WITH RAT信息清空 */
    NAS_MML_InitSimPlmnInfo(*pucPlmnNum, pstPlmnInfo);
    *pucPlmnNum = 0;

    /* 将非重复的网络添加进入源PLMN WITH RAT 列表 */
    for ( i = 0 ; i < ulTempPlmnNum ; i++ )
    {
        /* 当前网络已经在原有的列表中了，则不添加  */
        if ( VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnWithRatList(&astTempPlmnInfo[i].stPlmnId,
                                                                      *pucPlmnNum,
                                                                       pstPlmnInfo) )
        {
            continue;
        }

        /* 当前网络不在原有的列表中了，则添加入源PLMN WITH RAT 列表 */
        pstPlmnInfo[*pucPlmnNum].stPlmnId = astTempPlmnInfo[i].stPlmnId;
        pstPlmnInfo[*pucPlmnNum].usSimRat = usUeSptRat;

        (*pucPlmnNum)++;
    }

    return;
}


VOS_UINT32 NAS_MML_IsUsimStausValid(VOS_VOID)
{
    NAS_MML_SIM_STATUS_STRU            *pstSimStatus    = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_ECALL)
    NAS_MML_CALL_MODE_ENUM_UINT8        enCallMode;

    enCallMode = NAS_MML_GetCallMode();
#endif

    pstSimStatus = NAS_MML_GetSimStatus();

    if ( VOS_FALSE == NAS_MML_GetSimPresentStatus() )
    {
        return VOS_FALSE;
    }

    if (( VOS_FALSE == pstSimStatus->ucSimPsRegStatus)
     && ( VOS_FALSE == pstSimStatus->ucSimCsRegStatus))
    {
        return VOS_FALSE;
    }

    if ((VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
     && (VOS_FALSE == pstSimStatus->ucSimCsRegStatus))
    {
        return VOS_FALSE;
    }

    /* 非ecall only模式CS不允许attach并且PS卡无效才认为卡无效 */
    if ((VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_FALSE == pstSimStatus->ucSimPsRegStatus)
#if (FEATURE_ON == FEATURE_ECALL)
     && (NAS_MML_ECALL_ONLY != enCallMode)
#endif
       )
    {
        return VOS_FALSE;
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* LTE单模的情况PS卡无效的时候也认为此时卡无效 */
    if ((VOS_TRUE  == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
     && (VOS_FALSE == pstSimStatus->ucSimPsRegStatus))
    {
        return VOS_FALSE;
    }
#endif
    return VOS_TRUE;

}


VOS_UINT32 NAS_MML_IsMccInDestMccList (
    VOS_UINT32                          ulMcc,
    VOS_UINT32                          ulMccNum,
    VOS_UINT32                         *pulMccList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulFlg;

    ulFlg = VOS_FALSE;

    for ( i = 0; i < ulMccNum; i++ )
    {
        if ( VOS_TRUE == NAS_MML_ComparePlmnMcc(ulMcc,
                                                  pulMccList[i]) )
        {
            ulFlg = VOS_TRUE;
            break;
        }
    }

    return (ulFlg);
}



VOS_VOID NAS_MML_GetMmlCsTransactionStatus(
    NAS_MML_CS_TRANSACTION_ENUM_UINT8                        *penAsCsTransactionsStatus
)
{
    /* 被叫就是CSFB的被叫*/
    if ( NAS_MML_CSFB_SERVICE_STATUS_MT_EXIST == NAS_MML_GetCsfbServiceStatus())
    {
        *penAsCsTransactionsStatus = NAS_MML_CS_TRANSACTION_MT_EXIST;

        return;
    }

    if (NAS_MML_TIMER_STATUS_RUNNING == NAS_MML_GetCsfbMtPagingTimerStatus())
    {
        *penAsCsTransactionsStatus = NAS_MML_CS_TRANSACTION_MT_EXIST;

        return;
    }

    if (NAS_MML_CSFB_MO_NORMAL_CALL_SETUP_START == NAS_MML_GetMoCallStatus())
    {
        *penAsCsTransactionsStatus = NAS_MML_CS_TRANSACTION_MO_NORMAL_EXIST;

        return;
    }

    if (NAS_MML_CSFB_MO_EMC_CALL_SETUP_START == NAS_MML_GetMoCallStatus())
    {
        *penAsCsTransactionsStatus = NAS_MML_CS_TRANSACTION_EMERGENCY_EXIST;

        return;
    }

    if (NAS_MML_CSFB_MO_SS_SETUP_START == NAS_MML_GetMoSsStatus())
    {
        *penAsCsTransactionsStatus = NAS_MML_CS_TRANSACTION_MO_NORMAL_EXIST;

        return;
    }

    if ( VOS_TRUE == NAS_MML_GetCsServiceExistFlg())
    {
        /* 存在紧急呼*/
        if ( VOS_TRUE == NAS_MML_GetCsEmergencyServiceFlg())
        {
            *penAsCsTransactionsStatus = NAS_MML_CS_TRANSACTION_EMERGENCY_EXIST;
        }
        /* 不存在紧急呼，普通呼叫，SS,SM都算作普通呼叫*/
        else
        {
            *penAsCsTransactionsStatus = NAS_MML_CS_TRANSACTION_MO_NORMAL_EXIST;
        }

        return;
    }

    /*不存在业务*/
    *penAsCsTransactionsStatus = NAS_MML_CS_TRANSACTION_NO_EXIST;

    return;
}




#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_MML_IsPlmnIdInCsDomainUnAvailPlmnList(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_ID_STRU               *pstPlmnList = VOS_NULL_PTR;
    NAS_LMM_INFO_STRU                   stLmmInfo;

    PS_MEM_SET(&stLmmInfo, 0x0, sizeof(NAS_LMM_INFO_STRU));


    /* 平台不支持LTE */
    if (VOS_FALSE == NAS_MML_IsPlatformSupportLte())
    {
        return VOS_FALSE;
    }

    /* 调用LNAS提供接口函数，成功返回VOS_OK */
    if (MMC_LMM_SUCC != Nas_GetLteInfo(NAS_LMM_CS_DOMAIN_UNAVAILABLE_PLMN_LIST, &stLmmInfo))
    {
        return VOS_FALSE;
    }

    /* 申请动态内存 */
    pstPlmnList = (NAS_MML_PLMN_ID_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC, (MMC_LMM_MAX_PLMN_NUM*sizeof(NAS_MML_PLMN_ID_STRU)));

    /* 如果申请失败，则返回 */
    if ( VOS_NULL_PTR == pstPlmnList )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MML_IsPlmnIdInCsDomainUnAvailPlmnList: Dynamic Memory Alloc Error");

        return VOS_FALSE;
    }

    NAS_MMC_ConvertLmmPlmnToGUNasPlmn(stLmmInfo.u.stCsDomainUnAvailPlmnList.ulPlmnNum,
                                      stLmmInfo.u.stCsDomainUnAvailPlmnList.astPlmnId,
                                      pstPlmnList);

    /* 在#18列表中 */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                         (VOS_UINT8)(stLmmInfo.u.stCsDomainUnAvailPlmnList.ulPlmnNum),
                                         pstPlmnList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC,"NAS_MML_IsPlmnIdInCsDomainUnAvailPlmnList: PLMN IN #18 LIST");

        /* 释放动态内存 */
        PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnList);

        return VOS_TRUE;
    }

    /* 释放动态内存 */
    PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnList);

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_GetLteRPlmn(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_ID_STRU                stRPlmnId;
    NAS_LMM_INFO_STRU                   stLmmInfoStru;

    PS_MEM_SET(&stRPlmnId,     0x00, sizeof(NAS_MML_PLMN_ID_STRU));
    PS_MEM_SET(&stLmmInfoStru, 0x00, sizeof(NAS_LMM_INFO_STRU));

    /* 获取L模的GUTI, 获取失败则返回 VOS_FALSE */
    /* 如果平台能力不支持lte,返回不存在LTE的RPLMN */
    if (VOS_FALSE == NAS_MML_IsPlatformSupportLte())
    {
        return VOS_FALSE;
    }

    if (MMC_LMM_SUCC != Nas_GetLteInfo(NAS_LMM_RPLMN, &stLmmInfoStru))
    {
        return VOS_FALSE;
    }

    /* 将GUTI中的PLMN转换为GUNAS使用的PLMN格式 */
    NAS_MMC_ConvertLmmPlmnToGUNasFormat(&(stLmmInfoStru.u.stPlmn), &(stRPlmnId));

    /* 若该PLMN有效且不是禁止网络则直接返回 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&stRPlmnId))
    {
        if (VOS_FALSE == NAS_MML_IsPlmnIdInForbidPlmnList(&stRPlmnId))
        {
            *pstPlmnId = stRPlmnId;

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsTaiInDestTaiList (
    NAS_MML_TAI_STRU                   *pstTai,
    VOS_UINT32                          ulTaiNum,
    NAS_LMM_TAI_STRU                   *pstTaiList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulIsTaiInTaiListFlg;
    NAS_MML_PLMN_ID_STRU                stPlmnId;
    VOS_UINT32                          ulIsSamePlmn;

    ulIsTaiInTaiListFlg = VOS_FALSE;

    for ( i = 0; i < ulTaiNum; i++ )
    {
        NAS_MMC_ConvertLmmPlmnToGUNasFormat(&pstTaiList[i].stPlmnId, &stPlmnId);

        ulIsSamePlmn = NAS_MML_CompareBcchPlmnwithSimPlmn(&pstTai->stPlmnId, &stPlmnId);

        if ((VOS_TRUE == ulIsSamePlmn)
         && (pstTai->stTac.ucTac == pstTaiList[i].stTac.ucTac)
         && (pstTai->stTac.ucTacCnt == pstTaiList[i].stTac.ucTacCnt))
        {
            ulIsTaiInTaiListFlg = VOS_TRUE;
            break;
        }
    }

    return ulIsTaiInTaiListFlg;
}


VOS_UINT32 NAS_MML_IsSupportLteCapability(VOS_VOID)
{
    VOS_UINT32                          ulIsLteRatSupportFlg;

    ulIsLteRatSupportFlg = NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE);

    if ((VOS_TRUE == NAS_MML_IsLteCapabilityDisabled(NAS_MML_GetLteCapabilityStatus(),NAS_MML_GetDisableLteRoamFlg()))
     || (VOS_FALSE == ulIsLteRatSupportFlg))
    {
        /* L处于disable状态或syscfg设置不支持L,返回VOS_FALSE*/
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


#endif

 
 VOS_UINT32 NAS_MML_GetPrefRPlmn(
     NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
     NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList;
    VOS_UINT32                          ulIsRplmnExist;

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    ulIsRplmnExist = VOS_TRUE;

    if (NAS_MML_NET_RAT_TYPE_LTE == enRat)
    {
        if (VOS_FALSE == NAS_MML_GetLteRPlmn(pstPlmnId))
        {
            ulIsRplmnExist = VOS_FALSE;

            if (VOS_FALSE == NAS_MML_IsLteOnlyMode(pstPrioRatList))
            {
                ulIsRplmnExist = NAS_MML_GetGURPlmn(pstPlmnId);
            }
        }
    }
    else
    {
        /* GU下注册被#2/#3拒绝之后会导致GURplmn不存在，但是不影响LRplmn，此时不宜使用LRplmn，否则GCF用例过不了 */
        ulIsRplmnExist = NAS_MML_GetGURPlmn(pstPlmnId);
    }

    return ulIsRplmnExist;
}



VOS_UINT32 NAS_MML_GetGU_CsRPlmn(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_ID_STRU                stRPlmnId;
    NAS_MML_LAI_STRU                   *pstLai = VOS_NULL_PTR;
    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8   enStatus;

    enStatus = NAS_MML_GetCsUpdateStatus();

    if ((NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED == enStatus)
     || (NAS_MML_LOCATION_UPDATE_STATUS_BUTT == enStatus))
    {
        return VOS_FALSE;
    }

    pstLai = NAS_MML_GetCsLastSuccLai();

    stRPlmnId = pstLai->stPlmnId;

    /* 若该PLMN有效且不在禁止网络中，返回该网络 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&stRPlmnId))
    {
        if (VOS_FALSE == NAS_MML_IsPlmnIdInForbidPlmnList(&stRPlmnId))
        {
           *pstPlmnId = stRPlmnId;

           return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MML_GetGU_PsRPlmn(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_ID_STRU                stRPlmnId;
    NAS_MML_RAI_STRU                   *pstRai = VOS_NULL_PTR;
    NAS_MML_ROUTING_UPDATE_STATUS_ENUM_UINT8    enStatus;

    enStatus = NAS_MML_GetPsUpdateStatus();

    if ((NAS_MML_ROUTING_UPDATE_STATUS_PLMN_NOT_ALLOWED == enStatus)
     || (NAS_MML_ROUTING_UPDATE_STATUS_BUTT == enStatus))
    {
        return VOS_FALSE;
    }

    pstRai  = NAS_MML_GetPsLastSuccRai();

    stRPlmnId = pstRai->stLai.stPlmnId;

    /* 若该PLMN有效且不在禁止网络中，返回该网络 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&stRPlmnId))
    {
        if (VOS_FALSE == NAS_MML_IsPlmnIdInForbidPlmnList(&stRPlmnId))
        {
           *pstPlmnId = stRPlmnId;

           return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MML_GetGURPlmn(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    if (NAS_MML_MS_MODE_PS_ONLY == NAS_MML_GetMsMode())
    {
        return NAS_MML_GetGU_PsRPlmn(pstPlmnId);
    }
    else if (NAS_MML_MS_MODE_CS_ONLY == NAS_MML_GetMsMode())
    {
        return NAS_MML_GetGU_CsRPlmn(pstPlmnId);
    }
    else
    {
        /* CS和PS模式下，先判断CS的Rplmn,再判断PS的Rplmn */
        if (VOS_TRUE == NAS_MML_GetGU_CsRPlmn(pstPlmnId))
        {
            return VOS_TRUE;
        }
        else if (VOS_TRUE == NAS_MML_GetGU_PsRPlmn(pstPlmnId))
        {
            return VOS_TRUE;
        }
        else
        {
            return VOS_FALSE;
        }
    }

}


VOS_UINT32 NAS_MML_GetEfLociPlmn(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_ID_STRU                stRPlmnId;
    NAS_MML_RAI_STRU                   *pstRai = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                   *pstLai = VOS_NULL_PTR;
    NAS_MML_ROUTING_UPDATE_STATUS_ENUM_UINT8    enPsStatus;
    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8   enCsStatus;

    pstRai      = NAS_MML_GetPsLastSuccRai();
    pstLai      = NAS_MML_GetCsLastSuccLai();
    enPsStatus  = NAS_MML_GetPsUpdateStatus();
    enCsStatus  = NAS_MML_GetCsUpdateStatus();

    if (NAS_MML_MS_MODE_PS_ONLY == NAS_MML_GetMsMode())
    {
        if (NAS_MML_ROUTING_UPDATE_STATUS_BUTT == enPsStatus)
        {
            return VOS_FALSE;
        }

        stRPlmnId = pstRai->stLai.stPlmnId;
    }
    else
    {
        if (NAS_MML_LOCATION_UPDATE_STATUS_BUTT == enCsStatus)
        {
            return VOS_FALSE;
        }

        stRPlmnId = pstLai->stPlmnId;
    }

    /* 若该PLMN有效且不在禁止网络中，返回该网络 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&stRPlmnId))
    {
        if (VOS_FALSE == NAS_MML_IsPlmnIdInForbidPlmnList(&stRPlmnId))
        {
           *pstPlmnId = stRPlmnId;

           return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_GetRPlmn(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_NET_RAT_TYPE_LTE == enRat)
    {
        return NAS_MML_GetLteRPlmn(pstPlmnId);
    }
#endif

    return NAS_MML_GetGURPlmn(pstPlmnId);

}


VOS_UINT32 NAS_MML_AddForbPlmn (
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    VOS_UINT32                          ulRlst;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbPlmnInfo = VOS_NULL_PTR;
    VOS_UINT8                           ucForbPlmnNum;
    VOS_UINT32                          ulChgFlag;

    ulChgFlag  = VOS_FALSE;

    ulRlst    = NAS_MML_ComparePlmnIdWithHplmn(pstPlmnId);

    if (VOS_TRUE == ulRlst )
    {
        return ulChgFlag;
    }

    /*添加ForbPlmn，并向Usim发送更新消息*/
    pstForbPlmnInfo = NAS_MML_GetForbidPlmnInfo();
    ucForbPlmnNum   = pstForbPlmnInfo->ucForbPlmnNum;

#ifndef DMT
    if (0 == pstForbPlmnInfo->ucUsimForbPlmnNum)
    {
        /* 全局变量中维护的卡FORB PLMN个数为0，后续写卡时分配内存必然失败，直接返回 */
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_AddForbPlmn: ucUsimForbPlmnNum is 0, return FALSE");
        return ulChgFlag;
    }
#endif


    /* 在forbid plmn列表中 */
    ulRlst = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                          pstForbPlmnInfo->ucForbPlmnNum,
                                          pstForbPlmnInfo->astForbPlmnIdList);

#ifndef DMT
    if (VOS_TRUE == ulRlst)
    {
        ulChgFlag  = VOS_TRUE;
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_AddForbPlmn: Added PLMN is already in the Forbid PLMN list");
    }
#endif
    
    if (VOS_FALSE == ulRlst)
    {
        /* 列表已满时，删除最先保存的元素 */
        if (( NAS_MML_MAX_FORBPLMN_NUM == ucForbPlmnNum )
         || ((pstForbPlmnInfo->ucUsimForbPlmnNum == ucForbPlmnNum)
          && (0 < ucForbPlmnNum)))
        {
            (VOS_VOID)PS_MEM_MOVE(pstForbPlmnInfo->astForbPlmnIdList,
                        (pstForbPlmnInfo->astForbPlmnIdList + 1),
                        (NAS_MML_MAX_FORBPLMN_NUM -1) * sizeof(NAS_MML_PLMN_ID_STRU));

            ucForbPlmnNum--;
        }

        pstForbPlmnInfo->astForbPlmnIdList[ucForbPlmnNum].ulMcc
                                                       = pstPlmnId->ulMcc;
        pstForbPlmnInfo->astForbPlmnIdList[ucForbPlmnNum].ulMnc
                                                       = pstPlmnId->ulMnc;

        ucForbPlmnNum++;

        pstForbPlmnInfo->ucForbPlmnNum = ucForbPlmnNum;

        ulChgFlag = VOS_TRUE;

    }

    return ulChgFlag;
}

VOS_UINT32 NAS_MML_DelForbPlmn (
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulIdx;
    VOS_UINT32                          ulChgFlag;

    ulChgFlag  = VOS_FALSE;

    pstForbPlmnInfo   = NAS_MML_GetForbidPlmnInfo();

#ifndef DMT
    ulChgFlag  = VOS_TRUE;

    if (0 == pstForbPlmnInfo->ucUsimForbPlmnNum)
    {
        /* 全局变量中维护的卡FORB PLMN个数为0，后续写卡时分配内存必然失败，直接返回 */
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_DelForbPlmn: ucUsimForbPlmnNum is 0, return FALSE");
        return VOS_FALSE;
    }
#endif

    /* 在forbid plmn列表中 */
    ulIdx = NAS_MML_GetPlmnIdxInList(pstPlmnId,
                    pstForbPlmnInfo->ucForbPlmnNum, pstForbPlmnInfo->astForbPlmnIdList);

    /* 找到对应的PlmnId,删除，更新ForbPlmn，并向Usim发送更新消息*/
    if (ulIdx < pstForbPlmnInfo->ucForbPlmnNum)
    {
        if (ulIdx == (pstForbPlmnInfo->ucForbPlmnNum -1))
        {
            pstForbPlmnInfo->astForbPlmnIdList[ulIdx].ulMcc = NAS_MML_INVALID_MCC;
            pstForbPlmnInfo->astForbPlmnIdList[ulIdx].ulMnc = NAS_MML_INVALID_MNC;
        }
        else
        {
            (VOS_VOID)PS_MEM_MOVE(pstForbPlmnInfo->astForbPlmnIdList + ulIdx,
                 (pstForbPlmnInfo->astForbPlmnIdList + (ulIdx + 1)),
                 (NAS_MML_MAX_FORBPLMN_NUM - (ulIdx + 1)) * sizeof(NAS_MML_PLMN_ID_STRU));
        }

        pstForbPlmnInfo->ucForbPlmnNum --;

        ulChgFlag  = VOS_TRUE;

    }

    return ulChgFlag;
}


VOS_VOID NAS_MML_AddForbGprsPlmn(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbPlmnInfo = VOS_NULL_PTR;
    VOS_UINT8                           ucForbGprsPlmnNum;
    VOS_UINT32                          ulRlst;

    ulRlst = VOS_FALSE;

    /* 用于获取禁止GPRS PLMN lsit */
    pstForbPlmnInfo = NAS_MML_GetForbidPlmnInfo();

    /* Ts23.122 Ch 3.1 The HPLMN (if the EHPLMN list is not present or is empty)
    or an EHPLMN (if the EHPLMN list is present) shall not be stored
    on the list of "forbidden PLMNs for GPRS service"
    相关测试用例: ts34.123 GCF12.3.2.8 cellA in MCC2/MNC1/LAC1/RAC1(RAI-2, Not HPLMN) */
    ulRlst = NAS_MML_ComparePlmnIdWithHplmn(pstPlmnId);
    if (VOS_TRUE == ulRlst)
    {
        return;
    }

    /* 该PLMN已在禁止GPRS PLMN lsit中，则不添加 */
    ulRlst = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                            pstForbPlmnInfo->ucForbGprsPlmnNum,
                                            pstForbPlmnInfo->astForbGprsPlmnList);


    if (VOS_TRUE == ulRlst)
    {
        return;
    }

    /* 获取禁止GPRS PLMN 的个数 */
    ucForbGprsPlmnNum   = pstForbPlmnInfo->ucForbGprsPlmnNum;

    /* 列表已满时，移出最先保存的元素 */
    if ( NAS_MML_MAX_FORBGPRSPLMN_NUM == ucForbGprsPlmnNum )
    {
        (VOS_VOID)PS_MEM_MOVE(&(pstForbPlmnInfo->astForbGprsPlmnList[0]),
                    &(pstForbPlmnInfo->astForbGprsPlmnList[1]),
                     (ucForbGprsPlmnNum-1) * sizeof(NAS_MML_PLMN_ID_STRU));
        ucForbGprsPlmnNum--;
    }

    /* 添加禁止GPRS PLMN到列表的最后 */
    pstForbPlmnInfo->astForbGprsPlmnList[ucForbGprsPlmnNum].ulMcc
                                                   = pstPlmnId->ulMcc;
    pstForbPlmnInfo->astForbGprsPlmnList[ucForbGprsPlmnNum].ulMnc
                                                   = pstPlmnId->ulMnc;

    /* 更新禁止GPRS PLMN的个数 */
    ucForbGprsPlmnNum++;
    pstForbPlmnInfo->ucForbGprsPlmnNum = ucForbGprsPlmnNum;

    return;
}


VOS_VOID NAS_MML_DelForbGprsPlmn (
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulIdx;

    pstForbPlmnInfo   = NAS_MML_GetForbidPlmnInfo();

    /* 在forbid plmn for Gprs列表中 */
    ulIdx = NAS_MML_GetPlmnIdxInList(pstPlmnId,
                    pstForbPlmnInfo->ucForbGprsPlmnNum, pstForbPlmnInfo->astForbGprsPlmnList);

    /* 找到对应的PlmnId,删除，更新ForbPlmn */
    if (ulIdx < pstForbPlmnInfo->ucForbGprsPlmnNum)
    {
        if (ulIdx == (pstForbPlmnInfo->ucForbGprsPlmnNum -1))
        {
            pstForbPlmnInfo->astForbGprsPlmnList[ulIdx].ulMcc = NAS_MML_INVALID_MCC;
            pstForbPlmnInfo->astForbGprsPlmnList[ulIdx].ulMnc = NAS_MML_INVALID_MNC;
        }
        else
        {
            (VOS_VOID)PS_MEM_MOVE(pstForbPlmnInfo->astForbGprsPlmnList + ulIdx,
                 (pstForbPlmnInfo->astForbGprsPlmnList + (ulIdx + 1)),
                 (NAS_MML_MAX_FORBGPRSPLMN_NUM - (ulIdx + 1)) * sizeof(NAS_MML_PLMN_ID_STRU));
        }

        pstForbPlmnInfo->ucForbGprsPlmnNum --;

    }

    return;
}



VOS_VOID NAS_MML_AddForbRoamLa (
    NAS_MML_LAI_STRU                                       *pstLai,
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU               *pstForbRoamLaInfo
)
{
    NAS_MML_MISCELLANEOUS_CFG_INFO_STRU                    *pstMiscellaneousCfgInfo = VOS_NULL_PTR;
    VOS_UINT8                                               ucRealMaxForbRoamLaNum;
    VOS_UINT8                                               ucForbRoamLaNum;
    VOS_UINT32                                              ulRlst;

    ulRlst = VOS_FALSE;

    /* 已在"Forbidden las for roaming" list中，则不添加 */
    ulRlst = NAS_MML_IsLaiInDestLaiList(pstLai, pstForbRoamLaInfo->ucForbRoamLaNum,
                                        pstForbRoamLaInfo->astForbRomLaList);
    if (VOS_TRUE == ulRlst)
    {
        return;
    }

    /* 用于获取NV中保存的最大禁止LA个数信息 */
    pstMiscellaneousCfgInfo = NAS_MML_GetMiscellaneousCfgInfo();

    /* 结合NV中的最大禁止LA个数信息获取有效的最大禁止个数 */
    ucRealMaxForbRoamLaNum = NAS_MML_MAX_FORBLA_FOR_ROAM_NUM;
    if (VOS_TRUE == pstMiscellaneousCfgInfo->ucMaxForbRoamLaFlg)
    {
        if (pstMiscellaneousCfgInfo->ucMaxForbRoamLaNum <= NAS_MML_MAX_FORBLA_FOR_ROAM_NUM)
        {
            ucRealMaxForbRoamLaNum = pstMiscellaneousCfgInfo->ucMaxForbRoamLaNum;
        }
    }

    if (0 == ucRealMaxForbRoamLaNum )
    {
        return;
    }

    /* 获取当前列表中的禁止LA个数 */
    ucForbRoamLaNum = pstForbRoamLaInfo->ucForbRoamLaNum;

    /* 列表已满时，移出最先保存的元素 */
    if (ucRealMaxForbRoamLaNum == ucForbRoamLaNum)
    {
        (VOS_VOID)PS_MEM_MOVE(&(pstForbRoamLaInfo->astForbRomLaList[0]),
                    &(pstForbRoamLaInfo->astForbRomLaList[1]),
                     (ucForbRoamLaNum - 1) * sizeof(NAS_MML_LAI_STRU ) );
        ucForbRoamLaNum--;
    }

    /* 添加Forbidden for roaming LA到列表的最后 */
    pstForbRoamLaInfo->astForbRomLaList[ucForbRoamLaNum] = (*pstLai);

    /* 更新当前列表中的禁止LA个数 */
    ucForbRoamLaNum++;
    pstForbRoamLaInfo->ucForbRoamLaNum = ucForbRoamLaNum;

    return;
}




VOS_VOID NAS_MML_AddForbRegLa (
    NAS_MML_LAI_STRU                   *pstLai
)
{
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbPlmnInfo = VOS_NULL_PTR;
    VOS_UINT8                           ucForbRegLaNum;
    VOS_UINT32                          ulRlst;

    ulRlst = VOS_FALSE;

    /* 用于获取"Forbidden LA for regional provision" list */
    pstForbPlmnInfo = NAS_MML_GetForbidPlmnInfo();

    /* 已在"Forbidden LA for regional provision" list，则不添加 */
    ulRlst = NAS_MML_IsLaiInDestLaiList(pstLai, pstForbPlmnInfo->ucForbRegLaNum,
                                        pstForbPlmnInfo->astForbRegLaList);
    if (VOS_TRUE == ulRlst)
    {
        return;
    }

    /* 获取当前列表中Forbidden LA的个数 */
    ucForbRegLaNum = pstForbPlmnInfo->ucForbRegLaNum;

    /* 列表已满，则移出最先保存的元素 */
    if (NAS_MML_MAX_FORBLA_FOR_REG_NUM == ucForbRegLaNum)
    {
        (VOS_VOID)PS_MEM_MOVE(&(pstForbPlmnInfo->astForbRegLaList[0]),
                    &(pstForbPlmnInfo->astForbRegLaList[1]),
                    (ucForbRegLaNum-1)*sizeof(NAS_MML_LAI_STRU));
        ucForbRegLaNum--;
    }

    /* 添加Forbidden LA for regional provision到列表的最后 */
    pstForbPlmnInfo->astForbRegLaList[ucForbRegLaNum] = (*pstLai);

    /* 更新当前列表中的禁止LA个数 */
    ucForbRegLaNum++;
    pstForbPlmnInfo->ucForbRegLaNum = ucForbRegLaNum;

    return;
}


VOS_UINT32 NAS_MML_DelForbLa (
    NAS_MML_LAI_STRU                   *pstLai,
    NAS_MML_REG_DOMAIN_ENUM_UINT8       enDomain
)
{
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU             *pstForbLaWithValidPeriodList   = VOS_NULL_PTR;

    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                        *pstForbPlmnInfo                = VOS_NULL_PTR;
    VOS_UINT32                                              ulRlst;
    VOS_UINT8                                               ucLaiIndex;

    VOS_UINT32                                              ulIsLaInForbLaWithValidPeriodList;

    ulRlst                              = VOS_FALSE;
    ulIsLaInForbLaWithValidPeriodList   = VOS_FALSE;
    /* 用于获取禁止LA的信息 */
    pstForbPlmnInfo = NAS_MML_GetForbidPlmnInfo();

    /* Forbidden LA For Roaming发生改变才需要通知接入层,应该先判断Forbidden LA for regional provision，后判断
       Forbidden las for roaming */

    /* 在"Forbidden LA for regional provision" list 中查找要删除的LAI */
    ulRlst = NAS_MML_GetLaiIndexInDestLaiList(pstLai, &ucLaiIndex,
                                              pstForbPlmnInfo->ucForbRegLaNum,
                                              pstForbPlmnInfo->astForbRegLaList);
    /* 查找到LAI则进行删除操作 */
    if(VOS_TRUE == ulRlst)
    {
        if (ucLaiIndex < (pstForbPlmnInfo->ucForbRegLaNum - 1))
        {
            (VOS_VOID)PS_MEM_MOVE(&(pstForbPlmnInfo->astForbRegLaList[ucLaiIndex]),
                         &(pstForbPlmnInfo->astForbRegLaList[ucLaiIndex + 1]),
                         ((pstForbPlmnInfo->ucForbRegLaNum-ucLaiIndex) - 1) * sizeof(NAS_MML_LAI_STRU));
        }

        /* 删除"Forbidden LA for regional provision后，更新个数 */
        pstForbPlmnInfo->ucForbRegLaNum--;
    }

    if (NAS_MML_REG_DOMAIN_CS == enDomain)
    {
        pstForbLaWithValidPeriodList = NAS_MML_GetForbLaWithValidPeriodList();
        ulIsLaInForbLaWithValidPeriodList = NAS_MML_GetLaiIndexInRegFailForbLaList(pstLai, &ucLaiIndex,
                                                           pstForbLaWithValidPeriodList->ulForbLaWithValidPeriodNum,
                                                           pstForbLaWithValidPeriodList->astForbLaWithValidPeriodList);
        /* 查找到LAI则进行删除操作 */
        if (VOS_TRUE == ulIsLaInForbLaWithValidPeriodList)
        {
            NAS_MML_DeleteLaFromForbLaWithValidPeriodList(pstForbLaWithValidPeriodList, ucLaiIndex);

            /* 启动定时器 */
            NAS_MML_StartCustomizedForbLaTimer();
        }
    }

    /* 在"Forbidden las for roaming" list 中查找要删除的LAI */
    ulRlst = NAS_MML_GetLaiIndexInDestLaiList(pstLai, &ucLaiIndex,
                                              pstForbPlmnInfo->ucForbRoamLaNum,
                                              pstForbPlmnInfo->astForbRomLaList);

    /* 查找到LAI则进行删除操作 */
    if (VOS_TRUE == ulRlst)
    {
        /* 删除指定的LAI */
        if (ucLaiIndex < (pstForbPlmnInfo->ucForbRoamLaNum - 1))
        {
            (VOS_VOID)PS_MEM_MOVE(&(pstForbPlmnInfo->astForbRomLaList[ucLaiIndex]),
                         &(pstForbPlmnInfo->astForbRomLaList[ucLaiIndex + 1]),
                         ((pstForbPlmnInfo->ucForbRoamLaNum - ucLaiIndex) - 1) * sizeof(NAS_MML_LAI_STRU));
        }

        /* 删除Forbidden LA For Roam后, 更新个数 */
        pstForbPlmnInfo->ucForbRoamLaNum--;
    }
    return (ulRlst | ulIsLaInForbLaWithValidPeriodList);
}


VOS_VOID NAS_MML_DelNotAllowRoamPlmnInList(
    VOS_UINT8                          *pucPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstPlmnList
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;

    ucPlmnNum = *pucPlmnNum;


    i = 0;
    while ( i < ucPlmnNum )
    {
        if (VOS_FALSE == NAS_MML_IsPlmnIdRoamingAllowed(&pstPlmnList[i]))
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pstPlmnList[i], &pstPlmnList[i+1],
                            ((ucPlmnNum - i)-1) * sizeof(NAS_MML_PLMN_ID_STRU));
            }

            ucPlmnNum--;

            continue;
        }

        i++;
    }


    *pucPlmnNum = ucPlmnNum;

    return;
}



VOS_UINT32 NAS_MML_DelForbPlmnInList(
    VOS_UINT32                          ulPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pHandlingPlmnList
)
{
    VOS_UINT8                           ucLeftPlmnNum;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbidPlmnInfo  = VOS_NULL_PTR;

    NAS_MML_MS_MODE_ENUM_UINT8          enMsMode;

    NAS_MML_PLMN_LOCK_CFG_INFO_STRU    *pstPlmnLockCfg     = VOS_NULL_PTR;


    /* 用于获取黑白名单 */
    pstPlmnLockCfg = NAS_MML_GetPlmnLockCfg();

    enMsMode = NAS_MML_GetMsMode();

    ucLeftPlmnNum     = (VOS_UINT8)ulPlmnNum;
    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();

    /* 用于获取黑白名单 */

    /* 删除存在于黑名单中的网络 */
    NAS_MML_DelPlmnsInRefList(&ucLeftPlmnNum, pHandlingPlmnList,
                   pstPlmnLockCfg->ucBlackPlmnLockNum, pstPlmnLockCfg->astBlackPlmnId);

    /* 删除不存在于白名单中的网络 */
    if (pstPlmnLockCfg->ucWhitePlmnLockNum != 0)
    {
        NAS_MML_DelPlmnsNotInRefList(&ucLeftPlmnNum, pHandlingPlmnList,
                       pstPlmnLockCfg->ucWhitePlmnLockNum, pstPlmnLockCfg->astWhitePlmnId);
    }

    /* 删除禁止网络的网络,用户指定搜的PLMN在禁止列表时，注册成功后需要在MMC删除该PLMN，
    为了规避时序造成的删除了已经注册成功的网络，所以此处不能删除当前驻留的网络 */
    NAS_MML_DelEqualPlmnsInForbList(&ucLeftPlmnNum, pHandlingPlmnList,
                                pstForbidPlmnInfo->ucForbPlmnNum, pstForbidPlmnInfo->astForbPlmnIdList);

    /* CS卡无效或PS ONLY时删除禁止GPRS列表中的网络 */
    if ((VOS_FALSE == NAS_MML_GetSimCsRegStatus())
     || (NAS_MML_MS_MODE_PS_ONLY == enMsMode))
    {

        /* 删除禁止网络的网络,用户指定搜的PLMN在禁止列表时，注册成功后需要在MMC删除该PLMN，
        为了规避时序造成的删除了已经注册成功的网络，所以此处不能删除当前驻留的网络 */
        NAS_MML_DelEqualPlmnsInForbList(&ucLeftPlmnNum, pHandlingPlmnList,
                                        pstForbidPlmnInfo->ucForbGprsPlmnNum, pstForbidPlmnInfo->astForbGprsPlmnList);
    }

    /* 从列表中删除不允许漫游的网络 */
    NAS_MML_DelNotAllowRoamPlmnInList(&ucLeftPlmnNum, pHandlingPlmnList);

    return ucLeftPlmnNum;

}


VOS_UINT32 NAS_MML_DelInvalidPlmnFromList(
    VOS_UINT32                          ulPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstHandlingPlmnList
)
{

    VOS_UINT32                          i;

    /* 删除该列表中的无效网络 */


    i = 0;
    while ( i < ulPlmnNum )
    {
        if (VOS_FALSE == NAS_MML_IsPlmnIdValid(pstHandlingPlmnList + i))
        {
            if (i < (ulPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pstHandlingPlmnList [i], &pstHandlingPlmnList[i+1],
                            ((ulPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_ID_STRU) );
            }
            ulPlmnNum--;
            continue;
        }

        i++;
    }


    return ulPlmnNum;

}


VOS_VOID NAS_MML_DelPlmnsInRefList(
    VOS_UINT8                          *pHandlingPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pHandlingPlmnList,
    VOS_UINT8                           ucRefPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pRefPlmnList
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;
    VOS_UINT32                          ulRlst;

    ucPlmnNum = *pHandlingPlmnNum;


    i = 0;
    while ( i < ucPlmnNum )
    {
        ulRlst = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pHandlingPlmnList + i, ucRefPlmnNum, pRefPlmnList);

        if ( VOS_TRUE == ulRlst )
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pHandlingPlmnList [i], &pHandlingPlmnList[ i + 1 ],
                            ((ucPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_ID_STRU));
            }
            ucPlmnNum--;
            continue;
        }
        i++;
    }


    *pHandlingPlmnNum = ucPlmnNum;

    return;
}


VOS_VOID NAS_MML_DelPlmnsNotInRefList(
    VOS_UINT8                          *pHandlingPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pHandlingPlmnList,
    VOS_UINT8                           ucRefPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pRefPlmnList
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;
    VOS_UINT32                          ulRlst;

    ucPlmnNum = *pHandlingPlmnNum;


    i = 0;
    while ( i < ucPlmnNum )
    {
        ulRlst = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pHandlingPlmnList + i, ucRefPlmnNum, pRefPlmnList);

        /* 不存在于pRefPlmnList中，则删除 */
        if ( VOS_FALSE == ulRlst )
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pHandlingPlmnList [i], &pHandlingPlmnList[ i + 1 ],
                            ((ucPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_ID_STRU));
            }

            ucPlmnNum--;
            continue;
        }
        i++;
    }


    *pHandlingPlmnNum = ucPlmnNum;

    return;
}


VOS_VOID NAS_MML_DelSpecPlmnWithRatInPlmnListWithRat(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstSpecPlmnWithRat,
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstPlmnListWithRat
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;
    VOS_UINT32                          ulRlst;

    ucPlmnNum = (VOS_UINT8)NAS_MML_MIN(pstPlmnListWithRat->ulPlmnNum, NAS_MML_MAX_EHPLMN_NUM);

    i = 0;
    while ( i < ucPlmnNum )
    {
        ulRlst = NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmnListWithRat->astPlmnId[i].stPlmnId), &(pstSpecPlmnWithRat->stPlmnId));

        /* PLMN相同并且RAT相同，从列表中删除 */
        if (( VOS_TRUE == ulRlst )
         && ( pstPlmnListWithRat->astPlmnId[i].enRat == pstSpecPlmnWithRat->enRat))
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&(pstPlmnListWithRat->astPlmnId[i]), &(pstPlmnListWithRat->astPlmnId[i + 1]),
                            ((ucPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
            }
            ucPlmnNum--;
            continue;
        }
        i++;
    }

    /* 更新NUM */
    pstPlmnListWithRat->ulPlmnNum = ucPlmnNum;

    return;
}



VOS_UINT32 NAS_MML_GetPlmnIdxInList (
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    VOS_UINT32                          ulPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstPlmnIdList
)
{
    VOS_UINT32                          i;

    for ( i = 0; i < ulPlmnNum; i++ )
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId, (pstPlmnIdList + i)))
        {
            break;
        }
    }

    return i;
}


VOS_UINT32 NAS_MML_IsGsmOnlyMode(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatOrder
)
{
    if ( (1                        == pstRatOrder->ucRatNum)
      && (NAS_MML_NET_RAT_TYPE_GSM == pstRatOrder->aucRatPrio[0]) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsWcdmaOnlyMode(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatOrder
)
{
    if ( (1                          == pstRatOrder->ucRatNum)
      && (NAS_MML_NET_RAT_TYPE_WCDMA == pstRatOrder->aucRatPrio[0]) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsWcdmaPrioGsm(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatOrder
)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT8                           ucWcdmaPrioGsmFlag;

    ucWcdmaPrioGsmFlag = VOS_FALSE;

    for (ulIndex = 0; ulIndex < pstRatOrder->ucRatNum; ulIndex++)
    {
        if (NAS_MML_NET_RAT_TYPE_GSM == pstRatOrder->aucRatPrio[ulIndex])
        {
            ucWcdmaPrioGsmFlag = VOS_FALSE;
            return ucWcdmaPrioGsmFlag;
        }

        if (NAS_MML_NET_RAT_TYPE_WCDMA == pstRatOrder->aucRatPrio[ulIndex])
        {
            ucWcdmaPrioGsmFlag = VOS_TRUE;
            return ucWcdmaPrioGsmFlag;
        }
    }

    return ucWcdmaPrioGsmFlag;
}



VOS_UINT32 NAS_MML_IsLteOnlyMode(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatOrder
)
{
    if ( (1                        == pstRatOrder->ucRatNum)
      && (NAS_MML_NET_RAT_TYPE_LTE == pstRatOrder->aucRatPrio[0]) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_VOID NAS_MML_RemoveRoamPlmnInSrchList(
    VOS_UINT8                          *pucPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstPlmnList
)
{
    VOS_UINT32                          i;

    for ( i = 0; i < *pucPlmnNum; )
    {
        /* 如果列表中的PLMN不准许漫游，则将该PLMN移除 */
        if ( VOS_FALSE == NAS_MML_IsPlmnIdRoamingAllowed(&pstPlmnList[i]) )
        {
            /* 如果不是最后一个元素，则移除 */
            if ( (i + 1) != (*pucPlmnNum) )
            {
                (VOS_VOID)PS_MEM_MOVE(&pstPlmnList[i], &pstPlmnList[i+1],
                            ((*pucPlmnNum - i) - 1)*sizeof(NAS_MML_PLMN_ID_STRU));

            }

            /* 将总个数减一 */
            (*pucPlmnNum)--;

            continue;
        }
        i++;
    }

    return;
}

VOS_UINT32 NAS_MMC_IsRoam()
{
    NAS_MML_PLMN_ID_STRU               *pstCurplmn = VOS_NULL_PTR;

    pstCurplmn = NAS_MML_GetCurrCampPlmnId();

    /*与HPLMN相同，直接返回允许*/
    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithHplmn(pstCurplmn))
    {
        return VOS_FALSE;
    }
    else
    {
        return VOS_TRUE;
    }
}


VOS_UINT32 NAS_MML_IsHplmnInEplmnList()
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnList = VOS_NULL_PTR;
    VOS_UINT32                          i;

    pstEplmnList = NAS_MML_GetEquPlmnList();

    if (VOS_TRUE == pstEplmnList->ucValidFlg)
    {
        for (i = 0; i < pstEplmnList->ucEquPlmnNum; i++)
        {
            if (VOS_TRUE == NAS_MML_ComparePlmnIdWithHplmn(&pstEplmnList->astEquPlmnAddr[i]))
            {
                return VOS_TRUE;
            }
        }
    }
    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsCurrCampPlmnInEplmnList(VOS_VOID)
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnList        = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU               *pstCurrCampPlmnId   = VOS_NULL_PTR;
    VOS_UINT32                          i;

    pstEplmnList              = NAS_MML_GetEquPlmnList();
    pstCurrCampPlmnId         = NAS_MML_GetCurrCampPlmnId();

    if (VOS_TRUE == pstEplmnList->ucValidFlg)
    {
        for (i = 0; i < pstEplmnList->ucEquPlmnNum; i++)
        {
            if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstCurrCampPlmnId, &pstEplmnList->astEquPlmnAddr[i]))
            {
                return VOS_TRUE;
            }
        }
    }
    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsPlmnInEplmnList(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnList        = VOS_NULL_PTR;
    VOS_UINT32                          i;

    pstEplmnList = NAS_MML_GetEquPlmnList();

    if (VOS_TRUE == pstEplmnList->ucValidFlg)
    {
        for (i = 0; i < pstEplmnList->ucEquPlmnNum; i++)
        {
            if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId, &pstEplmnList->astEquPlmnAddr[i]))
            {
                return VOS_TRUE;
            }
        }
    }
    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_GetLaiForbType(
    NAS_MML_LAI_STRU                   *pstLai
)
{
    VOS_UINT32                          ulPlmnLaType;
    NAS_MML_PLMN_ID_STRU               *pstPlmnId         = VOS_NULL_PTR;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbidPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulRlst;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_LMM_INFO_STRU                   stLmmInfo;
    NAS_MML_TAI_STRU                    stTai;
    VOS_UINT32                          ulForbidFlag;

    VOS_UINT32                          ulForbiddenRegLaFlag;
#endif

    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    ulPlmnLaType      = NAS_MML_PLMN_FORBID_NULL;
    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();
    pstPlmnId         = &(pstLai->stPlmnId);


    /* 在forbid plmn列表中 */
    ulRlst = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                          pstForbidPlmnInfo->ucForbPlmnNum,
                                          pstForbidPlmnInfo->astForbPlmnIdList);
    if(VOS_TRUE == ulRlst)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN:IN FORB PLMN LIST");

        return NAS_MML_PLMN_FORBID_PLMN;
    }


    /* 构造当前接入技术的PLMN ID */
    stPlmnWithRat.stPlmnId.ulMcc    = pstLai->stPlmnId.ulMcc;
    stPlmnWithRat.stPlmnId.ulMnc    = pstLai->stPlmnId.ulMnc;
    stPlmnWithRat.enRat             = pstLai->enCampPlmnNetRat;

    /* 判断当前PLMN是否在禁止RAT的PLMN列表中同时当前接入技术被禁止,则认为当前PLMN为禁止网络 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList(&stPlmnWithRat))
    {

        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN:IN FORB PLMN WITH PERIOD LIST");

        return NAS_MML_PLMN_FORBID_PLMN;
    }

    /* 判断PLMN的RAT在禁止接入技术列表中，则认为当前RAT对应网络为禁止网络 */
    if (VOS_TRUE == NAS_MML_IsRatInForbiddenList(stPlmnWithRat.enRat))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN:IN FORB RAT LIST");

        return NAS_MML_PLMN_FORBID_PLMN;
    }


    /* 该PLMN ID是否在锁网锁卡的PLMN 列表中 */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInLockPlmnList(pstPlmnId))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN:IN LockPlmnList");

        return NAS_MML_PLMN_FORBID_PLMN;
    }

    /* 不允许漫游时 */
    if (VOS_FALSE == NAS_MML_IsPlmnIdRoamingAllowed(pstPlmnId))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN:PlmnIdRoamingAllowed");

        return NAS_MML_PLMN_FORBID_PLMN;
    }

    /* 在forbid plmn for gprs列表中 */
    ulRlst = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                          pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                          pstForbidPlmnInfo->astForbGprsPlmnList);
    if(VOS_TRUE == ulRlst)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType:IN ForbGprsPlmnList");

        ulPlmnLaType  |=   NAS_MML_PLMN_FORBID_PLMN_FOR_GPRS;
    }

    /* L下TAI需要从LAI中获取，收到L的系统消息，更新的是LAI */
#if (FEATURE_ON == FEATURE_LTE)
    ulForbidFlag         = VOS_FALSE;

    ulForbiddenRegLaFlag = NAS_MML_PLMN_FORBID_NULL;

    stTai.stPlmnId.ulMcc = pstLai->stPlmnId.ulMcc;
    stTai.stPlmnId.ulMnc = pstLai->stPlmnId.ulMnc;
    stTai.stTac.ucTac    = pstLai->aucLac[0];
    stTai.stTac.ucTacCnt = pstLai->aucLac[1];

    /* UT用例里用的是桩，初始化一下，否则是随机值，会exception */
    PS_MEM_SET(&stLmmInfo, 0, sizeof(NAS_LMM_INFO_STRU));

    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        if ( MMC_LMM_SUCC == Nas_GetLteInfo(NAS_LMM_FORB_TAI_ROAM_LIST, &stLmmInfo) )
        {
            ulForbidFlag = NAS_MML_IsTaiInDestTaiList (&stTai,
                                                        stLmmInfo.u.stForbRoamTaiList.ulTaNum,
                                                        stLmmInfo.u.stForbRoamTaiList.astTa);
        }

        if (VOS_TRUE == ulForbidFlag)
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN:IN stForbRoamTaiList");

            ulPlmnLaType |= NAS_MML_PLMN_FORBID_PLMN;
        }

        if ( MMC_LMM_SUCC == Nas_GetLteInfo(NAS_LMM_FORB_TAI_RPOS_LIST, &stLmmInfo) )
        {
            ulForbiddenRegLaFlag = NAS_MML_IsTaiInDestTaiList (&stTai,
                                                        stLmmInfo.u.stForbRposTaiList.ulTaNum,
                                                        stLmmInfo.u.stForbRposTaiList.astTa);
        }

        if (VOS_TRUE == ulForbiddenRegLaFlag)
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN REG LA:IN stForbRposTaiList");

             ulPlmnLaType  |= NAS_MML_PLMN_FORBID_REG_LA;
        }

    }
    else
#endif
    {
        /* 是否在 ForbLA消息中 */
        ulRlst =  NAS_MML_GetLaiIndexInForbLaWithValidPeriodList(pstLai,
                    &(pstForbidPlmnInfo->stForbLaWithValidPeriod));

        if(NAS_MML_CUSTOMIZE_FORB_LA_MAX_NUM != ulRlst)
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN REG LA:IN stForbLaWithValidPeriod");

             ulPlmnLaType  |=   NAS_MML_PLMN_FORBID_ROAM_LA;
        }

        ulRlst =  NAS_MML_IsLaiInDestLaiList(pstLai,
                                            pstForbidPlmnInfo->ucForbRoamLaNum,
                                            pstForbidPlmnInfo->astForbRomLaList);

        if(VOS_TRUE == ulRlst)
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN ROAM LA:IN astForbRomLaList");

             ulPlmnLaType  |=   NAS_MML_PLMN_FORBID_ROAM_LA;
        }

        ulRlst =  NAS_MML_IsLaiInDestLaiList(pstLai,
                                            pstForbidPlmnInfo->ucForbRegLaNum,
                                            pstForbidPlmnInfo->astForbRegLaList);

        if (VOS_TRUE == ulRlst)
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiForbType FORBIDDEN REG LA:IN astForbRegLaList");

            ulPlmnLaType  |=   NAS_MML_PLMN_FORBID_REG_LA;
        }
    }

    return ulPlmnLaType;
}





VOS_UINT32 NAS_MML_IsCampPlmnInfoChanged(
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstOldCampInfo,
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstNewCampInfo
)
{
    VOS_UINT32                          i;

    /* 新老Plmn或 lac,rac,cellid或cell个数，或系统子模式是否有变化 */
    if ((pstOldCampInfo->stLai.stPlmnId.ulMcc  != pstNewCampInfo->stLai.stPlmnId.ulMcc)
     || (pstOldCampInfo->stLai.stPlmnId.ulMnc  != pstNewCampInfo->stLai.stPlmnId.ulMnc)
     || (pstOldCampInfo->ucRac                 != pstNewCampInfo->ucRac)
     || (pstOldCampInfo->enSysSubMode          != pstNewCampInfo->enSysSubMode)

     )
    {
        return VOS_TRUE;
    }

    for ( i = 0 ; i < NAS_MML_MAX_LAC_LEN ; i++ )
    {
        if (pstOldCampInfo->stLai.aucLac[i] != pstNewCampInfo->stLai.aucLac[i])
        {
            return VOS_TRUE;
        }
    }

    if (pstOldCampInfo->stCampCellInfo.ucCellNum != pstNewCampInfo->stCampCellInfo.ucCellNum)
    {
        return VOS_TRUE;
    }

    for ( i = 0 ; i < RRC_NAS_AT_CELL_MAX_NUM ; i++ )
    {
        if (pstOldCampInfo->stCampCellInfo.astCellInfo[i].ulCellId
                    != pstNewCampInfo->stCampCellInfo.astCellInfo[i].ulCellId)
        {
            return VOS_TRUE;
        }
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsCampPlmnInfoChanged, Camp PLMN Info not change");

    return VOS_FALSE;
}



NAS_MML_RRC_SYS_SUBMODE_ENUM_UINT8 NAS_MML_ConvertGprsSupportIndToSysSubMode(
    VOS_UINT8                           ucGprsSupportInd
)
{
    NAS_MML_RRC_SYS_SUBMODE_ENUM_UINT8  enSysSubMode;

    switch ( ucGprsSupportInd )
    {
        case NAS_MML_GAS_SUBMODE_GSM:
            enSysSubMode = NAS_MML_RRC_SYS_SUBMODE_GSM;
            break;

        case NAS_MML_GAS_SUBMODE_GPRS:
            enSysSubMode = NAS_MML_RRC_SYS_SUBMODE_GPRS;
            break;

        case NAS_MML_GAS_SUBMODE_EDGE:
            enSysSubMode = NAS_MML_RRC_SYS_SUBMODE_EDGE;
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MML_ConvertGasNetModeToMmlNetMode, enSysSubMode invalid");
            enSysSubMode = NAS_MML_RRC_SYS_SUBMODE_NULL;
            break;
    }

    return enSysSubMode;
}


VOS_VOID NAS_MML_SecContext3GTO2G(
    VOS_UINT8                           *pucIk,
    VOS_UINT8                           *pucCk,
    VOS_UINT8                           *pucKc
)
{
    /* 临时变量，用于 for 循环 */
    VOS_UINT8       i;

    /* 保存输入参数 IK 的临时变量 */
    VOS_UINT8       aucIk[NAS_MML_UMTS_IK_LEN];

    /* 保存输入参数 CK 的临时变量 */
    VOS_UINT8       aucCk[NAS_MML_UMTS_CK_LEN];

    /* 变量 Kc */
    VOS_UINT8       aucKc[NAS_MML_GSM_KC_LEN];

    PS_MEM_SET(aucIk, 0x00, sizeof(aucIk));
    PS_MEM_SET(aucCk, 0x00, sizeof(aucCk));
    PS_MEM_SET(aucKc, 0x00, sizeof(aucKc));

    /* 获得 IK */
    PS_MEM_CPY(aucIk, pucIk, NAS_MML_UMTS_IK_LEN);

    /* 获得 CK */
    PS_MEM_CPY(aucCk, pucCk, NAS_MML_UMTS_CK_LEN);

    /* 调用公式计算 aucKc */
    /*Kc[GSM] = CK1 xor CK2 xor IK1 xor IK2（其中CKi 和 IKi 的长度都为 64 bits ；
    CK = CK1 || CK2 ；IK = IK1 || IK2。*/
    for (i = 0; i < NAS_MML_GSM_KC_LEN; i ++)
    {
        aucKc[i] = aucCk[i] ^ aucCk[NAS_MML_GSM_KC_LEN + i];
    }
    for (i = 0; i < NAS_MML_GSM_KC_LEN; i ++)
    {
        aucKc[i] = aucKc[i] ^ aucIk[i];
    }
    for (i = 0; i < NAS_MML_GSM_KC_LEN; i ++)
    {
        aucKc[i] = aucKc[i] ^ aucIk[NAS_MML_GSM_KC_LEN + i];
    }

    /* 将 KC 写回到输出参数中 */
    PS_MEM_CPY(pucKc, aucKc, NAS_MML_GSM_KC_LEN);

    return;

}


VOS_VOID NAS_MML_SecContext2GTO3G(
    VOS_UINT8                           *pucIk,
    VOS_UINT8                           *pucCk,
    VOS_UINT8                           *pucKc
)
{
    /* 临时变量，用于 for 循环 */
    VOS_UINT8       i;

    /* 变量 IK */
    VOS_UINT8       aucIk[NAS_MML_UMTS_IK_LEN];

    /* 变量 CK */
    VOS_UINT8       aucCk[NAS_MML_UMTS_CK_LEN];

    /* 保存输入参数 KC 的临时变量 */
    VOS_UINT8       aucKc[NAS_MML_GSM_KC_LEN];

    PS_MEM_SET(aucIk, 0x00, sizeof(aucIk));
    PS_MEM_SET(aucCk, 0x00, sizeof(aucCk));
    PS_MEM_SET(aucKc, 0x00, sizeof(aucKc));

    /* 获得 KC */
    PS_MEM_CPY(aucKc, pucKc, NAS_MML_GSM_KC_LEN);

    /* 调用公式计算 IK、CK */
    /* 转换加密密钥 */
    /* ck=kc||kc(其中||为连接符号) */
    PS_MEM_CPY(aucCk, aucKc, NAS_MML_GSM_KC_LEN);
    PS_MEM_CPY((aucCk + NAS_MML_GSM_KC_LEN), aucKc, NAS_MML_GSM_KC_LEN);

    /* 转换一致性检查密钥 */
    /* ik=Kc1 xor Kc2 || Kc || Kc1 xor Kc2(其中||为连接符号、Kc = Kc1 || Kc2) */
    for (i = 0; i < (NAS_MML_GSM_KC_LEN/2); i++)
    {
        aucIk[i] = aucKc[i] ^ aucKc[(NAS_MML_GSM_KC_LEN/2) + i];
        aucIk[NAS_MML_GSM_KC_LEN + (NAS_MML_GSM_KC_LEN/2) + i] = aucIk[i];
    }
    PS_MEM_CPY((aucIk + (NAS_MML_GSM_KC_LEN/2)), aucKc, NAS_MML_GSM_KC_LEN);

    /* 将 IK 写回到输出参数中 */
    PS_MEM_CPY(pucIk, aucIk, NAS_MML_UMTS_IK_LEN);

    /* 将 CK 写回到输出参数中 */
    PS_MEM_CPY(pucCk, aucCk, NAS_MML_UMTS_CK_LEN);

    return;
}

#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID  NAS_MML_ConvertNasPlmnToLMMFormat(
    MMC_LMM_PLMN_ID_STRU               *pstLMMPlmn,
    NAS_MML_PLMN_ID_STRU               *pstGUNasPlmn
)
{
    pstLMMPlmn->aucPlmnId[0]
      = (VOS_UINT8)(0x0000000F & pstGUNasPlmn->ulMcc);
    pstLMMPlmn->aucPlmnId[0]
     |= (VOS_UINT8)((0x00000F00 & pstGUNasPlmn->ulMcc) >> 4);

    pstLMMPlmn->aucPlmnId[1]
     = (VOS_UINT8)((0x000F0000 & pstGUNasPlmn->ulMcc) >> 16);

    pstLMMPlmn->aucPlmnId[1]
     |= (VOS_UINT8)((0x000F0000 & pstGUNasPlmn->ulMnc) >> 12);

    pstLMMPlmn->aucPlmnId[2]
     = (VOS_UINT8)(0x0000000F & pstGUNasPlmn->ulMnc);
    pstLMMPlmn->aucPlmnId[2]
     |= (VOS_UINT8)((0x00000F00 & pstGUNasPlmn->ulMnc) >> 4);

    return ;

}
#endif


VOS_VOID NAS_MML_SortSpecRatPrioHighest(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enSpecRat,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList
)
{
    VOS_UINT32                          i;

    /* 排序方法是，在pstPrioRatList找到指定的接入技术后，将其放在pstPrioRatList最前面 */
    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        if (enSpecRat == pstPrioRatList->aucRatPrio[i])
        {
            (VOS_VOID)PS_MEM_MOVE(&(pstPrioRatList->aucRatPrio[1]),
                        &(pstPrioRatList->aucRatPrio[0]), i);

            pstPrioRatList->aucRatPrio[0] = enSpecRat;

            break;
        }
    }

    return;
}


VOS_VOID NAS_MML_SortSpecRatPrioLowest(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enSpecRat,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList
)
{
    VOS_UINT32                             i;
    VOS_UINT8                              ucRatNumIndex;

    if (pstPrioRatList->ucRatNum <= 1)
    {
        return;
    }

    ucRatNumIndex = pstPrioRatList->ucRatNum - 1;

    /* 排序方法是，在pstPrioRatList找到指定的接入技术后，将其放在pstPrioRatList最后面 */
    for (i = 0; i < ucRatNumIndex; i++)
    {
        if (enSpecRat == pstPrioRatList->aucRatPrio[i])
        {
            /*lint -e961*/
            (VOS_VOID)PS_MEM_MOVE(&(pstPrioRatList->aucRatPrio[i]),
                        &(pstPrioRatList->aucRatPrio[i+1]),
                        (pstPrioRatList->ucRatNum-i-1));
            /*lint +e961*/
            pstPrioRatList->aucRatPrio[pstPrioRatList->ucRatNum-1] = enSpecRat;

            break;
        }
    }

    return;
}



VOS_VOID NAS_MML_DelSpecRatInPrioRatList(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enSpecRat,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList
)
{
    VOS_UINT32                          i;

    if (pstPrioRatList->ucRatNum <= 1)
    {
        return;
    }

    /* 排序方法是，在pstPrioRatList找到指定的接入技术后，将后面的接入技术前移 */
    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        if (enSpecRat == pstPrioRatList->aucRatPrio[i])
        {
            /*lint -e961*/
            (VOS_VOID)PS_MEM_MOVE(&(pstPrioRatList->aucRatPrio[i]),
                        &(pstPrioRatList->aucRatPrio[i+1]),
                        (pstPrioRatList->ucRatNum-i-1));
            /*lint +e961*/

            pstPrioRatList->ucRatNum--;
            break;
        }
    }

    return;
}


VOS_VOID  NAS_MML_GetMsPsCurrRatCapabilityList(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstMsNotfityNwPsPrioRatList
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstMsPrioRatList    = VOS_NULL_PTR;
    VOS_UINT32                          ucIndex;
    VOS_UINT32                          ulIsLteCapabilityDisabled;
    VOS_UINT32                          ulDisableLteFlg;

    ucIndex                             = 0;
    pstMsPrioRatList                    = NAS_MML_GetMsPrioRatList();
    ulDisableLteFlg                     = NAS_MML_GetDisableLteRoamFlg();
    ulIsLteCapabilityDisabled           = NAS_MML_IsLteCapabilityDisabled(NAS_MML_GetLteCapabilityStatus(), ulDisableLteFlg);

    PS_MEM_SET(pstMsNotfityNwPsPrioRatList, 0X0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    if (pstMsPrioRatList->ucRatNum > NAS_MSCC_PIF_MAX_RAT_NUM)
    {
        pstMsPrioRatList->ucRatNum = NAS_MSCC_PIF_MAX_RAT_NUM;
    }

    for (ucIndex = 0; ucIndex < pstMsPrioRatList->ucRatNum; ucIndex++)
    {
        if ((NAS_MSCC_PIF_NET_RAT_TYPE_LTE  == pstMsPrioRatList->aucRatPrio[ucIndex])
         && (VOS_TRUE                       == ulIsLteCapabilityDisabled))
        {
            /* LTE 被disable了，剔除LTE */
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetMsPsCurrRatCapabilityList: lte disabled." );
        }
        else
        {
            pstMsNotfityNwPsPrioRatList->aucRatPrio[pstMsNotfityNwPsPrioRatList->ucRatNum] = pstMsPrioRatList->aucRatPrio[ucIndex];
            pstMsNotfityNwPsPrioRatList->ucRatNum ++;
        }
    }

    return;
}



VOS_VOID  NAS_MML_GetMsCsCurrRatCapabilityList(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstMsNotfityNwPsPrioRatList
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstMsPrioRatList    = VOS_NULL_PTR;
    VOS_UINT32                          ucIndex;


    ucIndex                             = 0;
    pstMsPrioRatList                    = NAS_MML_GetMsPrioRatList();


    NAS_MEM_SET_S(pstMsNotfityNwPsPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU), 0x00, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    if (pstMsPrioRatList->ucRatNum > NAS_MSCC_PIF_MAX_RAT_NUM)
    {
        pstMsPrioRatList->ucRatNum = NAS_MSCC_PIF_MAX_RAT_NUM;
    }

    for (ucIndex = 0; ucIndex < pstMsPrioRatList->ucRatNum; ucIndex++)
    {
        /* 从LWG到WG，不需要做LAU */
        pstMsNotfityNwPsPrioRatList->aucRatPrio[pstMsNotfityNwPsPrioRatList->ucRatNum] = pstMsPrioRatList->aucRatPrio[ucIndex];
        pstMsNotfityNwPsPrioRatList->ucRatNum ++;
    }

    return;
}



VOS_UINT32  NAS_MML_IsMsRatCapabilityChanged(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstNewPrioRatList,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstOldPrioRatList
)
{
    VOS_UINT32                          ucIdx;
    VOS_UINT32                          ulResult;

    ucIdx                               = 0;

    if (pstNewPrioRatList->ucRatNum > NAS_MSCC_PIF_MAX_RAT_NUM)
    {
        pstNewPrioRatList->ucRatNum = NAS_MSCC_PIF_MAX_RAT_NUM;
    }

    if (pstOldPrioRatList->ucRatNum > NAS_MSCC_PIF_MAX_RAT_NUM)
    {
        pstOldPrioRatList->ucRatNum = NAS_MSCC_PIF_MAX_RAT_NUM;
    }

    /* 如果当前通知网络的接入技术为0,则出发保护不发起LAU或者RAU*/
    if (0 == pstOldPrioRatList->ucRatNum)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsMsRatCapabilityChanged: pstMsPrioRatList->ucRatNum is zero,return false" );
        return VOS_FALSE;
    }

    if (pstOldPrioRatList->ucRatNum != pstNewPrioRatList->ucRatNum)
    {
        NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MML_IsMsRatCapabilityChanged: rat num is changed, ucOldRatNum is;ucNewRatNum is;",pstOldPrioRatList->ucRatNum,pstNewPrioRatList->ucRatNum);
        return VOS_TRUE;
    }

    for (ucIdx = 0; ucIdx < pstNewPrioRatList->ucRatNum; ucIdx++)
    {
        ulResult = NAS_MML_IsSpecRatInRatList(pstNewPrioRatList->aucRatPrio[ucIdx], pstOldPrioRatList);

        if (VOS_FALSE == ulResult)
        {
            NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MML_IsMsRatCapabilityChanged: rat= is not in oldratlist;",pstOldPrioRatList->ucRatNum,pstNewPrioRatList->ucRatNum);

            return VOS_TRUE;
        }
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsMsRatCapabilityChanged: rat cap not changed");

    return VOS_FALSE;
}



VOS_UINT32  NAS_MML_IsMsPsRatCapabilityListChanged(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstNewPrioRatList,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstOldPrioRatList
)
{
    VOS_UINT32                          ulIsRatCapChangeFlg;

    /* 如果NV RatBalancing 没开，则保持原逻辑: 只判别LTE 是否有变化 */
    if (VOS_FALSE == NAS_MML_GetRatBalancingEnableFlg())
    {
        /* 原逻辑不受RatBalancing控制: LTE有变化就返回true */
        if (NAS_MML_IsSupportLteCapability() != NAS_MML_IsSpecRatInRatList(NAS_MML_NET_RAT_TYPE_LTE, pstOldPrioRatList))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsMsPsRatCapabilityListChanged: RatBalancingEnableFlg is false and ps rat cap changed");
            return VOS_TRUE;
        }

        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsMsPsRatCapabilityListChanged: RatBalancingEnableFlg is false and ps rat cap not changed");

        return VOS_FALSE;
    }

    if (0 == pstOldPrioRatList->ucRatNum)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsMsPsRatCapabilityListChanged: pstOldPrioRatList->ucRatNum is 0 and return false");
        return VOS_FALSE;
    }


    ulIsRatCapChangeFlg = NAS_MML_IsMsRatCapabilityChanged(pstNewPrioRatList, pstOldPrioRatList);

    return ulIsRatCapChangeFlg;
}




VOS_UINT32 NAS_MML_IsPlmnIdForbiddenByWhiteBlackList (
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_LOCK_CFG_INFO_STRU    *pstPlmnLockCfg = VOS_NULL_PTR;
    /* 用于获取黑白名单 */
    pstPlmnLockCfg = NAS_MML_GetPlmnLockCfg();

    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                                   pstPlmnLockCfg->ucBlackPlmnLockNum,
                                                   pstPlmnLockCfg->astBlackPlmnId))     /* 该PLMN在黑名单列表中 */
    {
       return VOS_TRUE;
    }

    if (pstPlmnLockCfg->ucWhitePlmnLockNum != 0)
    {

        if (VOS_FALSE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                                        pstPlmnLockCfg->ucWhitePlmnLockNum,
                                                        pstPlmnLockCfg->astWhitePlmnId))     /* 该PLMN不在白名单列表中 */
        {
            return VOS_TRUE;
        }

    }

    return VOS_FALSE;
}

#if (FEATURE_ON == FEATURE_LTE)


VOS_VOID NAS_MML_UpdateEpsConnectionStatus(MMC_LMM_CONN_STATE_ENUM_UINT32 enEpsConnState)
{
    /* 更新LMM的链接状态 */
    switch (enEpsConnState)
    {
        case MMC_LMM_CONN_ESTING:
        case MMC_LMM_CONNECTED_SIG:
            NAS_MML_SetEpsSigConnStatusFlg(VOS_TRUE);
            NAS_MML_SetEpsServiceConnStatusFlg(VOS_FALSE);
            break;

        case MMC_LMM_CONNECTED_DATA:
            NAS_MML_SetEpsServiceConnStatusFlg(VOS_TRUE);
            break;

        case MMC_LMM_CONN_IDLE:
            NAS_MML_SetEpsSigConnStatusFlg(VOS_FALSE);
            NAS_MML_SetEpsServiceConnStatusFlg(VOS_FALSE);
            break;

        default:
            break;
    }

}




VOS_UINT32 NAS_MML_IsCsfbServiceStatusExist(VOS_VOID)
{
    NAS_MML_CSFB_SERVICE_STATUS_ENUM_UINT8                  enCsfbServiceStatus;
    VOS_UINT32                                              ulCsfbExistFlag;

    /* 默认CSFB不存在 */
    ulCsfbExistFlag     = VOS_FALSE;
    enCsfbServiceStatus = NAS_MML_GetCsfbServiceStatus();

    switch ( enCsfbServiceStatus )
    {
        /* CSFB标志不存在 */
        case NAS_MML_CSFB_SERVICE_STATUS_NOT_EXIST :
            ulCsfbExistFlag = VOS_FALSE;
            break;

        /* 存在MO非紧急呼叫的CC的CSFB标志 */
        case NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_CC_EXIST :
            ulCsfbExistFlag = VOS_TRUE;
            break;

        /* 存在MO的SS的CSFB标志 */
        case NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_SS_EXIST :
            ulCsfbExistFlag = VOS_TRUE;
            break;

        /* 存在MO的LCS的CSFB标志 */
        case NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_LCS_EXIST :
            ulCsfbExistFlag = VOS_TRUE;
            break;

        /* 存在MO急呼叫的CSFB标志 */
        case NAS_MML_CSFB_SERVICE_STATUS_MO_EMERGENCY_EXIST :
            ulCsfbExistFlag = VOS_TRUE;
            break;

        /* 存在MT呼叫的CSFB标志 */
        case NAS_MML_CSFB_SERVICE_STATUS_MT_EXIST :
            ulCsfbExistFlag = VOS_TRUE;
            break;

        /* CSFB标志不存在 */
        default:
            ulCsfbExistFlag = VOS_FALSE;
            break;
    }

    return ulCsfbExistFlag;
}



NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8 NAS_MML_GetLteUeOperationMode( VOS_VOID)
{
    NAS_MML_LTE_UE_USAGE_SETTING_ENUM_UINT8                 enLteUeUsageSetting;
    NAS_MML_MS_MODE_ENUM_UINT8                              enMsMode;

    enMsMode            = NAS_MML_GetMsMode();
    enLteUeUsageSetting = NAS_MML_GetLteUeUsageSetting();

    /* PS ONLY且为数据中心，则为PS_2 */
    if ( (NAS_MML_MS_MODE_PS_ONLY                    == enMsMode)
      && (NAS_MML_LTE_UE_USAGE_SETTING_DATA_CENTRIC  == enLteUeUsageSetting) )
    {
        return NAS_MML_LTE_UE_OPERATION_MODE_PS_2;
    }

    /* CS_PS 且为数据中心，则为CS_PS_2 */
    if ( (NAS_MML_MS_MODE_PS_CS                      == enMsMode)
      && (NAS_MML_LTE_UE_USAGE_SETTING_DATA_CENTRIC  == enLteUeUsageSetting) )
    {
        return NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_2;
    }

    /* PS ONLY且为语音中心，则为PS_1 */
    if ( (NAS_MML_MS_MODE_PS_ONLY                    == enMsMode)
      && (NAS_MML_LTE_UE_USAGE_SETTING_VOICE_CENTRIC == enLteUeUsageSetting) )
    {
        return NAS_MML_LTE_UE_OPERATION_MODE_PS_1;
    }

    /* CS_PS 且为语音中心，则为CS_PS_1 */
    if ( (NAS_MML_MS_MODE_PS_CS                      == enMsMode)
      && (NAS_MML_LTE_UE_USAGE_SETTING_VOICE_CENTRIC == enLteUeUsageSetting) )
    {
        return NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_1;
    }

    /* 异常处理,PS ONLY，则为PS_2 */
    if ( NAS_MML_MS_MODE_PS_ONLY                    == enMsMode )
    {
        return NAS_MML_LTE_UE_OPERATION_MODE_PS_2;
    }

    /* CS_PS，则为CS_PS_2 */
    return NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_2;
}



VOS_UINT32 NAS_MML_IsCsfbMoServiceStatusExist(VOS_VOID)
{
    NAS_MML_CSFB_SERVICE_STATUS_ENUM_UINT8                  enCsfbServiceStatus;

    enCsfbServiceStatus = NAS_MML_GetCsfbServiceStatus();

    if ( (NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_CC_EXIST == enCsfbServiceStatus)
      || (NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_SS_EXIST == enCsfbServiceStatus)
      || (NAS_MML_CSFB_SERVICE_STATUS_MO_EMERGENCY_EXIST == enCsfbServiceStatus) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


#endif

VOS_UINT32 NAS_MML_IsEquPlmnInfoChanged(
    NAS_MML_EQUPLMN_INFO_STRU          *pstSrcEquPlmnInfo
)
{
    VOS_UINT32                          i;
    NAS_MML_EQUPLMN_INFO_STRU          *pstDstEquPlmnInfo = VOS_NULL_PTR;

    pstDstEquPlmnInfo   = NAS_MML_GetEquPlmnList();

    /* 等效PLMN的个数相等，且等效PLMN完全相同，则Eplmn信息没有改变 */
    if ( pstSrcEquPlmnInfo->ucEquPlmnNum == pstDstEquPlmnInfo->ucEquPlmnNum )
    {
        for ( i = 0 ; i < pstSrcEquPlmnInfo->ucEquPlmnNum; i++ )
        {
            if ((pstSrcEquPlmnInfo->astEquPlmnAddr[i].ulMcc != pstDstEquPlmnInfo->astEquPlmnAddr[i].ulMcc)
             || (pstSrcEquPlmnInfo->astEquPlmnAddr[i].ulMnc != pstDstEquPlmnInfo->astEquPlmnAddr[i].ulMnc))
            {
                /* 等效PLMN改变，返回 VOS_TRUE */
                return VOS_TRUE;
            }

        }

        /* 等效PLMN没有改变，返回 VOS_FALSE */
        return VOS_FALSE;
    }

    /* 等效PLMN的个数不等，等效PLMN肯定发生了改变，返回 VOS_TRUE */
    return VOS_TRUE;
}


VOS_VOID NAS_MML_GetMsNetworkCapability(
    NAS_MML_MS_NETWORK_CAPACILITY_STRU  *pstMsNetworkCapbility
)
{
    VOS_INT8                            cVersion;
    NAS_MML_MS_CAPACILITY_INFO_STRU    *pstMsCapability = VOS_NULL_PTR;


#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_MS_MODE_ENUM_UINT8          enMsMode;
    VOS_UINT8                           ucIsrSupport;

    enMsMode        = NAS_MML_GetMsMode();
    ucIsrSupport    = NAS_MML_GetIsrSupportFlg();
#endif

    pstMsCapability = NAS_MML_GetMsCapability();
    cVersion        = NAS_Common_Get_Supported_3GPP_Version(MM_COM_SRVDOMAIN_PS);

    pstMsNetworkCapbility->ucNetworkCapabilityLen = NAS_MML_DEFAULT_NETWORKCAPABILITY_LEN;
    PS_MEM_CPY(pstMsNetworkCapbility->aucNetworkCapability, pstMsCapability->stMsNetworkCapability.aucNetworkCapability,
                pstMsNetworkCapbility->ucNetworkCapabilityLen);

    if(PS_PTL_VER_PRE_R99 == cVersion)
    {
        pstMsNetworkCapbility->aucNetworkCapability[0] &= 0xfe;
    }
    else
    {
        pstMsNetworkCapbility->aucNetworkCapability[0] |= 0x01;
    }

    if (PS_PTL_VER_R7 <= cVersion)
    {
        /* 目前NV项中Network cap填写长度为2,如果协议版本为R7或R7之后,其长度为3 */
        pstMsNetworkCapbility->ucNetworkCapabilityLen++;

        if (pstMsCapability->stMsNetworkCapability.ucNetworkCapabilityLen > 2)
        {
            pstMsNetworkCapbility->aucNetworkCapability[2] = pstMsCapability->stMsNetworkCapability.aucNetworkCapability[2];
        }
        else
        {
            pstMsNetworkCapbility->aucNetworkCapability[2] = 0x0;
        }

#if (FEATURE_ON == FEATURE_LTE)

        /* 如果支持LTE且模式为CS_PS时认为支持EMM Combined procedures capability */
        if ((VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
         && (NAS_MML_MS_MODE_PS_CS == enMsMode))
        {
            pstMsNetworkCapbility->aucNetworkCapability[2] |= 0x20;
        }
        else
#endif
        {
            pstMsNetworkCapbility->aucNetworkCapability[2] &= 0xDF;
        }
        /* 如果支持LTE,则认为支持ISR, ISR support对应bit填1 */

#if (FEATURE_ON == FEATURE_LTE)
        if ( (VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
          && (VOS_TRUE == ucIsrSupport))
        {
            pstMsNetworkCapbility->aucNetworkCapability[2] |= 0x10;
        }
        else
#endif
        {
            pstMsNetworkCapbility->aucNetworkCapability[2] &= 0xEF;
        }

    }
}


VOS_VOID NAS_MML_SoftReBoot_WithLineNoAndFileID(
    VOS_UINT32                          ulLineNO,
    VOS_UINT32                          ulFileID,
    NAS_MML_REBOOT_SCENE_ENUM_UINT8     ucRebootScean
)
{
    NAS_MML_LOG_EVENT_STATE_STRU       *pstLogEventState    = VOS_NULL_PTR;
    VOS_UINT32                          ulTaskTcb;
    VOS_UINT32                          ulPid;
    VOS_UINT32                          enFsmId;
    VOS_UINT32                          enFsmTopState;

    pstLogEventState = &(NAS_MML_GetMmlCtx()->stMaintainInfo.stLogEventState);


#if   (FEATURE_ON == FEATURE_LTE)
    if ( NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        ulPid = PS_PID_MM;
    }
    else
#endif
    {
        /* GAS 和 WRR的FID相同 */
        ulPid = WUEPS_PID_WRR;
    }

    ulTaskTcb = VOS_GetTCBFromPid(ulPid);

    enFsmTopState = NAS_MMC_GetFsmTopState();
    enFsmId       = NAS_MMC_GetCurrFsmId();
    /* 复位时保存接入层的TCB信息 */
    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_SoftReBoot_WithLineNoAndFileID(): Trigger modem reset");
    mdrv_om_system_error( NAS_REBOOT_MOD_ID_MML | (VOS_INT)ucRebootScean
                          | (VOS_INT)(enFsmTopState << 8) | (VOS_INT)(enFsmId << 16),
                      (VOS_INT)ulTaskTcb,
                      (VOS_INT)((ulFileID << 16) | ulLineNO),
                      (VOS_CHAR *)(pstLogEventState),
                      sizeof(NAS_MML_LOG_EVENT_STATE_STRU) );
    return;

}


VOS_UINT8 NAS_MML_IsRaiChanged(VOS_VOID)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCampInfo;
    NAS_MML_RAI_STRU                   *pstLastSuccRai;
    VOS_UINT32                           i;

    pstCampInfo    = NAS_MML_GetCurrCampPlmnInfo();
    pstLastSuccRai = NAS_MML_GetPsLastSuccRai();

    if ((pstCampInfo->stLai.stPlmnId.ulMcc != pstLastSuccRai->stLai.stPlmnId.ulMcc)
     || (pstCampInfo->stLai.stPlmnId.ulMnc != pstLastSuccRai->stLai.stPlmnId.ulMnc)
     || (pstCampInfo->ucRac != pstLastSuccRai->ucRac))
    {
        return VOS_TRUE;
    }

    for ( i = 0 ; i < NAS_MML_MAX_LAC_LEN ; i++ )
    {
        if (pstCampInfo->stLai.aucLac[i] != pstLastSuccRai->stLai.aucLac[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;

}


VOS_UINT8 NAS_MML_CompareLai(
    NAS_MML_LAI_STRU                    *pstCurLai,
    NAS_MML_LAI_STRU                    *pstOldLai
)
{
    VOS_UINT32                         i;

    if (VOS_FALSE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstCurLai->stPlmnId), &(pstOldLai->stPlmnId)))
    {
        return VOS_FALSE;
    }

    for ( i = 0 ; i < NAS_MML_MAX_LAC_LEN ; i++ )
    {
        if (pstCurLai->aucLac[i] != pstOldLai->aucLac[i])
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;

}


VOS_UINT32 NAS_MML_IsNetworkRegFailCause(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 usRegFailCause
)
{
    /* 原因值小于280时，则是跟网侧交互的注册原因值 */
    if ( usRegFailCause < NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT )
    {
        return VOS_TRUE;
    }

    /* 原因值大于等于280时，则是用户自定义的注册原因值 */
    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsRegFailCauseNotifyLmm(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  usRegFailCause
)
{
    /* 原因值小于300时，需要将注册结果通知给LMM */
    if ( usRegFailCause < NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE )
    {
        return VOS_TRUE;
    }

    /* 原因值大于等于300时，*/
    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_UnCompressData(
    VOS_UINT8                          *pucDest,
    VOS_UINT32                         *pulDestLen,
    VOS_UINT8                          *pucSrc,
    VOS_UINT32                         *pulSrcLen
)
{
    VOS_UINT32                          ulRslt;

    /* 格式为,cmpsSize(32BITs) + data(length=cmpsSize) */

    /* 前四个字节为cmpsSize,获取cmpsSize */
    PS_MEM_CPY(pulSrcLen, pucSrc, sizeof(VOS_UINT32));
    pucSrc += sizeof(VOS_UINT32);

    /* 从第五个字节开始存放压缩后的数据 */
    ulRslt  = (VOS_UINT32)_uncompress(pucDest, (unsigned long *)pulDestLen,
                                      pucSrc, *pulSrcLen);
    if (VOS_OK != ulRslt)
    {
        NAS_WARNING_LOG1(WUEPS_PID_MMC,
                         "NAS_MML_UnCompressData: ulRslt",
                         ulRslt);

        return VOS_FALSE;
    }

    /* 补充4个cmpsSize空间 */
    *pulSrcLen += sizeof(VOS_UINT32);

    return VOS_TRUE;
}


VOS_UINT32  NAS_MML_CompressData(
    VOS_UINT8                          *pucDest,
    VOS_UINT32                         *pulDestLen,
    VOS_UINT8                          *pucSrc,
    VOS_UINT32                          ulSrcLen
)
{
    VOS_UINT32                          ulRslt;

    /*格式为,cmpsSize(32BITs) + data(length=cmpsSize)*/
    if ((*pulDestLen) < sizeof(VOS_UINT32))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC,
                         "NAS_MML_CompressData: 剩余空间不足");
        return VOS_FALSE;
    }

    /*预留前四个字节，从第五个字节开始存放压缩后的数据*/
    (*pulDestLen) -= sizeof(VOS_UINT32);

    ulRslt         = (VOS_UINT32)compress(pucDest + sizeof(VOS_UINT32), (unsigned long *)pulDestLen,
                                   pucSrc, ulSrcLen);

    if( VOS_OK != ulRslt )
    {
        NAS_WARNING_LOG1(WUEPS_PID_MMC,
                         "NAS_MML_CompressData: compress", ulRslt);

        return VOS_FALSE;
    }

    /*在预留的前四个字节上，填充cmpsSize*/
    PS_MEM_CPY( pucDest, pulDestLen, sizeof(VOS_UINT32));

    /*补充4个字节长度*/
    (*pulDestLen) += sizeof(VOS_UINT32);

    return VOS_TRUE;
}




VOS_UINT32 NAS_MML_IsSndOmPcRecurMsgValid(VOS_VOID)
{
    /* 当前UE与PC工具未连接, 无需发送回放消息 */
    if ( VOS_FALSE == NAS_MML_GetOmConnectFlg() )
    {
        return VOS_FALSE;
    }

#ifndef WIN32
    /* 当前PC工具未使能NAS回放消息的发送, 无需发送回放消息 */
    if ( VOS_FALSE == NAS_MML_GetOmPcRecurEnableFlg() )
    {
        return VOS_FALSE;
    }
#endif

    return VOS_TRUE;
}



VOS_VOID NAS_MML_DelEqualPlmnsInForbList(
    VOS_UINT8                          *pEPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pEPlmnList,
    VOS_UINT8                           ucRefPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pRefPlmnList
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;
    VOS_UINT32                          ulRlst;
    NAS_MML_PLMN_ID_STRU               *pstCurrPlmnId;                          /* PlMN标识 */
    NAS_MML_PLMN_ID_STRU               *pstEPlmn;

    pstCurrPlmnId = NAS_MML_GetCurrCampPlmnId();

    ucPlmnNum = *pEPlmnNum;

    for ( i = 0; i < ucPlmnNum;  )
    {
        ulRlst = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pEPlmnList + i, ucRefPlmnNum, pRefPlmnList);

        pstEPlmn = pEPlmnList + i;

        /* 将在禁止列表且不是当前驻留网络删除 */
        if ((VOS_FALSE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstCurrPlmnId, pstEPlmn))
         && ( VOS_TRUE == ulRlst ))
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pEPlmnList [i], &pEPlmnList[ i + 1 ],
                            ((ucPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_ID_STRU));
            }

            ucPlmnNum--;

            if ( i > 0 )
            {
                i--;
            }
            else
            {
                continue;
            }

        }

        i++;
    }

    *pEPlmnNum = ucPlmnNum;

    return;
}



VOS_UINT32 NAS_MML_IsPsBearerExist(VOS_VOID)
{
    NAS_MML_PS_BEARER_CONTEXT_STRU     *pstPsBearerCtx;
    VOS_UINT32                          i;

    pstPsBearerCtx = NAS_MML_GetPsBearerCtx();

    for (i = 0; i < NAS_MML_MAX_PS_BEARER_NUM; i++)
    {
        if (NAS_MML_PS_BEARER_STATE_ACTIVE == pstPsBearerCtx[i].enPsBearerState)
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_VOID NAS_MML_UpdateAllPsBearIsrFlg(
    NAS_MML_PS_BEARER_ISR_ENUM_UINT8    enPsBearerIsrFlg
)
{
    NAS_MML_PS_BEARER_CONTEXT_STRU     *pstPsBearerCtx;
    VOS_UINT8                           i;

    pstPsBearerCtx = NAS_MML_GetPsBearerCtx();

    for (i = 0; i < NAS_MML_MAX_PS_BEARER_NUM; i++)
    {
        pstPsBearerCtx[i].enPsBearerIsrFlg = enPsBearerIsrFlg;
    }

    return;
}


VOS_UINT32 NAS_MML_IsPsBearerExistBeforeIsrAct(VOS_UINT8 ucNsapi)
{
    NAS_MML_PS_BEARER_CONTEXT_STRU     *pstPsBearerCtx;

    pstPsBearerCtx = NAS_MML_GetPsBearerCtx();

    /* 合法性检查，如果ucRabId取值不在协议定义[5,15]范围，则不处理直接返回 */
    if ((ucNsapi < NAS_MML_MIN_NSAPI)
     || (ucNsapi > NAS_MML_MAX_NSAPI))
    {
        return VOS_FALSE;
    }

    if ((NAS_MML_PS_BEARER_STATE_ACTIVE == pstPsBearerCtx[ucNsapi - 5].enPsBearerState)
     && (NAS_MML_PS_BEARER_EXIST_BEFORE_ISR_ACT == pstPsBearerCtx[ucNsapi - 5].enPsBearerIsrFlg))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;

}


VOS_UINT32 NAS_MML_IsPsBearerAfterIsrActExist(VOS_VOID)
{
    NAS_MML_PS_BEARER_CONTEXT_STRU     *pstPsBearerCtx;
    VOS_UINT8                           i;

    pstPsBearerCtx = NAS_MML_GetPsBearerCtx();

    for (i = 0; i < NAS_MML_MAX_PS_BEARER_NUM; i++)
    {
        if ((NAS_MML_PS_BEARER_STATE_ACTIVE == pstPsBearerCtx[i].enPsBearerState)
         && (NAS_MML_PS_BEARER_EXIST_AFTER_ISR_ACT == pstPsBearerCtx[i].enPsBearerIsrFlg))
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;

}

#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MML_ConvertMmlCsfbStatusToLmmCsfbStatus(
    NAS_MML_CSFB_SERVICE_STATUS_ENUM_UINT8                  enMmlCsfbStatus,
    MM_LMM_CSFB_SERVICE_TYPE_ENUM_UINT32                   *penLmmCsfbStatus
)
{
    switch ( enMmlCsfbStatus )
    {
        case NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_CC_EXIST :
        case NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_SS_EXIST :
        case NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_LCS_EXIST :
            *penLmmCsfbStatus = MM_LMM_CSFB_SERVICE_MO_NORMAL;
            break;

        case NAS_MML_CSFB_SERVICE_STATUS_MO_EMERGENCY_EXIST :
            *penLmmCsfbStatus = MM_LMM_CSFB_SERVICE_MO_EMERGENCY;
            break;

        case NAS_MML_CSFB_SERVICE_STATUS_MT_EXIST :
            *penLmmCsfbStatus = MM_LMM_CSFB_SERVICE_MT_NORMAL;
            break;

        default:
            *penLmmCsfbStatus = MM_LMM_CSFB_SERVICE_BUTT;
            break;
    }

    return;
}

VOS_UINT32 NAS_MML_GetCsPsMode1EnableLteTimerLen(VOS_VOID)
{
    /* 如果当前驻留plmn在支持DAM特性网络列表中，则返回T3402定时器时长，
       否则返回9220 en_NV_Item_Enable_Lte_Timer_Len NV项中配置的时长 */
    if (VOS_TRUE == NAS_MML_IsPlmnSupportDam(NAS_MML_GetCurrCampPlmnId()))
    {
        return NAS_MML_GetT3402Len();
    }

    /* 如果 en_NV_Item_DISABLE_LTE_START_T3402_ENABLE_LTE_CFG 开，则从ID_LMM_MMC_T3402_LEN_NOTIFY消息中的 T3402定时器时长，
       否则返回9220 en_NV_Item_Enable_Lte_Timer_Len NV项中配置的时长 */
    if (VOS_TRUE == NAS_MML_GetDisableLteStartT3402EnableLteFlag())
    {
        return NAS_MML_GetT3402Len();
    }


    return NAS_MML_GetCsPsMode13GPPEnableLteTimerLen();
}

#endif

VOS_UINT32 NAS_MML_IsLteCapabilityDisabled(
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapStatus,
    VOS_UINT32                                              ulDisableLteRoamFlg
)
{
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapStatus)
     || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapStatus)
     || (VOS_TRUE == ulDisableLteRoamFlg))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;

}


VOS_UINT32 NAS_MML_IsNeedAddDamPlmnInDisabledPlmnWithForbiddenPeriodList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat
)
{
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledPlmnIdWithRatList = VOS_NULL_PTR;
    VOS_UINT32                                              ulIndex;
    VOS_UINT32                                              ulIsPlmnSupportDam;

    pstDisabledPlmnIdWithRatList = NAS_MML_GetDisabledRatPlmnInfo();

    /* 如果pstPlmnWithRat已经在禁止接入技术网络列表中则无需处理 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList(pstPlmnWithRat))
    {
        return VOS_FALSE;
    }

    ulIndex = pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum;

    /* 列表已满返回VOS_FALSE */
    if (NAS_MML_MAX_DISABLED_RAT_PLMN_NUM == ulIndex)
    {
        return VOS_FALSE;
    }

    ulIsPlmnSupportDam = NAS_MML_IsPlmnSupportDam(&pstPlmnWithRat->stPlmnId);

    /* DAM特性生效且在DAM网络disable lte后需要将DAM网络加入禁止接入
       技术网络列表中，则动态加入禁止接入技术网络列表 */
    if ((VOS_TRUE == NAS_MML_GetAddDamPlmnInDisablePlmnWithRatListFlag())
     && (VOS_TRUE == ulIsPlmnSupportDam))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_GetDamDisabledPlmnWithForbiddenPeriodTimeSlice(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstPlmnWithRat
)
{
    VOS_UINT32                          ulExpiredTimeSlice;
    VOS_UINT32                          ulIsPlmnSupportDam;

    ulExpiredTimeSlice = 0;

    ulIsPlmnSupportDam = NAS_MML_IsPlmnSupportDam(&pstPlmnWithRat->stPlmnId);

    /* DAM特性生效且在DAM网络disable lte后需要将DAM网络加入禁止接入
       技术网络列表中，根据T3402定时器时长返回对应的timeslice */
    if ((VOS_TRUE == NAS_MML_GetAddDamPlmnInDisablePlmnWithRatListFlag())
     && (VOS_TRUE == ulIsPlmnSupportDam))
    {
        ulExpiredTimeSlice = VOS_GetSliceUnit() * (NAS_MML_GetT3402Len()/1000);
    }

    return ulExpiredTimeSlice;
}


VOS_UINT32 NAS_MML_IsNeedDeleteDisabledPlmnWithValidPeriodInPlmnList (
    NAS_MML_DISABLED_PLMN_WITH_FORBIDDEN_PERIOD_STRU       *pstDisabledPlmnIdWithRat
)
{
    VOS_UINT32                          ulCurrTimeSlice;
    VOS_UINT32                          ulDuration;

    ulCurrTimeSlice = VOS_GetSlice();

    /* 如果禁止接入技术网络是nv配置的禁止期限为全F，则返回VOS_FALSE无需动态删除 */
    if (NAS_MML_MAX_TIME_SLICE == pstDisabledPlmnIdWithRat->ulPlmnDisabledForbiddenSlice)
    {
        return VOS_FALSE;
    }

    /* 达到禁止时间，则从禁止接入技术网络列表中删除该网络,因为存在翻转所以如果
       ulCurrTimeSlice小于ulPlmnDisabledBeginSlice也认为达到禁止时间，从禁止接入技术网络列表中删除该网络*/
    if (ulCurrTimeSlice < pstDisabledPlmnIdWithRat->ulPlmnDisabledBeginSlice)
    {
        ulDuration = NAS_MML_MAX_TIME_SLICE - pstDisabledPlmnIdWithRat->ulPlmnDisabledBeginSlice + ulCurrTimeSlice + 1;
    }
    else
    {
        ulDuration = ulCurrTimeSlice - pstDisabledPlmnIdWithRat->ulPlmnDisabledBeginSlice;
    }

    if (ulDuration >= pstDisabledPlmnIdWithRat->ulPlmnDisabledForbiddenSlice)
    {
        return VOS_TRUE;
    }


    return VOS_FALSE;
}





VOS_UINT32 NAS_MML_GetFirstAddPlmnIndexInDestDisabledPlmnWithForbiddenPeriodList (
    VOS_UINT32                                              ulDestPlmnNum,
    NAS_MML_DISABLED_PLMN_WITH_FORBIDDEN_PERIOD_STRU       *pstDestPlmnIdList
)
{
    VOS_UINT32                          ulFirstAddPlmnIdIndex;
    VOS_UINT32                          j;
    VOS_UINT32                          ulIndex;

    ulIndex               = ulDestPlmnNum;
    ulFirstAddPlmnIdIndex = NAS_MML_MAX_DISABLED_RAT_PLMN_NUM;

    if (ulIndex > NAS_MML_MAX_DISABLED_RAT_PLMN_NUM)
    {
        ulIndex = NAS_MML_MAX_DISABLED_RAT_PLMN_NUM;
    }

    /* 找第一个惩罚时间不为NAS_MML_MAX_TIME_SLICE的网络 */
    for (j = 0; j < ulIndex; j++)
    {
        if (pstDestPlmnIdList[j].ulPlmnDisabledForbiddenSlice != NAS_MML_MAX_TIME_SLICE)
        {
            ulFirstAddPlmnIdIndex = j;
            break;
        }
    }

    /* 如果都是惩罚时间为NAS_MML_MAX_TIME_SLICE的网络，则返回无效值 */
    if (NAS_MML_MAX_DISABLED_RAT_PLMN_NUM == ulFirstAddPlmnIdIndex)
    {
        return ulFirstAddPlmnIdIndex;
    }

    /* 找最早加入且惩罚时间不是ulFirstAddPlmnIdIndex的网络index */
    for (j = ulFirstAddPlmnIdIndex + 1; j < ulIndex; ++j)
    {
        if ((pstDestPlmnIdList[j].ulPlmnDisabledBeginSlice < pstDestPlmnIdList[ulFirstAddPlmnIdIndex].ulPlmnDisabledBeginSlice)
         && (pstDestPlmnIdList[j].ulPlmnDisabledForbiddenSlice != NAS_MML_MAX_TIME_SLICE))
        {
            ulFirstAddPlmnIdIndex = j;
        }
    }

    return ulFirstAddPlmnIdIndex;
}



VOS_VOID NAS_MML_AddDisabledRatPlmnWithForbiddenPeriodList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat,
    VOS_UINT32                                              ulPlmnDisabledInvalidSlice,
    MMC_LMM_DISABLE_LTE_REASON_ENUM_UINT32                  enDisableLteReason
)
{
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledPlmnIdWithRatList = VOS_NULL_PTR;
    VOS_UINT32                                              ulIndex;

    pstDisabledPlmnIdWithRatList = NAS_MML_GetDisabledRatPlmnInfo();

    /* 如果pstPlmnWithRat已经在禁止接入技术网络列表中则无需添加，更新下begin slice、惩罚时长和禁止原因 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnWithForbiddenPeriodList(pstPlmnWithRat))
    {
        ulIndex = NAS_MML_GetPlmnWithRatIndexInDestDisabledPlmnWithForbiddenPeriodList(pstPlmnWithRat,
                                                   pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum,
                                                   pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId);

        if (ulIndex < NAS_MML_MAX_DISABLED_RAT_PLMN_NUM)
        {
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].ulPlmnDisabledForbiddenSlice = ulPlmnDisabledInvalidSlice;

            /* 如果是因为4G未开户禁止该PLMN，并且该PLMN已经在禁止列表中，不更新BeginSlice */
            if (MMC_LMM_DISABLE_LTE_REASON_LMM_NOTIFY_EUTRAN_NOT_ALLOW != enDisableLteReason)
            {
                pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].ulPlmnDisabledBeginSlice     = VOS_GetSlice();
            }

            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].enDisableLteReason           = enDisableLteReason;
        }
        return;
    }

    /* 列表已满,删除最早加入禁止接入技术网络列表中的网络 */
    if (NAS_MML_MAX_DISABLED_RAT_PLMN_NUM == pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum)
    {
        ulIndex = NAS_MML_GetFirstAddPlmnIndexInDestDisabledPlmnWithForbiddenPeriodList(pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum,
                                pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId);

        if (ulIndex < NAS_MML_MAX_DISABLED_RAT_PLMN_NUM)
        {
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].stPlmnWithRat.stPlmnId.ulMcc = pstPlmnWithRat->stPlmnId.ulMcc;
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].stPlmnWithRat.stPlmnId.ulMnc = pstPlmnWithRat->stPlmnId.ulMnc;
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].stPlmnWithRat.enRat          = pstPlmnWithRat->enRat;
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].ulPlmnDisabledForbiddenSlice = ulPlmnDisabledInvalidSlice;
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].ulPlmnDisabledBeginSlice     = VOS_GetSlice();
            pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].enDisableLteReason           = enDisableLteReason;

            NAS_MMC_LogForbiddenPlmnRelatedInfo();
        }
        return;
    }

    /* 当前PLMN不在惩罚列表里面，且当前列表未满，则需要将PLMN加入到禁止接入技术列表里面 */
    ulIndex                      = pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum;

    pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].stPlmnWithRat.stPlmnId.ulMcc = pstPlmnWithRat->stPlmnId.ulMcc;
    pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].stPlmnWithRat.stPlmnId.ulMnc = pstPlmnWithRat->stPlmnId.ulMnc;
    pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].stPlmnWithRat.enRat          = pstPlmnWithRat->enRat;
    pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].ulPlmnDisabledForbiddenSlice = ulPlmnDisabledInvalidSlice;
    pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].ulPlmnDisabledBeginSlice     = VOS_GetSlice();
    pstDisabledPlmnIdWithRatList->astDisabledRatPlmnId[ulIndex].enDisableLteReason           = enDisableLteReason;
    pstDisabledPlmnIdWithRatList->ulDisabledRatPlmnNum++;

    /* 当禁止时长不是最大值时，则启动周期定时器判断加入禁止接入技术网络禁止时间是否到达，
        到达后需要删除认为该网络不再禁止且重新通知接入层等效plmn信息 */
    if ((NAS_MMC_TIMER_STATUS_RUNING != NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_DELETE_DISABLED_PLMN_WITH_RAT_TIMER))
     && (NAS_MML_MAX_TIME_SLICE != ulPlmnDisabledInvalidSlice))
    {
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_DELETE_DISABLED_PLMN_WITH_RAT_TIMER,
                            TI_NAS_MMC_PERIOD_DELETE_DISABLED_PLMN_WITH_RAT_TIMER_LEN);
    }

    NAS_MMC_LogForbiddenPlmnRelatedInfo();

    return;
}




VOS_UINT32 NAS_MML_IsImsiHplmnInDamImsiPlmnList(VOS_VOID)
{
    VOS_UINT32                          ulIsHplmnInDamImsiPlmnList;

    ulIsHplmnInDamImsiPlmnList = NAS_MML_GetDamActiveFlag();

    return ulIsHplmnInDamImsiPlmnList;
   

}


VOS_UINT32 NAS_MML_IsPlmnSupportDam(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_SUPPORT_DAM_PLMN_INFO_STRU *pstSupportDamPlmnList = VOS_NULL_PTR;
    VOS_UINT32                          ulIsPlmnInDamPlmnList;
    VOS_UINT32                          ulIsHplmnInDamImsiPlmnList;

    pstSupportDamPlmnList      = NAS_MML_GetSupportDamPlmnList();
    ulIsPlmnInDamPlmnList      = VOS_FALSE;
    ulIsHplmnInDamImsiPlmnList = VOS_FALSE;

    ulIsPlmnInDamPlmnList      = NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstPlmnId,
                                              pstSupportDamPlmnList->ucPlmnNum,
                                              pstSupportDamPlmnList->astPlmnId);

    /* 如果DAM support plmn个数为1，且mcc和mnc为无效，如0XFFFFFF,则对所有PLMN都按照DAM网络处理 */
    if ((VOS_FALSE == NAS_MML_IsPlmnIdValid(&(pstSupportDamPlmnList->astPlmnId[0])))
     && (1 == pstSupportDamPlmnList->ucPlmnNum))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsPlmnSupportDam: CUR CONFIG ALL PLMN like DAM PLMN ");
        ulIsPlmnInDamPlmnList = VOS_TRUE;
    }

    ulIsHplmnInDamImsiPlmnList = NAS_MML_IsImsiHplmnInDamImsiPlmnList();

    /* IMSI HPLMN在DAM imsi plmn列表中并且驻留网络在支持DAM特性的网络列表中返回TRUE */
    if ((VOS_TRUE == ulIsPlmnInDamPlmnList)
     && (VOS_TRUE == ulIsHplmnInDamImsiPlmnList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC,"NAS_MML_IsPlmnSupportDam: CUR PLMN SUPPORT DAM");
        return VOS_TRUE;
    }

    NAS_NORMAL_LOG2(WUEPS_PID_MMC,"NAS_MML_IsPlmnSupportDam: CUR PLMN NOT SUPPORT DAM,ulIsPlmnInDamPlmnList is;  ulIsHplmnInDamImsiPlmnList is;",ulIsPlmnInDamPlmnList,ulIsHplmnInDamImsiPlmnList);

    return VOS_FALSE;
}




VOS_UINT8 NAS_MML_IsCsLaiChanged(VOS_VOID)
{
    NAS_MML_LAI_STRU                   *pstCampLai;
    NAS_MML_LAI_STRU                   *pstLastSuccLai;
    VOS_UINT32                          i;

    pstCampLai     = NAS_MML_GetCurrCampLai();
    pstLastSuccLai = NAS_MML_GetCsLastSuccLai();

    if ((pstCampLai->stPlmnId.ulMcc != pstLastSuccLai->stPlmnId.ulMcc)
     || (pstCampLai->stPlmnId.ulMnc != pstLastSuccLai->stPlmnId.ulMnc))
    {
        return VOS_TRUE;
    }

    for ( i = 0 ; i < NAS_MML_MAX_LAC_LEN ; i++ )
    {
        if (pstCampLai->aucLac[i] != pstLastSuccLai->aucLac[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;

}



VOS_UINT32 NAS_MML_IsExistBufferedEmgCall(VOS_VOID)
{
    VOS_UINT8                           ucIsExistEmgCall;

    ucIsExistEmgCall = NAS_MML_GetCsEmergencyServiceFlg();

    if ((VOS_TRUE == NAS_MML_GetCsServiceBufferStatusFlg())
     && (VOS_TRUE == ucIsExistEmgCall))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}




VOS_UINT32 NAS_MML_IsExistBufferedNormalCall(VOS_VOID)
{
    NAS_MML_MO_CALL_STATUS_ENUM_UINT8   enCallStatus;

    enCallStatus    = NAS_MML_GetMoCallStatus();

    if ((NAS_MML_CSFB_MO_NORMAL_CALL_SETUP_START  == enCallStatus)
     || (NAS_MML_MO_NORMAL_CALL_SETUP_START  == enCallStatus))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}




VOS_VOID NAS_MML_GetValidUserCfgEhplmnInfo(
    VOS_UINT8                          *pucImsi,
    VOS_UINT8                          *pucEhplmnListNum,
    NAS_MML_PLMN_ID_STRU               *pstEhPlmnList
)
{
    VOS_UINT32                                  i;
    VOS_UINT32                                  j;
    VOS_UINT32                                  ulUserCfgEhplmnNum;
    NAS_MML_PLMN_ID_STRU                        stHPlmn;
    NAS_MML_USER_CFG_EXT_EHPLMN_INFO_STRU      *pstUserCfgExtEHplmnInfo;

    pstUserCfgExtEHplmnInfo = NAS_MML_GetUserCfgExtEhplmnInfo();
    ulUserCfgEhplmnNum = pstUserCfgExtEHplmnInfo->ulUserCfgEhplmnNum;
    /* 防止结构体数组越界 */
    if ( ulUserCfgEhplmnNum > NAS_MML_MAX_USER_CFG_EXT_EHPLMN_NUM )
    {
        ulUserCfgEhplmnNum = NAS_MML_MAX_USER_CFG_EXT_EHPLMN_NUM;
    }

    /* 默认用户设置的EHplmn的个数为0个 */
    *pucEhplmnListNum = 0;

    /* 从当前的IMSI中取出home plmn */
    stHPlmn = NAS_MML_GetImsiHomePlmn(pucImsi);


    /* 如果当前的IMSI中的Home Plmn在astImsiPlmnList中，则取当前的EHplmn */
    for (j = 0 ; j < ulUserCfgEhplmnNum; j++)
    {
        for ( i = 0 ; i < pstUserCfgExtEHplmnInfo->astUserCfgEhplmnInfo[j].ucImsiPlmnListNum; i++ )
        {
            /* 用户配置的IMSI plmn网络是BCCH格式的 */
            if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstUserCfgExtEHplmnInfo->astUserCfgEhplmnInfo[j].astImsiPlmnList[i],
                                                            &stHPlmn) )
            {
                *pucEhplmnListNum = pstUserCfgExtEHplmnInfo->astUserCfgEhplmnInfo[j].ucEhplmnListNum;

                /* 取出用户配置的EHplmn内容 */
                PS_MEM_CPY(pstEhPlmnList, pstUserCfgExtEHplmnInfo->astUserCfgEhplmnInfo[j].astEhPlmnList,
                                            sizeof(pstUserCfgExtEHplmnInfo->astUserCfgEhplmnInfo[j].astEhPlmnList) );
                return;
            }
        }
    }


    return ;
}


VOS_UINT32  NAS_MML_GetRrcSimStatus(VOS_VOID)
{
    RRC_NAS_SIM_STATUS_ENUM_UINT32      ulCardStatus;

    /* 卡存在 */
    if ( VOS_TRUE == NAS_MML_GetSimPresentStatus() )
    {
        /* SIM卡存在 */
        if ( NAS_MML_SIM_TYPE_SIM  == NAS_MML_GetSimType())
        {
            ulCardStatus = RRC_NAS_SIM_PRESENT;
        }
        /* USIM卡存在 */
        else
        {
            ulCardStatus = RRC_NAS_USIM_PRESENT;
        }
    }
    /* 卡不存在 */
    else
    {
        ulCardStatus = RRC_NAS_UICC_ABSENT;
    }

    return ulCardStatus;
}



VOS_UINT32 NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList (
    NAS_MML_PLMN_WITH_RAT_STRU         *pstSrcPlmnId,
    VOS_UINT32                          ulDestPlmnNum,
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmnIdList
)
{
    VOS_UINT32                          i;

    for ( i = 0; i < ulDestPlmnNum; i++ )
    {
        if ((VOS_TRUE               == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstSrcPlmnId->stPlmnId), &(pstDestPlmnIdList[i].stPlmnId)))
         && (pstSrcPlmnId->enRat    == pstDestPlmnIdList[i].enRat))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsImsiInForbiddenList (VOS_VOID)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_ID_STRU                stHplmnId;
    NAS_MML_PLMN_ID_STRU                stBlackPlmnId;
    NAS_MML_RAT_FORBIDDEN_LIST_STRU    *pstRatBlackList = VOS_NULL_PTR;
    VOS_UINT8                          *pucImsi         = VOS_NULL_PTR;

    i               = 0;
    pstRatBlackList = NAS_MML_GetRatForbiddenListCfg();

    PS_MEM_SET(&stHplmnId,     0x00, sizeof(stHplmnId));
    PS_MEM_SET(&stBlackPlmnId, 0x00, sizeof(stBlackPlmnId));

    if ( VOS_FALSE == NAS_MML_GetSimPresentStatus() )
    {
        return VOS_FALSE;
    }

    /* 增加测试卡保护，测试卡时不修改，直接返回 */
    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        return VOS_FALSE;
    }


    if ( NAS_MML_RAT_FORBIDDEN_LIST_SWITCH_INACTIVE == pstRatBlackList->enSwitchFlag )
    {
        return VOS_FALSE;
    }

    /* 取得本地维护的IMSI的地址 */
    pucImsi    = NAS_MML_GetSimImsi();

    /* 从当前的IMSI中取出home plmn */
    stHplmnId  = NAS_MML_GetImsiHomePlmn(pucImsi);

    /* 判断IMSI是否在禁止的IMSI列表中 */
    if (NAS_MML_RAT_FORBIDDEN_LIST_SWITCH_BLACK == pstRatBlackList->enSwitchFlag)
    {
        for (i = 0; i < pstRatBlackList->ucImsiListNum; i++)
        {
            /* 如果在黑名单中，将ucIsPlmnIdInPlmnBlackList置为TRUE，需要禁止对应的RAT */
            stBlackPlmnId.ulMcc = pstRatBlackList->astImsiList[i].ulMcc;
            stBlackPlmnId.ulMnc = pstRatBlackList->astImsiList[i].ulMnc;

            if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stBlackPlmnId, &stHplmnId))
            {
                return VOS_TRUE;
            }
        }

        return VOS_FALSE;
    }
    else if (NAS_MML_RAT_FORBIDDEN_LIST_SWITCH_WHITE == pstRatBlackList->enSwitchFlag)
    {
        for (i = 0; i < pstRatBlackList->ucImsiListNum; i++)
        {
            /* 如果在白名单中，直接返回VOS_FALSE(不在黑名单) */
            stBlackPlmnId.ulMcc = pstRatBlackList->astImsiList[i].ulMcc;
            stBlackPlmnId.ulMnc = pstRatBlackList->astImsiList[i].ulMnc;

            if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stBlackPlmnId, &stHplmnId))
            {
                return VOS_FALSE;
            }
        }

        return VOS_TRUE;
    }
    else
    {
    }

    return VOS_FALSE;

}





VOS_UINT32 NAS_MML_IsRatInForbiddenList (
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType
)
{
    VOS_UINT32                          i;
    NAS_MML_RAT_FORBIDDEN_LIST_STRU    *pstRatBlackList = VOS_NULL_PTR;

    i                   = 0;
    pstRatBlackList     = NAS_MML_GetRatForbiddenListCfg();

    /* 如果不在IMSI黑名单中，返回VOS_FALSE */
    if (VOS_FALSE == NAS_MML_GetImsiInForbiddenListFlg())
    {
        return VOS_FALSE;
    }

    /* 如果在禁止IMSI列表中，判断RAT是否在禁止的网络制式列表中 */
    for (i = 0; i < pstRatBlackList->ucForbidRatNum; i++)
    {
        /* 如果传入的RAT在禁止网络制式列表中则返回VOS_TRUE */
        if (enRatType == pstRatBlackList->aenForbidRatList[i])
        {
            NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MML_IsRatInForbiddenList enRatType == ",enRatType);

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;

}



VOS_UINT32 NAS_MML_IsAllMsRatInForbiddenList (VOS_VOID)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList       = VOS_NULL_PTR;
    VOS_UINT32                          i;

    i = 0;

     /* 获取当前接入技术优先级信息 */
    pstPrioRatList  = NAS_MML_GetMsPrioRatList();

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        if (VOS_FALSE == NAS_MML_IsRatInForbiddenList(pstPrioRatList->aucRatPrio[i]))
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}



#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_MML_IsPlatformSupportLte (VOS_VOID)
{
    NAS_MML_PLATFORM_RAT_CAP_STRU              *pstPlatformRatCap;
    VOS_UINT32                                  i;

    /* 获取平台接入技术能力 */
    pstPlatformRatCap = NAS_MML_GetPlatformRatCap();

    for ( i = 0; i < pstPlatformRatCap->ucRatNum; i++ )
    {
        if (NAS_MML_PLATFORM_RAT_TYPE_LTE == pstPlatformRatCap->aenRatPrio[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
#endif


VOS_UINT32 NAS_MML_IsPlatformSupportUtranFdd (VOS_VOID)
{
    NAS_MML_PLATFORM_RAT_CAP_STRU              *pstPlatformRatCap;
    VOS_UINT32                                  i;

    /* 获取平台接入技术能力 */
    pstPlatformRatCap = NAS_MML_GetPlatformRatCap();

    for ( i = 0; i < pstPlatformRatCap->ucRatNum; i++ )
    {
        if (NAS_MML_PLATFORM_RAT_TYPE_WCDMA == pstPlatformRatCap->aenRatPrio[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

#if (FEATURE_ON == FEATURE_UE_MODE_TDS)

VOS_UINT32 NAS_MML_IsPlatformSupportUtranTdd (VOS_VOID)
{
    NAS_MML_PLATFORM_RAT_CAP_STRU              *pstPlatformRatCap;
    VOS_UINT32                                  i;

    /* 获取平台接入技术能力 */
    pstPlatformRatCap = NAS_MML_GetPlatformRatCap();

    for ( i = 0; i < pstPlatformRatCap->ucRatNum; i++ )
    {
        if (NAS_MML_PLATFORM_RAT_TYPE_TD_SCDMA == pstPlatformRatCap->aenRatPrio[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
#endif


VOS_UINT32 NAS_MML_IsDsdsMultiModemMode(VOS_VOID)
{
    /* 如果DSDS 功能未开启，则返回FALSE */
    if (VOS_FALSE == NAS_MML_GetDsdsRfShareFlg() )
    {
        return VOS_FALSE;
    }

    /* 如果当前不是多卡模式，则返回FALSE */
    if (VOS_FALSE == NAS_MML_GetDsdsMultiModemModeFlg())
    {
        return VOS_FALSE;
    }

    /* DSDS功能开启，并且当前是多卡模式，则返回TRUE */
    return VOS_TRUE;
}


VOS_UINT32 NAS_MML_IsPlatformSupportGsm (VOS_VOID)
{
    NAS_MML_PLATFORM_RAT_CAP_STRU              *pstPlatformRatCap;
    VOS_UINT32                                  i;

    /* 获取平台接入技术能力 */
    pstPlatformRatCap = NAS_MML_GetPlatformRatCap();

    for ( i = 0; i < pstPlatformRatCap->ucRatNum; i++ )
    {
        if (NAS_MML_PLATFORM_RAT_TYPE_GSM == pstPlatformRatCap->aenRatPrio[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsPlatformSupportUtran (VOS_VOID)
{
    if (VOS_TRUE == NAS_MML_IsPlatformSupportUtranFdd())
    {
        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_UE_MODE_TDS)
    if (VOS_TRUE == NAS_MML_IsPlatformSupportUtranTdd())
    {
        return VOS_TRUE;
    }
#endif

    return VOS_FALSE;
}

VOS_UINT32 NAS_MML_IsAcInfoChanged(
    NAS_MML_ACCESS_RESTRICTION_STRU    *pstOldAcRestrictInfo,
    NAS_MML_ACCESS_RESTRICTION_STRU    *pstNewAcRestrictInfo
)
{
    /* 新老限制注册、限制寻呼、限制正常业务、限制紧急业务标记是否有变化 */
    if ((pstOldAcRestrictInfo->ucRestrictPagingRsp        != pstNewAcRestrictInfo->ucRestrictPagingRsp)
     || (pstOldAcRestrictInfo->ucRestrictRegister         != pstNewAcRestrictInfo->ucRestrictRegister)
     || (pstOldAcRestrictInfo->ucRestrictNormalService    != pstNewAcRestrictInfo->ucRestrictNormalService)
     || (pstOldAcRestrictInfo->ucRestrictEmergencyService != pstNewAcRestrictInfo->ucRestrictEmergencyService))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_VOID NAS_MML_Update_Revision_Level(
    VOS_UINT8                          *pucClassMark
)
{
    VOS_INT8                    cVersion;
    VOS_UINT8                   ucRevisionLevel;

    cVersion = NAS_Common_Get_Supported_3GPP_Version(MM_COM_SRVDOMAIN_CS);

    /* R99以前版本，上报 revision level 为 GSM PHASE 2*/
    if(PS_PTL_VER_PRE_R99 == cVersion)
    {
        ucRevisionLevel = MM_CLASSMARK_REVISION_LEVEL_GSM_PH2;
    }
    else
    {
        ucRevisionLevel = MM_CLASSMARK_REVISION_LEVEL_R99_OR_LATER;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* L使用时都是R99以后的版本*/
    if(NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        ucRevisionLevel = MM_CLASSMARK_REVISION_LEVEL_R99_OR_LATER;
    }
#endif

    *pucClassMark &= 0x9f;
    *pucClassMark |= (VOS_UINT8)(ucRevisionLevel << 5);

    return;
}


VOS_VOID NAS_MML_GetSupportBandNum(
    VOS_UINT32                          ulBand,
    VOS_UINT8                          *pucBandNum
)
{
    /* ucIndicator用来进行移位指示 */
    VOS_UINT8                           ucIndicator;

    /* ucTotalLen表示用来参数usBand的总bit位数 */
    VOS_UINT8                           ucTotalLen;

    /* ucBandNum用来记录bit值为1的个数，并作为函数的返回值 */
    *pucBandNum = 0;
    ucIndicator = 0;
    ucTotalLen  = sizeof(ulBand) * 8;

    /* 通过循环移位计算ulBand中的bit值为1的总个数，并保存到ucBandNum中 */
    while (ucIndicator < ucTotalLen)
    {
        if ((ulBand>>ucIndicator) & 0x01)
        {
            (*pucBandNum)++;
        }

        ucIndicator++;
    }

    return;
}


VOS_VOID NAS_MML_Fill_IE_ClassMark2(
    VOS_UINT8                          *pClassMark2
)
{
    GSM_BAND_SET_UN                     unSysCfgSetGsmBand;

    VOS_UINT8                           ucBandNum;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrNetRat;
    NAS_MML_MS_BAND_INFO_STRU          *pstCurrBandInfo = VOS_NULL_PTR;
    NAS_MML_MS_CAPACILITY_INFO_STRU    *pstMsCapability = VOS_NULL_PTR;
    NAS_MML_MS_BAND_INFO_STRU          *pstUserSetBand  = VOS_NULL_PTR;

    pstUserSetBand  = NAS_MML_GetMsSupportBand();
    pstMsCapability = NAS_MML_GetMsCapability();
    pstCurrBandInfo = NAS_MML_GetCurrBandInfo();
    enCurrNetRat    = NAS_MML_GetCurrNetRatType();

    if (pstMsCapability->aucClassmark2[0] != 0)
    {
        pClassMark2[0] = pstMsCapability->aucClassmark2[0];
        pClassMark2[1] = pstMsCapability->aucClassmark2[1];
        pClassMark2[2] = pstMsCapability->aucClassmark2[2];
        pClassMark2[3] = pstMsCapability->aucClassmark2[3];

        NAS_MML_Update_Revision_Level(&pClassMark2[1]);

        /* 默认设置FC为0 */
        pClassMark2[2] &= 0xFE;

        /* 不支持G就不填写GSM能力 */
        if ( VOS_FALSE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_GSM))
        {
            // W only
            // ES IND => 0, A5/1 => 1, RF power capability => 111
            pClassMark2[1] &= 0xE0;
            pClassMark2[1] |= 0x0F;

            /* PS capability => 0, VBS => 0, VGCS => 0 */
            pClassMark2[2] &= 0xB9;
            /* SoLSA => 0, A5/3 => 0 */
            pClassMark2[3] &= 0xF5;

        }
        else
        {   /* W/G or G only */
            /* 获取当前的小区网络频段 */
            unSysCfgSetGsmBand.ulBand = pstCurrBandInfo->unGsmBand.ulBand;
            if ( (NAS_MML_NET_RAT_TYPE_WCDMA == enCurrNetRat)
#if (FEATURE_ON == FEATURE_LTE)
              || (NAS_MML_NET_RAT_TYPE_LTE == enCurrNetRat)
#endif
             )
            {
                unSysCfgSetGsmBand.ulBand = pstUserSetBand->unGsmBand.ulBand;
            }
            /*根据当前驻留的频段填写RF power capability*/
            if (( VOS_TRUE == unSysCfgSetGsmBand.stBitBand.BandGsm1800)
             || (VOS_TRUE== unSysCfgSetGsmBand.stBitBand.BandGsm1900))
            {
                /*power class 1*/
                pClassMark2[1] &= ~MM_CLASSMARK_RF_POWER_CAP_MASK;
                pClassMark2[1] |= MM_CLASSMARK_RF_POWER_CAP_CLASS1;
            }
            else /* 其它频段 */
            {
                /*power class 4*/
                pClassMark2[1] &= ~MM_CLASSMARK_RF_POWER_CAP_MASK;
                pClassMark2[1] |= MM_CLASSMARK_RF_POWER_CAP_CLASS4;
            }

            /*当前网络驻留在G下*/
            if (NAS_MML_NET_RAT_TYPE_GSM == enCurrNetRat)
            {

                /* 如果是900，需要判断是否支持E900或R900,如果支持FC为1，
                   否则只支持P900且驻留P900，则FC为0 */
                if ( ( (VOS_TRUE == unSysCfgSetGsmBand.stBitBand.BandGsmE900)
                    || (VOS_TRUE == unSysCfgSetGsmBand.stBitBand.BandGsmR900))
                 || (VOS_TRUE == unSysCfgSetGsmBand.stBitBand.BandGsmP900))
                {
                    if (pstUserSetBand->unGsmBand.ulBand & 0x0030)
                    {
                        pClassMark2[2] |= 0x01;
                    }
                }
            }
            else
            {
                /* 获取用户设置的可在小区内驻留的频段 */
                unSysCfgSetGsmBand.ulBand = pstUserSetBand->unGsmBand.ulBand;

                NAS_MML_GetSupportBandNum(unSysCfgSetGsmBand.ulBand, &ucBandNum);

                /*如果支持多频段或者不支持任何频段，设置RF power capability => 111*/
                if ( ((ucBandNum > 1)
                     ||(0 == ucBandNum))
                   ||((1 == ucBandNum) && ((VOS_TRUE == unSysCfgSetGsmBand.stBitBand.BandGsmE900)
                   || (VOS_TRUE == unSysCfgSetGsmBand.stBitBand.BandGsmR900))) )
                {
                    pClassMark2[1] |= MM_CLASSMARK_RF_POWER_CAP_MASK;
                }

            }

        }
    }
    return;
}



VOS_UINT32  NAS_MML_IsCsServDomainAvail(
    NAS_MML_MS_MODE_ENUM_UINT8          enMsMode
)
{
    /* A模式、CS ONLY模式时，认为CS域可用，其他模式认为CS域不可用 */
    if ( (NAS_MML_MS_MODE_PS_CS == enMsMode)
      || (NAS_MML_MS_MODE_CS_ONLY == enMsMode) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_VOID NAS_MML_ConvertOamSendPid(
    VOS_UINT32                         *pulConvertedSendPid
)
{
    VOS_UINT32                          ulOrigSendPid;

    ulOrigSendPid = *pulConvertedSendPid;

    if ((I0_WUEPS_PID_USIM == ulOrigSendPid)
     || (I1_WUEPS_PID_USIM == ulOrigSendPid)
     || (I2_WUEPS_PID_USIM == ulOrigSendPid))
    {
        *pulConvertedSendPid = WUEPS_PID_USIM;
        return;
    }

    if ((I0_MAPS_STK_PID == ulOrigSendPid)
     || (I1_MAPS_STK_PID == ulOrigSendPid)
     || (I2_MAPS_STK_PID == ulOrigSendPid))
    {
        *pulConvertedSendPid = MAPS_STK_PID;
        return;
    }

    if ((I0_MAPS_PB_PID == ulOrigSendPid)
     || (I1_MAPS_PB_PID == ulOrigSendPid)
     || (I2_MAPS_PB_PID == ulOrigSendPid))
    {
        *pulConvertedSendPid = MAPS_PB_PID;
        return;
    }

    if ((I0_MAPS_PIH_PID == ulOrigSendPid)
     || (I1_MAPS_PIH_PID == ulOrigSendPid)
     || (I2_MAPS_PIH_PID == ulOrigSendPid))
    {
        *pulConvertedSendPid = MAPS_PIH_PID;
        return;
    }
}

#if (FEATURE_ON == FEATURE_PTM)

VOS_UINT32 NAS_MML_IsErrLogNeedRecord(VOS_UINT16 usLevel)
{
    /* Log开关关闭，不需要上报 */
    if (0 == NAS_MML_GetErrlogCtrlFlag())
    {
        return VOS_FALSE;
    }

    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        return VOS_FALSE;
    }

    /* 模块log级别usLevel大于用户设置的log上报级别或usLevel无效，不需要上报 */
    if ((NAS_MML_GetErrlogAlmLevel() < usLevel)
     || (NAS_ERR_LOG_CTRL_LEVEL_NULL == usLevel))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MML_IsNeedNwSearchInfoRecord(
    VOS_VOID                           *pstMsg
)
{
    MSG_HEADER_STRU                    *pstMsgHeader  = VOS_NULL_PTR;
    NAS_MML_ERRLOG_NW_SEARCH_INFO_STRU *pstNwSrchInfo = VOS_NULL_PTR;
    NAS_MML_REG_STATUS_ENUM_UINT8       ucCsRegState;
    NAS_MML_REG_STATUS_ENUM_UINT8       ucPsRegState;
    VOS_UINT32                          ulIsAsPid;

    pstMsgHeader  = (MSG_HEADER_STRU *)pstMsg;
    ucCsRegState  = NAS_MML_GetCsRegStatus();
    ucPsRegState  = NAS_MML_GetPsRegStatus();
    pstNwSrchInfo = NAS_MML_GetErrLogNwSearchInfoAddr();
    ulIsAsPid     = NAS_MML_IsAsPid(pstMsgHeader->ulSenderPid);

    if (WUEPS_PID_MMC != pstMsgHeader->ulReceiverPid)
    {
        return VOS_FALSE;
    }

    /* 搜网结果回复以及停止搜网结果回复需要记录CHR */
    if (((ID_LMM_MMC_PLMN_SRCH_CNF      == pstMsgHeader->ulMsgName)
      || (ID_LMM_MMC_STOP_PLMN_SRCH_CNF == pstMsgHeader->ulMsgName)
      || (RRMM_PLMN_SEARCH_CNF          == pstMsgHeader->ulMsgName)
      || (RRMM_PLMN_SEARCH_STOP_CNF     == pstMsgHeader->ulMsgName))
     && (VOS_TRUE == ulIsAsPid))
    {
        /* 如果当前在注册状态发生的搜网，则不记录CHR */
        if (((NAS_MML_REG_REGISTERED_HOME_NETWORK == ucCsRegState)
          || (NAS_MML_REG_REGISTERED_ROAM         == ucCsRegState))
         && ((NAS_MML_REG_REGISTERED_HOME_NETWORK == ucPsRegState)
          || (NAS_MML_REG_REGISTERED_ROAM         == ucPsRegState)))
        {
            return VOS_FALSE;
        }

        /* L下的快速指定搜，则不需要记录CHR */
        if ((PS_PID_MM                   == pstMsgHeader->ulSenderPid)
         && (MMC_LMM_PLMN_SRCH_FAST_SPEC == pstNwSrchInfo->ulSearchType))
        {
            return VOS_FALSE;
        }

        /* GUT下的快速指定搜，则不需要记录CHR */
        if ((VOS_TRUE                    == NAS_MML_IsGutAsPid(pstMsgHeader->ulSenderPid))
         && (RRC_PLMN_SEARCH_SPEC_FAST   == pstNwSrchInfo->ulSearchType))
        {
            return VOS_FALSE;
        }

        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MML_RecordNwSearchInfo(
    VOS_VOID                           *pstMsg
)
{
    MSG_HEADER_STRU                            *pstMsgHeader      = VOS_NULL_PTR;
    NAS_MML_ERRLOG_NW_SEARCH_INFO_STRU         *pstNwSrchInfo     = VOS_NULL_PTR;
    LMM_MMC_PLMN_SRCH_CNF_STRU                 *pstLmmSrchCnfMsg  = VOS_NULL_PTR;
    RRMM_PLMN_SEARCH_CNF_STRU                  *pstRrmmSrchCnfMsg = VOS_NULL_PTR;
    NAS_ERR_LOG_OOS_NW_SRCH_PROC_INFO_STRU      stProcInfoEvent;
    VOS_UINT32                                  ulIsLogNeedRecord;
    VOS_UINT32                                  ulLength;
    VOS_UINT32                                  ulResult;
    VOS_UINT16                                  usLevel;
    VOS_UINT8                                   ucCfgActiveFlag;
    VOS_UINT32                                  ulCfgRecordNum;

    /* 查询对应Alarm Id是否需要记录异常信息 */
    usLevel           = NAS_GetErrLogAlmLevel(NAS_ERR_LOG_ALM_OOS_NW_SRCH_PROC_INFO);
    ulIsLogNeedRecord = NAS_MML_IsErrLogNeedRecord(usLevel);
    pstNwSrchInfo     = NAS_MML_GetErrLogNwSearchInfoAddr();
    ulCfgRecordNum    = NAS_MML_GetErrLogNwSrchCfgRecordNum();
    ucCfgActiveFlag   = NAS_MML_GetErrLogNwSrchCfgActiveFlag();

    pstMsgHeader      = (MSG_HEADER_STRU *)pstMsg;

    /* 1. 模块异常不需要记录
       2. NV配置不需要记录 */
    if ((VOS_FALSE == ulIsLogNeedRecord)
     || (VOS_FALSE == ucCfgActiveFlag))
    {
        return;
    }

    /* 如果超过NV记录的最大次数，则不再记录 */
    if (pstNwSrchInfo->ulSearchCount > ulCfgRecordNum)
    {
        return;
    }

    /* 搜索过程记录次数加1 */
    NAS_MML_AddErrLogNwSearchCount();

    ulLength = sizeof(NAS_ERR_LOG_OOS_NW_SRCH_PROC_INFO_STRU);

    /* 填充本地记录信息 */
    PS_MEM_SET(&stProcInfoEvent, 0x00, ulLength);

    NAS_COMM_BULID_ERRLOG_HEADER_INFO(&stProcInfoEvent.stHeader,
                                      VOS_GetModemIDFromPid(WUEPS_PID_MMC),
                                      NAS_ERR_LOG_ALM_OOS_NW_SRCH_PROC_INFO,
                                      usLevel,
                                      VOS_GetSlice(),
                                      (ulLength - sizeof(OM_ERR_LOG_HEADER_STRU)));

    stProcInfoEvent.stPlmnId.ulMcc          = pstNwSrchInfo->stPlmnId.ulMcc;
    stProcInfoEvent.stPlmnId.ulMnc          = pstNwSrchInfo->stPlmnId.ulMnc;
    stProcInfoEvent.enSrchRat               = pstNwSrchInfo->enSrchRat;
    stProcInfoEvent.ucSearchType            = (VOS_UINT8)pstNwSrchInfo->ulSearchType;
    stProcInfoEvent.ulPlmnSrchReqSlice      = pstNwSrchInfo->ulPlmnSrchReqSlice;
    stProcInfoEvent.ulCurPlmnSelectionScene           = pstNwSrchInfo->ulCurPlmnSelectionScene;
    stProcInfoEvent.stOosInfo.ucCurOosPhaseNum        = pstNwSrchInfo->stOosInfo.ucCurOosPhaseNum;
    stProcInfoEvent.stOosInfo.ulCurHistoryTimerCount  = pstNwSrchInfo->stOosInfo.ulCurHistoryTimerCount;
    stProcInfoEvent.stOosInfo.ulCurPrefBandTimerCount = pstNwSrchInfo->stOosInfo.ulCurPrefBandTimerCount;
    stProcInfoEvent.stOosInfo.ulCurFullBandTimerCount = pstNwSrchInfo->stOosInfo.ulCurFullBandTimerCount;
    PS_MEM_CPY(stProcInfoEvent.aucDplmnVersionId, pstNwSrchInfo->aucDplmnVersionId, NAS_INFO_VERSION_INFO_LEN);
    PS_MEM_CPY(stProcInfoEvent.aucMccFreqVersion, pstNwSrchInfo->aucMccFreqVersion, NAS_INFO_VERSION_INFO_LEN);

    if (ID_LMM_MMC_PLMN_SRCH_CNF == pstMsgHeader->ulMsgName)
    {
        pstLmmSrchCnfMsg  = (LMM_MMC_PLMN_SRCH_CNF_STRU *)pstMsg;
        stProcInfoEvent.ucResult        = (VOS_UINT8)pstLmmSrchCnfMsg->enRlst;
        stProcInfoEvent.ucCoverageType  = (VOS_UINT8)pstLmmSrchCnfMsg->enCoverageType;
    }
    else if (RRMM_PLMN_SEARCH_CNF == pstMsgHeader->ulMsgName)
    {
        pstRrmmSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;
        stProcInfoEvent.ucResult        = (VOS_UINT8)pstRrmmSrchCnfMsg->ulSearchRlt;
        stProcInfoEvent.ucCoverageType  = (VOS_UINT8)pstRrmmSrchCnfMsg->enCoverageType;
    }
    else
    {
        stProcInfoEvent.ucResult        = 0xFF;
        stProcInfoEvent.ucCoverageType  = 0xFF;
    }

    /* 清空之前记录的搜网请求信息 */
    pstNwSrchInfo->stPlmnId.ulMcc           = 0;
    pstNwSrchInfo->stPlmnId.ulMnc           = 0;
    pstNwSrchInfo->enSrchRat                = NAS_ERR_LOG_RATMODE_BUTT;
    pstNwSrchInfo->ulSearchType             = 0xFF;
    pstNwSrchInfo->ulPlmnSrchReqSlice       = 0;
    pstNwSrchInfo->ulCurPlmnSelectionScene            = NAS_MMC_PLMN_SEARCH_SCENE_BUTT;
    PS_MEM_SET(&pstNwSrchInfo->stOosInfo, 0x00, sizeof(NAS_MML_OOS_INFO_STRU));
    pstNwSrchInfo->stOosInfo.ucCurOosPhaseNum         = NAS_MMC_OOS_PHASE_BUTT;
    PS_MEM_SET(pstNwSrchInfo->aucDplmnVersionId, 0x00, NAS_INFO_VERSION_INFO_LEN);
    PS_MEM_SET(pstNwSrchInfo->aucMccFreqVersion, 0x00, NAS_INFO_VERSION_INFO_LEN);

    /*
       将异常信息写入Buffer中
       实际写入的字符数与需要写入的不等则打印异常
     */
    ulResult = NAS_MML_PutErrLogRingBuf((VOS_CHAR *)&stProcInfoEvent, ulLength);
    if (ulResult != ulLength)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MML_RecordNwSearchInfo(): Push buffer error.");
    }

    NAS_COM_MntnPutRingbuf(NAS_ERR_LOG_ALM_OOS_NW_SRCH_PROC_INFO,
                           WUEPS_PID_MMC,
                           (VOS_UINT8 *)&stProcInfoEvent,
                           sizeof(stProcInfoEvent));

    return;
}

#endif


VOS_UINT32 NAS_MML_IsGutAsPid(VOS_UINT32 ulPid)
{
    VOS_UINT32                          ulRslt;

    switch (ulPid)
    {
        case UEPS_PID_GAS:
        case WUEPS_PID_WRR:
        case TPS_PID_RRC:
            ulRslt = VOS_TRUE;
            break;

        default:
            ulRslt = VOS_FALSE;
            break;
    }

    return ulRslt;
}


VOS_UINT32 NAS_MML_IsAsPid(VOS_UINT32 ulPid)
{
    VOS_UINT32                          ulRslt;

    switch (ulPid)
    {
        case PS_PID_MM:
        case UEPS_PID_GAS:
        case WUEPS_PID_WRR:
        case TPS_PID_RRC:
            ulRslt = VOS_TRUE;
            break;

        default:
            ulRslt = VOS_FALSE;
            break;
    }

    return ulRslt;
}


VOS_VOID NAS_MML_SaveNwSearchReqInfo(
    VOS_VOID                           *pstMsg
)
{
    MSG_HEADER_STRU                        *pstMsgHeader       = VOS_NULL_PTR;
    MMC_LMM_PLMN_SRCH_REQ_STRU             *pstLmmPlmnSrchReq  = VOS_NULL_PTR;
    RRMM_PLMN_SEARCH_REQ_STRU              *pstRrmmPlmnSrchReq = VOS_NULL_PTR;
    NAS_MML_ERRLOG_NW_SEARCH_INFO_STRU     *pstNwSrchInfo      = VOS_NULL_PTR;
    NAS_MML_SIM_FORMAT_PLMN_ID              stSimPlmn;
    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU      *pstDplmnNPlmnCfgInfo = VOS_NULL_PTR;
    NAS_MML_OOS_INFO_STRU                   stOosInfo;
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32   enCurPlmnSelectionScene;
    VOS_UINT8                               aucMccFreqVersion[NAS_INFO_VERSION_INFO_LEN] = {0};
    VOS_UINT32                          ulReceiverPid;

    pstMsgHeader  = (MSG_HEADER_STRU *)pstMsg;
    pstNwSrchInfo = NAS_MML_GetErrLogNwSearchInfoAddr();

    ulReceiverPid = pstMsgHeader->ulReceiverPid;

    /* 记录OOS相关信息 */
    PS_MEM_SET(&stOosInfo, 0x00, sizeof(NAS_MML_OOS_INFO_STRU));
    NAS_MMC_GetOosCurInfo(&stOosInfo);

    /* 记录当前搜网场景 */
    enCurPlmnSelectionScene = NAS_MMC_PLMN_SEARCH_SCENE_BUTT;

    if (NAS_MMC_FSM_PLMN_SELECTION == NAS_MMC_GetCurrFsmId())
    {
        enCurPlmnSelectionScene = NAS_MMC_GetPlmnSearchScene_PlmnSelection();
    }

    /* 记录Dplmn版本号 */
    pstDplmnNPlmnCfgInfo = NAS_MMC_GetDPlmnNPlmnCfgInfo();

    /* 记录预置频点版本号 */
    //aucMccFreqVersion[NAS_INFO_VERSION_INFO_LEN] = {0};

    /* LTE的搜网请求 */
    if ((ID_MMC_LMM_PLMN_SRCH_REQ == pstMsgHeader->ulMsgName)
     && (PS_PID_MM                == ulReceiverPid))
    {
        PS_MEM_SET(&stSimPlmn, 0x0, sizeof(NAS_MML_SIM_FORMAT_PLMN_ID));

        pstLmmPlmnSrchReq = (MMC_LMM_PLMN_SRCH_REQ_STRU *)pstMsg;

        pstNwSrchInfo->enSrchRat          = NAS_ERR_LOG_RATMODE_LTE;
        pstNwSrchInfo->ulSearchType       = pstLmmPlmnSrchReq->enSrchType;
        pstNwSrchInfo->ulPlmnSrchReqSlice = VOS_GetSlice();

        pstNwSrchInfo->ulCurPlmnSelectionScene            = enCurPlmnSelectionScene;
        pstNwSrchInfo->stOosInfo.ucCurOosPhaseNum         = stOosInfo.ucCurOosPhaseNum;
        pstNwSrchInfo->stOosInfo.ulCurHistoryTimerCount   = stOosInfo.ulCurHistoryTimerCount;
        pstNwSrchInfo->stOosInfo.ulCurPrefBandTimerCount  = stOosInfo.ulCurPrefBandTimerCount;
        pstNwSrchInfo->stOosInfo.ulCurFullBandTimerCount  = stOosInfo.ulCurFullBandTimerCount;
        PS_MEM_CPY(pstNwSrchInfo->aucDplmnVersionId, pstDplmnNPlmnCfgInfo->aucVersionId, NAS_INFO_VERSION_INFO_LEN);
        PS_MEM_CPY(pstNwSrchInfo->aucMccFreqVersion, aucMccFreqVersion, NAS_INFO_VERSION_INFO_LEN);

        /* 先转换为SIM格式 */
        PS_MEM_CPY(stSimPlmn.aucSimPlmn,
                   pstLmmPlmnSrchReq->stSpecPlmnId.aucPlmnId,
                   MMC_LMM_PLMN_ID_LEN);

        NAS_MML_ConvertSimPlmnToNasPLMN(&stSimPlmn,
                                        (NAS_MML_PLMN_ID_STRU *)&pstNwSrchInfo->stPlmnId);

        return;
    }

    /* GUT的搜网请求 */
    if (RRMM_PLMN_SEARCH_REQ == pstMsgHeader->ulMsgName)
    {
        pstRrmmPlmnSrchReq = (RRMM_PLMN_SEARCH_REQ_STRU *)pstMsg;

        pstNwSrchInfo->ulSearchType       = pstRrmmPlmnSrchReq->ulSearchType;
        pstNwSrchInfo->stPlmnId.ulMcc     = pstRrmmPlmnSrchReq->aPlmnIdList[0].ulMcc;
        pstNwSrchInfo->stPlmnId.ulMnc     = pstRrmmPlmnSrchReq->aPlmnIdList[0].ulMnc;
        pstNwSrchInfo->ulPlmnSrchReqSlice = VOS_GetSlice();

        pstNwSrchInfo->ulCurPlmnSelectionScene            = enCurPlmnSelectionScene;
        pstNwSrchInfo->stOosInfo.ucCurOosPhaseNum         = stOosInfo.ucCurOosPhaseNum;
        pstNwSrchInfo->stOosInfo.ulCurHistoryTimerCount   = stOosInfo.ulCurHistoryTimerCount;
        pstNwSrchInfo->stOosInfo.ulCurPrefBandTimerCount  = stOosInfo.ulCurPrefBandTimerCount;
        pstNwSrchInfo->stOosInfo.ulCurFullBandTimerCount  = stOosInfo.ulCurFullBandTimerCount;
        PS_MEM_CPY(pstNwSrchInfo->aucDplmnVersionId, pstDplmnNPlmnCfgInfo->aucVersionId, NAS_INFO_VERSION_INFO_LEN);
        PS_MEM_CPY(pstNwSrchInfo->aucMccFreqVersion, aucMccFreqVersion, NAS_INFO_VERSION_INFO_LEN);

        if (UEPS_PID_GAS == ulReceiverPid)
        {
            pstNwSrchInfo->enSrchRat = NAS_ERR_LOG_RATMODE_GSM;
        }
        else if (WUEPS_PID_WRR == ulReceiverPid)
        {
            pstNwSrchInfo->enSrchRat = NAS_ERR_LOG_RATMODE_WCDMA;
        }
        else
        {
            pstNwSrchInfo->enSrchRat = NAS_ERR_LOG_RATMODE_TDS;
        }

        return;

    }
}


VOS_VOID  NAS_MML_ConvertSimPlmnToNasPLMN(
    NAS_MML_SIM_FORMAT_PLMN_ID          *pstSimPlmn,
    NAS_MML_PLMN_ID_STRU                *pstNasPlmn
)
{

    VOS_UINT32                          ulTempData;
    NAS_MML_PLMN_ID_STRU                stPlmn;

    /*
       PLMN BCD 编码 方式

       MCC digit 2,  MCC digit 1, ----OCTET 1
       MNC digit 3,  MCC digit 3, ----OCTET 2
       MNC digit 2,  MNC digit 1, ----OCTET 3
    */
    ulTempData = NAS_MML_OCTET_LOW_FOUR_BITS & (pstSimPlmn->aucSimPlmn[0]);
    stPlmn.ulMcc = ulTempData;
    ulTempData = (NAS_MML_OCTET_HIGH_FOUR_BITS & (pstSimPlmn->aucSimPlmn[0])) >> NAS_MML_OCTET_MOVE_FOUR_BITS;
    stPlmn.ulMcc |= ulTempData << NAS_MML_OCTET_MOVE_EIGHT_BITS;

    ulTempData = NAS_MML_OCTET_LOW_FOUR_BITS & (pstSimPlmn->aucSimPlmn[1]);
    stPlmn.ulMcc |= ulTempData << NAS_MML_OCTET_MOVE_SIXTEEN_BITS;
    ulTempData = (NAS_MML_OCTET_HIGH_FOUR_BITS & (pstSimPlmn->aucSimPlmn[1])) >> NAS_MML_OCTET_MOVE_FOUR_BITS;

    stPlmn.ulMnc = ulTempData << NAS_MML_OCTET_MOVE_SIXTEEN_BITS;
    ulTempData = NAS_MML_OCTET_LOW_FOUR_BITS & (pstSimPlmn->aucSimPlmn[2]);
    stPlmn.ulMnc |= ulTempData;
    ulTempData = (NAS_MML_OCTET_HIGH_FOUR_BITS & (pstSimPlmn->aucSimPlmn[2])) >> NAS_MML_OCTET_MOVE_FOUR_BITS;
    stPlmn.ulMnc |= ulTempData << NAS_MML_OCTET_MOVE_EIGHT_BITS;

    *pstNasPlmn = stPlmn;


    return ;

}





VOS_UINT32 NAS_MML_DecodeEmergencyNumList(
    VOS_UINT16                         *pusIndex,
    VOS_UINT8                          *pucRcvMsg,
    VOS_UINT16                          usMsgSize
)
{
    NAS_MML_EMERGENCY_NUM_LIST_STRU    *pstEmergencyNumList = VOS_NULL_PTR;
    VOS_UINT16                          usIndex;
    VOS_UINT8                           ucTotalIeLength;
    VOS_UINT8                           ucParseEmcTotalLength;
    VOS_UINT8                           ucTempLength;
    VOS_UINT8                           ucLength;
    VOS_UINT8                           i;

    usIndex                             = *pusIndex;
    ucTotalIeLength                     = 0;
    ucTempLength                        = 0;
    ucLength                            = 0;
    ucParseEmcTotalLength               = 0;

    pstEmergencyNumList                 = NAS_MML_GetEmergencyNumList();

    pstEmergencyNumList->ucEmergencyNumber = 0;

    /* 整个紧急呼IE的长度，包括IEI */
    ucTotalIeLength       = pucRcvMsg[usIndex + 1] + 2;

    /* Length of Emergency Number List IE contents */
    ucParseEmcTotalLength = pucRcvMsg[usIndex + 1];

    /* 按照紧急呼填写的总长度值解析，会超过整个消息的长度，则认为异常 */
    if ( ( *pusIndex + ucTotalIeLength) > usMsgSize )
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MML_DecodeEmergencyNumList: IE emergency number length error!!!");

        *pusIndex = usMsgSize;

        /* 解析异常，直接返回END */
        return VOS_FALSE;
    }

    if ( ucParseEmcTotalLength < NAS_MML_MIN_EMERGENCY_NUM_LEN )
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MML_DecodeEmergencyNumList: IE emergency number length < NAS_MML_MIN_EMERGENCY_NUM_LEN error !!!");

        /* 直接指向紧急呼的结束符号:即下一个IEI或解析完毕了 */
        *pusIndex = *pusIndex + ucTotalIeLength;

        return VOS_FALSE;
    }

    if ( ucParseEmcTotalLength > NAS_MML_MAX_EMERGENCY_NUM_LEN )
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MML_DecodeEmergencyNumList: IE emergency number length > NAS_MML_MAX_EMERGENCY_NUM_LEN error !!!");

        ucParseEmcTotalLength =  NAS_MML_MAX_EMERGENCY_NUM_LEN;
    }

    /* 指向第一个紧急呼列表 */
    usIndex = usIndex + 2;

    while (ucParseEmcTotalLength > ucTempLength)
    {

       ucLength     = pucRcvMsg[usIndex];                                      /* Length of Nst Emergency Number information */

       /*************************************************************************
       *TS 24.007, section 11.4.2
       * -- It is not a syntactical error that a type 4 standard IE specifies in
       * its length indicator a greater length than possible according to the
       * value part specification: extra bits shall be ignored.
       *
       *TS 24.007, section 11.2.1.1.4
       *A type 4 standard information element has format LV or TLV. Its LI precedes
       *the value part, which consists of zero, one, or more octets; if present,
       *its IEI has one octet length and precedes the LI.
       *
       *Hence, this is a type 4 information element.
       **************************************************************************/
        if ( (ucTempLength + 1 + NAS_MML_MIN(ucLength, NAS_MML_MAX_EMERGENCY_NUM_INFO_LEN)) > ucParseEmcTotalLength )
        {
            /* 直接指向紧急呼的结束符号:即下一个IEI或解析完毕了 */
            *pusIndex = *pusIndex + ucTotalIeLength;

            return VOS_FALSE;
        }

        /* 含有无效的EMERGENCY Number List子项,则认为整个紧急呼列表都无效 */
        if (ucLength < NAS_MML_MIN_EMERGENCY_NUM_INFO_LEN)
        {
            pstEmergencyNumList->ucEmergencyNumber = 0;
            *pusIndex = *pusIndex + ucTotalIeLength;
            return VOS_FALSE;
        }

        ucTempLength = ucTempLength + ucLength + 1;                             /* 累计所有Emergency Number List的总长度，包括 Length IE */

        if (ucLength > NAS_MML_MAX_EMERGENCY_NUM_INFO_LEN)
        {
            /* usIndex指向下一个紧急呼长度 */
            usIndex += ucTempLength;

            /* 仅忽略掉当前紧急呼项 */
            continue;
        }

        pstEmergencyNumList->ucEmergencyNumber++;
        pstEmergencyNumList->aucEmergencyList[pstEmergencyNumList->ucEmergencyNumber - 1].ucEmcNumLen
            = ucLength - 1;
        pstEmergencyNumList->aucEmergencyList[pstEmergencyNumList->ucEmergencyNumber - 1].ucCategory
            = pucRcvMsg[usIndex + 1] & 0x1F;                                    /* Emergency Service Category Value         */

        /* 偏移到 Number digit 1 字节 */
        usIndex = usIndex + 2;

        for (i = 0; i < (ucLength - 1); i++)
        {
            pstEmergencyNumList->aucEmergencyList[pstEmergencyNumList->ucEmergencyNumber - 1].aucEmcNum[i]
                = pucRcvMsg[usIndex];                                        /* Number digit N                           */
            usIndex++;
        }

        /* usIndex指向下一个紧急呼长度 */

        if (pstEmergencyNumList->ucEmergencyNumber >= NAS_MML_EMERGENCY_NUM_LIST_MAX_RECORDS)
        {
            /* 达到自大紧急呼个数，则认为解析完毕 */
            *pusIndex = *pusIndex + ucTotalIeLength;

            return VOS_TRUE;
        }

    }

    /* 解析完毕，则指向最后一个字符 */
    *pusIndex = *pusIndex + ucTotalIeLength;

    return VOS_TRUE;
}




VOS_UINT32 NAS_MML_IsNvimOplmnAvail(VOS_VOID)
{
    VOS_UINT32                          ulStep;
    NAS_MML_PLMN_ID_STRU                stHPlmn;
    RRC_NAS_SIM_STATUS_ENUM_UINT32      ulCardStatus;
    VOS_UINT8                          *pucImsi             = VOS_NULL_PTR;
    NAS_MML_USER_CFG_OPLMN_INFO_STRU   *pstUserCfgOPlmnInfo = VOS_NULL_PTR;

    PS_MEM_SET(&stHPlmn, 0x00, sizeof(NAS_MML_PLMN_ID_STRU));

    pstUserCfgOPlmnInfo = NAS_MML_GetUserCfgOPlmnInfo();

    /* 特性开关未打开，或OPLMN个数为0，内置OPLMN功能无效 */
    if ((VOS_FALSE == pstUserCfgOPlmnInfo->ucActiveFlg)
     || (0 == pstUserCfgOPlmnInfo->usOplmnListNum))
    {
        return VOS_FALSE;
    }

    /* 卡类型不为USIM，或卡不存在时，内置OPLMN不生效 */
    ulCardStatus = NAS_MML_GetRrcSimStatus();

    if (RRC_NAS_USIM_PRESENT != ulCardStatus)
    {
        return VOS_FALSE;
    }

    /* IMSI检查未使能，不需要做IMSI检查 */
    if (VOS_FALSE == pstUserCfgOPlmnInfo->ucImsiCheckFlg)
    {
        return VOS_TRUE;
    }

    /* 取得本地维护的IMSI的地址 */
    pucImsi  = NAS_MML_GetSimImsi();

    /* 从当前的IMSI中取出home plmn */
    stHPlmn  = NAS_MML_GetImsiHomePlmn(pucImsi);

    /* 如果当前IMSI中的Home Plmn在astImsiPlmnList中，内置OPLMN功能生效 */
    for ( ulStep = 0 ; ulStep < pstUserCfgOPlmnInfo->ucImsiPlmnListNum; ulStep++ )
    {
        if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstUserCfgOPlmnInfo->astImsiPlmnList[ulStep],
                                                            &stHPlmn) )
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsNvimCustomSupplementOplmnAvail(VOS_VOID)
{
    VOS_UINT32                                              ulStep;
    NAS_MML_PLMN_ID_STRU                                    stHPlmn;
    RRC_NAS_SIM_STATUS_ENUM_UINT32                          ulCardStatus;
    VOS_UINT8                                              *pucImsi;
    NAS_MML_CUSTOM_SUPPLEMENT_OPLMN_INFO_STRU              *pstCustomSupplementOplmnInfo;

    PS_MEM_SET(&stHPlmn, 0x00, sizeof(NAS_MML_PLMN_ID_STRU));

    pstCustomSupplementOplmnInfo = NAS_MML_GetCustomSupplementOplmnInfo();

    /* OPLMN个数为0，内置OPLMN功能无效 */
    if (0 == pstCustomSupplementOplmnInfo->ucOplmnListNum)
    {
        return VOS_FALSE;
    }

    /* 卡类型不为USIM，或卡不存在时，内置OPLMN不生效 */
    ulCardStatus = NAS_MML_GetRrcSimStatus();

    if (RRC_NAS_USIM_PRESENT != ulCardStatus)
    {
        return VOS_FALSE;
    }

    /* 取得本地维护的IMSI的地址 */
    pucImsi  = NAS_MML_GetSimImsi();

    /* 从当前的IMSI中取出home plmn */
    stHPlmn  = NAS_MML_GetImsiHomePlmn(pucImsi);

    /* 如果当前IMSI中的Home Plmn在astImsiPlmnList中，内置OPLMN功能生效 */
    for ( ulStep = 0 ; ulStep < pstCustomSupplementOplmnInfo->ucImsiPlmnListNum; ulStep++ )
    {
        if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&pstCustomSupplementOplmnInfo->astImsiPlmnList[ulStep],
                                                            &stHPlmn) )
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}




VOS_VOID NAS_MML_Fill_IE_TddClassMark3(
    VOS_UINT8                          *pucTddClassMark3
)
{
    NAS_MML_MS_CAPACILITY_INFO_STRU    *pstMsCapability = VOS_NULL_PTR;
    VOS_UINT8                           ucLen;
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulRst;
    RRMM_CLASSMARK_CHANGE_IND_STRU     *pstRrmmClassMarkChgInd = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurNetRatType;
    RRC_PLMN_ID_STRU                    stRrmmPlmnId;

    enCurNetRatType        = NAS_MML_GetCurrNetRatType();

    pstRrmmClassMarkChgInd = (RRMM_CLASSMARK_CHANGE_IND_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC, sizeof(RRMM_CLASSMARK_CHANGE_IND_STRU));

    /* 当前NV打开，申请内存成功且当前模不是GSM */
    if ( (VOS_TRUE == NAS_MML_GetIsDelFddBandSwitchOnFlg())
      && (VOS_NULL_PTR != pstRrmmClassMarkChgInd)
      && (NAS_MML_NET_RAT_TYPE_GSM != enCurNetRatType) )
    {
        PS_MEM_SET(pstRrmmClassMarkChgInd, 0x00, sizeof(RRMM_CLASSMARK_CHANGE_IND_STRU));

        stRrmmPlmnId.ulMcc = NAS_MML_GetCurrCampPlmnId()->ulMcc;
        stRrmmPlmnId.ulMnc = NAS_MML_GetCurrCampPlmnId()->ulMnc;

        /* 调用GAS接口获取TDD classmark3 */
        ulRst = GASGCOM_GetMsCapability(&stRrmmPlmnId, NAS_GAS_MS_CAP_TYPE_CLASSMARK,
                                                sizeof(RRMM_CLASSMARK_CHANGE_IND_STRU),
                                                (VOS_UINT8*)pstRrmmClassMarkChgInd);

        if ( VOS_TRUE == ulRst )
        {
            pucTddClassMark3[0] = pstRrmmClassMarkChgInd->ucClassmark3TddLen;
            PS_MEM_CPY(&pucTddClassMark3[1], pstRrmmClassMarkChgInd->aucClassmark3Tdd, pucTddClassMark3[0]);

            PS_MEM_FREE(WUEPS_PID_MMC, pstRrmmClassMarkChgInd);
            return;
        }
    }
#endif

    pstMsCapability = NAS_MML_GetMsCapability();

    /* 第0位填写为长度 */
    ucLen = pstMsCapability->aucTddClassmark3[0];

    PS_MEM_CPY(&pucTddClassMark3[0], &(pstMsCapability->aucTddClassmark3[0]), ucLen+1);

#if (FEATURE_ON == FEATURE_LTE)
    if ( VOS_NULL_PTR != pstRrmmClassMarkChgInd )
    {
        PS_MEM_FREE(WUEPS_PID_MMC, pstRrmmClassMarkChgInd);
    }
#endif

    return;
}


VOS_VOID NAS_MML_Fill_IE_FddClassMark3(
    VOS_UINT8                          *pucFddClassMark3
)
{
    NAS_MML_MS_CAPACILITY_INFO_STRU    *pstMsCapability = VOS_NULL_PTR;
    VOS_UINT8                           ucLen;
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulRst;
    RRMM_CLASSMARK_CHANGE_IND_STRU     *pstRrmmClassMarkChgInd = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurNetRatType;
    RRC_PLMN_ID_STRU                    stRrmmPlmnId;

    enCurNetRatType        = NAS_MML_GetCurrNetRatType();

    pstRrmmClassMarkChgInd = (RRMM_CLASSMARK_CHANGE_IND_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC, sizeof(RRMM_CLASSMARK_CHANGE_IND_STRU));

    /* 当前NV打开，申请内存成功且当前模不是GSM */
    if ( (VOS_TRUE == NAS_MML_GetIsDelFddBandSwitchOnFlg())
      && (VOS_NULL_PTR != pstRrmmClassMarkChgInd)
      && (NAS_MML_NET_RAT_TYPE_GSM != enCurNetRatType) )
    {
        PS_MEM_SET(pstRrmmClassMarkChgInd, 0x00, sizeof(RRMM_CLASSMARK_CHANGE_IND_STRU));

        stRrmmPlmnId.ulMcc = NAS_MML_GetCurrCampPlmnId()->ulMcc;
        stRrmmPlmnId.ulMnc = NAS_MML_GetCurrCampPlmnId()->ulMnc;

        /* 调用GAS接口获取FDD classmark3 */
        ulRst = GASGCOM_GetMsCapability(&stRrmmPlmnId, NAS_GAS_MS_CAP_TYPE_CLASSMARK,
                                                sizeof(RRMM_CLASSMARK_CHANGE_IND_STRU),
                                                (VOS_UINT8*)pstRrmmClassMarkChgInd);

        if ( VOS_TRUE == ulRst )
        {
            pucFddClassMark3[0] = pstRrmmClassMarkChgInd->ucClassmark3FddLen;
            PS_MEM_CPY(&pucFddClassMark3[1], pstRrmmClassMarkChgInd->aucClassmark3Fdd, pucFddClassMark3[0]);

            PS_MEM_FREE(WUEPS_PID_MMC, pstRrmmClassMarkChgInd);
            return;
        }
    }
#endif

    pstMsCapability = NAS_MML_GetMsCapability();

    /* 第0位填写为长度 */
    ucLen = pstMsCapability->aucFddClassmark3[0];

    PS_MEM_CPY(&pucFddClassMark3[0], &(pstMsCapability->aucFddClassmark3[0]), ucLen+1);

#if(FEATURE_ON == FEATURE_LTE)
    if ( VOS_NULL_PTR != pstRrmmClassMarkChgInd )
    {
        PS_MEM_FREE(WUEPS_PID_MMC, pstRrmmClassMarkChgInd);
    }
#endif

    return;
}





VOS_VOID NAS_MML_DelEqualPlmnsInDisableRatList(
    VOS_UINT8                                              *pucEPlmnNum,
    NAS_MML_PLMN_ID_STRU                                   *pstEPlmnList,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enSpecRat,
    VOS_UINT32                                              ulDisabledRatPlmnNum,
    NAS_MML_DISABLED_PLMN_WITH_FORBIDDEN_PERIOD_STRU       *pstDisabledRatPlmnIdList
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;
    VOS_UINT32                          ulRlst;
    NAS_MML_PLMN_WITH_RAT_STRU          stPlmnWithRat;

    ucPlmnNum = *pucEPlmnNum;

    for ( i = 0; i < ucPlmnNum;  )
    {
        /* 构造带接入技术的PLMN ID */
        stPlmnWithRat.enRat         = enSpecRat;
        stPlmnWithRat.stPlmnId      = pstEPlmnList[i];

        ulRlst = NAS_MML_IsPlmnIdWithRatInDestDisabledPlmnRatList(&stPlmnWithRat,
                                                                  ulDisabledRatPlmnNum,
                                                                  pstDisabledRatPlmnIdList);

        /* 将在禁止列表且不是当前驻留网络删除 */
        if ( VOS_TRUE == ulRlst )
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pstEPlmnList [i], &pstEPlmnList[ i + 1 ],
                            ((ucPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_ID_STRU));
            }

            ucPlmnNum--;

            if ( i > 0 )
            {
                i--;
            }
            else
            {
                continue;
            }

        }

        i++;
    }

    *pucEPlmnNum = ucPlmnNum;

    return;
}


VOS_VOID NAS_MML_DelEqualPlmnNotInAllowLteRoamMccList(
    VOS_UINT8                          *pucEPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstEPlmnList,
    VOS_UINT32                          ulAllowLteRoamMccNum,
    VOS_UINT32                         *pulAllowLteRoamMccList
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;
    VOS_UINT8                           ucLteRoamFlg;


    ucPlmnNum       = *pucEPlmnNum;
    ucLteRoamFlg    = NAS_MML_GetLteRoamAllowedFlg();

    /* 允许LTE国际漫游时，不删除 */
    if (VOS_TRUE == ucLteRoamFlg)
    {
        return;
    }

    for ( i = 0; i < ucPlmnNum;  )
    {
            /* EPLMN ID为国际漫游网络,不允许漫游,需要删除 */
        if (VOS_FALSE == NAS_MML_IsMccInDestMccList(pstEPlmnList[i].ulMcc, ulAllowLteRoamMccNum, pulAllowLteRoamMccList))
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pstEPlmnList [i], &pstEPlmnList[ i + 1 ],
                            ((ucPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_ID_STRU));
            }

            ucPlmnNum--;

            if ( i > 0 )
            {
                i--;
            }
            else
            {
                continue;
            }
        }

        i++;
    }

    *pucEPlmnNum = ucPlmnNum;

    return;
}



VOS_VOID NAS_MMC_DelDuplicatedPlmnInPlmnList(
    VOS_UINT8                          *pucPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstPlmnList
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;


    ucPlmnNum       = *pucPlmnNum;

    /* 第一个PLMN肯定不会重复，默认在列表中，循环变量从1开始 */
    for ( i = 1; i < ucPlmnNum;  )
    {
        if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(&(pstPlmnList[i]),
                                               i, pstPlmnList))
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pstPlmnList [i], &pstPlmnList[ i + 1 ],
                            ((ucPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_ID_STRU));
            }

            ucPlmnNum--;

            if ( i > 0 )
            {
                i--;
            }
            else
            {
                continue;
            }
        }

        i++;
    }

    *pucPlmnNum = ucPlmnNum;

    return;
}


VOS_VOID NAS_MML_DelForbPlmnInPlmnList(
    VOS_UINT8                          *pucPlmnNum,
    NAS_MML_PLMN_ID_STRU               *pstPlmnList
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           ucPlmnNum;


    ucPlmnNum       = *pucPlmnNum;

    for ( i = 0; i < ucPlmnNum;  )
    {
        if (VOS_TRUE == NAS_MML_IsPlmnIdInForbidPlmnList(&(pstPlmnList[i])))
        {
            if (i < (ucPlmnNum - 1))
            {
                (VOS_VOID)PS_MEM_MOVE(&pstPlmnList [i], &pstPlmnList[ i + 1 ],
                            ((ucPlmnNum - i) - 1 ) * sizeof(NAS_MML_PLMN_ID_STRU));
            }

            ucPlmnNum--;

            if ( i > 0 )
            {
                i--;
            }
            else
            {
                continue;
            }
        }

        i++;
    }

    *pucPlmnNum = ucPlmnNum;

    return;
}



VOS_UINT32 NAS_MML_GetCsOnlyDataServiceSupportFlg(VOS_VOID)
{
    VOS_UINT32                                              ulIsTestCard;
    NAS_MML_MISCELLANEOUS_CFG_INFO_STRU                    *pstMiscellaneousCfgInfo = VOS_NULL_PTR;

    /* GCF测试卡允许PS业务触发注册流程 */
    ulIsTestCard    = NAS_USIMMAPI_IsTestCard();
    if (VOS_TRUE == ulIsTestCard)
    {
        return VOS_TRUE;
    }

    pstMiscellaneousCfgInfo = NAS_MML_GetMiscellaneousCfgInfo();

    if (VOS_FALSE == pstMiscellaneousCfgInfo->ucCsOnlyDataServiceSupportFlg)
    {
        return VOS_FALSE;
    }
    else
    {
        return VOS_TRUE;
    }
}


VOS_UINT32 NAS_MML_IsPlmnListSrchRslt(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    VOS_UINT32                          enPlmnSrchRslt
)
{

    switch(enRat)
    {
        /* GU时这些搜网结果是列表搜结果 */
        case NAS_MML_NET_RAT_TYPE_GSM:
        case NAS_MML_NET_RAT_TYPE_WCDMA:

            if ((RRC_PLMN_SEARCH_RLT_USER_LIST           == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_USER_LIST_REJ       == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_USER_LIST_FAIL      == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_USER_LIST_NO_RF     == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_INTER_LIST_SUCC     == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_INTER_LIST_FAIL     == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_INTER_LIST_NO_RF    == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_INTER_LIST_REJ      == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_INTER_LIST_PREF_BAND_SUCC  == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_INTER_LIST_PREF_BAND_FAIL  == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_INTER_LIST_PREF_BAND_NO_RF == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_INTER_LIST_PREF_BAND_REJ   == enPlmnSrchRslt))
            {
                return VOS_TRUE;
            }
            break;

        /* LTE时这些搜网结果是列表搜结果 */
        case NAS_MML_NET_RAT_TYPE_LTE:

            if ((MMC_LMM_PLMN_SRCH_RLT_USER_LIST_SUCC    == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_USER_LIST_REJ     == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_USER_LIST_NO_RF   == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_INTER_LIST_FULL_BAND_SUCC  == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_INTER_LIST_FULL_BAND_FAIL  == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_INTER_LIST_FULL_BAND_NO_RF == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_INTER_LIST_FULL_BAND_REJ   == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_INTER_LIST_PREF_BAND_SUCC  == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_INTER_LIST_PREF_BAND_FAIL  == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_INTER_LIST_PREF_BAND_NO_RF == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_INTER_LIST_PREF_BAND_REJ   == enPlmnSrchRslt))
            {
                return VOS_TRUE;
            }
            break;

        default:

            break;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsGetGeoPlmnSrchRslt(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    VOS_UINT32                          enPlmnSrchRslt
)
{
    switch(enRat)
    {
        /* GU下时这些搜网结果是GET_GEO结果 */
        case NAS_MML_NET_RAT_TYPE_GSM:
        case NAS_MML_NET_RAT_TYPE_WCDMA:

            if ((RRC_PLMN_SEARCH_RLT_GET_GEO_SUCC   == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_GET_GEO_FAIL   == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_GET_GEO_NO_RF  == enPlmnSrchRslt))
            {
                return VOS_TRUE;
            }
            break;

        /* LTE时这些搜网结果是GET_GEO结果 */
        case NAS_MML_NET_RAT_TYPE_LTE:

            if ((MMC_LMM_PLMN_SRCH_RLT_GET_GEO_FAIL     == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_GET_GEO_SUCC     == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_GET_GEO_NO_RF    == enPlmnSrchRslt))
            {
                return VOS_TRUE;
            }
            break;

        default:

            break;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsPrefBandPlmnSrchRslt(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    VOS_UINT32                          enPlmnSrchRslt
)
{
    switch(enRat)
    {
        /* GU下时这些搜网结果是pref band结果 */
        case NAS_MML_NET_RAT_TYPE_GSM:
        case NAS_MML_NET_RAT_TYPE_WCDMA:

            if ((RRC_PLMN_SEARCH_RLT_PREF_BAND_SUCC   == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_PREF_BAND_FAIL   == enPlmnSrchRslt)
             || (RRC_PLMN_SEARCH_RLT_PREF_BAND_NO_RF  == enPlmnSrchRslt))
            {
                return VOS_TRUE;
            }
            break;

        /* LTE时这些搜网结果是pref band结果 */
        case NAS_MML_NET_RAT_TYPE_LTE:

            if ((MMC_LMM_PLMN_SRCH_RLT_PREF_BAND_FAIL     == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_PREF_BAND_SUCC     == enPlmnSrchRslt)
             || (MMC_LMM_PLMN_SRCH_RLT_PREF_BAND_NO_RF    == enPlmnSrchRslt))
            {
                return VOS_TRUE;
            }
            break;

        default:

            break;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsCampLaiInfoChanged( VOS_VOID )
{
    NAS_MML_LAI_STRU                   *pstOldLai = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                   *pstCurLai = VOS_NULL_PTR;

    pstOldLai   = NAS_MML_GetOldCampLai();
    pstCurLai   = NAS_MML_GetCurrCampLai();

    if ((pstOldLai->stPlmnId.ulMcc != pstCurLai->stPlmnId.ulMcc)
     || (pstOldLai->stPlmnId.ulMnc != pstCurLai->stPlmnId.ulMnc)
     || (pstOldLai->aucLac[0]      != pstCurLai->aucLac[0])
     || (pstOldLai->aucLac[1]      != pstCurLai->aucLac[1]))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}





VOS_UINT8 NAS_MML_IsRoamingRejectNoRetryFlgActived(VOS_UINT8 ucCause)
 {
    NAS_MML_PLMN_ID_STRU                                   *pstCurplmn    = VOS_NULL_PTR;
    VOS_UINT32                                              ulIsRoam;
    NAS_MML_ROAMINGREJECT_NORETYR_CFG_STRU                 *pstNoRetryCfg = VOS_NULL_PTR;
    VOS_UINT32                                              i;

    i               = 0;
    pstCurplmn      = NAS_MML_GetCurrCampPlmnId();
    ulIsRoam        = NAS_MMC_IsRoam();
    pstNoRetryCfg   = NAS_MML_GetRoamingRejectNoRetryCfg();

    /*
    满足如下条件的时候收到reject 17不重试
    1. 不是测试卡；
    2. 定制nv打开,拒绝原因值在nv配置中；
    3. 漫游状态且当前PLMN不是UPLMN也不是OPLMN；
    */

    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        return VOS_FALSE;
    }

    if (VOS_FALSE == ulIsRoam)
    {
        return VOS_FALSE;
    }

    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithUplmn(pstCurplmn))
    {
        return VOS_FALSE;
    }

    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithOplmn(pstCurplmn))
    {
        return VOS_FALSE;
    }

    if (0 == pstNoRetryCfg->ucNoRetryRejectCauseNum)
    {
        return VOS_FALSE;
    }

    for (i=0; i<pstNoRetryCfg->ucNoRetryRejectCauseNum; i++)
    {
        if (ucCause == pstNoRetryCfg->aucNoRetryRejectCause[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT8 NAS_MML_IsUeSupportIms(VOS_VOID)
{
    VOS_UINT8                           ucVoiceOnIms;
    VOS_UINT8                           ucSmsOnIms;
    VOS_UINT8                           ucVideoOnIms;
    VOS_UINT8                           ucUssdOnIms;


    /* 总控开关未打开时，认为UE不支持IMS */
    if (VOS_FALSE == NAS_MML_GetLteImsSupportFlg())
    {
        return VOS_FALSE;
    }

    /* 任意一项业务能力支持时，认为UE支持IMS */
    ucVoiceOnIms    = NAS_MML_GetVoiceCallOnImsSupportFlag();
    ucSmsOnIms      = NAS_MML_GetSmsOnImsSupportFlag();
    ucVideoOnIms    = NAS_MML_GetVideoCallOnImsSupportFlag();
    ucUssdOnIms     = NAS_MML_GetUssdOnImsSupportFlag();

    if ( (VOS_TRUE == ucVoiceOnIms)
      || (VOS_TRUE == ucSmsOnIms)
      || (VOS_TRUE == ucVideoOnIms)
      || (VOS_TRUE == ucUssdOnIms) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT8 NAS_MML_GetMoCallSetupFlg(VOS_VOID)
{
    NAS_MML_MO_CALL_STATUS_ENUM_UINT8   enCallStatus;
    VOS_UINT8                           ucRslt;

    enCallStatus    = NAS_MML_GetMoCallStatus();

    switch(enCallStatus)
    {
        case NAS_MML_CSFB_MO_NORMAL_CALL_SETUP_START:
        case NAS_MML_CSFB_MO_EMC_CALL_SETUP_START:
        case NAS_MML_MO_NORMAL_CALL_SETUP_START:
        case NAS_MML_MO_EMC_CALL_SETUP_START:

            ucRslt  = VOS_TRUE;
            break;

        default:

            ucRslt  = VOS_FALSE;
            break;
    }

    return ucRslt;
}


VOS_UINT8 NAS_MML_GetMoSsSetupFlg(VOS_VOID)
{
    NAS_MML_MO_SS_STATUS_ENUM_UINT8     enSsStatus;
    VOS_UINT8                           ucRslt;

    enSsStatus      = NAS_MML_GetMoSsStatus();

    switch(enSsStatus)
    {
        case NAS_MML_CSFB_MO_SS_SETUP_START:
        case NAS_MML_MO_NORMAL_SS_SETUP_START:

            ucRslt  = VOS_TRUE;
            break;

        default:
            ucRslt  = VOS_FALSE;
            break;

    }

    return ucRslt;
}


VOS_UINT8 NAS_MML_GetMoLcsSetupFlg(VOS_VOID)
{
    NAS_MML_MO_LCS_STATUS_ENUM_UINT8    enLcsStatus;
    VOS_UINT8                           ucRslt;

    enLcsStatus     = NAS_MML_GetMoLcsStatus();

    switch(enLcsStatus)
    {
        case NAS_MML_CSFB_MO_LCS_SETUP_START:
        case NAS_MML_MO_LCS_SETUP_START:
            ucRslt  = VOS_TRUE;
            break;

        default:
            ucRslt  = VOS_FALSE;
            break;
    }

    return ucRslt;
}


VOS_UINT8 NAS_MML_GetCmServiceSetupFlg( VOS_VOID )
{

    /* MO呼叫业务标志 */
    if (VOS_TRUE == NAS_MML_GetMoCallSetupFlg())
    {
        return VOS_TRUE;
    }

    /* MO SS业务标志 */
    if (VOS_TRUE == NAS_MML_GetMoSsSetupFlg())
    {
        return VOS_TRUE;
    }

    /* MO LCS业务标志 */
    if (VOS_TRUE == NAS_MML_GetMoLcsSetupFlg())
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_Is3Gpp2Rat(
    NAS_MSCC_PIF_NET_RAT_TYPE_ENUM_UINT8            enRatType
)
{
    if ((NAS_MSCC_PIF_NET_RAT_TYPE_1X == enRatType)
      ||(NAS_MSCC_PIF_NET_RAT_TYPE_HRPD == enRatType))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_MML_IsImsiInLteOos2GPrefPlmnSelCfg(VOS_VOID)
{
    VOS_UINT32                                              i;
    NAS_MML_PLMN_ID_STRU                                    stHplmnId;
    NAS_MML_PLMN_ID_STRU                                    stPlmnId;
    NAS_MML_LTE_OOS_2G_PREF_PLMN_SEL_CFG_STRU              *pstLteOosPlmnSelCfg = VOS_NULL_PTR;
    VOS_UINT8                                              *pucImsi             = VOS_NULL_PTR;

    i                   = 0;
    pstLteOosPlmnSelCfg = NAS_MML_GetLteOos2GPrefPlmnSelCfg();

    PS_MEM_SET(&stHplmnId,0x00, sizeof(stHplmnId));
    PS_MEM_SET(&stPlmnId, 0x00, sizeof(stPlmnId));

    if ( VOS_FALSE == NAS_MML_GetSimPresentStatus() )
    {
        return VOS_FALSE;
    }

    /* 增加测试卡保护，测试卡时不修改，直接返回 */
    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        return VOS_FALSE;
    }

    /* 取得本地维护的IMSI的地址 */
    pucImsi   = NAS_MML_GetSimImsi();

    /* 从当前的IMSI中取出home plmn */
    stHplmnId = NAS_MML_GetImsiHomePlmn(pucImsi);

    /* 判断IMSI是否在LTE OOS选网优化配置列表中 */
    for (i = 0; i < pstLteOosPlmnSelCfg->ucImsiListNum; i++)
    {
        stPlmnId.ulMcc = pstLteOosPlmnSelCfg->astImsiList[i].ulMcc;
        stPlmnId.ulMnc = pstLteOosPlmnSelCfg->astImsiList[i].ulMnc;

        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stPlmnId, &stHplmnId))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsNeedSortRatPrioForLteOos2GPrefPlmnSel(
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList
)
{
    if ( (VOS_TRUE                      == NAS_MML_IsImsiInLteOos2GPrefPlmnSelCfg())
      && (NAS_MML_MAX_RAT_NUM           == pstPrioRatList->ucRatNum)
      && (NAS_MML_NET_RAT_TYPE_LTE      == pstPrioRatList->aucRatPrio[0])
      && (NAS_MML_NET_RAT_TYPE_WCDMA    == pstPrioRatList->aucRatPrio[1])
      && (NAS_MML_NET_RAT_TYPE_GSM      == pstPrioRatList->aucRatPrio[2]))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsLastCampedPlmnIdRoam(VOS_VOID)
{
    NAS_MML_PLMN_ID_STRU               *pstLastCampedPlmn = VOS_NULL_PTR;

    pstLastCampedPlmn = NAS_MMC_GetLastCampedPlmnid();

    /* 与HPLMN相同 */
    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithHplmn(pstLastCampedPlmn))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}
#endif


VOS_UINT32 NAS_MML_IsGULModeConfigured(VOS_VOID)
{
    /* syscfg set not support HRPD and 1X, we consider it's GUL mode */
    VOS_UINT32                          ulSupport1x;
    VOS_UINT32                          ulSupportHrpd;

    ulSupport1x   = NAS_MML_IsSpec3Gpp2RatInRatList(NAS_MML_3GPP2_RAT_TYPE_CDMA1X, NAS_MML_GetMs3Gpp2PrioRatList());
    ulSupportHrpd = NAS_MML_IsSpec3Gpp2RatInRatList(NAS_MML_3GPP2_RAT_TYPE_HRPD, NAS_MML_GetMs3Gpp2PrioRatList());

    if ((VOS_FALSE == ulSupportHrpd)
     && (VOS_FALSE == ulSupport1x))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsGULModeConfigured(): return TRUE");
        return VOS_TRUE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsGULModeConfigured(): return FALSE");
    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsCLModeSupportedInRatList(
    NAS_MSCC_PIF_RAT_PRIO_STRU         *pstRatPrioList
)
{
    VOS_UINT32                          i;

    for ( i = 0 ; i < (VOS_UINT32)pstRatPrioList->ucRatNum; i++ )
    {
        if ( (NAS_MSCC_PIF_NET_RAT_TYPE_1X == pstRatPrioList->aucRatPrio[i])
          || (NAS_MSCC_PIF_NET_RAT_TYPE_HRPD == pstRatPrioList->aucRatPrio[i]) )
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MML_GetNewRatFromRatList(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstOriRatList,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstNewRatList,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstOutputRatList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulNum;
    VOS_UINT32                          ulResult;

    /* 初始化 */
    ulNum    = pstNewRatList->ucRatNum;
    ulResult = VOS_FALSE;

    PS_MEM_SET(pstOutputRatList, 0x00, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    for (i = 0; i < ulNum; i++)
    {
        /* 判断指定的接入技术是否在接入技术列表中 */
        ulResult = NAS_MML_IsSpecRatInRatList( pstNewRatList->aucRatPrio[i],
                                               pstOriRatList );

        if (VOS_FALSE == ulResult)
        {
            pstOutputRatList->aucRatPrio[pstOutputRatList->ucRatNum] = pstNewRatList->aucRatPrio[i];
           (pstOutputRatList->ucRatNum)++;
        }
    }
}


VOS_UINT8 NAS_MML_GetCsfbMoCallSetupFlg(VOS_VOID)
{
    NAS_MML_MO_CALL_STATUS_ENUM_UINT8   enCallStatus;
    VOS_UINT8                           ucRslt;

    enCallStatus    = NAS_MML_GetMoCallStatus();

    switch (enCallStatus)
    {
        case NAS_MML_CSFB_MO_NORMAL_CALL_SETUP_START:
        case NAS_MML_CSFB_MO_EMC_CALL_SETUP_START:

            ucRslt  = VOS_TRUE;
            break;

        default:

            ucRslt  = VOS_FALSE;
            break;
    }

    return ucRslt;
}


VOS_UINT8 NAS_MML_GetCsfbMoSsSetupFlg(VOS_VOID)
{
    NAS_MML_MO_SS_STATUS_ENUM_UINT8     enSsStatus;
    VOS_UINT8                           ucRslt;

    enSsStatus  = NAS_MML_GetMoSsStatus();

    switch (enSsStatus)
    {
        case NAS_MML_CSFB_MO_SS_SETUP_START:
            ucRslt  = VOS_TRUE;
            break;

        default:
            ucRslt  = VOS_FALSE;
            break;
    }

    return ucRslt;
}


VOS_UINT8 NAS_MML_GetCsfbMoLcsSetupFlg(VOS_VOID)
{
    NAS_MML_MO_LCS_STATUS_ENUM_UINT8    enLcsStatus;
    VOS_UINT8                           ucRslt;

    enLcsStatus = NAS_MML_GetMoLcsStatus();

    switch (enLcsStatus)
    {
        case NAS_MML_CSFB_MO_LCS_SETUP_START:
            ucRslt  = VOS_TRUE;
            break;

        default:
            ucRslt  = VOS_FALSE;
            break;
    }

    return ucRslt;
}


VOS_VOID NAS_MML_SetCsfbMtPagingTimerStatus(
    NAS_MML_TIMER_STATUS_ENUM_UINT8     enStatus
)
{
    NAS_MML_GetMmlCtx()->stNetworkInfo.stCsDomainInfo.stCmSrvStatus.enMtCsfbPagingTimerStatus = enStatus;

    return;
}


NAS_MML_TIMER_STATUS_ENUM_UINT8 NAS_MML_GetCsfbMtPagingTimerStatus(VOS_VOID)
{
    return (NAS_MML_GetMmlCtx()->stNetworkInfo.stCsDomainInfo.stCmSrvStatus.enMtCsfbPagingTimerStatus);
}



VOS_UINT32 NAS_MML_IsPidUnderGunas(
    VOS_UINT32                          ulPid
)
{
    if ((ulPid == WUEPS_PID_MMC)
     || (ulPid == WUEPS_PID_MM)
     || (ulPid == WUEPS_PID_GMM)
     || (ulPid == WUEPS_PID_MMA))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsNeedDiscardMsg(
    VOS_UINT32                          ulSenderPid,
    VOS_UINT32                          ulReceiverPid,
    VOS_UINT32                          ulMsgName
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulRslt;

    ulRslt = NAS_MML_IsPidUnderGunas(ulReceiverPid);

    /* 发送方和接收方都不是GUNAS PID则丢弃 */
    if ((VOS_FALSE == NAS_MML_IsPidUnderGunas(ulSenderPid))
      &&(VOS_FALSE == ulRslt))
    {
        return VOS_TRUE;
    }

    /* 丢弃在忽略列表中的消息 */
    for (i = 0; i < (sizeof(g_stNasMmlDiscardMsgTbl)/sizeof(NAS_MML_DISCARD_MSG_STRU)); i++)
    {
        if ((ulSenderPid                == g_stNasMmlDiscardMsgTbl[i].ulSenderPid)
         && (ulReceiverPid              == g_stNasMmlDiscardMsgTbl[i].ulReceiverPid)
         && (ulMsgName                  == g_stNasMmlDiscardMsgTbl[i].ulMsgName))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_IsCurrGURatMode(VOS_VOID)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enCurrRat;

    enCurrRat = NAS_MML_GetCurrNetRatType();

    if ((NAS_MML_NET_RAT_TYPE_WCDMA == enCurrRat)
     || (NAS_MML_NET_RAT_TYPE_GSM == enCurrRat))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MML_IsNeedTriggerPlmnSearchWhenRemoveCustomizedFrobLa(VOS_VOID)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCampPlmnInfo            = VOS_NULL_PTR;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo          = VOS_NULL_PTR;
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriod   = VOS_NULL_PTR;
    VOS_UINT32                                              ulRlstLaiInForbidRegLaiList;
    VOS_UINT32                                              i;

    pstForbLaWithValidPeriod            = NAS_MML_GetForbLaWithValidPeriodList();

    /* 获取当前驻留PLMN信息 */
    pstCampPlmnInfo                     = NAS_MML_GetCurrCampPlmnInfo();

    /* 获取FPLMN信息*/
    pstForbidPlmnInfo                   = NAS_MML_GetForbidPlmnInfo();

    ulRlstLaiInForbidRegLaiList         = NAS_MML_IsLaiInDestLaiList(&(pstCampPlmnInfo->stLai),
        pstForbidPlmnInfo->ucForbRegLaNum, pstForbidPlmnInfo->astForbRegLaList);

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsNeedTriggerPlmnSearchWhenRemoveCustomizedFrobLa");

    /* 驻留PLMN在禁止FPLMN列表中 */
    if (VOS_TRUE == NAS_MML_IsPlmnIdInForbidPlmnList(&(pstCampPlmnInfo->stLai.stPlmnId)))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsNeedTriggerPlmnSearchWhenRemoveCustomizedFrobLa: Cur PLMN in FPLMN, No need search");
        return VOS_FALSE;
    }

    /* 驻留的LAI在禁止LAI列表中 */
    if ((VOS_TRUE == NAS_MML_IsLaiInDestLaiList(&(pstCampPlmnInfo->stLai),
            pstForbidPlmnInfo->ucForbRoamLaNum, pstForbidPlmnInfo->astForbRomLaList)) ||
        (VOS_TRUE == ulRlstLaiInForbidRegLaiList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsNeedTriggerPlmnSearchWhenRemoveCustomizedFrobLa: Cur LA in Forb LA, No need search");
        return VOS_FALSE;
    }

    /* 判断超时的LAI是否是当前驻留的LAI */
    for (i = 0; i < pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum; i++)
    {
        if (0 == NAS_MMC_GetCustomizedForbLaLeftTimeLen(&(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i])))
        {
            if(VOS_TRUE == NAS_MML_CompareLai(&(pstCampPlmnInfo->stLai),
                &(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].stForbLaWithValidPeriodLai)))
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsNeedTriggerPlmnSearchWhenRemoveCustomizedFrobLa: Cur LA will remove from Customized Forb LA, need search");
                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
 }


VOS_VOID NAS_MML_StartCustomizedForbLaTimer(VOS_VOID)
{
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriod = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulLeftTimeLen;
    VOS_UINT32                                              ulTmpTimeLen;

    pstForbLaWithValidPeriod            = NAS_MML_GetForbLaWithValidPeriodList();
    ulLeftTimeLen                       = NAS_MML_MAX_TIME_TICK;

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_StartCustomizedForbLaTimer");

    /* 禁止LA列表为空，不处理 */
    if (0 == pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_StartCustomizedForbLaTimer: Customized Forb LA is empty");
        return;
    }

    (VOS_VOID)NAS_MMC_StopTimer(TI_NAS_MMC_CUSTOMIZED_FORB_LA_TIMER);

    /* 获取当前列表中最短的剩余时长 */
    for ( i = 0; i < pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum; i++ )
    {
        ulTmpTimeLen = NAS_MMC_GetCustomizedForbLaLeftTimeLen(&(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i]));

        if (0 == ulTmpTimeLen)
        {
            continue;
        }

        ulLeftTimeLen = NAS_MML_MIN(ulLeftTimeLen, ulTmpTimeLen);
    }

    NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MML_StartCustomizedForbLaTimer: Get the Customized Forb LA num and next expire timer len: ",
        pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum, ulLeftTimeLen);

    if (NAS_MML_MAX_TIME_TICK != ulLeftTimeLen)
    {
        /* 启动定时器 */
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_StartCustomizedForbLaTimer: Start FORB LA timer");
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_CUSTOMIZED_FORB_LA_TIMER,
                            (((ulLeftTimeLen)*NAS_MML_ONE_THOUSAND_MILLISECOND)/NAS_MML_SECOND_TO_TICK));
    }

    return;
}


VOS_UINT16 NAS_MML_GetForbLaWithValidPeriodTimerLen(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enRegFailCause,
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_CFG_INFO_LIST_STRU   *pstForbLaWithValidPeriodCfg
)
{
    VOS_UINT32                                              i;



    for (i = 0; i < pstForbLaWithValidPeriodCfg->ulSupportForbLaWithValidPeriodNum; i++)
    {
        if (pstForbLaWithValidPeriodCfg->astForbLaWithValidPeriodCfgList[i].usRegFailCause == enRegFailCause)
        {
            NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MML_GetForbLaWithValidPeriodTimerLen: ", pstForbLaWithValidPeriodCfg->astForbLaWithValidPeriodCfgList[i].usForbLaValidPeriodLen);

            return pstForbLaWithValidPeriodCfg->astForbLaWithValidPeriodCfgList[i].usForbLaValidPeriodLen;
        }
    }

    /* 没有找到对应匹配时长，设置为0 */
    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetForbLaWithValidPeriodTimerLen: 0");

    return 0;
}


VOS_UINT16 NAS_MML_GetCustomizedForbLaPunishTimeLen(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enRegFailCause,
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_CFG_INFO_LIST_STRU   *pstForbLaWithValidPeriodCfg
)
{
    VOS_UINT32                                              i;



    for (i = 0; i < pstForbLaWithValidPeriodCfg->ulSupportForbLaWithValidPeriodNum; i++)
    {
        if (pstForbLaWithValidPeriodCfg->astForbLaWithValidPeriodCfgList[i].usRegFailCause == enRegFailCause)
        {
            NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MML_GetCustomizedForbLaPunishTimeLen: ", pstForbLaWithValidPeriodCfg->astForbLaWithValidPeriodCfgList[i].usPunishTimeLen);

            return pstForbLaWithValidPeriodCfg->astForbLaWithValidPeriodCfgList[i].usPunishTimeLen;
        }
    }

    /* 没有找到对应匹配时长，设置为0 */
    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetCustomizedForbLaPunishTimeLen: 0");
    return 0;
}


VOS_UINT32 NAS_MML_IsCustomizedForbLaTimerExpired(
    VOS_UINT32                                              ulDuration,
    VOS_UINT32                                              ulPeriod
)
{
    if ((ulDuration >= ulPeriod)
     || ((ulDuration + NAS_MML_CUSTOMIZED_FORB_LA_TIMER_PRECISION) >= ulPeriod))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_GetCustomizedForbLaLeftTimeLen(
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_STRU                 *pstLaWithValidPeriod
)
{
    VOS_UINT32                                              ulCurrTimeTick;
    VOS_UINT32                                              ulDuration;
    VOS_UINT32                                              ulPunishDuration;
    VOS_UINT32                                              ulLeftTimeLen;
    VOS_UINT32                                              ulPunishLeftTimeLen;

    ulCurrTimeTick = VOS_GetTick();

    NAS_NORMAL_LOG3(WUEPS_PID_MMC,
        "NAS_MMC_GetCustomizedForbLaLeftTimeLen: Current Tick and Forb La BeginTick, Period",
        ulCurrTimeTick,
        pstLaWithValidPeriod->ulForbLaWithValidPeriodBeginTick,
        pstLaWithValidPeriod->ulForbLaWithValidPeriodTick
        );

    /* 如果LAI 是nv配置的禁止期限为全F，则返回VOS_FALSE无需动态删除 */
    if (NAS_MML_MAX_TIME_TICK == pstLaWithValidPeriod->ulForbLaWithValidPeriodTick)
    {
        return NAS_MML_MAX_TIME_TICK;
    }

    ulDuration = ulCurrTimeTick - pstLaWithValidPeriod->ulForbLaWithValidPeriodBeginTick;

    ulPunishDuration = ulCurrTimeTick - pstLaWithValidPeriod->ulForbLaWithValidPeriodPunishBeginTick;

    /* 定时器精度为1秒，离到期1秒内都算超时 */
    if (VOS_TRUE == NAS_MML_IsCustomizedForbLaTimerExpired(ulDuration, pstLaWithValidPeriod->ulForbLaWithValidPeriodTick))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_GetCustomizedForbLaLeftTimeLen: Customized Forb LA timer expired");
        ulLeftTimeLen = 0;
    }
    else
    {
        /* 计算当前剩余禁止有效时长 */
        ulLeftTimeLen = pstLaWithValidPeriod->ulForbLaWithValidPeriodTick - ulDuration;
    }

    /* 防乒乓惩罚时间已过 */
    if (VOS_TRUE == NAS_MML_IsCustomizedForbLaTimerExpired(ulPunishDuration,
        pstLaWithValidPeriod->ulForbLaWithValidPeriodPunishDurationTick))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_GetCustomizedForbLaLeftTimeLen: Customized Forb LA punish timer expired");
        ulPunishLeftTimeLen = 0;
    }
    else
    {
        /* 计算当前惩罚剩余时长 */
        ulPunishLeftTimeLen =  pstLaWithValidPeriod->ulForbLaWithValidPeriodPunishDurationTick - ulPunishDuration;
    }

    /* 取两者剩余时长中的最小值为返回值 */
    return NAS_MML_MIN(ulLeftTimeLen, ulPunishLeftTimeLen);

}


VOS_UINT32 NAS_MML_CompareLac(
    NAS_MML_LAI_STRU                                       *pstCurLai,
    NAS_MML_LAI_STRU                                       *pstOldLai
)
{
    VOS_UINT32                         i;

    for ( i = 0 ; i < NAS_MML_MAX_LAC_LEN ; i++ )
    {
        if (pstCurLai->aucLac[i] != pstOldLai->aucLac[i])
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


VOS_VOID NAS_MML_DeleteLaFromForbLaWithValidPeriodList(
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriod,
    VOS_UINT32                                              ulIndex
)
{

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_DeleteLaFromForbLaWithValidPeriodList");

    /* 删除指定的LAI */
    if (ulIndex < (pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum - 1))
    {
        (VOS_VOID)PS_MEM_MOVE(&(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[ulIndex]),
                     &(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[ulIndex + 1]),
                     ((pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum- ulIndex) - 1) * sizeof(NAS_MML_FORB_LA_WITH_VALID_PERIOD_STRU));
    }

    /* 删除LA后, 更新个数 */
    pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum--;

    /* 可维可测打印 */
    NAS_MMC_LogForbLaWithValidPeriodListInfo();

}


VOS_UINT32 NAS_MML_IsNeedProtectionBeforeDeleteCustomizedForbLa(
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriod,
    VOS_UINT32                                              ulIndex
)
{
    NAS_MML_LAI_STRU                                       *pstCurrCampLai      = VOS_NULL_PTR;
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_STRU                 *pstCustomizedForbLa = VOS_NULL_PTR;
    VOS_UINT32                                              ulMatchEplmnFlg;
    VOS_UINT32                                              ulPunishTimeDuration;
    VOS_UINT32                                              ulCsServiceFlg;

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsNeedProtectionBeforeDeleteCustomizedForbLa");

    pstCurrCampLai                      = NAS_MML_GetCurrCampLai();
    pstCustomizedForbLa                 = &(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[ulIndex]);
    ulMatchEplmnFlg                     = NAS_MML_IsPlmnInEplmnList(&(pstCustomizedForbLa->stForbLaWithValidPeriodLai.stPlmnId));
    ulPunishTimeDuration                = VOS_GetTick() - pstCustomizedForbLa->ulForbLaWithValidPeriodPunishBeginTick;
    ulCsServiceFlg                      = NAS_MMC_IsCsNormalService();

    NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MML_IsNeedProtectionBeforeDeleteCustomizedForbLa: ulCsServiceFlg and ulMatchEplmnFlg",
        ulCsServiceFlg, ulMatchEplmnFlg);

    /* 防兵乓机制，惩罚时长精度为1秒 */
    if ( (VOS_TRUE == ulCsServiceFlg)
      && (VOS_TRUE == ulMatchEplmnFlg)
      && ((ulPunishTimeDuration + NAS_MML_CUSTOMIZED_FORB_LA_TIMER_PRECISION) < pstCustomizedForbLa->ulForbLaWithValidPeriodPunishDurationTick) )
    {
        /* 比较当前驻留PLMN与要删除的PLMN是否相同 */
        if (VOS_FALSE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstCurrCampLai->stPlmnId),
                            &(pstCustomizedForbLa->stForbLaWithValidPeriodLai.stPlmnId)))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsNeedProtectionBeforeDeleteCustomizedForbLa(): In EPLMN list, Start Protection");
            return VOS_TRUE;
        }

        /* 比较当前驻留的LAC与要删除的LAC是否相同 */
        if (VOS_FALSE == NAS_MML_CompareLac(pstCurrCampLai,
                            &(pstCustomizedForbLa->stForbLaWithValidPeriodLai)))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsNeedProtectionBeforeDeleteCustomizedForbLa(): Same PLMN, Diff LA, Start Protection");
            return VOS_TRUE;
        }

        return VOS_FALSE;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MML_DeleteExpiredLaFromForbLaWithValidPeriodList(
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriod
)
{
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_CFG_INFO_LIST_STRU   *pstForbLaWithValidPeriodCfg = VOS_NULL_PTR;
    VOS_UINT16                                              usTimeLen;
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulDeleteFlg;

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_DeleteExpiredLaFromForbLaWithValidPeriodList");

    ulDeleteFlg                         = VOS_FALSE;
    pstForbLaWithValidPeriodCfg         = NAS_MML_GetForbLaWithValidPeriodCfg();

    /* 判断是列表中哪一个LA超时 */
    for (i = 0; i < pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum; i++)
    {
        usTimeLen = NAS_MML_GetForbLaWithValidPeriodTimerLen(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].enCsRegFailCause,
                                                pstForbLaWithValidPeriodCfg);
        if (0 == NAS_MMC_GetCustomizedForbLaLeftTimeLen(&(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i])))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_DeleteExpiredLaFromForbLaWithValidPeriodList: Got expired LA");

            /* 防兵乓机制是否需要启动 */
            if (VOS_TRUE == NAS_MML_IsNeedProtectionBeforeDeleteCustomizedForbLa(pstForbLaWithValidPeriod, i))
            {
                /* 更新禁止LA信息 */
                pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].ulForbLaWithValidPeriodTick                    = NAS_MML_SECOND_TO_TICK * usTimeLen;
                pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].ulForbLaWithValidPeriodBeginTick               = VOS_GetTick();
                NAS_NORMAL_LOG4(WUEPS_PID_MMC,
                    "NAS_MML_DeleteExpiredLaFromForbLaWithValidPeriodList: Ping-Pang protection started, update Forb La info(BeginTick, Period, PunishBeginTick)",
                    i,
                    pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].ulForbLaWithValidPeriodBeginTick,
                    pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].ulForbLaWithValidPeriodTick,
                    pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].ulForbLaWithValidPeriodPunishBeginTick
                    );

                continue;
            }

            /* 删除指定的LAI */
            NAS_MML_DeleteLaFromForbLaWithValidPeriodList(pstForbLaWithValidPeriod, i);

            ulDeleteFlg = VOS_TRUE;
        }
    }

    return ulDeleteFlg;
}


VOS_UINT32 NAS_MML_GetLaiIndexInForbLaWithValidPeriodList(
    NAS_MML_LAI_STRU                                       *pstCurrCampLai,
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriod
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulIndex;

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_GetLaiIndexInForbLaWithValidPeriodList");

    ulIndex = NAS_MML_MIN(pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum, NAS_MML_CUSTOMIZE_FORB_LA_MAX_NUM);

    for ( i = 0; i < ulIndex; i++ )
    {
        /* 比较LAI */
        if(VOS_TRUE == NAS_MML_CompareLai(pstCurrCampLai,
            &(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[i].stForbLaWithValidPeriodLai)))
        {
            return i;
        }
    }

    return NAS_MML_CUSTOMIZE_FORB_LA_MAX_NUM;
}


VOS_UINT32 NAS_MML_AddCsRegFailLaIntoForbLaListWithValidPeriod(
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_CFG_INFO_STRU        *pstForbLaWithValidPeriodCfgInfo
)
{
    NAS_MML_LAI_STRU                                       *pstCurrCampLai              = VOS_NULL_PTR;
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_LIST_STRU            *pstForbLaWithValidPeriod    = VOS_NULL_PTR;
    NAS_MML_FORB_LA_WITH_VALID_PERIOD_STRU                 *pstCustomizedForbLa         = VOS_NULL_PTR;
    VOS_UINT32                                              ulIndex;

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_AddCsRegFailLaIntoForbLaListWithValidPeriod");

    pstCurrCampLai                      = NAS_MML_GetCurrCampLai();
    pstForbLaWithValidPeriod            = NAS_MML_GetForbLaWithValidPeriodList();

    /* 在禁止LA列表中寻找当前LAI匹配 */
    ulIndex                             = NAS_MML_GetLaiIndexInForbLaWithValidPeriodList(pstCurrCampLai, pstForbLaWithValidPeriod);

    /* 如果pstForbLai已经在禁止LA 列表中则无需添加 */
    if (NAS_MML_CUSTOMIZE_FORB_LA_MAX_NUM != ulIndex)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MML_AddCsRegFailLaIntoForbLaListWithValidPeriod: LAI already added in Forb LA With Valid Period List");
        return VOS_FALSE;
    }

    /* 列表已满,放弃加入禁止LA列表，丢弃此LAI */
    if (NAS_MML_CUSTOMIZE_FORB_LA_MAX_NUM <= pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MML_AddCsRegFailLaIntoForbLaListWithValidPeriod: Could not add  LAI into Forb LA With Valid Period List because it's full");
        return VOS_FALSE;
    }

    /* 当前LAI不在禁止LA列表里面，且当前列表未满，则需要将LAI加入到禁止LA列表里面 */
    ulIndex             = pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum;
    pstCustomizedForbLa = &(pstForbLaWithValidPeriod->astForbLaWithValidPeriodList[ulIndex]);

    pstCustomizedForbLa->stForbLaWithValidPeriodLai.stPlmnId.ulMcc               = pstCurrCampLai->stPlmnId.ulMcc;
    pstCustomizedForbLa->stForbLaWithValidPeriodLai.stPlmnId.ulMnc               = pstCurrCampLai->stPlmnId.ulMnc;
    pstCustomizedForbLa->stForbLaWithValidPeriodLai.aucLac[0]                    = pstCurrCampLai->aucLac[0];
    pstCustomizedForbLa->stForbLaWithValidPeriodLai.aucLac[1]                    = pstCurrCampLai->aucLac[1];
    pstCustomizedForbLa->stForbLaWithValidPeriodLai.enCampPlmnNetRat             = pstCurrCampLai->enCampPlmnNetRat;
    pstCustomizedForbLa->stForbLaWithValidPeriodLai.ucRac                        = pstCurrCampLai->ucRac;

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MML_AddCsRegFailLaIntoForbLaListWithValidPeriod: Current Tick", VOS_GetTick());

    /* 更新禁止LA有效时长 */
    pstCustomizedForbLa->ulForbLaWithValidPeriodTick                             = NAS_MML_SECOND_TO_TICK * (pstForbLaWithValidPeriodCfgInfo->usForbLaValidPeriodLen);
    pstCustomizedForbLa->ulForbLaWithValidPeriodBeginTick                        = VOS_GetTick();

    /* 更新防乒乓机制惩罚时长 */
    pstCustomizedForbLa->ulForbLaWithValidPeriodPunishBeginTick                  = VOS_GetTick();
    pstCustomizedForbLa->ulForbLaWithValidPeriodPunishDurationTick               = NAS_MML_SECOND_TO_TICK * (pstForbLaWithValidPeriodCfgInfo->usPunishTimeLen);
    pstCustomizedForbLa->enCsRegFailCause                                        = pstForbLaWithValidPeriodCfgInfo->usRegFailCause;
    pstForbLaWithValidPeriod->ulForbLaWithValidPeriodNum++;

    /* 可维可测打印 */
    NAS_MMC_LogForbLaWithValidPeriodListInfo();

    return VOS_TRUE;
}


VOS_UINT8 NAS_MML_GetCsfbMoSetupFlg(VOS_VOID)
{
    VOS_UINT8                           ucCsfbMoCallFlag;
    VOS_UINT8                           ucCsfbMoSsFlag;
    VOS_UINT32                          ulCsfbMoLcsFlag;

    ucCsfbMoCallFlag    = NAS_MML_GetCsfbMoCallSetupFlg();
    ucCsfbMoSsFlag      = NAS_MML_GetCsfbMoSsSetupFlg();
    ulCsfbMoLcsFlag     = NAS_MML_GetCsfbMoLcsSetupFlg();

    if ( (VOS_TRUE == ucCsfbMoCallFlag)
      || (VOS_TRUE == ucCsfbMoSsFlag)
      || (VOS_TRUE == ulCsfbMoLcsFlag) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT8 NAS_MML_IsSavePowerMode(VOS_VOID)
{
#if (FEATURE_OFF == FEATURE_MULTI_MODEM)
    /* sensor hub feature need multi modem ON */
    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsSavePowerMode: FEATURE_MULTI_MODEM is OFF");
    return VOS_FALSE;
#else 

    VOS_UINT32                          ulHour;
    VOS_INT32                           lResult;
    MTA_MTC_SENSOR_STAT_ENUM_UINT8      enSensorStat;
    MTA_MTC_SCREEN_STAT_ENUM_UINT8      enScreenStat;

    ulHour  = 0;
    lResult = VOS_ERROR;

    enSensorStat    = MTC_GetSensorStat();
    enScreenStat    = MTC_GetScreenStat();

    /*判断sensor hub状态和屏幕状态，如果不是静止或者灭屏，退出低功耗模式*/
    if ( (enSensorStat != MTA_MTC_SENSOR_STAT_STILL)
       ||(enScreenStat != MTA_MTC_SCREEN_STAT_OFF) )
    {
        NAS_NORMAL_LOG2(WUEPS_PID_MMC, "NAS_MML_IsSavePowerMode: Leave Save Power Mode.", enSensorStat, enScreenStat);
        return VOS_FALSE;
    }

    /*判断在适用区域和适用时间段开启低功耗模式*/
    if  (VOS_TRUE == NAS_MML_IsSavePowerArea())
    {
        lResult = MTC_GetCurrentHour(&ulHour);
        if ( (VOS_OK == lResult)
         &&( (VOS_TRUE == NAS_MML_IsSavePowerPeriod(ulHour))))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsSavePowerMode: Enter Save Power Mode.");
            return VOS_TRUE;
        }
    }
    else
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MML_IsSavePowerMode: Leave Save Power Mode.");
    }

    return VOS_FALSE;

#endif 
}


VOS_UINT8 NAS_MML_IsSavePowerArea(VOS_VOID)
{
    VOS_UINT8                           ucApplyArea;
    NAS_MML_PLMN_ID_STRU               *pstCurrPlmn        = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU               *pstLastPlmn        = VOS_NULL_PTR;

    pstCurrPlmn     = NAS_MML_GetCurrCampPlmnId();
    pstLastPlmn     = NAS_MMC_GetLastCampedPlmnid();
    ucApplyArea     = NAS_MML_GetAiModemApplyArea();

    NAS_NORMAL_LOG3(WUEPS_PID_MMC, "NAS_MML_IsSavePowerArea: ",
        pstCurrPlmn->ulMcc, pstLastPlmn->ulMcc, ucApplyArea);

    /*如果NV配置0，仅在中国大陆生效，NV配置非0，则全球适用*/
    if(NAS_MML_AI_SAVE_POWER_AREA_CHINA_MAINLAND == ucApplyArea)
    {
        if  ( (NAS_MML_CHINA_MAINLAND_MCC_HEX == pstCurrPlmn->ulMcc)
          ||( (NAS_MML_INVALID_MCC == pstCurrPlmn->ulMcc)
            &&(NAS_MML_CHINA_MAINLAND_MCC_HEX== pstLastPlmn->ulMcc)))
        {
            return VOS_TRUE;
        }
        else
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


VOS_UINT8 NAS_MML_IsSavePowerPeriod(VOS_UINT32 ulHour)
{
    VOS_UINT8 ucTimeStart;
    VOS_UINT8 ucTimeEnd;


    ucTimeStart = NAS_MML_GetAiModemTimeStart();
    ucTimeEnd   = NAS_MML_GetAiModemTimeEnd();

    NAS_NORMAL_LOG3(WUEPS_PID_MMC, "NAS_MML_IsSavePowerPeriod: ",
        ucTimeStart, ucTimeEnd, ulHour);

    /*如果起始时间<=结束时间，判断当前时间在区间[起始时间，结束时间)*/
    if(ucTimeStart <= ucTimeEnd)
    {
        if(ulHour >= ucTimeStart && ulHour < ucTimeEnd)
        {
            return VOS_TRUE;
        }
        else
        {
            return VOS_FALSE;
        }
    }
    /*如果起始时间>结束时间，判断当前时间在区间[起始时间，24)或[0，结束时间)*/
    else if(ucTimeStart > ucTimeEnd)
    {
        if(ulHour >= ucTimeStart || ulHour < ucTimeEnd)
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


/*lint -restore */

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

