

#ifndef __CNAS_XSD_SND_XREG_H__
#define __CNAS_XSD_SND_XREG_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include  "vos.h"
#include "cas_1x_idlectrl_proc_nas_pif.h"
#include "xsd_xreg_pif.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/


/*****************************************************************************
  3 枚举定义
*****************************************************************************/


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

VOS_VOID CNAS_XSD_SndXregPowerOffReq(VOS_VOID);
VOS_VOID CNAS_XSD_SndXregStartReq(VOS_VOID);

VOS_VOID CNAS_XSD_SndXregDeregisterInd(
    XSD_XREG_DEREGISTER_REASON_ENUM_UINT8                   enCause
);

VOS_VOID CNAS_XSD_SndXregSysInfoInd(
    XSD_XREG_SERVICE_STATUS_ENUM_UINT32                     enSrvSta,
    XSD_XREG_SYS_INFO_STRU                                 *pstSysInfo
);

VOS_VOID CNAS_XSD_SndXregRedirInd(VOS_VOID);
VOS_VOID CNAS_XSD_BuildXregSysInfoInd(
    CAS_CNAS_1X_OHM_IND_STRU           *pstCasCnas1xOhmInd,
    XSD_XREG_SYS_INFO_STRU             *pstXsdXregSysInfo
);


VOS_VOID CNAS_XSD_SndXregUeStateInd(
    CAS_CNAS_1X_CAS_STATE_ENUM_UINT8                        enCasState,
    CAS_CNAS_1X_CAS_SUB_STATE_ENUM_UINT8                    enCasSubState
);

VOS_VOID CNAS_XSD_SndXregNormalServiceInd(
    VOS_UINT32                                              ulMcc,
    VOS_RATMODE_ENUM_UINT32                                 ucRatType
);

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

#endif /* end of CnasXsdSndXreg.h */
