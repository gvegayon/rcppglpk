#' GLPK constants
#' @section Boundaries:
#'
#' - `GLP_FR`: 1, free (unbounded) variable.
#' - `GLP_LO`: 2, variable with lower bound.
#' - `GLP_UP`: 3, variable with upper bound.
#' - `GLP_DB`: 4, double-bounded variable.
#' - `GLP_FX`: 5, fixed variable.
#'
#' @section Type of problem:
#'
#' - `GLP_MIN`: 1, Minimization
#' - `GLP_MAX`: 2, Maximization
#'
#'
#' @name GLPK-constants
NULL

#' @export
#' @rdname GLPK-constants
GLP_FR <- 1L

#' @export
#' @rdname GLPK-constants
GLP_LO <- 2L

#' @export
#' @rdname GLPK-constants
GLP_UP <- 3L

#' @export
#' @rdname GLPK-constants
GLP_DB <- 4L

#' @export
#' @rdname GLPK-constants
GLP_FX <- 5L

#' @export
#' @rdname GLPK-constants
GLP_MIN <- 1L

#' @export
#' @rdname GLPK-constants
GLP_MAX <- 2L
