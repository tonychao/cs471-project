clc
clear all 
close all

N=10; % Number of Bats

Function_name='F1'; % Name of the test function that can be from F1 to F23 (Table 1,2,3 in the paper)

Max_iter=200; % Maximum number of iterations

% Load details of the selected benchmark function
[lb,ub,dim,fobj]=Get_Functions_details(Function_name);
[bestfit,BestPositions,fmax,BAT_Cg_curve]=newBAT(N,Max_iter,lb,ub,dim,fobj);
figure('position',[500 500 660 290])
%Draw search space
subplot(1,2,1);
func_plot(Function_name);
title('Parameter space')
xlabel('x_1');
ylabel('x_2');
zlabel([Function_name,'( x_1 , x_2 )'])

%Draw objective space
subplot(1,2,2);
semilogy(BAT_Cg_curve,'Color','r')
title('Objective space')
xlabel('iteration');
ylabel('Best fitness obtained so far');
axis tight
grid on
box on
legend('newBAT')

display(['The best solution obtained by BAT is : ', num2str(BestPositions)]);
display(['The best optimal value of the objective funciton found by BAT is : ', num2str(bestfit)]);


