#include <Rcpp.h>

using namespace Rcpp;


#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

// "Row indices of each element are stored in the array ia, column indices are stored in
// the array ja, and numerical values of corresponding elements are stored in the array ar."
// So... data must be presented as:
// (Row, Column, Data) arrays of vectors starting at 1 (not 0)
void as_glpk_array(
    const NumericMatrix & x,
    int *ai, int *aj,
    double *av
) {

  int n = x.nrow();
  int k = x.ncol();

  for (int i = 0; i < n; i++)
    for (int j = 0; j < k; j++) {
      ai[i + 1 + j*k] = i + 1;
      aj[i + 1 + j*k] = j + 1;
      av[i + 1 + j*k] = x.at(i,j);
    }


  return;
}

//' A mininmal example
//' @param obj Numeric vector of size \eqn{K}. Coeficients in the objective function.
//' @param M Numeric matrix of size \eqn{K\times m}. Constraints.
//' @param pname Character scalar. Name of the LP.
//' @export
//' @examples
//' obj <- c(10, 6, 4)
//' dat <- matrix(c(1, 10, 2, 1, 4, 2, 1, 5, 6), ncol=3)
//' glpk_example(obj, dat)
// [[Rcpp::export]]
List glpk_example(
    const NumericVector & obj,
    const NumericMatrix & M,
    StringVector pname = "sample")
{
  // Initializing objects and parameters
  glp_prob *lp;
  glp_smcp param;
  glp_init_smcp(&param);
  param.msg_lev = GLP_MSG_ERR;

  int ia[1+1000], ja[1+1000];
  double ar[1+1000], z, x1, x2, x3;

  // Setting up the problem
  lp = glp_create_prob();
  glp_set_prob_name(lp, pname[0]);

  // We want to maximize
  glp_set_obj_dir(lp, GLP_MAX);

  // We have 3 constriants: First specify the slack variables
  glp_add_rows(lp, 3);

  glp_set_row_name(lp, 1, "p");
  glp_set_row_bnds(lp, 1, GLP_UP, 0.0, 100.0);

  glp_set_row_name(lp, 2, "q");
  glp_set_row_bnds(lp, 2, GLP_UP, 0.0, 600.0);

  glp_set_row_name(lp, 3, "r");
  glp_set_row_bnds(lp, 3, GLP_UP, 0.0, 300.0);

  // Now adding the columns (x1, x2, x3)
  // All three positive (GLP_LO with single bound at 0.0)
  // 10x 1 + 6x 2 + 4x 3
  glp_add_cols(lp, obj.length());

  for (int i = 0; i<obj.length(); i++) {
    // glp_set_col_name(lp, 1, "x1");
    glp_set_col_bnds(lp, i+1, GLP_LO, 0.0, 0.0);
    glp_set_obj_coef(lp, i+1, obj.at(i));
  }

  // glp_set_col_name(lp, 1, "x1");
  // glp_set_col_bnds(lp, 1, GLP_LO, 0.0, 0.0);
  // glp_set_obj_coef(lp, 1, 10.0);
  //
  // glp_set_col_name(lp, 2, "x2");
  // glp_set_col_bnds(lp, 2, GLP_LO, 0.0, 0.0);
  // glp_set_obj_coef(lp, 2, 6.0);
  //
  // glp_set_col_name(lp, 3, "x3");
  // glp_set_col_bnds(lp, 3, GLP_LO, 0.0, 0.0);
  // glp_set_obj_coef(lp, 3, 4.0);

  // Loading the matrix
  // ia[1] = 1, ja[1] = 1, ar[1] = 1.0;
  // ia[2] = 1, ja[2] = 2, ar[2] = 1.0;
  // ia[3] = 1, ja[3] = 3, ar[3] = 1.0;
  // ia[4] = 2, ja[4] = 1, ar[4] = 10.0;
  // ia[5] = 3, ja[5] = 1, ar[5] = 2.0;
  // ia[6] = 2, ja[6] = 2, ar[6] = 4.0;
  // ia[7] = 3, ja[7] = 2, ar[7] = 2.0;
  // ia[8] = 2, ja[8] = 3, ar[8] = 5.0;
  // ia[9] = 3, ja[9] = 3, ar[9] = 6.0;

  as_glpk_array(M, ia, ja, ar);

  glp_load_matrix(lp, M.ncol()*M.nrow(), ia, ja, ar);

  // Solving the problem and retrieving the obj
  // value
  glp_simplex(lp, &param);

  NumericVector ans(obj.length());

  z = glp_get_obj_val(lp);
  for (int i =0;i<ans.length(); i++)
    ans.at(i) = glp_get_col_prim(lp, i+1);
  // x1 = glp_get_col_prim(lp, 1);
  // x2 = glp_get_col_prim(lp, 2);
  // x3 = glp_get_col_prim(lp, 3);

  // Returning and deleting the problem
  // Rprintf("\nz = %g; x1 = %g; x2 = %g\n", z, x1, x2, x3);
  glp_delete_prob(lp);

  return List::create(
    _["z"] = z,
    _["x"] = ans
  );
}

