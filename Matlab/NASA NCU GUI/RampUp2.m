function RampUp2(IVT, PVT, RURT, OxyComp, A, DC, N2Unit, O2Unit, N2UnitP, O2UnitP)
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

RURT0 = PVT/RUT; %Ramp up rate from 0 to plateau velocity
I0_dt = RURT0*dt; %Ramp up from 0 in fraction of time

RDRT = IVT/RUT; %Ramp down rate total
D_dt = RDRT*dt; %Ramp down in a fraction of time

tic
for i = 1:N
    RVT = IVT + I_dt*i; %(Ramp Velocity Total increasing) --> opposite in RampDown.m
    Q_T1 = RVT*A*DC*0.06; %Flow Total

    if N2Unit == N2UnitP && O2Unit ~= O2UnitP
        Q_A1 = Q_T1 - (OxyComp*Q_T1/100); %Flow A
        Q_B1 = (IVT - D_dt*i)*A*DC*0.06*OxyComp/100;
        Q_B2 = I0_dt*i*A*DC*0.06*OxyComp/100;
        
        setFlow(Q_A1, N2Unit);
        setFlow(Q_B1, O2Unit);
        setFlow(Q_B2, O2UnitP);
        
    elseif N2Unit ~= N2UnitP && O2Unit == O2UnitP
        Q_A1 = (IVT - D_dt*i)*A*DC*0.06*(100-OxyComp)/100;
        Q_B1 = OxyComp*Q_T1/100; %Flow A
        Q_A2 = I0_dt*i*A*DC*0.06*(100-OxyComp)/100;

        setFlow(Q_A1, N2Unit);
        setFlow(Q_B1, O2Unit);
        setFlow(Q_A2, N2UnitP)
        
    elseif N2Unit ~= N2UnitP && O2Unit ~= O2UnitP
        Q_A1 = (IVT - D_dt*i)*A*DC*0.06*(100-OxyComp)/100;
        Q_B1 = (IVT - D_dt*i)*A*DC*0.06*OxyComp/100;
        Q_A2 = I0_dt*i*A*DC*0.06*(100-OxyComp)/100;
        Q_B2 = I0_dt*i*A*DC*0.06*OxyComp/100;
        
        setFlow(Q_A1, N2Unit);
        setFlow(Q_B1, O2Unit);
        setFlow(Q_A2, N2UnitP);
        setFlow(Q_B2, O2UnitP);

    end
    pause(dt);
end
toc
end
