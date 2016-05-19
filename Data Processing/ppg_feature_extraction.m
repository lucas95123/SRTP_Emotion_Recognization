clc;
%clear all;
%close all;
%load t.mat;
feat2=zeros(70,42);%%%%%%%%%%%%%%%%%%%%%%%要修改
for i=1:7
    dataname=['=scene' num2str(i)];
    eval(['data',dataname]);
    for j=1:10
        mai_ori=squeeze(data(j,:,1));
        temp=wpdec(mai_ori,3,'db3');%小波去噪
        mai=wprcoef(temp,[3,0]);%小波去噪
        
        [C,L]=wavedec(mai,3,'db3');%小波
        temp2=detcoef(C,L,3);%主波
        [pks,locs] = findpeaks(temp2,'minpeakdistance',6);%波峰,间隔大于6
        pks_diff=diff(pks);%zhubo yijie chafen
        locs_diff=diff(locs);%主波波峰间隔
        
        feat2((i-1)*10+j,1)=mean(mai);
        feat2((i-1)*10+j,2)=median(mai);
        feat2((i-1)*10+j,3)=std(mai);
        feat2((i-1)*10+j,4)=min(mai);
        feat2((i-1)*10+j,5)=max(mai);
        feat2((i-1)*10+j,6)=max(mai)-min(mai);
        
        feat2((i-1)*10+j,7)=mean(locs_diff);
        feat2((i-1)*10+j,8)=median(locs_diff);
        feat2((i-1)*10+j,9)=std(locs_diff);
        feat2((i-1)*10+j,10)=min(locs_diff);
        feat2((i-1)*10+j,11)=max(locs_diff);
        feat2((i-1)*10+j,12)=max(locs_diff)-min(locs_diff);
        
        feat2((i-1)*10+j,13)=mean(pks);
        feat2((i-1)*10+j,14)=median(pks);
        feat2((i-1)*10+j,15)=std(pks);
        feat2((i-1)*10+j,16)=min(pks);
        feat2((i-1)*10+j,17)=max(pks);
        feat2((i-1)*10+j,18)=max(pks)-min(pks);
        
        feat2((i-1)*10+j,19)=mean(pks_diff);
        feat2((i-1)*10+j,20)=median(pks_diff);
        feat2((i-1)*10+j,21)=std(pks_diff);
        feat2((i-1)*10+j,22)=min(pks_diff);
        feat2((i-1)*10+j,23)=max(pks_diff);
        feat2((i-1)*10+j,24)=max(pks_diff)-min(pks_diff);
        
         for k=1:3
           temp3=appcoef(C,L,'db3',k);
            feat2((i-1)*10+j,25+(k-1)*6)=mean(temp3);
            feat2((i-1)*10+j,26+(k-1)*6)=median(temp3);
            feat2((i-1)*10+j,27+(k-1)*6)=std(temp3);
            feat2((i-1)*10+j,28+(k-1)*6)=min(temp3);
            feat2((i-1)*10+j,29+(k-1)*6)=max(temp3);
            feat2((i-1)*10+j,30+(k-1)*6)=max(temp3)-min(temp3);
         end
    end
end