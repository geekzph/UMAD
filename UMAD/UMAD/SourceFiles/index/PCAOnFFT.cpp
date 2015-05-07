#include "../../HeaderFiles/index/PCAOnFFT.h"

#include "../../HeaderFiles/index/IncrementalSelection.h"

#include "../../HeaderFiles/index/FFTPivotSelectionMethod.h"

#include "../../HeaderFiles/index/PCA.h"

#include <stdio.h>

#include "../../SourceFiles/util/GSL/include/gsl/gsl_rng.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_matrix_double.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_vector_double.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_blas.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_eigen.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_linalg.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_statistics.h"





#include <math.h>

#include <time.h>

vector<int> removeDuplicate(CMetricDistance *metric,vector<shared_ptr<CMetricData> > data,vector<int> &pivots,int first,int dataSize);



CPCAOnFFT::CPCAOnFFT(int _FFTScale) : FFTScale(_FFTScale){}



CPCAOnFFT::~CPCAOnFFT(){}



vector<int> CPCAOnFFT::selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, int numPivots)

{
	vector<int> a;
	return a;

}



vector<int> CPCAOnFFT::selectPivots(CMetricDistance *metric, vector<shared_ptr<CMetricData> > &data, 

                                    int first, int dataSize, int numPivots)

{



	if (numPivots >= dataSize) 

	{

		vector<int>* pivots=new vector<int>;

		for (int i = first; i < first+dataSize + 0; i++)

			pivots->push_back(i);

		

		return removeDuplicate(metric, data, *pivots,first,dataSize);

	}
	
	


   CFFTPivotSelectionMethod FFTMethod;

	vector<int> fftResult = FFTMethod.selectPivots(metric,data,first,dataSize, numPivots*FFTScale);

	
	 if(fftResult.size() <= min(numPivots, dataSize))

        return fftResult;

	

    gsl_matrix* matrix = gsl_matrix_alloc(dataSize, fftResult.size());

 
	for (int col = 0; col < fftResult.size(); col++)
            for (int row = 0; row < dataSize; row++)
                gsl_matrix_set(matrix, row, col, metric->getDistance( data.at(row).get(), data.at(fftResult.at(col)).get() ));


    gsl_matrix* pcaResult = gsl_matrix_alloc( numPivots, fftResult.size());

	

    try

    {

        pcaResult = CPCA::EMPCA( matrix, numPivots);

    }

    catch(exception e)

    {

        cout<< "Exception!  data size =" << dataSize 

            << " , num pivots = " << numPivots << endl;  

        cout<< e.what() << endl;



        if (numPivots >= fftResult.size())

            return fftResult;

        else

        {

            fftResult.resize(numPivots);

            return fftResult;

        }

    }

    vector<int> selectByAngleResult 

        = CPCA::pivotSelectionByAngleWithAxes( pcaResult, numPivots);
	 
    vector<int> finalResult;
    for (int i = 0; i < numPivots; ++i)
    {	
		finalResult.push_back( fftResult.at( selectByAngleResult.at(i) ));
    }


	

	/*vector<int> selectByProjectionResult  
	      = CPCA::pivotSelectionByPCAResultProjection( matrix,pcaResult, numPivots);
	     
	
    vector<int> finalResult;

    for (int i = 0; i < numPivots; ++i)

    {
		finalResult.push_back( selectByProjectionResult .at(i) + first ) ;
    }*/

	
	

	/*vector<int> selectByAngleResult 
	      = CPCA::pivotSelectionByAngleWithPoint( matrix,pcaResult, numPivots);
	  
    vector<int> finalResult;

    for (int i = 0; i < numPivots; ++i)

    {
		finalResult.push_back( selectByAngleResult.at(i) +first) ;
    }*/

	
	
	
	return finalResult;


}








