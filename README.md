
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
obj      <- c(10, 6, 4)
subj_lhs <- matrix(c(1, 10, 2, 1, 4, 2, 1, 5, 6), ncol = 3)
subj_rhs <- c(100, 600, 300)
glpk_example(obj, subj_lhs, subj_rhs)
#> $val
#> [1] 733.3333
#> 
#> $par
#> [1] 33.33333 66.66667  0.00000
```

## Linking to `rcppglpk`

To use the header of `rcppglpk` you need to add the following:

1.  `LinkingTo: Rcpp, rcppglpk` in your `DESCRIPTION` file
2.  `PKG_LIBS = -lglpk` in your `Makevars` file

Voila\! You should be able to compile your R package linking to
`rcppglpk`
