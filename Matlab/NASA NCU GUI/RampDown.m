function RampDown(IVT, PVT, RDRT, OxyComp, A, DC, N2Unit, O2Unit )
%Input Order: Initial Velocity Total, Plateau Velocity Total, Ramp Rate,
%Oxygen Composition, Area, Density Correction Factor, Nitrogen Unit ID,
%Oxygen Unit ID
%similar to RampUp.m
%%This is a rudimentary code to ramp a flow at the correct rate. For future advances, consider
%%using the Matlab timer function (can adjust time period, how often, etc.)
RDT = (PVT - IVT)/RDRT; %Ramp Down Time (Total)
N = 50; %Adjust this value to find a good spot to match the ramp time. Too low = longer pauses, Too high = more iterations (too long).
dt = RDT / N; %Fraction of the total time RDT (Ramp Down Time) dictated by N
D_dt = RDRT*dt; %Ramp down in a fraction of time. D_dt = Decrease per dt

tic
for i = 1:N
    RVT = IVT + D_dt*i; %(Ramp Velocity Total decreasing) --> opposite in RampUp.m
    Q_T1 = RVT*A*DC*0.06; %Flow Total
    Q_B1 = OxyComp*Q_T1/100; %Flow A
    Q_A1 = Q_T1 - Q_B1; %Flow B
    
    %Set Flows Here
    setFlow(Q_A1, N2Unit);
    setFlow(Q_B1, O2Unit);
    
    pause(dt);
end
toc
end
