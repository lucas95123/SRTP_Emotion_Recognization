# SRTP_Emotion_Recognization
SRTP_Project_Emotion_Recognition
# Introduction
This is an emotion recognization project conducted by __Kuan Lu__, __Shilun Ding__, and __Lihui Qian__, all of whom are undergraduate students of Zhejiang University. The project conducted an experiment on 10 participants and record their EDA, PPG, zEMG signal in response to a collection of short film that is intended to elicit emotion from the participant. With the data we used serveral algorithms to achieve the best true prediction rate
# Usage
The five folders in the directory are Arduino Device, Data, Data Processing, Feature selection, Feature Classification.
- In the Arduino Device Folder are codes for an arduino device that collects GSR, PPG, EMG signals
- In the Data Folder are Data retrieved from the 10 participant including RAW data and processed data, The Data specifications can be found in later updates. All the participant agree us to publish their data on github for other emotion related tests of other people
- The data processing Folder contained the matlab code for __Filtering__ , __Featrure extracting__
- The Feature selection Folder contains genetic algorithm and PCA implemented in python
- The Feature classification Folder contains SVM(C++ by libsvm) KNN(in C++ and Python) Decision Tree(C++) Random Forest(C++)

# Special thanks
- scikit-learn python machine learning library
- libsvm
