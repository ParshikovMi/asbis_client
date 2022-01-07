/*
 * Test.cpp
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

#include "Test.h"
#include "Test_private.h"

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
void TestModelClass::rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = static_cast<ODE3_IntgData *>(rtsiGetSolverData(si));
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) std::memcpy(y, x,
                     static_cast<uint_T>(nXc)*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Test_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  this->step();
  Test_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  this->step();
  Test_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void TestModelClass::step()
{
  if (rtmIsMajorTimeStep((&Test_M))) {
    /* set solver stop time */
    if (!((&Test_M)->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&(&Test_M)->solverInfo, (((&Test_M)
        ->Timing.clockTickH0 + 1) * (&Test_M)->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&(&Test_M)->solverInfo, (((&Test_M)
        ->Timing.clockTick0 + 1) * (&Test_M)->Timing.stepSize0 + (&Test_M)
        ->Timing.clockTickH0 * (&Test_M)->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep((&Test_M))) {
    (&Test_M)->Timing.t[0] = rtsiGetT(&(&Test_M)->solverInfo);
  }

  /* Integrator: '<Root>/Integrator' */
  Test_B.Integrator = Test_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep((&Test_M))) {
  }

  /* Sum: '<Root>/Sum' incorporates:
   *  Constant: '<Root>/Constant'
   */
  Test_B.Sum = Test_P.Constant_Value - Test_B.Integrator;

  /* Gain: '<S30>/Integral Gain' */
  Test_B.IntegralGain = Test_P.PIDController_I * Test_B.Sum;

  /* Integrator: '<S33>/Integrator' */
  Test_B.Integrator_i = Test_X.Integrator_CSTATE_h;

  /* Gain: '<S38>/Proportional Gain' */
  Test_B.ProportionalGain = Test_P.PIDController_P * Test_B.Sum;

  /* Sum: '<S42>/Sum' */
  Test_B.Sum_k = Test_B.ProportionalGain + Test_B.Integrator_i;

  /* TransferFcn: '<Root>/Transfer Fcn' */
  Test_B.TransferFcn = 0.0;
  Test_B.TransferFcn += Test_P.TransferFcn_C[0] * Test_X.TransferFcn_CSTATE[0];
  Test_B.TransferFcn += Test_P.TransferFcn_C[1] * Test_X.TransferFcn_CSTATE[1];
  if (rtmIsMajorTimeStep((&Test_M))) {
    /* Matfile logging */
    rt_UpdateTXYLogVars((&Test_M)->rtwLogInfo, ((&Test_M)->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep((&Test_M))) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal((&Test_M))!=-1) &&
          !((rtmGetTFinal((&Test_M))-((((&Test_M)->Timing.clockTick1+(&Test_M)
               ->Timing.clockTickH1* 4294967296.0)) * 0.2)) > ((((&Test_M)
              ->Timing.clockTick1+(&Test_M)->Timing.clockTickH1* 4294967296.0)) *
            0.2) * (DBL_EPSILON))) {
        rtmSetErrorStatus((&Test_M), "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&(&Test_M)->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++(&Test_M)->Timing.clockTick0)) {
      ++(&Test_M)->Timing.clockTickH0;
    }

    (&Test_M)->Timing.t[0] = rtsiGetSolverStopTime(&(&Test_M)->solverInfo);

    {
      /* Update absolute timer for sample time: [0.2s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.2, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      (&Test_M)->Timing.clockTick1++;
      if (!(&Test_M)->Timing.clockTick1) {
        (&Test_M)->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void TestModelClass::Test_derivatives()
{
  XDot_Test_T *_rtXdot;
  _rtXdot = ((XDot_Test_T *) (&Test_M)->derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  _rtXdot->Integrator_CSTATE = Test_B.TransferFcn;

  /* Derivatives for Integrator: '<S33>/Integrator' */
  _rtXdot->Integrator_CSTATE_h = Test_B.IntegralGain;

  /* Derivatives for TransferFcn: '<Root>/Transfer Fcn' */
  _rtXdot->TransferFcn_CSTATE[0] = 0.0;
  _rtXdot->TransferFcn_CSTATE[0] += Test_P.TransferFcn_A[0] *
    Test_X.TransferFcn_CSTATE[0];
  _rtXdot->TransferFcn_CSTATE[1] = 0.0;
  _rtXdot->TransferFcn_CSTATE[0] += Test_P.TransferFcn_A[1] *
    Test_X.TransferFcn_CSTATE[1];
  _rtXdot->TransferFcn_CSTATE[1] += Test_X.TransferFcn_CSTATE[0];
  _rtXdot->TransferFcn_CSTATE[0] += Test_B.Sum_k;
}

/* Model initialize function */
void TestModelClass::initialize()
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&(&Test_M)->solverInfo, &(&Test_M)->Timing.simTimeStep);
    rtsiSetTPtr(&(&Test_M)->solverInfo, &rtmGetTPtr((&Test_M)));
    rtsiSetStepSizePtr(&(&Test_M)->solverInfo, &(&Test_M)->Timing.stepSize0);
    rtsiSetdXPtr(&(&Test_M)->solverInfo, &(&Test_M)->derivs);
    rtsiSetContStatesPtr(&(&Test_M)->solverInfo, (real_T **) &(&Test_M)
                         ->contStates);
    rtsiSetNumContStatesPtr(&(&Test_M)->solverInfo, &(&Test_M)
      ->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&Test_M)->solverInfo, &(&Test_M)
      ->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&Test_M)->solverInfo, &(&Test_M)
      ->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&Test_M)->solverInfo, &(&Test_M)
      ->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&(&Test_M)->solverInfo, (&rtmGetErrorStatus((&Test_M))));
    rtsiSetRTModelPtr(&(&Test_M)->solverInfo, (&Test_M));
  }

  rtsiSetSimTimeStep(&(&Test_M)->solverInfo, MAJOR_TIME_STEP);
  (&Test_M)->intgData.y = (&Test_M)->odeY;
  (&Test_M)->intgData.f[0] = (&Test_M)->odeF[0];
  (&Test_M)->intgData.f[1] = (&Test_M)->odeF[1];
  (&Test_M)->intgData.f[2] = (&Test_M)->odeF[2];
  (&Test_M)->contStates = ((X_Test_T *) &Test_X);
  rtsiSetSolverData(&(&Test_M)->solverInfo, static_cast<void *>(&(&Test_M)
    ->intgData));
  rtsiSetSolverName(&(&Test_M)->solverInfo,"ode3");
  rtmSetTPtr((&Test_M), &(&Test_M)->Timing.tArray[0]);
  rtmSetTFinal((&Test_M), 10.0);
  (&Test_M)->Timing.stepSize0 = 0.2;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    (&Test_M)->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo((&Test_M)->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs((&Test_M)->rtwLogInfo, (NULL));
    rtliSetLogT((&Test_M)->rtwLogInfo, "tout");
    rtliSetLogX((&Test_M)->rtwLogInfo, "");
    rtliSetLogXFinal((&Test_M)->rtwLogInfo, "");
    rtliSetLogVarNameModifier((&Test_M)->rtwLogInfo, "rt_");
    rtliSetLogFormat((&Test_M)->rtwLogInfo, 4);
    rtliSetLogMaxRows((&Test_M)->rtwLogInfo, 0);
    rtliSetLogDecimation((&Test_M)->rtwLogInfo, 1);
    rtliSetLogY((&Test_M)->rtwLogInfo, "");
    rtliSetLogYSignalInfo((&Test_M)->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs((&Test_M)->rtwLogInfo, (NULL));
  }

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime((&Test_M)->rtwLogInfo, 0.0, rtmGetTFinal
    ((&Test_M)), (&Test_M)->Timing.stepSize0, (&rtmGetErrorStatus((&Test_M))));

  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  Test_X.Integrator_CSTATE = Test_P.Integrator_IC;

  /* InitializeConditions for Integrator: '<S33>/Integrator' */
  Test_X.Integrator_CSTATE_h = Test_P.PIDController_InitialConditionF;

  /* InitializeConditions for TransferFcn: '<Root>/Transfer Fcn' */
  Test_X.TransferFcn_CSTATE[0] = 0.0;
  Test_X.TransferFcn_CSTATE[1] = 0.0;
}

/* Model terminate function */
void TestModelClass::terminate()
{
  /* (no terminate code required) */
}

/* Constructor */
TestModelClass::TestModelClass():
  Test_B()
  ,Test_X()
  ,Test_M()
{
  /* Currently there is no constructor body generated.*/
}

/* Destructor */
TestModelClass::~TestModelClass()
{
  /* Currently there is no destructor body generated.*/
}

/* Real-Time Model get method */
RT_MODEL_Test_T * TestModelClass::getRTM()
{
  return (&Test_M);
}
