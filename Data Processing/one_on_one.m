clc;
clear all;
close all;
load 'three_feat.mat'
numoffeat=174;
for first=1:6
    for second=first+1:6
        count=1;    
        data2v2=zeros(30,numoffeat+1);
        data2v2_without_label=zeros(30,numoffeat);

        for i=1:70
           if(three_feat(i,numoffeat+1)==first || three_feat(i,numoffeat+1)==second)
                data2v2(count,:)=three_feat(i,:);
                data2v2_without_label(count,:)=three_feat(i,1:numoffeat);
                count=count+1;
           end
        end
        
        data2v2=data2v2';
        data2v2(:,find(sum(abs(data2v2),1)==0))=[];
        data2v2=data2v2';
        
        data2v2_without_label=data2v2_without_label';
        data2v2_without_label(:,find(sum(abs(data2v2_without_label),1)==0))=[];
        data2v2_without_label=data2v2_without_label';
        
        [PC,SCORE,latent,tsquare]=princomp(data2v2_without_label);

        SCORE(:,find(sum(abs(SCORE),1)==0))=[];

        num_features=size(SCORE,2);
        %num_test=round((count-1)*0.25);
        
        SCORE(:,num_features+1)=data2v2(:,numoffeat+1);

        train=zeros(30,num_features+1);
        test=zeros(20,num_features+1);

        count_test=1;
        count_train=1;
        next_row=ceil(4*rand(1));
        for i=1:count-1
           %if(i==1||i==4||i==7||i==10||i==13||i==16||i==19||i==22)
           if(i==next_row)
             test(count_test,:)=SCORE(i,:);
             next_row=count_test*4+ceil(4*rand(1));
             count_test=count_test+1;
           else
             train(count_train,:)=SCORE(i,:); 
             count_train=count_train+1;
           end
        end

        test=test';
        test(:,find(sum(abs(test),1)==0))=[];
        test=test';

        train=train';
        train(:,find(sum(abs(train),1)==0))=[];
        train=train';

        save(strcat('train_',num2str(first),'_',num2str(second),'_pca.mat'),'train');
        save(strcat('test_',num2str(first),'_',num2str(second),'_pca.mat'),'test');
    end
end