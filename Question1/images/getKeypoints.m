function [keyXs, keyYs] = getKeypoints(im,tau)

keyXs=1;
keyYs=1;
%%Blur the image
sigma =1;
gaussian = fspecial('gaussian',ceil(6*sigma),sigma);

im = imfilter(im,gaussian);

%%Find Ix and Iy with smoothing
dx =[-1 0 1;-1 0 1;-1 0 1];
dy =[-1 -1 -1;0 0 0;1 1 1];

Ix = imfilter(im,dx);
Iy = imfilter(im,dy);

%%finding gaussian filtered Ixx Iyy and Ixy
Ixx = imfilter(Ix.*Ix,gaussian);
Iyy = imfilter(Iy.*Iy,gaussian);
Ixy = imfilter(Ix.*Iy,gaussian);
Iyx =Ixy;

%%Shi Tomasi Function

b =(Ixx+Iyy)/2;
c = sqrt((Ixx.^2)-(2*Ixx.*Iyy)+(Iyy.^2)+(4.*Ixy.*Iyx))./2;

lambda1 = b+c;
lambda2 = b-c;

heat_map = min(lambda1,lambda2);

%%Non Maximum Suppression

radius =3;
diameter =2*radius +1;
max_map = ordfilt2(heat_map,(diameter^2),ones(diameter));
corners =(heat_map==max_map).*(max_map>tau);

[keyYs, keyXs] = find(corners);

imshow(im); hold on
plot(keyXs,keyYs,'g.', 'MarkerSize',7);

end