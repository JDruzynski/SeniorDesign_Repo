%%%%This is a file to help structure the MATLAB GUI
%%
%Initializing some variables
%gChPress; gRefPress; ChTempK; gRefTemp;
%gOxyComp; gIVT; gGapWidth; gGapHeight;
%gPVT1; gRDRT1; PVA1; IVA; PVB1; IVB; gPVT1; gIVT
%gPVT2; gRDRT2; gRURT; gFVT;
%gO2_percent_plateau; gO2_percent_RDR;
%DC; A; Dryvel; 
%%
%Reference Temperature in Kelvin
function [RefTempK] = ReferenceTemperatureK (gRefTemp)
RefTempK=gRefTemp+273.15;
end
%%
%Density Correction Factor
function [DC] = DensityCorrection (gChPress,gRefPress, ChTempK, RefTempK)
DC =(gChPress/gRefPress)*(ChTempK/RefTempK);
end
%%
%Drying Procedure
function [Dry2MFC, A] = DryingProcedure (gGapWidth, gGapHeight)
A = gGapWidth*gGapHeight;
DryVel = 5;
QDry = DryVel*A*DC*.06;
Dry2MFC = QDry*64000/5;
end
%%
%Initial Velocities
function [QT1, QB1, QA1, IVA, IVB, ISA50, ISA1, ISB50, ISB1] = InitialVelocities(gIVT, A, DC, gOxyComp)
QT1 = gIVT*A*DC*.06; %Must call the function that outputs A or outside
QB1 = gOxyComp*QT1/100;
QA1 =(QT1-QB1);
IVA = QA1/(A*.06);
IVB = QB1/(A*.06);
ISA50 = QA1*64000/50;
ISA1 = QA1*64000/1;
ISB50 = QB1*64000/50;
ISB1 = QB1*64000/1;
end
%%
%Ramp Down Flow Rate Total 1 
function [QRT1] = RampDownFlowRate1 (gRDRT1)
QRT1 = gRDRT1*A*DC*6/100;
end
%%
%First Plateau Velocities, and Ramp Down Rate
function [QPT1, PVB1, QPB1, PVA1, QPA1, RDRA1, RDRB1, QRA1, QRB1] = Plateau1(gPVT1, gRDRT1, gIVT, gOxyComp)
QPT1 = gPVT1*A*DC*6/100;
PVB1 = gPVT1*gOxyComp/100;
QPB1 = PVB1*A*DC*6/100;
PVA1 = gPVT1-PVB1;
QPA1 = PVA1*A*DC*6/100;
RDRA1 = gRDRT1*((PVA1-IVA)/(gPVT1-gIVT));
RDRB1 = gRDRT1*((PVB1-IVB)/(gPVT1-gIVT));
QRA1 = RDRA1*A*DC*6/100;
QRB1 = RDRB1*A*DC*6/100;

end
%%
%First Phase Duration
function [RD1T] = PhaseDuration1(gPVT1, gIVT, gRDRT1)
RD1T = (gPVT1-gIVT)/(gRDRT1);
end
%%
%O2_N2_Flow
%--From Thao--
function [O2_flow_plateau, O2_Q_ramp_rate, duration] = O2_N2_Flow(gIVT, gO2_percent_plateau, gO2_percent_ramp_down_rate, gOxyComp)
O2_vel_plateau = gIVT*gO2_percent_plateu/100;
O2_flow_plateau = QT1*gO2_percent_plateu/100;

N2_vel_plateau = gIVT-O2_vel_plateau;
N2_flow_plateau = QT1-O2_flow_plateau;

O2_Q_ramp_rate = QT1*gO2_percent_ramp_down_rate/100;
N2_Q_ramp_rate = -1*O2_Q_ramp_rate;

duration = (gO2_percent_plateau-gOxyComp)/gO2_percent_ramp_down_rate;
end
%%
%Ramp Down Rate Total2
function [QRT2] = RampDownFlowRate2(gRDRT2)
QRT2 = gRDRT2*A*DC*6/100;
end
%%
%Second Plateau Velocities, and Ramp Down Rate
function [QPT2, PVB2, QPB2, PVA2, QPA2, RDRA2, RDRB2, QRA2, QRB2] = Plateau2(gPVT2, gRDRT2, gPVT1, gOxyComp)
QPT2 = gPVT2*A*DC*6/100;
PVB2 = gPVT2*gOxyComp/100;
QPB2 = PVB2*A*DC*6/100;
PVA2 = gPVT2-PVB2;
QPA2 = PVA2*A*DC*6/100;
RDRA2 = gRDRT2*((PVA2-PVA1)/(gPVT2-gPVT1));
RDRB2 = gRDRT2*((PVB2-PVB1)/(gPVT2-gPVT1));
QRA2 = RDRA2*A*DC*6/100;
QRB2 = RDRB2*A*DC*6/100;
end
%%
%Second Phase Duration
function [RD2T] = PhaseDuration2(gPVT2, gPVT1, gRDRT2)
RD2T =(gPVT2-gPVT1)/(gRDRT2);
end
%%
%Ramp Up Rates
function [QRT3] = RampUpFlowRate(gRURT)
QRT3 = gRURT*A*DC*6/100;
end
%%
%Ramp Up Velocities, Ramp Up Flow Rates
function [QFT, PFB, QFB, PFA, QFA, RURA, RURB, QRA3, QRB3] = Plateau3(gFVT, gRURT, gPVT2, gOxyComp)
QFT = gFVT*A*DC*6/100;
PFB = gFVT*gOxyComp/100;
QFB = PFB*A*DC*6/100;
PFA = gFVT-PFB;
QFA = PFA*A*DC*6/100;
RURA = gRURT*((PFA-PVA2)/(gFVT-gPVT2));
RURB = gRURT*((PFB-PVA2)/(gFVT-gPVT2));
QRA3 = RURA*A*DC*6/100;
QRB3 = RURB*A*DC*6/100;

end
%%
%Ramp Up Phase Duration
function [RUT] = PhaseDuration3(gFVT, gPVT2, gRURT)
RUT = (gFVT-gPVT2)/(gRURT);
end
%%
%open(NarrowChannelApp.mlapp)
%matlab.apputil.run('NarrowChannelApp')
