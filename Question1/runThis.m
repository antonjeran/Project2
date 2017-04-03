clear all
close all
clc

frame1 = imread('C:/Users/anton/Desktop/Semester7/project2/images/hotel.seq0.png');
frame1 = im2double(frame1);
tau=0.01;

%%Detecting Key Points
[keyXs, keyYs, corners ,heat_map] = getKeypoints(frame1,tau);

%%Tracking Key Points

imagefiles = dir('C:/Users/anton/Desktop/Semester7/project2/images/*.png');
old_frame = frame1;
oldXs =keyXs;
oldYs =keyYs;

track_X =keyXs;
track_Y =keyYs;
for i=1:length(imagefiles)
    image_name = strcat('C:/Users/anton/Desktop/Semester7/project2/images/',imagefiles(i).name);
 
    new_frame= imread(image_name);
    new_frame = im2double(new_frame);
    [newXs newYs] = predictTranslationAll(oldXs,oldYs,old_frame,new_frame);
    
    track_X =[track_X newXs];
    track_Y =[track_Y newYs];
    
    oldXs =newXs;
    oldYs =newYs;
    old_frame = new_frame;
end

track_X = track_X';
track_Y = track_Y';

moved_points = find(min(track_X)==-1);

figure
imshow(frame1)
hold on;
plot(track_X(1,moved_points),track_Y(1,moved_points),'*b', 'MarkerSize',5);

tracked_points =find(min(track_X>0));

figure
imshow(frame1)
hold on;
plot(track_X(:,tracked_points),track_Y(:,tracked_points),'g.','MarkerSize',5);


