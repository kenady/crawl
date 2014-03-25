SUBROUTINE crwdrift_predict( tau2y, tau2x, Qmat, Tmat, x, y, loctype, &
                             stay, ay, ax, Py, Px, lonadj, N, lly, llx, &
                              my, uy, jky, mx, ux, jkx, predy, predx, vary, varx)
 !! LOADED VALUES/ARRAYS !!
  INTEGER           N
  INTEGER           loctype(N), stay(N)
  DOUBLE PRECISION  lly, llx, tau2y(N), tau2x(N), Qmat(N,5), Tmat(N,4)
  DOUBLE PRECISION  x(N), y(N), lonadj(N)
  DOUBLE PRECISION  My(N,1), uy(N,1), jky(N), Mx(N,1), ux(N,1), jkx(N)
  DOUBLE PRECISION  ax(3,1), ay(3,1), Py(3,3), Px(3,3)
  DOUBLE PRECISION  predy(N,3), predx(N,3), vary(3,3,N), varx(3,3,N)

 !! DERIVED ARRAYS !!
  DOUBLE PRECISION vy(N), vx(N), Fy(N), Fx(N)
  DOUBLE PRECISION Z(1,3)
  DOUBLE PRECISION KyArr(3,1,N), KxArr(3,1,N)
  DOUBLE PRECISION Qy(3,3), Qx(3,3), T(3,3)
  DOUBLE PRECISION PyArr(3,3,N+1), PxArr(3,3,N+1), LyArr(3,3,N), LxArr(3,3,N)
  DOUBLE PRECISION ayArr(3,1,N+1), axArr(3,1,N+1)
  DOUBLE PRECISION ry(3,1), Ny(3,3), rx(3,1), Nx(3,3)

 !! INTIAL CONDITIONS !!
  ayArr(:,:,1) = ay
  axArr(:,:,1) = ax
  PyArr(:,:,1) = Py
  PxArr(:,:,1) = Px
  Z            = RESHAPE((/1.0, 0.0, 0.0/),(/1, 3/))
  Qy           = 0.0
  Qx           = 0.0
  T            = 0.0
  T(1,1)       = 1.0
  vy           = 0.0
  vx           = 0.0
  Fy           = tau2y
  Fx           = tau2x
  ry           = 0.0
  rx           = 0.0
  Ny           = 0.0
  Nx           = 0.0

 !! BEGIN FILTER LOOP !!
  DO i=1,N

   !! GENERATE Q AND T MATRICES !!
    IF(stay(i)==1) THEN
      Qy = 0.0
      Qx = 0.0
      T(1,2) = 0.0
      T(2,2) = 0.0
      T(1,3) = 0.0
      T(3,3) = 0.0
    ELSE
      Qy(1,1) = Qmat(i,1)
      Qy(2,1) = Qmat(i,2)
      Qy(3,1) = Qmat(i,3)
      Qy(1,2) = Qy(2,1)
      Qy(2,2) = Qmat(i,4)
      Qy(1,3) = Qy(3,1)
      Qy(3,3) = Qmat(i,5)
      Qx = Qy/(lonadj(i)*lonadj(i))
      T(1,2) = Tmat(i,1)
      T(2,2) = Tmat(i,2)
      T(1,3) = Tmat(i,3)
      T(3,3) = Tmat(i,4)
    END IF

   !! GENERAL KF FILTER !!
    Fy(i) = PyArr(1,1,i) + tau2y(i)
    Fx(i) = PxArr(1,1,i) + tau2x(i)/(lonadj(i)*lonadj(i))
    IF(loctype(i)==1 .OR. Fy(i)==0.0) THEN
      ayArr(:,:,i+1) = MATMUL(T,ayArr(:,:,i))
      PyArr(:,:,i+1) = MATMUL(MATMUL(T,PyArr(:,:,i)),TRANSPOSE(T)) + Qy
      KyArr(:,:,i) = 0.0
      LyArr(:,:,i) = T
    ELSE
      vy(i) = y(i)-ayArr(1,1,i)
      lly = lly - (log(Fy(i)) + vy(i)*vy(i)/Fy(i))/2
      KyArr(:,:,i) = MATMUL(MATMUL(T,PyArr(:,:,i)),TRANSPOSE(Z))/Fy(i)
      LyArr(:,:,i) = T - MATMUL(KyArr(:,:,i),Z)
      ayArr(:,:,i+1) = MATMUL(T,ayArr(:,:,i)) + KyArr(:,:,i)*vy(i)
      PyArr(:,:,i+1) = MATMUL(MATMUL(T,PyArr(:,:,i)),TRANSPOSE(LyArr(:,:,i))) + Qy
    END IF
    IF(loctype(i)==1 .OR. Fx(i)==0.0) THEN          
      axArr(:,:,i+1) = MATMUL(T,axArr(:,:,i))      
      PxArr(:,:,i+1) = MATMUL(MATMUL(T,PxArr(:,:,i)),TRANSPOSE(T)) + Qx
      KxArr(:,:,i) = 0.0
      LxArr(:,:,i) = T
    ELSE
      vx(i) = x(i)-axArr(1,1,i)
      llx = llx - (log(Fx(i)) + vx(i)*vx(i)/Fx(i))/2     
      KxArr(:,:,i) = MATMUL(MATMUL(T,PxArr(:,:,i)),TRANSPOSE(Z))/Fx(i)      
      LxArr(:,:,i) = T - MATMUL(KxArr(:,:,i),Z)
      axArr(:,:,i+1) = MATMUL(T,axArr(:,:,i)) + KxArr(:,:,i)*vx(i)      
      PxArr(:,:,i+1) = MATMUL(MATMUL(T,PxArr(:,:,i)),TRANSPOSE(LxArr(:,:,i))) + Qx
    END IF
  END DO

 !! BEGIN SMOOTHING LOOP!!
  DO j=N,1,-1
    IF(loctype(j)==1 .OR. Fy(j)==0.0) THEN
      ry = MATMUL(TRANSPOSE(LyArr(:,:,j)),ry)
      Ny = MATMUL(MATMUL(TRANSPOSE(LyArr(:,:,j)), Ny), LyArr(:,:,j))
    ELSE
      uy(j,:) = vy(j)/Fy(j) - RESHAPE(MATMUL(TRANSPOSE(KyArr(:,:,j)), ry), (/1/))
      My(j,:) = (1/Fy(j)) + RESHAPE(MATMUL(MATMUL(TRANSPOSE(KyArr(:,:,j)), Ny), KyArr(:,:,j)),(/1/))
      jky(j) = y(j) - uy(j,1)/My(j,1)
      ry = TRANSPOSE(Z)*vy(j)/Fy(j) + MATMUL(TRANSPOSE(LyArr(:,:,j)),ry)
      Ny = MATMUL(TRANSPOSE(Z),Z)/Fy(j) + MATMUL(MATMUL(TRANSPOSE(LyArr(:,:,j)), Ny), LyArr(:,:,j))      
    END IF
    
    IF(loctype(j)==1 .OR. Fx(j)==0.0) THEN
      rx = MATMUL(TRANSPOSE(LxArr(:,:,j)),rx)
      Nx = MATMUL(MATMUL(TRANSPOSE(LxArr(:,:,j)), Nx), LxArr(:,:,j))
    ELSE      
      ux(j,:) = vx(j)/Fx(j) - RESHAPE(MATMUL(TRANSPOSE(KxArr(:,:,j)), rx), (/1/))
      Mx(j,:) = (1/Fx(j)) + RESHAPE(MATMUL(MATMUL(TRANSPOSE(KxArr(:,:,j)), Nx), KxArr(:,:,j)), (/1/))
      jkx(j) = x(j) - ux(j,1)/Mx(j,1)
      rx = TRANSPOSE(Z)*vx(j)/Fx(j) + MATMUL(TRANSPOSE(LxArr(:,:,j)),rx)      
      Nx = MATMUL(TRANSPOSE(Z),Z)/Fx(j) + MATMUL(MATMUL(TRANSPOSE(LxArr(:,:,j)), Nx), LxArr(:,:,j))
    END IF
    predy(j,:) = RESHAPE(ayArr(:,:,j) + MATMUL(PyArr(:,:,j), ry), (/3/))
    predx(j,:) = RESHAPE(axArr(:,:,j) + MATMUL(PxArr(:,:,j), rx), (/3/))
    vary(:,:,j) = PyArr(:,:,j) - MATMUL(MATMUL(PyArr(:,:,j),Ny), PyArr(:,:,j))
    varx(:,:,j) = PxArr(:,:,j) - MATMUL(MATMUL(PxArr(:,:,j),Nx), PxArr(:,:,j))
  END DO
END SUBROUTINE crwdrift_predict