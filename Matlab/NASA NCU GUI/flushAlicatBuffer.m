function flushAlicatBuffer

global aliComm;
if isempty(aliComm), aliComm=connectAlicat; end

%fprintf('Flushing Alicat serial buffer')
while aliComm.BytesAvailable>0
    fread(aliComm,aliComm.BytesAvailable);
    %fprintf('.')
    %pause(0.05)
end
fprintf('\n')