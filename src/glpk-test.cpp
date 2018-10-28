#include <rcppglpk.h>

using namespace Rcpp;


//' Simplex LP solver
//'
//' @param obj Numeric vector of size \eqn{K}. Coeficients in the objective function.
//' @param subj_lhs Numeric matrix of size \eqn{K\times m}. Constraints.
//' @param subj_rhs Numeric vector of size \eqn{1\times m}. Constraints.
//' @param pname Character scalar. Name of the LP.
//' @param cols_bnds_type,cols_bnds_lb,cols_bnds_ub passed to `glp_set_col_bnds`
//' @param rows_bnds_type,rows_bnds_lb,rows_bnds_ub passed to `glp_set_row_bnds`
//' @param DIR Integer. Type of problem (see [GLP_MAX]).
//' @export
//'
//' @examples
//' # Brief example from de glpk manual:
//' # max:
//' #   z = 10x_1 + 6x_2 + 4x_3
//' #
//' # subject to:
//' #   p = x_1 + x_2 + x_3
//' #   q = 10x_1 + 4x_2 + 5x_3
//' #   r = 2x_1 + 2x_2 + 6x_3
//' #
//' # And bounds of variables
//' #   p <= 100, 0 <= x_1
//' #   q <= 600, 0 <= x_2
//' #   r <= 300, 0 <= x_3
//' #
//' # Where (p,q,r) are auxiliary variables, and (x_1,x_2,x_2) are
//' # structural variables.
//'
//' obj      <- c(10, 6, 4)
//' subj_lhs <- matrix(c(1, 10, 2, 1, 4, 2, 1, 5, 6), ncol = 3)
//' subj_rhs <- c(100, 600, 300)
//'
//' # Bounds
//' cols_bnds_type <- rep(GLP_LO, 3)
//' cols_bnds_lb <- cols_bnds_ub <- rep(0.0, 3)
//'
//' rows_bnds_type <- rep(GLP_UP, 3)
//' rows_bnds_lb <- rep(0.0, 3)
//' rows_bnds_ub <- subj_rhs
//'
//' lp(
//'   obj, subj_lhs, subj_rhs,
//'   cols_bnds_type, cols_bnds_lb, cols_bnds_ub,
//'   rows_bnds_type, rows_bnds_lb, rows_bnds_ub,
//'   DIR = GLP_MAX
//'   )
// [[Rcpp::export]]
List lp(
    const NumericVector & obj,
    const NumericMatrix & subj_lhs,
    const NumericVector & subj_rhs,
    const IntegerVector & cols_bnds_type,
    const NumericVector & cols_bnds_lb,
    const NumericVector & cols_bnds_ub,
    const IntegerVector & rows_bnds_type,
    const NumericVector & rows_bnds_lb,
    const NumericVector & rows_bnds_ub,
    int DIR,
    const StringVector pname = "sample")
{

  // Checking dimensions
  if (obj.size() != subj_lhs.ncol())
    stop("The objective function doesn't have as many columns in there are in constraints.");

  if (subj_lhs.nrow() != subj_rhs.size())
    stop("The constraints don't have as many rows as the RHS.");

  // Creating the object
  rcppglpk::LP lp(
      obj,
      subj_lhs,
      subj_rhs,
      // Columns
      cols_bnds_type,
      cols_bnds_lb,
      cols_bnds_ub,
      // Rows
      rows_bnds_type,
      rows_bnds_lb,
      rows_bnds_ub,
      DIR
    );

  // Calling the solver
  lp.simplex();


  return lp.getSol();
}
