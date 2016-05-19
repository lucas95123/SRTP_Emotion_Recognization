clc;
clear all;
close all;
load t.mat;
feat=zeros(70,108);%%%%%%%%%%%%%%%%%%%%%%%
for i=1:7
    dataname=['=scene' num2str(i)];
    eval(['data',dataname]);
    for j=1:10
        emg_ori=squeeze(data(j,:,2));
        %emg_ori=squeeze(temp);
        temp2=wpdec(emg_ori,4,'sym8');%小波去噪
        emg=wprcoef(temp2,[4,0]);
        emgdiff1=diff(emg);%一阶差分
        emgdiff2=diff(emgdiff1);%二阶差分
        
       feat((i-1)*10+j,1)=mean(emg);
       feat((i-1)*10+j,2)=std(emg);
       feat((i-1)*10+j,3)=max(emg);
       feat((i-1)*10+j,4)=min(emg);
       feat((i-1)*10+j,5)= feat((i-1)*10+j,4)-feat((i-1)*10+j,3);
       feat((i-1)*10+j,6)=median(emg);
       
       feat((i-1)*10+j,7)=mean(emgdiff1);
       feat((i-1)*10+j,8)=std(emgdiff1);
       feat((i-1)*10+j,9)=max(emgdiff1);
       feat((i-1)*10+j,10)=min(emgdiff1);
       feat((i-1)*10+j,11)= feat((i-1)*10+j,10)-feat((i-1)*10+j,9);
       feat((i-1)*10+j,12)=median(emgdiff1);

       feat((i-1)*10+j,13)=mean(emgdiff2);
       feat((i-1)*10+j,14)=std(emgdiff2);
       feat((i-1)*10+j,15)=max(emgdiff2);
       feat((i-1)*10+j,16)=min(emgdiff2);
       feat((i-1)*10+j,17)= feat((i-1)*10+j,16)-feat((i-1)*10+j,15);
       feat((i-1)*10+j,18)=median(emgdiff2);
       
       [C,L]=wavedec(emg,5,'db5');
        for k=1:5
           temp3=appcoef(C,L,'db5',k);
           temp3diff1=diff(temp3);
           temp3diff2=diff(temp3diff1);
           feat((i-1)*10+j,19+(k-1)*18)=mean(temp3);
           feat((i-1)*10+j,20+(k-1)*18)=std(temp3);
           feat((i-1)*10+j,21+(k-1)*18)=max(temp3);
           feat((i-1)*10+j,22+(k-1)*18)=min(temp3);
           feat((i-1)*10+j,23+(k-1)*18)= feat((i-1)*10+j,22+(k-1)*18)-feat((i-1)*10+j,21+(k-1)*18);
           feat((i-1)*10+j,24+(k-1)*18)=median(temp3);
           
           feat((i-1)*10+j,25+(k-1)*18)=mean(temp3diff1);
           feat((i-1)*10+j,26+(k-1)*18)=std(temp3diff1);
           feat((i-1)*10+j,27+(k-1)*18)=max(temp3diff1);
           feat((i-1)*10+j,28+(k-1)*18)=min(temp3diff1);
           feat((i-1)*10+j,29+(k-1)*18)= feat((i-1)*10+j,28+(k-1)*18)-feat((i-1)*10+j,27+(k-1)*18);
           feat((i-1)*10+j,30+(k-1)*18)=median(temp3diff1);
           
           feat((i-1)*10+j,31+(k-1)*18)=mean(temp3diff2);
           feat((i-1)*10+j,32+(k-1)*18)=std(temp3diff2);
           feat((i-1)*10+j,33+(k-1)*18)=max(temp3diff2);
           feat((i-1)*10+j,34+(k-1)*18)=min(temp3diff2);
           feat((i-1)*10+j,35+(k-1)*18)= feat((i-1)*10+j,34+(k-1)*18)-feat((i-1)*10+j,33+(k-1)*18);
           feat((i-1)*10+j,36+(k-1)*18)=median(temp3diff2);
        end  
    end
end