function [RSA1] = timetest2(RD1T)

IVT = 15;
RSA1 = IVT;
PVT = 10;
RDRT = -1;
N = 100;
%RD1T = (PVT - IVT)/RDRT; %5sec
dt = RD1T / N;

D_T = RDRT*RD1T;
D_dt = RDRT*dt;

tic
for i = 1:N
    RSA1 = IVT + D_dt*i
    pause(dt);
end
toc
end