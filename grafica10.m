LIMITE_X=11000;
TAM_X=LIMITE_X+1;
x = 0:LIMITE_X;
#plot (x, sin (x));
m=[500,400,300,299,298,297,200,100,50,0];
b=[ 0, 500000, 1000000, 1500000, 2000000, 2000500, 2001000];
m
b
y=zeros(11);
for i=1:7
	y(i,1:TAM_X)=m(i)*x+b(i);
	if i>=2 && i<=4
#		continue;
		p1=plot(x,y(i,1:TAM_X));
         	legend(p1,int2str(i));
	 	hold all;
	end
end
grid
figure
#y

