

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "CnasHsdFsmSwitchOn.h"
#include "CnasHsdFsmSwitchOnTbl.h"
#include "CnasHsdComFunc.h"
#include "cas_hrpd_airlinkmgmt_nas_pif.h"
#include "hsm_hsd_pif.h"
#include "UsimPsInterface.h"
#include "hsd_hlu_pif.h"
#include "ehsm_hsd_pif.h"
#include "hsd_hrm_pif.h"

#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_CNAS_HSD_FSM_SWITCH_ON_TBL_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* CNAS HSD״̬������:������״̬������ */
NAS_FSM_DESC_STRU                       g_stCnasHsdSwitchOnFsmDesc;

/* CNAS_HSD_SWITCH_ON_STA_INIT״̬�¶����� */
NAS_ACT_STRU g_astCnasHsdSwitchOnInitActTbl[] =
{
    NAS_ACT_TBL_ITEM( UEPS_PID_MSCC,
                      ID_MSCC_HSD_START_REQ,
                      CNAS_HSD_RcvMsccStartReq_SwitchOn_Init),

    NAS_ACT_TBL_ITEM( I1_UEPS_PID_MSCC,
                      ID_MSCC_HSD_START_REQ,
                      CNAS_HSD_RcvMsccStartReq_SwitchOn_Init),
};

/* CNAS_HSD_SWITCH_ON_STA_WAIT_CARD_FILE_CNF������ */
NAS_ACT_STRU g_astCnasHsdSwitchOnWaitCardFileCnfActTbl[] =
{
    NAS_ACT_TBL_ITEM( WUEPS_PID_USIM,
                      USIMM_READFILE_CNF,
                      CNAS_HSD_RcvCardGetFileCnf_SwitchOn_WaitCardFileCnf),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_CNAS_HSD_WAIT_CARD_FILE_CNF,
                      CNAS_HSD_RcvTiCardGetFileCnfExpired_SwitchOn_WaitCardFileCnf)
};

/* CNAS_HSD_SWITCH_ON_STA_WAIT_CAS_START_CNF������ */
NAS_ACT_STRU g_astCnasHsdSwitchOnWaitCasStartCnfActTbl[] =
{
    NAS_ACT_TBL_ITEM( UEPS_PID_HALMP,
                      ID_CAS_CNAS_HRPD_START_CNF,
                      CNAS_HSD_RcvCasStartCnf_SwitchOn_WaitCasStartCnf),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_CNAS_HSD_WAIT_CAS_START_CNF,
                      CNAS_HSD_RcvTiCasStartCnfExpired_SwitchOn_WaitCasStartCnf)
};

/* CNAS_HSD_SWITCH_ON_STA_WAIT_HRM_START_CNF������ */
NAS_ACT_STRU g_astCnasHsdSwitchOnWaitHrmStartCnfActTbl[] =
{
    NAS_ACT_TBL_ITEM( UEPS_PID_HRM,
                      ID_HRM_HSD_START_CNF,
                      CNAS_HSD_RcvHrmStartCnf_SwitchOn_WaitHrmStartCnf),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_CNAS_HSD_WAIT_HRM_START_CNF,
                      CNAS_HSD_RcvTiHrmStartCnfExpired_SwitchOn_WaitHrmStartCnf)
};

/* CNAS_HSD_SWITCH_ON_STA_WAIT_HSM_START_CNF������ */
NAS_ACT_STRU g_astCnasHsdSwitchOnWaitHsmStartCnfActTbl[] =
{
    NAS_ACT_TBL_ITEM( UEPS_PID_HSM,
                      ID_HSM_HSD_START_CNF,
                      CNAS_HSD_RcvHsmStartCnf_SwitchOn_WaitHsmStartCnf),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_CNAS_HSD_WAIT_HSM_START_CNF,
                      CNAS_HSD_RcvTiHsmStartCnfExpired_SwitchOn_WaitHsmStartCnf)
};

/* CNAS_HSD_SWITCH_ON_STA_WAIT_EHSM_START_CNF������ */
NAS_ACT_STRU g_astCnasHsdSwitchOnWaitEhsmStartCnfActTbl[] =
{
    NAS_ACT_TBL_ITEM( UEPS_PID_EHSM,
                      ID_EHSM_HSD_START_CNF,
                      CNAS_HSD_RcvEhsmStartCnf_SwitchOn_WaitEhsmStartCnf),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_CNAS_HSD_WAIT_EHSM_START_CNF,
                      CNAS_HSD_RcvTiEhsmStartCnfExpired_SwitchOn_WaitEhsmStartCnf)
};

/* CNAS_HSD_SWITCH_ON_STA_WAIT_HLU_START_CNF������ */
NAS_ACT_STRU g_astCnasHsdSwitchOnWaitHluStartCnfActTbl[] =
{
    NAS_ACT_TBL_ITEM( UEPS_PID_HLU,
                      ID_HLU_HSD_START_CNF,
                      CNAS_HSD_RcvHluStartCnf_SwitchOn_WaitHluStartCnf),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_CNAS_HSD_WAIT_HLU_START_CNF,
                      CNAS_HSD_RcvTiHluStartCnfExpired_SwitchOn_WaitHluStartCnf)
};

/* ����״̬����Ϣ ״̬�� */
NAS_STA_STRU g_astCnasHsdSwitchOnStaTbl[] =
{
    /*****************���忪����ʼ״̬ת�Ʊ�**********************/
    NAS_STA_TBL_ITEM( CNAS_HSD_SWITCH_ON_STA_INIT,
                      g_astCnasHsdSwitchOnInitActTbl),

    /*****************����ȴ����ļ���Ϣ״̬ת�Ʊ�**********************/
    NAS_STA_TBL_ITEM( CNAS_HSD_SWITCH_ON_STA_WAIT_CARD_FILE_CNF,
                      g_astCnasHsdSwitchOnWaitCardFileCnfActTbl ),

    /*****************����ȴ�CAS���������Ϣ״̬ת�Ʊ�**********************/
    NAS_STA_TBL_ITEM( CNAS_HSD_SWITCH_ON_STA_WAIT_CAS_START_CNF,
                      g_astCnasHsdSwitchOnWaitCasStartCnfActTbl),

    /*****************����ȴ�HSM���������Ϣ״̬ת�Ʊ�**********************/
    NAS_STA_TBL_ITEM( CNAS_HSD_SWITCH_ON_STA_WAIT_HSM_START_CNF,
                      g_astCnasHsdSwitchOnWaitHsmStartCnfActTbl),

    /*****************����ȴ�HSM���������Ϣ״̬ת�Ʊ�**********************/
    NAS_STA_TBL_ITEM( CNAS_HSD_SWITCH_ON_STA_WAIT_EHSM_START_CNF,
                      g_astCnasHsdSwitchOnWaitEhsmStartCnfActTbl),

    /*****************����ȴ�HLU���������Ϣ״̬ת�Ʊ�**********************/
    NAS_STA_TBL_ITEM( CNAS_HSD_SWITCH_ON_STA_WAIT_HLU_START_CNF,
                      g_astCnasHsdSwitchOnWaitHluStartCnfActTbl),

    /*****************����ȴ�HLU���������Ϣ״̬ת�Ʊ�**********************/
    NAS_STA_TBL_ITEM( CNAS_HSD_SWITCH_ON_STA_WAIT_HRM_START_CNF,
                      g_astCnasHsdSwitchOnWaitHrmStartCnfActTbl),
};

/*****************************************************************************
  3 ��������
*****************************************************************************/
/*lint -save -e958*/

VOS_UINT32 CNAS_HSD_GetSwitchOnStaTblSize(VOS_VOID)
{
    return (sizeof(g_astCnasHsdSwitchOnStaTbl)/sizeof(NAS_STA_STRU));
}



NAS_FSM_DESC_STRU * CNAS_HSD_GetSwitchOnFsmDescAddr(VOS_VOID)
{
    return (&g_stCnasHsdSwitchOnFsmDesc);
}
/*lint -restore*/
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


