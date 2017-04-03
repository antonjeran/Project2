function [newXs newYs] = predictTranslationAll(startXs, startYs, im0,im1)

points = length(startXs);
radius = 7;
win_size = (2*radius+1)^2;
[rows, cols] =size(im0);

rows = rows-radius;
cols = cols-radius;

newXs=startXs;
newYs=startYs;

%%Find Ix and Iy
dx =[-0.5 0 0.5];
dy =[-0.5; 0 ;0.5];
Ix = imfilter(im0,dx);
Iy = imfilter(im0,dy);

for i=1:1:points
      
    if(floor(newXs(i))>radius && floor(newYs(i))>radius && ceil(newYs(i))<=rows && ceil(newXs(i))<=cols)
       
    range_x =(floor(newXs(i))-radius):(ceil(newXs(i))+radius);
    range_y =(floor(newYs(i))-radius):(ceil(newYs(i))+radius);
    [back_x, back_y] =meshgrid(range_x,range_y);
    [mesh_x, mesh_y] =meshgrid((startXs(i)-radius):(startXs(i)+radius),(startYs(i)-radius):(startYs(i)+radius));
   
    Ix_sub = reshape(interp2(back_x,back_y,Ix(range_y,range_x),mesh_x,mesh_y),[win_size 1]);
    Iy_sub = reshape(interp2(back_x,back_y,Iy(range_y,range_x),mesh_x,mesh_y),[win_size 1]);
    im0_sub = reshape(interp2(back_x,back_y,im0(range_y,range_x),mesh_x,mesh_y),[win_size 1]);
    im1_sub = reshape(interp2(back_x,back_y,im1(range_y,range_x),mesh_x,mesh_y),[win_size 1]);
    
    
    A = [Ix_sub Iy_sub];
    b = -(im1_sub - im0_sub);
    uv = (A'*A)\(A'*b);
    
    newXs(i) = newXs(i)+uv(1);
    newYs(i) = newYs(i) +uv(2);
    val = sum(abs(uv));
    val_old=100;
    while(val<10^-5  && (val_old-val)<0)
        val_old=val;
        if(newXs(i)>radius && newYs(i)>radius && newYs(i)<=rows && newXs(i)<=cols)
           
            range_x =(floor(newXs(i))-radius):(ceil(newXs(i))+radius);
            range_y =(floor(newYs(i))-radius):(ceil(newYs(i))+radius);
            [back_x, back_y] =meshgrid(range_x,range_y);
            [mesh_x, mesh_y] =meshgrid((newXs(i)-radius):(newXs(i)+radius),(newYs(i)-radius):(newYs(i)+radius));
            
            im1_sub = reshape(interp2(back_x,back_y,im1(range_y,range_x),mesh_x,mesh_y),[win_size 1]);
            
            A = [Ix_sub Iy_sub];
            b = -(im1_sub - im0_sub);
            uv = (A'*A)\(A'*b);
            
            newXs(i) = newXs(i)+uv(1);
            newYs(i) = newYs(i) +uv(2);
            val = sum(abs(uv));
          
        else
            newXs(i) =-1;
            newYs(i) =-1;
            val=0;
        end
     
    end
  
    else
    newXs(i) = -1;
    newYs(i) = -1;
    end
   
end

plot_pointX = find(newXs>0);

figure
imshow(im1);hold on;
plot(newXs(plot_pointX),newYs(plot_pointX),'g.', 'MarkerSize',5);

end