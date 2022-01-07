/*
 * Test_data.cpp
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

/* Block parameters (default storage) */
P_Test_T TestModelClass::Test_P = {
  /* Mask Parameter: PIDController_I
   * Referenced by: '<S30>/Integral Gain'
   */
  -1.76905536445549,

  /* Mask Parameter: PIDController_InitialConditionF
   * Referenced by: '<S33>/Integrator'
   */
  0.0,

  /* Mask Parameter: PIDController_P
   * Referenced by: '<S38>/Proportional Gain'
   */
  -0.349022407839073,

  /* Expression: 0
   * Referenced by: '<Root>/Integrator'
   */
  0.0,

  /* Expression: 2
   * Referenced by: '<Root>/Constant'
   */
  2.0,

  /* Computed Parameter: TransferFcn_A
   * Referenced by: '<Root>/Transfer Fcn'
   */
  { -5.25, -43.88 },

  /* Computed Parameter: TransferFcn_C
   * Referenced by: '<Root>/Transfer Fcn'
   */
  { -49.0, -117.6 }
};
