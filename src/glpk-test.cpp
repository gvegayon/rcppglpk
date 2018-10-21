#include <rcppglpk.h>

using namespace Rcpp;


//' A mininmal example
//' @param obj Numeric vector of size \eqn{K}. Coeficients in the objective function.
//' @param subj_lhs Numeric matrix of size \eqn{K\times m}. Constraints.
//' @param pname Character scalar. Name of the LP.
//' @export
//' @examples
//' obj      <- c(10, 6, 4)
//' subj_lhs <- matrix(c(1, 10, 2, 1, 4, 2, 1, 5, 6), ncol = 3)
//' subj_rhs <- c(100, 600, 300)
//' glpk_example(obj, subj_lhs, subj_rhs)
// [[Rcpp::export]]
List glpk_example(
    const NumericVector & obj,
    const NumericMatrix & subj_lhs,
    const NumericVector & subj_rhs,
    const StringVector pname = "sample")
{

  // Checking dimensions
  if (obj.size() != subj_lhs.ncol())
    stop("The objective function doesn't have as many columns in there are in constraints.");

  if (subj_lhs.nrow() != subj_rhs.size())
    stop("The constraints don't have as many rows as the RHS.");

  // Creating the object
  rcppglpk::LP lp(obj, subj_lhs, subj_rhs, GLP_MAX);

  // Calling the solver
  lp.simplex();


  return lp.getSol();
}
