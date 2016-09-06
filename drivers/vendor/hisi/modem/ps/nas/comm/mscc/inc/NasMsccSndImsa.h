

#ifndef __CNAS_XSD_PRE_PROC_ACT_H__
#define __CNAS_XSD_PRE_PROC_ACT_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include "PsTypeDef.h"
#include "NasCommDef.h"

#if (FEATURE_ON == FEATURE_IMS)
#include "ImsaMsccInterface.h"
#endif
#include "NasMsccCtx.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
#if (FEATURE_ON == FEATURE_IMS)
VOS_VOID NAS_MSCC_SndImsaSrvInfoNotify(
    NAS_MSCC_SERVICE_STATUS_ENUM_UINT8  enCurPsServiceSta,
    VOS_UINT8                           ucRoamingValid,
    VOS_UINT8                           ucRoamingFlg
);

VOS_VOID NAS_MSCC_SndImsaCampInfoChangeInd(VOS_VOID);

VOS_VOID NAS_MSCC_SndImsaStopReq(VOS_VOID);

VOS_VOID NAS_MSCC_SndImsaStartReq(VOS_VOID);

VOS_VOID NAS_MSCC_SndImsaRegReq(VOS_VOID);

VOS_VOID NAS_MSCC_SndImsaDeregReq(VOS_VOID);

VOS_VOID NAS_MSCC_SndImsaVoiceDomainChangeInd(
    MSCC_IMSA_VOICE_DOMAIN_ENUM_UINT32                      enVoiceDomain
);

VOS_VOID NAS_MSCC_SndImsaImsDomainCfgSetReq(
    NAS_MSCC_PIF_IMS_DOMAIN_ENUM_UINT32         enImsDomain
);
VOS_VOID NAS_MSCC_SndImsaRoamImsSupportSetReq(
    MSCC_IMSA_SET_ROAM_IMS_SUPPORT_ENUM_UINT32         enRoamImsSupport
);

VOS_VOID NAS_MSCC_SndImsaApAreaLostInd(VOS_VOID);
#endif


#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of NasMsccSndImsa.h */