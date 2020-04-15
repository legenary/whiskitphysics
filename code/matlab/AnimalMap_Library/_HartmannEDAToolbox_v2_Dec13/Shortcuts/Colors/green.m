%  Hartmann EDA Toolbox v1, Dec 2004
function [void]=green(n),
% turns the object green
if nargin==0
	n=1;
end;
h=get(gca,'Children');
h=h(n);
set(h,'color',[0,.8,0]);