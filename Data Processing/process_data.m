close all
trail=(10:80000:4); %mat data
for i =1:3
FilenameTrail = strcat('test_0',num2str(i),'_trail.mat'); %get the trail filename
load(FilenameTrail);
    for j=2:5
        tmp = downsample(data(:,j),10);
        if j==5
            hd = design(fdesign.bandpass('N,F3dB1,F3dB2',2,1e-9,0.3,100),'butter');
            tmp=filter(hd,tmp);
        end;
       	for k=1:size(tmp,1);
            trail(i,k,j-1)=tmp(k);
        end;
    end;
end
for i =4:10
if i<10
    FilenameTrail = strcat('test_0',num2str(i),'_trail.mat'); %get the trail filename
else
    FilenameTrail = strcat('test_',num2str(i),'_trail.mat'); %get the trail filename
end
load(FilenameTrail);
    for j=1:4
        tmp = downsample(data(:,j),10);
        if j==4
            hd = design(fdesign.bandpass('N,F3dB1,F3dB2',2,1e-9,0.3,100),'butter');
            tmp=filter(hd,tmp);
        end;
       	for k=1:size(tmp,1);
            trail(i,k,j)=tmp(k);
        end;
    end;
end
save('DATA_DOWNSAMPLED_NORMALIZED.mat','trail');

%The following part cut the signal into parts 
%The following part cut the signal into parts 
scene0=zeros(10,301,4);%baseline
scene1=zeros(10,1861,4);
scene2=zeros(10,1321,4);
scene3=zeros(10,1421,4);
scene4=zeros(10,1341,4);
scene5=zeros(10,1921,4);
scene6=zeros(10,621,4);
scene7=zeros(10,1201,4);

for t=1:10
    for c=1:4
    %ave=mean(data);
    scene0(t,:,c) = trail(t,700:1000,c);%baseline
    avg=mean(trail(t,700:1000,c));
    
    scene1(t,:,c) = trail(t,1140:3000,c)-avg;
    scene2(t,:,c) = trail(t,3940:5260,c)-avg;
    scene3(t,:,c) = trail(t,6060:7480,c)-avg;
    scene4(t,:,c) = trail(t,8340:9680,c)-avg;
    scene5(t,:,c) = trail(t,10480:12400,c)-avg;
    scene6(t,:,c) = trail(t,13120:13740,c)-avg;
    scene7(t,:,c) = trail(t,14560:15760,c)-avg;
    end
end
save('DATA_DOWNSAMPLED_NORMALIZED_TRUNCATED.mat','scene0', 'scene1','scene2','scene3','scene4','scene5','scene6','scene7');
%clear all;
%close all;