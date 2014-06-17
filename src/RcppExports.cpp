// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// CTCRWNLL
Rcpp::List CTCRWNLL(const arma::mat& y, const arma::mat& Hmat, const arma::mat& Qmat, const arma::mat& Tmat, const arma::vec& noObs, const arma::vec& activity, const arma::colvec& a, const arma::mat& P);
RcppExport SEXP crawl_CTCRWNLL(SEXP ySEXP, SEXP HmatSEXP, SEXP QmatSEXP, SEXP TmatSEXP, SEXP noObsSEXP, SEXP activitySEXP, SEXP aSEXP, SEXP PSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        //Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< const arma::mat& >::type y(ySEXP );
        Rcpp::traits::input_parameter< const arma::mat& >::type Hmat(HmatSEXP );
        Rcpp::traits::input_parameter< const arma::mat& >::type Qmat(QmatSEXP );
        Rcpp::traits::input_parameter< const arma::mat& >::type Tmat(TmatSEXP );
        Rcpp::traits::input_parameter< const arma::vec& >::type noObs(noObsSEXP );
        Rcpp::traits::input_parameter< const arma::vec& >::type activity(activitySEXP );
        Rcpp::traits::input_parameter< const arma::colvec& >::type a(aSEXP );
        Rcpp::traits::input_parameter< const arma::mat& >::type P(PSEXP );
        Rcpp::List __result = CTCRWNLL(y, Hmat, Qmat, Tmat, noObs, activity, a, P);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// CTCRWPREDICT
Rcpp::List CTCRWPREDICT(const arma::mat& y, const arma::mat& Hmat, const arma::mat& Qmat, const arma::mat& Tmat, const arma::vec& noObs, const arma::vec& activity, const arma::colvec& a, const arma::mat& P);
RcppExport SEXP crawl_CTCRWPREDICT(SEXP ySEXP, SEXP HmatSEXP, SEXP QmatSEXP, SEXP TmatSEXP, SEXP noObsSEXP, SEXP activitySEXP, SEXP aSEXP, SEXP PSEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        //Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< const arma::mat& >::type y(ySEXP );
        Rcpp::traits::input_parameter< const arma::mat& >::type Hmat(HmatSEXP );
        Rcpp::traits::input_parameter< const arma::mat& >::type Qmat(QmatSEXP );
        Rcpp::traits::input_parameter< const arma::mat& >::type Tmat(TmatSEXP );
        Rcpp::traits::input_parameter< const arma::vec& >::type noObs(noObsSEXP );
        Rcpp::traits::input_parameter< const arma::vec& >::type activity(activitySEXP );
        Rcpp::traits::input_parameter< const arma::colvec& >::type a(aSEXP );
        Rcpp::traits::input_parameter< const arma::mat& >::type P(PSEXP );
        Rcpp::List __result = CTCRWPREDICT(y, Hmat, Qmat, Tmat, noObs, activity, a, P);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}