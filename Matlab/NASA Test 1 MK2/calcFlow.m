function flow=calcFlow(setPoint, UnitID)
% function flow=calcFlow(setPoint)
%
% Convert a desired flow rate into the correct units for the MFC. See p. 22
% of operating manual. 
% (desired set point * 64000)/Full scale range 

%format shortG %Formats the notation such that there's no exp notation

%flow = (setPoint * 64000)/5; %64000/5 for MFC E

if UnitID == 'A'|| UnitID == 'B'
    flow = (setPoint * 64000)/50; %ISA50 and ISB50 %64000/50 for MFC A and B
elseif UnitID == 'C'||UnitID == 'D'
    flow = (setPoint * 64000)/1; %ISA1 and ISAB1 %64000/1 for MFC C and D
else %if UnitID is E
    flow = (setPoint * 64000)/5; %64000/5 for MFC E
end
 
 