/*
 * Test.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Test".
 *
 * Model version              : 1.2
 * Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
 * C++ source code generated on : Fri Jan  7 21:04:01 2022
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objective: Debugging
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Test_h_
#define RTW_HEADER_Test_h_
#include <cfloat>
#include <cstring>
#include <stddef.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "Test_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
# define rtmGetContStateDisabled(rtm)  ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
# define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
# define rtmGetContStates(rtm)         ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
# define rtmSetContStates(rtm, val)    ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
# define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
# define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
# define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
# define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetIntgData
# define rtmGetIntgData(rtm)           ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
# define rtmSetIntgData(rtm, val)      ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
# define rtmGetOdeF(rtm)               ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
# define rtmSetOdeF(rtm, val)          ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
# define rtmGetOdeY(rtm)               ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
# define rtmSetOdeY(rtm, val)          ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
# define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
# define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
# define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
# define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetRTWLogInfo
# define rtmGetRTWLogInfo(rtm)         ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetZCCacheNeedsReset
# define rtmGetZCCacheNeedsReset(rtm)  ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
# define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
# define rtmGetdX(rtm)                 ((rtm)->derivs)
#endif

#ifndef rtmSetdX
# define rtmSetdX(rtm, val)            ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Integrator;                   /* '<Root>/Integrator' */
  real_T Sum;                          /* '<Root>/Sum' */
  real_T IntegralGain;                 /* '<S30>/Integral Gain' */
  real_T Integrator_i;                 /* '<S33>/Integrator' */
  real_T ProportionalGain;             /* '<S38>/Proportional Gain' */
  real_T Sum_k;                        /* '<S42>/Sum' */
  real_T TransferFcn;                  /* '<Root>/Transfer Fcn' */
} B_Test_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<Root>/Integrator' */
  real_T Integrator_CSTATE_h;          /* '<S33>/Integrator' */
  real_T TransferFcn_CSTATE[2];        /* '<Root>/Transfer Fcn' */
} X_Test_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<Root>/Integrator' */
  real_T Integrator_CSTATE_h;          /* '<S33>/Integrator' */
  real_T TransferFcn_CSTATE[2];        /* '<Root>/Transfer Fcn' */
} XDot_Test_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<Root>/Integrator' */
  boolean_T Integrator_CSTATE_h;       /* '<S33>/Integrator' */
  boolean_T TransferFcn_CSTATE[2];     /* '<Root>/Transfer Fcn' */
} XDis_Test_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* Parameters (default storage) */
struct P_Test_T_ {
  real_T PIDController_I;              /* Mask Parameter: PIDController_I
                                        * Referenced by: '<S30>/Integral Gain'
                                        */
  real_T PIDController_InitialConditionF;
                              /* Mask Parameter: PIDController_InitialConditionF
                               * Referenced by: '<S33>/Integrator'
                               */
  real_T PIDController_P;              /* Mask Parameter: PIDController_P
                                        * Referenced by: '<S38>/Proportional Gain'
                                        */
  real_T Integrator_IC;                /* Expression: 0
                                        * Referenced by: '<Root>/Integrator'
                                        */
  real_T Constant_Value;               /* Expression: 2
                                        * Referenced by: '<Root>/Constant'
                                        */
  real_T TransferFcn_A[2];             /* Computed Parameter: TransferFcn_A
                                        * Referenced by: '<Root>/Transfer Fcn'
                                        */
  real_T TransferFcn_C[2];             /* Computed Parameter: TransferFcn_C
                                        * Referenced by: '<Root>/Transfer Fcn'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_Test_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;
  X_Test_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[4];
  real_T odeF[3][4];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Class declaration for model Test */
class TestModelClass {
  /* public data and function members */
 public:
  /* model initialize function */
  void initialize();

  /* model step function */
  void step();

  /* model terminate function */
  void terminate();

  /* Constructor */
  TestModelClass();

  /* Destructor */
  ~TestModelClass();

  /* Real-Time Model get method */
  RT_MODEL_Test_T * getRTM();

  /* private data and function members */
 private:
  /* Tunable parameters */
  static P_Test_T Test_P;

  /* Block signals */
  B_Test_T Test_B;
  X_Test_T Test_X;                     /* Block continuous states */

  /* Real-Time Model */
  RT_MODEL_Test_T Test_M;

  /* Continuous states update member function*/
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  /* Derivatives member function */
  void Test_derivatives();
};

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Test'
 * '<S1>'   : 'Test/PID Controller'
 * '<S2>'   : 'Test/PID Controller/Anti-windup'
 * '<S3>'   : 'Test/PID Controller/D Gain'
 * '<S4>'   : 'Test/PID Controller/Filter'
 * '<S5>'   : 'Test/PID Controller/Filter ICs'
 * '<S6>'   : 'Test/PID Controller/I Gain'
 * '<S7>'   : 'Test/PID Controller/Ideal P Gain'
 * '<S8>'   : 'Test/PID Controller/Ideal P Gain Fdbk'
 * '<S9>'   : 'Test/PID Controller/Integrator'
 * '<S10>'  : 'Test/PID Controller/Integrator ICs'
 * '<S11>'  : 'Test/PID Controller/N Copy'
 * '<S12>'  : 'Test/PID Controller/N Gain'
 * '<S13>'  : 'Test/PID Controller/P Copy'
 * '<S14>'  : 'Test/PID Controller/Parallel P Gain'
 * '<S15>'  : 'Test/PID Controller/Reset Signal'
 * '<S16>'  : 'Test/PID Controller/Saturation'
 * '<S17>'  : 'Test/PID Controller/Saturation Fdbk'
 * '<S18>'  : 'Test/PID Controller/Sum'
 * '<S19>'  : 'Test/PID Controller/Sum Fdbk'
 * '<S20>'  : 'Test/PID Controller/Tracking Mode'
 * '<S21>'  : 'Test/PID Controller/Tracking Mode Sum'
 * '<S22>'  : 'Test/PID Controller/Tsamp - Integral'
 * '<S23>'  : 'Test/PID Controller/Tsamp - Ngain'
 * '<S24>'  : 'Test/PID Controller/postSat Signal'
 * '<S25>'  : 'Test/PID Controller/preSat Signal'
 * '<S26>'  : 'Test/PID Controller/Anti-windup/Passthrough'
 * '<S27>'  : 'Test/PID Controller/D Gain/Disabled'
 * '<S28>'  : 'Test/PID Controller/Filter/Disabled'
 * '<S29>'  : 'Test/PID Controller/Filter ICs/Disabled'
 * '<S30>'  : 'Test/PID Controller/I Gain/Internal Parameters'
 * '<S31>'  : 'Test/PID Controller/Ideal P Gain/Passthrough'
 * '<S32>'  : 'Test/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S33>'  : 'Test/PID Controller/Integrator/Continuous'
 * '<S34>'  : 'Test/PID Controller/Integrator ICs/Internal IC'
 * '<S35>'  : 'Test/PID Controller/N Copy/Disabled wSignal Specification'
 * '<S36>'  : 'Test/PID Controller/N Gain/Disabled'
 * '<S37>'  : 'Test/PID Controller/P Copy/Disabled'
 * '<S38>'  : 'Test/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S39>'  : 'Test/PID Controller/Reset Signal/Disabled'
 * '<S40>'  : 'Test/PID Controller/Saturation/Passthrough'
 * '<S41>'  : 'Test/PID Controller/Saturation Fdbk/Disabled'
 * '<S42>'  : 'Test/PID Controller/Sum/Sum_PI'
 * '<S43>'  : 'Test/PID Controller/Sum Fdbk/Disabled'
 * '<S44>'  : 'Test/PID Controller/Tracking Mode/Disabled'
 * '<S45>'  : 'Test/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S46>'  : 'Test/PID Controller/Tsamp - Integral/Passthrough'
 * '<S47>'  : 'Test/PID Controller/Tsamp - Ngain/Passthrough'
 * '<S48>'  : 'Test/PID Controller/postSat Signal/Forward_Path'
 * '<S49>'  : 'Test/PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* RTW_HEADER_Test_h_ */
