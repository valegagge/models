#define S_FUNCTION_NAME addOffset /* Defines and Includes */
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 2);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch reported by the Simulink engine*/
    }

    ssSetNumIWork(S, 1);

    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 1 /*DYNAMICALLY_SIZED*/);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S,1)) return;
    ssSetOutputPortWidth(S, 0, 1 /*DYNAMICALLY_SIZED*/);

    ssSetNumSampleTimes(S, 1);

    /* Take care when specifying exception free code - see sfuntmpl.doc */
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
    }
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}
static void mdlOutputs(SimStruct *S, int_T tid)
{
    int_T i;
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);
    real_T *y = ssGetOutputPortRealSignal(S,0);
    int_T width = ssGetOutputPortWidth(S,0);

    real_T thr = (real_T) *mxGetPr(ssGetSFcnParam(S,1));

    if(*uPtrs[0]>thr){
    //if(false){
        int_T *lastVal=ssGetIWork(S);
        //lastVal[0]=350;
        for (i=0; i<width; i++) {
        *y++ = lastVal[0];
        }
    }
    else{
        //read value of parameter
        real_T offset = (real_T) *mxGetPr(ssGetSFcnParam(S,0));
        for (i=0; i<width; i++) {
            *y++ = offset + (*uPtrs[i]);
        }
    }
}

#define MDL_UPDATE
/* Function: mdlUpdate ========================================================
 * Abstract:
 *      Every time through the simulation loop, update the
 *      transfer function coefficients. Here we update the oldest bank.
 */
static void mdlUpdate(SimStruct *S, int_T tid)
{
    UNUSED_ARG(tid); /* not used in single tasking mode */
    ssGetIWork(S)[0]=ssGetOutputPortRealSignal(S,0)[0];
}
static void mdlTerminate(SimStruct *S){}

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif