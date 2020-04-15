function xprime = brotate(x,theta,origin)% function xprime = brotate(x,theta,[origin])% x is an n x 2 matrix of (x,y) coordinates.  % returns xprime = x * [cos(theta) -sin(theta); sin(theta) cos(theta)].% 11-12-90, by Brian Rasnow% made (0,0) the default origin and checked for transposed matrix -- MJH% Hartmann EDA Toolbox v1, Dec 2004[a,b]=size(x);if b~=2    x=x';end;if nargin<3    origin=[0,0];elseif nargin <2    disp('Syntax: xprime = rotate(x,theta,[origin])');end;x(:,1)=x(:,1)-origin(1);x(:,2)=x(:,2)-origin(2);r = [cos(theta) -sin(theta); sin(theta) cos(theta)];xprime = x * r;xprime(:,1)=xprime(:,1)+origin(1);xprime(:,2)=xprime(:,2)+origin(2);