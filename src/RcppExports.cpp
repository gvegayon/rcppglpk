// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// glpk_example
List glpk_example(const NumericVector& obj, const NumericMatrix& M, StringVector pname);
RcppExport SEXP _rcppglpk_glpk_example(SEXP objSEXP, SEXP MSEXP, SEXP pnameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericVector& >::type obj(objSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type M(MSEXP);
    Rcpp::traits::input_parameter< StringVector >::type pname(pnameSEXP);
    rcpp_result_gen = Rcpp::wrap(glpk_example(obj, M, pname));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_rcppglpk_glpk_example", (DL_FUNC) &_rcppglpk_glpk_example, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_rcppglpk(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}