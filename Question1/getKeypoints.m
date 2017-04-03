function [keyXs, keyYs , corners ,heat_map] = getKeypoints(im,tau)

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
diameter = 2*radius;
corners = heat_map;
heat_map = padarray(heat_map,[radius radius]);
[row, col] = size(corners);
for i=1:row
    for j=1:col
        if(corners(i,j)==max(max(heat_map(i:i+diameter,j:j+diameter)))&& (corners(i,j)>tau))
          
        else
               corners(i,j) =0;
        end
    end
end

[keyYs, keyXs] = find(corners);

imshow(im); hold on
plot(keyXs,keyYs,'g.', 'MarkerSize',5);

end