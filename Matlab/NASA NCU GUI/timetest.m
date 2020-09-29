function [RSA1, RSB1] = timetest(RD1T)

QA1 = 15;
QB1 = 15;
RSA1 = 15;
RSB1 = 15;
QPA1 = 10;
QPB1 = 10;
%RD1T = 5;

N = 100;

dt = RD1T/N;
%timerVal = tic;

%elapsedTime = toc(timerVal)
%while elapsedTime <= dt
    %elapsedTime = toc(timerVal)
    %RSA1 = RSA1 - ((RSA1-QPA1)/N);
    %RSB1 = RSB1 - ((RSB1-QPB1)/N);
%end
%RSA1 = [15];

tic
for i = 1:N
    %iteration(i) = i
    timerVal = tic;
    elapsedTime = toc(timerVal);
    while elapsedTime <= dt
        %RSA1(i) = RSA1(i-1) - ((RSA1(i-1)-QPA1)/N)
        RSA1 = RSA1 - ((QA1-QPA1)/N)
        RSB1 = RSB1 - ((QB1-QPB1)/N)
        elapsedTime = toc(timerVal)
        %setFlow(RSA1, N2Unit)
        %setFlow(RSB1, O2Unit)
        if RSA1 == QPA1 && RSB1 == QPB1
            break
        end
    end
    
end
toc
end