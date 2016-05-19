clc;
%clear all;
%close all;
%load t.mat;
fs=100;%频率
%N=8064;%样本数
N=[1861 1321 1421 1341 1921 621 1201];
%front=0.08*N/fs;%傅里叶变换频域特征
%rear=0.2*N/fs;
feat1=zeros(70,24);
front=zeros(7,1);
rear=zeros(7,1);
for i=1:7
    dataname=['=scene' num2str(i)];
    eval(['data',dataname]);
    for j=1:10
        gsr_ori=squeeze(data(j,:,4));%  4代表皮电
        gsrdiff1=diff(gsr_ori);%一阶差分
        gsrdiff2=diff(gsrdiff1);%二阶差分
        
       feat1((i-1)*10+j,1)=mean(gsr_ori);
       feat1((i-1)*10+j,2)=std(gsr_ori);
       feat1((i-1)*10+j,3)=max(gsr_ori);
       feat1((i-1)*10+j,4)=min(gsr_ori);
       feat1((i-1)*10+j,5)= feat1((i-1)*10+j,4)-feat1((i-1)*10+j,3);
       feat1((i-1)*10+j,6)=median(gsr_ori);
       
       feat1((i-1)*10+j,7)=mean(gsrdiff1);
       feat1((i-1)*10+j,8)=std(gsrdiff1);
       feat1((i-1)*10+j,9)=max(gsrdiff1);
       feat1((i-1)*10+j,10)=min(gsrdiff1);
       feat1((i-1)*10+j,11)= feat1((i-1)*10+j,10)-feat1((i-1)*10+j,9);
       feat1((i-1)*10+j,12)=median(gsrdiff1);

       feat1((i-1)*10+j,13)=mean(gsrdiff2);
       feat1((i-1)*10+j,14)=std(gsrdiff2);
       feat1((i-1)*10+j,15)=max(gsrdiff2);
       feat1((i-1)*10+j,16)=min(gsrdiff2);
       feat1((i-1)*10+j,17)= feat1((i-1)*10+j,16)-feat1((i-1)*10+j,15);
       feat1((i-1)*10+j,18)=median(gsrdiff2);
        
        gsrfft=fft(gsr_ori,round(N(i)));
        mag=abs(gsrfft);
        front(i)=ceil(0.08*N(i)/fs);%傅里叶变换频域特征
        rear(i)=ceil(0.2*N(i)/fs);
      
       feat1((i-1)*10+j,19)=mean(mag(front(i):rear(i)));
       feat1((i-1)*10+j,20)=std(mag(front(i):rear(i)));
       feat1((i-1)*10+j,21)=max(mag(front(i):rear(i)));
       feat1((i-1)*10+j,22)=min(mag(front(i):rear(i)));
       feat1((i-1)*10+j,23)= feat1((i-1)*10+j,22)-feat1((i-1)*10+j,21);
       feat1((i-1)*10+j,24)=median(mag(front(i):rear(i)));
    end 
end