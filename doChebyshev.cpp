//
//  doChebyshev.cpp
//  certain_Astro_Parameters
//
//  Created by Clifford Campo on 2/21/21.
//  Copyright © 2021 CliffordCampo. All rights reserved.
//
//
//  doChebyshev.cpp
//  astroParameter
//  copied from:
//  Created by Clifford Campo on 11/30/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//

#include "doChebyshev.hpp"
#include <stdlib.h>

Constants *con = nullptr; //I need to define this here as a global so bracket_search can find it, since binary_search routine \
can not pass to its call-back routine the pointer to the instance of the class object named Constants. doChebyshev initializes con upon its entry \
// int numberOfLoops = 0;   //Keep some statistics
// int numberOfEntries = 0; //Keep some statistics
#ifdef BRACKETSEARCH
int bracket_search (const void *, const void *);
int bracket_search (const void *keyValue, const void *entryWithinArrayOfEntries) {
#define TOOHOT    1
#define TOOCOLD  -1
#define JUSTRIGHT 0
    // bracket_search looks for a value where [gotThis-1] <= `lookingForThis` <= [gotThis + 0] this condition requires a return code\
    of 0 to stop the binary search. Intend to call the bracket_search routine from the gsl_cheb_init call-back routine, \
    chebyschevFunction. \
    If `lookingForThis` > [gotThis + 0], this condition warrents a return code of  1 (i.e., key exceeds member value) \
    If `lookingForThis` < [gotThis + 0], this condition warrents a return code of -1
    ++con->numberOf_BS_Entries;   //Used for metrics. Keeps track of number of times we've entered bracket_search.
    
    //b points to some entry within an array of doubles.
    con->bhigher = (myParameters *)entryWithinArrayOfEntries;
    con->blower = con->bhigher-1; //have bprevious point to the double prior to
    //NOTE:    The above two entries "bracket" the values between which we wish keyValue to lie between
    con->a = *(double *)keyValue; //a points to the trial value we want to find that lies between con->bprevious (low end) and \
    con->bcurrent (high end) of the bracket interval, the upper-most of which is provided by input parameter entryWithinArrayOfEntries.
    //    if (con->a > (con->largestIV || con->a < (con->smallestIV) {
    
    if (con->a > (con->bhigher)->dbl_value ) {
        return TOOHOT; //Tell bsearch to give us another interval because the trial value bsearch gave us to evaluate lies above \
        the trial interval.
    } else if (con->a < (con->blower)->dbl_value ) {
        return TOOCOLD; //Tell besarch to give us another interval because the trial value bsearch gave us to evaluate lies below \
        the trial interval.
    } else if (con->a >= (con->blower)->dbl_value && con->a <= (con->bhigher)->dbl_value ) {
        return JUSTRIGHT; //Tell bsearch to stop binary searching because we found the vaule for which we're searching.
    } else {
        fprintf(stderr,"Line %d of File %s: Error, the value we are looking for, %f lies outside the range of our array of ranges:\
                \nlowest value of %f and highest value of %f\n", __LINE__, __FILE__, con->a, (con->dc->aai)->dbl_value, \
                (con->dc->aai + con->dc->o - 1)->dbl_value);
        return -2; //This return is made purposefully bad so that the calling program, bsearch, will projectile vomit all over the \
        floor because the entry bsearch gave us lies outside the boundry of our array of entries. Should never see this -2 return code \
        because getting to this point in the code IS MOST MOST UNUSUAL.
    }
}
#endif
// ===============================================================================================================================

double cf (double x, void *p) {  //cf stands for call-back function. \
    x contains the value provided by the GSQL routine; \
    numberOfEntries++;  //bump-up the number of times we've entered this gsl call-back function
    con->dc = (do_Chebyshev *)p; //Recast input parameter, p, from type `pointer-to-void` to type `pointer-to-do_Chebyshev`
    con->dc->workTrialDependent = MAXFLOAT; //Initialize this variable to something unusually huge so we know, when stepping \
through debugger, that this value has been deliberately changed.
    
#ifdef BRACKETSEARCH  //When BRACKETSEARCH is specified as a compiler option then we are doing a binary search through the array \
of independent variables.
    //Returns with con->ptrTemp pointing to the higher double of the bracket. Therefore, con->ptrTemp-1 must then points to the \
    next lower value of the bracket
    con->ptrIndependentUpper = (myParameters *)bsearch(&x, con->dc->aai, (unsigned int)con->dc->o, sizeof(myParameters), bracket_search);
    //
    con->ptrIndependentLower = con->ptrIndependentUpper -1; //compute pointer to the dependent value previous to the pointer to \
    the value returned by bsearch. 
    //--   con->dc->workIndependentLower = *(con->bhigher); //con->bhigher set by bracket_search, call-back routine of bsearch
    //--   con->dc->workInDependentUpper = *(con->blower);  //con->blower set by bracket_search, call-back routine of bsearch
    con->lowerIndex = con->blower->index;
    con->upperIndex = con->bhigher->index;
    con->delta = ((con->bhigher)->dbl_value - (con->blower)->dbl_value);
    if (con->delta > 0) {
        con->dc->workTrialIndependent = (( x - (con->blower)->dbl_value )/con->delta);
        con->dc->workTrialDependent = (con->ptrArrayDependent + con->lowerIndex)->dbl_value;
        con->workDouble = (con->ptrArrayDependent + con->upperIndex)->dbl_value - (con->ptrArrayDependent + con->lowerIndex)->dbl_value;
        con->workDouble *= con->dc->workTrialIndependent;
        con->dc->workTrialDependent += con->workDouble;
    } else { //This is the WTF destination of the above if statement. \
        Got here because (con->bhigher)->dbl_value === con->blower)->dbl_value
        cerr << "(con->bhigher)->dbl_value: " << (con->bhigher)->dbl_value << ", (con->blower)->dbl_value: " << (con->blower)->dbl_value << endl;
        con->dc->workTrialDependent = NAN;  //This is what you get when trying to divide by zero.
        return(con->dc->workTrialDependent);
    }
#else  //Do a simple linear search if BRACKETSEARCH is not specified as a compiler option. This is the baseline
    for (con->dc->loopCounter=0; con->dc->loopCounter < con->dc->o; con->dc->loopCounter++ ) {  // Linear Search through both independent and dependent parameter arrays
        if ( (x >= *(con->dc->aai + con->dc->loopCounter)) && (x <= *(con->dc->aai + con->dc->loopCounter + 1) ) ) {
            con->dc->workIndependentLower = *(con->dc->aai + con->dc->loopCounter );
            con->dc->workInDependentUpper = *(con->dc->aai + con->dc->loopCounter  + 1);
            con->dc->workTrialIndependent = ( x - con->c->workIndependentLower )/(con->dc->workInDependentUpper - con->dc->workIndependentLower);
            con->dc->workTrialDependent = con->dc->workTrialIndependent * (*(con->dc->aad + con->dc->loopCounter  +1) - *(con->dc->aad + con->dc->loopCounter )) + *(con->dc->aad + con->dc->loopCounter );
            break;
        } //End of IF satatement
    } //EndOfForLoop
    con->numberOfLoops += con->dc->loopCounter; //Update the number of internal loops executed for this entry to the chebyshev call-back function.
#endif
    
    return (con->dc->workTrialDependent); //Return this value to gsl_cheb_eval
}

//
// points to dependent variable, found by chebyshev interpolation technique (NOT USED)-
// points to array of associated dependent variables ------------------------+
// points to array of independent variables --------------------+            |
// points to desired THE independent variable -+                |            |
// points to chebyshev order ---------+        |                |            |
// points to class Instance +         |        |                |            |
//                          |         |        |                |            |
//                          V         V        V                V            V
double doChebyshev(Constants &c,    size_t o, double *di, myParameters *aai, myParameters *aad ) {

    con = &c;  //Make sure the global, named con, has addressability to the instance object of class named Constants.
    con->dc = (do_Chebyshev *)calloc(sizeof(do_Chebyshev), 1); //Get working memory
    con->dc->o   = o;     // Number of values in each array aai and aad
    con->dc->di  = di;    // The independent variable for which we're requesting cheb to determing corresponding dependent variable.
    con->dc->aai = aai;    // Array of Independent variables
    con->dc->aad = aad;    // Array of Dependent variables.
    con->dc->fd  = nullptr;  //fd not used now that we're returning a double in an orthodox manner. This instruction can be removed.
    strcpy(con->dc->dochebyshev, "DOCHEBYSHEV");
    c.cs = gsl_cheb_alloc( o );         //Allocate gsl_chebyshev's working storage (can we do this just once in the Constants \
    class's constructor and then deallocate this storage in the Constants destructor? Will it save time? No we can't because we \
    don't know the value of o at the time we execute the constructor. Of course, we could allocate a max possible value of o.
    c.F.function = cf;                                        // Communicate to gsl_cheb the address of the call-back function.
    c.F.params = con->dc;                                     // point to the "doChebyshev" work area.
    con->dc->a = con->dc->aai[0].dbl_value;             //Designate the minimum value of this interval of independent parameters
    con->dc->b = con->dc->aai[o-1].dbl_value; //Designate the maximum value of this interval of independent parameters
#ifdef  BRACKETSEARCH
    if (c.db2__ && ((c.numberOfEntries) != 0) ) cout << "accumulated numberOfEntries: " << c.numberOfEntries  <<  \
        "\naccumulated numberOf_BS_Entries: " << c.numberOf_BS_Entries <<  "; ratio=>" << \
        ((1.0)*c.numberOf_BS_Entries)/(1.0*(c.numberOfEntries)) << "\ncon->dc->a: " << std::fixed << con->dc->a << \
        "\ncon->dc->b: " <<  std::fixed   << con->dc->b << endl;
#else
    if (c.db2__) cout << "accumulated numberOfEntries: " << c.numberOfEntries  << "\naccumulated numberOfLoops: " << \
        c.numberOfLoops << "\ncon->dc->a: " << std::fixed << con->dc->a << "\ncon->dc->b: " <<  std::fixed   << \
        con->dc->b << endl;
#endif
    con->dc->rc = gsl_cheb_init((gsl_cheb_series *)c.cs, (const gsl_function *)&c.F, (const double)con->dc->a, (const double)con->dc->b);
    con->dc->workResult = gsl_cheb_eval((const gsl_cheb_series *)c.cs, (double)*di);
    c.workDouble = con->dc->workResult;   //////////////////////////////////////////////////////////////////////////////
    if (c.cs) gsl_cheb_free(c.cs); //If c.cs is not zero, which it shouldn't be at this point in the processing, then free it.
    c.cs=nullptr;
    if (con->dc) free(con->dc); //If con->dc is not zero, which it shouldn't be at this point in the processing, then free it.
    con->dc = nullptr;
    return(c.workDouble);              //Return the dependent value we've discovered through the Chebyshev Polynomial Interpolation.
}

