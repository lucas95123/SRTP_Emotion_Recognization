clc;
clear all;
close all;
load 'three_feat.mat'
numoffeat=174;
for first=1:6
    for second=first+1:6
        
        train=zeros(30,numoffeat+1);%%%
        test=zeros(20,numoffeat+1);
        count=1;
        count_train=1;
        count_test=1;
        
     
        next_row=ceil(4*rand(1));
        for i=1:70
           if(three_feat(i,numoffeat+1)==first || three_feat(i,numoffeat+1)==second)            
               
               %if(count==1||count==4||count==7||count==10||count==13||count==16||count==19||count==22)
               if(count==next_row)
                 test(count_test,:)=three_feat(i,:);  
                 next_row=count_test*4+ceil(4*rand(1));
                 count_test=count_test+1;
               else
                 train(count_train,:)=three_feat(i,:);              
                 count_train=count_train+1;
               end
               count=count+1;
               
           end
        end
        
        test=test';
        test(:,find(sum(abs(test),1)==0))=[];
        test=test';

        train=train';
        train(:,find(sum(abs(train),1)==0))=[];
        train=train';
        
        save(strcat('train_',num2str(first),'_',num2str(second),'_no_pca.mat'),'train');
        save(strcat('test_',num2str(first),'_',num2str(second),'_no_pca.mat'),'test');
    end
end
