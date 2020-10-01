function flow=calcFlow(setPoint, fullScale)
% function flow=calcFlow(setPoint)
%
% Convert a desired flow rate into the correct units for the MFC. See p. 22
% of operating manual. 
% (desired set point * 64000)/Full scale range 

%format shortG %Formats the notation such that there's no exp notation

%flow = (setPoint * 64000)/5; %64000/5 for MFC E

flow = (setPoint * 64000)/fullScale;
end
 
 