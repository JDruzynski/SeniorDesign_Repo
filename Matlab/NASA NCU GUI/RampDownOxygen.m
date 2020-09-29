function RampDownOxygen(Q_T1, O2_percent_plateau, O2_percent_RDR, OxyComp, N2Unit, O2Unit )
%Input Order: Initial Velocity Total, Plateau Velocity Total, Ramp Rate,
%Oxygen Composition, Area, Density Correction Factor, Nitrogen Unit ID,
%Oxygen Unit ID
%similar to RampUp.m
%%This is a rudimentary code to ramp a flow at the correct rate. For future advances, consider
%%using the Matlab timer function (can adjust time period, how often, etc.)

duration = (O2_percent_plateau - OxyComp)/O2_percent_RDR; %Ramp Down Time (Total)
N = 50; %Adjust this value to find a good spot to match the ramp time. Too low = longer pauses, Too high = more iterations (too long).
dt = duration / N; %Fraction of the total time RDT (Ramp Down Time) dictated by N
D_dt = O2_percent_RDR*dt; %Ramp down in a fraction of time. D_dt = Decrease per dt

tic
for i = 1:N
    Q_B1 = (OxyComp + D_dt*i)*Q_T1/100; %Flow A
    Q_A1 = Q_T1 - Q_B1; %comment this line and setFlow for QA1 below, if you dont want Flow A to increase when Flow B decreases, and vice versa
    
    %Set Flows Here
    setFlow(Q_A1, N2Unit);
    setFlow(Q_B1, O2Unit);
    
    pause(dt);
end
toc
end