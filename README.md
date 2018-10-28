
<!-- README.md is generated from README.Rmd. Please edit that file -->

# rcppglpk

[![lifecycle](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)

While the R packages [Rglpk](https://cran.r-project.org/package=Rglpk),
and [lpSolveAPI](https://cran.r-project.org/package=lpSolveAPI) are
great interfaces for [GLPK]() and [lpsolve](), the logic behind the
development of these R wrappers was thinking in a single problem at a
time, which is not good enough for me.

In another project that I’m working on, I need to solve a couple of
hundred (or thousand) problems at the same time. The good news are that
a lot of the structure of these problems is equal across instances,
which means that I can save time by just updating some parameters of the
LP and resolve it. Moreover, if all this could be done in a seemless way
using [Rcpp](https://cran.r-project.org/package=Rcpp), it would be
ideal. This was the main reason why I decided to write `rcppglpk`.

**Important** Notice that the `rcpp` is in lowercase.

## Installation

You can install this package using devtools.

``` r
devtools::install_github("gvegayon/rcppglpk")
```

To compile it, you will need to have GNU GLPK installed in your system.

## Example

The following function is included in
[src/glpk-test.cpp](src/glpk-test.cpp)

``` cpp
#include <rcppglpk.h>

using namespace Rcpp;

List glpk_example(
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
```

Things to point out:

1.  The `rcppglpk.h` header includes Rcpp, so we don’t need to add it
    again.

2.  To create a new linear programming problem, we just typed
    `rcppglpk::LP` and passed the arguments.

3.  We called the `simplex` solver via the method of the same name.

4.  The method `getSol()` returns an object of class `Rcpp::List` with
    the value of the objective function, and the parameters.

Once compiled, we can call it in R:

``` r
library(rcppglpk)

# Brief example from de glpk manual:
# max:
#   z = 10x_1 + 6x_2 + 4x_3
#
# subject to:
#   p = x_1 + x_2 + x_3
#   q = 10x_1 + 4x_2 + 5x_3
#   r = 2x_1 + 2x_2 + 6x_3
#
# And bounds of variables
#   p <= 100, 0 <= x_1
#   q <= 600, 0 <= x_2
#   r <= 300, 0 <= x_3
#
# Where (p,q,r) are auxiliary variables, and (x_1,x_2,x_2) are
# structural variables.

obj      <- c(10, 6, 4)
subj_lhs <- matrix(c(1, 10, 2, 1, 4, 2, 1, 5, 6), ncol = 3)
subj_rhs <- c(100, 600, 300)

# Bounds
cols_bnds_type <- rep(GLP_LO, 3)
cols_bnds_lb <- cols_bnds_ub <- rep(0.0, 3)

rows_bnds_type <- rep(GLP_UP, 3)
rows_bnds_lb <- rep(0.0, 3)
rows_bnds_ub <- subj_rhs

lp(
  obj, subj_lhs, subj_rhs,
  cols_bnds_type, cols_bnds_lb, cols_bnds_ub,
  rows_bnds_type, rows_bnds_lb, rows_bnds_ub,
  DIR = GLP_MAX
  )
#> $val
#> [1] 733.3333
#> 
#> $par
#> [1] 33.33333 66.66667  0.00000
```

## Linking to `rcppglpk`

To use the header of `rcppglpk` you need to add the following:

1.  `LinkingTo: Rcpp, rcppglpk` in your `DESCRIPTION` file
2.  `PKG_LIBS = -lglpk` in your `Makevars` file (**you need to have glpk
    in your system**)

Voila\! You should be able to compile your R package linking to
`rcppglpk`
