function [A,st] = setGas(MFC,gas)
%This Function sets a MFC's gas based on a string input, and returns a
%confirmation string


gasses = ["Air","Ar","CH4","CO","CO2","C2H6","H2","He","N2","N2O","Ne","O2",
    "C3H8","n-C4H10","C2H2","C2H4","i-C2H10","Kr","Xe","SF6","C-25","C-10","C-8",
    "C-2","C-75","A-75","A-25","A1025","Star29","p-5"];

global aliComm;
if isempty(aliComm), aliComm=connectAlicat; end

%Sets multiple gasses at the same time if both inputs are arrays, otherwise
%sets one 
if (ismatrix(MFC)) && (ismatrix(gas))
    if length(MFC) ~= length(gas), error('MFC and gas arrays must be equal!'); end
    
    for i = 1:length(MFC)
        j = find(gasses==gas(i));
        fprintf(aliComm, sprintf('%s%%%%%s',MFC(i),j));
    end
    
else
    i = find(gasses==gas);
    fprintf(aliComm, sprintf('%s%%%%%s',MFC,i));
    
end

A=readMFC(aliComm);
st = sprintf('%s%g',MFC,gas);

end