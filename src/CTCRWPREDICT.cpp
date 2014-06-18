// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "RcppArmadillo.h"
// [[Rcpp::depends(RcppArmadillo)]]
using namespace Rcpp;
using namespace arma;


// [[Rcpp::export]]
Rcpp::List CTCRWPREDICT(const arma::mat& y, const  arma::mat& Hmat, 
const arma::mat& Qmat, const arma::mat& Tmat, 
const arma::vec& noObs,const arma::vec& active, const arma::colvec& a,
const arma::mat& P)
{
  int I = y.n_rows;
  arma::mat u(2,I, fill::zeros);
  arma::mat jk(2,I, fill::zeros);
  arma::cube M(2,2,I, fill::zeros);
  arma::mat pred(4,I, fill::zeros);
  arma::cube predVar(4,4,I, fill::zeros);
  arma::mat Z(2,4, fill::zeros); Z(0,0) = 1; Z(1,2) = 1;
  arma::mat T(4,4, fill::zeros); T(0,0) = 1; T(2,2) = 1;
  arma::mat Q(4,4, fill::zeros);
  arma::cube F(2,2,I, fill::zeros);
  arma::mat H(2,2, fill::zeros);
  arma::cube K(4,2,I, fill::zeros);
  arma::cube L(4,4,I, fill::zeros);
  arma::mat v(2,I, fill::zeros);
  arma::mat aest(4,I+1, fill::zeros);
  aest.col(0)=a;
  arma::cube Pest(4,4,I+1, fill::zeros);
  Pest.slice(0)=P;
  arma::colvec r(4, fill::zeros);
  arma::mat N(4,4, fill::zeros);
  arma::vec chisq(I, fill::zeros);
  
  double ll=0;
  //Forward filter
  for(int i=0; i<I; i++){
    if(active(i)==0){
      T(0,1) = 0; T(1,1)=0; T(2,3)=0; T(3,3)=0;
      Q = 0.0*Q;
    } else{
      Q(0,0) = Qmat(i,0);
      Q(1,0) = Qmat(i,1);
      Q(0,1) = Qmat(i,1);
      Q(1,1) = Qmat(i,2);     
      Q(2,2) = Qmat(i,0);
      Q(3,2) = Qmat(i,1);
      Q(2,3) = Qmat(i,1);
      Q(3,3) = Qmat(i,2);
      T(0,1) = Tmat(i,0);
      T(1,1) = Tmat(i,1);
      T(2,3) = Tmat(i,0);
      T(3,3) = Tmat(i,1);
    }
    if(noObs(i)==1){
      aest.col(i+1) = T*aest.col(i);
      Pest.slice(i+1) = T*Pest.slice(i)*T.t() + Q;
      L.slice(i) = T;
    } else {
      H(0,0) = Hmat(i,0); 
      H(1,1) = Hmat(i,1); 
      H(0,1) = Hmat(i,2); 
      H(1,0) = Hmat(i,2);
      v.col(i) = y.row(i).t()-Z*aest.col(i);
      F.slice(i) = Z*Pest.slice(i)*Z.t() + H; 
      ll += - (log(det(F.slice(i))) + dot(v.col(i),solve(F.slice(i),v.col(i))))/2; 
      K.slice(i) = T*Pest.slice(i)*Z.t()*F.slice(i).i();     
      L.slice(i) = T - K.slice(i)*Z;
      aest.col(i+1) = T*aest.col(i) + K.slice(i)*v.col(i);
      Pest.slice(i+1) = T*Pest.slice(i)*L.slice(i).t() + Q;
    }
  }
  
  // Backward smooth
  for(int j=I; j>0; j--){
    if(noObs(j-1)==1 || F.slice(j-1)(0,0)*F.slice(j-1)(1,1)==0){
      r = L.slice(j-1).t() * r;
      N = L.slice(j-1).t() * N * L.slice(j-1);
    } else{
      u.col(j-1) = solve(F.slice(j-1),v.col(j-1))-K.slice(j-1).t()*r;
      M.slice(j-1) = F.slice(j-1).i() + K.slice(j-1).t()*N*K.slice(j-1);
      chisq(j-1) = dot(u.col(j-1),solve(M.slice(j-1),u.col(j-1)));
      jk.col(j-1) = y.row(j-1).t() - solve(M.slice(j-1),u.col(j-1));
      r = Z.t()*solve(F.slice(j-1),v.col(j-1)) + L.slice(j-1).t() * r;
      N = Z.t() * solve(F.slice(j-1),Z) + L.slice(j-1).t()*N*L.slice(j-1); 
    }
    pred.col(j-1) = aest.col(j-1) + Pest.slice(j-1)*r;
    predVar.slice(j-1) = Pest.slice(j-1) - Pest.slice(j-1)*N*Pest.slice(j-1); 
  }
  return Rcpp::List::create(
    Rcpp::Named("ll") = ll, 
    Rcpp::Named("pred") = pred, Rcpp::Named("predVar")=predVar,
    Rcpp::Named("chisq")=chisq, Rcpp::Named("predObs")=jk
    );
}
