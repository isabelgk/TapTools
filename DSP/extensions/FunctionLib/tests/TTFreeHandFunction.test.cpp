/** @file
 *
 * @ingroup dspFunctionLib
 *
 * @brief Unit tests for the Jamoma DSP #TTFreeHandFunction of the #TTFunctionLib
 *
 * @details
 *
 * @authors Trond Lossius, Théo de la Hogue
 *
 * @copyright Copyright © 2012 by Trond Lossius @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "TTFunction.h"
#include "TTFreeHandFunction.h"


/*
 test if a new freehand function unit is like a linear function.
 */

TTErr TTFreeHandFunction::test(TTValue& returnedTestInfo)
{
	int					errorCount = 0;
	int					testAssertionCount = 0;
	int					badSampleCount = 0;
	TTAudioSignalPtr	input = NULL;
	TTAudioSignalPtr	output = NULL;
	int					N = 128;
	TTValue				v;

	TTFloat64 expectedSignalTest1[128] = {
        0.0000000000000000e+00,
        7.8740157480314960e-03,
        1.5748031496062992e-02,
        2.3622047244094488e-02,
        3.1496062992125984e-02,
        3.9370078740157480e-02,
        4.7244094488188976e-02,
        5.5118110236220472e-02,
        6.2992125984251968e-02,
        7.0866141732283464e-02,
        7.8740157480314960e-02,
        8.6614173228346455e-02,
        9.4488188976377951e-02,
        1.0236220472440945e-01,
        1.1023622047244094e-01,
        1.1811023622047244e-01,
        1.2598425196850394e-01,
        1.3385826771653545e-01,
        1.4173228346456693e-01,
        1.4960629921259844e-01,
        1.5748031496062992e-01,
        1.6535433070866143e-01,
        1.7322834645669291e-01,
        1.8110236220472442e-01,
        1.8897637795275590e-01,
        1.9685039370078741e-01,
        2.0472440944881889e-01,
        2.1259842519685040e-01,
        2.2047244094488189e-01,
        2.2834645669291340e-01,
        2.3622047244094488e-01,
        2.4409448818897639e-01,
        2.5196850393700787e-01,
        2.5984251968503935e-01,
        2.6771653543307089e-01,
        2.7559055118110237e-01,
        2.8346456692913385e-01,
        2.9133858267716534e-01,
        2.9921259842519687e-01,
        3.0708661417322836e-01,
        3.1496062992125984e-01,
        3.2283464566929132e-01,
        3.3070866141732286e-01,
        3.3858267716535434e-01,
        3.4645669291338582e-01,
        3.5433070866141730e-01,
        3.6220472440944884e-01,
        3.7007874015748032e-01,
        3.7795275590551181e-01,
        3.8582677165354329e-01,
        3.9370078740157483e-01,
        4.0157480314960631e-01,
        4.0944881889763779e-01,
        4.1732283464566927e-01,
        4.2519685039370081e-01,
        4.3307086614173229e-01,
        4.4094488188976377e-01,
        4.4881889763779526e-01,
        4.5669291338582679e-01,
        4.6456692913385828e-01,
        4.7244094488188976e-01,
        4.8031496062992124e-01,
        4.8818897637795278e-01,
        4.9606299212598426e-01,
        5.0393700787401574e-01,
        5.1181102362204722e-01,
        5.1968503937007871e-01,
        5.2755905511811019e-01,
        5.3543307086614178e-01,
        5.4330708661417326e-01,
        5.5118110236220474e-01,
        5.5905511811023623e-01,
        5.6692913385826771e-01,
        5.7480314960629919e-01,
        5.8267716535433067e-01,
        5.9055118110236215e-01,
        5.9842519685039375e-01,
        6.0629921259842523e-01,
        6.1417322834645671e-01,
        6.2204724409448819e-01,
        6.2992125984251968e-01,
        6.3779527559055116e-01,
        6.4566929133858264e-01,
        6.5354330708661412e-01,
        6.6141732283464572e-01,
        6.6929133858267720e-01,
        6.7716535433070868e-01,
        6.8503937007874016e-01,
        6.9291338582677164e-01,
        7.0078740157480313e-01,
        7.0866141732283461e-01,
        7.1653543307086609e-01,
        7.2440944881889768e-01,
        7.3228346456692917e-01,
        7.4015748031496065e-01,
        7.4803149606299213e-01,
        7.5590551181102361e-01,
        7.6377952755905509e-01,
        7.7165354330708658e-01,
        7.7952755905511806e-01,
        7.8740157480314965e-01,
        7.9527559055118113e-01, 
        8.0314960629921262e-01, 
        8.1102362204724410e-01, 
        8.1889763779527558e-01, 
        8.2677165354330706e-01, 
        8.3464566929133854e-01, 
        8.4251968503937003e-01, 
        8.5039370078740162e-01, 
        8.5826771653543310e-01, 
        8.6614173228346458e-01, 
        8.7401574803149606e-01, 
        8.8188976377952755e-01, 
        8.8976377952755903e-01, 
        8.9763779527559051e-01, 
        9.0551181102362199e-01, 
        9.1338582677165359e-01, 
        9.2125984251968507e-01, 
        9.2913385826771655e-01, 
        9.3700787401574803e-01, 
        9.4488188976377951e-01, 
        9.5275590551181100e-01, 
        9.6062992125984248e-01, 
        9.6850393700787396e-01, 
        9.7637795275590555e-01, 
        9.8425196850393704e-01, 
        9.9212598425196852e-01, 
        1.0000000000000000e+00	
    };
	
	// setup Function 
	this->setAttributeValue(TT("function"), TT("freehand"));

	
	// create 1 channel audio signal objects
	TTObjectBaseInstantiate(kTTSym_audiosignal, &input, 1);
	TTObjectBaseInstantiate(kTTSym_audiosignal, &output, 1);
	input->allocWithVectorSize(N);
	output->allocWithVectorSize(N);
	
	// create a signal to be transformed and then process it)
	input->clear();	
	for (int i=0; i<N; i++)
		input->mSampleVectors[0][i] = expectedSignalTest1[i];
	
	this->process(input, output);
	
	// now test the output
	for (int n=0; n<N; n++)
	{
		TTBoolean result = !TTTestFloatEquivalence(output->mSampleVectors[0][n], expectedSignalTest1[n]);
		badSampleCount += result;
		if (result) 
			std::cout << "BAD SAMPLE @ n=" << n << " ( value=" << output->mSampleVectors[0][n] << " expected=" << expectedSignalTest1[n] << " )\n";
	}
	
    if (badSampleCount)
		std::cout << "badSampleCount is " << badSampleCount << "\n";
    
    /*
     TTTestAssertion("Produces correct function values",
     badSampleCount == 0,
     testAssertionCount,
     errorCount);
     
     */
	
	TTObjectBaseRelease(&input);
	TTObjectBaseRelease(&output);
	
	// wrap up test results and pass back to whoever called test
	return TTTestFinish(testAssertionCount, errorCount, returnedTestInfo);
	
}