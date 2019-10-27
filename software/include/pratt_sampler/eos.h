#ifndef __EOS_H_
#define __EOS_H_

#include <complex>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <gsl/gsl_sf.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_poly.h>
#include <boost/math/special_functions/bessel.hpp>
#include "classdefs.h"
#include "constants.h"
#include "sf.h"
#include "resonances.h"

using namespace std;

namespace EOS{
    void freegascalc_onespecies(CboseMap &npidens,CboseMap &npiP,CboseMap &npiepsilon,CboseMap &npidedt,CparameterMap *parmap,CresInfo *resinfo,double T,double m,double &epsilon,double &P,double &dens,double &sigma2,double &dedt);
    void freegascalc_onespecies_finitewidth(CboseMap &npidens,CboseMap &npiP,CboseMap &npiepsilon,CboseMap &npidedt,CparameterMap *parmap,CresInfo *resinfo,double T,double resmass,double m1, double m2, double reswidth_alpha, 
                                            double &epsilon,double &P,double &dens,double &sigma2,double &dedt,double &maxweight);

};

#endif