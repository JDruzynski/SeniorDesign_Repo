function RampUp(IVT, PVT, RURT, OxyComp, A, DC, N2Unit, O2Unit )
%Input Order: Initial Velocity Total, Plateau Velocity Total, Ramp Rate,
%Oxygen Composition, Area, Density Correction Factor, Nitrogen Unit ID,
%Oxygen Unit ID
%similar to RampDown.m
%%This is a rudimentary code to ramp a flow at the correct rate. For future advances, consider
%%using the Matlab timer function (can adjust time period, how often, etc.)
RUT = (PVT - IVT)/RURT; %Ramp Up Time (Total)
N = 50; %Adjust this value to find a good spot to match the ramp time. Too low = longer pauses, Too high = more iterations (too long).
dt = RUT / N; %Fraction of the total time RUT (Ramp Up Time) dictated by N
I_dt = RURT*dt; %Ramp up in a fraction of time. I_dt = Increase per dt

tic
for i = 1:N
    RVT = IVT + I_dt*i; %(Ramp Velocity Total increasing) --> opposite in RampDown.m
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
