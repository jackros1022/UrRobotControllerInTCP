//
// File: ert_main.cpp
//
// Code generated for Simulink model 'ForceController'.
//
// Model version                  : 1.135
// Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
// C/C++ source code generated on : Mon Apr 17 17:49:30 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: 32-bit Generic
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <stddef.h>
#include <stdio.h>                     // This ert_main.c example uses printf/fflush 
#include "ForceController.h"           // Model's header file
#include "rtwtypes.h"

static ForceControllerClass ForceController_Obj;// Instance of model class

// '<Root>/force_ee'
static real_T force_ee[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

// '<Root>/gravity_ee'
static real_T gravity_ee[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

// '<Root>/poseOffset_ee'
static real_T poseOffset_ee[6];

//
// Associating rt_OneStep with a real-time clock or interrupt service routine
// is what makes the generated code "real-time".  The function rt_OneStep is
// always associated with the base rate of the model.  Subrates are managed
// by the base rate from inside the generated code.  Enabling/disabling
// interrupts and floating point context switches are target specific.  This
// example code indicates where these should take place relative to executing
// the generated code step function.  Overrun behavior should be tailored to
// your application needs.  This example simply sets an error status in the
// real-time model and returns from rt_OneStep.
//
void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // Step the model for base rate
  ForceController_Obj.step(force_ee, gravity_ee, poseOffset_ee);

  // Get model outputs here

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

//
// The example "main" function illustrates what is required by your
// application code to initialize, execute, and terminate the generated code.
// Attaching rt_OneStep to a real-time clock is target specific.  This example
// illustrates how you do this relative to initializing the model.
//
int_T main(int_T argc, const char *argv[])
{
  // Unused arguments
  (void)(argc);
  (void)(argv);

  // Initialize model
  ForceController_Obj.initialize();

  // Attach rt_OneStep to a timer or interrupt service routine with
  //  period 0.002 seconds (the model's base sample time) here.  The
  //  call syntax for rt_OneStep is
  //
  //   rt_OneStep();

  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (((void*) 0) == (NULL)) {
    //  Perform other application tasks here
  }

  // The option 'Remove error status field in real-time model data structure'
  //  is selected, therefore the following code does not need to execute.

#if 0

  // Disable rt_OneStep() here

  // Terminate model
  ForceController_Obj.terminate();

#endif

  return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
